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

/**
* @file  VR_DebugRecorderEventHandler_Null.cpp
* @brief Declaration file of class VR_DebugRecorderEventHandler_Null.
*
* This file includes the declaration of class VR_DebugRecorderEventHandler_Null.
*
* @attention used for C++ only.
*/

#include   "stdafx.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "VR_ConfigureIF.h"
#include   "VR_RunableThread.h"
#include   "VR_DebugRecorderEventHandler_Null.h"
#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using  namespace  navi::VoiceRecog;

//  VR_DiagEventHandler_Null
VR_DiagEventHandler_Null::VR_DiagEventHandler_Null(VR_DebugRecorderEventHandler *pDebugRecorderEventHandler,
                                                         const nutshell::NCRunnableLooper &looper)
    : VR_DiagEventHandler(pDebugRecorderEventHandler, looper)
{
    VR_LOGD_FUNC();
}



VR_DiagEventHandler_Null::~VR_DiagEventHandler_Null()
{
    VR_LOGD_FUNC();
}


void   VR_DiagEventHandler_Null::handleEvent(const nutshell::NCEvent &event)
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    nutshell::NCString   ncSpaceName = event.space();
    if (-1 == ncSpaceName.find(DialogServiceSpaceName.getString())) {
       VR_LOGD("Event Space Name error: [%s]", ncSpaceName.getString());
       return;
    }

    nutshell::NCString   ncIdentify("");
    event.identity(&ncIdentify);
    VR_LOGD("ncIdentify = [%s]", ncIdentify.getString());

    if (Dialog_Event_Copy == ncIdentify) {
        nutshell::NCEvSysContext  context;
        nutshell::NCEvMessageIF*  eventSender = context.queryEventSender();
        nutshell::NCEvent*        event       = context.obtainEvent(VoiceRecogDialogSpace, Voice_Event_Copy);
        if (NULL != eventSender && NULL != event) {
            if (nutshell::NC_FALSE == eventSender->sendEvent(event)) {
                VR_LOGD("Send event error.");
            }
            else {
                VR_LOGD("*********Send Event Ok************");
            }
        }
        else {
            VR_LOGD("Send event error.");
        }
        context.releaseEvent(event);
    }
    else if (Dialog_Event_Delete == ncIdentify) {
        nutshell::NCEvSysContext   context;
        nutshell::NCEvMessageIF*   eventSender = context.queryEventSender();
        nutshell::NCEvent*         event       = context.obtainEvent(VoiceRecogDialogSpace, Voice_Event_Delete);
        if (NULL != eventSender && NULL != event) {
            if (nutshell::NC_FALSE == eventSender->sendEvent(event)) {
                VR_LOGD("Send event error.");
            }
            else {
                VR_LOGD("*********Send Event Ok**************");
            }
        }
        else {
            VR_LOGD("Send event error.");
        }
        context.releaseEvent(event);
    }
    else {
        VR_LOGD("Evnet Identify error: [%s]", ncIdentify.getString());
    }
#endif
}

// VR_DebugRecorderEventHandler_Null
VR_DebugRecorderEventHandler_Null::VR_DebugRecorderEventHandler_Null(spVR_RunableThread  spRunnableThread, spVR_DialogEngineIF spDialogEngine)
    : VR_DebugRecorderEventHandler(spRunnableThread, spDialogEngine)
{
    VR_LOGD_FUNC();
}


VR_DebugRecorderEventHandler_Null::~VR_DebugRecorderEventHandler_Null()
{
    VR_LOGD_FUNC();
}

void   VR_DebugRecorderEventHandler_Null::OnStart()
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    m_filter.push_back(m_context.buildEventFullName(DialogServiceSpaceName, Dialog_Event_Copy));
    m_filter.push_back(m_context.buildEventFullName(DialogServiceSpaceName, Dialog_Event_Delete));
    m_spDiagEventHandler =  VR_new  VR_DiagEventHandler_Null(this, m_spRunableThread->getRunableLooper());
    m_context.registerHandler(m_spDiagEventHandler, m_filter);
#endif
}

void VR_DebugRecorderEventHandler_Null::OnStop()
{
    VR_LOGD_FUNC();
    m_context.unregisterHandler(m_spDiagEventHandler, m_filter);
    m_filter.clear();
}

/* EOF */

