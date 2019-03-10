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
#include "VR_HVACShowScreenAction.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACShowScreenAction_Test
*
* The class is just for VR_HVACShowScreenAction test.
*/
class VR_HVACShowScreenAction_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACShowScreenAction *m_Vr_ShowScreenAction;
    VR_HVACShowScreenAction *m_Vr_ShowRearScreen;
    VR_HVACShowScreenAction *m_Vr_ShowSteeringScreen;
    VR_HVACShowScreenAction *m_Vr_ShowFrontSeatScreen;
    VR_HVACShowScreenAction *m_Vr_ShowRearSeatScreen;
    VR_HVACShowScreenAction *m_Vr_HVACLexusConciergeScreen;
    VR_HVACShowScreenAction *m_Vr_HVACSeatOperationScreen;
    VR_HVACShowScreenAction *m_Vr_FrontSeatVentilationScreen;
    VR_HVACShowScreenAction *m_Vr_RearSeatVentilationScreen;
    VR_HVACShowScreenAction *m_Vr_InvalidShowScreen;
};

void
VR_HVACShowScreenAction_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = HVACFrontScreen;
     std::unique_ptr<VrHVACFrontScreen> ProtoMsg(VrHVACFrontScreen::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_ShowScreenAction = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *ProtoMsg);

     nActionType = HVACRearScreen;
     std::unique_ptr<VrHVACRearScreen> RearScreen(VrHVACRearScreen::default_instance().New());
     m_Vr_ShowRearScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *RearScreen);

     nActionType = HVACSteeringScreen;
     std::unique_ptr<VrHVACSteeringScreen>  SteeringScreen(VrHVACSteeringScreen::default_instance().New());
     m_Vr_ShowSteeringScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *SteeringScreen);

     nActionType = HVACFrontSeatScreen;
     std::unique_ptr<VrHVACFrontSeatScreen>  FrontSeatScreen(VrHVACFrontSeatScreen::default_instance().New());
     m_Vr_ShowFrontSeatScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *FrontSeatScreen);

     nActionType = HVACRearSeatScreen;
     std::unique_ptr<VrHVACRearSeatScreen>  RearSeatScreen(VrHVACRearSeatScreen::default_instance().New());
     m_Vr_ShowRearSeatScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *RearSeatScreen);

     nActionType = HVACLexusConciergeScreen;
     std::unique_ptr<VrHVACLexusConciergeScreen>  LexusConciergeScreen(VrHVACLexusConciergeScreen::default_instance().New());
     m_Vr_HVACLexusConciergeScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *LexusConciergeScreen);

     nActionType = HVACSeatOperationScreen;
     std::unique_ptr<VrHVACSeatOperationScreen>  SeatOperationScreen(VrHVACSeatOperationScreen::default_instance().New());
     m_Vr_HVACSeatOperationScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *SeatOperationScreen);

     nActionType = FrontSeatVentilationScreen;
     std::unique_ptr<VrFrontSeatVentilationScreen>  SeatVentilationScreen(VrFrontSeatVentilationScreen::default_instance().New());
     m_Vr_FrontSeatVentilationScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *SeatVentilationScreen);

     nActionType = RearSeatVentilationScreen;
     std::unique_ptr<VrRearSeatVentilationScreen>  seatVentilationScreen(VrRearSeatVentilationScreen::default_instance().New());
     m_Vr_RearSeatVentilationScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *seatVentilationScreen);

     nActionType = AudioON;
     std::unique_ptr<VrAudioON>  AudioOnMsg(VrAudioON::default_instance().New());
     m_Vr_InvalidShowScreen = VR_new VR_HVACShowScreenAction(actionSender, nEngineId, nActionType, *AudioOnMsg);
}

void
VR_HVACShowScreenAction_Test::TearDown()
{
    if (NULL != m_Vr_RearSeatVentilationScreen) {
        delete m_Vr_RearSeatVentilationScreen;
        m_Vr_RearSeatVentilationScreen = NULL;
    }
    if (NULL != m_Vr_FrontSeatVentilationScreen) {
        delete m_Vr_FrontSeatVentilationScreen;
        m_Vr_FrontSeatVentilationScreen = NULL;
    }
    if (NULL != m_Vr_HVACSeatOperationScreen) {
        delete m_Vr_HVACSeatOperationScreen;
        m_Vr_HVACSeatOperationScreen = NULL;
    }
    if (NULL != m_Vr_HVACLexusConciergeScreen) {
        delete m_Vr_HVACLexusConciergeScreen;
        m_Vr_HVACLexusConciergeScreen = NULL;
    }
    if (NULL != m_Vr_ShowFrontSeatScreen) {
        delete m_Vr_ShowFrontSeatScreen;
        m_Vr_ShowFrontSeatScreen = NULL;
    }
    if (NULL != m_Vr_ShowSteeringScreen) {
        delete m_Vr_ShowSteeringScreen;
        m_Vr_ShowSteeringScreen = NULL;
    }
    if (NULL != m_Vr_ShowScreenAction) {
        delete m_Vr_ShowScreenAction;
        m_Vr_ShowScreenAction = NULL;
    }
    if (NULL != m_Vr_ShowRearScreen) {
        delete m_Vr_ShowRearScreen;
        m_Vr_ShowRearScreen = NULL;
    }
    if (NULL != m_Vr_ShowRearSeatScreen) {
        delete m_Vr_ShowRearSeatScreen;
        m_Vr_ShowRearSeatScreen = NULL;
    }
    if (NULL != m_Vr_InvalidShowScreen) {
        delete m_Vr_InvalidShowScreen;
        m_Vr_InvalidShowScreen = NULL;
    }
}

TEST_F(VR_HVACShowScreenAction_Test, RearSeatVentilationScreen_Case)
{
    m_Vr_RearSeatVentilationScreen->Run();
    m_Vr_InvalidShowScreen->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, FrontSeatVentilationScreen_Case)
{
    m_Vr_FrontSeatVentilationScreen->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, HVACSeatOperationScreen_Case)
{
    m_Vr_HVACSeatOperationScreen->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, HVACLexusConciergeScreen_Case)
{
    m_Vr_HVACLexusConciergeScreen->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, HVACRearSeatScreen_Case)
{
    m_Vr_ShowRearSeatScreen->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, HVACFrontSeatScreen_Case)
{
    m_Vr_ShowFrontSeatScreen->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, Run_Case)
{
    m_Vr_ShowScreenAction->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, HVACRearScreen_Case)
{
    m_Vr_ShowRearScreen->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, HVACSteeringScreen_Case)
{
    m_Vr_ShowSteeringScreen->Run();
    SUCCEED();
}

TEST_F(VR_HVACShowScreenAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(HVACFrontScreen);
    actionComfirm->set_processing_result(true);

    m_Vr_ShowScreenAction->OnReceive(*actionComfirm);
    SUCCEED();
}
/* EOF */

