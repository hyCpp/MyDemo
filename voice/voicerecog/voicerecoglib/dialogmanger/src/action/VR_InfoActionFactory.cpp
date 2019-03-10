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
#include "VR_InfoActionFactory.h"

#include "VR_AudioPlayingAction.h"
#include "VR_AudioSourceAction.h"
// #include "VrAgent/VR_AudioInfoAction.h"
#include "VR_AudioPlayAndShow.h"
#include "VR_ActionTtsPlay.h"
#include "VR_ActionTtsStop.h"
#include "VR_ActionBeepPlay.h"
#include "VR_ActionBeepStop.h"
#include "VR_RadioActiveBandInfoAction.h"
#include "VR_RadioTuningAction.h"
#include "VR_RadioInfoAction.h"
// #include "VR_HVACFacilitySwitchAction.h"
// #include "VR_HVACTuneClimateAction.h"
// #include "VR_HVACTuneFanAction.h"
// #include "VR_HVACStatusInfoAction.h"
// #include "VR_HVACShowScreenAction.h"
// #include "VR_HVACValidTemperature.h"
// #include "VR_HVACValidFanSpeed.h"
// #include "VR_HVACCheckTemperature.h"
// #include "VR_HVACCheckFanSpeed.h"
#include "VR_VehicleMotion.h"
#include "VR_SmartAgentPrompt.h"
#include "VR_VoiceTagAction.h"
// #include "VR_HFDGetInfoAction.h"
#include "VR_HFDGetMsgBodyAction.h"
#include "VR_AppsStartAction.h"
// #include "VR_Common_VrUnitAgent.h"
#include "VR_EngineStatusAction.h"
#include "VR_MediaSourceStatus.h"
#include "VR_StartFinishNotifyAction.h"
#include "VR_ActionTtsListPlay.h"
#include "VR_ActionTtsListStop.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"

using namespace navi::VoiceRecog;

#ifndef VR_ACTION_CASE
#define VR_ACTION_CASE(MSG_TYPE, MSG_ID, MSG_CLASS, MSG_SENDER,  MSG ) \
    case(MSG_TYPE): \
    { \
        spAction.reset(VR_new MSG_CLASS(MSG_SENDER, MSG_ID, MSG_TYPE, MSG)); \
    } \
    break;
#endif

#ifndef VR_HAVC_CASE
#define VR_HVAC_CASE(MSG_TYPE, MSG_ID, MSG_CLASS, MSG_SENDER, MSG)  \
    case (MSG_TYPE) : \
    {  \
        spAction.reset(VR_new MSG_CLASS(MSG_ID, MSG_TYPE, MSG, MSG_SENDER));  \
    }   \
    break
#endif

VR_InfoActionFactory::VR_InfoActionFactory(spVR_ActionEventSender sender)
    : m_sender(sender)
//    , m_spAvcLanSender()
{
    VR_LOGD_FUNC();
}

VR_InfoActionFactory::~VR_InfoActionFactory()
{
    VR_LOGD_FUNC();
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(int actionId, const VrRecogResult &proto)
{
    int type = proto.action_id();
    int id = actionId;
    VR_LOGD("actionId=[%d],action type=[%d]", id, type);

    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
#ifdef HAVE_NUTSHELL_OS
    switch (type) {
   //  VR_ACTION_CASE(SwitchSource,    id, VR_AudioSourceAction,  m_sender, proto.switch_source());

 //   VR_ACTION_CASE(PlayAlbum,       id, VR_AudioPlayingAction, m_sender, proto.play_album());
//    VR_ACTION_CASE(PlayAll,         id, VR_AudioPlayingAction, m_sender, proto.play_all());
//    VR_ACTION_CASE(PlayArtist,      id, VR_AudioPlayingAction, m_sender, proto.play_artist());
//    VR_ACTION_CASE(PlayAudioBook,   id, VR_AudioPlayingAction, m_sender, proto.play_audio_book());
//    VR_ACTION_CASE(PlayComposer,    id, VR_AudioPlayingAction, m_sender, proto.play_composer());
//    VR_ACTION_CASE(PlayGenre,       id, VR_AudioPlayingAction, m_sender, proto.play_genre());
//    VR_ACTION_CASE(PlayPlaylist,    id, VR_AudioPlayingAction, m_sender, proto.play_list());
//    VR_ACTION_CASE(PlayPodcast,     id, VR_AudioPlayingAction, m_sender, proto.play_podcast());
//    VR_ACTION_CASE(PlayPodcastItem, id, VR_AudioPlayingAction, m_sender, proto.play_podcast_item());
//    VR_ACTION_CASE(PlaySong,        id, VR_AudioPlayingAction, m_sender, proto.play_song());
    // hvac action
//    VR_HVAC_CASE(AutoAirConditionerON,       id, VR_HVACFacilitySwitchAction, m_sender, proto.auto_air_conditioner_on());
//    VR_HVAC_CASE(AutoAirConditionerOFF,      id, VR_HVACFacilitySwitchAction, m_sender, proto.auto_air_conditioner_off());
//    VR_HVAC_CASE(ACON,                       id, VR_HVACFacilitySwitchAction, m_sender, proto.ac_on());
//    VR_HVAC_CASE(ACOFF,                      id, VR_HVACFacilitySwitchAction, m_sender, proto.ac_off());
//    VR_HVAC_CASE(DualModeON,                 id, VR_HVACFacilitySwitchAction, m_sender, proto.dual_mode_on());
//    VR_HVAC_CASE(DualModeOFF,                id, VR_HVACFacilitySwitchAction, m_sender, proto.dual_mode_off());
//    VR_HVAC_CASE(WiperDeicerON,              id, VR_HVACFacilitySwitchAction, m_sender, proto.wiper_deicer_on());
//    VR_HVAC_CASE(WiperDeicerOFF,             id, VR_HVACFacilitySwitchAction, m_sender, proto.wiper_deicer_off());
//    VR_HVAC_CASE(RearSeatAirConditionerON,   id, VR_HVACFacilitySwitchAction, m_sender, proto.rear_seat_air_conditioner_on());
//    VR_HVAC_CASE(RearSeatAirConditionerOFF,  id, VR_HVACFacilitySwitchAction, m_sender, proto.rear_seat_air_conditioner_off());
//    VR_HVAC_CASE(RearDefoggerON,             id, VR_HVACFacilitySwitchAction, m_sender, proto.rear_defogger_on());
//    VR_HVAC_CASE(RearDefoggerOFF,            id, VR_HVACFacilitySwitchAction, m_sender, proto.rear_defogger_off());
//    VR_HVAC_CASE(ClimateConciergeModeON,     id, VR_HVACFacilitySwitchAction, m_sender, proto.climate_concierge_mode_on());

//    VR_HVAC_CASE(ChangeTemperature,       id, VR_HVACTuneClimateAction, m_sender, proto.change_temperature());
//    VR_HVAC_CASE(DecreaseTemperature,     id, VR_HVACTuneClimateAction, m_sender, proto.decrease_temperature());
//    VR_HVAC_CASE(IncreaseTemperature,     id, VR_HVACTuneClimateAction, m_sender, proto.increase_temperature());
//    VR_HVAC_CASE(AFewWarmer,              id, VR_HVACTuneClimateAction, m_sender, proto.afew_warmer());
//    VR_HVAC_CASE(AFewCooler,              id, VR_HVACTuneClimateAction, m_sender, proto.afew_cooler());
//    VR_HVAC_CASE(TemperatureMax,          id, VR_HVACTuneClimateAction, m_sender, proto.temperature_max());
//    VR_HVAC_CASE(TemperatureMin,          id, VR_HVACTuneClimateAction, m_sender, proto.temperature_min());
//    VR_HVAC_CASE(ChangeFanSpeed,          id, VR_HVACTuneFanAction, m_sender, proto.change_fan_speed());
//    VR_HVAC_CASE(FanSpeedMax,             id, VR_HVACTuneFanAction, m_sender, proto.fan_speed_max());
//    VR_HVAC_CASE(FanSpeedMin,             id, VR_HVACTuneFanAction, m_sender, proto.fan_speed_min());
//    VR_HVAC_CASE(BlowerModeFace,          id, VR_HVACTuneFanAction, m_sender, proto.blower_mode_face());
//    VR_HVAC_CASE(BlowerModeFoot,          id, VR_HVACTuneFanAction, m_sender, proto.blower_mode_foot());
//    VR_HVAC_CASE(BlowerModeFaceAndFoot,   id, VR_HVACTuneFanAction, m_sender, proto.blower_mode_face_foot());
//    VR_HVAC_CASE(BlowerModeWindowAndFoot, id, VR_HVACTuneFanAction, m_sender, proto.blower_mode_window_foot());

//    VR_ACTION_CASE(HVACFrontScreen,             id, VR_HVACShowScreenAction, m_sender, proto.hvac_front_screen());
//    VR_ACTION_CASE(HVACRearScreen,              id, VR_HVACShowScreenAction, m_sender, proto.hvac_rear_screen());
//    VR_ACTION_CASE(HVACSteeringScreen,          id, VR_HVACShowScreenAction, m_sender, proto.hvac_steering_screen());
//    VR_ACTION_CASE(HVACFrontSeatScreen,         id, VR_HVACShowScreenAction, m_sender, proto.hvac_front_seat_screen());
//    VR_ACTION_CASE(HVACRearSeatScreen,          id, VR_HVACShowScreenAction, m_sender, proto.hvac_rear_seat_screen());
//    VR_ACTION_CASE(HVACLexusConciergeScreen,    id, VR_HVACShowScreenAction, m_sender, proto.hvac_lexus_concierge_screen());
//    VR_ACTION_CASE(HVACSeatOperationScreen,     id, VR_HVACShowScreenAction, m_sender, proto.hvac_seat_operation_screen());
//    VR_ACTION_CASE(FrontSeatVentilationScreen,  id, VR_HVACShowScreenAction, m_sender, proto.front_seat_ventilation_screen());
//    VR_ACTION_CASE(RearSeatVentilationScreen,   id, VR_HVACShowScreenAction, m_sender, proto.rear_seat_ventilation_screen());
    // Radio
    VR_ACTION_CASE(StartRadio,         id, VR_RadioTuningAction, m_sender, proto.start_radio());
    VR_ACTION_CASE(ChangeHDSubChannel,         id, VR_RadioTuningAction, m_sender, proto.change_hd_subchannel());
    VR_ACTION_CASE(TuneFrequency,              id, VR_RadioTuningAction, m_sender, proto.tune_frequency());
    VR_ACTION_CASE(TuneFMHDFrequency,          id, VR_RadioTuningAction, m_sender, proto.tune_fmhd_frequency());
    VR_ACTION_CASE(TuneFrequencyAM,            id, VR_RadioTuningAction, m_sender, proto.tune_frequency_am());
    VR_ACTION_CASE(TuneFrequencyFM,            id, VR_RadioTuningAction, m_sender, proto.tune_frequency_fm());
    VR_ACTION_CASE(TuneFrequencySatellite,     id, VR_RadioTuningAction, m_sender, proto.tune_frequency_satellite());
    VR_ACTION_CASE(TunePreset,                 id, VR_RadioTuningAction, m_sender, proto.tune_preset());
    VR_ACTION_CASE(TuneSatelliteChannelName,   id, VR_RadioTuningAction, m_sender, proto.tune_satellite_channel_name());
    VR_ACTION_CASE(TuneSatelliteChannelNumber, id, VR_RadioTuningAction, m_sender, proto.tune_satellite_channel_number());
    VR_ACTION_CASE(TuneDABChannelName,         id, VR_RadioTuningAction, m_sender, proto.tune_dab_channel_name());
    VR_ACTION_CASE(TuneFMChannelName,          id, VR_RadioTuningAction, m_sender, proto.tune_fm_channel_name());
    VR_ACTION_CASE(TuneFMGenre,                id, VR_RadioTuningAction, m_sender, proto.tune_fm_genre());
    VR_ACTION_CASE(TuneSatelliteGenre,         id, VR_RadioTuningAction, m_sender, proto.tune_satellite_genre());

    // APPS
    VR_ACTION_CASE(StartApps,                  id, VR_AppsStartAction,   m_sender, proto.start_apps());
    default:
        break;
    }
#endif
     return spAction;
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(int actionId, const VrAdditionalInfoReq &proto)
{
    int type = proto.request_id();
    int id   = actionId;
    VR_LOGD("actionId=[%d],action type=[%d]", id, type);
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
#ifdef HAVE_NUTSHELL_OS
    switch (type) {
#ifdef VR_DM_TARGET_VRAGENT
    VR_ACTION_CASE(USBDeviceInfo,        id, VR_AudioInfoAction, m_sender, proto.usb_device_info_req());
#endif

    // Radio
    VR_ACTION_CASE(RadioBandState,       id, VR_RadioInfoAction, m_sender, proto.radio_band_state_req());
    VR_ACTION_CASE(CurFMChannelState,    id, VR_RadioInfoAction, m_sender, proto.cur_fm_channel_state_req());
    VR_ACTION_CASE(SpecifiedFMState,     id, VR_RadioInfoAction, m_sender, proto.specified_fm_state_req());
    VR_ACTION_CASE(PresetState,          id, VR_RadioInfoAction, m_sender, proto.preset_state_req());
    VR_ACTION_CASE(SpecifiedPresetState, id, VR_RadioInfoAction, m_sender, proto.specified_preset_state_req());
    VR_ACTION_CASE(RadioState,           id, VR_RadioInfoAction, m_sender, proto.radio_state_req());
    VR_ACTION_CASE(GenreStationState,    id, VR_RadioInfoAction, m_sender, proto.genre_station_state_req());

    // hvac info
//    VR_HVAC_CASE(AirConditionState,      id, VR_HVACStatusInfoAction, m_sender, proto.air_condition_state_req());
//    VR_HVAC_CASE(ACState,                id, VR_HVACStatusInfoAction, m_sender, proto.ac_state_req());
//    VR_HVAC_CASE(Temperature,            id, VR_HVACStatusInfoAction, m_sender, proto.temperature_req());
//    VR_HVAC_CASE(TemperatureRange,       id, VR_HVACStatusInfoAction, m_sender, proto.temperature_range_req());
//    VR_HVAC_CASE(FanSpeed,               id, VR_HVACStatusInfoAction, m_sender, proto.fan_speed_state_req());
//    VR_HVAC_CASE(DualMode,               id, VR_HVACStatusInfoAction, m_sender, proto.dual_mode_req());
//    VR_HVAC_CASE(WiperDeicer,            id, VR_HVACStatusInfoAction, m_sender, proto.wiper_deicer_req());
//    VR_HVAC_CASE(RearDefogger,           id, VR_HVACStatusInfoAction, m_sender, proto.rear_defogger_state_req());
//    VR_HVAC_CASE(BlowerMode,             id, VR_HVACStatusInfoAction, m_sender, proto.blower_mode_req());
//    VR_HVAC_CASE(ClimateConciergeMode,   id, VR_HVACStatusInfoAction, m_sender, proto.climate_concierge_mode_req());
    default:
        break;
    }
#endif
    return spAction;
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(int actionId, const VrTTSCommandReq &proto)
{
    int type = proto.func_code();
    VR_LOGD("actionId=[%d],action type=[%d]", actionId, type);
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
#ifdef HAVE_NUTSHELL_OS
    switch (type) {
    VR_ACTION_CASE(TTSProcessType_Play, actionId, VR_ActionTtsPlay, m_sender, proto);
    VR_ACTION_CASE(TTSProcessType_Stop, actionId, VR_ActionTtsStop, m_sender, proto);

    default:
        break;
    }
#endif
    return spAction;
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(int actionId, const VrBeepCommandReq &proto)
{
    VR_LOGD("action type beep, actionId = [%d]", actionId);

    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
#ifdef HAVE_NUTSHELL_OS
    int type = proto.func_code();
    switch (type) {
    VR_ACTION_CASE(BeepProcessType_Play, actionId, VR_ActionBeepPlay, m_sender, proto);
    VR_ACTION_CASE(BeepProcessType_Stop, actionId, VR_ActionBeepStop, m_sender, proto);

    default:
        break;
    }
#endif
    return spAction;
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(int actionId, const VrActiveInfoReq &proto)
{
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
#ifdef HAVE_NUTSHELL_OS
    int type = proto.request_id();
    int id   = actionId;
    VR_LOGD("actionId=[%d],action type=[%d]", actionId, type);

    switch (type) {
    VR_ACTION_CASE(RadioActiveBand,       id, VR_RadioActiveBandInfoAction, m_sender, proto.radio_active_band_req());
//    VR_HVAC_CASE(ValidTemperature,        id, VR_HVACValidTemperature,      m_sender, proto.temperature_valid_req());
//    VR_HVAC_CASE(ValidFanSpeed,           id, VR_HVACValidFanSpeed,         m_sender, proto.fan_speed_valid_req());
//    VR_HVAC_CASE(CheckTemperature,        id, VR_HVACCheckTemperature,      m_sender, proto.temperature_check_req());
//    VR_HVAC_CASE(CheckFanSpeed,           id, VR_HVACCheckFanSpeed,         m_sender, proto.fan_speed_check_req());
//    VR_HVAC_CASE(MusicPlayString,         id, VR_AudioPlayAndShow,          m_sender, proto.music_specific_play_req());
    VR_ACTION_CASE(VehicleInMotion,       id, VR_VehicleMotion,             m_sender, proto.vehicle_in_motion());
    VR_ACTION_CASE(SmartAgentPromptLevel, id, VR_SmartAgentPrompt,          m_sender, proto.smart_prompt_level());
    // HFD
    // VR_ACTION_CASE(RecentCallList,          id, VR_HFDGetInfoAction,  m_sender, proto.recent_call_list_req());
    // VR_ACTION_CASE(RecentIncomingCall,      id, VR_HFDGetInfoAction,  m_sender, proto.incoming_call_list_req());
    // VR_ACTION_CASE(RecentOutgoingCall,      id, VR_HFDGetInfoAction,  m_sender, proto.outgoing_call_list_req());
    // VR_ACTION_CASE(RecentMessageList,       id, VR_HFDGetInfoAction,  m_sender, proto.recent_msg_list_req());
    // VR_ACTION_CASE(RecentMessageListByType, id, VR_HFDGetInfoAction,  m_sender, proto.recent_type_msg_list_req());
    // VR_ACTION_CASE(PhoneVehicleInMontion,   id, VR_HFDGetInfoAction,  m_sender, proto.phone_vehicle_in_motion());

    VR_ACTION_CASE(DEStatusResult,          id, VR_EngineStatusAction,      m_sender, proto.destatus_result());
    // VR_ACTION_CASE1(MediaSourceStatus,      id, VR_MediaSourceStatus,       m_sender, proto.vr_source_status_req(), m_spAvcLanSender);
    // VR_ACTION_CASE(StartFinishNotify,       id, VR_StartFinishNotifyAction, m_sender, proto.startfinish_notify());

    VR_ACTION_CASE(TuneFMStationName,       id, VR_RadioActiveBandInfoAction, m_sender, proto.tune_fm_station_name());
    VR_ACTION_CASE(TuneDABStationName,      id, VR_RadioActiveBandInfoAction, m_sender, proto.tune_dab_station_name());
    default:
        break;
    }
#endif
    return spAction;
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(const EV_EventContext &context, const RequestVrVoiceTag& proto)
{
    VR_LOGD("action type=[%d]", proto.funccode());
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
#ifdef HAVE_NUTSHELL_OS
    spAction.reset(VR_new VR_VoiceTagAction(m_sender, context, proto));
#endif
    return spAction;
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(int actionId, const VrMessageAcquisitionReq &proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(int actionId, const VrActionNavi &proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_InfoActionFactory::CreateAction(int actionId, const VrTtsListCommandReq &proto)
{
    int type = proto.func_code();
    VR_LOGD("actionId=[%d],action type=[%d]", actionId, type);
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
#ifdef HAVE_NUTSHELL_OS
    switch (type) {
    VR_ACTION_CASE(VrTtsListCommandReq_VrTtsListType_play, actionId, VR_ActionTtsListPlay, m_sender, proto);
    VR_ACTION_CASE(VrTtsListCommandReq_VrTtsListType_stop, actionId, VR_ActionTtsListStop, m_sender, proto);

    default:
        break;
    }
#endif
    return spAction;
}
/* EOF */
