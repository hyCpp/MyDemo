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
/**
 * @file TTS_EngineiFlytek
 * @brief Declaration file of class TTS_EngineiFlytek.
 *
 * This file includes the declaration of test class TTS_EngineiFlytek and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#include <string>
#include <boost/format.hpp>

#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

#include "tinyxml.h"
#include "BL_File.h"

#include "stdafx.h"
#include "TTS_EngineiFlytek.h"

using namespace std;

static const char* TTS_IFLYTEK_WORK_DIR = "TTS/iFlytek";

TTS_EngineiFlytek::TTS_EngineiFlytek()
    : m_buffer(NULL)
    , m_sessionID(NULL)
{
    TTS_LOGD_FUNC();
}

TTS_EngineiFlytek::~TTS_EngineiFlytek()
{
    TTS_LOGD_FUNC();
}

bool TTS_EngineiFlytek::Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo)
{
    ttsInit();
    return true;
}

CL_ERROR TTS_EngineiFlytek::Uninit()
{
    ttsUninit();
    return CL_ERROR_NO_ERROR;
}

bool TTS_EngineiFlytek::LoadEngine(const TTS_EngineInfo& engineInfo)
{
    return true;
}

bool TTS_EngineiFlytek::ReLoadEngine(const TTS_EngineInfo& engineInfo)
{
    return true;
}

bool TTS_EngineiFlytek::SynthesizeText(const XCHAR* pszText, const std::string& module,
                                       const std::string& lang, const std::string& sex, TTS_EngineBuffers* buffer)
{
    TTS_LOGD_FUNC();
    TTS_LOGD("pszText = [%s], module: [%s], lang: [%s], sex: [%s]", pszText, module.c_str(), lang.c_str(), sex.c_str());
    if (NULL == pszText || NULL == buffer) {
        TTS_LOGD("parameter error");
        return false;
    }

    int ret = MSP_SUCCESS;
    const char* session_params = "engine_type = local, text_encoding = UTF8,"\
                                 "tts_res_path = fo|res/tts/xiaoyan.jet;fo|res/tts/common.jet,"\
                                 "sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";

    // synthesize begin
    m_sessionID = QTTSSessionBegin(session_params, &ret);
    if (MSP_SUCCESS != ret || NULL == m_sessionID) {
        TTS_LOGD("QTTSSessionBegin failed, error code: %d.", ret);
        return false;
    }

    // handle xml document
    const char* text = pszText;
    TiXmlDocument doc;
    doc.Parse(pszText);
    if (!doc.Error()) {
        const TiXmlElement* rootNode = doc.RootElement();
        if (NULL == rootNode) {
            text = pszText;
        }
        text = rootNode->GetText();
    }

    TTS_LOGD("[TEST]: text = %s", text);
    ret = QTTSTextPut(m_sessionID, text, (unsigned int)strlen(text), NULL);
    if (MSP_SUCCESS != ret) {
        TTS_LOGD("QTTSTextPut failed, error code: %d.", ret);
        return false;
    }

    m_buffer = buffer;
    // synthesize wav buffer and save wave buffer data
    while (OutputTTSData(ret)) {
    }

    if (MSP_SUCCESS != ret) {
        TTS_LOGD("QTTSAudioGet failed, error code: %d.", ret);
    }

    ret = QTTSSessionEnd(m_sessionID, "Normal");
    if (MSP_SUCCESS != ret) {
        TTS_LOGD("QTTSSessionEnd failed, error code: %d.", ret);
        return false;
    }

    return true;
}

CL_ERROR TTS_EngineiFlytek::Stop(const std::string& lang)
{
    return CL_ERROR_NO_ERROR;
}

CL_ERROR TTS_EngineiFlytek::GetIntonation(TtsIntonation* pIntonation) const
{
    return CL_ERROR_NO_ERROR;
}

CL_ERROR TTS_EngineiFlytek::SetIntonation(TtsIntonation intonation)
{
    return CL_ERROR_NO_ERROR;
}

INT TTS_EngineiFlytek::GetSampleRate() const
{
    return 16000;
}

void TTS_EngineiFlytek::ResetEngineParam(const TTS_EngineLanguage& param)
{
    return;
}

bool TTS_EngineiFlytek::ttsInit()
{
    TTS_LOGD_FUNC();

    int ret = MSP_SUCCESS;
    BL_String workDir;
    BL_String ttsDir(TTS_IFLYTEK_WORK_DIR);
    BL_File file(BL_FILE_PREFIX_NDATA);

    if (!file.GetLogicalPath(ttsDir, workDir)) {
        TTS_LOGE("Init full path fail");
        return false;
    }

    TTS_LOGD("workDir:[%s]", workDir.GetString());
    const char* format = "appid = 590fe493, work_dir = %1%";

    string param = (boost::format(format) % workDir.GetString()).str();

    ret = MSPLogin(NULL, NULL, param.c_str());
    if (MSP_SUCCESS != ret) {
        TTS_LOGD("MSPLogin failed, error code: %d.", ret);
        return false;
    }

    return true;
}

void TTS_EngineiFlytek::ttsUninit()
{
    TTS_LOGD_FUNC();
    MSPLogout();

    if (NULL == m_sessionID) {
        return;
    }
}

bool TTS_EngineiFlytek::OutputTTSData(int &res)
{
    unsigned int audio_len = 0;
    int                ret = MSP_SUCCESS;
    int       synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

    const void* data = QTTSAudioGet(m_sessionID, &audio_len, &synth_status, &ret);
    if (MSP_SUCCESS != ret) {
        res = MSP_ERROR_FAIL;
        return false;
    }

    // case audio len is 0 is normal
    if (0 == audio_len) {
        return true;
    }

    // write wav data to buffer
    int pos = audio_len;
    do {
        int len = 0;
        int write_buffer_len = 0;
        BYTE* buf = m_buffer->GetWriteBuffer(len);
        if (NULL == buf || 0 == len) {
            res = MSP_ERROR_FAIL;
            return false;
        }

        if (pos >= len) {
            write_buffer_len = len;
            pos = pos - len;
        }
        else {
            write_buffer_len = pos;
            pos = 0;
        }

        memcpy(buf, data, write_buffer_len);
        m_buffer->SetWriteLen(write_buffer_len);
    } while (0 != pos);

    if (MSP_TTS_FLAG_DATA_END == synth_status) {
        res = MSP_SUCCESS;
        return false;
    }

    return true;
}

extern "C" {
    // iflytek engine instance
    __attribute__((visibility("default"))) TTS_Engine* GetInstance()
    {
        return new TTS_EngineiFlytek();
    }
};

/* EOF */
