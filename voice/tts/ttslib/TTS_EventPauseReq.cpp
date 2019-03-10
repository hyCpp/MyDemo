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
#include "TTS_EventPauseReq.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_PlaylistInfoManager.h"

TTS_EventPauseReq::TTS_EventPauseReq(TTS_PlayThread* thread, TTS_TextToSpeechImpl::PlaybackListener* listener,
       TTS_PlaylistInfoManager* manager, int reqID, const BL_String &sender)
: m_playThread(thread),
  m_listener(listener),
  m_manager(manager),  
  m_reqID(reqID), 
  m_sender(sender)
{

}

TTS_EventPauseReq::~TTS_EventPauseReq()
{

}

void TTS_EventPauseReq::OnEvent()
{
    TTS_LOGD("Pause send=%s,Id=%d", m_sender.GetString(), m_reqID);
    Pause();
}

void TTS_EventPauseReq::Pause()
{
    TTS_LOGI("Pause Id=%d,send=%s", m_reqID, m_sender.GetString());

    TTS_PlaylistInfo *curPlaylist = m_manager->GetCurrPlaylist();

    if (curPlaylist != NULL && curPlaylist->GetRequestId() == m_reqID && curPlaylist->GetSender() == m_sender) {
        TTS_LOGI("PauseReq in play");
        m_playThread->Pause(curPlaylist);  
        return;
    }

    TTS_PlaylistInfo* playlist = m_manager->TakePlayOrPauseList(m_sender, m_reqID);

    if (NULL != playlist) {
        m_manager->AddPauseList(playlist);
        m_listener->OnPlayStateOver(m_sender, m_reqID, TTS_PLAYER_PAUSE, -1);
        return;
    }
    else {
        m_listener->OnPlayStateOver(m_sender, m_reqID, TTS_PLAYER_ERROR_ID, -1);
    }

}

/* EOF */
