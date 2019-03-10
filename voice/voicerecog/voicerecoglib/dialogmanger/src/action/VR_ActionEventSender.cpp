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
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

#ifndef VR_ACTIONEVENTSENDER_H
#   include "VR_ActionEventSender.h"
#endif

#include "VR_DialogEngineIF.h"
#include "VR_TransformProtoToXml.h"
#include "VR_EventSenderIF.h"

using namespace navi::VoiceRecog;

#ifndef VR_ACTION_CASE
#define VR_ACTION_CASE(MSG_TYPE, MSG_NAME, MSG_ACTION_PROTO) \
    case(MSG_TYPE): \
    { \
        MSG_NAME * actionProto = MSG_ACTION_PROTO; \
        result->set_action_id(MSG_TYPE); \
        actionProto->CheckTypeAndMergeFrom(msg); \
    } \
    break;
#endif

VR_ActionEventSender::VR_ActionEventSender(spVR_DialogEngineIF spEngine)
    : m_spEngineIF(spEngine)
    , m_spEventSenderIF(VR_EventSenderIF::GetSender())
{

    VR_LOGD_FUNC();
}

VR_ActionEventSender::~VR_ActionEventSender()
{
    VR_LOGD_FUNC();
}

void VR_ActionEventSender::sendActionRequest(int id, int actionType, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    VrActionDM2FC toSend;
    toSend.set_id(id);
    toSend.set_vr_dm_action_type(ActionType_RECOG_RESULT);
    VrRecogResult* result = toSend.mutable_recog_result();

    switch (actionType) {
    VR_ACTION_CASE(AudioOFF,        VrAudioOFF,         result->mutable_audio_off());
    VR_ACTION_CASE(AudioON,         VrAudioON,          result->mutable_audio_on());
    VR_ACTION_CASE(SwitchSource,    VrSwitchSource,     result->mutable_switch_source());

    VR_ACTION_CASE(PlayAlbum,       VrPlayAlbum,        result->mutable_play_album());
    VR_ACTION_CASE(PlayAll,         VrPlayAll,          result->mutable_play_all());
    VR_ACTION_CASE(PlayArtist,      VrPlayArtist,       result->mutable_play_artist());
    VR_ACTION_CASE(PlayAudioBook,   VrPlayAudioBook,    result->mutable_play_audio_book());
    VR_ACTION_CASE(PlayComposer,    VrPlayComposer,     result->mutable_play_composer());
    VR_ACTION_CASE(PlayGenre,       VrPlayGenre,        result->mutable_play_genre());
    VR_ACTION_CASE(PlayPlaylist,    VrPlayPlaylist,     result->mutable_play_list());
    VR_ACTION_CASE(PlayPodcast,     VrPlayPodcast,      result->mutable_play_podcast());
    VR_ACTION_CASE(PlayPodcastItem, VrPlayPodcastItem,  result->mutable_play_podcast_item());
    VR_ACTION_CASE(PlaySong,        VrPlaySong,         result->mutable_play_song());

    // Radio
    VR_ACTION_CASE(ChangeHDSubChannel,         VrChangeHDSubChannel,         result->mutable_change_hd_subchannel());
    VR_ACTION_CASE(TuneFrequency,              VrTuneFrequency,              result->mutable_tune_frequency());
    VR_ACTION_CASE(TuneFMHDFrequency,          VrTuneFMHDFrequency,          result->mutable_tune_fmhd_frequency());
    VR_ACTION_CASE(TuneFrequencyAM,            VrTuneFrequencyAM,            result->mutable_tune_frequency_am());
    VR_ACTION_CASE(TuneFrequencyFM,            VrTuneFrequencyFM,            result->mutable_tune_frequency_fm());
    VR_ACTION_CASE(TuneFrequencySatellite,     VrTuneFrequencySatellite,     result->mutable_tune_frequency_satellite());
    VR_ACTION_CASE(TunePreset,                 VrTunePreset,                 result->mutable_tune_preset());
    VR_ACTION_CASE(TuneSatelliteChannelName,   VrTuneSatelliteChannelName,   result->mutable_tune_satellite_channel_name());
    VR_ACTION_CASE(TuneSatelliteChannelNumber, VrTuneSatelliteChannelNumber, result->mutable_tune_satellite_channel_number());
    VR_ACTION_CASE(TuneDABChannelName,         VrTuneDABChannelName,         result->mutable_tune_dab_channel_name());
    VR_ACTION_CASE(TuneFMGenre,                VrTuneFMGenre,                result->mutable_tune_fm_genre());
    VR_ACTION_CASE(TuneSatelliteGenre,         VrTuneSatelliteGenre,         result->mutable_tune_satellite_genre());
    default:
        break;
     }
}

void VR_ActionEventSender::sendInfoRequest(int id, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrAdditionalInfoRsp &additionalInfoRsp = (const VrAdditionalInfoRsp &)msg;
    VR_String msgStr = VR_TransformProtoToXml().Transform(additionalInfoRsp);
    m_spEngineIF->SendMessage(msgStr, id);
}

void VR_ActionEventSender::sendSpecialInfoRequest(int id, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrActiveInfoRsp &activeInfoRsp = (const VrActiveInfoRsp &)msg;
    VR_String msgStr = VR_TransformProtoToXml().Transform(activeInfoRsp);
    m_spEngineIF->SendMessage(msgStr, id);
}

void VR_ActionEventSender::sendMessageToFC(const char *protoTag, VR_ProtoMessageBase *msg)
{
    VR_LOGD_FUNC();
    m_spEventSenderIF->SendEvent(protoTag, msg);
}

void VR_ActionEventSender::sendActionReply(int id, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrRecogResultConfirm &recogResultConfirm = (const VrRecogResultConfirm &)msg;
    VR_String msgStr = VR_TransformProtoToXml().Transform(recogResultConfirm);
    m_spEngineIF->SendMessage(msgStr, id);
}


void VR_ActionEventSender::sendEngineReply(int id, const VR_ProtoMessageBase &msg)
{
    VR_LOGD("id=[%d]", id);

    const VrActionDM2DE& replyMsg = (const VrActionDM2DE&)msg;
    VrDMActionType actionType = replyMsg.vr_dm_action_type();
    VrTTSProcessType processType = replyMsg.tts_result().func_code();

    if (ActionType_TTS_COMMAND_RESULT == actionType && TTSProcessType_Stop == processType) {
        const static VR_String msgStr  =   "<action-result agent=\"prompt\"  op=\"playTts\" errcode=\"0\">"
                                    "</action-result>";
        VR_LOGD("playTts xml=[%s]", msgStr.c_str());
        m_spEngineIF->SendMessage(msgStr, id);
    }
    else if (ActionType_BEEP_COMMAND_RESP == actionType && TTSProcessType_Stop == processType) {
        const static VR_String msgStr  =   "<action-result agent=\"prompt\"  op=\"playBeep\" errcode=\"0\">"
                                    "</action-result>";
        VR_LOGD("playBeep xml=[%s]", msgStr.c_str());
        m_spEngineIF->SendMessage(msgStr, id);
    }
    else if (ActionType_TTS_COMMAND_RESULT == actionType && TTSProcessType_Pause == processType) {
        const static VR_String msgStr  =   "<action-result agent=\"prompt\"  op=\"stopTts\" errcode=\"0\">"
                                    "</action-result>";
        VR_LOGD("stopTts xml=[%s]", msgStr.c_str());
        m_spEngineIF->SendMessage(msgStr, id);
    }
    else if (ActionType_BEEP_COMMAND_RESP == actionType && TTSProcessType_Pause == processType) {
        const static VR_String msgStr  =   "<action-result agent=\"prompt\"  op=\"stopBeep\" errcode=\"0\">"
                                    "</action-result>";
        VR_LOGD("stopBeep xml=[%s]", msgStr.c_str());
        m_spEngineIF->SendMessage(msgStr, id);
    }
    else {
        VR_LOGD("vr_dm_action_type=[%d], tts_result.func_code()=[%d]", actionType, processType);
    }
}

void VR_ActionEventSender::sendEngineReply(int id, const VR_String &msg)
{
    VR_LOGD_FUNC();
    VR_LOGD("id:[%d],  Msg: [%s]",  id,  msg.substr(0, 864).c_str());
    m_spEngineIF->SendMessage(msg, id);
}

spVR_DialogEngineIF VR_ActionEventSender::getDialogEngine()
{
    return m_spEngineIF;
}

/* EOF */
