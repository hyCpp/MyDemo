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
#include "VR_RadioTuningAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_RadioTuningAction_VrUnit_Test
*
* The class is just for VR_RadioTuningAction_VrUnit test.
*/
class VR_RadioTuningAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_RadioTuningAction_VrUnit *m_action;
};

void
VR_RadioTuningAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = ChangeHDSubChannel;
     std::unique_ptr<VrChangeHDSubChannel> ProtoMsg(VrChangeHDSubChannel::default_instance().New());
     ProtoMsg->set_fm_sps_number(1);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
     m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
}

void
VR_RadioTuningAction_VrUnit_Test::TearDown()
{
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_ChangeHDSubChannel)
{
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneFrequency)
{
    int nEngineId = 101;
    int nActionType = TuneFrequency;
    std::unique_ptr<VrTuneFrequency> ProtoMsg(VrTuneFrequency::default_instance().New());
    ProtoMsg->set_radio_band_kind(0);
    ProtoMsg->set_frequency(1000);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneFMHDFrequency)
{
    int nEngineId = 101;
    int nActionType = TuneFMHDFrequency;
    std::unique_ptr<VrTuneFMHDFrequency> ProtoMsg(VrTuneFMHDFrequency::default_instance().New());
    ProtoMsg->set_fm_sps_number(1);
    ProtoMsg->set_frequency(10170);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneFrequencyAM)
{
    int nEngineId = 101;
    int nActionType = TuneFrequencyAM;
    std::unique_ptr<VrTuneFrequencyAM> ProtoMsg(VrTuneFrequencyAM::default_instance().New());
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneFrequencyFM)
{
    int nEngineId = 101;
    int nActionType = TuneFrequencyFM;
    std::unique_ptr<VrTuneFrequencyFM> ProtoMsg(VrTuneFrequencyFM::default_instance().New());
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneFrequencySatellite)
{
    int nEngineId = 101;
    int nActionType = TuneFrequencySatellite;
    std::unique_ptr<VrTuneFrequencySatellite> ProtoMsg(VrTuneFrequencySatellite::default_instance().New());
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TunePreset)
{
    int nEngineId = 101;
    int nActionType = TunePreset;
    std::unique_ptr<VrTunePreset> ProtoMsg(VrTunePreset::default_instance().New());
    ProtoMsg->set_radio_band_kind(0);
    ProtoMsg->set_preset_number(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneSatelliteChannelName)
{
    int nEngineId = 101;
    int nActionType = TuneSatelliteChannelName;
    std::unique_ptr<VrTuneSatelliteChannelName> ProtoMsg(VrTuneSatelliteChannelName::default_instance().New());
    ProtoMsg->set_station_name("station");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneSatelliteChannelNumber)
{
    int nEngineId = 101;
    int nActionType = TuneSatelliteChannelNumber;
    std::unique_ptr<VrTuneSatelliteChannelNumber> ProtoMsg(VrTuneSatelliteChannelNumber::default_instance().New());
    ProtoMsg->set_ch_number(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneDABChannelName)
{
    int nEngineId = 101;
    int nActionType = TuneDABChannelName;
    std::unique_ptr<VrTuneDABChannelName> ProtoMsg(VrTuneDABChannelName::default_instance().New());
    ProtoMsg->set_station_name("station");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneFMGenre)
{
    int nEngineId = 101;
    int nActionType = TuneFMGenre;
    std::unique_ptr<VrTuneFMGenre> ProtoMsg(VrTuneFMGenre::default_instance().New());
    ProtoMsg->set_genre_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneSatelliteGenre)
{
    int nEngineId = 101;
    int nActionType = TuneSatelliteGenre;
    std::unique_ptr<VrTuneSatelliteGenre> ProtoMsg(VrTuneSatelliteGenre::default_instance().New());
    ProtoMsg->set_genre_name("genre");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_TuneFMChannelName)
{
    int nEngineId = 101;
    int nActionType = TuneFMChannelName;
    std::unique_ptr<VrTuneFMChannelName> ProtoMsg(VrTuneFMChannelName::default_instance().New());
    ProtoMsg->set_station_name("station");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, Run_default)
{
    int nEngineId = 101;
    int nActionType = Call;
    std::unique_ptr<VrTuneFMChannelName> ProtoMsg(VrTuneFMChannelName::default_instance().New());
    ProtoMsg->set_station_name("genre");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_action = VR_new VR_RadioTuningAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(ChangeHDSubChannel);
    protoMsg->set_processing_result(true);

    m_action->OnReceive(*protoMsg);
    SUCCEED();
}
/* EOF */


