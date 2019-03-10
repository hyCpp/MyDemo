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
#include "VP_ModuleIF.h"
#include "VP_ReceiverCb.h"

#ifndef CXX_BL_LOG_H
#   include "BL_Log.h"
#endif

using namespace std;

VP_ModuleIF* VP_ModuleIF::s_pInstance = nullptr;
CL_SyncObj VP_ModuleIF::m_lock;

VP_ModuleIF* VP_ModuleIF::Instance() 
{
    CL_AutoSync lock(m_lock);

    if (nullptr == s_pInstance) {
        s_pInstance = new(MEM_Voice) VP_ModuleIF();
    }
    return s_pInstance;
}

VP_ModuleIF::VP_ModuleIF() 
    : m_pRecvCb(nullptr)
    , m_pEvtRecv(nullptr)
    , m_pNCEventRecv(nullptr)
{
}

VP_ModuleIF::~VP_ModuleIF() 
{
    delete m_pRecvCb;
    delete m_pEvtRecv;
    delete m_pNCEventRecv;

    m_pRecvCb = NULL;
    m_pEvtRecv = NULL;
    m_pNCEventRecv = NULL;
}

void VP_ModuleIF::Initialize() 
{
    // init the vp module
    m_pPlayControl = VP_Factory::Instance()->AcquirePlayControl();
    if (nullptr == m_pPlayControl) {
        return;
    }

    if (!m_channelMgr) {
        m_channelMgr = make_shared<VP_ChannelManager>();
    }

    m_pPlayControl->Initialize(m_channelMgr);

    if (!m_pRecvCb) {
        m_pRecvCb = new(MEM_Voice) VP_ReceiverCb();
    }

    m_pEvtRecv = new(MEM_Voice) EV_EventReceiver(EV_MODULE_VOICEPLAY);

    // Register VP mesage IDs
    m_pRecvCb->Register(m_pEvtRecv);

    // Start to wait for call event
    m_pEvtRecv->StartReceive();

    m_pNCEventRecv = new(MEM_Voice) VP_NCEventReceiver();
    m_pNCEventRecv->initialize();
}

void VP_ModuleIF::Start() 
{
//    navi::VoicePlay::VpPlaySoundList* soundList = new(MEM_Voice) navi::VoicePlay::VpPlaySoundList();
//    if (nullptr == soundList) {
//        VP_LOGE("create play list failed!");
//        return ;
//    }
//
//    navi::VoicePlay::VpPlaySoundList_VpPhrase* phrase =
//        soundList->add_phrases();
//    navi::VoicePlay::VpPlaySoundList_VpPhrase* ttsPhrase = 
//        soundList->add_phrases();
//
//    ttsPhrase->set_text("Test for playing TTS");
//    ttsPhrase->set_ttstype(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_TTS_TYPE_VP_TTS_GENERAL);
//    ttsPhrase->set_type(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);
//
//    phrase->set_text("using namespace");
//    phrase->set_ttstype(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_TTS_TYPE_VP_TTS_GENERAL);
//    phrase->set_type(navi::VoicePlay::VpPlaySoundList_VpPhrase_VP_SOUND_TYPE_VP_SOUND_TTS);
//
//    soundList->set_sender("0x58.RouteGuidance");
//    soundList->set_userdata(0);
//    soundList->set_needplayendnotificy(true);
//
//    m_pRecvCb->OnRecvSoundPlayList(soundList);
}

void VP_ModuleIF::Stop() 
{
    VP_LOGI("system stop");
    if (nullptr != m_pNCEventRecv) {
        m_pNCEventRecv->stopRecv();
    }

    if (nullptr != m_pEvtRecv) {
        m_pEvtRecv->StopReceive();
    }

    if (nullptr != m_pPlayControl) {
        m_pPlayControl->Quit();
    }
}

void VP_ModuleIF::Destory() 
{
}

#ifdef __cplusplus
extern "C" 
   
MODULE_API ModuleIFBase* GetInstance() 
{ 
    return VP_ModuleIF::Instance(); 
}

#endif
/* EOF */
