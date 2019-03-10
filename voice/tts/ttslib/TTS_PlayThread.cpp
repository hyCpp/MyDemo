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
#include "TTS_Controler.h"
#include "TTS_PlaylistInfo.h"
#include "VC_CommonIF.h"
#include "TTS_EngineManager.h"
#include "TTS_Event.h"
#include "TTS_PlayThread.h"
#include "VC_CommonIF.h"
#include "VC_CommonDef.h"
#include "AplThreadName.h"

#include "TTS_AudioOutThread.h"

TTS_PlayThread::TTS_PlayThread() : m_stop(false), m_stopType(TTS_PLAYER_STOPED), m_playlist(NULL)
{
}

TTS_PlayThread::~TTS_PlayThread()
{
    TTS_LOGD_FUNC();
}


void TTS_PlayThread::Start()
{
    RegisterName(TTS_PLAY_THREAD);
    StartRegistThread();

    m_audioThread.StartAudioThread();
}

void TTS_PlayThread::Stop()
{
    TTS_LOGI("tts system stop wait...");
    StopPlayThread();
    TTS_LOGI("tts system stop end..."); 
}

void TTS_PlayThread::Run()
{
    while (1) {
        Wait();
        if (TRUE == CheckQuit()) {
            break;
        }
        Play();
    }

}

void TTS_PlayThread::PlayReq(TTS_PlaylistInfo* playlist)
{
    assert(m_playlist == NULL);
    TTS_LOGI("Notify.");
    m_playlist = playlist;
    m_stopType = TTS_PLAYER_STOPED;
    m_stop = false;
    m_buffer.Clear();
    Notify();
}

void TTS_PlayThread::StopPlay(TTS_PLAYER_STATE stopType)
{
    SynchronizeStart();
    if (m_stop) {
        return;
    }
    m_stopType = stopType;

    m_buffer.Stop();
    // m_audioThread.Flush();

    SynchronizeEnd();
}


void TTS_PlayThread::Play()
{
    TTS_LOGD("start ...");
    int j = m_playlist->GetCurrentPlay();

    m_playlist->SendPlayState(TTS_PLAYER_START, j);

    CL_List<TTS_PlayElementBase>& elementList =  m_playlist->GetPlayElementList();

    TTS_STREAM_DEVICE_TYPE stream = m_playlist->GetStream();
    int tokenid = m_playlist->GetTokenID();

    for (; j < elementList.Length(); ++j) {
        TTS_LOGD("element play,j=%d", j);
        TTS_PlayElementBase* elem = elementList.Nth(j); // get the frist element that need to play

        if (elem == NULL) {
            m_stopType = TTS_PLAYER_ERROR;
            break;
        }

        m_playlist->SendPlayState(TTS_PLAYER_PHRASE_FINISH, j);
        m_playlist->SetCurrentPlay(j);
   
        m_buffer.NewPlayer();
       
        bool drainWait = (j == elementList.Length() -1) && m_playlist->IsDrainWait(); 

        if (!m_audioThread.Play(&m_buffer, elem->GetSampleRate(), elem->GetChannel(), stream, tokenid, drainWait)) {
            m_stopType = TTS_PLAYER_ERROR;
            break;
        }
        if (!elem->DoPlay(&m_buffer)) {
            TTS_LOGI("element play end");
            m_stopType = TTS_PLAYER_ERROR;
        } // add the playover and send the j.

        m_buffer.FinishPhrase();
        m_audioThread.JoinPlayer();

        if (m_buffer.IsStop()) {
            break;
        }

        TTS_LOGD("element play end,j=%d", j);
    }


    SynchronizeStart();
    m_stop = true;
    m_playlist->SendPlayEnd(m_stopType);
    SynchronizeEnd();


    TTS_LOGD("DoPlay end...");
}


void TTS_PlayThread::StopPlayThread()
{
    StopThread();
}

void TTS_PlayThread::Pause(TTS_PlaylistInfo *playlist)
{
    StopPlay(TTS_PLAYER_PAUSE);
}

void TTS_PlayThread::Resume(TTS_PlaylistInfo *playlist)
{
}
/* EOF */
