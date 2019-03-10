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
 *  @file   VR_StartFinishNotifyAction.cpp
 *  @brief  Implementation  class  of  VR_StartFinishNotifyAction
 */

#include   "stdafx.h"
#include   "BL_AplInfo.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "VR_StartFinishNotifyAction.h"

#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include   "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using  namespace navi::VoiceRecog;

VR_StartFinishNotifyAction::VR_StartFinishNotifyAction(spVR_ActionEventSender sender, int nEngineId, int nActionType, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_nEngineId(nEngineId)
    , m_nActionType(nActionType)
    , m_ReqMsg(proto.New())
{
    VR_LOGD("nEngineId = [%d], nActionType = [%d]", m_nEngineId, m_nActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_StartFinishNotifyAction::~VR_StartFinishNotifyAction()
{
    VR_LOGD_FUNC();
}

void  VR_StartFinishNotifyAction::Run()
{
    VR_LOGD_FUNC();

    BL_AplInfo::UpdateLazyServicReady();
    VR_LOGI("voicerecog lazy service ready now.[in dcu or avclan-meu]");

    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_LAZYSERVICE_NOTIFY);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, proto.release());

    this->SetActionComplete();
}

void  VR_StartFinishNotifyAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}

/* EOF */
