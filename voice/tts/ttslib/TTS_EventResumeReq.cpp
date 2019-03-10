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
#include "TTS_PlayThread.h"
#include "TTS_PlaylistInfoManager.h"
#include "TTS_EventResumeReq.h"

TTS_EventResumeReq::TTS_EventResumeReq(TTS_PlayThread* thread, TTS_TextToSpeechImpl::PlaybackListener* listener,
       TTS_PlaylistInfoManager* manager, int reqID, const BL_String &sender)
: m_playThread(thread),
  m_listener(listener),
  m_manager(manager),  
  m_reqID(reqID), 
  m_sender(sender)
{

}

TTS_EventResumeReq::~TTS_EventResumeReq()
{

}

void TTS_EventResumeReq::OnEvent()
{
    TTS_LOGD("Resume,send=%s,Id=%d", m_sender.GetString(), m_reqID);
    Resume();
}

void TTS_EventResumeReq::Resume()
{
    TTS_PlaylistInfo *playlist = m_manager->TakePauseList(m_sender, m_reqID); 

    if (NULL == playlist) {
        m_listener->OnPlayStateOver(m_sender, m_reqID, TTS_PLAYER_ERROR_ID, -1);
        return;
    }

    if (m_manager->CanPlayNow()) {
        m_listener->OnPlayStateOver(m_sender, m_reqID, TTS_PLAYER_RESUME, playlist->GetCurrentPlay());
        m_playThread->PlayReq(playlist);
        m_manager->SetCurrPlaylist(playlist);
        return;
    }

    m_manager->AddPlayList(playlist);
    TTS_LOGI("Id=%d,send=%s", m_reqID, m_sender.GetString());
}

/* EOF */
