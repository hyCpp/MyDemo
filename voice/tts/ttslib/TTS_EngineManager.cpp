/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include <dlfcn.h>
#include <BL_File.h>
#include <BL_SystemLanguage.h>
#include "stdafx.h"
#include "TTS_EngineManager.h"
#include <algorithm>

using namespace std;

static const XCHAR* const TTS_DATA_PATH = XTEXT("TTS");

TTS_EngineManager::TTS_EngineManager()
#ifdef HAVE_NUTSHELL_OS
    : m_defaultEngine(NULL), m_loadedEngine(false),
      m_sysSettingListner("TTS_SYSSETTING_LISTNER", this)
#else
    : m_defaultEngine(NULL), m_loadedEngine(false)
#endif
{
}

TTS_EngineManager::~TTS_EngineManager()
{
    TTS_LOGD_FUNC();
    // clear();

    if (NULL != m_defaultEngine) {
        delete m_defaultEngine;
        m_defaultEngine = NULL;
    }
}

bool TTS_EngineManager::Initialize()
{
    TTS_LOGD_FUNC();
    
    if (!m_configIF.init()) {
        TTS_ERROR_MSG(VOICE_TTS_ERR_CONFIG_FILE, 0, "TTS config file init failed!");
        return false;
    }

    RegisterSysEvent();

    return true; 
    // return LoadEngine();
}

void TTS_EngineManager::RegisterSysEvent()
{
#ifdef HAVE_NUTSHELL_OS
    if (m_configIF.IsLoadEngineBySystem()) {
        nutshell::NCUri urisys("content://settings/system?language");
        nutshell::NCUri urivr("content://settings/system?vr_language");

        VoiceList<nutshell::NCUri>::type uriList;
        uriList.push_back(urisys);
        uriList.push_back(urivr);
        m_sysSettingListner.listenUri(uriList);
    }
#endif
}

bool TTS_EngineManager::LoadEngine()
{
    TTS_EngineInfo ttsEngine;
    if (!m_configIF.getDefaultEngine(ttsEngine)) {
        TTS_LOGE("No default Engine");
        return false;
    }

    std::string dataPath;
    if (!GetAbsoluteDataPath(TTS_DATA_PATH, dataPath)) {
        return false;
    }

    void* handler = dlopen(ttsEngine.path.c_str(), RTLD_LAZY);
    if (handler == NULL) {
        char* error = dlerror();

        if (nullptr != error) {
            TTS_ERROR_MSG(VOICE_TTS_ERR_UNSUPPORTED_ENGINE, 0, "dlopen path: %s, err = %s", ttsEngine.path.c_str(), error);
        }
        else {
            TTS_ERROR_MSG(VOICE_TTS_ERR_UNSUPPORTED_ENGINE, 0, "dlopen path: %s, err = null", ttsEngine.path.c_str());
        }
        return false;
    }
        
    GetInstance_t symbolGetInstance = (GetInstance_t)(dlsym(handler, "GetInstance"));
    if (symbolGetInstance == NULL) {
        TTS_LOGE("dlsym GetInstance fail, error:%s", dlerror());
        return false;
    }

    TTS_Engine *engineInst = symbolGetInstance();
    if (engineInst == NULL) {
        TTS_LOGE("create tts engine fail");
        return false;
    }
    
    TTS_LOGI("engineInst:%p", engineInst);

    if (!engineInst->Init(dataPath.c_str(), ttsEngine)) {
        TTS_ERROR_MSG(VOICE_TTS_ERR_ENGINE_RESOURE, 0, "init engine fail!");
        delete engineInst;
        engineInst = NULL;
        return false;
    }

    m_defaultEngine = engineInst;
    
    TTS_EngineInfo sysEngine;
    if (!m_configIF.getEngineLanguage(sysEngine)) {
        TTS_LOGE("No default Engine");
        return false;
    }

    m_defaultEngine->LoadEngine(sysEngine);
    m_defaultParams = sysEngine.languages;

    m_loadedEngine = true;
    return true;
}


bool TTS_EngineManager::ReLoadEngine()
{
    TTS_EngineInfo ttsEngine;
    if (!m_configIF.getEngineLanguage(ttsEngine)) {
        TTS_LOGE("No default Engine");
        return false;
    }
    
    m_defaultParams = ttsEngine.languages;
    m_senderParams.clear();

    return m_defaultEngine->ReLoadEngine(ttsEngine);
}

bool TTS_EngineManager::GetAbsoluteDataPath(const std::string& path, std::string& absolutePath)
{
    BL_File dataFile(BL_FILE_PREFIX_NDATA);
    BL_String ttsDataFullPath;
    if (!dataFile.GetLogicalPath(path.c_str(), ttsDataFullPath)) {
        TTS_LOGE("build [SoundS/TTS/] fail");
        return false;
    }

    absolutePath = ttsDataFullPath.GetString();

    return true;
}

bool TTS_EngineManager::SynthesizeText(const XCHAR* pszText, const std::string& sender, const std::string& lang, const std::string& speakersex, TTS_EngineBuffers* buffer)
{
    if (m_defaultEngine == NULL) {
        return false;
    }

    TTS_EngineLanguage param;
    if (GetParameter(sender, lang, param)) {

        if (!speakersex.empty()) {
            param.SetDefaultRoleBySex(speakersex);
        }

        m_defaultEngine->ResetEngineParam(param);
    }

    return m_defaultEngine->SynthesizeText(pszText, sender, lang, speakersex, buffer);
}

#if 0 
TTS_Engine* TTS_EngineManager::GetEngineByLang(const std::string& lang)
{
    TtsEngineInsts::iterator it = m_ttsEngines.begin();
    for (; it != m_ttsEngines.end(); it++) {
        if ((*it)->engineInfo.IsSupportLang(lang)) {
            return (*it)->engine;
        }
    }
    
    TTS_LOGI("GetEngingeByLang error, lang is : %s", lang.c_str());
    return NULL;
}

#endif

void TTS_EngineManager::ListRole(TtsEngineRoles& roles)
{
    TtsEngineLanguages::const_iterator it = m_defaultParams.begin();
    for (; it != m_defaultParams.end(); ++it) {
        roles.insert(roles.end(), it->roles.begin(), it->roles.end());
    }
}


bool TTS_EngineManager::SetParameter(const std::string& sender, const std::string& language, const TTS_EngineLanguage& param)
{
    if (!IsParamValid(param)) {
        return false;
    }

    if (m_senderParams.find(sender) == m_senderParams.end()) {
        m_senderParams[sender] = m_defaultParams;
    }

    TtsEngineLanguages::iterator it = m_senderParams[sender].begin();
    for (; it != m_senderParams[sender].end(); ++it) {
        if (it->name == language) {
            *it = param;
            return true;
        }
    }

    return false;
}

bool TTS_EngineManager::GetParameter(const std::string& sender, const std::string& language, TTS_EngineLanguage& param) const
{
    TtsEngineLanguages defaultParams = m_defaultParams;
    VoiceMap<std::string, TtsEngineLanguages>::const_iterator it = m_senderParams.find(sender);
    if (it != m_senderParams.end()) {
        defaultParams = it->second;
    }

    TtsEngineLanguages::const_iterator item = defaultParams.begin();
    for (; item != defaultParams.end(); ++item) {
        if (item->name == language) {
            param = *item;
            return true;
        }
    }

    return false;
}

bool TTS_EngineManager::IsParamValid(const TTS_EngineLanguage& langParam) const
{

    if (langParam.defaultRole < 1 || langParam.defaultRole > static_cast<int>(langParam.roles.size())) {
        return false;
    }

    const TTS_EngineParam& param = langParam.parameters;

    if (param.pitch < param.minPitch || param.pitch > param.maxPitch) {
        return false;
    }

    if (param.speed < param.minSpeed || param.speed > param.maxSpeed) {
        return false;
    }

    if (param.volume < param.minVolume || param.volume > param.maxVolume) {
        return false;
    }

    return true;
}

BL_String TTS_EngineManager::GetDefaultLanguage() const
{
    return m_configIF.getDefaultLanguage().c_str();
}

int TTS_EngineManager::GetSampleRate() const 
{
    if (NULL == m_defaultEngine) {
        return -1;
    }
    
    return m_defaultEngine->GetSampleRate();
}

#if 0
void TTS_EngineManager::AddTtsEngine(TTS_Engine *engine, const TTS_EngineInfo& engineInfo) 
{
    static int engineIndex = 0;    // for engine id
    if (!engine) {
        TTS_LOGE("engine is NULL");
        return;
    }

    TTS_EngineInst *engineInst = new(MEM_Voice) TTS_EngineInst();
    if (!engineInst) {
        TTS_LOGE("new TTS_EngineParam failed");
        return;
    }

    engineInst->engine = engine;
    engineInst->engineId = engineIndex++;
    engineInst->engineInfo = engineInfo;

    m_ttsEngines.push_back(engineInst);
    TTS_LOGD("length=%d", m_ttsEngines.size());
}


void TTS_EngineManager::clear()
{
    for (TtsEngineInsts::iterator it = m_ttsEngines.begin();
         it != m_ttsEngines.end(); ++it) {
        TTS_EngineInst *engineInst = *it;
        if (engineInst == NULL) {
            continue;
        }
        if (engineInst->engine != NULL) {
            TTS_LOGI("delete engine inst:%p", engineInst->engine);
            delete engineInst->engine;
        }
    }
    m_ttsEngines.clear();

    for (VoiceMap<std::string, void*>::iterator it = m_libraries.begin();
         it != m_libraries.end(); ++it) {
        if (it->second != NULL) {
            TTS_LOGI("dlclose(), enginelib of % s", (it->first).c_str());
            dlclose(it->second);
        }
    }
    m_libraries.clear();
}

string TTS_EngineManager::GetLangName(TTS_LANG_TYPE lang) const
{
    for (unsigned int i = 0; i < sizeof(langMap) / sizeof(TTS_LANGAGE_MAP); ++i) {
        if (langMap[i].languageType  == lang)    {
            return langMap[i].languageName;
        }
    }
    return "INVALID_LANG";
}

TTS_LANG_TYPE TTS_EngineManager::GetLangType(string langName) const
{
    for (unsigned int i = 0; i < sizeof(langMap) / sizeof(TTS_LANGAGE_MAP); ++i) {
        if (langMap[i].languageName  == langName)    {
            return langMap[i].languageType;
        }
    }
    return TTS_LANG_UK_ENGLISH;     
}

int TTS_EngineManager::GetSysLang(string langName) const
{
    for (unsigned int i = 0; i < sizeof(langMap) / sizeof(TTS_LANGAGE_MAP); ++i) {
        if (langMap[i].languageName  == langName)    {
            return langMap[i].systemLanguage;
        }
    }
    return BL_LANGUAGE_TYPE_UKENGLISH;
}
#endif

/* EOF */
