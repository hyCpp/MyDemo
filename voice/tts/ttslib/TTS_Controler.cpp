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
#include "TTS_Controler.h"
#include "TTS_Common.h"
#include "AplThreadName.h"
#include "TTS_EngineManager.h"
#include "TTS_PlaylistInfo.h"
#include "VC_CommonIF.h"
#include "TTS_EventStopReq.h" 
// #include "TTS_TuningManager.h"

using namespace std;


TTS_Controler::TTS_Controler()
{
    // m_tuningManager = new(MEM_Voice) TTS_TuningManager();
}

TTS_Controler::~TTS_Controler()
{
    // delete m_tuningManager;
}

void TTS_Controler::Initialize()
{
}

void TTS_Controler::Start()
{
    StartThreads();
}

void TTS_Controler::Stop()
{
   /* m_systemStopLock.SyncStart();
    m_systemStop = CL_TRUE;
    m_systemStopLock.SyncEnd();
    TTS_EventStopReq *event = new(MEM_Voice) TTS_EventStopReq(this, 0, SENDER_STOPCURRENT, CL_FALSE);
    OnEvent(event);
    m_systemStopWait.Reset();
    TTS_LOGI("tts system stop wait start...");
    m_systemStopWait.Wait(4000);    // max wait 4s 
    */
    TTS_LOGI("tts system stop wait...");
    StopThreads();
    TTS_LOGI("tts system stop end..."); 
}

void TTS_Controler::Destory()
{
}

void TTS_Controler::StartThreads()
{
    RegisterName(TTS_PLAYMGR_THREAD);
    StartRegistThread();

}

void TTS_Controler::StopThreads()
{
    // stop myself thread
    StopThread();
}

#if 0
CL_ERROR TTS_Controler::TuningData(const BL_String &sender, TTS_TuningData* tuningData)
{
    if ("" != tuningData->replaceFileFullPath) {
        return m_tuningManager->LoadFile(tuningData->replaceFileFullPath.GetString(), sender.GetString());
    }  // APDB file path ---
    return TTS_REGISTER_RESULT_ERROR;
}

#endif

CL_ERROR TTS_Controler::RegisterSound(const BL_String &sender, const BL_String& soundFileName, int soundId)
{
    string senderIn = sender.GetString();
    m_lock.SyncStart();
    SenderFileName_t::iterator it = m_registerredSounds.find(senderIn);
    if (m_registerredSounds.end() == it) {
        IdFileName_t* sounds = new(MEM_Voice) IdFileName_t();
        if (!sounds) {
            m_lock.SyncEnd();
            TTS_LOGE("new map failed");
            return TTS_ERROR_NO_RESOURCE;
        }

        sounds->insert(make_pair(soundId, soundFileName));
        m_registerredSounds.insert(make_pair(senderIn, sounds));
        m_lock.SyncEnd();
        return TTS_ERROR_NO_ERROR;
    }

    it->second->insert(make_pair(soundId, soundFileName));
    m_lock.SyncEnd();
    return TTS_ERROR_NO_ERROR;
}

CL_ERROR TTS_Controler::UnRegisterSound(const BL_String &sender, int soundId)
{
    string senderIn = sender.GetString();
    m_lock.SyncStart();
    SenderFileName_t::iterator it = m_registerredSounds.find(senderIn);
    if (m_registerredSounds.end() == it) {
        m_lock.SyncEnd();
        return TTS_ERROR_NO_ERROR;
    }

    IdFileName_t::iterator subIt = it->second->find(soundId);
    if (it->second->end() != subIt) {
        it->second->erase(subIt);
    }
    m_lock.SyncEnd();

    return TTS_ERROR_NO_ERROR;
}

BL_String TTS_Controler::GetSoundName(const BL_String &sender, int soundID)
{
    string senderIn = sender.GetString();
    m_lock.SyncStart();
    TTS_LOGD("map size=%d", m_registerredSounds.size());
    SenderFileName_t::const_iterator it = m_registerredSounds.find(senderIn);
    if (m_registerredSounds.end() == it) {
        m_lock.SyncEnd();
        return "";
    }

    IdFileName_t::iterator subIt = it->second->find(soundID);
    if (it->second->end() == subIt) {
        m_lock.SyncEnd();
        return "";
    }
    m_lock.SyncEnd();

    return subIt->second;
}

/* EOF */
