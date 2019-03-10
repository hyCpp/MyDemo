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
#include "TTS_Common.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_PlayElementText.h"
#include "TTS_PlayElementSoundName.h"
#include "TTS_Controler.h"
#include "TTS_PlayElementSilence.h"

TTS_PlaylistInfo::TTS_PlaylistInfo(const BL_String &sender, TTS_TextToSpeechImpl* owner)
: m_playElementList(),
  m_reqID(0), 
  m_sender(sender), 
  m_currentElem(0),
  m_owner(owner),
  m_policy(TTS_POLICY()),
  m_stream(TTS_STREAM_DEVICE_OUTPUT_INTERRUPT),
//  m_tuningManager(NULL),
  m_tokenID(0),
  m_drainWait(true)  
{
}

TTS_PlaylistInfo::TTS_PlaylistInfo(const BL_String &sender, const TTS_POLICY &policy, TTS_TextToSpeechImpl* owner)
: m_playElementList(),
  m_reqID(0), 
  m_sender(sender), 
  m_currentElem(0),
  m_owner(owner),
  m_policy(policy),
  m_stream(TTS_STREAM_DEVICE_OUTPUT_INTERRUPT),
//  m_tuningManager(NULL),
  m_tokenID(0),
  m_drainWait(true)  
{
}

TTS_PlaylistInfo::~TTS_PlaylistInfo()
{
    int count = m_playElementList.Length();
    for (int i = 0; i < count; ++i) {
        if (m_playElementList.Nth(i)) {
            delete m_playElementList.Nth(i);
        }
    }

    m_playElementList.Clear();

}

CL_ERROR TTS_PlaylistInfo::AddText(const BL_String &text, const BL_String& lang, const BL_String& speakersex)
{ 
    TTS_LOGI("lang : %s, spearsex : %s, text : %s", lang.GetString(), speakersex.GetString(), text.GetString());
    if (m_owner == NULL) {
        return TTS_ERROR_PARAM;
    }

    BL_String language = lang;
    if (lang.GetLength() == 0) {
        language = m_owner->GetDefaultLanguage();
    }

    TTS_EngineManager* engine = m_owner->GetEngineManager();

    if (engine == NULL) {
        return TTS_ERROR_LANG_ERROR;
    }

    TTS_PlayElementText *element = new(MEM_Voice) TTS_PlayElementText(engine, text, m_sender, language, speakersex);
    if (!element) {
        return TTS_ERROR_NO_RESOURCE;
    }
    
    m_playElementList.Append(element);
    return TTS_ERROR_NO_ERROR;
}

/* CL_ERROR TTS_PlaylistInfo::AddText(const BL_String& text, const BL_String& lang, const BL_String &filePath)
{
    TTS_PlayElementText *element = new(MEM_Voice) TTS_PlayElementText(text, m_sender, lang, filePath);
    if (!element) {
        return TTS_ERROR_NO_RESOURCE;
    }
    
    m_playElementList.Append(element);
    return TTS_ERROR_NO_ERROR;
} */

CL_ERROR TTS_PlaylistInfo::AddSound(const BL_String &soundFileName)
{
    TTS_LOGI("file : %s", soundFileName.GetString());

    TTS_PlayElementSoundName *element = new(MEM_Voice) TTS_PlayElementSoundName();
    if (!element) {
        return TTS_ERROR_NO_RESOURCE;
    }

    element->SetSoundName(soundFileName);
    m_playElementList.Append(element);

    return TTS_ERROR_NO_ERROR;
}

 CL_ERROR TTS_PlaylistInfo::AddSound(int soundID)
 {
    TTS_LOGI("id : %d", soundID);
    if (NULL == m_owner) {
        return TTS_ERROR_NO_RESOURCE;
    }
    TTS_PlayElementSoundName *element = new(MEM_Voice) TTS_PlayElementSoundName();
    if (!element) {
          return TTS_ERROR_NO_RESOURCE;
    }
  
    const BL_String soundName = m_owner->GetSoundName(m_sender, soundID);
    element->SetSoundName(soundName);
    m_playElementList.Append(element);
  
    return TTS_ERROR_NO_ERROR;
}


CL_ERROR TTS_PlaylistInfo::AddSilence(int millisecond)
{
    TTS_LOGI("second : %d", millisecond);

    TTS_PlayElementSilence *element = new(MEM_Voice) TTS_PlayElementSilence();
    if (!element) {
        return TTS_ERROR_NO_RESOURCE;
    }

    element->SetSilenceTime(millisecond);
    m_playElementList.Append(element);

    return TTS_ERROR_NO_ERROR;
}

void TTS_PlaylistInfo::SendPlayEnd(TTS_PLAYER_STATE state)
{
    if (m_owner) {
        m_owner->OnPlayEnd(this, state);
    }
}

void TTS_PlaylistInfo::SendPlayState(TTS_PLAYER_STATE state, int playNum)
{
    if (m_owner) {
        m_owner->OnPlayState(m_reqID, m_sender, state, playNum);
    }
}
/* EOF */
