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
#include "VR_ClimateShowScreenAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateShowScreenAction_VrUnit_Test
*
* The class is just for VR_ClimateShowScreenAction_VrUnit test.
*/
class VR_ClimateShowScreenAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateShowScreenAction_VrUnit *m_Vr_ShowScreenAction;
    VR_ClimateShowScreenAction_VrUnit *m_Vr_HVACRearScreen;
    VR_ClimateShowScreenAction_VrUnit *m_Vr_HVACSteeringScreen;
    VR_ClimateShowScreenAction_VrUnit *m_Vr_HVACFrontSeatScreen;
    VR_ClimateShowScreenAction_VrUnit *m_Vr_HVACRearSeatScreen;
    VR_ClimateShowScreenAction_VrUnit *m_Vr_HVACLexusConciergeScreen;
    VR_ClimateShowScreenAction_VrUnit *m_Vr_HVACSeatOperationScreen;
    VR_ClimateShowScreenAction_VrUnit *m_Vr_FrontSeatVentilationScreen;
    VR_ClimateShowScreenAction_VrUnit *m_Vr_RearSeatVentilationScreen;
    /////////////////// test other ////////////////////////////////////////////////
    VR_ClimateShowScreenAction_VrUnit *m_Vr_HavcTestOther;
};

void
VR_ClimateShowScreenAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = HVACFrontScreen;
     std::unique_ptr<VrHVACFrontScreen> ProtoMsg(VrHVACFrontScreen::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock());
     m_Vr_ShowScreenAction = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *ProtoMsg, actionSender, avclanSender);
     ////////////// HVACRearScreen //////////////////////////////////
     nActionType = HVACRearScreen;
     std::unique_ptr<VrHVACRearScreen>  RearScreen(VrHVACRearScreen::default_instance().New());
     m_Vr_HVACRearScreen = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *RearScreen, actionSender, avclanSender);
     ////////////// HVACSteeringScreen //////////////////////////////////
     nActionType = HVACSteeringScreen;
     std::unique_ptr<VrHVACSteeringScreen>  SteeringScreen(VrHVACSteeringScreen::default_instance().New());
     m_Vr_HVACSteeringScreen = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *SteeringScreen, actionSender, avclanSender);
     ////////////// HVACFrontSeatScreen //////////////////////////////////
     nActionType = HVACFrontSeatScreen;
     std::unique_ptr<VrHVACFrontSeatScreen>  FrontSeatScreen(VrHVACFrontSeatScreen::default_instance().New());
     m_Vr_HVACFrontSeatScreen = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *FrontSeatScreen, actionSender, avclanSender);
     ////////////// HVACRearSeatScreen //////////////////////////////////
     nActionType = HVACRearSeatScreen;
     std::unique_ptr<VrHVACRearSeatScreen>  RearSeatScreen(VrHVACRearSeatScreen::default_instance().New());
     m_Vr_HVACRearSeatScreen = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *RearSeatScreen, actionSender, avclanSender);
     ////////////// HVACLexusConciergeScreen //////////////////////////////////
     nActionType = HVACLexusConciergeScreen;
     std::unique_ptr<VrHVACLexusConciergeScreen>  ConciergeScreen(VrHVACLexusConciergeScreen::default_instance().New());
     m_Vr_HVACLexusConciergeScreen = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *ConciergeScreen, actionSender, avclanSender);
     ////////////// HVACSeatOperationScreen //////////////////////////////////
     nActionType = HVACSeatOperationScreen;
     std::unique_ptr<VrHVACSeatOperationScreen>  SeatOperationScreen(VrHVACSeatOperationScreen::default_instance().New());
     m_Vr_HVACSeatOperationScreen = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *SeatOperationScreen, actionSender, avclanSender);
     ////////////// FrontSeatVentilationScreen //////////////////////////////////
     nActionType = FrontSeatVentilationScreen;
     std::unique_ptr<VrFrontSeatVentilationScreen>  FrontSeatVentilation(VrFrontSeatVentilationScreen::default_instance().New());
     m_Vr_FrontSeatVentilationScreen = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *FrontSeatVentilation, actionSender, avclanSender);
     ////////////// RearSeatVentilationScreen //////////////////////////////////
     nActionType = RearSeatVentilationScreen;
     std::unique_ptr<VrRearSeatVentilationScreen>  RearSeatVentilation(VrRearSeatVentilationScreen::default_instance().New());
     m_Vr_RearSeatVentilationScreen = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *RearSeatVentilation, actionSender, avclanSender);

     nActionType = AudioON;
     std::unique_ptr<VrAudioON>  AudioOn(VrAudioON::default_instance().New());
     m_Vr_HavcTestOther = VR_new VR_ClimateShowScreenAction_VrUnit(nEngineId, nActionType, *AudioOn, actionSender, avclanSender);
}

void
VR_ClimateShowScreenAction_VrUnit_Test::TearDown()
{
    if (NULL != m_Vr_ShowScreenAction) {
        delete m_Vr_ShowScreenAction;
        m_Vr_ShowScreenAction = NULL;
    }
    if (NULL != m_Vr_HVACRearScreen) {
        delete m_Vr_HVACRearScreen;
        m_Vr_HVACRearScreen = NULL;
    }
    if (NULL != m_Vr_HVACSteeringScreen) {
        delete m_Vr_HVACSteeringScreen;
        m_Vr_HVACSteeringScreen = NULL;
    }
    if (NULL != m_Vr_HVACFrontSeatScreen) {
        delete m_Vr_HVACFrontSeatScreen;
        m_Vr_HVACFrontSeatScreen = NULL;
    }
    if (NULL != m_Vr_HVACRearSeatScreen) {
        delete m_Vr_HVACRearSeatScreen;
        m_Vr_HVACRearSeatScreen = NULL;
    }
    if (NULL != m_Vr_HVACLexusConciergeScreen) {
        delete m_Vr_HVACLexusConciergeScreen;
        m_Vr_HVACLexusConciergeScreen = NULL;
    }
    if (NULL != m_Vr_HVACSeatOperationScreen) {
        delete m_Vr_HVACSeatOperationScreen;
        m_Vr_HVACSeatOperationScreen = NULL;
    }
    if (NULL != m_Vr_FrontSeatVentilationScreen) {
        delete m_Vr_FrontSeatVentilationScreen;
        m_Vr_FrontSeatVentilationScreen = NULL;
    }
    if (NULL != m_Vr_RearSeatVentilationScreen) {
        delete m_Vr_RearSeatVentilationScreen;
        m_Vr_RearSeatVentilationScreen = NULL;
    }
    if (NULL != m_Vr_HavcTestOther) {
        delete m_Vr_HavcTestOther;
        m_Vr_HavcTestOther = NULL;
    }
}

TEST_F(VR_ClimateShowScreenAction_VrUnit_Test, Run_Case)
{
    m_Vr_ShowScreenAction->Run();
    m_Vr_HVACRearScreen->Run();
    m_Vr_HVACSteeringScreen->Run();
    m_Vr_HVACFrontSeatScreen->Run();
    m_Vr_HVACRearSeatScreen->Run();
    m_Vr_HVACLexusConciergeScreen->Run();
    m_Vr_HVACSeatOperationScreen->Run();
    m_Vr_FrontSeatVentilationScreen->Run();
    m_Vr_RearSeatVentilationScreen->Run();
    m_Vr_HavcTestOther->Run();
    SUCCEED();
}

TEST_F(VR_ClimateShowScreenAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(HVACFrontScreen);
    actionComfirm->set_processing_result(true);

    m_Vr_ShowScreenAction->OnReceive(*actionComfirm);
    SUCCEED();
}
/* EOF */


