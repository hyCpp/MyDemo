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
#include "VR_HVACValidTemperature.h"
#include "VR_HAVCTypedefine.h"
#include "VR_HVACXMLMessageBuilder.h"

#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

#define VR_SUCCESS        0
#define VR_ALREADY_AT_MIN 1
#define VR_ALREADY_AT_MAX 2

using namespace navi::VoiceRecog;
VR_HVACValidTemperature::VR_HVACValidTemperature(int nEngineId, int nActionType, const VR_ProtoMessageBase &ProtoMsg,
                                                         spVR_ActionEventSender pSender)
    : VR_ActionBase(pSender),
      m_nEngineId(nEngineId),
      m_nActionType(nActionType),
      m_pProtoMsg(ProtoMsg.New()),
      m_nLowTemp(0),
      m_nMaxTemp(0)
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_nEngineId, m_nActionType);
    m_pProtoMsg->CheckTypeAndMergeFrom(ProtoMsg);
}

VR_HVACValidTemperature::~VR_HVACValidTemperature()
{
    VR_LOGD_FUNC();
}

void VR_HVACValidTemperature::Run()
{
    VR_LOGI("GetActionId()=[%d]", GetActionId());
    SetTempRange();
    VR_VehicleInfoProxy::CreateInstance().GetTemperatureStatus(GetActionId());
}

void VR_HVACValidTemperature::SetTempRange()
{
    VR_HVACXMLMessageBuilder& msgBuilder = VR_HVACXMLMessageBuilder::GetMessageBuilder();
    m_nLowTemp = msgBuilder.GetLowTemp();
    m_nMaxTemp = msgBuilder.GetHighTemp();
    VR_LOGI("m_nLowTemp = [%d], m_nMaxTemp = [%d]", m_nLowTemp, m_nMaxTemp);
}

void VR_HVACValidTemperature::SetTempRange(int nLow, int nHigh)
{
    VR_LOGD_FUNC();
    VR_LOGI("nLow = [%d], nHigh = [%d]", nLow, nHigh);
    m_nLowTemp = nLow;
    m_nMaxTemp = nHigh;
}

std::string VR_HVACValidTemperature::GetReplyMsg(const VR_ProtoMessageBase& proto) const
{
    VR_LOGD_FUNC();

    std::string  resultXml = "";
    static  std::string  startXml =  "<action-result agent = \"climate\""
                                       " op = \"getValidTemperature\" errcode = \"0\" >"
                                       "<type>%1%</type>"
                                       "<status>%2%</status>"
                                       "<degree>%3%</degree>"
                                   "</action-result>";
    std::string strType("increase");
    std::string strStatus("0");
    std::string strDegree("max");
    const VrValidTemperature& validTemp = (const VrValidTemperature&)(*m_pProtoMsg);
    VrValidTemperatureType type = validTemp.type();
    const VrTemperatureResp& tempResp = (const VrTemperatureResp&)proto;
    int nTemperature = tempResp.temperature_state();
    float fTemp(VR_TEMP_INVALID);
    VR_LOGI("type=[%d], nTemperature=[%d]", type, nTemperature);

    switch (type) {
    case ValidTemperatureType_Increase:
        {
            strType = "increase";
            if (nTemperature == VR_TEMP_MAX) {
                strStatus = (boost::format("%1%") % VR_ALREADY_AT_MAX).str();
            }
            if (nTemperature >= VR_CEL_STEP_START && nTemperature <= VR_CEL_STEP_END) {
                fTemp = VR_CELSIUS + (nTemperature - VR_CEL_STEP_START) * VR_CEL_RATE;
                fTemp += VR_CEL_STEP;
            }
            else if (nTemperature >= VR_FAH_STEP_START && nTemperature <= VR_FAH_STEP_END) {
                fTemp = VR_FAHRENHEIT + (nTemperature - VR_FAH_STEP_START) * VR_TEMP_FLOAT;
                fTemp += VR_FAH_STEP;
            }
            else if (nTemperature == VR_TEMP_MIN) {
                if (m_nMaxTemp  * VR_TEMP_FLOAT <= VR_CELSIUS_MAX) {
                    fTemp = m_nLowTemp * VR_TEMP_FLOAT - VR_CEL_RATE + VR_CEL_STEP;
                }
                else {
                    fTemp = m_nLowTemp * VR_TEMP_FLOAT - VR_TEMP_FLOAT + VR_FAH_STEP;
                }
            }
            if (fTemp > m_nMaxTemp * VR_TEMP_FLOAT) {
                strDegree = (boost::format("%1%") % "max").str();
            }
            else {
                strDegree = (boost::format("%1%") % fTemp).str();
            }
            break;
        }
    case ValidTemperatureType_Decrease:
        {
            strType = "decrease";
            if (nTemperature == VR_TEMP_MIN) {
                strStatus = (boost::format("%1%") % VR_ALREADY_AT_MIN).str();
            }
            else if (nTemperature >= VR_CEL_STEP_START && nTemperature <= VR_CEL_STEP_END) {
                fTemp = VR_CELSIUS + (nTemperature - VR_CEL_STEP_START) * VR_CEL_RATE;
                fTemp -= VR_CEL_STEP;
            }
            else if (nTemperature >= VR_FAH_STEP_START && nTemperature <= VR_FAH_STEP_END) {
                fTemp = VR_FAHRENHEIT + (nTemperature - VR_FAH_STEP_START) * VR_TEMP_FLOAT;
                fTemp -= VR_FAH_STEP;
            }
            else if (nTemperature == VR_TEMP_MAX) {
                if (m_nMaxTemp * VR_TEMP_FLOAT <= VR_CELSIUS_MAX) {
                    fTemp = m_nMaxTemp * VR_TEMP_FLOAT + VR_CEL_RATE - VR_CEL_STEP;
                }
                else {
                    fTemp = m_nMaxTemp * VR_TEMP_FLOAT + VR_TEMP_FLOAT - VR_FAH_STEP;
                }
            }
            if (fTemp < m_nLowTemp * VR_TEMP_FLOAT) {
                strDegree = (boost::format("%1%") % "min").str();
            }
            else {
                strDegree = (boost::format("%1%") % fTemp).str();
            }
            break;
        }
    case ValidTemperatureType_Max:
        {
            strType = "max";
            if (nTemperature == VR_TEMP_MAX) {
                strStatus = (boost::format("%1%") % VR_ALREADY_AT_MAX).str();
            }
            break;
        }
    case validTemperatureTeyp_Min:
        {
            strType = "min";
            if (nTemperature == VR_TEMP_MIN) {
                strStatus = (boost::format("%1%") % VR_ALREADY_AT_MIN).str();
            }
            break;
        }
    default : break;
    }
    resultXml = (boost::format(startXml) % strType % strStatus % strDegree).str();
    VR_LOGI("[VR_HVACValidTemperature : replyMsg = %s\n]", resultXml.c_str());
    return resultXml;
}

void VR_HVACValidTemperature::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();

    const VrAdditionalInfoRsp& additionInfoRsp = (const VrAdditionalInfoRsp&)proto;
    VR_LOGI("additionInfoRsp.response_id()=[%d]", additionInfoRsp.response_id());
    if (Temperature == additionInfoRsp.response_id()) {
        VR_LOGI("Temperature == additionInfoRsp.response_id()");
        const VrTemperatureResp& tempResp = additionInfoRsp.temperature_resp();
        std::string replyMsg = GetReplyMsg(tempResp);
        m_sender->sendEngineReply(m_nEngineId, replyMsg);
        this->SetActionComplete();
    }
    else {
        VR_LOGI("VR_HVACValidTemperature aciton has a error. ");
    }
}
/* EOF */
