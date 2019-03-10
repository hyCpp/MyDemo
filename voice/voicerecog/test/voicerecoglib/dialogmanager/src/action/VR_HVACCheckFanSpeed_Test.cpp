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
#include "VR_HVACCheckFanSpeed.h"
#include "VR_HVACXMLMessageBuilder.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACCheckFanSpeed_Test
*
* The class is just for VR_HVACCheckFanSpeed test.
*/
class VR_HVACCheckFanSpeed_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACCheckFanSpeed *m_Vr_CheckFanSpeedAction;
    VR_HVACCheckFanSpeed *m_Vr_InvalidFanSpeedAction;
};

void
VR_HVACCheckFanSpeed_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = CheckFanSpeed;
     std::unique_ptr<VrCheckFanSpeed> ProtoMsg(VrCheckFanSpeed::default_instance().New());
     ProtoMsg->set_speed(3);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_CheckFanSpeedAction = VR_new VR_HVACCheckFanSpeed(nEngineId, nActionType, *ProtoMsg, actionSender);

     std::unique_ptr<VrCheckFanSpeed> InvalidSpeed(VrCheckFanSpeed::default_instance().New());
     InvalidSpeed->set_speed(100);
     m_Vr_InvalidFanSpeedAction = VR_new VR_HVACCheckFanSpeed(nEngineId, nActionType, *InvalidSpeed, actionSender);
}

void
VR_HVACCheckFanSpeed_Test::TearDown()
{
    if (NULL != m_Vr_CheckFanSpeedAction) {
        delete m_Vr_CheckFanSpeedAction;
        m_Vr_CheckFanSpeedAction = NULL;
    }
    if (NULL != m_Vr_InvalidFanSpeedAction) {
        delete m_Vr_InvalidFanSpeedAction;
        m_Vr_InvalidFanSpeedAction = NULL;
    }
}

TEST_F(VR_HVACCheckFanSpeed_Test, Run_Case)
{
    m_Vr_CheckFanSpeedAction->Run();
    SUCCEED();
}

TEST_F(VR_HVACCheckFanSpeed_Test, GetReplyMsg_Case)
{
    m_Vr_CheckFanSpeedAction->GetReplyMsg();
    SUCCEED();
}

TEST_F(VR_HVACCheckFanSpeed_Test, GetCheckFanSpeed_Case)
{
   m_Vr_CheckFanSpeedAction->GetCheckFanSpeed();
   SUCCEED();
}

TEST_F(VR_HVACCheckFanSpeed_Test, SetCurrFanSpeed_Case)
{
    std::unique_ptr<VrFanSpeedStateResp> fanResp(VrFanSpeedStateResp::default_instance().New());
    fanResp->set_fan_speed_state(3);
    m_Vr_CheckFanSpeedAction->SetCurrFanSpeed(*fanResp);
    SUCCEED();
}

TEST_F(VR_HVACCheckFanSpeed_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(FanSpeed);
    VrFanSpeedStateResp *pFanSpeedRsp = additionInfoResp->mutable_fan_speed_state_resp();
    pFanSpeedRsp->set_fan_speed_state(2);

    m_Vr_CheckFanSpeedAction->OnReceive(*additionInfoResp);
    SUCCEED();
}

TEST_F(VR_HVACCheckFanSpeed_Test, OnReceive_Equal_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(FanSpeed);
    VrFanSpeedStateResp *pFanSpeedRsp = additionInfoResp->mutable_fan_speed_state_resp();
    pFanSpeedRsp->set_fan_speed_state(3);
    m_Vr_CheckFanSpeedAction->OnReceive(*additionInfoResp);
    ////////////////////// FAN_SPEED_7 ///////////////////////////////////////////
    VR_HVACXMLMessageBuilder::GetMessageBuilder().SetBlowStep(true);
    m_Vr_CheckFanSpeedAction->OnReceive(*additionInfoResp);
    /////////////////////// END  /////////////////////////////////////////////////
    SUCCEED();
}

TEST_F(VR_HVACCheckFanSpeed_Test, OnReceive_Invalid_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(FanSpeed);
    VrFanSpeedStateResp *pFanSpeedRsp = additionInfoResp->mutable_fan_speed_state_resp();
    pFanSpeedRsp->set_fan_speed_state(3);
    m_Vr_InvalidFanSpeedAction->OnReceive(*additionInfoResp);
    ////////////////////////////////// Invalid Response Id //////////////////////////////////////////
    VrAdditionalInfoRsp  InvalidResponse;
    InvalidResponse.set_response_id(ACState);
    VrACStateResp *pACStateResp = InvalidResponse.mutable_ac_state_resp();
    pACStateResp->set_ac_state(true);
    m_Vr_InvalidFanSpeedAction->OnReceive(InvalidResponse);
    ///////////////////// END ///////////////////////////////////////////////////////////////////////
    SUCCEED();
}
/* EOF */

