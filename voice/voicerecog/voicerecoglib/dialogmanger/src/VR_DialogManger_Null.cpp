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
#include "VR_RunableThread.h"
#include "VR_DialogEngineIF.h"
#include "VR_DialogManger_Null.h"
#include "BL_AplInfo.h"
#include "VR_DialogEngineFactory.h"

const  std::string   VR_EVENTHANDLER_THREAD_NULL = "VR_Voice_EventHandler";

VR_DialogManger_Null::VR_DialogManger_Null()
    : m_spRunnableThread(VR_new  VR_RunableThread(VR_EVENTHANDLER_THREAD_NULL.c_str(), false))
    , m_spDialogEngine(VR_DialogEngineFactory::CreateDialogEngine(true))
    , m_spDebugEventHandler(VR_new  VR_DebugRecorderEventHandler_Null(m_spRunnableThread, m_spDialogEngine))
{
    VR_LOGD_FUNC();
}

void   VR_DialogManger_Null::Initialize()
{
    VR_LOGD_FUNC();
}


void   VR_DialogManger_Null::Start()
{
    VR_LOGD_FUNC();

    m_spRunnableThread->startThread();
    m_spDebugEventHandler->OnStart();
    BL_AplInfo::UpdateLazyServicReady();
}

void   VR_DialogManger_Null::Stop()
{
    VR_LOGD_FUNC();

    m_spDebugEventHandler->OnStop();
    m_spRunnableThread->stopThread();
}

void  VR_DialogManger_Null::Destory()
{
    VR_LOGD_FUNC();
}

/* EOF */
