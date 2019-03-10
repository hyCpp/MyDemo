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
#include "TTS_IF.h"
#include "TTS_Common.h"
#include "TTS_PlayThread.h"
#include "TTS_EventPlayEnd.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_PlaylistInfoManager.h"


TTS_EventPlayEnd::TTS_EventPlayEnd(TTS_PlayThread *thread, TTS_PlaylistInfo *playlist,
                 TTS_PlaylistInfoManager* manager, TTS_TextToSpeechImpl::PlaybackListener *listener, TTS_PLAYER_STATE status)
: m_thread(thread),
  m_playlist(playlist),
  m_manager(manager),
  m_listener(listener),
  m_status(status)  
{

}

TTS_EventPlayEnd::~TTS_EventPlayEnd()
{
}

void TTS_EventPlayEnd::OnEvent()
{
    assert(NULL != m_manager && NULL != m_manager && NULL != m_playlist && NULL != m_listener);

    PlayEnd();
}

void TTS_EventPlayEnd::PlayEnd()
{
    TTS_LOGI("Play end");

    // TTS_LOGI("Play End, reqId=%d, sender=%s, status is %d", m_playlist->GetRequestId(), m_playlist->GetSender().GetString(), m_status);

    TTS_PlaylistInfo* currPlaylist = m_manager->GetCurrPlaylist();

    if (!m_playlist || m_playlist != currPlaylist) {
        if (m_playlist != NULL) {
            m_listener->OnPlayStateOver(m_playlist->GetSender(), m_playlist->GetRequestId(), m_status, -1);
        }
        TTS_LOGE("m_playlist null or equal");
        return;
    }


    m_listener->OnPlayStateOver(m_playlist->GetSender(), m_playlist->GetRequestId(), m_status, m_playlist->GetCurrentPlay());

    if (m_status >= TTS_PLAYER_STOPED) {
        int playResult = m_status - TTS_PLAYER_STOPED;
        m_listener->OnTtsSpeechCompleted(m_playlist->GetSender(), m_playlist->GetRequestId(), TTS_PLAY_RESULT(playResult));
    }

    if (m_status == TTS_PLAYER_PAUSE) {
        m_manager->AddPauseList(m_playlist);
    }
    else {
        delete m_playlist;
        m_playlist = NULL;
    }

    m_manager->SetCurrPlaylist(NULL);

    TTS_PlaylistInfo* playlist =  m_manager->GetFirstPlayList();

    if (NULL != playlist) {
        m_thread->PlayReq(playlist);
        m_manager->SetCurrPlaylist(playlist);
    }
}


/* EOF */

