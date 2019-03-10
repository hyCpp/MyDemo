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
#include "TTS_EngineCommDef.h"
#include "TTS_EngineFlite.h"

using namespace std;

const string TTS_EngineFlite::TTS_FLITE_DATA_DIR = "/Flite/";
const string TTS_EngineFlite::TTS_FLITE_AEW_VOX = "cmu_us_clb.flitevox";

TTS_EngineFlite::TTS_EngineFlite()
    : m_sampleRate(0)
    , m_buffer(nullptr)
    , m_state(ST_NONE)
    , m_pVoice(nullptr)
{
}

TTS_EngineFlite::~TTS_EngineFlite()
{
}

bool TTS_EngineFlite::Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo)
{
    flite_init();

    flite_add_lang("eng", usenglish_init, cmulex_init);
    flite_add_lang("usenglish", usenglish_init, cmulex_init);

    flite_voice_list = cons_val(voice_val(register_cmu_us_rms(NULL)), flite_voice_list);
    flite_voice_list = val_reverse(flite_voice_list);

    m_userDataDir = conf;

    return true;
}

bool TTS_EngineFlite::LoadEngine(const TTS_EngineInfo &engineInfo)
{
    string dataPath = m_userDataDir + TTS_FLITE_DATA_DIR + TTS_FLITE_AEW_VOX;
    string uri = "file://" + dataPath;

    m_pVoice = flite_voice_select(uri.c_str());

    if (nullptr == m_pVoice) {
        TTS_LOGE("Can't load voice file [%s]", dataPath.c_str());
        return false;
    }

    return true;
}

bool TTS_EngineFlite::ReLoadEngine(const TTS_EngineInfo &engineInfo)
{
    return true;
}

CL_ERROR TTS_EngineFlite::Uninit()
{
    return TTS_ERR_NO_ERROR;
}

bool TTS_EngineFlite::SynthesizeText(const XCHAR *pszText, const string &module,
                                     const string &lang, const string& sex, TTS_EngineBuffers *buffer)
{
    TTS_LOGI("lang: %s", lang.c_str());
    if (NULL == m_pVoice) {
        TTS_LOGE("Flite engine is not initialized correctly!");
        return false;
    }

    if (NULL == pszText || NULL == buffer) {
        TTS_LOGE("SynthesizeText null param");
        return false;
    }

    m_buffer = buffer;

    cst_utterance* u = flite_synth_text(pszText, m_pVoice);
    cst_wave *w = utt_wave(u);

    m_sampleRate = w->sample_rate;
    TTS_LOGI("Sample Rate is: [%d]", m_sampleRate);

    int totalLen = w->num_samples * w->num_channels;
    totalLen <<= 1;
    TTS_LOGI("Total length is: [%d]", totalLen);

    int len = 0, curPos = 0;

    while(curPos < totalLen) {
        BYTE* buf = m_buffer->GetWriteBuffer(len);

        TTS_LOGI("Buffer is [%p]", buf);

        if (nullptr == buf || len <= 0) {
            return false;
        }

        // find a proper length of buffer to store content
        len = totalLen - curPos > len ? len : totalLen - curPos;
        TTS_LOGI("Allocate [%d] bytes to play", len);
        memcpy(buf, reinterpret_cast<BYTE*>(w->samples) + curPos, len);
        m_buffer->SetWriteLen(len);

        curPos += len;
    }

    return true;
}

CL_ERROR TTS_EngineFlite::Stop(const string &lang)
{
    return TTS_ERR_NO_ERROR;
}

CL_ERROR TTS_EngineFlite::GetIntonation(TtsIntonation *pIntonation) const
{
    return TTS_ERR_NO_ERROR;
}

CL_ERROR TTS_EngineFlite::SetIntonation(TtsIntonation intonation)
{
    return TTS_ERR_NO_ERROR;
}

int TTS_EngineFlite::GetSampleRate() const
{
    return m_sampleRate == 0 ? 16000 : m_sampleRate;
}

void TTS_EngineFlite::ResetEngineParam(const TTS_EngineLanguage &param)
{
    return;
}

#ifdef __cplusplus
extern "C"
{
    __attribute__((visibility("default"))) TTS_Engine* GetInstance()
    {
       return new(MEM_Voice) TTS_EngineFlite();
    }
}
#endif
