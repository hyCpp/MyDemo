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
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_ActionTtsStop_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ActionTtsStop_VrUnit_Test
*
* The class is just for VR_ActionTtsStop_VrUnit test.
*/
class VR_ActionTtsStop_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ActionTtsStop_VrUnit *m_action;
};

void
VR_ActionTtsStop_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = TTSProcessType_Stop;
     std::unique_ptr<VrTTSCommandResp> ProtoMsg(VrTTSCommandResp::default_instance().New());
     ProtoMsg->set_func_code(TTSProcessType_Stop);
     ProtoMsg->set_result(true);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
     m_action = VR_new VR_ActionTtsStop_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
}

void
VR_ActionTtsStop_VrUnit_Test::TearDown()
{
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
}

TEST_F(VR_ActionTtsStop_VrUnit_Test, Run_Case)
{
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_ActionTtsStop_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(FanSpeed);
    VrFanSpeedStateResp *pFanSpeedRsp = additionInfoResp->mutable_fan_speed_state_resp();
    pFanSpeedRsp->set_fan_speed_state(2);

    m_action->OnReceive(*additionInfoResp);
    SUCCEED();
}
/* EOF */


