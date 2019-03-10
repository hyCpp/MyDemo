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
 * @file SDLVR_RecogTaskStateBase.cpp
 * @brief Declaration file of class SDLVR_RecogTaskStateBase and Derived classes.
 *
 * This file includes the declaration of class SDLVR_RecogTaskStateBase and Derived classes.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include "SDLVR_RecogTaskState.h"
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

#include "navi/Voice/VoiceRecog/RequestSDLVr.pb.h"
#include "navi/Voice/VoiceRecog/RespSDLVrResult.pb.h"

using namespace navi::VoiceRecog;

/***** SDLVR_TaskBase ******/
SDLVR_RecogTaskStateBase::SDLVR_RecogTaskStateBase(VR_RecogTaskIF* task)
    : VR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void  SDLVR_RecogTaskStateBase::Start(int nType)
{
    VR_LOGD_FUNC();
    VR_UNUSED_VAR(nType);
    std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
    SDLVRResult->set_sender("dm");
    SDLVRResult->set_funccode(SDLVrFunc_StartSDLVoiceRecog);
    SDLVrStartSDLVoiceRecogResult* startVoiceRecog = SDLVRResult->mutable_startsdlvoicerecogresult();
    startVoiceRecog->set_operresult(VrOpeResult_Undefined);
    GetRecogTask()->ReplyRequest(SDLVRResult.release());
}

void  SDLVR_RecogTaskStateBase::Cancel()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
    SDLVRResult->set_sender("dm");
    SDLVRResult->set_funccode(SDLVrFunc_CancelSDLVoiceRecog);
    SDLVrCancelSDLVoiceRecogResult* cancelVoiceRecog = SDLVRResult->mutable_cancelsdlvoicerecogresult();
    cancelVoiceRecog->set_operresult(VrOpeResult_Undefined);
    GetRecogTask()->ReplyRequest(SDLVRResult.release());
}


/******* SDLVR StateIDL ********/
SDLVR_RecogTaskStateIDL::SDLVR_RecogTaskStateIDL(VR_RecogTaskIF* task)
    : SDLVR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void  SDLVR_RecogTaskStateIDL::Start(int nType)
{
    VR_LOGD_FUNC();
    int timeout = GetRecogTask()->GetTimeout();
    VR_String strprompt = GetRecogTask()->GetPrompt();
    VR_String startxml = VR_DialogEngineMessageBuilder::CreateSDLStartDialogMsg(nType, timeout, strprompt);
    GetRecogTask()->SendMessage2Engine(startxml);

    spVR_RecogTaskStateBase state(VR_new SDLVR_RecogTaskStateStart(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void  SDLVR_RecogTaskStateIDL::Cancel()
{
    VR_LOGD_FUNC();
    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new SDLVR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

/******* SDLVR StateStart *********/
SDLVR_RecogTaskStateStart::SDLVR_RecogTaskStateStart(VR_RecogTaskIF* task)
    : SDLVR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}


void  SDLVR_RecogTaskStateStart::Cancel()
{
    VR_LOGD_FUNC();
    const VR_String cancelXML = VR_DialogEngineMessageBuilder::CreateCancelDialogMsg();
    GetRecogTask()->SendMessage2Engine(cancelXML);

    spVR_RecogTaskStateBase state(VR_new SDLVR_RecogTaskStateCancel(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void  SDLVR_RecogTaskStateStart::OnStarted(bool bResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
    SDLVRResult->set_sender("dm");
    SDLVRResult->set_funccode(SDLVrFunc_StartSDLVoiceRecog);
    SDLVrStartSDLVoiceRecogResult* startVoiceRecog = SDLVRResult->mutable_startsdlvoicerecogresult();
    startVoiceRecog->set_session(GetRecogTask()->GetTaskSession());
    startVoiceRecog->set_taskid(GetRecogTask()->GetTaskId());
    VrOpeResult emRet = bResult ? VrOpeResult_OK : VrOpeResult_Invaild;
    startVoiceRecog->set_operresult(emRet);
    GetRecogTask()->ReplyRequest(SDLVRResult.release());

    spVR_RecogTaskStateBase state(VR_new SDLVR_RecogTaskStateWork(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

SDLVR_RecogTaskStateCancel::SDLVR_RecogTaskStateCancel(VR_RecogTaskIF *task)
    : SDLVR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void  SDLVR_RecogTaskStateCancel::OnCanceled()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
    SDLVRResult->set_sender("dm");
    SDLVRResult->set_funccode(SDLVrFunc_CancelSDLVoiceRecog);
    SDLVrCancelSDLVoiceRecogResult* cancelVoiceRecog = SDLVRResult->mutable_cancelsdlvoicerecogresult();
    cancelVoiceRecog->set_taskid(GetRecogTask()->GetTaskId());
    GetRecogTask()->ReplyRequest(SDLVRResult.release());

    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new SDLVR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

SDLVR_RecogTaskStateWork::SDLVR_RecogTaskStateWork(VR_RecogTaskIF* task)
    : SDLVR_RecogTaskStateBase(task)
{
    VR_LOGD_FUNC();
}

void  SDLVR_RecogTaskStateWork::Cancel()
{
    VR_LOGD_FUNC();
    const VR_String cancelxml = VR_DialogEngineMessageBuilder::CreateCancelDialogMsg();
    GetRecogTask()->SendMessage2Engine(cancelxml);

    std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
    SDLVRResult->set_sender("dm");
    SDLVRResult->set_funccode(SDLVrFunc_CancelSDLVoiceRecog);
    SDLVrCancelSDLVoiceRecogResult* cancelVoiceRecog = SDLVRResult->mutable_cancelsdlvoicerecogresult();
    cancelVoiceRecog->set_taskid(GetRecogTask()->GetTaskId());
    GetRecogTask()->ReplyRequest(SDLVRResult.release());

    GetRecogTask()->SetTaskComplete();
    spVR_RecogTaskStateBase state(VR_new SDLVR_RecogTaskStateBase(GetRecogTask()));
    GetRecogTask()->SetTaskState(state);
}

void SDLVR_RecogTaskStateWork::UserOperation(const VR_String & type)
{

}

void SDLVR_RecogTaskStateWork::OnUsrOperationEnd(const VR_String & type)
{

}

void SDLVR_RecogTaskStateWork::OnNotifyResult()
{

}

void SDLVR_RecogTaskStateWork::OnNotifyComplete()
{

}
/* EOF */
