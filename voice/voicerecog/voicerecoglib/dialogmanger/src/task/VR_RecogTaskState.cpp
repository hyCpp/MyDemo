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
 * @file RecogTask_State.cpp
 * @brief Declaration file of class VR_RecogTaskStateBase and Derived classes.
 *
 * This file includes the declaration of class VR_RecogTaskStateBase and Derived classes.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"

#include <memory>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "VR_RecogTaskState.h"
#include "VR_RecogTask.h"
#include "VR_DailogEngineMessageBuilder.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"

#include "navi/Voice/VoiceRecog/RespVrStartRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCancelRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/RespVrReqOperationResult.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace navi::VoiceRecog;

/* StateBase */
VR_RecogTaskStateBase::VR_RecogTaskStateBase(VR_RecogTaskIF *task)
    : m_RecogTask(task)
{
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateBase::Start(int nType)
{
    VR_LOGI("nType=[%d]", nType);
    VR_UNUSED_VAR(nType);

    std::unique_ptr<RespVrStartRecognitionResult> replyMsg(RespVrStartRecognitionResult::default_instance().New());
    replyMsg->set_result(VrOpeResult_Undefined);
    GetRecogTask()->ReplyRequest(replyMsg.release());
}

void VR_RecogTaskStateBase::Cancel()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrCancelRecognitionResult> replyMsg(RespVrCancelRecognitionResult::default_instance().New());
    replyMsg->set_result(VrOpeResult_Undefined);
    GetRecogTask()->ReplyRequest(replyMsg.release());
}

void VR_RecogTaskStateBase::UserOperation(const VR_String &type)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrReqOperationResult> replyMsg(RespVrReqOperationResult::default_instance().New());
    replyMsg->set_result(VrOpeResult_Undefined);
    GetRecogTask()->ReplyRequest(replyMsg.release());
}

void VR_RecogTaskStateBase::OnStarted(bool bResult)
{
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateBase::OnCanceled()
{
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateBase::OnNotifyResult()
{
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateBase::OnNotifyComplete()
{
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateBase::OnUsrOperationEnd(const VR_String & type)
{
    VR_LOGD_FUNC();
    VR_UNUSED_VAR(type);
}

VR_RecogTaskIF *VR_RecogTaskStateBase::GetRecogTask()
{
    VR_LOGD_FUNC();
    return m_RecogTask;
}

/* IDL */
VR_RecogTaskStateIDL::VR_RecogTaskStateIDL(VR_RecogTaskIF* task)
    : VR_RecogTaskStateBase(task)
{
    VR_LOGI("VR_RecogTaskStateIDL=[%p]", task);
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateIDL::Start(int nType)
{
    VR_LOGI("nType=[%d]", nType);
    const VR_String startXml = VR_DialogEngineMessageBuilder::CreateStartDialogMsg(nType);
    GetRecogTask()->SendMessage2Engine(startXml);

    spVR_RecogTaskStateBase state(VR_new VR_RecogTaskStateStart(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void VR_RecogTaskStateIDL::Cancel()
{
    VR_LOGD_FUNC();
    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new VR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

/* Start */
VR_RecogTaskStateStart::VR_RecogTaskStateStart(VR_RecogTaskIF* task)
    : VR_RecogTaskStateBase(task)
{
    VR_LOGI("VR_RecogTaskStateStart=[%p]", task);
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateStart::Cancel()
{
    VR_LOGD_FUNC();
    const VR_String cancelXML = VR_DialogEngineMessageBuilder::CreateCancelDialogMsg();
    GetRecogTask()->SendMessage2Engine(cancelXML);

    spVR_RecogTaskStateBase state(VR_new VR_RecogTaskStateCancel(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void VR_RecogTaskStateStart::OnStarted(bool bResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrStartRecognitionResult> replyMsg(RespVrStartRecognitionResult::default_instance().New());
    replyMsg->set_session(GetRecogTask()->GetTaskSession());
    replyMsg->set_taskid(GetRecogTask()->GetTaskId());
    VrOpeResult emRet = bResult ? VrOpeResult_OK : VrOpeResult_Invaild;
    replyMsg->set_result(emRet);
    GetRecogTask()->ReplyRequest(replyMsg.release());

    spVR_RecogTaskStateBase state(VR_new VR_RecogTaskStateWork(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

/* Cancel */
VR_RecogTaskStateCancel::VR_RecogTaskStateCancel(VR_RecogTaskIF* task)
    : VR_RecogTaskStateBase(task)
{
    VR_LOGI("VR_RecogTaskStateCancel=[%p]", task);
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateCancel::OnCanceled()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrCancelRecognitionResult> replyMsg(RespVrCancelRecognitionResult::default_instance().New());
    replyMsg->set_taskid(GetRecogTask()->GetTaskId());
    GetRecogTask()->ReplyRequest(replyMsg.release());

    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new VR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

/* Work */
VR_RecogTaskStateWork::VR_RecogTaskStateWork(VR_RecogTaskIF* task)
    : VR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void VR_RecogTaskStateWork::Cancel()
{
    VR_LOGD_FUNC();
    const VR_String cancelXML = VR_DialogEngineMessageBuilder::CreateCancelDialogMsg();
    GetRecogTask()->SendMessage2Engine(cancelXML);

    std::unique_ptr<RespVrCancelRecognitionResult> replyMsg(RespVrCancelRecognitionResult::default_instance().New());
    replyMsg->set_taskid(GetRecogTask()->GetTaskId());
    GetRecogTask()->ReplyRequest(replyMsg.release());
    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new VR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void VR_RecogTaskStateWork::UserOperation(const VR_String &type)
{
    VR_LOGD_FUNC();
    const VR_String operationXML = VR_DialogEngineMessageBuilder::UsrOperationMsg(type);
    GetRecogTask()->SendMessage2Engine(operationXML);
}

void VR_RecogTaskStateWork::OnNotifyResult()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrCancelRecognitionResult> replyMsg(RespVrCancelRecognitionResult::default_instance().New());
    replyMsg->set_taskid(GetRecogTask()->GetTaskId());
    replyMsg->set_result(VrOpeResult_OK);
    GetRecogTask()->ReplyRequest(replyMsg.release());
}

void VR_RecogTaskStateWork::OnNotifyComplete()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrCancelRecognitionResult> replyMsg(RespVrCancelRecognitionResult::default_instance().New());
    replyMsg->set_taskid(GetRecogTask()->GetTaskId());
    GetRecogTask()->ReplyRequest(replyMsg.release());

    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new VR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void VR_RecogTaskStateWork::OnUsrOperationEnd(const VR_String &type)
{
    VR_LOGD_FUNC();
    VR_UNUSED_VAR(type);
    std::unique_ptr<RespVrReqOperationResult> replyMsg(RespVrReqOperationResult::default_instance().New());
    replyMsg->set_taskid(GetRecogTask()->GetTaskId());
    replyMsg->set_result(VrOpeResult_OK);
    GetRecogTask()->ReplyRequest(replyMsg.release());
}


/* EOF */

