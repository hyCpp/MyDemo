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
 * @file MMVR_RecogTaskStateBase.cpp
 * @brief Declaration file of class MMVR_RecogTaskStateBase and Derived classes.
 *
 * This file includes the declaration of class MMVR_RecogTaskStateBase and Derived classes.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include "MMVR_RecogTaskState.h"
#include <memory>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

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

#include "navi/Voice/VoiceRecog/RequestMMVr.pb.h"
#include "navi/Voice/VoiceRecog/RespMMVrResult.pb.h"

using namespace navi::VoiceRecog;

/***** MMVR_TaskBase ******/
MMVR_RecogTaskStateBase::MMVR_RecogTaskStateBase(VR_RecogTaskIF* task)
    : VR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void  MMVR_RecogTaskStateBase::Start(int nType)
{
    VR_LOGD_FUNC();
    VR_UNUSED_VAR(nType);
    std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
    MMVRResult->set_sender("dm");
    MMVRResult->set_funccode(VrMMFunc_StartVoiceRecog);
    VrStartMMVoiceRecogResult* startVoiceRecog = MMVRResult->mutable_startmmvoicerecogresult();
    startVoiceRecog->set_operresult(VrOpeResult_Undefined);
    GetRecogTask()->ReplyRequest(MMVRResult.release());
}

void  MMVR_RecogTaskStateBase::Cancel()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
    MMVRResult->set_sender("dm");
    MMVRResult->set_funccode(VrMMFunc_CancelVoiceRecog);
    VrCancelMMVoiceRecogResult* cancelVoiceRecog = MMVRResult->mutable_cancelmmvoicerecogresult();
    cancelVoiceRecog->set_operresult(VrOpeResult_Undefined);
    GetRecogTask()->ReplyRequest(MMVRResult.release());
}


/******* MMVR StateIDL ********/
MMVR_RecogTaskStateIDL::MMVR_RecogTaskStateIDL(VR_RecogTaskIF* task)
    : MMVR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void  MMVR_RecogTaskStateIDL::Start(int nType)
{
    VR_LOGD_FUNC();
    VR_String strprompt = GetRecogTask()->GetPrompt();
    int       screentype = GetRecogTask()->GetScreenType();
    VR_String startxml = VR_DialogEngineMessageBuilder::CreateMMVRStartDialogMsg(nType, strprompt, screentype);
    GetRecogTask()->SendMessage2Engine(startxml);

    spVR_RecogTaskStateBase state(VR_new MMVR_RecogTaskStateStart(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void  MMVR_RecogTaskStateIDL::Cancel()
{
    VR_LOGD_FUNC();
    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new MMVR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

/******* MMVR StateStart *********/
MMVR_RecogTaskStateStart::MMVR_RecogTaskStateStart(VR_RecogTaskIF* task)
    : MMVR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}


void  MMVR_RecogTaskStateStart::Cancel()
{
    VR_LOGD_FUNC();
    const VR_String cancelXML = VR_DialogEngineMessageBuilder::CreateCancelDialogMsg();
    GetRecogTask()->SendMessage2Engine(cancelXML);

    spVR_RecogTaskStateBase state(VR_new MMVR_RecogTaskStateCancel(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void  MMVR_RecogTaskStateStart::OnStarted(bool bResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
    MMVRResult->set_sender("dm");
    MMVRResult->set_funccode(VrMMFunc_StartVoiceRecog);
    VrStartMMVoiceRecogResult* startVoiceRecog = MMVRResult->mutable_startmmvoicerecogresult();
    startVoiceRecog->set_session(GetRecogTask()->GetTaskSession());
    startVoiceRecog->set_taskid(GetRecogTask()->GetTaskId());
    VrOpeResult emRet = bResult ? VrOpeResult_OK : VrOpeResult_Invaild;
    startVoiceRecog->set_operresult(emRet);
    GetRecogTask()->ReplyRequest(MMVRResult.release());

    spVR_RecogTaskStateBase state(VR_new MMVR_RecogTaskStateWork(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

MMVR_RecogTaskStateCancel::MMVR_RecogTaskStateCancel(VR_RecogTaskIF *task)
    : MMVR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void  MMVR_RecogTaskStateCancel::OnCanceled()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
    MMVRResult->set_sender("dm");
    MMVRResult->set_funccode(VrMMFunc_CancelVoiceRecog);
    VrCancelMMVoiceRecogResult* cancelVoiceRecog = MMVRResult->mutable_cancelmmvoicerecogresult();
    cancelVoiceRecog->set_taskid(GetRecogTask()->GetTaskId());
    GetRecogTask()->ReplyRequest(MMVRResult.release());

    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new MMVR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

MMVR_RecogTaskStateWork::MMVR_RecogTaskStateWork(VR_RecogTaskIF* task)
    : MMVR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void  MMVR_RecogTaskStateWork::Cancel()
{
    VR_LOGD_FUNC();
    const VR_String cancelxml = VR_DialogEngineMessageBuilder::CreateCancelDialogMsg();
    GetRecogTask()->SendMessage2Engine(cancelxml);

    std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
    MMVRResult->set_sender("dm");
    MMVRResult->set_funccode(VrMMFunc_CancelVoiceRecog);
    VrCancelMMVoiceRecogResult* cancelVoiceRecog = MMVRResult->mutable_cancelmmvoicerecogresult();
    cancelVoiceRecog->set_taskid(GetRecogTask()->GetTaskId());
    GetRecogTask()->ReplyRequest(MMVRResult.release());

    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new MMVR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void MMVR_RecogTaskStateWork::UserOperation(const VR_String & type)
{

}

void MMVR_RecogTaskStateWork::OnUsrOperationEnd(const VR_String & type)
{

}

void MMVR_RecogTaskStateWork::OnNotifyResult()
{

}

void MMVR_RecogTaskStateWork::OnNotifyComplete()
{

}
/* EOF */
