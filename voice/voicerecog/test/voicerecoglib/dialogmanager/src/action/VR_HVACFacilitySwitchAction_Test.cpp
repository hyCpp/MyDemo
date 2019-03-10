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
#include "VR_HVACFacilitySwitchAction.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACFacilitySwitchAction_Test
*
* The class is just for VR_HVACFacilitySwitchAction test.
*/
class VR_HVACFacilitySwitchAction_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACFacilitySwitchAction *m_Vr_FacilifySwitchAction;
    VR_HVACFacilitySwitchAction *m_Vr_AutoAirConditionOff;
    VR_HVACFacilitySwitchAction *m_Vr_ACON;
    VR_HVACFacilitySwitchAction *m_Vr_ACOFF;
    VR_HVACFacilitySwitchAction *m_Vr_DualModeOn;
    VR_HVACFacilitySwitchAction *m_Vr_DualModeOff;
    VR_HVACFacilitySwitchAction *m_Vr_WiperDeicerON;
    VR_HVACFacilitySwitchAction *m_Vr_WiperDeicerOFF;
    VR_HVACFacilitySwitchAction *m_Vr_RearSeatAirConditionerON;
    VR_HVACFacilitySwitchAction *m_Vr_RearSeatAirConditionerOFF;
    VR_HVACFacilitySwitchAction *m_Vr_RearDefoggerON;
    VR_HVACFacilitySwitchAction *m_Vr_RearDefoggerOFF;
    VR_HVACFacilitySwitchAction *m_Vr_ClimateCON;
    VR_HVACFacilitySwitchAction *m_Vr_InvalidOperator;
};

void
VR_HVACFacilitySwitchAction_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = AutoAirConditionerON;
     std::unique_ptr<VrAutoAirConditionerON> ProtoMsg(VrAutoAirConditionerON::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_FacilifySwitchAction = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *ProtoMsg, actionSender);

     nActionType = AutoAirConditionerOFF;
     std::unique_ptr<VrAutoAirConditionerOFF> AutoAirConditionOFF(VrAutoAirConditionerOFF::default_instance().New());
     m_Vr_AutoAirConditionOff = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *AutoAirConditionOFF, actionSender);

     nActionType = ACON;
     std::unique_ptr<VrACON> AcOn(VrACON::default_instance().New());
     m_Vr_ACON = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *AcOn, actionSender);

     nActionType = ACOFF;
     std::unique_ptr<VrACOFF>  AcOff(VrACOFF::default_instance().New());
     m_Vr_ACOFF = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *AcOff, actionSender);

     nActionType = DualModeON;
     std::unique_ptr<VrDualModeON> DualMode(VrDualModeON::default_instance().New());
     m_Vr_DualModeOn = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *DualMode, actionSender);

     nActionType = DualModeOFF;
     std::unique_ptr<VrDualModeOFF> DualModeOff(VrDualModeOFF::default_instance().New());
     m_Vr_DualModeOff = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *DualModeOff, actionSender);

     nActionType = WiperDeicerON;
     std::unique_ptr<VrWiperDeicerON>  WiperDeicerOn(VrWiperDeicerON::default_instance().New());
     m_Vr_WiperDeicerON = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *WiperDeicerOn, actionSender);

     nActionType = WiperDeicerOFF;
     std::unique_ptr<VrWiperDeicerOFF> WiperDeicerOff(VrWiperDeicerOFF::default_instance().New());
     m_Vr_WiperDeicerOFF = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *WiperDeicerOff, actionSender);

     nActionType = RearSeatAirConditionerON;
     std::unique_ptr<VrRearSeatAirConditionerON>  RearSeatACON(VrRearSeatAirConditionerON::default_instance().New());
     m_Vr_RearSeatAirConditionerON = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *RearSeatACON, actionSender);

     nActionType = RearSeatAirConditionerOFF;
     std::unique_ptr<VrRearSeatAirConditionerOFF> RearSeatACOFF(VrRearSeatAirConditionerOFF::default_instance().New());
     m_Vr_RearSeatAirConditionerOFF = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *RearSeatACOFF, actionSender);

     nActionType = RearDefoggerON;
     std::unique_ptr<VrRearDefoggerON> RearDefoggerOn(VrRearDefoggerON::default_instance().New());
     m_Vr_RearDefoggerON = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *RearDefoggerOn, actionSender);

     nActionType = RearDefoggerOFF;
     std::unique_ptr<VrRearDefoggerOFF> RearDefoggerOff(VrRearDefoggerOFF::default_instance().New());
     m_Vr_RearDefoggerOFF = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *RearDefoggerOff, actionSender);

     nActionType = ClimateConciergeModeON;
     std::unique_ptr<VrClimateConciergeModeON> ClimateCON(VrClimateConciergeModeON::default_instance().New());
     m_Vr_ClimateCON = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *ClimateCON, actionSender);

     nActionType = AudioON;
     std::unique_ptr<VrAudioON>  AudioOnMsg(VrAudioON::default_instance().New());
     m_Vr_InvalidOperator = VR_new VR_HVACFacilitySwitchAction(nEngineId, nActionType, *AudioOnMsg, actionSender);
}

void
VR_HVACFacilitySwitchAction_Test::TearDown()
{
    if (NULL != m_Vr_FacilifySwitchAction) {
        delete m_Vr_FacilifySwitchAction;
        m_Vr_FacilifySwitchAction = NULL;
    }
    if (NULL != m_Vr_AutoAirConditionOff) {
        delete m_Vr_AutoAirConditionOff;
        m_Vr_AutoAirConditionOff = NULL;
    }
    if (NULL != m_Vr_ACON) {
        delete m_Vr_ACON;
        m_Vr_ACON = NULL;
    }
    if (NULL != m_Vr_ACOFF) {
        delete m_Vr_ACOFF;
        m_Vr_ACOFF = NULL;
    }
    if (NULL != m_Vr_DualModeOn) {
        delete m_Vr_DualModeOn;
        m_Vr_DualModeOn = NULL;
    }
    if (NULL != m_Vr_DualModeOff) {
        delete m_Vr_DualModeOff;
        m_Vr_DualModeOff = NULL;
    }
    if (NULL != m_Vr_WiperDeicerON) {
        delete m_Vr_WiperDeicerON;
        m_Vr_WiperDeicerON = NULL;
    }
    if (NULL != m_Vr_WiperDeicerOFF) {
        delete m_Vr_WiperDeicerOFF;
        m_Vr_WiperDeicerOFF = NULL;
    }
    if (NULL != m_Vr_RearSeatAirConditionerON) {
        delete m_Vr_RearSeatAirConditionerON;
        m_Vr_RearSeatAirConditionerON = NULL;
    }
    if (NULL != m_Vr_RearSeatAirConditionerOFF) {
        delete m_Vr_RearSeatAirConditionerOFF;
        m_Vr_RearSeatAirConditionerOFF = NULL;
    }
    if (NULL != m_Vr_RearDefoggerON) {
        delete m_Vr_RearDefoggerON;
        m_Vr_RearDefoggerON = NULL;
    }
    if (NULL != m_Vr_RearDefoggerOFF) {
        delete m_Vr_RearDefoggerOFF;
        m_Vr_RearDefoggerOFF = NULL;
    }
    if (NULL != m_Vr_ClimateCON) {
        delete m_Vr_ClimateCON;
        m_Vr_ClimateCON = NULL;
    }
    if (NULL != m_Vr_InvalidOperator) {
        delete m_Vr_InvalidOperator;
        m_Vr_InvalidOperator = NULL;
    }
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_Case)
{
    m_Vr_FacilifySwitchAction->Run();
    m_Vr_InvalidOperator->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_AutoAirConditionerOFF_Case)
{
    m_Vr_AutoAirConditionOff->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_ACON_Case)
{
    m_Vr_ACON->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_ACOFF_Case)
{
    m_Vr_ACOFF->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_DualModeON_Case)
{
    m_Vr_DualModeOn->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_DualModeOFF_Case)
{
    m_Vr_DualModeOff->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_WiperDeicerON_Case)
{
    m_Vr_WiperDeicerON->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_WiperDeicerOFF_Case)
{
    m_Vr_WiperDeicerOFF->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_RearSeatAirConditionerON_Case)
{
    m_Vr_RearSeatAirConditionerON->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_RearSeatAirConditionerOFF_Case)
{
    m_Vr_RearSeatAirConditionerOFF->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_RearDefoggerON_Case)
{
    m_Vr_RearDefoggerON->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_RearDefoggerOFF_Case)
{
    m_Vr_RearDefoggerOFF->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, Run_ClimateConciergeModeON_Case)
{
    m_Vr_ClimateCON->Run();
    SUCCEED();
}

TEST_F(VR_HVACFacilitySwitchAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(AutoAirConditionerON);
    actionComfirm->set_processing_result(true);

    m_Vr_FacilifySwitchAction->OnReceive(*actionComfirm);
    SUCCEED();
}
/* EOF */
