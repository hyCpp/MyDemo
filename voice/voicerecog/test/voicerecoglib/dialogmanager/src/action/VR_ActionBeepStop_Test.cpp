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
#include "VR_ActionBeepStop.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "navi/Voice/TTS/TtsStop.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ActionBeepStop_Test
*
* The class is just for VR_ActionBeepStop test.
*/
class VR_ActionBeepStop_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_ActionBeepStop *m_beepStopAction;
};

void VR_ActionBeepStop_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrBeepCommandReq> protoMsg(VrBeepCommandReq::default_instance().New());
    protoMsg->set_func_code(BeepProcessType_Stop);
    m_beepStopAction = VR_new VR_ActionBeepStop(m_sp_EventSender, 1, 1, *protoMsg);
}

void VR_ActionBeepStop_Test::TearDown()
{
    if (NULL != m_beepStopAction) {
        delete m_beepStopAction;
        m_beepStopAction = NULL;
    }
}

TEST_F(VR_ActionBeepStop_Test, Run_Case)
{
    m_beepStopAction->Run();
    SUCCEED();
}

TEST_F(VR_ActionBeepStop_Test, OnReceive_Stop)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Stop);
    protoMsg->set_result(true);

    m_beepStopAction->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_ActionBeepStop_Test, OnReceive_Pause)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Pause);
    protoMsg->set_result(true);

    m_beepStopAction->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_ActionBeepStop_Test, OnReceive_Play)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Play);
    protoMsg->set_result(true);

    m_beepStopAction->OnReceive(*protoMsg);
    SUCCEED();
}

/* EOF */
