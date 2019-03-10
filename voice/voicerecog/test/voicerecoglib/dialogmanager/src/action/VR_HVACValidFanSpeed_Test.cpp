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
#include "VR_HVACValidFanSpeed.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACValidFanSpeed_Test
*
* The class is just for VR_HVACValidFanSpeed test.
*/
class VR_HVACValidFanSpeed_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACValidFanSpeed *m_Vr_ValidFanSpeedAction;
    VR_HVACValidFanSpeed *m_Vr_ValidDecreaseAction;
    VR_HVACValidFanSpeed *m_Vr_InvalidSpeedAction;
};

void
VR_HVACValidFanSpeed_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = ValidFanSpeed;
     std::unique_ptr<VrValidFanSpeed> ProtoMsg(VrValidFanSpeed::default_instance().New());
     ProtoMsg->set_type(ValidFanSpeedType_Increase);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_ValidFanSpeedAction = VR_new VR_HVACValidFanSpeed(nEngineId, nActionType, *ProtoMsg, actionSender);

     std::unique_ptr<VrValidFanSpeed> DecreaseSpeed(VrValidFanSpeed::default_instance().New());
     DecreaseSpeed->set_type(ValidFanSpeedType_Decrease);
     m_Vr_ValidDecreaseAction = VR_new VR_HVACValidFanSpeed(nEngineId, nActionType, *DecreaseSpeed, actionSender);

     std::unique_ptr<VrValidFanSpeed>  InvalidSpeed(VrValidFanSpeed::default_instance().New());
     InvalidSpeed->set_type((VrValidFanSpeedType)100);
     m_Vr_InvalidSpeedAction = VR_new VR_HVACValidFanSpeed(nEngineId, nActionType, *InvalidSpeed, actionSender);
}

void
VR_HVACValidFanSpeed_Test::TearDown()
{
    if (NULL != m_Vr_ValidFanSpeedAction) {
        delete m_Vr_ValidFanSpeedAction;
        m_Vr_ValidFanSpeedAction = NULL;
    }
    if (NULL != m_Vr_ValidDecreaseAction) {
        delete m_Vr_ValidDecreaseAction;
        m_Vr_ValidDecreaseAction = NULL;
    }
    if (NULL != m_Vr_InvalidSpeedAction) {
        delete m_Vr_InvalidSpeedAction;
        m_Vr_InvalidSpeedAction = NULL;
    }
}

TEST_F(VR_HVACValidFanSpeed_Test, Run_Case)
{
    m_Vr_ValidFanSpeedAction->Run();
    SUCCEED();
}

TEST_F(VR_HVACValidFanSpeed_Test, GetReplyMsg_Case)
{
    std::unique_ptr<VrFanSpeedStateResp> fanSpeed(VrFanSpeedStateResp::default_instance().New());
    fanSpeed->set_fan_speed_state(2);

    m_Vr_ValidFanSpeedAction->GetReplyMsg(*fanSpeed);
    SUCCEED();
}

TEST_F(VR_HVACValidFanSpeed_Test, GetReplyMsg_Decrease_Case)
{
    std::unique_ptr<VrFanSpeedStateResp> fanSpeed(VrFanSpeedStateResp::default_instance().New());
    fanSpeed->set_fan_speed_state(1);

    m_Vr_ValidDecreaseAction->GetReplyMsg(*fanSpeed);
    SUCCEED();
}

TEST_F(VR_HVACValidFanSpeed_Test, GetReplyMsg_Decrease_001_Case)
{
    std::unique_ptr<VrFanSpeedStateResp> fanSpeed(VrFanSpeedStateResp::default_instance().New());
    fanSpeed->set_fan_speed_state(4);

    m_Vr_ValidDecreaseAction->GetReplyMsg(*fanSpeed);
    SUCCEED();
}

TEST_F(VR_HVACValidFanSpeed_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(FanSpeed);
    VrFanSpeedStateResp *pFanSpeedRsp = additionInfoResp->mutable_fan_speed_state_resp();
    pFanSpeedRsp->set_fan_speed_state(4);
    m_Vr_ValidFanSpeedAction->OnReceive(*additionInfoResp);
    ///////////////////////////  invalid type ///////////////////////////////////////////
    m_Vr_InvalidSpeedAction->OnReceive(*additionInfoResp);
    VrAdditionalInfoRsp   InvalidInfo;
    InvalidInfo.set_response_id(Temperature);
    VrTemperatureResp *pTemp = InvalidInfo.mutable_temperature_resp();
    pTemp->set_temperature_state(0x10);
    m_Vr_ValidFanSpeedAction->OnReceive(InvalidInfo);
    SUCCEED();
}

TEST_F(VR_HVACValidFanSpeed_Test, OnReceive_ATMAX_Case)
{
    VrFanSpeedStateResp  FanSpeedResp;
    FanSpeedResp.set_fan_speed_state(5);

    m_Vr_ValidFanSpeedAction->GetReplyMsg(FanSpeedResp);
    SUCCEED();
}
/* EOF */

