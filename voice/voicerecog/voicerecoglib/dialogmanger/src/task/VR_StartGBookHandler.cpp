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
* @file  VR_StartGBookHandler.cpp
* @brief Declaration file of class VR_StartGBookHandler.
*
* This file includes the declaration of class VR_StartGBookHandler.
*
* @attention used for C++ only.
*/

#include   "stdafx.h"
#include   "VR_StartGBookHandler.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "VR_EventSenderIF.h"
#include   "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include   "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"

using namespace navi::VoiceRecog;

VR_StartGBookHandler::VR_StartGBookHandler(spVR_EventSenderIF spEventSender, spVR_DialogEngineIF spDialogEngine)
    : VR_StartGBookBase(spEventSender, spDialogEngine)
{
    VR_LOGD_FUNC();
}

VR_StartGBookHandler::~VR_StartGBookHandler()
{
    VR_LOGD_FUNC();
}

void VR_StartGBookHandler::OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty&  reqCommonProperty = (const RequestVrCommonProperty&)msg;
    if (!reqCommonProperty.has_hybirdcommuconnectstatus()) {
        VR_LOGD("has_hybirdcommuconnectstatus() = [%d]", reqCommonProperty.has_hybirdcommuconnectstatus());
        return;
    }
    VR_LOGD("has_hybirdcommuconnectstatus() = [%d]", reqCommonProperty.has_hybirdcommuconnectstatus());

    std::unique_ptr<RespVrCommonPropertyResult>  replyMsg(RespVrCommonPropertyResult::default_instance().New());
    replyMsg->set_sender("dm");
    replyMsg->set_taskid(-1);   // -1 means a invalid task id
    VrReqHybirdCommuConnectStatusResult* ConnectResult = replyMsg->mutable_hybirdcommuconnectstatusresult();
    ConnectResult->set_result(false);  // if fc does not block the message;
    m_spEventSender->Reply(context, replyMsg.release());
}

void VR_StartGBookHandler::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_StartGBookHandler::OnStart()
{
    VR_LOGD_FUNC();
}


void VR_StartGBookHandler::OnStop()
{
    VR_LOGD_FUNC();
}

std::string VR_StartGBookHandler::getServerVRUrl()
{
    VR_LOGD_FUNC();
    std::string cServerVRUrl = "";
    return cServerVRUrl;
}

/* EOF */
