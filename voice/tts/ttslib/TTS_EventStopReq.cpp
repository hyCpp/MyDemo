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
#include "TTS_EventStopReq.h"
#include "TTS_PlaylistInfoManager.h"


TTS_EventStopReq::TTS_EventStopReq(TTS_PlayThread *thread, TTS_PlaylistInfoManager *manager, 
        TTS_TextToSpeechImpl::PlaybackListener *listener, int reqID, const BL_String &sender, CL_BOOL stopAll)
: m_playThread(thread),
  m_manager(manager),  
  m_listener(listener),
  m_reqID(reqID), 
  m_sender(sender), 
  m_stopAll(stopAll)
{

}

TTS_EventStopReq::~TTS_EventStopReq()
{

}

void TTS_EventStopReq::OnEvent()
{
    TTS_LOGD("Stop,send=%s,Id=%d", m_sender.GetString(), m_reqID);

    if (m_stopAll) {
        StopAll();
    }
    else {
        Stop();
    }
    
}

void TTS_EventStopReq::Stop()
{
    TTS_PlaylistInfo *curPlaylist = m_manager->GetCurrPlaylist();
    
    if (curPlaylist) {
        if (curPlaylist->GetSender() == m_sender) {
            if (curPlaylist->GetRequestId() == m_reqID) {  
                m_playThread->StopPlay(TTS_PLAYER_ABORT);
                return;
            }
        }
    }

    m_manager->RemovePlayAndPause(m_sender, m_reqID);

    m_listener->OnPlayStateOver(m_sender, m_reqID, TTS_PLAYER_ABORT, -1);
}

void TTS_EventStopReq::StopAll()
{
    TTS_PlaylistInfo *curPlaylist = m_manager->GetCurrPlaylist();
    
    if (curPlaylist) {
        if (curPlaylist->GetSender() == m_sender) {
            m_playThread->StopPlay(TTS_PLAYER_ABORT);
        }
    }

    m_manager->RemovePlayAndPause(m_sender);

    m_listener->OnPlayStateOver(m_sender, -1, TTS_PLAYER_ABORT, -1);
}


/* EOF */
