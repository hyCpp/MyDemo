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

#include "stdafx.h"
#include "gtest/gtest.h"
#include "VR_DialogEngine_mock.h"
// #include "VR_AvcLanSender_mock.h"

#include "VR_AudioPlayingAction.h"
#include "VR_AudioSourceAction.h"
#include "VrAgent/VR_AudioInfoAction.h"
#include "VR_AudioPlayAndShow.h"
#include "VR_ActionTtsPlay.h"
#include "VR_ActionTtsStop.h"
#include "VR_ActionBeepPlay.h"
#include "VR_ActionBeepStop.h"
#include "VR_RadioActiveBandInfoAction.h"
#include "VR_InfoActionFactory.h"
#include "VR_RadioInfoAction.h"
#include "VR_HVACFacilitySwitchAction.h"
#include "VR_HVACTuneClimateAction.h"
#include "VR_HVACTuneFanAction.h"
#include "VR_HVACStatusInfoAction.h"
#include "VR_HVACShowScreenAction.h"
#include "VR_HVACValidTemperature.h"
#include "VR_HVACValidFanSpeed.h"
#include "VR_HVACCheckTemperature.h"
#include "VR_HVACCheckFanSpeed.h"
#include "VR_VehicleMotion.h"
#include "VR_SmartAgentPrompt.h"
#include "VR_VoiceTagAction.h"
#include "VR_HFDGetInfoAction.h"
#include "VR_HFDGetMsgBodyAction.h"
#include "VR_AppsStartAction.h"
#include "VR_ActionEventSender.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_InfoActionFactory_Test
*
* The class is just for VR_InfoActionFactory test.
*/
class VR_InfoActionFactory_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_InfoActionFactory *m_VR_InfoActionFactory;
    VR_InfoActionFactory *m_VR_InfoActionFactoryAvcLan;
};

void VR_InfoActionFactory_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_VR_InfoActionFactory = VR_new VR_InfoActionFactory(m_sp_EventSender);
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_VR_InfoActionFactoryAvcLan = VR_new VR_InfoActionFactory(m_sp_EventSender, spAvcSender);
}

void VR_InfoActionFactory_Test::TearDown()
{
    if (NULL != m_VR_InfoActionFactory) {
        delete m_VR_InfoActionFactory;
        m_VR_InfoActionFactory = NULL;
    }
}

TEST_F(VR_InfoActionFactory_Test, CreateAction_VrRecogResult)
{
    int actionId = 1;
    std::unique_ptr<VrRecogResult> protoMsg(VrRecogResult::default_instance().New());
    // SwitchSource
    protoMsg->set_action_id(SwitchSource);
    protoMsg->mutable_switch_source()->set_source_name("usb");
    protoMsg->mutable_switch_source()->set_audio_mode(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayAlbum
    protoMsg->set_action_id(PlayAlbum);
    protoMsg->mutable_play_album()->set_source_type(1);
    protoMsg->mutable_play_album()->set_source_id(1);
    protoMsg->mutable_play_album()->set_shuffle_on(1);
    protoMsg->mutable_play_album()->set_album_name("0");
    protoMsg->mutable_play_album()->set_artist_name("0");
    protoMsg->mutable_play_album()->set_album_id(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayAll
    protoMsg->set_action_id(PlayAll);
    protoMsg->mutable_play_all()->set_source_type(1);
    protoMsg->mutable_play_all()->set_source_id(1);
    protoMsg->mutable_play_all()->set_shuffle_on(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayArtist
    protoMsg->set_action_id(PlayArtist);
    protoMsg->mutable_play_artist()->set_source_type(1);
    protoMsg->mutable_play_artist()->set_source_id(1);
    protoMsg->mutable_play_artist()->set_shuffle_on(1);
    protoMsg->mutable_play_artist()->set_artist_name("1");
    protoMsg->mutable_play_artist()->set_artist_id(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayAudioBook
    protoMsg->set_action_id(PlayAudioBook);
    protoMsg->mutable_play_audio_book()->set_source_type(1);
    protoMsg->mutable_play_audio_book()->set_source_id(1);
    protoMsg->mutable_play_audio_book()->set_audio_book_name("1");
    protoMsg->mutable_play_audio_book()->set_audio_book_id(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayComposer
    protoMsg->set_action_id(PlayComposer);
    protoMsg->mutable_play_composer()->set_source_type(1);
    protoMsg->mutable_play_composer()->set_source_id(1);
    protoMsg->mutable_play_composer()->set_shuffle_on(1);
    protoMsg->mutable_play_composer()->set_composer_name("1");
    protoMsg->mutable_play_composer()->set_composer_id(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayGenre
    protoMsg->set_action_id(PlayGenre);
    protoMsg->mutable_play_genre()->set_source_type(1);
    protoMsg->mutable_play_genre()->set_source_id(1);
    protoMsg->mutable_play_genre()->set_shuffle_on(1);
    protoMsg->mutable_play_genre()->set_genre_name("1");
    protoMsg->mutable_play_genre()->set_genre_id(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayPlaylist
    protoMsg->set_action_id(PlayPlaylist);
    protoMsg->mutable_play_list()->set_source_type(1);
    protoMsg->mutable_play_list()->set_source_id(1);
    protoMsg->mutable_play_list()->set_shuffle_on(1);
    protoMsg->mutable_play_list()->set_playlist_name("1");
    protoMsg->mutable_play_list()->set_playlist_id(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayPodcast
    protoMsg->set_action_id(PlayPodcast);
    protoMsg->mutable_play_podcast()->set_source_type(1);
    protoMsg->mutable_play_podcast()->set_source_id(1);
    protoMsg->mutable_play_podcast()->set_podcast_name("1");
    protoMsg->mutable_play_podcast()->set_podcast_id(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlayPodcastItem
    protoMsg->set_action_id(PlayPodcastItem);
    protoMsg->mutable_play_podcast_item()->set_source_type(1);
    protoMsg->mutable_play_podcast_item()->set_source_id(1);
    protoMsg->mutable_play_podcast_item()->set_podcast_name("1");
    protoMsg->mutable_play_podcast_item()->set_podcast_id(1);
    protoMsg->mutable_play_podcast_item()->set_episode_name("1");
    protoMsg->mutable_play_podcast_item()->set_episode_id(1);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PlaySong
    protoMsg->set_action_id(PlaySong);
    protoMsg->mutable_play_song()->set_source_type(1);
    protoMsg->mutable_play_song()->set_source_id(1);
    protoMsg->mutable_play_song()->set_song_id(1);
    protoMsg->mutable_play_song()->set_song_name("1");
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // AutoAirConditionerON
    protoMsg->set_action_id(AutoAirConditionerON);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // AutoAirConditionerOFF
    protoMsg->set_action_id(AutoAirConditionerOFF);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ACON
    protoMsg->set_action_id(ACON);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ACOFF
    protoMsg->set_action_id(ACOFF);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // DualModeON
    protoMsg->set_action_id(DualModeON);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // DualModeOFF
    protoMsg->set_action_id(DualModeOFF);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // WiperDeicerON
    protoMsg->set_action_id(WiperDeicerON);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // WiperDeicerOFF
    protoMsg->set_action_id(WiperDeicerOFF);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RearSeatAirConditionerON
    protoMsg->set_action_id(RearSeatAirConditionerON);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RearSeatAirConditionerOFF
    protoMsg->set_action_id(RearSeatAirConditionerOFF);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RearDefoggerON
    protoMsg->set_action_id(RearDefoggerON);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RearDefoggerOFF
    protoMsg->set_action_id(RearDefoggerOFF);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ClimateConciergeModeON
    protoMsg->set_action_id(ClimateConciergeModeON);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ChangeTemperature
    protoMsg->set_action_id(ChangeTemperature);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // DecreaseTemperature
    protoMsg->set_action_id(DecreaseTemperature);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // IncreaseTemperature
    protoMsg->set_action_id(IncreaseTemperature);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // AFewWarmer
    protoMsg->set_action_id(AFewWarmer);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // AFewCooler
    protoMsg->set_action_id(AFewCooler);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TemperatureMax
    protoMsg->set_action_id(TemperatureMax);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TemperatureMin
    protoMsg->set_action_id(TemperatureMin);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ChangeFanSpeed
    protoMsg->set_action_id(ChangeFanSpeed);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // FanSpeedMax
    protoMsg->set_action_id(FanSpeedMax);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // FanSpeedMin
    protoMsg->set_action_id(FanSpeedMin);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // BlowerModeFace
    protoMsg->set_action_id(BlowerModeFace);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // BlowerModeFoot
    protoMsg->set_action_id(BlowerModeFoot);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // BlowerModeFaceAndFoot
    protoMsg->set_action_id(BlowerModeFaceAndFoot);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // BlowerModeWindowAndFoot
    protoMsg->set_action_id(BlowerModeWindowAndFoot);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // HVACFrontScreen
    protoMsg->set_action_id(HVACFrontScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // HVACRearScreen
    protoMsg->set_action_id(HVACRearScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // HVACSteeringScreen
    protoMsg->set_action_id(HVACSteeringScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // HVACFrontSeatScreen
    protoMsg->set_action_id(HVACFrontSeatScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // HVACRearSeatScreen
    protoMsg->set_action_id(HVACRearSeatScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // HVACLexusConciergeScreen
    protoMsg->set_action_id(HVACLexusConciergeScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // HVACSeatOperationScreen
    protoMsg->set_action_id(HVACSeatOperationScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // FrontSeatVentilationScreen
    protoMsg->set_action_id(FrontSeatVentilationScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RearSeatVentilationScreen
    protoMsg->set_action_id(RearSeatVentilationScreen);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ChangeHDSubChannel
    protoMsg->set_action_id(ChangeHDSubChannel);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneFrequency
    protoMsg->set_action_id(TuneFrequency);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneFMHDFrequency
    protoMsg->set_action_id(TuneFMHDFrequency);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneFrequencyAM
    protoMsg->set_action_id(TuneFrequencyAM);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneFrequencyFM
    protoMsg->set_action_id(TuneFrequencyFM);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneFrequencySatellite
    protoMsg->set_action_id(TuneFrequencySatellite);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TunePreset
    protoMsg->set_action_id(TunePreset);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneSatelliteChannelName
    protoMsg->set_action_id(TuneSatelliteChannelName);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneSatelliteChannelNumber
    protoMsg->set_action_id(TuneSatelliteChannelNumber);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneDABChannelName
    protoMsg->set_action_id(TuneDABChannelName);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneFMChannelName
    protoMsg->set_action_id(TuneFMChannelName);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneFMGenre
    protoMsg->set_action_id(TuneFMGenre);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TuneSatelliteGenre
    protoMsg->set_action_id(TuneSatelliteGenre);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // StartApps
    protoMsg->set_action_id(StartApps);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TagSong
    protoMsg->set_action_id(TagSong);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    SUCCEED();
}

TEST_F(VR_InfoActionFactory_Test, CreateAction_VrAdditionalInfoReq)
{
    int actionId = 1;
    std::unique_ptr<VrAdditionalInfoReq> protoMsg(VrAdditionalInfoReq::default_instance().New());
    // USBDeviceInfo
    protoMsg->set_request_id(USBDeviceInfo);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RadioBandState
    protoMsg->set_request_id(RadioBandState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // CurFMChannelState
    protoMsg->set_request_id(CurFMChannelState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // SpecifiedFMState
    protoMsg->set_request_id(SpecifiedFMState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PresetState
    protoMsg->set_request_id(PresetState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // SpecifiedPresetState
    protoMsg->set_request_id(SpecifiedPresetState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RadioState
    protoMsg->set_request_id(RadioState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // GenreStationState
    protoMsg->set_request_id(GenreStationState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // AirConditionState
    protoMsg->set_request_id(AirConditionState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ACState
    protoMsg->set_request_id(ACState);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // Temperature
    protoMsg->set_request_id(Temperature);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TemperatureRange
    protoMsg->set_request_id(TemperatureRange);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // FanSpeed
    protoMsg->set_request_id(FanSpeed);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // DualMode
    protoMsg->set_request_id(DualMode);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // WiperDeicer
    protoMsg->set_request_id(WiperDeicer);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RearDefogger
    protoMsg->set_request_id(RearDefogger);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // BlowerMode
    protoMsg->set_request_id(BlowerMode);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ClimateConciergeMode
    protoMsg->set_request_id(ClimateConciergeMode);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // default
    protoMsg->set_request_id(100);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_InfoActionFactory_Test, CreateAction_VrTTSCommandReq)
{
    int actionId = 1;
    std::unique_ptr<VrTTSCommandReq> protoMsg(VrTTSCommandReq::default_instance().New());
    // TTSProcessType_Play
    protoMsg->set_func_code(TTSProcessType_Play);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TTSProcessType_Stop
    protoMsg->set_func_code(TTSProcessType_Stop);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // TTSProcessType_Loop
    protoMsg->set_func_code(TTSProcessType_Loop);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_InfoActionFactory_Test, CreateAction_VrBeepCommandReq)
{
    int actionId = 1;
    std::unique_ptr<VrBeepCommandReq> protoMsg(VrBeepCommandReq::default_instance().New());
    // BeepProcessType_Play
    protoMsg->set_func_code(BeepProcessType_Play);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // BeepProcessType_Stop
    protoMsg->set_func_code(BeepProcessType_Stop);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // default
    protoMsg->set_func_code((navi::VoiceRecog::VrBeepProcessType)2);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_InfoActionFactory_Test, CreateAction_VrActiveInfoReq)
{
    int actionId = 1;
    std::unique_ptr<VrActiveInfoReq> protoMsg(VrActiveInfoReq::default_instance().New());
    // RadioActiveBand
    protoMsg->set_request_id(RadioActiveBand);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ValidTemperature
    protoMsg->set_request_id(ValidTemperature);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // ValidFanSpeed
    protoMsg->set_request_id(ValidFanSpeed);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // CheckTemperature
    protoMsg->set_request_id(CheckTemperature);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // CheckFanSpeed
    protoMsg->set_request_id(CheckFanSpeed);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // MusicPlayString
    protoMsg->set_request_id(MusicPlayString);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // VehicleInMotion
    protoMsg->set_request_id(VehicleInMotion);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // SmartAgentPromptLevel
    protoMsg->set_request_id(SmartAgentPromptLevel);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RecentCallList
    protoMsg->set_request_id(RecentCallList);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RecentIncomingCall
    protoMsg->set_request_id(RecentIncomingCall);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RecentOutgoingCall
    protoMsg->set_request_id(RecentOutgoingCall);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RecentMessageList
    protoMsg->set_request_id(RecentMessageList);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // RecentMessageListByType
    protoMsg->set_request_id(RecentMessageListByType);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // PhoneVehicleInMontion
    protoMsg->set_request_id(PhoneVehicleInMontion);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
    // default
    protoMsg->set_request_id((navi::VoiceRecog::VrActiveInfoId)1000);
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_InfoActionFactory_Test, CreateAction_VrMessageAcquisitionReq)
{
    int actionId = 1;
    std::unique_ptr<VrMessageAcquisitionReq> protoMsg(VrMessageAcquisitionReq::default_instance().New());
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_InfoActionFactory_Test, CreateAction_VrActionNavi)
{
    int actionId = 1;
    std::unique_ptr<VrActionNavi> protoMsg(VrActionNavi::default_instance().New());
    m_VR_InfoActionFactory->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_InfoActionFactory_Test, CreateAction_RequestVrVoiceTag)
{
    int actionId = 1;
    EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    m_VR_InfoActionFactory->CreateAction(context, *protoMsg);
}

/* EOF */
