/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

#include "gtest/gtest.h"
#include "stdafx.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_ClimateStatusInfoAction_VrAgent.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateStatusInfoAction_VrAgent_Test
*
* The class is just for VR_ClimateStatusInfoAction_VrAgent test.
*/
class VR_ClimateStatusInfoAction_VrAgent_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateStatusInfoAction_VrAgent *m_Vr_StatusInfoAction;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_ACState;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_Temperature;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_FanSpeed;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_DualMode;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_WiperDeicer;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_RearDefogger;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_BlowerMode;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_ClimateConciergeMode;
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_TemperatureRange;
    ////////////////// test other ////////////////////////////////////////
    VR_ClimateStatusInfoAction_VrAgent *m_Vr_ClimateTestOther;
};

void
VR_ClimateStatusInfoAction_VrAgent_Test::SetUp()
{
    int nEngineId = 101;
    int nActionType = AirConditionState;
    std::unique_ptr<VrAirConditionStateReq> ProtoMsg(VrAirConditionStateReq::default_instance().New());
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock());
    m_Vr_StatusInfoAction = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *ProtoMsg, actionSender, avclanSender);
    //////////////////////  ACState ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = ACState;
    std::unique_ptr<VrACStateReq> AcState(VrACStateReq::default_instance().New());
    m_Vr_ACState = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *AcState, actionSender, avclanSender);
    //////////////////////  Temperature ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = Temperature;
    std::unique_ptr<VrTemperatureReq> TempReq(VrTemperatureReq::default_instance().New());
    m_Vr_Temperature = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *TempReq, actionSender, avclanSender);
    //////////////////////  FanSpeed ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = FanSpeed;
    std::unique_ptr<VrFanSpeedStateReq> FanSpeedReq(VrFanSpeedStateReq::default_instance().New());
    m_Vr_FanSpeed = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *FanSpeedReq, actionSender, avclanSender);
    //////////////////////  DualMode ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = DualMode;
    std::unique_ptr<VrDualModeReq> DualModeReq(VrDualModeReq::default_instance().New());
    m_Vr_DualMode = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *DualModeReq, actionSender, avclanSender);
    //////////////////////  WiperDeicer ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = WiperDeicer;
    std::unique_ptr<VrWiperDeicerReq> WiperDeicerReq(VrWiperDeicerReq::default_instance().New());
    m_Vr_WiperDeicer = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *WiperDeicerReq, actionSender, avclanSender);
    //////////////////////  RearDefogger ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = RearDefogger;
    std::unique_ptr<VrRearDefoggerStateReq> RearDefoggerReq(VrRearDefoggerStateReq::default_instance().New());
    m_Vr_RearDefogger = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *RearDefoggerReq, actionSender, avclanSender);
    //////////////////////  BlowerMode ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = BlowerMode;
    std::unique_ptr<VrBlowerModeReq> BlowerModeReq(VrBlowerModeReq::default_instance().New());
    m_Vr_BlowerMode = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *BlowerModeReq, actionSender, avclanSender);
    //////////////////////  ClimateConciergeMode ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = ClimateConciergeMode;
    std::unique_ptr<VrClimateConciergeModeReq> ClimateConciergeModeReq(VrClimateConciergeModeReq::default_instance().New());
    m_Vr_ClimateConciergeMode = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *ClimateConciergeModeReq, actionSender, avclanSender);
    //////////////////////  TemperatureRange ////////////////////////////////////////////////////////////////////////////////////////////////
    nActionType = TemperatureRange;
    std::unique_ptr<VrTemperatureRangeReq> TemperatureRangeReq(VrTemperatureRangeReq::default_instance().New());
    m_Vr_TemperatureRange = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *TemperatureRangeReq, actionSender, avclanSender);

    nActionType = RadioState;
    std::unique_ptr<VrRadioStateReq> RadioStateReq(VrRadioStateReq::default_instance().New());
    m_Vr_ClimateTestOther = VR_new VR_ClimateStatusInfoAction_VrAgent(nEngineId, nActionType, *RadioStateReq, actionSender, avclanSender);
}

void
VR_ClimateStatusInfoAction_VrAgent_Test::TearDown()
{
    if (NULL != m_Vr_StatusInfoAction) {
        delete m_Vr_StatusInfoAction;
        m_Vr_StatusInfoAction = NULL;
    }
    if (NULL != m_Vr_ACState) {
        delete m_Vr_ACState;
        m_Vr_ACState = NULL;
    }
    if (NULL != m_Vr_Temperature) {
        delete m_Vr_Temperature;
        m_Vr_Temperature = NULL;
    }
    if (NULL != m_Vr_FanSpeed) {
        delete m_Vr_FanSpeed;
        m_Vr_FanSpeed = NULL;
    }
    if (NULL != m_Vr_DualMode) {
        delete m_Vr_DualMode;
        m_Vr_DualMode = NULL;
    }
    if (NULL != m_Vr_WiperDeicer) {
        delete m_Vr_WiperDeicer;
        m_Vr_WiperDeicer = NULL;
    }
    if (NULL != m_Vr_RearDefogger) {
        delete m_Vr_RearDefogger;
        m_Vr_RearDefogger = NULL;
    }
    if (NULL != m_Vr_BlowerMode) {
        delete m_Vr_BlowerMode;
        m_Vr_BlowerMode = NULL;
    }
    if (NULL != m_Vr_ClimateConciergeMode) {
        delete m_Vr_ClimateConciergeMode;
        m_Vr_ClimateConciergeMode = NULL;
    }
    if (NULL != m_Vr_TemperatureRange) {
        delete m_Vr_TemperatureRange;
        m_Vr_TemperatureRange = NULL;
    }
    if (NULL != m_Vr_ClimateTestOther) {
        delete m_Vr_ClimateTestOther;
        m_Vr_ClimateTestOther = NULL;
    }
}

TEST_F(VR_ClimateStatusInfoAction_VrAgent_Test, Run_Case)
{
    m_Vr_StatusInfoAction->Run();
    m_Vr_ACState->Run();
    m_Vr_Temperature->Run();
    m_Vr_FanSpeed->Run();
    m_Vr_DualMode->Run();
    m_Vr_WiperDeicer->Run();
    m_Vr_RearDefogger->Run();
    m_Vr_BlowerMode->Run();
    m_Vr_ClimateConciergeMode->Run();
    m_Vr_TemperatureRange->Run();
    m_Vr_ClimateTestOther->Run();
    SUCCEED();
}

TEST_F(VR_ClimateStatusInfoAction_VrAgent_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionalInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoResp->set_response_id(AirConditionState);
    VrAirConditionStateResp* airconResp = additionalInfoResp->mutable_air_condition_state_resp();
    airconResp->set_air_condition_state(1);

    m_Vr_StatusInfoAction->OnReceive(*additionalInfoResp);
    SUCCEED();
}
/* EOF */


