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
#include "VR_ReceiverCb.h"
#include "VR_ActionEventSender.h"
#include "VR_HVACXMLMessageBuilder.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_ClimateCheckFanSpeed_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateCheckFanSpeed_VrUnit_Test
*
* The class is just for VR_ClimateCheckFanSpeed_VrUnit test.
*/
class VR_ClimateCheckFanSpeed_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateCheckFanSpeed_VrUnit *m_Vr_CheckFanSpeedAction;
    VR_ClimateCheckFanSpeed_VrUnit *m_Vr_InvalidFanSpeed;
};

void
VR_ClimateCheckFanSpeed_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = CheckFanSpeed;
     std::unique_ptr<VrCheckFanSpeed> ProtoMsg(VrCheckFanSpeed::default_instance().New());
     ProtoMsg->set_speed(3);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
     m_Vr_CheckFanSpeedAction = VR_new VR_ClimateCheckFanSpeed_VrUnit(nEngineId, nActionType, *ProtoMsg, actionSender, spAvcSender);

     std::unique_ptr<VrCheckFanSpeed>  InvalidSpeed(VrCheckFanSpeed::default_instance().New());
     InvalidSpeed->set_speed(100);
     m_Vr_InvalidFanSpeed = VR_new VR_ClimateCheckFanSpeed_VrUnit(nEngineId, nActionType, *InvalidSpeed, actionSender, spAvcSender);
}

void
VR_ClimateCheckFanSpeed_VrUnit_Test::TearDown()
{
    if (NULL != m_Vr_CheckFanSpeedAction) {
        delete m_Vr_CheckFanSpeedAction;
        m_Vr_CheckFanSpeedAction = NULL;
    }
    if (NULL != m_Vr_InvalidFanSpeed) {
        delete m_Vr_InvalidFanSpeed;
        m_Vr_InvalidFanSpeed = NULL;
    }
}

TEST_F(VR_ClimateCheckFanSpeed_VrUnit_Test, Run_Case)
{
    m_Vr_CheckFanSpeedAction->Run();
    SUCCEED();
}

TEST_F(VR_ClimateCheckFanSpeed_VrUnit_Test, GetReplyMsg_Case)
{
    m_Vr_CheckFanSpeedAction->GetReplyMsg();
    SUCCEED();
}

TEST_F(VR_ClimateCheckFanSpeed_VrUnit_Test, GetCheckFanSpeed_Case)
{
   m_Vr_CheckFanSpeedAction->GetCheckFanSpeed();
   SUCCEED();
}

TEST_F(VR_ClimateCheckFanSpeed_VrUnit_Test, SetCurrFanSpeed_Case)
{
    std::unique_ptr<VrFanSpeedStateResp> fanResp(VrFanSpeedStateResp::default_instance().New());
    fanResp->set_fan_speed_state(3);
    m_Vr_CheckFanSpeedAction->SetCurrFanSpeed(*fanResp);
    SUCCEED();
}

TEST_F(VR_ClimateCheckFanSpeed_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(FanSpeed);
    VrFanSpeedStateResp *pFanSpeedRsp = additionInfoResp->mutable_fan_speed_state_resp();
    pFanSpeedRsp->set_fan_speed_state(2);
    m_Vr_CheckFanSpeedAction->OnReceive(*additionInfoResp);
    /////////////// equal fan speed ///////////////////////////////
    VrAdditionalInfoRsp  EqualAdditional;
    EqualAdditional.set_response_id(FanSpeed);
    VrFanSpeedStateResp *pEqualFanSpeed  = EqualAdditional.mutable_fan_speed_state_resp();
    pEqualFanSpeed->set_fan_speed_state(3);  // m_CurrSpeed == nCheckSpeed
    m_Vr_CheckFanSpeedAction->OnReceive(EqualAdditional);
    /////////////  invalid fan speed /////////////////////////////
    VrAdditionalInfoRsp   InvalidAdditional;
    InvalidAdditional.set_response_id(FanSpeed);
    VrFanSpeedStateResp *pInvalidSpeed = InvalidAdditional.mutable_fan_speed_state_resp();
    pInvalidSpeed->set_fan_speed_state(3);
    m_Vr_InvalidFanSpeed->OnReceive(InvalidAdditional);   //  InvalidFanSpeed

    /////////////////////////// test other ////////////////////////////////////////////////
    VrAdditionalInfoRsp  AdditionOther;
    AdditionOther.set_response_id(Temperature);
    m_Vr_CheckFanSpeedAction->OnReceive(AdditionOther);

    VR_HVACXMLMessageBuilder::GetMessageBuilder().SetBlowStep(true);
    m_Vr_CheckFanSpeedAction->OnReceive(*additionInfoResp);
    ////////////////////////////////////////////////////////////////////////////////////////
    SUCCEED();
}
/* EOF */


