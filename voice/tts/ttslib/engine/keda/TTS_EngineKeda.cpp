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

#include "TTS_EngineKeda.h"
#include "TTS_EngineCommDef.h"

#include "iss_auth.h"
// #include "VAString.h"


/* Resource file count */
#define TTS_RES_FILE    "/Keda"


static const XCHAR* SILENT_TEXT = XTEXT("[p300] ");



void TTS_EngineKeda::OnTTSDataReady(void* pUsrArg)
{
    TTS_EngineKeda* pThis = static_cast<TTS_EngineKeda*>(pUsrArg);
    pThis->m_waitStop.Notify();
}

bool TTS_EngineKeda::OutputTTSData()
{
    if (m_buffer == NULL) {
        return true;
    }

    int len = 0;
    void *buf = m_buffer->GetWriteBuffer(len);

    if (NULL == buf || len <= 0) {
        return true;
    }

    bool bEnd = false;
    unsigned int nGet = 0;

    ISSErrID ret = ISSTTSGetAudioData(m_hTTS, buf,
                                 len / 2, &nGet, NULL, NULL);

    if (ISS_ERROR_TTS_COMPLETED == ret) {
        bEnd = true;
    }
    else if (ISS_SUCCESS == ret && nGet == 0) {
        // DebugBreak();
    }
    else if (ISS_SUCCESS == ret && nGet > 0) {

    } 
    else {
        // DebugBreak();
    }

    m_buffer->SetWriteLen(nGet * 2);

    return bEnd;
}

//////////////////////////////////////////////

TTS_EngineKeda::TTS_EngineKeda() : m_buffer(NULL)
{
}

TTS_EngineKeda::~TTS_EngineKeda()
{
    // Uninit();
}


bool TTS_EngineKeda::Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo)
{

    if (NULL == conf) {
        TTS_LOGE("Keda::Init invalid path!");
        return false;
    }

    std::string path = conf;
    path += TTS_RES_FILE;

    ISSErrID err = TtsInit(path.c_str());
    
    if (ISS_SUCCESS != err) {
        TTS_LOGE("Keda::Init failed");
        return false;
    }

    m_param = engineInfo.languages;

    TTS_LOGD("Keda::Init ok");
    return true;
}

CL_ERROR TTS_EngineKeda::Uninit()
{
    TTS_LOGI("Keda::Uninit");
    TtsUninit();
    return TTS_ERR_NO_ERROR;
}

CL_ERROR TTS_EngineKeda::SetLanguage(const std::string& lang)
{
    return TTS_ERR_NO_ERROR;
}

CL_ERROR TTS_EngineKeda::GetLanguage(std::string& lang)
{
    return TTS_ERR_NO_ERROR;
}

bool TTS_EngineKeda::SynthesizeText(const XCHAR* pszText, const std::string& module, const std::string& lang, TTS_EngineBuffers* buffer)
{
    TTS_LOGI("lang: %s, m_lang: %s", lang.c_str(), m_lang.c_str());
    if (NULL == m_hTTS) {
        TTS_LOGE("Synth:Keda not init!");
        return false;
    }

    if (NULL == pszText || NULL == buffer) {
        TTS_LOGE("SynthesizeText null param");
        return false;
    }

    m_buffer = buffer;

    // if (lang != m_lang) {
    if (SetParams(lang)) {
        m_lang = lang;
    }
    

    CL_String text(pszText);
    int len = text.GetStringLength(CL_CP_UTF16);

    UCHAR16* u16str  =  new(MEM_Voice) UCHAR16[len + 1];
    TTS_LOGD("Synth text=[%s],len=%ld", pszText, len);

    int u16len = text.GetUTF16LE(u16str, len + 1);
    m_waitStop.Reset();
    ISSErrID ret = ISSTTSStart(m_hTTS, u16str, u16len, ISS_TTS_CODEPAGE_UTF16LE);

    TTS_LOGD("SynthEnd txt=[%s],len=%ld", pszText, u16len);
    m_waitStop.Wait();

    while (!OutputTTSData()) {
    }

    m_buffer = NULL;

    if (NULL != u16str) {
        delete[] u16str;
        u16str = NULL;
    }

    return (ISS_SUCCESS == ret);
}

CL_ERROR TTS_EngineKeda::Stop(const std::string& lang)
{
    TTS_LOGD_FUNC();
    if (NULL == m_hTTS) {
        TTS_LOGE("Stop:Keda not init");
        return TTS_ERR_INVALID_OP;
    }

    ISSTTSStop(m_hTTS);

    return TTS_ERR_NO_ERROR;
}


CL_ERROR TTS_EngineKeda::GetIntonation(TtsIntonation* pIntonation) const
{
    return TTS_ERR_NO_ERROR;
}

CL_ERROR TTS_EngineKeda::SetIntonation(TtsIntonation intonation)
{
    return TTS_ERR_NO_ERROR;
}

INT TTS_EngineKeda::GetSampleRate() const
{
    return 16000;
}

int TTS_EngineKeda::TtsInit(const XCHAR *filename)
{

    ISSErrID ret;
    // 1. Load resource and malloc heap
    TTS_LOGD("TtsInit.., %s", filename);

    ret = ISSTTSInitRes(&m_hRes, filename, 0);
    if (ISS_SUCCESS != ret) {
        TTS_LOGD("TtsInit fail. %d.", ret);
        return TTS_ERR_ENGINE_OP_FAILED;
    }

    // 2. Create TTS instance
    ret = ISSTTSCreate(&m_hTTS, m_hRes, OnTTSDataReady, this);
    if (ISS_SUCCESS != ret) {
        TTS_LOGE("ISSTTSCreate, err=%d", ret);
        TtsUninit();
        return ret;
    }

    TTS_LOGD("TtsInit ok");
    return ret;
}

// Free heap and ResPack
void TTS_EngineKeda::TtsUninit()
{

    if (NULL != m_hTTS) {
        ISSTTSDestroy(&m_hTTS);
        m_hTTS = NULL;
    }

    TTS_LOGD("TtsUninit done");
}

bool TTS_EngineKeda::SetParams(const std::string& lang)
{
    for (const auto& param : m_param) {
        if (param.name  == lang) {
            TTS_LOGD("SetParam Lang:%s", lang.c_str());
            return SetParams(param);
        }
    }

    TTS_LOGE("wrong lang %s", lang.c_str());
    return false;
}

void TTS_EngineKeda::ResetEngineParam(const TTS_EngineLanguage& param) 
{
    for (auto& item : m_param) {
        if (item.name == param.name) {
            item = param;
            return;
        }
    }
}

bool TTS_EngineKeda::SetParams(const TTS_EngineLanguage& param)
{
    ISSErrID err;

    ISSTTSSetParam(m_hTTS, ISS_TTS_PARAM_VOICE_PITCH, param.parameters.pitch);
    ISSTTSSetParam(m_hTTS, ISS_TTS_PARAM_VOICE_SPEED, param.parameters.speed);
    ISSTTSSetParam(m_hTTS, ISS_TTS_PARAM_VOLUME, param.parameters.volume);
    err = ISSTTSSetParam(m_hTTS, ISS_TTS_PARAM_SPEAKER, param.roles[param.defaultRole - 1].roleId);

    if (param.name == "ENU") {
        ISSTTSSetParam(m_hTTS, ISS_TTS_PARAM_LANGUAGE, 2);
    }
    else {
        ISSTTSSetParam(m_hTTS, ISS_TTS_PARAM_LANGUAGE, 0);
    }

    TTS_LOGD("SetParams Id=%d, Err=%d", param.roles[param.defaultRole - 1].roleId, err);
    
    return err == ISS_SUCCESS;
}

#ifdef __cplusplus
extern "C"
{
    TTS_Engine* GetInstance()
    {
       return new(MEM_Voice) TTS_EngineKeda();
    }
}
#endif

/* EOF */
