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
#include "VR_HVACStatusInfoAction.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACStatusInfoAction_Test
*
* The class is just for VR_HVACStatusInfoAction test.
*/
class VR_HVACStatusInfoAction_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACStatusInfoAction *m_Vr_StatusInfoAction;
    VR_HVACStatusInfoAction *m_Vr_AcStateReq;
    VR_HVACStatusInfoAction *m_Vr_TempReq;
    VR_HVACStatusInfoAction *m_Vr_TempRangeReq;
    VR_HVACStatusInfoAction *m_Vr_FanSpeedReq;
    VR_HVACStatusInfoAction *m_Vr_DualMode;
    VR_HVACStatusInfoAction *m_Vr_WiperDeicer;
    VR_HVACStatusInfoAction *m_Vr_Defogger;
    VR_HVACStatusInfoAction *m_Vr_BlowerMode;
    VR_HVACStatusInfoAction *m_Vr_ClimateConcierge;
    VR_HVACStatusInfoAction *m_Vr_InvalidOperator;
};

void
VR_HVACStatusInfoAction_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = AirConditionState;
     std::unique_ptr<VrAirConditionStateReq> ProtoMsg(VrAirConditionStateReq::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_StatusInfoAction = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *ProtoMsg, actionSender);

     nActionType = ACState;
     std::unique_ptr<VrACStateReq> acReq(VrACStateReq::default_instance().New());
     m_Vr_AcStateReq = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *acReq, actionSender);

     nActionType = Temperature;
     std::unique_ptr<VrTemperatureReq> tempReq(VrTemperatureReq::default_instance().New());
     m_Vr_TempReq = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *tempReq, actionSender);

     nActionType = TemperatureRange;
     std::unique_ptr<VrTemperatureRangeReq> tempRangReq(VrTemperatureRangeReq::default_instance().New());
     m_Vr_TempRangeReq = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *tempRangReq, actionSender);

     nActionType = FanSpeed;
     std::unique_ptr<VrFanSpeedStateReq> fanSpeed(VrFanSpeedStateReq::default_instance().New());
     m_Vr_FanSpeedReq = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *fanSpeed, actionSender);

     nActionType = DualMode;
     std::unique_ptr<VrDualModeReq>  dualMode(VrDualModeReq::default_instance().New());
     m_Vr_DualMode = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *dualMode, actionSender);

     nActionType = WiperDeicer;
     std::unique_ptr<VrWiperDeicerReq> wiperDeicer(VrWiperDeicerReq::default_instance().New());
     m_Vr_WiperDeicer = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *wiperDeicer, actionSender);

     nActionType = RearDefogger;
     std::unique_ptr<VrRearDefoggerStateReq> rearDefogger(VrRearDefoggerStateReq::default_instance().New());
     m_Vr_Defogger = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *rearDefogger, actionSender);

     nActionType = BlowerMode;
     std::unique_ptr<VrBlowerModeReq> blowermode(VrBlowerModeReq::default_instance().New());
     m_Vr_BlowerMode = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *blowermode, actionSender);

     nActionType = ClimateConciergeMode;
     std::unique_ptr<VrClimateConciergeModeReq> ClimateConcierge(VrClimateConciergeModeReq::default_instance().New());
     m_Vr_ClimateConcierge = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *ClimateConcierge, actionSender);

     nActionType = AudioON;
     std::unique_ptr<VrAudioON> AudioONReq(VrAudioON::default_instance().New());
     m_Vr_InvalidOperator = VR_new VR_HVACStatusInfoAction(nEngineId, nActionType, *AudioONReq, actionSender);
}

void
VR_HVACStatusInfoAction_Test::TearDown()
{
    if (NULL != m_Vr_StatusInfoAction) {
        delete m_Vr_StatusInfoAction;
        m_Vr_StatusInfoAction = NULL;
    }
    if (NULL != m_Vr_AcStateReq) {
        delete m_Vr_AcStateReq;
        m_Vr_AcStateReq = NULL;
    }
    if (NULL != m_Vr_TempReq) {
        delete m_Vr_TempReq;
        m_Vr_TempReq = NULL;
    }
    if (NULL != m_Vr_TempRangeReq) {
        delete m_Vr_TempRangeReq;
        m_Vr_TempRangeReq = NULL;
    }
    if (NULL != m_Vr_FanSpeedReq) {
        delete m_Vr_FanSpeedReq;
        m_Vr_FanSpeedReq = NULL;
    }
    if (NULL != m_Vr_DualMode) {
        delete m_Vr_DualMode;
        m_Vr_DualMode = NULL;
    }
    if (NULL != m_Vr_WiperDeicer) {
        delete m_Vr_WiperDeicer;
        m_Vr_WiperDeicer = NULL;
    }
    if (NULL != m_Vr_Defogger) {
        delete m_Vr_Defogger;
        m_Vr_Defogger = NULL;
    }
    if (NULL != m_Vr_BlowerMode) {
        delete m_Vr_BlowerMode;
        m_Vr_BlowerMode = NULL;
    }
    if (NULL != m_Vr_ClimateConcierge) {
        delete m_Vr_ClimateConcierge;
        m_Vr_ClimateConcierge = NULL;
    }
    if (NULL != m_Vr_InvalidOperator) {
        delete m_Vr_InvalidOperator;
        m_Vr_InvalidOperator = NULL;
    }
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_Case)
{
    m_Vr_StatusInfoAction->Run();
    m_Vr_InvalidOperator->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_AC_Case)
{
    m_Vr_AcStateReq->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_Temp_Case)
{
    m_Vr_TempReq->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_TempRange_Case)
{
    m_Vr_TempRangeReq->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_Fan_Speed_Case)
{
    m_Vr_FanSpeedReq->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_Dual_Mode_Case)
{
    m_Vr_DualMode->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_Wiper_Case)
{
    m_Vr_WiperDeicer->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_RearDefogger_Case)
{
    m_Vr_Defogger->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_BlowerMode_Case)
{
    m_Vr_BlowerMode->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, Run_ClimateConcierge_Case)
{
    m_Vr_ClimateConcierge->Run();
    SUCCEED();
}

TEST_F(VR_HVACStatusInfoAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionalInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoResp->set_response_id(AirConditionState);
    VrAirConditionStateResp* airconResp = additionalInfoResp->mutable_air_condition_state_resp();
    airconResp->set_air_condition_state(1);

    m_Vr_StatusInfoAction->OnReceive(*additionalInfoResp);
    SUCCEED();
}
/* EOF */

