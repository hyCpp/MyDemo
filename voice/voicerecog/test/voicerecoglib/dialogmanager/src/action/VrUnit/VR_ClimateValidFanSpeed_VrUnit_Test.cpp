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
#include "VR_ClimateValidFanSpeed_VrUnit.h"
#include "VR_AvcLanSender_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateValidFanSpeed_VrUnit_Test
*
* The class is just for VR_ClimateValidFanSpeed_VrUnit test.
*/
class VR_ClimateValidFanSpeed_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateValidFanSpeed_VrUnit *m_Vr_ValidFanSpeedAction;
    VR_ClimateValidFanSpeed_VrUnit *m_Vr_DereaseFanSpeed;
};

void
VR_ClimateValidFanSpeed_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = ValidFanSpeed;
     std::unique_ptr<VrValidFanSpeed> ProtoMsg(VrValidFanSpeed::default_instance().New());
     ProtoMsg->set_type(ValidFanSpeedType_Increase);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
     m_Vr_ValidFanSpeedAction = VR_new VR_ClimateValidFanSpeed_VrUnit(nEngineId, nActionType, *ProtoMsg, actionSender, avclanSender);
     //////////////////////////// decrease fan speed ///////////////////////////////////////////////////////////////////////////////
     nActionType = ValidFanSpeed;
     std::unique_ptr<VrValidFanSpeed> DecreaseFanSpeed(VrValidFanSpeed::default_instance().New());
     DecreaseFanSpeed->set_type(ValidFanSpeedType_Decrease);
     m_Vr_DereaseFanSpeed = VR_new VR_ClimateValidFanSpeed_VrUnit(nEngineId, nActionType, *DecreaseFanSpeed, actionSender, avclanSender);
}

void
VR_ClimateValidFanSpeed_VrUnit_Test::TearDown()
{
    if (NULL != m_Vr_ValidFanSpeedAction) {
        delete m_Vr_ValidFanSpeedAction;
        m_Vr_ValidFanSpeedAction = NULL;
    }
}

TEST_F(VR_ClimateValidFanSpeed_VrUnit_Test, Run_Case)
{
    m_Vr_ValidFanSpeedAction->Run();
    SUCCEED();
}

TEST_F(VR_ClimateValidFanSpeed_VrUnit_Test, GetReplyMsg_Case)
{
    std::unique_ptr<VrFanSpeedStateResp> fanSpeed(VrFanSpeedStateResp::default_instance().New());
    fanSpeed->set_fan_speed_state(2);
    m_Vr_ValidFanSpeedAction->GetReplyMsg(*fanSpeed);
    ////////////////////////////// already at max /////////////////////////////////////////////
    VrFanSpeedStateResp  MaxFanSpeed;
    MaxFanSpeed.set_fan_speed_state(5);
    m_Vr_ValidFanSpeedAction->GetReplyMsg(MaxFanSpeed);
    ///////////////////////////// decrease fan speed //////////////////////////////////////////
    VrFanSpeedStateResp MinFanSpeed;
    MinFanSpeed.set_fan_speed_state(1);
    m_Vr_DereaseFanSpeed->GetReplyMsg(MinFanSpeed);
    ////////////////////////////// normal fan speed ////////////////////////////////////////////
    VrFanSpeedStateResp  DecreaseFanSpeed;
    DecreaseFanSpeed.set_fan_speed_state(4);
    m_Vr_DereaseFanSpeed->GetReplyMsg(DecreaseFanSpeed);
    /////////////////////////////////////  invalid fanspeed //////////////////////////////////////////////
    VrAdditionalInfoRsp  AdditionInvalid;
    AdditionInvalid.set_response_id(Temperature);
    m_Vr_ValidFanSpeedAction->GetReplyMsg(AdditionInvalid);
    SUCCEED();
}

TEST_F(VR_ClimateValidFanSpeed_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(FanSpeed);
    VrFanSpeedStateResp *pFanSpeedRsp = additionInfoResp->mutable_fan_speed_state_resp();
    pFanSpeedRsp->set_fan_speed_state(4);

    m_Vr_ValidFanSpeedAction->OnReceive(*additionInfoResp);
    SUCCEED();
}
/* EOF */


