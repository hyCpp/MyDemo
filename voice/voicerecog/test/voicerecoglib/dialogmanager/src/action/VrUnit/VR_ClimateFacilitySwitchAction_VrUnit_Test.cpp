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
#include "VR_ClimateFacilitySwitchAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateFacilitySwitchAction_VrUnit_Test
*
* The class is just for VR_ClimateFacilitySwitchAction_VrUnit test.
*/
class VR_ClimateFacilitySwitchAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_FacilifySwitchAction;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_AutoAirConditionerOFF;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_ACON;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_ACOFF;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_DualModeON;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_DualModeOFF;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_WiperDeicerON;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_WiperDeicerOFF;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_RearSeatAirConditionerON;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_RearSeatAirConditionerOFF;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_RearDefoggerON;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_RearDefoggerOFF;
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_ClimateConciergeModeON;
    //////////////////////// test other //////////////////////////////////////////////
    VR_ClimateFacilitySwitchAction_VrUnit *m_Vr_ClimateTestOther;
};

void
VR_ClimateFacilitySwitchAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = AutoAirConditionerON;
     std::unique_ptr<VrAutoAirConditionerON> ProtoMsg(VrAutoAirConditionerON::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock());
     m_Vr_FacilifySwitchAction = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *ProtoMsg, actionSender, avclanSender);
     /////// AutoAirConditionerOFF ///////////////
     nActionType = AutoAirConditionerOFF;
     std::unique_ptr<VrAutoAirConditionerOFF> ConditionerOFF(VrAutoAirConditionerOFF::default_instance().New());
     m_Vr_AutoAirConditionerOFF = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *ConditionerOFF, actionSender, avclanSender);
     ///////// ACON /////////////////////////////
     nActionType = ACON;
     std::unique_ptr<VrACON> AcOn(VrACON::default_instance().New());
     m_Vr_ACON =  VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *AcOn, actionSender, avclanSender);
     //////// ACOFF ////////////////////////////
     nActionType = ACOFF;
     std::unique_ptr<VrACOFF> AcOff(VrACOFF::default_instance().New());
     m_Vr_ACOFF = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *AcOff, actionSender, avclanSender);
     //////// DualModeON //////////////////////
     nActionType = DualModeON;
     std::unique_ptr<VrDualModeON> ModeON(VrDualModeON::default_instance().New());
     m_Vr_DualModeON = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *ModeON, actionSender, avclanSender);
     /////////// DualModeOFF //////////////////
     nActionType = DualModeOFF;
     std::unique_ptr<VrDualModeOFF> ModeOFF(VrDualModeOFF::default_instance().New());
     m_Vr_DualModeOFF = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *ModeOFF, actionSender, avclanSender);
     /////////// WiperDeicerON //////////////////
     nActionType = WiperDeicerON;
     std::unique_ptr<VrWiperDeicerON> DeicerON(VrWiperDeicerON::default_instance().New());
     m_Vr_WiperDeicerON = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *DeicerON, actionSender, avclanSender);
     /////////// WiperDeicerOFF //////////////////
     nActionType = WiperDeicerOFF;
     std::unique_ptr<VrWiperDeicerOFF> DeicerOFF(VrWiperDeicerOFF::default_instance().New());
     m_Vr_WiperDeicerOFF = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *DeicerOFF, actionSender, avclanSender);
     /////////// RearSeatAirConditionerON //////////////////
     nActionType = RearSeatAirConditionerON;
     std::unique_ptr<VrRearSeatAirConditionerON> RearSeatON(VrRearSeatAirConditionerON::default_instance().New());
     m_Vr_RearSeatAirConditionerON = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *RearSeatON, actionSender, avclanSender);
     /////////// RearSeatAirConditionerOFF //////////////////
     nActionType = RearSeatAirConditionerOFF;
     std::unique_ptr<VrRearSeatAirConditionerOFF> RearSeatOFF(VrRearSeatAirConditionerOFF::default_instance().New());
     m_Vr_RearSeatAirConditionerOFF = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *RearSeatOFF, actionSender, avclanSender);
     /////////// RearDefoggerON //////////////////
     nActionType = RearDefoggerON;
     std::unique_ptr<VrRearDefoggerON> DefoggerON(VrRearDefoggerON::default_instance().New());
     m_Vr_RearDefoggerON = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *DefoggerON, actionSender, avclanSender);
     /////////// RearDefoggerOFF //////////////////
     nActionType = RearDefoggerOFF;
     std::unique_ptr<VrRearDefoggerOFF> DefoggerOFF(VrRearDefoggerOFF::default_instance().New());
     m_Vr_RearDefoggerOFF = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *DefoggerOFF, actionSender, avclanSender);
     /////////// ClimateConciergeModeON //////////////////
     nActionType = ClimateConciergeModeON;
     std::unique_ptr<VrClimateConciergeModeON> ConciergeModeON(VrClimateConciergeModeON::default_instance().New());
     m_Vr_ClimateConciergeModeON = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *ConciergeModeON, actionSender, avclanSender);

     nActionType = AudioON;
     std::unique_ptr<VrAudioON>  AudioOn(VrAudioON::default_instance().New());
     m_Vr_ClimateTestOther = VR_new VR_ClimateFacilitySwitchAction_VrUnit(nEngineId, nActionType, *AudioOn, actionSender, avclanSender);
}

void
VR_ClimateFacilitySwitchAction_VrUnit_Test::TearDown()
{
    if (NULL != m_Vr_FacilifySwitchAction) {
        delete m_Vr_FacilifySwitchAction;
        m_Vr_FacilifySwitchAction = NULL;
    }
    if (NULL != m_Vr_AutoAirConditionerOFF) {
        delete m_Vr_AutoAirConditionerOFF;
        m_Vr_AutoAirConditionerOFF = NULL;
    }
    if (NULL != m_Vr_ACON) {
        delete m_Vr_ACON;
        m_Vr_ACON = NULL;
    }
    if (NULL != m_Vr_ACOFF) {
        delete m_Vr_ACOFF;
        m_Vr_ACOFF = NULL;
    }
    if (NULL != m_Vr_DualModeON) {
        delete m_Vr_DualModeON;
        m_Vr_DualModeON = NULL;
    }
    if (NULL != m_Vr_DualModeOFF) {
        delete m_Vr_DualModeOFF;
        m_Vr_DualModeOFF = NULL;
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
    if (NULL != m_Vr_ClimateConciergeModeON) {
        delete m_Vr_ClimateConciergeModeON;
        m_Vr_ClimateConciergeModeON = NULL;
    }
    if (NULL != m_Vr_ClimateTestOther) {
        delete m_Vr_ClimateTestOther;
        m_Vr_ClimateTestOther = NULL;
    }
}

TEST_F(VR_ClimateFacilitySwitchAction_VrUnit_Test, Run_Case)
{
    m_Vr_FacilifySwitchAction->Run();
    m_Vr_AutoAirConditionerOFF->Run();
    m_Vr_ACON->Run();
    m_Vr_ACOFF->Run();
    m_Vr_DualModeON->Run();
    m_Vr_DualModeOFF->Run();
    m_Vr_WiperDeicerON->Run();
    m_Vr_WiperDeicerOFF->Run();
    m_Vr_RearSeatAirConditionerON->Run();
    m_Vr_RearSeatAirConditionerOFF->Run();
    m_Vr_RearDefoggerON->Run();
    m_Vr_RearDefoggerOFF->Run();
    m_Vr_ClimateConciergeModeON->Run();
    m_Vr_ClimateTestOther->Run();
    SUCCEED();
}

TEST_F(VR_ClimateFacilitySwitchAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(AutoAirConditionerON);
    actionComfirm->set_processing_result(true);

    m_Vr_FacilifySwitchAction->OnReceive(*actionComfirm);
    SUCCEED();
}
/* EOF */

