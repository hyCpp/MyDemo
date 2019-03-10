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

#include "stdafx.h"
#include "TTS_EngineNuance.h"
#include "TTS_EngineCommDef.h"
#include "TTS_Platform.h"
#include "BL_FullFile.h"
#include <string>
#include <algorithm>
#include <stdlib.h>
#include "ve_ttsapi.h"
#include "TTS_File.h"
#include <dirent.h>
#include <sys/types.h>
#include "CL_String.h"
#include "BL_UnzipFile.h"

#include "ncore/NCAutoSync.h"

using namespace std;


#define TTS_NUANCE_DICT_PRO_POSTFIX		".dcb"
#define TTS_NUANCE_RULESET_POSTFIX		".rul"


#define TTS_NUANCE_USER_DATA			"/Nuance/APL/"						// user data path
#define TTS_NUANCE_ENG_DATA				"/Nuance/Engine/"						// engine data path
#define TTS_NUANCE_BROKER_DIR              "/speech/ve/"

#define TTS_NUANCE_DATA_DIR     "/speech/components/"
#define TTS_NUNACE_COMMON_DATA_DIR  "/common/speech/components/" 

#define TTS_NUANCE_MIME_DICT			"application/edct-bin-dictionary"			// dict file type
#define TTS_NUANCE_MIME_RULESET			"application/x-vocalizer-rettt+text"		// ruleset file type
#define TTS_NUANCE_MIME_ACTIVEPROMPT	"application/x-vocalizer-activeprompt-db"	// acitive prompt

#define TTS_ENGINEZIPPASSWORD           "suntectts"         // xml zip password

bool isSubstr(const string &str1, const string &str2);

NUAN_ERROR TTS_EngineNuance::OutputCb(VE_HINSTANCE hTtsInst,
                                      void* hOutDevInst,
                                      VE_CALLBACKMSG* pcbMessage)
{
    VE_OUTDATA   *pTtsOutData;
    TTS_EngineNuance *nuance = static_cast<TTS_EngineNuance*>(hOutDevInst);
    if (!nuance) {
        RETURN(NUAN_E_NOK);
    }

    TTS_EngineBuffers *buffer = nuance->GetBuffer();
    if (!buffer) {
        RETURN(NUAN_E_NOK);
    }

    switch (pcbMessage->eMessage) {
    case VE_MSG_BEGINPROCESS:
        break;
    case VE_MSG_ENDPROCESS:
    {
        // buffer->Stop();
        break;
    }
    case VE_MSG_OUTBUFDONE:
    {
        pTtsOutData = static_cast<VE_OUTDATA *>(pcbMessage->pParam);
        if (pTtsOutData->cntPcmBufLen != 0) {
            buffer->SetWriteLen(static_cast<int>(pTtsOutData->cntPcmBufLen));
        }
        break;
    }
    case VE_MSG_OUTBUFREQ:
    {
        pTtsOutData = static_cast<VE_OUTDATA *>(pcbMessage->pParam);
        int len = 0;
        void *buf = buffer->GetWriteBuffer(len);

        if (NULL == buf || 0 == len) {
            return NUAN_E_TTS_USERSTOP;
        }

        pTtsOutData->pOutPcmBuf = buf;
        pTtsOutData->cntPcmBufLen = static_cast<size_t>(len);
        pTtsOutData->pMrkList = NULL;
        pTtsOutData->cntMrkListLen = 0;
        break;
    }
    default:
        TTS_LOGI("OutputCb::type=%d", pcbMessage->eMessage);
        break;
    }

    RETURN(NUAN_OK);
}

TTS_EngineNuance::TTS_EngineNuance()
: m_platform(new(MEM_Voice) TTS_Platform())
, m_state(ST_NONE)
, m_userDataDir("")
, m_userResource(new(MEM_Voice) TTS_UserResource())  
, m_buffer(NULL)    
{
}

TTS_EngineNuance::~TTS_EngineNuance()
{
    Close();

    if (m_platform != NULL) {
        delete m_platform;
        m_platform = NULL;
    }

    // Uninit();

}

bool TTS_EngineNuance::InitDataFile(const TtsEngineLanguages& params, std::string& brokerInfo, VoiceVector<TTS_Platform::TTS_EngineDataFile>::type& dataFiles)
{
    std::string dirPath = m_userDataDir + TTS_NUANCE_ENG_DATA;
    for (const auto& param : params) {
        std::string stdLang = param.name;
        transform(stdLang.begin(), stdLang.end(), stdLang.begin(), ::tolower);
        std::string brokerPath = dirPath + stdLang + TTS_NUANCE_BROKER_DIR;

        VoiceVector<std::string>::type roleFiles;
        if (param.GetRoleFiles(roleFiles)) {
            LoadBrokerInfo(brokerPath, roleFiles, brokerInfo);
        }
        else {
            LoadBrokerInfo(brokerPath, brokerInfo);
        }

        std::string datafilePath = dirPath + stdLang + TTS_NUANCE_DATA_DIR;

        LoadDataFileInfo(datafilePath, dataFiles);
    }

    std::string commonfilePath = dirPath + TTS_NUNACE_COMMON_DATA_DIR;
    LoadDataFileInfo(commonfilePath, dataFiles);

    return !brokerInfo.empty() && !dataFiles.empty();
}

bool TTS_EngineNuance::Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo)
{
    nutshell::NCAutoSync sync(m_syncObj);
    TTS_LOGD("nuance init,conf=%s", conf);
    m_state = ST_INIT;
    // m_defaultLang = engineInfo.defaultLang;

    const TtsEngineLanguages& params = engineInfo.languages;

    memset(&m_install, 0, sizeof(VE_INSTALL));
    m_install.fmtVersion = VE_CURRENT_VERSION;
    m_install.pBinBrokerInfo = NULL;
    m_userDataDir = conf;

    
    InitDataFile(params, m_brokerInfo, m_dataFiles);

    TTS_LOGD("nuance Init Info=%s", m_brokerInfo.c_str());

    NUAN_ERROR err = m_platform->GetInterface(&m_install, &m_dataFiles);
    if (err != NUAN_OK) {
        TTS_LOGE("Interfaces Err:0x %lx", err);
        m_state = ST_NONE;
        return false;
    }

    m_install.pBinBrokerInfo = m_brokerInfo.c_str();

    /* Initialize the engine */
    err = ve_ttsInitialize(&m_install, &m_speech);
    m_install.pBinBrokerInfo = NULL;
    if (err != NUAN_OK) {
        TTS_LOGE("VE_ttsInit: 0x %lx\n", err);
        m_state = ST_NONE;
        return false;
    }


    std::string userDataPath = m_userDataDir + TTS_NUANCE_USER_DATA;

    LoadUserResources(userDataPath, m_userResource);
    m_state = ST_NONE;
    TTS_LOGI("nuance engine init OK!");

    return true;
}

CL_ERROR TTS_EngineNuance::Uninit()
{
    /* Uninitialize */
    TTS_LOGI("nuance Uninitialize");
    NUAN_ERROR err = ve_ttsUnInitialize(m_speech);
    if (err != NUAN_OK) {
        TTS_LOGE("ttsUnInit Err: 0x%lx", err);
    }

    return TTS_ERR_NO_ERROR;
}


bool TTS_EngineNuance::SynthesizeText(const XCHAR* pszText, const std::string& module, const std::string& lang, 
    const std::string& sex, TTS_EngineBuffers* buffer)
{
    TTS_LOGI("%s, module is : %s", pszText, module.c_str());
    nutshell::NCAutoSync sync(m_syncObj);

    std::string language = lang;
    if (language.empty()) {
        language = m_defaultLang;
    }

    if (m_mapInstance.find(language) == m_mapInstance.end()) {
        TTS_ERROR_MSG(VOICE_TTS_ERR_UNSUPPORTED_LANG, 0, "error lang: %s", language.c_str());
        return false;
    }

    VE_HINSTANCE instance = static_cast<VE_HINSTANCE>(m_mapInstance[language]);
    m_buffer = buffer;

    m_state = ST_RUNING;

    VE_INTEXT stInText;
    stInText.eTextFormat = VE_NORM_TEXT;
    // here xcslen directly return size_t?
    stInText.cntTextLength = static_cast<size_t>(xcslen(pszText));
    stInText.szInText = static_cast<void*>(const_cast<XCHAR*>(pszText));

    m_userResource->Active(instance, module, lang, sex, true);

    NUAN_ERROR nErrcode = ve_ttsProcessText2Speech(instance, &stInText);
    bool ret = (NUAN_OK != nErrcode && NUAN_E_TTS_USERSTOP != nErrcode);
    if (ret) {
        TTS_LOGE("Err VE_t2s:0x%lx", nErrcode);
    }
    m_state = ST_NONE;
    m_userResource->Active(instance, module, lang, sex, false);

    return !ret;
}

CL_ERROR TTS_EngineNuance::Stop(const std::string& lang)
{
    TTS_LOGI("Stop TTS instance");
    if (ST_RUNING == m_state) {

        if (m_mapInstance.find(lang) == m_mapInstance.end()) {
            return TTS_ERR_INVALID_PARAM;
        }

        VE_HINSTANCE instance = static_cast<VE_HINSTANCE>(m_mapInstance[lang]);

        NUAN_ERROR err = ve_ttsStop(instance);
        if (NUAN_OK != err) {
            TTS_LOGE("stop error...");
        }
    }
    return TTS_ERR_NO_ERROR;
}


bool TTS_EngineNuance::Open(const TTS_EngineLanguage& param)
{
    TTS_LOGI("Open Engine");
    VE_HINSTANCE instance;
    NUAN_ERROR err = ve_ttsOpen(m_speech, m_install.hHeap, m_install.hLog, &instance);
    if (err != NUAN_OK) {
        TTS_LOGE("open %s, Err=0x%x", param.name.c_str(), err);
    }
    else {

        // Nuance 64 max VE_Param is 22
        VE_PARAM ttsParam[22];
        NUAN_U16 cttsParam = 0;
        memset(ttsParam, 0, sizeof(ttsParam));
        if (!SetParamValue(param, ttsParam, cttsParam)) {
            TTS_LOGE("SetParameterValue error");
        }

        // Set parameters, these are initialized from the command line arguments
        NUAN_ERROR nErrcode = ve_ttsSetParamList(instance, &ttsParam[0], cttsParam);
        if (nErrcode != NUAN_OK) {
            TTS_ERROR_MSG(VOICE_TTS_ERR_INVALID_PARAM, nErrcode, "Set param for engine failed!"); 
        }
        else {
            TTS_LOGI("Open Finish! %s", param.name.c_str());
            SetCallBack(instance);
            m_mapInstance[param.name] = instance;
            return true;
        }
    } 

    return false;
}

NUAN_ERROR TTS_EngineNuance::Close()
{
    // Close the TTS instance
    TTS_LOGI("Close TTS instance");

    for (auto& instance : m_mapInstance) {
        NUAN_ERROR err = ve_ttsClose(instance.second);
        if (err != NUAN_OK) {
            TTS_LOGE("ve_Close %s err=0x%lx", instance.first.c_str(), err);
        }
    }

    return NUAN_OK;
}

bool TTS_EngineNuance::LoadEngine(const TTS_EngineInfo& engineInfo)
{
    // m_defaultLang = engineInfo.defaultLang;
    const TtsEngineLanguages& params = engineInfo.languages;

    for (const auto& param : params) {
        Open(param);
    }

    return !m_mapInstance.empty();
}

bool TTS_EngineNuance::ReLoadEngine(const TTS_EngineInfo& engineInfo)
{
    nutshell::NCAutoSync sync(m_syncObj);
    // m_defaultLang = engineInfo.defaultLang;
    const TtsEngineLanguages& params = engineInfo.languages;

    return AppendInstance(params) && CloseUnusedInstance(params);
}

bool TTS_EngineNuance::AppendInstance(const TtsEngineLanguages& params)
{
    for (const auto& param : params) {
        if (m_mapInstance.find(param.name) == m_mapInstance.end()) {
            Open(param);
        }
    }
    return true;
}

bool TTS_EngineNuance::CloseUnusedInstance(const TtsEngineLanguages& params)
{
    for (auto instance = m_mapInstance.begin(); instance != m_mapInstance.end();) {
        std::string name = instance->first;
        auto it = std::find_if(params.begin(), params.end(),\
                [&name](TTS_EngineLanguage const& param) { return param.name == name; });
        if (it == params.end()) {
            if (ve_ttsClose(instance->second) == NUAN_E_WRONG_STATE) {
                ++instance;
                TTS_LOGE("error when close tts engine!");
            }
            else {
                instance = m_mapInstance.erase(instance);
            }
        }

        ++instance;
    }

    return true;
}

bool TTS_EngineNuance::SetParamValue(const TTS_EngineLanguage& param, VE_PARAM *ttsParam, NUAN_U16 &cttsParam)
{
    std::string langCode = param.name;

    VE_LANGUAGE lang;
    if (!GetLanguageInfo(langCode, lang)) {
        // do log
        return false;
    }

    ttsParam[cttsParam].eID = VE_PARAM_MARKER_MODE;
    ttsParam[cttsParam++].uValue.usValue = (NUAN_U16)VE_MRK_OFF;

    ttsParam[cttsParam].eID = VE_PARAM_WAITFACTOR;
    ttsParam[cttsParam++].uValue.usValue = 1;

    ttsParam[cttsParam].eID = VE_PARAM_INITMODE;
    ttsParam[cttsParam++].uValue.usValue = VE_INITMODE_LOAD_ONCE_OPEN_ALL;

    ttsParam[cttsParam].eID = VE_PARAM_MAX_INPUT_LENGTH;
    ttsParam[cttsParam++].uValue.usValue = 250;
    
    // ttsParam[cttsParam].eID = VE_PARAM_TEXTMODE;
    // ttsParam[cttsParam++].uValue.usValue = VE_TEXTMODE_SMS;

    ttsParam[cttsParam].eID = VE_PARAM_FREQUENCY;
    ttsParam[cttsParam++].uValue.usValue = VE_FREQ_22KHZ;

    ttsParam[cttsParam].eID = VE_PARAM_TYPE_OF_CHAR;
    ttsParam[cttsParam++].uValue.usValue = VE_TYPE_OF_CHAR_UTF8;

    ttsParam[cttsParam].eID = VE_PARAM_VOLUME;
    ttsParam[cttsParam++].uValue.usValue = param.parameters.volume;	// volume level on a 0 to 100 scale.
    
    TTS_LOGI("volume is %d :", param.parameters.volume);

    ttsParam[cttsParam].eID = VE_PARAM_PITCH;
    ttsParam[cttsParam++].uValue.usValue = param.parameters.pitch;	// pitch level on a 1 to 100 scale.

    ttsParam[cttsParam].eID = VE_PARAM_SPEECHRATE;
    ttsParam[cttsParam++].uValue.usValue = param.parameters.speed;	// The valid range is [50..400]

    ttsParam[cttsParam].eID = VE_PARAM_LANGUAGE;
    strcpy(ttsParam[cttsParam++].uValue.szStringValue, lang.szLanguage);

    ttsParam[cttsParam].eID = VE_PARAM_VOICE;
    strcpy(ttsParam[cttsParam++].uValue.szStringValue, param.roles[param.defaultRole - 1].roleName.c_str());

    return true;
}


void TTS_EngineNuance::ResetEngineParam(const TTS_EngineLanguage& param)
{
    TTS_LOGD("ReSetParam...");

    m_currLanguageParam = param;

    if (m_mapInstance.find(param.name) == m_mapInstance.end()) {
        TTS_LOGE("wrong lang: %s", param.name.c_str());
        return;
    }

    VE_HINSTANCE instance = m_mapInstance[param.name];

    // max parameter count of nuance engine is 24
    VE_PARAM ttsParam[24];
    NUAN_U16 cttsParam = 0;
    memset(ttsParam, 0, sizeof(ttsParam));
    if (!ReSetParam(param, ttsParam, cttsParam)) {
        return;
    }

    // Set parameters, these are initialized from the command line arguments
    NUAN_ERROR nErrcode = ve_ttsSetParamList(instance, &ttsParam[0], cttsParam);
    if (nErrcode != NUAN_OK) {
        TTS_LOGE("Err VE_SetParList: 0x%lx", nErrcode);
        // Close();
    }

}

bool TTS_EngineNuance::ReSetParam(const TTS_EngineLanguage& param, VE_PARAM *ttsParam, NUAN_U16 &cttsParam)
{
    ttsParam[cttsParam].eID = VE_PARAM_VOLUME;
    ttsParam[cttsParam++].uValue.usValue = param.parameters.volume;	// volume level on a 0 to 100 scale.

    TTS_LOGI("volume is %d :", param.parameters.volume);

    ttsParam[cttsParam].eID = VE_PARAM_PITCH;
    ttsParam[cttsParam++].uValue.usValue = param.parameters.pitch;	// pitch level on a 1 to 100 scale.
    ttsParam[cttsParam].eID = VE_PARAM_SPEECHRATE;
    ttsParam[cttsParam++].uValue.usValue = param.parameters.speed;	// The valid range is [50..400]

    ttsParam[cttsParam].eID = VE_PARAM_VOICE;
    strcpy(ttsParam[cttsParam++].uValue.szStringValue, param.roles[param.defaultRole - 1].roleName.c_str());

    
    // ttsParam[cttsParam].eID = VE_PARAM_TEXTMODE;
    // ttsParam[cttsParam++].uValue.usValue = VE_TEXTMODE_SMS;

    return true;
}


NUAN_ERROR TTS_EngineNuance::SetCallBack(VE_HINSTANCE instance)
{
    VE_OUTDEVINFO outDevInfo;
    memset(&outDevInfo, 0, sizeof(outDevInfo));
    outDevInfo.pUserData = (void*)this;
    outDevInfo.pfOutNotify = TTS_EngineNuance::OutputCb;
    NUAN_ERROR err = ve_ttsSetOutDevice(instance, &outDevInfo);
    if (err != NUAN_OK) {
        TTS_LOGE("Err VE_Device: 0x%lx", err);
    }

    return err;
}

CL_ERROR TTS_EngineNuance::GetIntonation(TtsIntonation* pIntonation) const
{
    return TTS_ERR_NO_ERROR;
}

CL_ERROR TTS_EngineNuance::SetIntonation(TtsIntonation intonation)
{
    return TTS_ERR_NO_ERROR;
}

INT TTS_EngineNuance::GetSampleRate() const
{
    return 22050;
}

bool TTS_EngineNuance::LoadDataFileInfo(const std::string& path, VoiceVector<TTS_Platform::TTS_EngineDataFile>::type& dataFiles)
{
    DIR *dir = opendir(path.c_str());
    if (NULL == dir) {
        return false;
    }

    dirent *dt = NULL;
    while (true) {
        dt = readdir(dir);
        if (NULL == dt) {
            break;
        }
        if (DT_REG == dt->d_type) {
            if (isSubstr(dt->d_name, ".dat")) {
                TTS_Platform::TTS_EngineDataFile datafile;
                datafile.m_dirPath = path;
                datafile.m_brokerName = dt->d_name;

                dataFiles.push_back(datafile);
            }
        }
    }

    return true;
}

bool TTS_EngineNuance::LoadBrokerInfo(const std::string& path, const VoiceVector<std::string>::type& roleFiles, std::string& brokerInfo)
{
    for (auto& file : roleFiles) {
        std::string absPath = path + file;
        char *buf = NULL;
        long bufSize = ReadFile(absPath, buf);

        if (bufSize> 0 && buf != NULL) {
            brokerInfo.append(buf, bufSize);
        }

        if (NULL != buf) {
            delete[] buf;
            buf = NULL;
        }
    }

    return !brokerInfo.empty();
}

bool TTS_EngineNuance::LoadBrokerInfo(const std::string& path, std::string& brokerInfo)
{
    DIR *dir = opendir(path.c_str());
    if (NULL == dir) {
        TTS_ERROR_MSG(VOICE_TTS_ERR_NULL_PARAM, 0, "opendir %s failed!", path.c_str());
        return false;
    }

    dirent *dt = NULL;
    while (true) {
        dt = readdir(dir);
        if (NULL == dt) {
            break;
        }
        if (DT_REG == dt->d_type) {
            if (isSubstr(dt->d_name, ".hdr")) {
                string fileName = dt->d_name;

                string absPath = path + fileName;
                char *buf = NULL;
                long bufSize = ReadFile(absPath, buf);
                
                if (bufSize > 0 && buf != NULL) {
                    brokerInfo.append(buf, bufSize);
                }

                if (NULL != buf) {
                    delete[] buf;
                    buf = NULL;
                }
            }
        }
    }

    return !brokerInfo.empty();
}


LONG TTS_EngineNuance::ReadFile(const string& fileName, char *&buffer)
{
    FILE *fp = fopen(fileName.c_str(), "rw");
    if (NULL == fp) {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    buffer = new(MEM_Voice) char[size + 1];
    memset(buffer, 0, size + 1);
    long actual_size = fread(buffer, 1, size, fp);
    TTS_LOGD("Size=%d,ReadSize=%d", size, actual_size);
    fclose(fp);

    return actual_size;
}


bool TTS_EngineNuance::GetLanguageInfo(const std::string& langCode, VE_LANGUAGE& langInfo)
{
    TTS_LOGD("GetLanguageInfo...");
    NUAN_U16 langCount = 0;
    ve_ttsGetLanguageList(m_speech, NULL, &langCount);
    if (0 == langCount) {
        TTS_LOGE("ve_ttsGetLanguageList get lang num is 0!");
        return false;
    }

    std::unique_ptr<VE_LANGUAGE[]> pLangs(new(MEM_Voice) VE_LANGUAGE[langCount]);
    NUAN_ERROR err = ve_ttsGetLanguageList(m_speech, pLangs.get(), &langCount);
    if (NUAN_OK != err) {
        TTS_LOGE("ve_GetLang err=%x", err);
        return false;
    }

    for (int i = 0; i < langCount; ++i) {
        TTS_LOGI("Code=%s, tlw=%s", langCode.c_str(), pLangs[i].szLanguageTLW);
        if (0 == strcmp(pLangs[i].szLanguageTLW, langCode.c_str())) {
            langInfo = pLangs[i];
            return true;
        }
    }

    return false;
}

void TTS_EngineNuance::LoadUserResources(const std::string& path, std::shared_ptr<TTS_UserResource>& resource)
{
    DIR *dir = opendir(path.c_str());
    if (NULL == dir) {
        return;
    }
    
    while (true) {
        dirent *dt = readdir(dir);
        if (NULL == dt) {
            break;
        }
        if (DT_REG == dt->d_type) {           
            LoadUserResourceFile(path, dt->d_name, resource);
        }
        else if (DT_DIR == dt->d_type) {

            if (0 != strcmp(dt->d_name, ".")  && 0 != strcmp(dt->d_name, "..")) {
                std::shared_ptr<TTS_UserResource> subResource(new(MEM_Voice) TTS_UserResource());
                const std::string subpath = path + "/" + dt->d_name;

                LoadUserResources(subpath, subResource);
                if (!subResource->IsEmpty()) {
                    resource->AddSubResource(dt->d_name, subResource);
                }
            }
        }
        else {
        }
    }
}

bool isSubstr(const string &str1, const string &str2)
{
    if (str1.size() >=  str2.size()) {
        if (str1[0] == '.') {
            return false;
        }
        string::size_type len = str2.size();
        for (string::size_type i = 0; i<str1.size()-len + 1; i++) {
            string tmpstr = str1.substr(i, len);
            if (tmpstr == str2) {
                return true;
            }
        }
        return false;
    }
    return false;
}

void TTS_EngineNuance::LoadUserResourceFile(const std::string& path, const std::string& fileName, std::shared_ptr<TTS_UserResource>& source)
{
    std::string dataType{};

    if (isSubstr(fileName, ".dcb")) {
        dataType = TTS_NUANCE_MIME_DICT;
    }
    else if (isSubstr(fileName, ".rul")) {
        dataType = TTS_NUANCE_MIME_RULESET;
    }
    else if (isSubstr(fileName, ".dat")) {
        dataType = TTS_NUANCE_MIME_ACTIVEPROMPT;
    }
    else {
    }
    
    BYTE *userDataBuf = NULL;

    std::string fileFullPath = path + "/" + fileName;

    int userDataBufSize = UnzipSelectFileToBuffer(fileFullPath.c_str(), fileName.c_str(), TTS_ENGINEZIPPASSWORD, userDataBuf);
    if (0 >= userDataBufSize || NULL == userDataBuf) {
        TTS_LOGE("Unzip %s fail", path.c_str());
        return;
    }

    std::shared_ptr<TTS_UserData> userData (new(MEM_Voice) TTS_UserData(dataType, userDataBuf, userDataBufSize)); 

    source->AddRootData(userData);
}


extern "C" {
    // nuance engine instance
    __attribute__((visibility("default"))) TTS_Engine* GetInstance()
    {
        return new(MEM_Voice) TTS_EngineNuance();
    }
};

/* EOF */
