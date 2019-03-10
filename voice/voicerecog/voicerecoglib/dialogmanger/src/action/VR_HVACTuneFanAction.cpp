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
 *  @file   VR_HVACTuneFanAction.cpp
 *  @brief  Implementation class of VR_HVACTuneFanAction
 */

#include "stdafx.h"
#include "VR_HAVCTypedefine.h"
#include "VR_ActionEventSender.h"
#include "VR_HVACTuneFanAction.h"

#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

#ifndef VR_HVAC_ACTION_CASE
#define VR_HVAC_ACTION_CASE(MSG_TYPE, MSG_FUNC, ACTION_ID, ACTION_VALUE) \
    case (MSG_TYPE): \
    {   \
        pVehicleProxy->MSG_FUNC(ACTION_ID, ACTION_VALUE);   \
    }   \
    break
#endif

#ifndef VR_HVAC_ACTION_END
#define VR_HVAC_ACTION_END   \
default :  \
    break
#endif

using namespace navi::VoiceRecog;

VR_HVACTuneFanAction::VR_HVACTuneFanAction(int nEngineId, int nActionType, const VR_ProtoMessageBase &ProtoMsg,
                                           spVR_ActionEventSender pSender)
    : VR_ActionBase(pSender),
      m_nEngineId(nEngineId),
      m_nActionType(nActionType),
      m_pProtoMsg(ProtoMsg.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_nEngineId, m_nActionType);
    m_pProtoMsg->CheckTypeAndMergeFrom(ProtoMsg);
}

VR_HVACTuneFanAction::~VR_HVACTuneFanAction()
{
    VR_LOGD_FUNC();
}

int  VR_HVACTuneFanAction::GetFanSpeed() const
{
    int nFanSpeed(-1);
    if (m_nActionType == ChangeFanSpeed) {
        VR_LOGI("m_nActionType == ChangeFanSpeed");
        const VrChangeFanSpeed* pChangeSpeed = static_cast<const VrChangeFanSpeed*>(m_pProtoMsg.get());
        nFanSpeed = pChangeSpeed->fan_speed();
    }
    VR_LOGI("m_nActionType=[%d], nFanSpeed=[%d]", m_nActionType, nFanSpeed);
    return nFanSpeed;
}

void VR_HVACTuneFanAction::Run()
{
    int nActionId = GetActionId();
    VR_LOGI("GetActionId()=[%d], m_nActionType=[%d]", nActionId, m_nActionType);
    VR_VehicleInfoProxy* pVehicleProxy = &VR_VehicleInfoProxy::CreateInstance();
    switch (m_nActionType) {
    VR_HVAC_ACTION_CASE(ChangeFanSpeed, ChangeCurrentFanSpeed, nActionId, GetFanSpeed());
    VR_HVAC_ACTION_CASE(FanSpeedMax, ChangeCurrentFanSpeed, nActionId, fan_max);
    VR_HVAC_ACTION_CASE(FanSpeedMin, ChangeCurrentFanSpeed, nActionId, fan_min);
    VR_HVAC_ACTION_CASE(BlowerModeFace, ChangeFanWindMode, nActionId, face);
    VR_HVAC_ACTION_CASE(BlowerModeFoot, ChangeFanWindMode, nActionId, foot);
    VR_HVAC_ACTION_CASE(BlowerModeFaceAndFoot, ChangeFanWindMode, nActionId, face_and_foot);
    VR_HVAC_ACTION_CASE(BlowerModeWindowAndFoot, ChangeFanWindMode, nActionId, foot_and_window);
    default:
        break;
    }
}

void VR_HVACTuneFanAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    m_sender->sendActionReply(m_nEngineId, proto);
    SetActionComplete();
}
/* EOF */







