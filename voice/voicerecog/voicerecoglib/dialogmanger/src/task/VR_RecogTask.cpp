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

#include <memory>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "VR_EventSenderIF.h"

#include "VR_RecogTask.h"
#include "VR_RecogTaskState.h"
#include "SDLVR_RecogTaskState.h"
#include "MMVR_RecogTaskState.h"
#include "VR_DialogEngineIF.h"
#include "VR_DailogEngineMessageBuilder.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"

#include "navi/Voice/VoiceRecog/RespVrCancelRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/RespVrStartRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace navi::VoiceRecog;

VR_RecogTask::VR_RecogTask(const int type, const VR_String& session,
                           const EV_EventContext &context, spVR_DialogEngineIF engine)
    : m_nType(type)
    , m_strSession(session)
    , m_spContext(VR_EventSenderIF::CopyEventContext(&context))
    , m_spEngineIf(engine)
    , m_currentState(VR_new VR_RecogTaskStateIDL(this))
    , m_spEventSenderIF(VR_EventSenderIF::GetSender())
{
    VR_LOGI("m_nType=[%d],m_strSession=[%s]", m_nType, m_strSession.c_str());
}

VR_RecogTask::~VR_RecogTask()
{
    VR_LOGD_FUNC();
}

void VR_RecogTask::Start()
{
    VR_LOGD_FUNC();
    m_currentState->Start(m_nType);
}

void VR_RecogTask::UserOperation(const EV_EventContext &context, const VR_String &type)
{
    VR_LOGI("type=[%s]", type.c_str());
    m_spContext = VR_EventSenderIF::CopyEventContext(&context);
    m_currentState->UserOperation(type);
}

int VR_RecogTask::GetTaskId()
{
    VR_LOGI("GetTaskId()=[%d]", GetTaskID());
    return this->GetTaskID();
}

std::string VR_RecogTask::GetTaskSession()
{
    VR_LOGI("m_strSession=[%s]", m_strSession.c_str());
    return m_strSession;
}

void VR_RecogTask::SetTaskComplete()
{
    VR_LOGD_FUNC();
    this->SetComplete();
}

void VR_RecogTask::Cancel(const EV_EventContext &context)
{
    VR_LOGD_FUNC();
    m_spContext = VR_EventSenderIF::CopyEventContext(&context);
    m_currentState->Cancel();
}

void VR_RecogTask::OnMessage(const VR_ProtoMessageBase& msg)
{
    const VrEngineControlMessage& message = (const VrEngineControlMessage&) msg;

    VR_LOGI("message.type()=[%d]", message.type());
    switch (message.type()) {
    case DMTaskMessageType_StartEnd:
        m_currentState->OnStarted(message.start_end().result());
        break;
    case DMTaskMessageType_CancelEnd:
        m_currentState->OnCanceled();
        break;
    case DMTaskMessageType_NotifyResult:
        m_currentState->OnNotifyResult();
        break;
    case DMTaskMessageType_NotifyComplete:
        m_currentState->OnNotifyComplete();
        break;
    case DMTaskMessageType_RepOperCommand:
        m_currentState->OnUsrOperationEnd(message.reply_command().type());

        break;
    default:
        break;
    }
}

void VR_RecogTask::SetTaskState(spVR_RecogTaskStateBase state)
{
    VR_LOGD_FUNC();
    m_currentState = state;
}

bool VR_RecogTask::ReplyRequest(VR_ProtoMessageBase* respMsg)
{
    std::unique_ptr<VR_ProtoMessageBase> msg(respMsg);
    if (NULL != m_spContext) {
        VR_LOGI("NULL != m_spContext");
        m_spEventSenderIF->Reply(*m_spContext, msg.release());
    }
    return (NULL != m_spContext);
}

bool VR_RecogTask::SendMessage2Engine(const std::string &msg)
{
    VR_LOGI("m_spEngineIf=[%p], msg=[%s]",
           m_spEngineIf.get(), msg.c_str());

    if (NULL != m_spEngineIf) {
        m_spEngineIf->SendMessage(msg);
    }
    return (NULL != m_spEngineIf);
}

/***** SDLVR RecogTask ********/
SDLVR_RecogTask::SDLVR_RecogTask(const int type,
                                 const VR_String &session,
                                 const EV_EventContext &context,
                                 spVR_DialogEngineIF engine,
                                 int timeout,
                                 const VR_String &strprompt)
    : VR_RecogTask(type, session, context, engine)
    , m_timeout(timeout)
    , m_prompt(strprompt)
{
    VR_LOGD_FUNC();
    m_currentState = spVR_RecogTaskStateBase(VR_new SDLVR_RecogTaskStateIDL(this));
}

SDLVR_RecogTask::~SDLVR_RecogTask()
{
    VR_LOGD_FUNC();
}

int  SDLVR_RecogTask::GetTimeout()
{
    return m_timeout;
}

std::string  SDLVR_RecogTask::GetPrompt()
{
    return m_prompt;
}


/***** MMVR RecogTask *****/
MMVR_RecogTask::MMVR_RecogTask(const int type,
                               const VR_String &session,
                               const EV_EventContext &context,
                               spVR_DialogEngineIF engine,
                               const VR_String &strprompt,
                               int screentype
                               )
    : VR_RecogTask(type, session, context, engine)
    , m_strprompt(strprompt)
    , m_screentype(screentype)
{
    VR_LOGD_FUNC();
    m_currentState = spVR_RecogTaskStateBase(VR_new MMVR_RecogTaskStateIDL(this));
}

MMVR_RecogTask::~MMVR_RecogTask()
{
    VR_LOGD_FUNC();
}

std::string MMVR_RecogTask::GetPrompt()
{
    return m_strprompt;
}

int  MMVR_RecogTask::GetScreenType()
{
    return m_screentype;
}
/* EOF */
