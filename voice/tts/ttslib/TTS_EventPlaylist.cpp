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
#include "TTS_EventPlaylist.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_PlaylistInfoManager.h"


TTS_EventPlaylist::TTS_EventPlaylist(TTS_PlayThread *thread, TTS_PlaylistInfo *playlist,
                 TTS_PlaylistInfoManager* manager, TTS_TextToSpeechImpl::PlaybackListener * listener)
: m_thread(thread),
  m_playlist(playlist),
  m_manager(manager),
  m_listener(listener)  
{

}

TTS_EventPlaylist::~TTS_EventPlaylist()
{
    if (NULL != m_playlist) {
        delete m_playlist;
        m_playlist = NULL;
    }
}

void TTS_EventPlaylist::OnEvent()
{
    assert(NULL != m_manager && NULL != m_manager && NULL != m_playlist && NULL != m_listener);

    Play();
}

void TTS_EventPlaylist::Play()
{
    TTS_LOGI("Id=%d,send=%s", m_playlist->GetRequestId(), m_playlist->GetSender().GetString());

    if (m_manager->CanPlayNow()) {
        m_manager->SetCurrPlaylist(m_playlist);
        m_thread->PlayReq(m_playlist);
        m_playlist = NULL;
        return; 
    }

    if (m_playlist->GetPolicy().queuePolicy == TTS_OP_QUEUE_ADD_T) {
        m_manager->AddPlayList(m_playlist);
        m_playlist = NULL;
    }
    else {
        TTS_LOGI("abort");
        m_listener->OnPlayStateOver(m_playlist->GetSender(), m_playlist->GetRequestId(), TTS_PLAYER_ABORT, -1);
    }
}


/* EOF */
