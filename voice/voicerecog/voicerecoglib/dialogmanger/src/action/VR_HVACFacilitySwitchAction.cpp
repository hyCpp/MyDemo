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
 * @file   VR_HAVCFacilitySwitchAction.cpp
 * @brief  Implementation class of VR_HVACFacilitySwitch
 */

#include "stdafx.h"
#include "VR_ActionEventSender.h"
#include "VR_HVACFacilitySwitchAction.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

#ifndef VR_HVAC_ACTION_CASE
#define VR_HVAC_ACTION_CASE(MSG_TYPE, MSG_FUNC, ACTION_ID, ACTION_STATUS) \
    case (MSG_TYPE): \
    {  \
        pVehicleProxy->MSG_FUNC(ACTION_ID, ACTION_STATUS); \
    }  \
    break
#endif

#ifndef VR_HVAC_ACTION_END
#define VR_HVAC_ACTION_END   \
default :  \
    break
#endif

using namespace navi::VoiceRecog;

VR_HVACFacilitySwitchAction::VR_HVACFacilitySwitchAction(int nEngineId, int nActionType, const VR_ProtoMessageBase &ProtoMsg,
                                                         spVR_ActionEventSender pSender)
    : VR_ActionBase(pSender)
    , m_nEngineId(nEngineId)
    , m_nActionType(nActionType)
    , m_pProtoMsg(ProtoMsg.New())
{
    VR_LOGD("EngineReqId = [%d], action type = [%d]", m_nEngineId, m_nActionType);
    m_pProtoMsg->CheckTypeAndMergeFrom(ProtoMsg);
}

VR_HVACFacilitySwitchAction::~VR_HVACFacilitySwitchAction()
{
    VR_LOGD_FUNC();
}

void VR_HVACFacilitySwitchAction::Run()
{
    int nActionId = GetActionId();
    VR_LOGD("actionId = [%d], action type = [%d]", nActionId, m_nActionType);
    VR_VehicleInfoProxy* pVehicleProxy = &VR_VehicleInfoProxy::CreateInstance();

    switch (m_nActionType) {
    VR_HVAC_ACTION_CASE(AutoAirConditionerON,      SetAutoStatus,              nActionId, true);
    VR_HVAC_ACTION_CASE(AutoAirConditionerOFF,     SetAutoStatus,              nActionId, false);
    VR_HVAC_ACTION_CASE(ACON,                      SetACStatus,                nActionId, true);
    VR_HVAC_ACTION_CASE(ACOFF,                     SetACStatus,                nActionId, false);
    VR_HVAC_ACTION_CASE(DualModeON,                SetDaulModeStatus,          nActionId, true);
    VR_HVAC_ACTION_CASE(DualModeOFF,               SetDaulModeStatus,          nActionId, false);
    VR_HVAC_ACTION_CASE(WiperDeicerON,             SetWiperDeicerStatus,       nActionId, true);
    VR_HVAC_ACTION_CASE(WiperDeicerOFF,            SetWiperDeicerStatus,       nActionId, false);
    VR_HVAC_ACTION_CASE(RearSeatAirConditionerON,  SetRearSeatAutoStatus,      nActionId, true);
    VR_HVAC_ACTION_CASE(RearSeatAirConditionerOFF, SetRearSeatAutoStatus,      nActionId, false);
    VR_HVAC_ACTION_CASE(RearDefoggerON,            SetRearDefoggerStatus,      nActionId, true);
    VR_HVAC_ACTION_CASE(RearDefoggerOFF,           SetRearDefoggerStatus,      nActionId, false);
    VR_HVAC_ACTION_CASE(ClimateConciergeModeON,    SetClimateConciergeStatus,  nActionId, true);
    default :
        break;
    }
}

void VR_HVACFacilitySwitchAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();

    m_sender->sendActionReply(m_nEngineId, proto);
    this->SetActionComplete();
}
/* EOF */
