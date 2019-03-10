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
 *  @file  VR_HVACTuneClimateAction.cpp
 *  @brief  Implementaion class of VR_HAVCTuneClimateAction
 */

#include "stdafx.h"
#include "VR_HVACTuneClimateAction.h"
#include "VR_ActionEventSender.h"


#ifndef VR_HVAC_ACTION_CASE
#define VR_HVAC_ACTION_CASE(MSG_TYPE, MSG_FUNC, ACTION_ID, ACTION_VALUE)  \
    case (MSG_TYPE) :   \
    {   \
        pVehicleProxy->MSG_FUNC(ACTION_ID, ACTION_VALUE);  \
    }   \
    break
#endif

#ifndef VR_HVAC_ACTION_END
#define VR_HVAC_ACTION_END   \
default :    \
    break
#endif

#ifndef VR_HVAC_SPEC_ACTION_CASE
#define VR_HAVC_SPEC_ACTION_CASE(MSG_TYPE, MSG_CLASS, MSG_BASE ) \
    case (MSG_TYPE) : \
    {  \
        const MSG_CLASS* pMsg = static_cast<const MSG_CLASS*>(MSG_BASE);   \
        this->RunAction(*pMsg);  \
    }    \
    break
#endif

using namespace navi::VoiceRecog;

VR_HVACTuneClimateAction::VR_HVACTuneClimateAction(int nEngineId, int nActionType, const VR_ProtoMessageBase &ProtoMsg,
                                                   spVR_ActionEventSender pSender)
    : VR_ActionBase(pSender),
      m_nEngineId(nEngineId),
      m_nActionType(nActionType),
      m_pProtoMsg(ProtoMsg.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_nEngineId, m_nActionType);
    m_pProtoMsg->CheckTypeAndMergeFrom(ProtoMsg);
}

VR_HVACTuneClimateAction::~VR_HVACTuneClimateAction()
{
    VR_LOGD_FUNC();
}

template<class MsgType> void VR_HVACTuneClimateAction::RunAction(const MsgType& msgType)
{
    VR_LOGD_FUNC();
}

template<> void VR_HVACTuneClimateAction::RunAction(const VrChangeTemperature& msgProto)
{
    Temperature_Mode  mode = temperature_reserved;
    float fTemp = GetTemperatureValue(mode, msgProto);
    int nActionId = GetActionId();
    VR_LOGI("actionId=[%d],Temperature_Mode=[%d]", mode);
    VR_VehicleInfoProxy* pVehicleProxy = &VR_VehicleInfoProxy::CreateInstance();
    switch (mode) {
    VR_HVAC_ACTION_CASE(temperature_min, SetTemperature, nActionId, temperature_min);
    VR_HVAC_ACTION_CASE(temperature_max, SetTemperature, nActionId, temperature_max);
    VR_HVAC_ACTION_CASE(temperature_reserved, ChangeCurrentTemperature, nActionId, fTemp);
    default:
        break;
    }
}

float VR_HVACTuneClimateAction::GetTemperatureValue(Temperature_Mode& mode,
                                                    const VrChangeTemperature& proto) const
{
    float fTempValue(VR_TEMP_INVALID);
    int nStep = proto.temperature();
    if (nStep == VR_TEMP_MIN) {
        mode = temperature_min;
    }
    else if (nStep >= VR_CEL_STEP_START && nStep <= VR_CEL_STEP_END) {
        fTempValue = VR_CELSIUS + VR_CEL_RATE * ((nStep - VR_CEL_STEP_START) * VR_TEMP_FLOAT);
    }
    else if (nStep >= VR_FAH_STEP_START && nStep <= VR_FAH_STEP_END) {
        fTempValue = VR_FAHRENHEIT + (nStep - VR_FAH_STEP_START) * VR_TEMP_FLOAT;
    }
    else if (nStep == VR_TEMP_MAX) {
        mode = temperature_max;
    }
    else {

    }
    VR_LOGI("nStep=[%d],fTempValue=[%f]", nStep, fTempValue);
    return fTempValue;
}

void VR_HVACTuneClimateAction::Run()
{
    int nActionId = GetActionId();
    VR_LOGI("GetActionId()=[%d], action type=[%d]", nActionId, m_nActionType);
    VR_VehicleInfoProxy* pVehicleProxy = &VR_VehicleInfoProxy::CreateInstance();
    switch (m_nActionType) {
    VR_HAVC_SPEC_ACTION_CASE(ChangeTemperature, VrChangeTemperature, m_pProtoMsg.get());
    VR_HVAC_ACTION_CASE(DecreaseTemperature, SetTemperature, nActionId, cooler);
    VR_HVAC_ACTION_CASE(IncreaseTemperature, SetTemperature, nActionId, warmer);
    VR_HVAC_ACTION_CASE(AFewWarmer, SetTemperature, nActionId, a_few_warmer);
    VR_HVAC_ACTION_CASE(AFewCooler, SetTemperature, nActionId, a_few_cooler);
    VR_HVAC_ACTION_CASE(TemperatureMax, SetTemperature, nActionId, temperature_max);
    VR_HVAC_ACTION_CASE(TemperatureMin, SetTemperature, nActionId, temperature_min);
    default:
        break;
    }
}

void VR_HVACTuneClimateAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    m_sender->sendActionReply(m_nEngineId, proto);
    SetActionComplete();
}
/* EOF */
