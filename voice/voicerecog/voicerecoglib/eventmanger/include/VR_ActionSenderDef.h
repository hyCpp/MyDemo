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
/**
 * @file VR_ActionSenderDef.h
 * @brief Declaration file of class VR_ActionSenderDef.
 *
 * This file includes the declaration of class VR_EventSenderIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONSENDERDEF_H
#define VR_ACTIONSENDERDEF_H

#include "stdafx.h"
#include <boost/assign/list_of.hpp>
#include <memory>
#include "MEM_map.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

typedef std::pair<std::string, std::string>MsgType_Key;
typedef VoiceMap<MsgType_Key,   int32_t>::type MsgType_Map;
const int ACTION_ONLY_EUROPEAN_AND_DEC_TYPE  =  10000;
const int ACTION_TYPE_SHOW_VOICE_SETTINGS_UI =  10001;
const int ACTION_TYPE_SHOW_LEXUS_INSIDER_UI  =  10002;
const int ACTION_TYPE_SHOW_TUTORIALS_UI      =  10003;
const int ACTION_TYPE_SHOW_TRAINING_END_MSG  =  10004;
const int ACTION_TYPE_SHOW_CALLDESTINATION_UI = 10005;
const int ACTION_TYPE_GETMSG_NO_CHANGE_STATE =  10006;
const int ACTION_TYPE_GETMSG_DETIAL          =  10007;
const int ACTION_TYPE_PLAY_BY_FILTER           =  10008;
const int ACTION_TYPE_SHOW_MEDIA_SCREEN  =  10009;

const MsgType_Map  msgType_Map = boost::assign::map_list_of
        (std::make_pair("media", "changeSource"),                      int32_t(navi::VoiceRecog::SwitchSource))
        (std::make_pair("media", "turnAudioOff"),                      int32_t(navi::VoiceRecog::AudioOFF))
        (std::make_pair("media", "turnAudioOn"),                       int32_t(navi::VoiceRecog::AudioON))
        (std::make_pair("media", "playByGenreArtistAndAlbum"),         ACTION_ONLY_EUROPEAN_AND_DEC_TYPE)
        (std::make_pair("media", "playByGenreAndArtist"),              ACTION_ONLY_EUROPEAN_AND_DEC_TYPE)
        (std::make_pair("media", "playByArtistAndAlbum"),              ACTION_ONLY_EUROPEAN_AND_DEC_TYPE)
        (std::make_pair("media", "changeSourceByName"),                int32_t(navi::VoiceRecog::SwitchSource))
        (std::make_pair("phone", "dial"),                              int32_t(navi::VoiceRecog::Call))
        (std::make_pair("phone", "dial1"),                             int32_t(navi::VoiceRecog::CallWithNo))
        (std::make_pair("phone", "dial2"),                             int32_t(navi::VoiceRecog::CallRecent))
        (std::make_pair("phone", "dial3"),                             int32_t(navi::VoiceRecog::CallShortRing))
        (std::make_pair("phone", "redial"),                            int32_t(navi::VoiceRecog::Redial))
        (std::make_pair("phone", "muteCall"),                          int32_t(navi::VoiceRecog::MUTEON))
        (std::make_pair("phone", "muteOff"),                           int32_t(navi::VoiceRecog::MUTEOFF))
        (std::make_pair("phone", "sendMessage"),                       int32_t(navi::VoiceRecog::SendFixedMessage))
        (std::make_pair("phone", "sendDTMFTones"),                     int32_t(navi::VoiceRecog::SendDTMF))
        (std::make_pair("phone", "sendMessage"),                       int32_t(navi::VoiceRecog::SendMessage))
        (std::make_pair("phone", "showFavoriteContactList"),           int32_t(navi::VoiceRecog::PhoneListFavourites))
        (std::make_pair("phone", "showContactList"),                   int32_t(navi::VoiceRecog::ListAllContact))
        (std::make_pair("phone", "showAddVoiceTagUI"),                 int32_t(navi::VoiceRecog::AddVoiceTag))
        (std::make_pair("phone", "showRecentCallList"),                int32_t(navi::VoiceRecog::CallHistory))
        (std::make_pair("phone", "getMessageDetail"),                  ACTION_TYPE_GETMSG_DETIAL)
        (std::make_pair("phone", "getMsgDetailStatusNoChange"),        ACTION_TYPE_GETMSG_NO_CHANGE_STATE)
        (std::make_pair("help", "showVoiceSettingsUI"),                ACTION_TYPE_SHOW_VOICE_SETTINGS_UI)
        (std::make_pair("help", "showLexusInsiderUI"),                 ACTION_TYPE_SHOW_LEXUS_INSIDER_UI)
        (std::make_pair("help", "showTutorialsUI"),                    ACTION_TYPE_SHOW_TUTORIALS_UI)
        (std::make_pair("help", "showTrainingEndMsg"),                 ACTION_TYPE_SHOW_TRAINING_END_MSG)
        (std::make_pair("navi", "callDestinationAssist"),              ACTION_TYPE_SHOW_CALLDESTINATION_UI);

#endif // VR_ACTIONSENDERDEF_H
/* EOF */
