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
#include "VR_AvcLanSender_mock.h"
#include "VR_ActionFactory_VrUnitAgent.h"
#include "VR_RadioActiveBandInfoAction_VrUnit.h"
#include "VR_RadioTuningAction_VrUnit.h"
#include "VR_RadioInfoAction_VrUnit.h"
#include "VR_ActionTtsPlay_VrUnit.h"
#include "VR_ActionTtsStop_VrUnit.h"
#include "VR_AudioPlayingAction_VrUnit.h"
#include "VR_AudioSourceAction_VrUnit.h"
#include "VR_AudioInfoAction_VrUnit.h"
#include "VR_AppsStartAction_VrUnit.h"
#include "VR_VoiceTagAction_VrUnit.h"
#include "VR_ActionEventSender.h"

#include "VR_ClimateFacilitySwitchAction_VrUnit.h"
#include "VR_ClimateTuneAction_VrUnit.h"
#include "VR_ClimateTuneFanAction_VrUnit.h"
#include "VR_ClimateShowScreenAction_VrUnit.h"
#include "VR_ClimateStatusInfoAction_VrUnit.h"
#include "VR_ClimateCheckFanSpeed_VrUnit.h"
#include "VR_ClimateCheckTemperature_VrUnit.h"
#include "VR_ClimateValidFanSpeed_VrUnit.h"
#include "VR_ClimateValidTemperature_VrUnit.h"

#include "VR_HFDDoAction_VrUnit.h"
#include "VR_HFDGetInfoAction_VrUnit.h"
#include "VR_HFDGetMsgBodyAction_VrUnit.h"

#include "VR_NaviQueryLocationInfo.h"
#include "VR_NaviQueryPoiInfo.h"
#include "VR_NaviQueryRouteInfo.h"
#include "VR_NaviQueryLocInfo.h"
#include "VR_NaviQueryLocationInfoCN.h"
#include "action/VR_ActionBeepPlay.h"
#include "action/VR_ActionBeepStop.h"
#include "action/VR_ActionTtsPlay.h"
#include "action/VR_ActionTtsStop.h"
#include "VR_NaviQueryPoiByNameCN.h"

#include "action/VR_GBookStateCheckAction.h"
#include "action/VR_GBookCheckErrorAction.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ActionFactory_VrUnitAgent_Test
*
* The class is just for VR_ActionFactory_VrUnitAgent test.
*/
class VR_ActionFactory_VrUnitAgent_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_ActionFactory_VrUnitAgent *m_VR_ActionFactory_VrUnitAgent;
};

void VR_ActionFactory_VrUnitAgent_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_VR_ActionFactory_VrUnitAgent = VR_new VR_ActionFactory_VrUnitAgent(m_sp_EventSender, spAvcSender);
}

void VR_ActionFactory_VrUnitAgent_Test::TearDown()
{
    if (NULL != m_VR_ActionFactory_VrUnitAgent) {
        delete m_VR_ActionFactory_VrUnitAgent;
        m_VR_ActionFactory_VrUnitAgent = NULL;
    }
}

TEST_F(VR_ActionFactory_VrUnitAgent_Test, CreateAction_VrRecogResult)
{
    int actionId = 1;
    std::unique_ptr<VrRecogResult> protoMsg(VrRecogResult::default_instance().New());
    // AudioOFF
    protoMsg->set_action_id(AudioOFF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // AudioON
    protoMsg->set_action_id(AudioON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // SwitchSource
    protoMsg->set_action_id(SwitchSource);
    protoMsg->mutable_switch_source()->set_source_name("usb");
    protoMsg->mutable_switch_source()->set_audio_mode(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayAlbum
    protoMsg->set_action_id(PlayAlbum);
    protoMsg->mutable_play_album()->set_source_type(1);
    protoMsg->mutable_play_album()->set_source_id(1);
    protoMsg->mutable_play_album()->set_shuffle_on(1);
    protoMsg->mutable_play_album()->set_album_name("0");
    protoMsg->mutable_play_album()->set_artist_name("0");
    protoMsg->mutable_play_album()->set_album_id(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayAll
    protoMsg->set_action_id(PlayAll);
    protoMsg->mutable_play_all()->set_source_type(1);
    protoMsg->mutable_play_all()->set_source_id(1);
    protoMsg->mutable_play_all()->set_shuffle_on(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayArtist
    protoMsg->set_action_id(PlayArtist);
    protoMsg->mutable_play_artist()->set_source_type(1);
    protoMsg->mutable_play_artist()->set_source_id(1);
    protoMsg->mutable_play_artist()->set_shuffle_on(1);
    protoMsg->mutable_play_artist()->set_artist_name("1");
    protoMsg->mutable_play_artist()->set_artist_id(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayAudioBook
    protoMsg->set_action_id(PlayAudioBook);
    protoMsg->mutable_play_audio_book()->set_source_type(1);
    protoMsg->mutable_play_audio_book()->set_source_id(1);
    protoMsg->mutable_play_audio_book()->set_audio_book_name("1");
    protoMsg->mutable_play_audio_book()->set_audio_book_id(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayComposer
    protoMsg->set_action_id(PlayComposer);
    protoMsg->mutable_play_composer()->set_source_type(1);
    protoMsg->mutable_play_composer()->set_source_id(1);
    protoMsg->mutable_play_composer()->set_shuffle_on(1);
    protoMsg->mutable_play_composer()->set_composer_name("1");
    protoMsg->mutable_play_composer()->set_composer_id(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayGenre
    protoMsg->set_action_id(PlayGenre);
    protoMsg->mutable_play_genre()->set_source_type(1);
    protoMsg->mutable_play_genre()->set_source_id(1);
    protoMsg->mutable_play_genre()->set_shuffle_on(1);
    protoMsg->mutable_play_genre()->set_genre_name("1");
    protoMsg->mutable_play_genre()->set_genre_id(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayPlaylist
    protoMsg->set_action_id(PlayPlaylist);
    protoMsg->mutable_play_list()->set_source_type(1);
    protoMsg->mutable_play_list()->set_source_id(1);
    protoMsg->mutable_play_list()->set_shuffle_on(1);
    protoMsg->mutable_play_list()->set_playlist_name("1");
    protoMsg->mutable_play_list()->set_playlist_id(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayPodcast
    protoMsg->set_action_id(PlayPodcast);
    protoMsg->mutable_play_podcast()->set_source_type(1);
    protoMsg->mutable_play_podcast()->set_source_id(1);
    protoMsg->mutable_play_podcast()->set_podcast_name("1");
    protoMsg->mutable_play_podcast()->set_podcast_id(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlayPodcastItem
    protoMsg->set_action_id(PlayPodcastItem);
    protoMsg->mutable_play_podcast_item()->set_source_type(1);
    protoMsg->mutable_play_podcast_item()->set_source_id(1);
    protoMsg->mutable_play_podcast_item()->set_podcast_name("1");
    protoMsg->mutable_play_podcast_item()->set_podcast_id(1);
    protoMsg->mutable_play_podcast_item()->set_episode_name("1");
    protoMsg->mutable_play_podcast_item()->set_episode_id(1);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PlaySong
    protoMsg->set_action_id(PlaySong);
    protoMsg->mutable_play_song()->set_source_type(1);
    protoMsg->mutable_play_song()->set_source_id(1);
    protoMsg->mutable_play_song()->set_song_id(1);
    protoMsg->mutable_play_song()->set_song_name("1");
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // AutoAirConditionerON
    protoMsg->set_action_id(AutoAirConditionerON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // AutoAirConditionerOFF
    protoMsg->set_action_id(AutoAirConditionerOFF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ACON
    protoMsg->set_action_id(ACON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ACOFF
    protoMsg->set_action_id(ACOFF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // DualModeON
    protoMsg->set_action_id(DualModeON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // DualModeOFF
    protoMsg->set_action_id(DualModeOFF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // WiperDeicerON
    protoMsg->set_action_id(WiperDeicerON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // WiperDeicerOFF
    protoMsg->set_action_id(WiperDeicerOFF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RearSeatAirConditionerON
    protoMsg->set_action_id(RearSeatAirConditionerON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RearSeatAirConditionerOFF
    protoMsg->set_action_id(RearSeatAirConditionerOFF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RearDefoggerON
    protoMsg->set_action_id(RearDefoggerON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RearDefoggerOFF
    protoMsg->set_action_id(RearDefoggerOFF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ClimateConciergeModeON
    protoMsg->set_action_id(ClimateConciergeModeON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ChangeTemperature
    protoMsg->set_action_id(ChangeTemperature);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // DecreaseTemperature
    protoMsg->set_action_id(DecreaseTemperature);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // IncreaseTemperature
    protoMsg->set_action_id(IncreaseTemperature);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // AFewWarmer
    protoMsg->set_action_id(AFewWarmer);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // AFewCooler
    protoMsg->set_action_id(AFewCooler);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TemperatureMax
    protoMsg->set_action_id(TemperatureMax);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TemperatureMin
    protoMsg->set_action_id(TemperatureMin);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ChangeFanSpeed
    protoMsg->set_action_id(ChangeFanSpeed);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // FanSpeedMax
    protoMsg->set_action_id(FanSpeedMax);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // FanSpeedMin
    protoMsg->set_action_id(FanSpeedMin);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // BlowerModeFace
    protoMsg->set_action_id(BlowerModeFace);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // BlowerModeFoot
    protoMsg->set_action_id(BlowerModeFoot);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // BlowerModeFaceAndFoot
    protoMsg->set_action_id(BlowerModeFaceAndFoot);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // BlowerModeWindowAndFoot
    protoMsg->set_action_id(BlowerModeWindowAndFoot);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // HVACFrontScreen
    protoMsg->set_action_id(HVACFrontScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // HVACRearScreen
    protoMsg->set_action_id(HVACRearScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // HVACSteeringScreen
    protoMsg->set_action_id(HVACSteeringScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // HVACFrontSeatScreen
    protoMsg->set_action_id(HVACFrontSeatScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // HVACRearSeatScreen
    protoMsg->set_action_id(HVACRearSeatScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // HVACLexusConciergeScreen
    protoMsg->set_action_id(HVACLexusConciergeScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // HVACSeatOperationScreen
    protoMsg->set_action_id(HVACSeatOperationScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // FrontSeatVentilationScreen
    protoMsg->set_action_id(FrontSeatVentilationScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RearSeatVentilationScreen
    protoMsg->set_action_id(RearSeatVentilationScreen);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ChangeHDSubChannel
    protoMsg->set_action_id(ChangeHDSubChannel);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneFrequency
    protoMsg->set_action_id(TuneFrequency);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneFMHDFrequency
    protoMsg->set_action_id(TuneFMHDFrequency);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneFrequencyAM
    protoMsg->set_action_id(TuneFrequencyAM);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneFrequencyFM
    protoMsg->set_action_id(TuneFrequencyFM);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneFrequencySatellite
    protoMsg->set_action_id(TuneFrequencySatellite);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TunePreset
    protoMsg->set_action_id(TunePreset);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneSatelliteChannelName
    protoMsg->set_action_id(TuneSatelliteChannelName);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneSatelliteChannelNumber
    protoMsg->set_action_id(TuneSatelliteChannelNumber);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneDABChannelName
    protoMsg->set_action_id(TuneDABChannelName);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneFMChannelName
    protoMsg->set_action_id(TuneFMChannelName);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneFMGenre
    protoMsg->set_action_id(TuneFMGenre);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TuneSatelliteGenre
    protoMsg->set_action_id(TuneSatelliteGenre);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // StartApps
    protoMsg->set_action_id(StartApps);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // Call
    protoMsg->set_action_id(Call);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // CallWithNo
    protoMsg->set_action_id(CallWithNo);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // CallRecent
    protoMsg->set_action_id(CallRecent);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // MUTEON
    protoMsg->set_action_id(MUTEON);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // MUTEOFF
    protoMsg->set_action_id(MUTEOFF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // CallShortRing
    protoMsg->set_action_id(CallShortRing);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // Redial
    protoMsg->set_action_id(Redial);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // SendFixedMessage
    protoMsg->set_action_id(SendFixedMessage);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // SendDTMF
    protoMsg->set_action_id(SendDTMF);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // SendMessage
    protoMsg->set_action_id(SendMessage);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PhoneListFavourites
    protoMsg->set_action_id(PhoneListFavourites);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ListAllContact
    protoMsg->set_action_id(ListAllContact);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // CallHistory
    protoMsg->set_action_id(CallHistory);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // AddVoiceTag
    protoMsg->set_action_id(AddVoiceTag);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TagSong
    protoMsg->set_action_id(TagSong);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    SUCCEED();
}

TEST_F(VR_ActionFactory_VrUnitAgent_Test, CreateAction_VrAdditionalInfoReq)
{
    int actionId = 1;
    std::unique_ptr<VrAdditionalInfoReq> protoMsg(VrAdditionalInfoReq::default_instance().New());
    // USBDeviceInfo
    protoMsg->set_request_id(USBDeviceInfo);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RadioBandState
    protoMsg->set_request_id(RadioBandState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // CurFMChannelState
    protoMsg->set_request_id(CurFMChannelState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // SpecifiedFMState
    protoMsg->set_request_id(SpecifiedFMState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // PresetState
    protoMsg->set_request_id(PresetState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // SpecifiedPresetState
    protoMsg->set_request_id(SpecifiedPresetState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RadioState
    protoMsg->set_request_id(RadioState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // GenreStationState
    protoMsg->set_request_id(GenreStationState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // AirConditionState
    protoMsg->set_request_id(AirConditionState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ACState
    protoMsg->set_request_id(ACState);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // Temperature
    protoMsg->set_request_id(Temperature);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TemperatureRange
    protoMsg->set_request_id(TemperatureRange);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // FanSpeed
    protoMsg->set_request_id(FanSpeed);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // DualMode
    protoMsg->set_request_id(DualMode);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // WiperDeicer
    protoMsg->set_request_id(WiperDeicer);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RearDefogger
    protoMsg->set_request_id(RearDefogger);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // BlowerMode
    protoMsg->set_request_id(BlowerMode);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ClimateConciergeMode
    protoMsg->set_request_id(ClimateConciergeMode);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // default
    protoMsg->set_request_id(100);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_ActionFactory_VrUnitAgent_Test, CreateAction_VrTTSCommandReq)
{
    int actionId = 1;
    std::unique_ptr<VrTTSCommandReq> protoMsg(VrTTSCommandReq::default_instance().New());
    // TTSProcessType_Play
    protoMsg->set_func_code(TTSProcessType_Play);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TTSProcessType_Stop
    protoMsg->set_func_code(TTSProcessType_Stop);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // TTSProcessType_Loop
    protoMsg->set_func_code(TTSProcessType_Loop);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_ActionFactory_VrUnitAgent_Test, CreateAction_VrBeepCommandReq)
{
    int actionId = 1;
    std::unique_ptr<VrBeepCommandReq> protoMsg(VrBeepCommandReq::default_instance().New());
    // BeepProcessType_Play
    protoMsg->set_func_code(BeepProcessType_Play);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // BeepProcessType_Stop
    protoMsg->set_func_code(BeepProcessType_Stop);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // default
    protoMsg->set_func_code((navi::VoiceRecog::VrBeepProcessType)2);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_ActionFactory_VrUnitAgent_Test, CreateAction_VrActiveInfoReq)
{
    int actionId = 1;
    std::unique_ptr<VrActiveInfoReq> protoMsg(VrActiveInfoReq::default_instance().New());
    // RadioActiveBand
    protoMsg->set_request_id(RadioActiveBand);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ValidTemperature
    protoMsg->set_request_id(ValidTemperature);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // ValidFanSpeed
    protoMsg->set_request_id(ValidFanSpeed);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // CheckTemperature
    protoMsg->set_request_id(CheckTemperature);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // CheckFanSpeed
    protoMsg->set_request_id(CheckFanSpeed);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RecentCallList
    protoMsg->set_request_id(RecentCallList);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RecentIncomingCall
    protoMsg->set_request_id(RecentIncomingCall);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RecentOutgoingCall
    protoMsg->set_request_id(RecentOutgoingCall);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RecentMessageList
    protoMsg->set_request_id(RecentMessageList);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // RecentMessageListByType
    protoMsg->set_request_id(RecentMessageListByType);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // GbookStateCheck
    protoMsg->set_request_id(GbookStateCheck);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // GbookCallAssit
    protoMsg->set_request_id(GbookCallAssit);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // default
    protoMsg->set_request_id((navi::VoiceRecog::VrActiveInfoId)1000);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_ActionFactory_VrUnitAgent_Test, CreateAction_VrMessageAcquisitionReq)
{
    int actionId = 1;
    std::unique_ptr<VrMessageAcquisitionReq> protoMsg(VrMessageAcquisitionReq::default_instance().New());
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_ActionFactory_VrUnitAgent_Test, CreateAction_VrActionNavi)
{
    int actionId = 1;
    std::unique_ptr<VrActionNavi> protoMsg(VrActionNavi::default_instance().New());
    // NAVI_ACTION_queryDefaultInfo
    protoMsg->set_actiontype(NAVI_ACTION_queryDefaultInfo);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryStateInfo
    protoMsg->set_actiontype(NAVI_ACTION_queryStateInfo);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryCityInfo
    protoMsg->set_actiontype(NAVI_ACTION_queryCityInfo);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryStreetInfo
    protoMsg->set_actiontype(NAVI_ACTION_queryStreetInfo);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryHouseNoInfo
    protoMsg->set_actiontype(NAVI_ACTION_queryHouseNoInfo);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryShowingPOIIconNumber
    protoMsg->set_actiontype(NAVI_ACTION_queryShowingPOIIconNumber);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryPOICollectionNearby
    protoMsg->set_actiontype(NAVI_ACTION_queryPOICollectionNearby);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryPOICollectionNearDestination
    protoMsg->set_actiontype(NAVI_ACTION_queryPOICollectionNearDestination);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryPOICollectionAlongRoute
    protoMsg->set_actiontype(NAVI_ACTION_queryPOICollectionAlongRoute);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryRouteExist
    protoMsg->set_actiontype(NAVI_ACTION_queryRouteExist);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryUnpassedDestinationList
    protoMsg->set_actiontype(NAVI_ACTION_queryUnpassedDestinationList);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryAllDestinationList
    protoMsg->set_actiontype(NAVI_ACTION_queryAllDestinationList);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryPointType
    protoMsg->set_actiontype(NAVI_ACTION_queryPointType);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryNextDestination
    protoMsg->set_actiontype(NAVI_ACTION_queryNextDestination);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryPointCollection
    protoMsg->set_actiontype(NAVI_ACTION_queryPointCollection);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryDestinationHistory
    protoMsg->set_actiontype(NAVI_ACTION_queryDestinationHistory);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryPointDataArea
    protoMsg->set_actiontype(NAVI_ACTION_queryPointDataArea);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_ShowPoiIcon
    protoMsg->set_actiontype(NAVI_ACTION_ShowPoiIcon);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_hidePOIIcon
    protoMsg->set_actiontype(NAVI_ACTION_hidePOIIcon);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_addToRoute
    protoMsg->set_actiontype(NAVI_ACTION_addToRoute);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_deleteDestination
    protoMsg->set_actiontype(NAVI_ACTION_deleteDestination);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_calculateRoute
    protoMsg->set_actiontype(NAVI_ACTION_calculateRoute);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryStateInfoCN
    protoMsg->set_actiontype(NAVI_ACTION_queryStateInfoCN);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryCityInfoCN
    protoMsg->set_actiontype(NAVI_ACTION_queryCityInfoCN);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryDistrictInfoCN
    protoMsg->set_actiontype(NAVI_ACTION_queryDistrictInfoCN);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryStreetInfoCN
    protoMsg->set_actiontype(NAVI_ACTION_queryStreetInfoCN);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryHouseNoInfoCN
    protoMsg->set_actiontype(NAVI_ACTION_queryHouseNoInfoCN);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryAddressInfoCN
    protoMsg->set_actiontype(NAVI_ACTION_queryAddressInfoCN);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // NAVI_ACTION_queryPOINameNearby
    protoMsg->set_actiontype(NAVI_ACTION_queryPOINameNearby);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
    // default
    protoMsg->set_actiontype((::navi::VoiceRecog::VrNaviActionType)100);
    m_VR_ActionFactory_VrUnitAgent->CreateAction(actionId, *protoMsg);
}

TEST_F(VR_ActionFactory_VrUnitAgent_Test, CreateAction_RequestVrVoiceTag)
{
    int actionId = 1;
    EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    m_VR_ActionFactory_VrUnitAgent->CreateAction(context, *protoMsg);
}

/* EOF */
