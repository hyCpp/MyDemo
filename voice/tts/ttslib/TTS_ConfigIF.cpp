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
#include <algorithm>
#include <BL_File.h>
#include <BL_String.h>
#include <BL_UnzipFile.h>
#ifdef HAVE_NUTSHELL_OS
#include "contentresolver/NCContentListener.h"
#include "contentresolver/NCContentResolver.h"
#endif
#include "stdafx.h"
#include "TTS_ConfigIF.h"

using namespace std;

static const XCHAR* const TTS_CONFIGFILEPATH = XTEXT("TTS/tts_config.dat");
#if 0
static const XCHAR* const TTS_CONFIGFILEPATH = XTEXT("TTS/tts_config.xml");
#endif
static const XCHAR* const TTS_CONFIGFILENAME = XTEXT("tts_config.xml");
static const XCHAR* const TTS_CONFIGFILEPASSWORD = XTEXT("suntectts");
static const XCHAR* const TTS_VERIFIERDATA = XTEXT("../VDATA_ERROR.FLG");


VoiceMap<std::string, std::string>::type TTS_ConfigIF::m_mapLanguage({
    { "en-us", "ENU" },
    { "fr-ca", "FRC" },
    { "es-mx", "SPM" },
    { "en-au", "ENA" },
    { "zh-cn", "MNC" },
    { "zh-tw", "MNT" },
    { "zh-hk", "CAH" },
    { "ko-kr", "KOK" },
    { "pl",    "PLP" },
    { "fr",    "FRF" },
    { "nl",    "DUN" },
    { "en-gb", "ENG" },
    { "en-id", "ENI" },
    { "id",    "IDI" },
    { "el",    "GRG" },
    { "fi",    "FIF" },
    { "hi",    "HII" },
    { "sv",    "SWS" },
    { "he",    "HEI" },
    { "da",    "DAD" },
    { "no",    "NON" },
    { "sk",    "SKS" },
    { "ar",    "ARW" },
    { "it",    "ITI" },
    { "hu",    "HUH" },
    { "pt-br", "PTB" },
    { "tr",    "TRT" },
    { "th",    "THT" },
    { "de",    "GED" },
    { "ru",    "RUR" },
    { "cs",    "CZC" },
    { "ro",    "ROR" },
    { "es",    "SPE" },
    { "pt",    "PTP" }, 
    // for TTS does not language, we will set it to default Language
    { "vi",    "ENG" },
    { "ms",    "ENG" },
    { "en-me", "ENG" },
    // for language with sex
    { "ar-ml", "ARW-male" },
    { "ar-fm", "ARW-female" },
});

TTS_ConfigIF::TTS_ConfigIF()
{
}

TTS_ConfigIF::~TTS_ConfigIF()
{
}

bool TTS_ConfigIF::init()
{

    TTS_LOGI("TTS_ConfigIF::init()");
    BL_File dataFile(BL_FILE_PREFIX_NDATA);

    if (dataFile.FileExists(TTS_VERIFIERDATA)) {
        TTS_ERROR_MSG(VOICE_TTS_ERR_DATA_PATH, 0, "VDATA has error, can't load data for TTS engine");
        return false;
    }

    BL_String xmlFileFullPath;
    BL_String xmlFilePath(TTS_CONFIGFILEPATH);
    if (!dataFile.GetLogicalPath(xmlFilePath, xmlFileFullPath)) {
        TTS_LOGE("Init full path fail");
        return false;
    }

    BYTE *xmlBuf = NULL;

    int bufSize = UnzipSelectFileToBuffer(xmlFileFullPath.GetString(), TTS_CONFIGFILENAME, TTS_CONFIGFILEPASSWORD, xmlBuf);
    if (0 == bufSize || NULL == xmlBuf) {
        TTS_LOGE("Unzip file fail. path=%s", xmlFileFullPath.GetString());
        return false;
    }

    bool ret = parser(reinterpret_cast<char*>(xmlBuf));

    if (NULL != xmlBuf) {
        delete[] xmlBuf;
        xmlBuf = NULL;
    }

    RETURN(ret);
}

bool TTS_ConfigIF::loadEngines(const TiXmlElement* engines)
{
    const TiXmlElement *engineNode = engines->FirstChildElement("TtsEngine");
    while (NULL != engineNode) {
        if (!loadEngine(engineNode)) {
            TTS_LOGE("fail load engine!");
            return false;
        }
        engineNode = engineNode->NextSiblingElement("TtsEngnie");
    }

    TTS_LOGI("egines count=%d", m_engines.engines.size());
    return !m_engines.engines.empty();
}

bool TTS_ConfigIF::loadEngine(const TiXmlElement* engineNode)
{
    const char *engineName = engineNode->Attribute("name");
    const char *enginePath = engineNode->Attribute("path");
    if (engineName == NULL || enginePath == NULL) {
        TTS_LOGE("Load name or path NULL");
        return false;
    }

    TTS_EngineInfo engine;
    engine.name = engineName;
    engine.path = enginePath;

    const TiXmlElement *languageNode = engineNode->FirstChildElement("language");
    while (NULL != languageNode) {
        TTS_EngineLanguage language;
        if (!loadLanguage(languageNode, language)) {
            TTS_LOGE("fail load lang!");
            return false;
        }

        engine.languages.push_back(language);
        languageNode = languageNode->NextSiblingElement("language");
    }

    TTS_LOGI("put engine to engines");
    m_engines.engines.push_back(engine);

   return true;
}

bool TTS_ConfigIF::loadLanguage(const TiXmlElement*languageNode, TTS_EngineLanguage& language)
{
    if (NULL == languageNode) {
        TTS_LOGE("loadLang LangNode NULL");
        return false;
    }

    const char* name = languageNode->Attribute("name");
    if (NULL == name) {
        TTS_LOGE("loadLang name NULL");
        return false;
    }

    TTS_LOGI("loadLang: name= %s", name);

    language.name = name;

    const TiXmlElement *paramNode = languageNode->FirstChildElement("parameters");

    if (!loadParam(paramNode, language.parameters)) {
        TTS_LOGE("fail load params!");
        return false;
    }

    TtsEngineRoles roles;
    const TiXmlElement *rolesNode = languageNode->FirstChildElement("roles");
    if (!loadRoles(rolesNode, language.roles, language.defaultRole)) {
        TTS_LOGE("failed load roles");
        return false;
    }

    return true;

}

bool TTS_ConfigIF::loadParam(const TiXmlElement* parameters, TTS_EngineParam &param)
{
    if (parameters == NULL) {
        TTS_LOGE("loadParam Params NULL!");
        return false;
    }
    const TiXmlElement *pitch = parameters->FirstChildElement("pitch");
    const TiXmlElement *speed = parameters->FirstChildElement("speed");
    const TiXmlElement *volume = parameters->FirstChildElement("volume");
    if (pitch == NULL || speed == NULL || volume== NULL) {
        TTS_LOGE("pitch speed volume NULL");
        return false;
    }

    pitch->Attribute("min", &param.minPitch);
    pitch->Attribute("max", &param.maxPitch);
    pitch->Attribute("set", &param.pitch);

    speed->Attribute("min", &param.minSpeed);
    speed->Attribute("max", &param.maxSpeed);
    speed->Attribute("set", &param.speed);

    volume->Attribute("min", &param.minVolume);
    volume->Attribute("max", &param.maxVolume);
    volume->Attribute("set", &param.volume);

    return true;
}

bool TTS_ConfigIF::loadRoles(const TiXmlElement* rolesNode, TtsEngineRoles& roles, int& defaultRole)
{
    if (NULL == rolesNode) {
        TTS_LOGE("loadRoles rolesNdoe NULL");
        return false;
    }

    if (rolesNode->Attribute("default", &defaultRole) == NULL) {
        TTS_LOGE("default Node NULL!");
        return false;
    }

    const TiXmlElement *item = rolesNode->FirstChildElement("item");

    while (item != NULL) {
        TTS_EngineRole role;

        const char *type = item->Attribute("type");
        const char *roleName = item->Attribute("name");
        const char *roleFile = item->Attribute("file");

        if (item->Attribute("roleIndex", &role.roleIndex) == NULL || item->Attribute("roleId", &role.roleId) == NULL) {
            TTS_LOGE("roleIndex or Id NULL!");
            return false;
        }

        role.roleName = roleName;
        role.roleSex = type;

        if (NULL != roleFile) {
            role.roleFile = roleFile;
        }

        roles.push_back(role);
        item = item->NextSiblingElement("item");
    }

    return true;
}

bool TTS_ConfigIF::parser(const char* xmlBuf)
{
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(xmlBuf);
    if (xmlDoc.Error()) {
        TTS_LOGE("Parse xml failed");
        return false;
    }

    const TiXmlNode* rootNode = xmlDoc.RootElement();
    if (NULL == rootNode) {
        TTS_LOGE("Parse fail,rootNode NULL");
        return false;
    }
    const TiXmlElement* rootEle = rootNode->ToElement();
    if (NULL == rootEle) {
        TTS_LOGE("Parse fail,rootEle NULL");
        return false;
    }
    if (0 != strcmp("TtsConfig", rootEle->Value())) {
        TTS_LOGE("Parse,Not TtsConfig");
        return false;
    }

    const char *defaultLang = rootEle->Attribute("defaultLang");
    const char *defaultEngine = rootEle->Attribute("defaultEngine");
    if (defaultLang == NULL || defaultEngine == NULL) {
        TTS_LOGE("No default lang engine");
        return false;
    }

    m_engines.defaultLang = defaultLang;
    m_engines.defaultEngine = defaultEngine;
    const char* loadEngineBySystem = rootEle->Attribute("loadEngineBySystem");
    if (NULL != loadEngineBySystem) {
        m_engines.loadEngineBySystem = loadEngineBySystem;
    }
    
    const TiXmlElement* engines = rootEle->FirstChildElement("TtsEngines");
    return loadEngines(engines);
}

const TTS_EnginesInfo &TTS_ConfigIF::getEngines() const
{
    return m_engines;
}

bool TTS_ConfigIF::getEngineLanguage(TTS_EngineInfo& engineInfo) const
{
    if (IsLoadEngineBySystem()) {
        return getEngineBySys(engineInfo);
    }
    return getDefaultEngine(engineInfo);
}

bool TTS_ConfigIF::getDefaultEngine(TTS_EngineInfo& engineInfo) const
{
    TtsEngineInfos::const_iterator it = m_engines.engines.begin();

    for (; it != m_engines.engines.end(); ++it) {
        TTS_LOGI("getDefaultEngines: %s", it->name.c_str());
        if (it->name == m_engines.defaultEngine) {
            engineInfo = *it;
            return true;
        }
    }

    TTS_LOGI("No Default Engine: %s", m_engines.defaultEngine.c_str());

    return false;
}

bool TTS_ConfigIF::getEngineBySys(TTS_EngineInfo& engineInfo) const
{
    TTS_EngineInfo defaultEngine;
    if (!getDefaultEngine(defaultEngine)) {
        return false;
    }

    std::string sysLang;
    std::string vrLang;

    if (!getSysLanguage(sysLang, vrLang)) {
        return false;
    }

    TTS_LOGI("sys Language: %s, vr Language: %s", sysLang.c_str(), vrLang.c_str());

    engineInfo.name = defaultEngine.name;
    engineInfo.path = defaultEngine.path;

    for (auto language : defaultEngine.languages) {
        if (language.name == sysLang || language.name == vrLang) {
            engineInfo.languages.push_back(language);
        }
    }
    return !engineInfo.languages.empty();
}


bool TTS_ConfigIF::getSysLanguage(std::string& sysLang, std::string& vrLang) const
{ 
#ifdef HAVE_NUTSHELL_OS
    nutshell::NCUri uri("content://settings/system");
    nutshell::NCObjList<nutshell::NCString> projection;
    projection << new(MEM_Voice) nutshell::NCString("language");
    projection << new(MEM_Voice) nutshell::NCString("vr_language");
   
    nutshell::NCContentResolver resolver;
    nutshell::NCCursor* cursor = resolver.query(uri, projection, nutshell::NCString(), nutshell::NCObjList<nutshell::NCString>(), nutshell::NCString());

    if (NULL == cursor) {
        TTS_LOGE("can not get system language.");
        return false;
    }

    std::string sysValue, vrValue;


    if ((1 <= cursor->getCount()) && (cursor->moveToFirst())) {
        sysValue = cursor->getString(0);
        vrValue = cursor->getString(1);
    }
   
    if (cursor != NULL) { 
        delete cursor;
        cursor = NULL;
    }

    TTS_LOGI("sys Language: %s, vr Language: %s", sysValue.c_str(), vrValue.c_str());

    if (m_mapLanguage.find(sysValue.c_str()) != m_mapLanguage.end()) {
        sysLang = m_mapLanguage[sysValue.c_str()];
    }
    
    if (m_mapLanguage.find(vrValue.c_str()) != m_mapLanguage.end()) {
        vrLang = m_mapLanguage[vrValue.c_str()];
    }

    return true;
#else
    sysLang = "MNC";
    vrLang = "MNC";
    return true;
#endif
}

#if 0
const TTS_EngineLanguage* TTS_ConfigIF::getEngineLangInfo(const string &language) const
{
    for (TtsEngineInfos::const_iterator it = m_engines.engines.begin();
         it != m_engines.engines.end(); ++it) {
         if (it->IsSupportLang(language)) {
             return &(*it);
         }
    }

    return NULL;
}

const TtsEngineRoles &TTS_ConfigIF::getRoles() const
{
    return m_roles;
}

int TTS_ConfigIF::getRoleIdByIndex(int roleIndex) const
{
    for (TtsEngineRoles::const_iterator it = m_roles.begin(); it != m_roles.end(); ++it) {
        if (it->roleIndex == roleIndex) {
            int roleId = it->roleId;
            RETURN(roleId);
        }
    }

    return - 1;
}

#endif


/* EOF */
