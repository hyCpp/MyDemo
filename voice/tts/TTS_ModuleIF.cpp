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

#include "BL_Base.h"
#include "TTS_ModuleIF.h"
#include "TTS_ReceiverCb.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "navi/Voice/TTS/TtsRespServiceStatus.pb.h"
#include "navi/Voice/TTS/TtsPause.pb.h"
#include "stdafx.h"
#include "BL_AplInfo.h"

#define MODULENAME  "TTS_MAIN"

TTS_ModuleIF* TTS_ModuleIF::s_pInstance = NULL;

TTS_ModuleIF* TTS_ModuleIF::Instance()
{
    if (NULL == s_pInstance) {
        s_pInstance = new(MEM_Voice) TTS_ModuleIF();
    }
    return s_pInstance;
}

TTS_ModuleIF::TTS_ModuleIF() 
: m_pRecvCb(new(MEM_Voice) TTS_ReceiverCb),
  m_pEvtRecv(new(MEM_Voice) EV_EventReceiver(EV_MODULE_TTS))
{
}

TTS_ModuleIF::~TTS_ModuleIF()
{
    Destory();
}

void TTS_ModuleIF::PreInitialize()
{
    TTS_LOGI("PreInitialize");
    
    if (NULL == m_pRecvCb) {
        return;
    }
    
    if (NULL == m_pEvtRecv) {
        return;
    }
    
    m_pRecvCb->PreInitialize();
    // Register TTS mesage IDs
    m_pRecvCb->Register(m_pEvtRecv);
    // Start to wait for call event
    m_pEvtRecv->StartReceive();
}

void TTS_ModuleIF::Initialize()
{

    TTS_LOGI("Initialize");

    if (NULL == m_pRecvCb) {
        return;
    }

    m_pRecvCb->Initialize();

}

void TTS_ModuleIF::Start()
{
    TTS_LOGI("Start");
    if (m_pRecvCb) {
        m_pRecvCb->Start();
        
        RespServiceStatus();
    }

    BL_AplInfo::UpdateLazyServicReady();
    
/*
    sleep(5000);

    
    ::navi::TTS::TtsPause *pause = new(MEM_Voice) ::navi::TTS::TtsPause();
    if (!pause) {
        return;
    }

    if (NULL == m_pRecvCb) {
        return;
    }

    pause->set_sender("vr");
    pause->set_reqid(5);
    m_pRecvCb->OnRecvPause(pause);
*/
}

void TTS_ModuleIF::Stop()
{
    if (m_pEvtRecv) {
        m_pEvtRecv->StopReceive();
    }

    if (m_pRecvCb) {
        m_pRecvCb->Stop();
    }
}

void TTS_ModuleIF::Destory()
{
    if (NULL != m_pEvtRecv) {
        delete m_pEvtRecv;
        m_pEvtRecv = NULL;
    }
    if (NULL != m_pRecvCb) {
        m_pRecvCb->Destory();
        delete m_pRecvCb;
        m_pRecvCb = NULL;
    }
}

void TTS_ModuleIF::RespServiceStatus()
{
    ::navi::TTS::TtsRespServiceStatus *resp = new(MEM_Voice) ::navi::TTS::TtsRespServiceStatus();
    if (!resp) {
        return;
    }

    resp->set_sender(MODULENAME);
    resp->set_status(::navi::TTS::TtsRespServiceStatus::TTS_SERVICE_STATUS_READY);

    EV_EventSender cSender;
    cSender.SendEvent(EV_EVENT_TTS_RESPSERVICESTATUS, resp);
}

#ifdef __cplusplus
extern "C"    
MODULE_API ModuleIFBase* GetInstance()
{
    return TTS_ModuleIF::Instance(); 
}

#endif
/* EOF */
