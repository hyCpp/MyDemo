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
#include "VR_RadioTuningAction_VrAgent.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_AvcLanSenderIF.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"

using namespace navi::VoiceRecog;
using namespace nutshell::media;

/**
* VR_RadioTuningAction_VrAgent_Test
*
* The class is just for VR_RadioTuningAction_VrAgent test.
*/
class VR_RadioTuningAction_VrAgent_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_RadioTuningAction_VrAgent *m_vr_radio_tuning_action;
};

void VR_RadioTuningAction_VrAgent_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrChangeHDSubChannel> protoMsg(VrChangeHDSubChannel::default_instance().New());
    protoMsg->set_fm_sps_number(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_tuning_action = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, ChangeHDSubChannel, *protoMsg, avclanSender);
}

void VR_RadioTuningAction_VrAgent_Test::TearDown()
{
    if (NULL != m_vr_radio_tuning_action) {
        delete m_vr_radio_tuning_action;
        m_vr_radio_tuning_action = NULL;
    }
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_StartRadio_False)
{
    // StartRadio
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrStartRadio> protoMsg(VrStartRadio::default_instance().New());
    protoMsg->clear_radio_band_kind();
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_tuning_action = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, StartRadio, *protoMsg, avclanSender);
    m_vr_radio_tuning_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_ChangeHDSubChannel)
{
    // ChangeHDSubChannel
    m_vr_radio_tuning_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_ChangeHDSubChannel_NoNumber)
{
    // ChangeHDSubChannel
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrChangeHDSubChannel> protoMsg(VrChangeHDSubChannel::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_tuning_action = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, ChangeHDSubChannel, *protoMsg, avclanSender);
    m_vr_radio_tuning_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_ChangeHDSubChannel_false)
{
    // ChangeHDSubChannel
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrChangeHDSubChannel> protoMsg(VrChangeHDSubChannel::default_instance().New());
    protoMsg->clear_fm_sps_number();
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_tuning_action = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, ChangeHDSubChannel, *protoMsg, avclanSender);
    m_vr_radio_tuning_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFrequency)
{
    // TuneFrequency
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFrequency> tuneFrequency(VrTuneFrequency::default_instance().New());
    tuneFrequency->set_radio_band_kind(0);
    tuneFrequency->set_frequency(1000);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFrequencyAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFrequency, *tuneFrequency, avclanSender);
    tuneFrequencyAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFrequency_NoBandFre)
{
    // TuneFrequency
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFrequency> tuneFrequency(VrTuneFrequency::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFrequencyAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 2, TuneFrequency, *tuneFrequency, avclanSender);
    tuneFrequencyAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFrequency_Nullkind)
{
    // TuneFrequency
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFrequency> tuneFrequency(VrTuneFrequency::default_instance().New());
    tuneFrequency->clear_radio_band_kind();
    tuneFrequency->set_frequency(1000);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFrequencyAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFrequency, *tuneFrequency, avclanSender);
    tuneFrequencyAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFrequency_Nullfrequency)
{
    // TuneFrequency
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFrequency> tuneFrequency(VrTuneFrequency::default_instance().New());
    tuneFrequency->set_radio_band_kind(0);
    tuneFrequency->clear_frequency();
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFrequencyAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFrequency, *tuneFrequency, avclanSender);
    tuneFrequencyAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMHDFrequency)
{
    // TuneFMHDFrequency
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMHDFrequency> tuneFMHDFrequency(VrTuneFMHDFrequency::default_instance().New());
    tuneFMHDFrequency->set_fm_sps_number(1);
    tuneFMHDFrequency->set_frequency(10170);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMHDFrequencyAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMHDFrequency, *tuneFMHDFrequency, avclanSender);
    tuneFMHDFrequencyAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMHDFrequency_Nullsps)
{
    // TuneFMHDFrequency
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMHDFrequency> tuneFMHDFrequency(VrTuneFMHDFrequency::default_instance().New());
    tuneFMHDFrequency->clear_fm_sps_number();
    tuneFMHDFrequency->set_frequency(10170);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMHDFrequencyAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMHDFrequency, *tuneFMHDFrequency, avclanSender);
    tuneFMHDFrequencyAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMHDFrequency_Nullfrequency)
{
    // TuneFMHDFrequency
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMHDFrequency> tuneFMHDFrequency(VrTuneFMHDFrequency::default_instance().New());
    tuneFMHDFrequency->set_fm_sps_number(1);
    tuneFMHDFrequency->clear_frequency();
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMHDFrequencyAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMHDFrequency, *tuneFMHDFrequency, avclanSender);
    tuneFMHDFrequencyAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMHDFrequency_NoFreNumber)
{
    // TuneFMHDFrequency
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMHDFrequency> tuneFMHDFrequency(VrTuneFMHDFrequency::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMHDFrequencyAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMHDFrequency, *tuneFMHDFrequency, avclanSender);
    tuneFMHDFrequencyAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFrequencyAM)
{
    // TuneFrequencyAM
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFrequencyAM> tuneFrequencyAM(VrTuneFrequencyAM::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFrequencyAMAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFrequencyAM, *tuneFrequencyAM, avclanSender);
    tuneFrequencyAMAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFrequencyFM)
{
    // TuneFrequencyFM
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFrequencyFM> tuneFrequencyFM(VrTuneFrequencyFM::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFrequencyFMAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFrequencyFM, *tuneFrequencyFM, avclanSender);
    tuneFrequencyFMAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFrequencySatellite)
{
    // TuneFrequencySatellite
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFrequencySatellite> tuneFrequencySatellite(VrTuneFrequencySatellite::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFrequencySatelliteAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFrequencySatellite, *tuneFrequencySatellite, avclanSender);
    tuneFrequencySatelliteAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TunePreset)
{
    // TunePreset
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTunePreset> tunePreset(VrTunePreset::default_instance().New());
    tunePreset->set_radio_band_kind(0);
    tunePreset->set_preset_number(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tunePresetAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TunePreset, *tunePreset, avclanSender);
    tunePresetAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TunePreset_MIX)
{
    // TunePreset
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTunePreset> tunePreset(VrTunePreset::default_instance().New());
    tunePreset->set_radio_band_kind(0xFF);
    tunePreset->set_preset_number(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tunePresetAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TunePreset, *tunePreset, avclanSender);
    tunePresetAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TunePreset_NoBand)
{
    // TunePreset
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTunePreset> tunePreset(VrTunePreset::default_instance().New());
    tunePreset->clear_radio_band_kind();
    tunePreset->set_preset_number(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tunePresetAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TunePreset, *tunePreset, avclanSender);
    tunePresetAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteChannelName)
{
    // TuneSatelliteChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteChannelName> tuneSatelliteChannelName(VrTuneSatelliteChannelName::default_instance().New());
    tuneSatelliteChannelName->set_station_name("Satellite");
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteChannelName, *tuneSatelliteChannelName, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteChannelName_Null)
{
    // TuneSatelliteChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteChannelName> tuneSatelliteChannelName(VrTuneSatelliteChannelName::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteChannelName, *tuneSatelliteChannelName, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteChannelName_equalName)
{
    // TuneSatelliteChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteChannelName> tuneSatelliteChannelName(VrTuneSatelliteChannelName::default_instance().New());
    tuneSatelliteChannelName->set_station_name("Satellite");
    struct MediaInfo_XmChannelInfo xmChannelInfo;
    memset(&xmChannelInfo, 0, sizeof(xmChannelInfo));
    xmChannelInfo.channelValid = 1;
    strcpy((char*)xmChannelInfo.channels[0].channelName3, "Satellite");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_XmChannelInfo, &xmChannelInfo, sizeof(xmChannelInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteChannelName, *tuneSatelliteChannelName, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteChannelName_diffName)
{
    // TuneSatelliteChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteChannelName> tuneSatelliteChannelName(VrTuneSatelliteChannelName::default_instance().New());
    tuneSatelliteChannelName->set_station_name("Satellite");
    struct MediaInfo_XmChannelInfo xmChannelInfo;
    memset(&xmChannelInfo, 0, sizeof(xmChannelInfo));
    xmChannelInfo.channelValid = 1;
    strcpy((char*)xmChannelInfo.channels[0].channelName3, "Satellite1");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_XmChannelInfo, &xmChannelInfo, sizeof(xmChannelInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteChannelName, *tuneSatelliteChannelName, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteChannelNumber)
{
    // TuneSatelliteChannelNumber
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteChannelNumber> tuneSatelliteChannelNumber(VrTuneSatelliteChannelNumber::default_instance().New());
    tuneSatelliteChannelNumber->set_ch_number(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteChannelNumber, *tuneSatelliteChannelNumber, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteChannelNumberNoNum)
{
    // TuneSatelliteChannelNumber
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteChannelNumber> tuneSatelliteChannelNumber(VrTuneSatelliteChannelNumber::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteChannelNumber, *tuneSatelliteChannelNumber, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneDABChannelName)
{
    // TuneDABChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneDABChannelName> tuneDABChannelName(VrTuneDABChannelName::default_instance().New());
    tuneDABChannelName->set_station_name("dabChannel");
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneDABChannelName, *tuneDABChannelName, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneDABChannelName_equalName)
{
    // TuneDABChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneDABChannelName> tuneDABChannelName(VrTuneDABChannelName::default_instance().New());
    tuneDABChannelName->set_station_name("dabChannel");
    struct MediaInfo_DabServiceList dabServiceList;
    memset(&dabServiceList, 0, sizeof(dabServiceList));
    dabServiceList.serviceNum = 1;
    strcpy((char*)dabServiceList.componentLabel[0], "dabChannel");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_DabServiceList, &dabServiceList, sizeof(dabServiceList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneDABChannelName, *tuneDABChannelName, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneDABChannelName_diffName)
{
    // TuneDABChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneDABChannelName> tuneDABChannelName(VrTuneDABChannelName::default_instance().New());
    tuneDABChannelName->set_station_name("dabChannel");
    struct MediaInfo_DabServiceList dabServiceList;
    memset(&dabServiceList, 0, sizeof(dabServiceList));
    dabServiceList.serviceNum = 1;
    strcpy((char*)dabServiceList.componentLabel[0], "dabChannel1");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_DabServiceList, &dabServiceList, sizeof(dabServiceList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneSatelliteChannelNumberAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneDABChannelName, *tuneDABChannelName, avclanSender);
    tuneSatelliteChannelNumberAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMGenre)
{
    // TuneFMGenre
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMGenre> tuneFMGenre(VrTuneFMGenre::default_instance().New());
    tuneFMGenre->set_genre_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMGenre, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMGenre_Null)
{
    // TuneFMGenre
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMGenre> tuneFMGenre(VrTuneFMGenre::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMGenre, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteGenre)
{
    // TuneSatelliteGenre
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteGenre> tuneFMGenre(VrTuneSatelliteGenre::default_instance().New());
    tuneFMGenre->set_genre_name("genre");
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteGenre, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteGenre_equalName)
{
    // TuneSatelliteGenre
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteGenre> tuneFMGenre(VrTuneSatelliteGenre::default_instance().New());
    tuneFMGenre->set_genre_name("genre");
    struct MediaInfo_XmCategoryInfo xmCategoryInfo;
    memset(&xmCategoryInfo, 0, sizeof(xmCategoryInfo));
    xmCategoryInfo.categoryValid = 1;
    strcpy((char*)xmCategoryInfo.categorys[0].categoryName3, "genre");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_XmCategoryInfo, &xmCategoryInfo, sizeof(xmCategoryInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteGenre, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneSatelliteGenre_diffName)
{
    // TuneSatelliteGenre
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneSatelliteGenre> tuneFMGenre(VrTuneSatelliteGenre::default_instance().New());
    tuneFMGenre->set_genre_name("genre");
    struct MediaInfo_XmCategoryInfo xmCategoryInfo;
    memset(&xmCategoryInfo, 0, sizeof(xmCategoryInfo));
    xmCategoryInfo.categoryValid = 1;
    strcpy((char*)xmCategoryInfo.categorys[0].categoryName3, "genre1");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_XmCategoryInfo, &xmCategoryInfo, sizeof(xmCategoryInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneSatelliteGenre, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMChannelName)
{
    // TuneFMChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMChannelName> tuneFMGenre(VrTuneFMChannelName::default_instance().New());
    tuneFMGenre->set_station_name("fmChannel");
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMChannelName, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMChannelName_equalName)
{
    // TuneFMChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMChannelName> tuneFMGenre(VrTuneFMChannelName::default_instance().New());
    tuneFMGenre->set_station_name("fmChannel");
    struct MediaInfo_TunerStationList tunerStationList;
    memset(&tunerStationList, 0, sizeof(tunerStationList));
    tunerStationList.stationNum = 1;
    strcpy((char*)tunerStationList.station[0].psName, "fmChannel");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_TunerStationList, &tunerStationList, sizeof(tunerStationList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMChannelName, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_TuneFMChannelName_diffName)
{
    // TuneFMChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMChannelName> tuneFMGenre(VrTuneFMChannelName::default_instance().New());
    tuneFMGenre->set_station_name("fmChannel");
    struct MediaInfo_TunerStationList tunerStationList;
    memset(&tunerStationList, 0, sizeof(tunerStationList));
    tunerStationList.stationNum = 1;
    strcpy((char*)tunerStationList.station[0].psName, "fmChannel1");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_TunerStationList, &tunerStationList, sizeof(tunerStationList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TuneFMChannelName, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, Run_other)
{
    // TuneFMChannelName
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMChannelName> tuneFMGenre(VrTuneFMChannelName::default_instance().New());
    tuneFMGenre->set_station_name("fmChannel");
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    VR_RadioTuningAction_VrAgent *tuneFMGenreAction = VR_new VR_RadioTuningAction_VrAgent(m_sp_EventSender, 1, TagSong, *tuneFMGenre, avclanSender);
    tuneFMGenreAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(ChangeHDSubChannel);
    protoMsg->set_processing_result(true);

    m_vr_radio_tuning_action->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_RadioTuningAction_VrAgent_Test, ChangeBand2SourceID_Case)
{
    int nBand = VR_RadioTuningAction_VrAgent::VR_RadioBand_AM;
    EXPECT_EQ(MediaInfo_SrcID_AM, m_vr_radio_tuning_action->ChangeBand2SourceID(nBand));
    nBand = VR_RadioTuningAction_VrAgent::VR_RadioBand_FM;
    EXPECT_EQ(MediaInfo_SrcID_FM, m_vr_radio_tuning_action->ChangeBand2SourceID(nBand));
    nBand = VR_RadioTuningAction_VrAgent::VR_RadioBand_XM;
    EXPECT_EQ(MediaInfo_SrcID_SXM, m_vr_radio_tuning_action->ChangeBand2SourceID(nBand));
    nBand = VR_RadioTuningAction_VrAgent::VR_RadioBand_DAB;
    EXPECT_EQ(MediaInfo_SrcID_DAB, m_vr_radio_tuning_action->ChangeBand2SourceID(nBand));
    nBand = VR_RadioTuningAction_VrAgent::VR_RadioBand_MIX;
    EXPECT_EQ(MediaInfo_SrcID_Invalid, m_vr_radio_tuning_action->ChangeBand2SourceID(nBand));
    SUCCEED();
}

/* EOF */
