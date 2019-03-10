/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
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

#include "VR_AvcLanSenderIF.h"
#include "VR_ActionBeepPlay.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include   "VR_Indentifier.hpp"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ActionBeepPlay_Test
*
* The class is just for VR_ActionBeepPlay test.
*/
class VR_ActionBeepPlay_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_ActionBeepPlay *m_beepPlayAction;
};

void VR_ActionBeepPlay_Test::SetUp()
{
    VR_Indentifier<VR_ActionBase>::m_sRequestID = -88;
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrBeepCommandReq> protoMsg(VrBeepCommandReq::default_instance().New());
    protoMsg->set_beep_id(1);
    protoMsg->set_beep_file_path("/");
    protoMsg->set_isbargein(true);
    m_beepPlayAction = VR_new VR_ActionBeepPlay(m_sp_EventSender, 1, 1, *protoMsg);
}

void VR_ActionBeepPlay_Test::TearDown()
{
    if (NULL != m_beepPlayAction) {
        delete m_beepPlayAction;
        m_beepPlayAction = NULL;
    }
}

TEST_F(VR_ActionBeepPlay_Test, getLastActionId_Case)
{
    m_beepPlayAction->getLastActionId();
    SUCCEED();
}

TEST_F(VR_ActionBeepPlay_Test, Run_Case)
{
    m_beepPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_ActionBeepPlay_Test, OnReceive_Stop)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Stop);
    protoMsg->set_result(true);

    m_beepPlayAction->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_ActionBeepPlay_Test, OnReceive_Pause)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Pause);
    protoMsg->set_result(true);

    m_beepPlayAction->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_ActionBeepPlay_Test, OnReceive_Play)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Play);
    protoMsg->set_result(true);

    m_beepPlayAction->OnReceive(*protoMsg);
    SUCCEED();
}

/* EOF */
