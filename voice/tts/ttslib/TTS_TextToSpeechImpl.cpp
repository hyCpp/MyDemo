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
#include "TTS_EventPlaylist.h"
#include "TTS_TextToSpeechImpl.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_Controler.h"
#include "TTS_EventStopReq.h"
#include "TTS_EngineManager.h"
#include "CL_AutoSync.h"
#include "TTS_Controler.h"
#include "TTS_PlayThread.h"
#include "TTS_EventPauseReq.h"
#include "TTS_EventResumeReq.h"
// #include "TTS_EventRegisterTuningDataReq.h"
#include "TTS_EventPlayEnd.h"

#ifdef HAVE_NUTSHELL_OS
#include <system/NCSystemDefine.h>
#include <system/PfServiceDefine.h>
#include <cutils/properties.h>
using namespace nutshell;
#endif

using namespace std;


int TTS_TextToSpeechImpl::m_uRequestID = 0;

TTS_TextToSpeechImpl::TTS_TextToSpeechImpl(PlaybackListener* listener)
: m_control(new(MEM_Voice) TTS_Controler()),
  m_playThread(new(MEM_Voice) TTS_PlayThread),
  m_engineManager(new(MEM_Voice) TTS_EngineManager),  
  m_listener(listener)
{

}

TTS_TextToSpeechImpl::~TTS_TextToSpeechImpl()
{
    TTS_LOGD_FUNC();
    CL_AutoSync lock(m_lock);

    if (m_control != NULL) {
        delete m_control;
        m_control = NULL;
    }

    if (m_playThread != NULL) {
        delete m_playThread;
        m_playThread = NULL;
    }

    if (m_engineManager != NULL) {
        delete m_engineManager;
        m_engineManager = NULL;
    }
}

void TTS_TextToSpeechImpl::PreInitialize()
{
    if (m_control) {
        m_control->Start();
    }

    if (m_playThread) {
        m_playThread->Start();
    }
}

void TTS_TextToSpeechImpl::Initialize()
{

    m_engineManager->Initialize();

}

void TTS_TextToSpeechImpl::LoadEngine()
{
    TTS_LOGI("LoadEngine begin.");

    if (m_engineManager == NULL) {
        return;
    }

    m_engineManager->LoadEngine();
    
    TTS_LOGI("Load engine finish.");
}

void TTS_TextToSpeechImpl::ReLoadEngine()
{
    TTS_LOGI("LoadEngine begin.");
    
    CL_AutoSync lock(m_lock);

    if (m_engineManager == NULL) {
        return;
    }

    if (m_engineManager->IsLoadedEngine()) {
        m_engineManager->ReLoadEngine();
    }
    
    TTS_LOGI("ReLoad engine finish.");
}

void TTS_TextToSpeechImpl::Start()
{
    LoadEngine();
}

void TTS_TextToSpeechImpl::Stop()
{

    if (m_control) {
        m_control->Stop();
    }

    if (m_playThread) {
        m_playThread->Stop();
    }
}

void TTS_TextToSpeechImpl::Destory()
{
}

VOID TTS_TextToSpeechImpl::ListRole(TtsEngineRoles& roles)
{
     m_engineManager->ListRole(roles);
}

#if 0
CL_BOOL TTS_TextToSpeechImpl::GetDefaultRole(TTS_RoleInfo *role)
{
    return m_engineManager->GetDefaultRole(role);
}


void TTS_TextToSpeechImpl::ListParamRange(TTS_ParamRange** params, int* count)
{
    TTS_LOGD_FUNC();
    if (m_engineManager) {
        m_engineManager->ListParamRange(params, count);
    }
}

#endif

#if 0
void TTS_TextToSpeechImpl::InitTtsProperty(TTS_TextToSpeechProperty *property, const BL_String& lang)
{
    property->SetLanguage(lang);

    int roleId = m_engineManager->GetDefaultParameter(lang, TTS_PARAM_ROLE);
    property->SetParam(TTS_PARAM_ROLE, roleId);

    int speed = m_engineManager->GetDefaultParameter(lang, TTS_PARAM_SPEED);
    property->SetParam(TTS_PARAM_SPEED, speed);

    int pitch = m_engineManager->GetDefaultParameter(lang, TTS_PARAM_PITCH);
    property->SetParam(TTS_PARAM_PITCH, pitch);

    int volume = m_engineManager->GetDefaultParameter(lang, TTS_PARAM_VOLUME);
    property->SetParam(TTS_PARAM_VOLUME, volume);
}

#endif

bool TTS_TextToSpeechImpl::GetParameter(const std::string& sender, const std::string& language, TTS_EngineLanguage& param)
{
    TTS_LOGD("sender=%s", sender.c_str());
    return m_engineManager->GetParameter(sender, language, param);
}

bool TTS_TextToSpeechImpl::SetParameter(const std::string& sender, const std::string& language, const TTS_EngineLanguage& param)
{
    TTS_LOGD("sender=%s", sender.c_str());
    m_lock.SyncStart();
    bool ret = m_engineManager->SetParameter(sender, language, param);
    m_lock.SyncEnd();

    return ret;
}

#if 0
TTS_Engine* TTS_TextToSpeechImpl::GetEngineByLang(const BL_String& lang)
{
    return m_engineManager->GetEngineByLang(lang.GetString());
}

#endif

BL_String TTS_TextToSpeechImpl::GetDefaultLanguage()
{
    return m_engineManager->GetDefaultLanguage();
}

CL_ERROR TTS_TextToSpeechImpl::RegisterSound(const BL_String &sender, const BL_String &soundFileName, int* soundId)
{
    static int s_soundId = 0;
    *soundId = ++s_soundId;
    if (m_control) {
        return m_control->RegisterSound(sender, soundFileName, *soundId);
    } 
        
    return TTS_ERROR_NO_RESOURCE;
}

CL_ERROR TTS_TextToSpeechImpl::UnRegisterSound(const BL_String &sender, int soundId)
{
    if (m_control) {
        m_control->UnRegisterSound(sender, soundId);
    }
    
    return TTS_ERROR_NO_RESOURCE;
}
    
CL_ERROR TTS_TextToSpeechImpl::PlayImpl(const BL_String &sender, TTS_PlaylistInfo *pPlaylist, 
                                int *reqId)
{
    int requestId = (NULL == reqId) ? TTS_TextToSpeechImpl::m_uRequestID++ : *reqId;

    pPlaylist->SetRequestID(requestId);

    TTS_EventPlaylist *event = new(MEM_Voice) TTS_EventPlaylist(m_playThread, pPlaylist, &m_playlistManager, m_listener);
    if (!event) {
        TTS_LOGE("no resource!");
        return TTS_ERROR_NO_RESOURCE;
    }

    if (CL_FALSE == m_control->OnEvent(event)) {
        TTS_LOGE("can not push event!");
        delete event;
        event = NULL;
    }

    return TTS_ERROR_NO_ERROR;
}

TTS_PlaylistInfo *TTS_TextToSpeechImpl::CreatePlayList(const BL_String &sender)
{
    return new(MEM_Voice) TTS_PlaylistInfo(sender, this);
}

CL_ERROR TTS_TextToSpeechImpl::Stop(const BL_String &sender, int reqId)
{
    TTS_LOGI("stop reqId=%d", reqId);
    TTS_EventStopReq *event = new(MEM_Voice) TTS_EventStopReq(m_playThread, &m_playlistManager, m_listener, reqId, sender, CL_FALSE);
    if (!event) {
        return TTS_ERROR_NO_RESOURCE;
    }

    if (CL_FALSE == m_control->OnEvent(event)) {
        delete event;
        event = NULL;
    }

    return TTS_ERROR_NO_ERROR;
}

CL_ERROR TTS_TextToSpeechImpl::StopAll(const BL_String &sender)
{
    TTS_LOGI("Stop all");
    TTS_EventStopReq *event = new(MEM_Voice) TTS_EventStopReq(m_playThread, &m_playlistManager, m_listener, 0, sender, CL_TRUE);
    if (!event) {
        return TTS_ERROR_NO_RESOURCE;
    }

    if (CL_FALSE == m_control->OnEvent(event)) {
        delete event;
        event = NULL;
    }

    return TTS_ERROR_NO_ERROR;
}

void TTS_TextToSpeechImpl::OnPlayEnd(TTS_PLAY_RESULT result, const BL_String &sender, int reqID)
{
    MakeTimeTag("OnPlayEnd");
    LogoutTimeTags();
    ClearTags();    
    
    TTS_LOGD("result=%d,reqId=%d", result, reqID);
    TTS_LOGD("sender=%s", sender.GetString());
    m_lock.SyncStart();
    m_listener->OnTtsSpeechCompleted(sender, reqID, result);
    m_lock.SyncEnd();
}

void TTS_TextToSpeechImpl::OnPlayEnd(TTS_PlaylistInfo* playlist, TTS_PLAYER_STATE status)
{
    TTS_EventPlayEnd *event = new(MEM_Voice) TTS_EventPlayEnd(m_playThread, playlist, &m_playlistManager, m_listener, status);
    if (!event) {
        return;
    }

    if (CL_FALSE == m_control->OnEvent(event)) {
        delete event;
        event = NULL;
    }
}

CL_ERROR TTS_TextToSpeechImpl::Pause(int reqId, const BL_String &sender)
{
    TTS_LOGI("Pause Id=%d,send=%s", reqId, sender.GetString());

    TTS_EventPauseReq *event = new(MEM_Voice) TTS_EventPauseReq(m_playThread, m_listener, &m_playlistManager, reqId, sender);
    if (!event) {
        return TTS_ERROR_NO_RESOURCE;
    }

    if (CL_FALSE == m_control->OnEvent(event)) {
        delete event;
        event = NULL;
    }

    return TTS_ERROR_NO_ERROR;
}

CL_ERROR TTS_TextToSpeechImpl::Resume(int reqId, const BL_String &sender)
{
    TTS_LOGI("Resume Id=%d,send=%s", reqId, sender.GetString());

    TTS_EventResumeReq *event = new(MEM_Voice) TTS_EventResumeReq(m_playThread, m_listener, &m_playlistManager, reqId, sender);
    if (!event) {
        return TTS_ERROR_NO_RESOURCE;
    }

    if (CL_FALSE == m_control->OnEvent(event)) {
        delete event;
        event = NULL;
    }

    return TTS_ERROR_NO_ERROR;
}

void TTS_TextToSpeechImpl::OnPlayState(int reqID, const BL_String &sender, TTS_PLAYER_STATE state, int playNum)
{
    MakeTimeTag("OnPlayInfo");
    LogoutTimeTags();
    ClearTags();

    TTS_LOGD("state=%d,send=%s,Id=%d", state, sender.GetString(), reqID);
    m_lock.SyncStart();
    m_listener->OnPlayStateOver(sender, reqID, state, playNum);
    m_lock.SyncEnd();
}

#if 0
CL_ERROR TTS_TextToSpeechImpl::RegisterTuningData(const BL_String &sender, TTS_TuningData* tuningData)
{
    TTS_LOGI("RegisterTuningData sender= %s", sender.GetString());

    TTS_EventRegisterTuningDataReq *event = new(MEM_Voice) TTS_EventRegisterTuningDataReq(m_control, sender, tuningData, m_listener);
    if (!event) {
        return TTS_ERROR_NO_RESOURCE;
    }

    if (CL_FALSE == m_control->OnEvent(event)) {
        delete tuningData;
        delete event;
    }
    return TTS_ERROR_NO_ERROR;
}

#endif

CL_ERROR TTS_TextToSpeechImpl::Play(TTS_PlaylistInfo* playlist, const TTS_POLICY &policy, int *reqId)
{

    TTS_LOGI("reqId=%d", *reqId);
    playlist->SetPolicy(policy);
    return PlayImpl(playlist->GetSender(), playlist, reqId);
}

CL_ERROR TTS_TextToSpeechImpl::SynthesizeToFile(const BL_String &sender, const BL_String &text, const BL_String &filePath, 
        const TTS_POLICY &policy, int *reqId)
{
    /* TTS_LOGI("play, sender=%s, text=%s", sender.GetString(), text.GetString());
    if (reqId) {
        TTS_LOGD("playtext reqId=%d", *reqId);
    }

    TTS_PlaylistInfo *pPlaylist = new(MEM_Voice) TTS_PlaylistInfo(sender, policy, this);
    if (!pPlaylist) {
        TTS_LOGE("create TTS_PlaylistInfo failed");
         return TTS_ERROR_NO_RESOURCE;
     }
 
    pPlaylist->AddText(text, filePath);
    return PlayImpl(sender, pPlaylist, reqId); */

    return TTS_ERROR_NO_ERROR;
}

BL_String TTS_TextToSpeechImpl::GetSoundName(const BL_String &sender, int soundID)
{
    return m_control->GetSoundName(sender, soundID);
}

/* EOF */
