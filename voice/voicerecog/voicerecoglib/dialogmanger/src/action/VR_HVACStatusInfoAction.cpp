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
 * @file  VR_HVACStatusInfoAction.cpp
 * @brief Implementation class of VR_HVACStatusInfoAction
 */

#include "stdafx.h"
#include "VR_VehicleInfoApi.h"
#include "VR_HVACStatusInfoAction.h"
#include "VR_ActionEventSender.h"
#include "VR_HVACXMLMessageBuilder.h"
#include "boost/format.hpp"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

#ifndef VR_HVAC_ACTION_CASE
#define VR_HVAC_ACTION_CASE(MSG_TYPE, MSG_FUNC, ACTION_ID) \
    case (MSG_TYPE): \
    { \
        pVehicleProxy->MSG_FUNC(ACTION_ID); \
    } \
    break
#endif

#ifndef VR_HVAC_ACTION_END
#define VR_HVAC_ACTION_END   \
default :  \
    break
#endif

using namespace navi::VoiceRecog;

VR_HVACStatusInfoAction::VR_HVACStatusInfoAction(int nEengineId, int nActionType, const VR_ProtoMessageBase &protoMsg,
                                                 spVR_ActionEventSender pSender)
    : VR_ActionBase(pSender),
      m_nEngineId(nEengineId),
      m_nActionType(nActionType),
      m_pReqMsg(protoMsg.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_nEngineId, m_nActionType);
    m_pReqMsg->CheckTypeAndMergeFrom(protoMsg);
}


VR_HVACStatusInfoAction::~VR_HVACStatusInfoAction()
{
    VR_LOGD_FUNC();
}

void VR_HVACStatusInfoAction::Run()
{
    int nActionId = GetActionId();
    VR_LOGI("GetActionId=[%d], action type=[%d]", nActionId, m_nActionType);
    // Temperature range is a relatively fixed values, so make it in msg builder...
    if (TemperatureRange == m_nActionType) {
        VR_HVACXMLMessageBuilder& msgBuilder = VR_HVACXMLMessageBuilder::GetMessageBuilder();
        int nLowTemp = msgBuilder.GetLowTemp();
        int nHighTemp = msgBuilder.GetHighTemp();
        static  VR_String  startXml =  "<action-result agent = \"climate\""
                                           " op = \"queryTemperatureRange\" errcode = \"0\" >"
                                           "<mindegree>%1%</mindegree>"
                                           "<maxdegree>%2%</maxdegree>"
                                       "</action-result>";
        VR_String resultXml = (boost::format(startXml) % nLowTemp % nHighTemp).str();
        VR_LOGI("resultXml = [%s]", resultXml.c_str());
        m_sender->sendEngineReply(m_nEngineId, resultXml);
        this->SetActionComplete();
        return;
    }
    VR_VehicleInfoProxy* pVehicleProxy = &VR_VehicleInfoProxy::CreateInstance();
    switch (m_nActionType) {
    VR_HVAC_ACTION_CASE(AirConditionState, GetAutoStatus, nActionId);
    VR_HVAC_ACTION_CASE(ACState, GetACStatus, nActionId);
    VR_HVAC_ACTION_CASE(Temperature, GetTemperatureStatus, nActionId);
    // VR_HVAC_ACTION_CASE(TemperatureRange, GetTempRangleStatus, nActionId);
    VR_HVAC_ACTION_CASE(FanSpeed, GetFanSpeedStatus, nActionId);
    VR_HVAC_ACTION_CASE(DualMode, GetDaulModeStatus, nActionId);
    VR_HVAC_ACTION_CASE(WiperDeicer, GetWiperDeicerStatus, nActionId);
    VR_HVAC_ACTION_CASE(RearDefogger, GetRearDefoggerStatus, nActionId);
    VR_HVAC_ACTION_CASE(BlowerMode, GetBlowerModeStatus, nActionId);
    VR_HVAC_ACTION_CASE(ClimateConciergeMode, GetClimateConciergeModeStatus, nActionId);
    default :
        break;
    }
}

void  VR_HVACStatusInfoAction::OnReceive(const VR_ProtoMessageBase &protoMsg)
{
    VR_LOGD_FUNC();
    m_sender->sendInfoRequest(m_nEngineId, protoMsg);
    SetActionComplete();
}
/* EOF */
