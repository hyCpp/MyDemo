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
#include "VR_ClimateTuneFanAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateTuneFanAction_VrUnit_Test
*
* The class is just for VR_ClimateTuneFanAction_VrUnit test.
*/
class VR_ClimateTuneFanAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateTuneFanAction_VrUnit *m_Vr_TuneFanAction;
    VR_ClimateTuneFanAction_VrUnit *m_Vr_ChangeFanSpeed;
    VR_ClimateTuneFanAction_VrUnit *m_Vr_FanSpeedMin;
    VR_ClimateTuneFanAction_VrUnit *m_Vr_BlowerModeFace;
    VR_ClimateTuneFanAction_VrUnit *m_Vr_BlowerModeFoot;
    VR_ClimateTuneFanAction_VrUnit *m_Vr_BlowerModeFaceAndFoot;
    VR_ClimateTuneFanAction_VrUnit *m_Vr_BlowerModeWindowAndFoot;

    VR_ClimateTuneFanAction_VrUnit *m_Vr_ClimateTestOther;
};

void
VR_ClimateTuneFanAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = FanSpeedMax;
     std::unique_ptr<VrFanSpeedMax> ProtoMsg(VrFanSpeedMax::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock());
     m_Vr_TuneFanAction = VR_new VR_ClimateTuneFanAction_VrUnit(nEngineId, nActionType, *ProtoMsg, actionSender, avclanSender);
     ////////////////////// ChangeFanSpeed ///////////////////////////////////////////////////////////////////////
     nActionType = ChangeFanSpeed;
     std::unique_ptr<VrChangeFanSpeed> ChangeFanSpeedReq(VrChangeFanSpeed::default_instance().New());
     m_Vr_ChangeFanSpeed = VR_new VR_ClimateTuneFanAction_VrUnit(nEngineId, nActionType, *ChangeFanSpeedReq, actionSender, avclanSender);
     ////////////////////// FanSpeedMin ///////////////////////////////////////////////////////////////////////
     nActionType = FanSpeedMin;
     std::unique_ptr<VrFanSpeedMin> FanSpeedMinReq(VrFanSpeedMin::default_instance().New());
     m_Vr_FanSpeedMin = VR_new VR_ClimateTuneFanAction_VrUnit(nEngineId, nActionType, *FanSpeedMinReq, actionSender, avclanSender);
     ////////////////////// BlowerModeFace ///////////////////////////////////////////////////////////////////////
     nActionType = BlowerModeFace;
     std::unique_ptr<VrBlowerModeFace> BlowerModeFaceReq(VrBlowerModeFace::default_instance().New());
     m_Vr_BlowerModeFace = VR_new VR_ClimateTuneFanAction_VrUnit(nEngineId, nActionType, *BlowerModeFaceReq, actionSender, avclanSender);
     ////////////////////// BlowerModeFoot ///////////////////////////////////////////////////////////////////////
     nActionType = BlowerModeFoot;
     std::unique_ptr<VrBlowerModeFoot> BlowerModeFootReq(VrBlowerModeFoot::default_instance().New());
     m_Vr_BlowerModeFoot = VR_new VR_ClimateTuneFanAction_VrUnit(nEngineId, nActionType, *BlowerModeFootReq, actionSender, avclanSender);
     ////////////////////// BlowerModeWindowAndFoot ///////////////////////////////////////////////////////////////////////
     nActionType = BlowerModeWindowAndFoot;
     std::unique_ptr<VrBlowerModeWindowAndFoot> BlowerModeWindowAndFootReq(VrBlowerModeWindowAndFoot::default_instance().New());
     m_Vr_BlowerModeWindowAndFoot = VR_new VR_ClimateTuneFanAction_VrUnit(nEngineId, nActionType, *BlowerModeWindowAndFootReq, actionSender, avclanSender);
     ////////////////////// BlowerModeFaceAndFoot ///////////////////////////////////////////////////////////////////////
     nActionType = BlowerModeFaceAndFoot;
     std::unique_ptr<VrBlowerModeFaceAndFoot> BlowerModeFaceAndFootReq(VrBlowerModeFaceAndFoot::default_instance().New());
     m_Vr_BlowerModeFaceAndFoot = VR_new VR_ClimateTuneFanAction_VrUnit(nEngineId, nActionType, *BlowerModeFaceAndFootReq, actionSender, avclanSender);

     nActionType = AudioON;
     std::unique_ptr<VrAudioON> AudioONReq(VrAudioON::default_instance().New());
     m_Vr_ClimateTestOther = VR_new VR_ClimateTuneFanAction_VrUnit(nEngineId, nActionType, *AudioONReq, actionSender, avclanSender);
}

void
VR_ClimateTuneFanAction_VrUnit_Test::TearDown()
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
    if (NULL != m_Vr_ClimateTestOther) {
        delete m_Vr_ClimateTestOther;
        m_Vr_ClimateTestOther = NULL;
    }
}

TEST_F(VR_ClimateTuneFanAction_VrUnit_Test, Run_Case)
{
    m_Vr_TuneFanAction->Run();
    m_Vr_ChangeFanSpeed->Run();
    m_Vr_FanSpeedMin->Run();
    m_Vr_BlowerModeFace->Run();
    m_Vr_BlowerModeFoot->Run();
    m_Vr_BlowerModeFaceAndFoot->Run();
    m_Vr_BlowerModeWindowAndFoot->Run();
    m_Vr_ClimateTestOther->Run();
    SUCCEED();
}

TEST_F(VR_ClimateTuneFanAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(AutoAirConditionerON);
    actionComfirm->set_processing_result(true);

    m_Vr_TuneFanAction->OnReceive(*actionComfirm);
    SUCCEED();
}
/* EOF */


