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
 * @file   VR_HVACValidTemperature.cpp
 * @brief  Implementation class of VR_HVACValidTemperature
 */

#include "stdafx.h"
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#include "VR_ActionEventSender.h"
#include "VR_HVACCheckFanSpeed.h"
#include "VR_HAVCTypedefine.h"
#include "VR_HVACXMLMessageBuilder.h"

#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

#define VR_SUCCESS               "0"
#define VR_ALREADY_AT_FAN_SPEED  1
#define VR_INVALID_FAN_SPEED     2

using namespace navi::VoiceRecog;
VR_HVACCheckFanSpeed::VR_HVACCheckFanSpeed(int nEngineId, int nActionType, const VR_ProtoMessageBase &ProtoMsg,
                                                         spVR_ActionEventSender pSender)
    : VR_ActionBase(pSender),
      m_nEngineId(nEngineId),
      m_nActionType(nActionType),
      m_pProtoMsg(ProtoMsg.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_nEngineId, m_nActionType);
    m_pProtoMsg->CheckTypeAndMergeFrom(ProtoMsg);
    m_nFanSpeed = 0;
}

VR_HVACCheckFanSpeed::~VR_HVACCheckFanSpeed()
{
    VR_LOGD_FUNC();
}

void VR_HVACCheckFanSpeed::Run()
{
    VR_LOGD_FUNC();
    VR_VehicleInfoProxy::CreateInstance().GetFanSpeedStatus(GetActionId());
}

std::string VR_HVACCheckFanSpeed::GetReplyMsg() const
{
    std::string  resultXml = "";
    static  std::string  startXml =  "<action-result agent = \"climate\""
                                       " op = \"checkFanSpeed\" errcode = \"0\" >"
                                       "<status>%1%</status>"
                                   "</action-result>";
    std::string strStatus;
    int nCheckSpeed = GetCheckFanSpeed();
    bool bBlowerStep = VR_HVACXMLMessageBuilder::GetMessageBuilder().GetBlowStep();
    int nBlowerStep = bBlowerStep ? VR_FAN_SPEED_7 : VR_FAN_SPEED_5;
    if (nCheckSpeed == m_nFanSpeed) {
        VR_LOGI("nCheckSpeed == m_nFanSpeed");
        strStatus = (boost::format("%1%") % VR_ALREADY_AT_FAN_SPEED).str();
    }
    else if (nCheckSpeed < VR_FAN_SPEED_1 || nCheckSpeed > nBlowerStep) {
        VR_LOGI("nCheckSpeed < 1 || nCheckSpeed > nBlowerStep");
        strStatus = (boost::format("%1%") % VR_INVALID_FAN_SPEED).str();
    }
    else {
        VR_LOGI("else");
        strStatus = (boost::format("%1%") % VR_SUCCESS).str();
    }
    resultXml = (boost::format(startXml) % strStatus).str();

    VR_LOGI("[VR_HVACCheckFanSpeed : replyMsg = %s\n]", resultXml.c_str());
    return resultXml;
}

int  VR_HVACCheckFanSpeed::GetCheckFanSpeed()  const
{
    const VrCheckFanSpeed& checkFan = (const VrCheckFanSpeed&)(*m_pProtoMsg);
    int nSpeed = checkFan.speed();
    VR_LOGI("VrCheckFanSpeed().speed()=[%d]", nSpeed);
    return nSpeed;
}

void VR_HVACCheckFanSpeed::SetCurrFanSpeed(const VR_ProtoMessageBase &proto)
{
    const VrFanSpeedStateResp&  fanResp = (const VrFanSpeedStateResp&)proto;
    int nCurrSpeed = fanResp.fan_speed_state();
    m_nFanSpeed = nCurrSpeed;
    VR_LOGI("VrFanSpeedStateResp().fan_speed_state()=[%d]", m_nFanSpeed);
}

void VR_HVACCheckFanSpeed::OnReceive(const VR_ProtoMessageBase &proto)
{
    const VrAdditionalInfoRsp& additionInfoRsp = (const VrAdditionalInfoRsp&)proto;
    VR_LOGI("additionInfoRsp.response_id()=[%d]", additionInfoRsp.response_id());
    if (FanSpeed == additionInfoRsp.response_id()) {
        VR_LOGI("additionInfoRsp.response_id() == FanSpeed");
        const VrFanSpeedStateResp&  fanSpeed = additionInfoRsp.fan_speed_state_resp();
        SetCurrFanSpeed(fanSpeed);
        std::string replyMsg = GetReplyMsg();
        m_sender->sendEngineReply(m_nEngineId, replyMsg);
        SetActionComplete();
    }
}
/* EOF */









