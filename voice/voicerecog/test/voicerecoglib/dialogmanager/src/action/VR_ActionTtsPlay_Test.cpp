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
#include "VR_ActionTtsPlay.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ActionTtsPlay_Test
*
* The class is just for VR_ActionTtsPlay test.
*/
class VR_ActionTtsPlay_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_ActionTtsPlay *m_ttsPlayAction;
};

void VR_ActionTtsPlay_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTTSCommandReq> protoMsg(VrTTSCommandReq::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Play);
    protoMsg->set_tts_text("tts");
    protoMsg->set_tts_text_len(4);
    protoMsg->set_tts_language("cs");
    protoMsg->set_isbargein(true);
    protoMsg->set_data_type(TSDataType_Normal);
    m_ttsPlayAction = VR_new VR_ActionTtsPlay(m_sp_EventSender, 1, 1, *protoMsg);
}

void VR_ActionTtsPlay_Test::TearDown()
{
    if (NULL != m_ttsPlayAction) {
        delete m_ttsPlayAction;
        m_ttsPlayAction = NULL;
    }
}

TEST_F(VR_ActionTtsPlay_Test, getLastActionId_Case)
{
    m_ttsPlayAction->getLastActionId();
    SUCCEED();
}

TEST_F(VR_ActionTtsPlay_Test, Run_cs)
{
    m_ttsPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_ActionTtsPlay_Test, Run_NoLanguage)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTTSCommandReq> protoMsg(VrTTSCommandReq::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Play);
    protoMsg->set_data_type(TSDataType_Normal);
    protoMsg->set_tts_text("tts");
    protoMsg->set_tts_text_len(4);
    protoMsg->set_tts_language("");
    protoMsg->set_isbargein(true);
    m_ttsPlayAction = VR_new VR_ActionTtsPlay(m_sp_EventSender, 1, 1, *protoMsg);
    m_ttsPlayAction->Run();
    SUCCEED();
}

TEST_F(VR_ActionTtsPlay_Test, OnReceive_Stop)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Stop);
    protoMsg->set_result(true);

    m_ttsPlayAction->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_ActionTtsPlay_Test, OnReceive_Pause)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Pause);
    protoMsg->set_result(true);

    m_ttsPlayAction->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_ActionTtsPlay_Test, OnReceive_Play)
{
    std::unique_ptr<VrTTSCommandResult> protoMsg(VrTTSCommandResult::default_instance().New());
    protoMsg->set_func_code(TTSProcessType_Play);
    protoMsg->set_result(true);

    m_ttsPlayAction->OnReceive(*protoMsg);
    SUCCEED();
}

/* EOF */
