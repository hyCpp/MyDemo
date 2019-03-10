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
#include "VR_HVACTuneFanAction.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACTuneFanAction_Test
*
* The class is just for VR_HVACTuneFanAction test.
*/
class VR_HVACTuneFanAction_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACTuneFanAction *m_Vr_TuneFanAction;
    VR_HVACTuneFanAction *m_Vr_ChangeFanSpeed;
    VR_HVACTuneFanAction *m_Vr_FanSpeedMin;
    VR_HVACTuneFanAction *m_Vr_BlowerModeFace;
    VR_HVACTuneFanAction *m_Vr_BlowerModeFoot;
    VR_HVACTuneFanAction *m_Vr_BlowerModeFaceAndFoot;
    VR_HVACTuneFanAction *m_Vr_BlowerModeWindowAndFoot;
    ////////////// Invaild Fan Operator /////////////////////////////
    VR_HVACTuneFanAction *m_Vr_InvalidFanOperator;
};

void
VR_HVACTuneFanAction_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = FanSpeedMax;
     std::unique_ptr<VrFanSpeedMax> ProtoMsg(VrFanSpeedMax::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_TuneFanAction = VR_new VR_HVACTuneFanAction(nEngineId, nActionType, *ProtoMsg, actionSender);

     nActionType = ChangeFanSpeed;
     std::unique_ptr<VrChangeFanSpeed> changeFanSpeed(VrChangeFanSpeed::default_instance().New());
     changeFanSpeed->set_fan_speed(5);
     m_Vr_ChangeFanSpeed = VR_new VR_HVACTuneFanAction(nEngineId, nActionType, *changeFanSpeed, actionSender);

     nActionType = FanSpeedMin;
     std::unique_ptr<VrFanSpeedMin> SpeedMin(VrFanSpeedMin::default_instance().New());
     m_Vr_FanSpeedMin = VR_new VR_HVACTuneFanAction(nEngineId, nActionType, *SpeedMin, actionSender);

     nActionType = BlowerModeFace;
     std::unique_ptr<VrBlowerModeFace> ModeFace(VrBlowerModeFace::default_instance().New());
     m_Vr_BlowerModeFace = VR_new VR_HVACTuneFanAction(nEngineId, nActionType, *ModeFace, actionSender);

     nActionType = BlowerModeFoot;
     std::unique_ptr<VrBlowerModeFoot> ModeFoot(VrBlowerModeFoot::default_instance().New());
     m_Vr_BlowerModeFoot = VR_new VR_HVACTuneFanAction(nEngineId, nActionType, *ModeFoot, actionSender);

     nActionType = BlowerModeFaceAndFoot;
     std::unique_ptr<VrBlowerModeFaceAndFoot> ModeFaceAndFoot(VrBlowerModeFaceAndFoot::default_instance().New());
     m_Vr_BlowerModeFaceAndFoot = VR_new VR_HVACTuneFanAction(nEngineId, nActionType, *ModeFaceAndFoot, actionSender);

     nActionType = BlowerModeWindowAndFoot;
     std::unique_ptr<VrBlowerModeWindowAndFoot> WindowAndFoot(VrBlowerModeWindowAndFoot::default_instance().New());
     m_Vr_BlowerModeWindowAndFoot = VR_new VR_HVACTuneFanAction(nEngineId, nActionType, *WindowAndFoot, actionSender);

     ////////////// Invaild Fan Operator /////////////////////////////
     nActionType = AudioON;
     std::unique_ptr<VrAudioON>  AudioOn(VrAudioON::default_instance().New());
     m_Vr_InvalidFanOperator = VR_new VR_HVACTuneFanAction(nEngineId, nActionType, *AudioOn, actionSender);
}

void
VR_HVACTuneFanAction_Test::TearDown()
{
    if (NULL != m_Vr_TuneFanAction) {
        delete m_Vr_TuneFanAction;
        m_Vr_TuneFanAction = NULL;
    }
    if (NULL != m_Vr_ChangeFanSpeed) {
        delete m_Vr_ChangeFanSpeed;
        m_Vr_ChangeFanSpeed = NULL;
    }
    if (NULL != m_Vr_FanSpeedMin) {
        delete m_Vr_FanSpeedMin;
        m_Vr_FanSpeedMin = NULL;
    }
    if (NULL != m_Vr_BlowerModeFace) {
        delete m_Vr_BlowerModeFace;
        m_Vr_BlowerModeFace = NULL;
    }
    if (NULL != m_Vr_BlowerModeFoot) {
        delete m_Vr_BlowerModeFoot;
        m_Vr_BlowerModeFoot = NULL;
    }
    if (NULL != m_Vr_BlowerModeFaceAndFoot) {
        delete m_Vr_BlowerModeFaceAndFoot;
        m_Vr_BlowerModeFaceAndFoot = NULL;
    }
    if (NULL != m_Vr_BlowerModeWindowAndFoot) {
        delete m_Vr_BlowerModeWindowAndFoot;
        m_Vr_BlowerModeWindowAndFoot = NULL;
    }
    if (NULL != m_Vr_InvalidFanOperator) {
        delete m_Vr_InvalidFanOperator;
        m_Vr_InvalidFanOperator = NULL;
    }
}

TEST_F(VR_HVACTuneFanAction_Test, Run_Case)
{
    m_Vr_TuneFanAction->Run();
    m_Vr_InvalidFanOperator->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneFanAction_Test, ChangeFanSpeed_Case)
{
    m_Vr_ChangeFanSpeed->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneFanAction_Test, FanSpeedMin_Case)
{
    m_Vr_FanSpeedMin->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneFanAction_Test, BlowerModeFace_Case)
{
    m_Vr_BlowerModeFace->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneFanAction_Test, BlowerModeFoot_Case)
{
    m_Vr_BlowerModeFoot->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneFanAction_Test, BlowerModeFaceAndFoot_Case)
{
    m_Vr_BlowerModeFaceAndFoot->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneFanAction_Test, BlowerModeWindowAndFoot_Case)
{
    m_Vr_BlowerModeWindowAndFoot->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneFanAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(AutoAirConditionerON);
    actionComfirm->set_processing_result(true);

    m_Vr_TuneFanAction->OnReceive(*actionComfirm);
    SUCCEED();
}
/* EOF */

