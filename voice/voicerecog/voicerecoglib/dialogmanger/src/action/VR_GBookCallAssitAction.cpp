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
 *  @file   VR_GBookCallAssitAction.cpp
 *  @brief  Implementation  class  of  VR_GBookCallAssitAction
 */
#include "stdafx.h"
#include "VR_GBookCallAssitAction.h"
#include "VR_ActionEventSender.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"

using namespace navi::VoiceRecog;

VR_GBookCallAssitAction::VR_GBookCallAssitAction(spVR_ActionEventSender sender, int nEngineId, int nActionType, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_nEngineId(nEngineId)
    , m_nActionType(nActionType)
    , m_ReqMsg(proto.New())
{
    VR_LOGD("nEngineId = [%d], nActionType = [%d]", m_nEngineId, m_nActionType);

    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}


VR_GBookCallAssitAction::~VR_GBookCallAssitAction()
{
    VR_LOGD_FUNC();
}

void  VR_GBookCallAssitAction::Run()
{
    VR_LOGD_FUNC();
}

void VR_GBookCallAssitAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}
/* EOF */
