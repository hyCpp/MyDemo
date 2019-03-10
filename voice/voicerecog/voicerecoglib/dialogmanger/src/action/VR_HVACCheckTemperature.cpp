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
 * @file   VR_HVACCheckTemperature.cpp
 * @brief  Implementation class of VR_HVACCheckTemperature
 */

#include "stdafx.h"
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#include "VR_ActionEventSender.h"
#include "VR_HVACCheckTemperature.h"
#include "VR_HVACXMLMessageBuilder.h"
#include "VR_HAVCTypedefine.h"
#include "pugixml.hpp"

#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

#define VR_SUCCESS                  "0"
#define VR_ALREADY_AT_REQUEST_TEMP  1
#define VR_OVER_MAX_VALUE           2
#define VR_UNDER_MIN_VALUE          3

using namespace navi::VoiceRecog;
VR_HVACCheckTemperature::VR_HVACCheckTemperature(int nEngineId, int nActionType, const VR_ProtoMessageBase &ProtoMsg,
                                                         spVR_ActionEventSender pSender)
    : VR_ActionBase(pSender),
      m_nEngineId(nEngineId),
      m_nActionType(nActionType),
      m_pProtoMsg(ProtoMsg.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_nEngineId, m_nActionType);
    m_pProtoMsg->CheckTypeAndMergeFrom(ProtoMsg);
    m_nLowTemp =  0;
    m_nHighTemp = 0;
    m_nCurrTemp = 0;
}

VR_HVACCheckTemperature::~VR_HVACCheckTemperature()
{
    VR_LOGD_FUNC();
}

void VR_HVACCheckTemperature::Run()
{
    VR_LOGD_FUNC();
    SetTempRange();
    VR_VehicleInfoProxy::CreateInstance().GetTemperatureStatus(GetActionId());
}

std::string VR_HVACCheckTemperature::GetReplyMsg() const
{
    VR_LOGD_FUNC();

    std::string  resultXml = "";
    static  std::string  startXml =  "<action-result agent = \"climate\""
                                       " op = \"checkTemperature\" errcode = \"0\" >"
                                       "<status>%1%</status>"
                                   "</action-result>";

    std::string strStatus("0");
    float fTemperature = GetCheckTemp();
    float fCurrTemp(VR_TEMP_FLOAT);
    if (m_nCurrTemp >= VR_CEL_STEP_START && m_nCurrTemp <= VR_CEL_STEP_END) {
        VR_LOGI("m_nCurrTemp >= VR_CEL_STEP_START && m_nCurrTemp <= VR_CEL_STEP_END");
        fCurrTemp = VR_CELSIUS + (m_nCurrTemp - VR_CEL_STEP_START) * VR_CEL_RATE;
    }
    else if (m_nCurrTemp >= VR_FAH_STEP_START && m_nCurrTemp <= VR_FAH_STEP_END) {
        VR_LOGI("m_nCurrTemp >= FAH_STEP_START && m_nCurrTemp <= FAH_STEP_END");
        fCurrTemp = VR_FAHRENHEIT + (m_nCurrTemp - VR_FAH_STEP_START) * VR_TEMP_FLOAT;
    }
    if (fTemperature > m_nHighTemp * VR_TEMP_FLOAT) {
        VR_LOGI("fTemperature > m_nHighTemp * TEMP_FLOAT");
        strStatus = (boost::format("%1%") % VR_OVER_MAX_VALUE).str();
    }
    else if (fTemperature < m_nLowTemp * VR_TEMP_FLOAT) {
        VR_LOGI("fTemperature < m_nLowTemp * TEMP_FLOAT");
        strStatus = (boost::format("%1%") % VR_UNDER_MIN_VALUE).str();
    }
    else if (fTemperature == fCurrTemp) {
        VR_LOGI("fTemperature == fCurrTemp");
        strStatus = (boost::format("%1%") % VR_ALREADY_AT_REQUEST_TEMP).str();
    }
    else {
        strStatus = (boost::format("%1%") % VR_SUCCESS).str();
    }

    resultXml = (boost::format(startXml) % strStatus).str();
    VR_LOGI("[VR_HVACCheckTemperature : replyMsg = %s\n]", resultXml.c_str());
    return  resultXml;
}

float  VR_HVACCheckTemperature::GetCheckTemp() const
{
    const VrCheckTemperature& checkTemp = (const VrCheckTemperature&)(*m_pProtoMsg);
    float fTemp = checkTemp.degree();
    VR_LOGI("VrCheckTemperature().degree()=[%f]", fTemp);
    return fTemp;
}
void  VR_HVACCheckTemperature::SetTempRange()
{
    VR_HVACXMLMessageBuilder&  msgBuilder = VR_HVACXMLMessageBuilder::GetMessageBuilder();
    m_nLowTemp = msgBuilder.GetLowTemp();
    m_nHighTemp = msgBuilder.GetHighTemp();
    VR_LOGI("m_nLowTemp = [%d], m_nHighTemp = [%d]", m_nLowTemp, m_nHighTemp);
}

void  VR_HVACCheckTemperature::SetTempRange(const VR_ProtoMessageBase &proto)
{
    const VrTemperatureRangeResp& range = (const VrTemperatureRangeResp&)proto;
    m_nLowTemp = range.min_temperature();
    m_nHighTemp = range.max_temperature();
    VR_LOGI("VrTemperatureRangeResp().min_temperature()=[%d]", m_nLowTemp);
    VR_LOGI("VrTemperatureRangeResp().max_temperature()=[%d]", m_nHighTemp);
}

void VR_HVACCheckTemperature::SetCurrTemp(const VR_ProtoMessageBase &proto)
{
    const VrTemperatureResp&  temp = (const VrTemperatureResp&)proto;
    m_nCurrTemp = temp.temperature_state();
    VR_LOGI("VrTemperatureResp().temperature_state()=[%d]", m_nCurrTemp);
}

void VR_HVACCheckTemperature::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    const VrAdditionalInfoRsp& additionInfoRsp = (const VrAdditionalInfoRsp&)proto;
    if (Temperature == additionInfoRsp.response_id()) {
        VR_LOGI("Temperature == additionInfoRsp.response_id()");
        const VrTemperatureResp& temp = additionInfoRsp.temperature_resp();
        SetCurrTemp(temp);
        std::string strMsg = GetReplyMsg();
        m_sender->sendEngineReply(m_nEngineId, strMsg);
        SetActionComplete();
    }
    else {
        VR_LOGI("additionInfoRsp.response_id()=[%d]", additionInfoRsp.response_id());
    }
}
/* EOF */
