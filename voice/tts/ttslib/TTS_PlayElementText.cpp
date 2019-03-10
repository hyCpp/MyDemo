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
#include "VC_CommDef.h"
#include "TTS_PlayThread.h"
#include "TTS_EngineManager.h"
#include "TTS_PlayElementText.h"
#include "TTS_PlaylistInfo.h"
#include <time.h>

const int TTS_CACHE_BUF_COUNT = 3;     // cache 3 buffers, about playing 0.5s

TTS_PlayElementText::TTS_PlayElementText(TTS_EngineManager* engine, const BL_String &text, const BL_String& sender, const BL_String& language, const BL_String& speakersex)
: m_engine(engine),
  m_text(text),
  m_sender(sender),  
  m_language(language),
  m_speakersex(speakersex)  
{
}

TTS_PlayElementText::~TTS_PlayElementText()
{
}


int TTS_PlayElementText::GetSampleRate() const
{
    assert(NULL != m_engine);
    return m_engine->GetSampleRate();
} 

CL_BOOL TTS_PlayElementText::DoPlay(TTS_EngineBuffers* buffer)
{
    MakeTimeTag("DoPlay");
    TTS_LOGI("tts DoPlay...");
    if (!buffer) {
        TTS_LOGE("buffer is NULL");
        return CL_FALSE;
    }

    if (!m_engine) {
        TTS_LOGE("engine is NULL");
        return CL_FALSE;
    }
    
    return m_engine->SynthesizeText(m_text, m_sender.GetString(), m_language.GetString(), m_speakersex.GetString(), buffer);
}

/* EOF */
