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
#include "VR_AudioSourceAction.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_AudioSourceAction_Test
*
* The class is just for VR_AudioSourceAction test.
*/
class VR_AudioSourceAction_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_AudioSourceAction *m_audioSourceAction;
};

void VR_AudioSourceAction_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSwitchSource> protoMsg(VrSwitchSource::default_instance().New());
    protoMsg->set_source_name("AM");
    protoMsg->set_audio_mode(1);
    m_audioSourceAction = VR_new VR_AudioSourceAction(m_sp_EventSender, 1, SwitchSource, *protoMsg);
}

void VR_AudioSourceAction_Test::TearDown()
{
    if (NULL != m_audioSourceAction) {
        delete m_audioSourceAction;
        m_audioSourceAction = NULL;
    }
}

TEST_F(VR_AudioSourceAction_Test, Run_VrSwitchSource)
{
    m_audioSourceAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioSourceAction_Test, Run_AudioOFF)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrAudioOFF> protoMsg(VrAudioOFF::default_instance().New());
    m_audioSourceAction = VR_new VR_AudioSourceAction(m_sp_EventSender, 1, AudioOFF, *protoMsg);
    m_audioSourceAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioSourceAction_Test, Run_AudioON)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrAudioON> protoMsg(VrAudioON::default_instance().New());
    m_audioSourceAction = VR_new VR_AudioSourceAction(m_sp_EventSender, 1, AudioON, *protoMsg);
    m_audioSourceAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioSourceAction_Test, Run_Other)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrAudioON> protoMsg(VrAudioON::default_instance().New());
    m_audioSourceAction = VR_new VR_AudioSourceAction(m_sp_EventSender, 1, PlayAlbum, *protoMsg);
    m_audioSourceAction->Run();
    SUCCEED();
}

TEST_F(VR_AudioSourceAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(AudioON);
    protoMsg->set_processing_result(true);

    m_audioSourceAction->OnReceive(*protoMsg);
    SUCCEED();
}

/* EOF */
