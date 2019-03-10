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
#include "VR_AudioSourceAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_AudioSourceAction_VrUnit_Test
*
* The class is just for VR_AudioSourceAction_VrUnit test.
*/
class VR_AudioSourceAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_AudioSourceAction_VrUnit *m_action;
};

void
VR_AudioSourceAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = AudioOFF;
     std::unique_ptr<VrAudioOFF> ProtoMsg(VrAudioOFF::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
     m_action = VR_new VR_AudioSourceAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
}

void
VR_AudioSourceAction_VrUnit_Test::TearDown()
{
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
}

TEST_F(VR_AudioSourceAction_VrUnit_Test, Run_AudioOFF)
{
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_AudioSourceAction_VrUnit_Test, Run_AudioON)
{
    int nEngineId = 101;
    int nActionType = AudioON;
    std::unique_ptr<VrAudioON> ProtoMsg(VrAudioON::default_instance().New());
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_AudioSourceAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_AudioSourceAction_VrUnit_Test, Run_SwitchSource)
{
    int nEngineId = 101;
    int nActionType = SwitchSource;
    std::unique_ptr<VrSwitchSource> ProtoMsg(VrSwitchSource::default_instance().New());
    ProtoMsg->set_source_name("USB");
    ProtoMsg->set_audio_mode(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_AudioSourceAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_AudioSourceAction_VrUnit_Test, Run_default)
{
    int nEngineId = 101;
    int nActionType = PlayAlbum;
    std::unique_ptr<VrAudioON> ProtoMsg(VrAudioON::default_instance().New());
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_AudioSourceAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_AudioSourceAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(AudioOFF);
    protoMsg->set_processing_result(true);

    m_action->OnReceive(*protoMsg);
    SUCCEED();
}
/* EOF */


