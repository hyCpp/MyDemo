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
* @file  VR_DebugRecorderEventHandler.cpp
* @brief Declaration file of class VR_DebugRecorderEventHandler.
*
* This file includes the declaration of class VR_DebugRecorderEventHandler.
*
* @attention used for C++ only.
*/


#include   "stdafx.h"
#include   "boost/format.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "VR_DebugRecorderEventHandler.h"
#include   "VR_ConfigureIF.h"
#include   "VR_RunableThread.h"
#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using  namespace  navi::VoiceRecog;

VR_DiagEventHandler::VR_DiagEventHandler(VR_DebugRecorderEventHandler *pDebugHandler, const nutshell::NCRunnableLooper &looper)
    : nutshell::NCEventHandler(looper)
    , m_pDebugEventHandler(pDebugHandler)
{
    VR_LOGD_FUNC();
}


VR_DiagEventHandler::~VR_DiagEventHandler()
{
    VR_LOGD_FUNC();
}

void  VR_DiagEventHandler::handleEvent(const nutshell::NCEvent &event)
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

    VR_ConfigureIF *pConfig = VR_ConfigureIF::Instance();
    bool bFlag = (VR_ConfigureIF::VR_EngineType::VBT == pConfig->getEngineType()) ? true : false;

    if (Dialog_Event_Copy == ncIdentify) {
        std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage *controlMessage = loopMessage->mutable_m_controlmessage();
        controlMessage->set_type(DMTaskMessageType_DiagEventResult);
        if (bFlag) {
            controlMessage->mutable_diageventresult()->set_eventtype(event_copy_start);
            VR_LOGI("******Vr Service Send LoopBack Message*******[event_copy_start]");
        }
        else {
            controlMessage->mutable_diageventresult()->set_eventtype(event_copy_finish);
        }

        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    else if (Dialog_Event_Delete == ncIdentify) {
        std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
        controlMessage->set_type(DMTaskMessageType_DiagEventResult);
        if (bFlag) {
            controlMessage->mutable_diageventresult()->set_eventtype(event_delete_start);
        }
        else {
            controlMessage->mutable_diageventresult()->set_eventtype(event_delete_finish);
        }

        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    else {
        VR_LOGD("Evnet Identify error: [%s]", ncIdentify.getString());
    }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VR_DebugRecorderEventHandler::VR_DebugRecorderEventHandler(spVR_RunableThread   spRunableThread, spVR_DialogEngineIF spDialogEngine)
    : m_spRunableThread(spRunableThread)
    , m_spDialogEngine(spDialogEngine)
    , m_spDiagEventHandler(NULL)
{
    VR_LOGD_FUNC();
}


VR_DebugRecorderEventHandler::~VR_DebugRecorderEventHandler()
{
    VR_LOGD_FUNC();
}

void  VR_DebugRecorderEventHandler::OnStart()
{
    VR_LOGD_FUNC();
#ifdef HAVE_NUTSHELL_OS
    m_filter.push_back(m_context.buildEventFullName(DialogServiceSpaceName, Dialog_Event_Copy));
    m_filter.push_back(m_context.buildEventFullName(DialogServiceSpaceName, Dialog_Event_Delete));
    m_spDiagEventHandler =  VR_new  VR_DiagEventHandler(this, m_spRunableThread->getRunableLooper());
    m_context.registerHandler(m_spDiagEventHandler, m_filter);
#endif
}

void  VR_DebugRecorderEventHandler::OnStop()
{
    VR_LOGD_FUNC();

    m_context.unregisterHandler(m_spDiagEventHandler, m_filter);
    m_filter.clear();
}

void   VR_DebugRecorderEventHandler::OnMessage(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();

    const  VrDialogEventResult&  eventResult = (const VrDialogEventResult&)msg;
    VrDialogEventType            eventType   = eventResult.eventtype();

    if (event_copy_finish == eventType) {
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
    else if (event_delete_finish == eventType) {
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
        VR_LOGD("Event Type error.");
    }
}

/* EOF */
