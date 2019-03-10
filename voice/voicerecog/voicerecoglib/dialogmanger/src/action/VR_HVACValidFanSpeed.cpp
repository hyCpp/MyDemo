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
 * @file   VR_HVACValidFanSpeed.cpp
 * @brief  Implementation class of VR_HVACValidFanSpeed
 */

#include "stdafx.h"
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#include "VR_ActionEventSender.h"
#include "VR_HVACValidFanSpeed.h"
#include "VR_HAVCTypedefine.h"
#include "VR_HVACXMLMessageBuilder.h"

#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

#define VR_ALREADY_AT_MIN  1
#define VR_ALREADY_AT_MAX  2

using namespace navi::VoiceRecog;
VR_HVACValidFanSpeed::VR_HVACValidFanSpeed(int nEngineId, int nActionType, const VR_ProtoMessageBase &ProtoMsg,
                                                         spVR_ActionEventSender pSender)
    : VR_ActionBase(pSender),
      m_nEngineId(nEngineId),
      m_nActionType(nActionType),
      m_pProtoMsg(ProtoMsg.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_nEngineId, m_nActionType);
    m_pProtoMsg->CheckTypeAndMergeFrom(ProtoMsg);
}

VR_HVACValidFanSpeed::~VR_HVACValidFanSpeed()
{
    VR_LOGD_FUNC();
}

void VR_HVACValidFanSpeed::Run()
{
    VR_LOGI("GetActionId()=[%d]", GetActionId());
    VR_VehicleInfoProxy::CreateInstance().GetFanSpeedStatus(GetActionId());
}

std::string VR_HVACValidFanSpeed::GetReplyMsg(const VR_ProtoMessageBase& proto) const
{
    std::string  resultXml = "";
    static  std::string  startXml =  "<action-result agent = \"climate\""
                                       " op = \"getValidFanSpeed\" errcode = \"0\" >"
                                       "<type>%1%</type>"
                                       "<status>%2%</status>"
                                       "<speed>%3%</speed>"
                                   "</action-result>";

    std::string strType = "";
    std::string strStatus = "";
    std::string strSpeed = "";
    const VrValidFanSpeed& validTemp = (const VrValidFanSpeed&)(*m_pProtoMsg);
    VrValidFanSpeedType type = validTemp.type();

    const VrFanSpeedStateResp& fanResp = (const VrFanSpeedStateResp&)proto;
    int nSpeed = fanResp.fan_speed_state();
    VR_LOGI("type=[%d], nSpeed=[%d]", type, nSpeed);
    if (type == ValidFanSpeedType_Increase) {
        strType = "increase";
        bool bBlowerStep = VR_HVACXMLMessageBuilder::GetMessageBuilder().GetBlowStep();
        int  nBlowerStep = bBlowerStep ? VR_FAN_SPEED_7 : VR_FAN_SPEED_5;
        if (nSpeed == nBlowerStep) {
            strStatus = (boost::format("%1%") % VR_ALREADY_AT_MAX).str();
        }
        else {
            nSpeed += VR_FAN_STEP;
            strSpeed = (boost::format("%1%") % nSpeed).str();
            strStatus = "0";
        }
    }
    else if (type == ValidFanSpeedType_Decrease) {
        strType = "decrease";
        if (nSpeed == VR_FAN_SPEED_1) {
            strStatus = (boost::format("%1%") % VR_ALREADY_AT_MIN).str();
        }
        else {
            nSpeed -= VR_FAN_STEP;
            strSpeed = (boost::format("%1%") % nSpeed).str();
            strStatus = "0";
        }
    }

    resultXml = (boost::format(startXml) % strType % strStatus % strSpeed).str();
    VR_LOGI("[VR_HVACValidFanSpeed : replyMsg = %s]", resultXml.c_str());
    return resultXml;
}

void VR_HVACValidFanSpeed::OnReceive(const VR_ProtoMessageBase &proto)
{
    const VrAdditionalInfoRsp& additionInfoRsp = (const VrAdditionalInfoRsp&)proto;
    VR_LOGI("additionInfoRsp.response_id()=[%d]", additionInfoRsp.response_id());

    if (FanSpeed == additionInfoRsp.response_id()) {
        VR_LOGI("FanSpeed == additionInfoRsp.response_id()");
        const VrFanSpeedStateResp& fanResp = additionInfoRsp.fan_speed_state_resp();
        std::string replyMsg = GetReplyMsg(fanResp);
        m_sender->sendEngineReply(m_nEngineId, replyMsg);
    }
    SetActionComplete();
}
/* EOF */







