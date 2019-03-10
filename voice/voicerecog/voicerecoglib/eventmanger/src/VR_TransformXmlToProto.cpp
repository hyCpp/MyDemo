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
#include <stdlib.h>
#include <memory>
#include <sstream>
#include <boost/assign/list_of.hpp>
#include "pugixml.hpp"

#include "VR_TransformXmlToProto.h"
#include "VR_HAVCTypedefine.h"
#include "VR_DataProviderComm.h"
#include "VR_AudioSourceNameManager.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrMessageTrans.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"

using namespace navi::VoiceRecog;
// using namespace navi::dataprovider;

#ifndef VR_CREATE_DE2DM_PROTO
#define VR_CREATE_DE2DM_PROTO(PROTO_DE2DM_TYPE) \
    if (doc.empty()) { \
        return NULL; \
    } \
    std::unique_ptr<VrActionDE2DM> proto; \
    proto.reset(VrActionDE2DM::default_instance().New()); \
    proto->set_id(id); \
    proto->set_vr_dm_action_type(PROTO_DE2DM_TYPE);
#endif

#ifndef VR_CREATE_RECOGRESULT_PROTO
#define VR_CREATE_RECOGRESULT_PROTO(PROTO_RECOG_TYPE) \
    VrRecogResult *resultProto = proto->mutable_recog_result();\
    resultProto->set_action_id(PROTO_RECOG_TYPE);
#endif

#ifndef VR_CREATE_ADDITIONAL_PROTO
#define VR_CREATE_ADDITIONAL_PROTO(PROTO_ADDITIONAL_TYPE) \
    VrAdditionalInfoReq *AdditionalProto = proto->mutable_additional_info_req(); \
    AdditionalProto->set_request_id(PROTO_ADDITIONAL_TYPE);
#endif

#ifndef VR_ACTION_CASE
#define VR_ACTION_CASE(ACTION_TYPE, MUTABLE_ACTION_FUNC) \
    case ACTION_TYPE:  \
        {    \
            resultProto->set_action_id(ACTION_TYPE);    \
            resultProto->MUTABLE_ACTION_FUNC();   \
        }    \
        break
#endif

#ifndef VR_REQUEST_CASE
#define VR_REQUEST_CASE(REQUEST_TYPE, MUTABLE_REQUEST_FUNC) \
    case REQUEST_TYPE:  \
        {    \
            pAdditionalProto->set_request_id(REQUEST_TYPE);    \
            pAdditionalProto->MUTABLE_REQUEST_FUNC();   \
        }    \
        break
#endif

#ifndef VR_CREATE_ACTIVEINFOREQ_PROTO
#define VR_CREATE_ACTIVEINFOREQ_PROTO(PROTO_DMINFO_TYPE) \
    VrActiveInfoReq* activeInfoProto = proto->mutable_active_info_req(); \
    activeInfoProto->set_request_id(PROTO_DMINFO_TYPE);
#endif

const VR_TransformXmlToProto::TransformMap VR_TransformXmlToProto::m_TransformMap = boost::assign::map_list_of
// buttonPressed
        ("/event-result[@name='buttonPressed']/keycode",                        &VR_TransformXmlToProto::EventResultButtonPressedTransform)
// startAgent
        ("/event-result[@name='startAgent']",                                   &VR_TransformXmlToProto::EventResultStartAgentTransform)
// prepare
        ("/event-result[@name='prepare']",                                   &VR_TransformXmlToProto::EventResultPrepareTransform)
// cancel
        ("/event-result[@name='cancel']",                                       &VR_TransformXmlToProto::EventResultCancelTransform)
        ("/event-result[@name='longPressed']",                      &VR_TransformXmlToProto::CloseVoiceTagTtsSessionTransform)
        ("/event-result[@name='sessionOff']",                       &VR_TransformXmlToProto::CloseVoiceTagTtsSessionTransform)
// media
        ("/action[@agent='media' and @op='play']",                              &VR_TransformXmlToProto::MusicPlayTransform)
        ("/action[@agent='media' and @op='getCurrentAlbum']",                   &VR_TransformXmlToProto::MusicGetAlbumTransform)
        ("/action[@agent='media' and @op='getCurrentArtist']",                  &VR_TransformXmlToProto::MusicGetArtistTransform)
        ("/action[@agent='media' and @op='getCurrentComposer']",                &VR_TransformXmlToProto::MusicGetComposerTransform)
        ("/action[@agent='media' and @op='getCurrentGenre']",                   &VR_TransformXmlToProto::MusicGetGenreTransform)
        ("/action[@agent='media' and @op='querySourceStatus']",                 &VR_TransformXmlToProto::MusicQuerySourceStatus)

//      ("/action[@agent='media' and @op='changeSource']",                      &VR_TransformXmlToProto::MusicChanageTransform)
        ("/action[@agent='media' and @op='turnAudioOff']",                      &VR_TransformXmlToProto::MusicAudioOffTransform)
        ("/action[@agent='media' and @op='turnAudioOn']",                       &VR_TransformXmlToProto::MusicAudioOnTransform)

        ("/action[@agent='media' and @op='changeSourceByName']",                &VR_TransformXmlToProto::MusicChangeByNameTransform)
        ("/grammar_result[@agent='media' and @op='grammar']",                   &VR_TransformXmlToProto::MusicDicGrammarResultTransform)
        ("/grammar_result[@agent='media' and @op='active']",                    &VR_TransformXmlToProto::MusicDicActiveResultTransform)
        ("/grammar_result[@agent='media' and @op='disactive']",                 &VR_TransformXmlToProto::MusicDicInactiveResultTransform)
        ("/grammar_result[@agent='phone' and @op='grammar']",                   &VR_TransformXmlToProto::PhoneDicGrammarResultTransform)
        ("/grammar_result[@agent='phone' and @op='active']",                    &VR_TransformXmlToProto::PhoneDicActiveResultTransform)
        ("/grammar_result[@agent='phone' and @op='disactive']",                 &VR_TransformXmlToProto::PhoneDicInactiveResultTransform)
        ("/action[@agent='media' and @op='requestGrammar']",                    &VR_TransformXmlToProto::MusicDicGrammarRequestTransform)
        ("/action[@agent='phone' and @op='requestGrammar']",                    &VR_TransformXmlToProto::PhoneDicGrammarRequestTransform)
        ("/action[@agent='tuner' and @op='requestGrammar']",                    &VR_TransformXmlToProto::TunerDicGrammarRequestTransform)

// radio
        ("/action[@agent='media' and @op='startRadio']",                        &VR_TransformXmlToProto::RadioStartRadioTransform)
        ("/action[@agent='media' and @op='changeHDSubChannel']",                &VR_TransformXmlToProto::RadioChangeHDSubChannelTransform)
        ("/action[@agent='media' and @op='tuneFrequency']",                     &VR_TransformXmlToProto::RadioTuneFrequencyTransform)
        ("/action[@agent='media' and @op='tuneFMHDFrequency']",                 &VR_TransformXmlToProto::RadioTuneFMHDFrequencyTransform)
        ("/action[@agent='media' and @op='tuneLastSatellite']",                 &VR_TransformXmlToProto::RadioTuneLastSatelliteTransform)
        ("/action[@agent='media' and @op='tuneSatelliteChannelNumber']",        &VR_TransformXmlToProto::RadioTuneSatelliteChannelNumberTransform)
        ("/action[@agent='media' and @op='tuneLastAM']",                        &VR_TransformXmlToProto::RadioTuneLastAMTransform)
        ("/action[@agent='media' and @op='tuneLastFM']",                        &VR_TransformXmlToProto::RadioTuneLastFMTransform)
        ("/action[@agent='media' and @op='playByGenre']",                       &VR_TransformXmlToProto::RadioPlayByGenreTransform)
        ("/action[@agent='media' and @op='playByName']",                        &VR_TransformXmlToProto::RadioPlayByNameTransform)
        ("/action[@agent='media' and @op='playByPreset']",                      &VR_TransformXmlToProto::RadioPlayByPresetTransform)
        ("/action[@agent='media' and @op='queryActiveFrequencyBand']",          &VR_TransformXmlToProto::RadioQueryActiveFrequencyBandTransform)
        ("/action[@agent='media' and @op='queryFreqBandStatus']",               &VR_TransformXmlToProto::RadioQueryFreqBandStatusTransform)
        ("/action[@agent='media' and @op='queryRadioStatus']",                  &VR_TransformXmlToProto::RadioQueryRadioStatusTransform)
        ("/action[@agent='media' and @op='queryHDSubchannelAvailable']",        &VR_TransformXmlToProto::RadioQueryHDSubchannelAvailableTransform)
        ("/action[@agent='media' and @op='queryCurrentHDSubchannelAvailable']", &VR_TransformXmlToProto::RadioQueryCurrentHDSubchannelAvailableTransform)
        ("/action[@agent='media' and @op='queryPresetDefined']",                &VR_TransformXmlToProto::RadioQueryPresetDefinedTransform)
        ("/action[@agent='media' and @op='queryPresetsAvailable']",             &VR_TransformXmlToProto::RadioQueryPresetsAvailableTransform)
        ("/action[@agent='media' and @op='queryFMGenreAvailable']",             &VR_TransformXmlToProto::RadioQueryFMGenreAvailableTransform)
// apps
        ("/action[@agent='apps' and @op='launchApp']",                          &VR_TransformXmlToProto::LaunchAppTransform)
// tts
        ("/action[@agent='prompt' and @op='playTts']",                          &VR_TransformXmlToProto::PlayTtsTransform)
        ("/action[@agent='prompt' and @op='stopTts']",                          &VR_TransformXmlToProto::StopTtsTransform)
// beep
        ("/action[@agent='prompt' and @op='playBeep']",                         &VR_TransformXmlToProto::PlayBeepTransform)
        ("/action[@agent='prompt' and @op='stopBeep']",                         &VR_TransformXmlToProto::StopBeepTransform)
// tts list
        ("/action[@agent='prompt' and @op='playTtsList']",                      &VR_TransformXmlToProto::PlayTtsListTransform)
        ("/action[@agent='prompt' and @op='stopTtsList']",                      &VR_TransformXmlToProto::StopTtsListTransform)
// hvac addition info
        ("/action[@agent='climate' and @op='queryStatus']", &VR_TransformXmlToProto::QueryStatusTransform)
        ("/action[@agent='climate' and @op='queryTemperature']", &VR_TransformXmlToProto::TemperatureTransform)
        ("/action[@agent='climate' and @op='queryTemperatureRange']", &VR_TransformXmlToProto::TemperatureRangeTransform)
        ("/action[@agent='climate' and @op='queryFanSpeed']", &VR_TransformXmlToProto::FanSpeedTransform)
        ("/action[@agent='climate' and @op='getFanMode']", &VR_TransformXmlToProto::BlowerModeTransfrom)
// hvac action
        ("/action[@agent='climate' and @op='turn']", &VR_TransformXmlToProto::TurnClimateTransform)
        ("/action[@agent='climate' and @op='controlTemperature']", &VR_TransformXmlToProto::ControlTempTransform)
        ("/action[@agent='climate' and @op='changeTemperature']", &VR_TransformXmlToProto::ChangeTemperatureTransform)
        ("/action[@agent='climate' and @op='changeFanSpeed']", &VR_TransformXmlToProto::ChangeFanSpeedTransform)
        ("/action[@agent='climate' and @op='changeFanMode']", &VR_TransformXmlToProto::ChangeFanModeTransform)
        ("/action[@agent='climate' and @op='showScreen']", &VR_TransformXmlToProto::ShowScreenTransform)
// hvac vbt(add)
        ("/action[@agent='climate' and @op='getValidTemperature']", &VR_TransformXmlToProto::GetValidTemperature)
        ("/action[@agent='climate' and @op='checkTemperature']", &VR_TransformXmlToProto::CheckCurrTemperature)
        ("/action[@agent='climate' and @op='getValidFanSpeed']", &VR_TransformXmlToProto::GetValidFanSpeed)
        ("/action[@agent='climate' and @op='checkFanSpeed']", &VR_TransformXmlToProto::CheckCurrFanSpeed)
// voiceTag
        ("/event-result[@name='addRecordVoiceTag']",    &VR_TransformXmlToProto::EventResultAddVoiceTagTransform)
        ("/event-result[@name='editRecordVoiceTag']",   &VR_TransformXmlToProto::EventResultEditVoiceTagTransform)
        ("/event-result[@name='saveVoiceTag']",         &VR_TransformXmlToProto::EventResultSaveVoiceTagTransform)
        ("/event-result[@name='deleteVoiceTag']",       &VR_TransformXmlToProto::EventResultDeleteVoiceTagTransform)
        ("/event-result[@name='playVoiceTag']",         &VR_TransformXmlToProto::EventResultPlayVoiceTagTransform)
        ("/event-result[@name='stopVoiceTag']",         &VR_TransformXmlToProto::EventResultStopVoiceTagTransform)
        ("/event-result[@name='cancelRecordVoiceTag']", &VR_TransformXmlToProto::EventResultCancelRecoedVoiceTagTransform)
        ("/event-result[@name='cancelSaveVoiceTag']",   &VR_TransformXmlToProto::EventResultCancelSaveVoiceTagTransform)
        ("/event-result[@name='syncVoiceTag']",         &VR_TransformXmlToProto::EventResultSyncVoiceTagTransform)
// tsl
        ("/event-result[@name='install']",  &VR_TransformXmlToProto::EventResultTslTransform)
        ("/event-result[@name='uninstall']", &VR_TransformXmlToProto::EventResultTslTransform)
        ("/event-result[@name='getSupportedLanguage']", &VR_TransformXmlToProto::EventResultTslTransform)
        ("/event-result[@name='CancelAppRecognition']", &VR_TransformXmlToProto::EventResultTslTransform)
        ("/event-result[@name='StartAppRecognition']", &VR_TransformXmlToProto::EventResultTslTransform)

// tsl action(notify)
        ("/action[@name='notifyInstallProgress']", &VR_TransformXmlToProto::ActionNotifyTslTransform)
        ("/action[@name='notifyAppXmlMessage']", &VR_TransformXmlToProto::ActionNotifyTslTransform)
        ("/action[@name='notifyAppRecognitionForceEnd']", &VR_TransformXmlToProto::ActionNotifyTslTransform)
        ("/action[@name='notifyAppRecognitionEnd']", &VR_TransformXmlToProto::ActionNotifyTslTransform)
        ("/action[@name='notifyTSLVRInfo']", &VR_TransformXmlToProto::ActionNotifyTslTransform)

// close session action(notify)
        ("/action[@name='closeSession']", &VR_TransformXmlToProto::CloseSessionTransform)

// uistartCondition
        ("/event-result[@name='getResourceState']",   &VR_TransformXmlToProto::EventResultStartServiceConditonTransform)
// changelanguage
        ("event-result[@name='changeLanguage']",      &VR_TransformXmlToProto::EventResultChangeLanguageTransform)
// smart agent
        ("event-result[@name='smartagent']",          &VR_TransformXmlToProto::EventResultSmartAgentTransform)
// initial personal data
        ("event-result[@name='initialpersondata']",   &VR_TransformXmlToProto::EventResultInitialPersonalDataTransform)
// phone
        ("/action[@agent='phone' and @op='dial']",                     &VR_TransformXmlToProto::PhoneDialTransform)
        ("/action[@agent='phone' and @op='sendDTMFTones']",            &VR_TransformXmlToProto::PhoneSendDTMFToneTransform)
        ("/action[@agent='phone' and @op='showContactList']",          &VR_TransformXmlToProto::PhoneListAllContactTransform)
        ("/action[@agent='phone' and @op='showFavoriteContactList']",  &VR_TransformXmlToProto::PhoneListFavourContactTransform)
        ("/action[@agent='phone' and @op='sendMessage']",              &VR_TransformXmlToProto::PhoneSendMessageTransform)
        ("/action[@agent='phone' and @op='muteCall']",                 &VR_TransformXmlToProto::PhoneMuteOnTransform)
        ("/action[@agent='phone' and @op='muteOff']",                  &VR_TransformXmlToProto::PhoneMuteOffTransform)
        ("/action[@agent='phone' and @op='addVoiceTag']",              &VR_TransformXmlToProto::PhoneAddVoiceTagTransform)
        ("/action[@agent='phone' and @op='showRecentCallList']",       &VR_TransformXmlToProto::PhoneShowRecentCallTransform)
        ("/action[@agent='phone' and @op='getMessageDetail']",         &VR_TransformXmlToProto::PhoneGetMessageDetailTransform)
        ("/action[@agent='phone' and @op='queryIncomingCall']",        &VR_TransformXmlToProto::PhoneGetInComingCallListTransform)
        ("/action[@agent='phone' and @op='queryOutgoingCall']",        &VR_TransformXmlToProto::PhoneGetOutGoingCallListTransform)
        ("/action[@agent='phone' and @op='queryRecentCallList']",      &VR_TransformXmlToProto::PhoneGetRecentCallListTransform)
        ("/action[@agent='phone' and @op='queryRecentMessage']",       &VR_TransformXmlToProto::PhoneGetRecentMsgListTransform)
        ("/action[@agent='phone' and @op='queryRecentMessageByType']", &VR_TransformXmlToProto::PhoneGetMsgListByTypeTransform)
        ("/action[@agent='phone' and @op='queryVehicleinMotion']",     &VR_TransformXmlToProto::PhoneVehicleMontionTransform)
// vehicle motion
        ("/action[@agent='help'  and @op='queryVehicleinMotion']",     &VR_TransformXmlToProto::QueryVehicleMontionTransform)
// smart agent prompt level
        ("/action[@agent='smartagent' and @op='setPromptLevel']",      &VR_TransformXmlToProto::SmartAgentPromptLevelTransform)
        ("/action[@agent='navi' and @op='queryDestinationAssistAvailable']",      &VR_TransformXmlToProto::GBookCheckStatusTransform)
// gbook communication error code
        ("/action[@agent='communication' and @op='queryGBookStatus']",      &VR_TransformXmlToProto::GBookCheckErrorTransform)
// de status
        ("/action[@agent='destatus' and @op='notifyResourceState']",        &VR_TransformXmlToProto::RecourceStateTransform)
// start finish notify
        ("/action[@agent='destatus' and @op='notifyStartFinish']",          &VR_TransformXmlToProto::StartFinishNotifyTransform)
// gbook center url
        ("/action[@agent='communication' and @op='queryCenterVRURL']",      &VR_TransformXmlToProto::GBookCenterUrlTransform)
        // amazon
        ("/action[@agent='system' and @op='awake']",                        &VR_TransformXmlToProto::AwakenTransform)
        ("/action[@agent='system' and @op='quit']",                         &VR_TransformXmlToProto::QuitTransform)
        ("/action[@agent='system' and @op='awakewithoutstart']",            &VR_TransformXmlToProto::AwakenWithOutStartTransform);

const VR_TransformXmlToProto::StrTransformMap VR_TransformXmlToProto::m_PlayAndShowMap = boost::assign::map_list_of
        ("/action[@agent='media' and @op='playByFilter']",                      &VR_TransformXmlToProto::MusicPlayAndShowTransform)
        ("/action[@agent='media' and @op='showMediaScreen']",                   &VR_TransformXmlToProto::MusicPlayAndShowTransform)
        ("/action[@agent='navi' and @op='requestDefaultInfo']",                 &VR_TransformXmlToProto::NaviReqDfltInfoTransform)
        ("/action[@agent='navi' and @op='queryStateInfo']",                     &VR_TransformXmlToProto::NaviQueryStateInfoTransform)
        ("/action[@agent='navi' and @op='queryCityInfo']",                      &VR_TransformXmlToProto::NaviQueryCityInfoTransform)
        ("/action[@agent='navi' and @op='queryDistrictInfo']",                  &VR_TransformXmlToProto::NaviQueryDistrictInfoTransform)
        ("/action[@agent='navi' and @op='queryStreetInfo']",                    &VR_TransformXmlToProto::NaviQueryStreetInfoTransform)
        ("/action[@agent='navi' and @op='queryAddressInfo']",                   &VR_TransformXmlToProto::NaviQueryAddressInfoTransform)
        ("/action[@agent='navi' and @op='queryOneShotInfo']",                   &VR_TransformXmlToProto::NaviQueryOneShotInfoTransform)
        ("/action[@agent='navi' and @op='queryShowingPOIIconNumber']",          &VR_TransformXmlToProto::NaviQueryShowPOIIconNoTransform)
        ("/action[@agent='navi' and @op='queryPOICollectionNearby']",           &VR_TransformXmlToProto::NaviQueryPOINearbyTransform)
        ("/action[@agent='navi' and @op='queryPOICollectionNearDestination']",  &VR_TransformXmlToProto::NaviQueryPOINearDestinationTransform)
        ("/action[@agent='navi' and @op='queryPOICollectionAlongRoute']",       &VR_TransformXmlToProto::NaviQueryPOIAlongRouteTransform)
        ("/action[@agent='navi' and @op='queryPointCollection']",               &VR_TransformXmlToProto::NaviQueryPointCollectionTransform)
        ("/action[@agent='navi' and @op='queryDestinationHistory']",            &VR_TransformXmlToProto::NaviQueryDestHistoryTransform)
        ("/action[@agent='navi' and @op='queryRouteExist']",                    &VR_TransformXmlToProto::NaviQueryRouteExistTransform)
        ("/action[@agent='navi' and @op='queryAllDestinationList']",            &VR_TransformXmlToProto::NaviQueryAllDestListTransform)
        ("/action[@agent='navi' and @op='queryUnpassedDestinationList']",       &VR_TransformXmlToProto::NaviQueryUnpassedDestListTransform)
        ("/action[@agent='navi' and @op='queryPointType']",                     &VR_TransformXmlToProto::NaviQueryPointTypeTransform)
        ("/action[@agent='navi' and @op='queryNextDestination']",               &VR_TransformXmlToProto::NaviQueryNextDestTransform)
        ("/action[@agent='navi' and @op='queryPointDataArea']",                 &VR_TransformXmlToProto::NaviQueryPointDateAreaTransform)
        ("/action[@agent='navi' and @op='showPOIIcon']",                        &VR_TransformXmlToProto::NaviShowPOIIconTransform)
        ("/action[@agent='navi' and @op='hidePOIIcon']",                        &VR_TransformXmlToProto::NaviHidePOIIconTransform)
        ("/action[@agent='navi' and @op='addToRoute']",                         &VR_TransformXmlToProto::NaviAddToRouteTransform)
        ("/action[@agent='navi' and @op='deleteDestination']",                  &VR_TransformXmlToProto::NaviDelDestinationTransform)
        ("/action[@agent='navi' and @op='calculateRoute']",                     &VR_TransformXmlToProto::NaviCalcRouteTransform)
        ("/action[@agent='navi' and @op='queryCurrentPosition']",               &VR_TransformXmlToProto::NaviQueryCurrentPosTransform)
        ("/action[@agent='navi' and @op='queryPOIByName']",                     &VR_TransformXmlToProto::NaviQueryPOIByNameTransform)
        ("/action[@agent='system' and @op='DirveScore']",                       &VR_TransformXmlToProto::DirveScoreTransform);

// only for hfd
const VoiceMap<VR_INT32, VR_INT32>::type VrHfdPhoneTypeTrans = boost::assign::map_list_of
        (VR_HFD_PHONE_TYPE_DEFAULT, VR_AVC_PHONE_TYPE_HIDE_ICON)
        (VR_HFD_PHONE_TYPE_MOBILE,  VR_AVC_PHONE_TYPE_MOBILE_ICON_1)
        (VR_HFD_PHONE_TYPE_HOME,    VR_AVC_PHONE_TYPE_HOME_ICON_1)
        (VR_HFD_PHONE_TYPE_WORK,    VR_AVC_PHONE_TYPE_OFFICE_ICON_1)
        (VR_HFD_PHONE_TYPE_OTHER,   VR_AVC_PHONE_TYPE_UNKNOWN_ICON_1)
        (VR_HFD_PHONE_TYPE_MOBILE1, VR_AVC_PHONE_TYPE_MOBILE_ICON_1)
        (VR_HFD_PHONE_TYPE_HOME1,   VR_AVC_PHONE_TYPE_HOME_ICON_1)
        (VR_HFD_PHONE_TYPE_WORK1,   VR_AVC_PHONE_TYPE_OFFICE_ICON_1)
        (VR_HFD_PHONE_TYPE_OTHER1,  VR_AVC_PHONE_TYPE_UNKNOWN_ICON_1)
        (VR_HFD_PHONE_TYPE_MOBILE2, VR_AVC_PHONE_TYPE_MOBILE_ICON_2)
        (VR_HFD_PHONE_TYPE_HOME2,   VR_AVC_PHONE_TYPE_HOME_ICON_2)
        (VR_HFD_PHONE_TYPE_WORK2,   VR_AVC_PHONE_TYPE_OFFICE_ICON_2)
        (VR_HFD_PHONE_TYPE_OTHER2,  VR_AVC_PHONE_TYPE_UNKNOWN_ICON_2)
        (VR_HFD_PHONE_TYPE_MOBILE3, VR_AVC_PHONE_TYPE_MOBILE_ICON_3)
        (VR_HFD_PHONE_TYPE_HOME3,   VR_AVC_PHONE_TYPE_HOME_ICON_3)
        (VR_HFD_PHONE_TYPE_WORK3,   VR_AVC_PHONE_TYPE_OFFICE_ICON_3)
        (VR_HFD_PHONE_TYPE_OTHER3,  VR_AVC_PHONE_TYPE_UNKNOWN_ICON_3)
        (VR_HFD_PHONE_TYPE_MOBILE4, VR_AVC_PHONE_TYPE_MOBILE_ICON_4)
        (VR_HFD_PHONE_TYPE_HOME4,   VR_AVC_PHONE_TYPE_HOME_ICON_4)
        (VR_HFD_PHONE_TYPE_WORK4,   VR_AVC_PHONE_TYPE_OFFICE_ICON_4)
        (VR_HFD_PHONE_TYPE_OTHER4,  VR_AVC_PHONE_TYPE_UNKNOWN_ICON_4);

VR_TransformXmlToProto::VR_TransformXmlToProto()
{
    VR_LOGD_FUNC();
}

VR_TransformXmlToProto::~VR_TransformXmlToProto()
{
    VR_LOGD_FUNC();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::Transform(int id, const std::string& msg) const
{
    VR_LOGI("id=[%d],msg=[%s]", id, msg.c_str());
    std::unique_ptr<VR_ProtoMessageBase> proto;
    pugi::xml_document doc;

    if (doc.load(msg.c_str())) {
        strTransformMapIter strCitr;
        cTransformMapIter citr;
        for (strCitr = m_PlayAndShowMap.begin(); strCitr != m_PlayAndShowMap.end(); ++strCitr) {
            pugi::xpath_node node;
            node = doc.select_node(strCitr->first.c_str());
            if (node) {
                proto.reset((this->*(strCitr->second))(id, msg));
                return proto.release();
            }
        }
        for (citr = m_TransformMap.begin(); citr != m_TransformMap.end(); ++citr) {
            pugi::xpath_node node;
            node = doc.select_node(citr->first.c_str());
            if (node) {
                proto.reset((this->*(citr->second))(id, doc));
                return proto.release();
            }
        }
    }
    else {
        VR_LOGI("Transform Msg Failed [%s]", msg.c_str());
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicPlayTransform(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id=[%d]", id);
    static const VoiceMap<std::string, VrActionID>::type PlayMap = boost::assign::map_list_of
            ("PLAY_ALBUM",       PlayAlbum)
            ("PLAY_ALL",         PlayAll)
            ("PLAY_ARTIST",      PlayArtist)
            ("PLAY_AUDIO_BOOK",   PlayAudioBook)
            ("PLAY_COMPOSER",    PlayComposer)
            ("PLAY_GENRE",      PlayGenre)
            ("PLAY_PLAYLIST",    PlayPlaylist)
            ("PLAY_PODCAST",     PlayPodcast)
            ("PLAY_PODCAST_ITEM", PlayPodcastItem)
            ("PLAY_SONG",        PlaySong);

    std::string strSourceId = doc.select_node("//sourceid").node().child_value();
    std::string Majorid = doc.select_node("//id").node().child_value();
    std::string subId = doc.select_node("//subid").node().child_value();
    std::string shuffle = doc.select_node("//shuffle").node().child_value();
    int shuffleOn = (shuffle == "false") ? 0 : 1;
    std::string playStrType = doc.select_node("//playType").node().child_value();
    std::string strName = doc.select_node("//name").node().child_value();
    VR_LOGI("name=[%s]", strName.c_str());
    VR_INT32 nSourceId = atoi(strSourceId.c_str());
    VR_INT32 nSourceType = GetSourceType(nSourceId);

    VoiceMap<std::string, VrActionID>::type::const_iterator cPlayMap;
    cPlayMap = PlayMap.find(playStrType);
    if (cPlayMap == PlayMap.end()) {
        VR_LOGE("Search END! and donot find");
        return NULL;
    }
    VrActionID playEnumType = cPlayMap->second;

    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    VrRecogResult *resultProto = proto->mutable_recog_result();

    switch (playEnumType) {
    case PlayAlbum :
        {
            resultProto->set_action_id(PlayAlbum);
            VrPlayAlbum * playAlbum = resultProto->mutable_play_album();

            playAlbum->set_source_id(nSourceId);
            playAlbum->set_shuffle_on(shuffleOn);
            playAlbum->set_album_id(atoi(Majorid.c_str()));
            playAlbum->set_source_type(nSourceType);
            playAlbum->set_album_name(strName);
            playAlbum->set_artist_name("");
        }
        break;
    case PlayAll :
        {
            resultProto->set_action_id(PlayAll);
            VrPlayAll * playAll = resultProto->mutable_play_all();

            playAll->set_source_id(nSourceId);
            playAll->set_shuffle_on(shuffleOn);
            playAll->set_source_type(nSourceType);
        }
        break;
    case PlayArtist :
        {
            resultProto->set_action_id(PlayArtist);
            VrPlayArtist * playArtist = resultProto->mutable_play_artist();

            playArtist->set_source_id(nSourceId);
            playArtist->set_shuffle_on(shuffleOn);
            playArtist->set_artist_id(atoi(Majorid.c_str()));

            playArtist->set_source_type(nSourceType);
            playArtist->set_artist_name(strName);
        }
        break;
    case PlayAudioBook :
        {
            resultProto->set_action_id(PlayAudioBook);
            VrPlayAudioBook * playAudioBook = resultProto->mutable_play_audio_book();

            playAudioBook->set_source_id(nSourceId);
            playAudioBook->set_audio_book_id(atoi(Majorid.c_str()));

            playAudioBook->set_source_type(nSourceType);
            playAudioBook->set_audio_book_name("");
        }
        break;
    case PlayComposer :
        {
            resultProto->set_action_id(PlayComposer);
            VrPlayComposer * playComposer = resultProto->mutable_play_composer();

            playComposer->set_source_id(nSourceId);
            playComposer->set_shuffle_on(shuffleOn);
            playComposer->set_composer_id(atoi(Majorid.c_str()));

            playComposer->set_source_type(nSourceType);
            playComposer->set_composer_name("");
        }
        break;
    case PlayGenre :
        {
            resultProto->set_action_id(PlayGenre);
            VrPlayGenre* playGenre = resultProto->mutable_play_genre();

            playGenre->set_source_id(nSourceId);
            playGenre->set_shuffle_on(shuffleOn);
            playGenre->set_genre_id(atoi(Majorid.c_str()));

            playGenre->set_source_type(nSourceType);
            playGenre->set_genre_name("");
        }
        break;
    case PlayPlaylist :
        {
            resultProto->set_action_id(PlayPlaylist);
            VrPlayPlaylist * playPlaylist = resultProto->mutable_play_list();

            playPlaylist->set_source_id(nSourceId);
            playPlaylist->set_shuffle_on(shuffleOn);
            playPlaylist->set_playlist_id(atoi(Majorid.c_str()));

            playPlaylist->set_source_type(nSourceType);
            playPlaylist->set_playlist_name(strName);
        }
        break;
    case PlayPodcast :
        {
            resultProto->set_action_id(PlayPodcast);
            VrPlayPodcast * playPodcast = resultProto->mutable_play_podcast();

            playPodcast->set_source_id(nSourceId);
            playPodcast->set_podcast_id(atoi(Majorid.c_str()));

            playPodcast->set_source_type(nSourceType);
            playPodcast->set_podcast_name("");
        }
        break;
    case PlayPodcastItem :
        {
            resultProto->set_action_id(PlayPodcastItem);
            VrPlayPodcastItem * playPodcastItem = resultProto->mutable_play_podcast_item();

            playPodcastItem->set_source_type(nSourceType);
            playPodcastItem->set_source_id(nSourceId);
            playPodcastItem->set_podcast_name("");
            playPodcastItem->set_podcast_id(atoi(Majorid.c_str()));
            playPodcastItem->set_episode_name("");
            playPodcastItem->set_episode_id(atoi(subId.c_str()));
        }
        break;
    case PlaySong :
        {
            resultProto->set_action_id(PlaySong);
            VrPlaySong * playSong = resultProto->mutable_play_song();

            playSong->set_source_id(nSourceId);
            playSong->set_song_id(atoll(Majorid.c_str()));

            playSong->set_source_type(nSourceType);
            playSong->set_song_name(strName);


        }
        break;
    default:
        break;
    }
return proto.release();
}
/*
VR_ProtoMessageBase *VR_TransformXmlToProto::MusicChanageTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(SwitchSource)

    std::string sourceId = doc.select_node("//sourceid").node().child_value();
    VrSwitchSource *switchSourceProto = resultProto->mutable_switch_source();
    switchSourceProto->set_source_name(sourceId);
    return proto.release();
}
*/
VR_ProtoMessageBase *VR_TransformXmlToProto::MusicAudioOffTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(AudioOFF)

    resultProto->mutable_audio_off();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicAudioOnTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(AudioON)

    resultProto->mutable_audio_on();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicChangeByNameTransform(int id, pugi::xml_document &doc)const
{
    VR_LOGD_FUNC();
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(SwitchSource);
//    std::string sourceName = doc.select_node("//sourceName").node().child_value();
//    VR_LOGI("\nMusicSouceName:[%s]", sourceName.c_str());
//    VrSwitchSource *switchSourceProto = resultProto->mutable_switch_source();
//    switchSourceProto->set_source_name(sourceName.c_str());
//    VR_INT32 nModeId = VR_AudioSourceNameManager::FindModeId(sourceName.c_str());
//    VR_LOGI("nModeId=[%d]", nModeId);
//    switchSourceProto->set_audio_mode(nModeId);
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::MusicDicGrammarResultTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    INT grammarId = doc.child("grammar_result").attribute("grammarid").as_int();
    INT errorCode = doc.child("grammar_result").attribute("errcode").as_int();
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_RESULT);
    VrDPDictionaryResult* dpDicResultProto = proto->mutable_dp_dictionary_result();
    dpDicResultProto->set_type(DP_MUSIC_DICTIONARY_RESULT);
    VrDPMusicDictionaryResult* dpMusicDicResultProto = dpDicResultProto->mutable_music();
    dpMusicDicResultProto->set_result_type(DP_GRAMMAR_RESULT);
    dpMusicDicResultProto->set_grammar_id(grammarId);
    dpMusicDicResultProto->set_error_code(errorCode);
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::MusicDicActiveResultTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    INT grammarId = doc.child("grammar_result").attribute("grammarid").as_int();
    INT errorCode = doc.child("grammar_result").attribute("errcode").as_int();
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_RESULT);
    VrDPDictionaryResult* dpDicResultProto = proto->mutable_dp_dictionary_result();
    dpDicResultProto->set_type(DP_MUSIC_DICTIONARY_RESULT);
    VrDPMusicDictionaryResult* dpMusicDicResultProto = dpDicResultProto->mutable_music();
    dpMusicDicResultProto->set_result_type(DP_ACTIVE_RESULT);
    dpMusicDicResultProto->set_grammar_id(grammarId);
    dpMusicDicResultProto->set_error_code(errorCode);
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::MusicDicInactiveResultTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    INT grammarId = doc.child("grammar_result").attribute("grammarid").as_int();
    INT errorCode = doc.child("grammar_result").attribute("errcode").as_int();
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_RESULT);
    VrDPDictionaryResult* dpDicResultProto = proto->mutable_dp_dictionary_result();
    dpDicResultProto->set_type(DP_MUSIC_DICTIONARY_RESULT);
    VrDPMusicDictionaryResult* dpMusicDicResultProto = dpDicResultProto->mutable_music();
    dpMusicDicResultProto->set_result_type(DP_INACTIVE_RESULT);
    dpMusicDicResultProto->set_grammar_id(grammarId);
    dpMusicDicResultProto->set_error_code(errorCode);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneDicGrammarResultTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    INT errorCode = doc.child("grammar_result").attribute("errcode").as_int();
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_RESULT);
    VrDPDictionaryResult* dpDicResultProto = proto->mutable_dp_dictionary_result();
    dpDicResultProto->set_type(DP_PHONE_DICTIONARY_RESULT);
    VrDPPhoneDictionaryResult* dpPhoneDicResultProto = dpDicResultProto->mutable_phone();
    dpPhoneDicResultProto->set_result_type(DP_GRAMMAR_RESULT);
    dpPhoneDicResultProto->set_error_code(errorCode);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneDicActiveResultTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    INT errorCode = doc.child("grammar_result").attribute("errcode").as_int();
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_RESULT);
    VrDPDictionaryResult* dpDicResultProto = proto->mutable_dp_dictionary_result();
    dpDicResultProto->set_type(DP_PHONE_DICTIONARY_RESULT);
    VrDPPhoneDictionaryResult* dpPhoneDicResultProto = dpDicResultProto->mutable_phone();
    dpPhoneDicResultProto->set_result_type(DP_ACTIVE_RESULT);
    dpPhoneDicResultProto->set_error_code(errorCode);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneDicInactiveResultTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    INT errorCode = doc.child("grammar_result").attribute("errcode").as_int();
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_RESULT);
    VrDPDictionaryResult* dpDicResultProto = proto->mutable_dp_dictionary_result();
    dpDicResultProto->set_type(DP_PHONE_DICTIONARY_RESULT);
    VrDPPhoneDictionaryResult* dpPhoneDicResultProto = dpDicResultProto->mutable_phone();
    dpPhoneDicResultProto->set_result_type(DP_INACTIVE_RESULT);
    dpPhoneDicResultProto->set_error_code(errorCode);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicDicGrammarRequestTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    INT grammarId = doc.child("action").attribute("grammarID").as_int();
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_REQUEST);
    VrDPDictionaryRequest* dpDicResultProto = proto->mutable_dp_dictionary_request();
    dpDicResultProto->set_type(DP_MUSIC_DICTIONARY_RESULT);
    VrDPMusicDictionaryRequest* dpMusicDicResultProto = dpDicResultProto->mutable_music();
    dpMusicDicResultProto->set_grammar_id(grammarId);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneDicGrammarRequestTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_REQUEST);
    VrDPDictionaryRequest* dpDicResultProto = proto->mutable_dp_dictionary_request();
    dpDicResultProto->set_type(DP_PHONE_DICTIONARY_RESULT);
    dpDicResultProto->mutable_phone();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::TunerDicGrammarRequestTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_DP_DICTIONARY_REQUEST);
    VrDPDictionaryRequest* dpDicResultProto = proto->mutable_dp_dictionary_request();
    dpDicResultProto->set_type(DP_TUNER_DICTIONARY_RESULT);
    dpDicResultProto->mutable_tuner();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicPlayAndShowTransform(int id, const std::string &msg) const
{
    VR_LOGI("id=[%d]", id);
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        VR_CREATE_DE2DM_PROTO(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq* resultProto = proto->mutable_active_info_req();
        resultProto->set_request_id(MusicPlayString);
        *(resultProto->mutable_music_specific_play_req()) = msg;
        return proto.release();
    }
    else {
        return NULL;
    }
}

VR_INT32 VR_TransformXmlToProto::GetSourceType(int sourceid) const
{
      return 0;
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicGetAlbumTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);
    VR_CREATE_ADDITIONAL_PROTO(USBDeviceInfo);

    VrUSBDeviceInfoReq *UsbDeviceInfo = AdditionalProto->mutable_usb_device_info_req();
    UsbDeviceInfo->set_request_id(0);
    std::string sourceid = doc.select_node("//sourceid").node().child_value();
    VR_INT32 sourceType = GetSourceType(atoi(sourceid.c_str()));
    if (-1 == sourceType) {
        return NULL;
    }
    UsbDeviceInfo->set_source_type(sourceType);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicGetArtistTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);
    VR_CREATE_ADDITIONAL_PROTO(USBDeviceInfo);

    VrUSBDeviceInfoReq *UsbDeviceInfo = AdditionalProto->mutable_usb_device_info_req();
    UsbDeviceInfo->set_request_id(1);
    std::string sourceid = doc.select_node("//sourceid").node().child_value();
    VR_INT32 sourceType = GetSourceType(atoi(sourceid.c_str()));
    if (-1 == sourceType) {
        return NULL;
    }
    UsbDeviceInfo->set_source_type(sourceType);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicGetComposerTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);
    VR_CREATE_ADDITIONAL_PROTO(USBDeviceInfo);

    VrUSBDeviceInfoReq *UsbDeviceInfo = AdditionalProto->mutable_usb_device_info_req();
    UsbDeviceInfo->set_request_id(2);
    std::string sourceid = doc.select_node("//sourceid").node().child_value();
    VR_INT32 sourceType = GetSourceType(atoi(sourceid.c_str()));
    if (-1 == sourceType) {
        return NULL;
    }
    UsbDeviceInfo->set_source_type(sourceType);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicGetGenreTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);
    VR_CREATE_ADDITIONAL_PROTO(USBDeviceInfo);

    VrUSBDeviceInfoReq *UsbDeviceInfo = AdditionalProto->mutable_usb_device_info_req();
    UsbDeviceInfo->set_request_id(3);
    std::string sourceid = doc.select_node("//sourceid").node().child_value();
    VR_INT32 sourceType = GetSourceType(atoi(sourceid.c_str()));
    if (-1 == sourceType) {
        return NULL;
    }
    UsbDeviceInfo->set_source_type(sourceType);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::MusicQuerySourceStatus(int id, pugi::xml_document& doc) const
{    
    VR_LOGI("id=[%d]", id);
    int iSrcId = 0;

    VR_CREATE_DE2DM_PROTO(ActionType_ACTIVE_INFO_REQ);
    VR_CREATE_ACTIVEINFOREQ_PROTO(MediaSourceStatus);
    VrMediaSourceStatusReq* pcSrcStatusReq =  activeInfoProto->mutable_vr_source_status_req();
    if (NULL == pcSrcStatusReq) {
        return NULL;
    }
    iSrcId = atoi(doc.select_node("//sourceid").node().child_value());
    VR_LOGD("iSrcId = [%d]", iSrcId);
    pcSrcStatusReq->set_src_id(iSrcId);
    return proto.release();
}

VR_TransformXmlToProto::VR_RadioBand VR_TransformXmlToProto::ChangeBandToInt(std::string band) const
{
    VR_LOGI("band=[%s]", band.c_str());
    if (!band.compare("AM") || !band.compare("AM Radio")) {
        return VR_RadioBand_AM;
    }
    else if (!band.compare("FM") || !band.compare("FM Radio")) {
        return VR_RadioBand_FM;
    }
    else if (!band.compare("XM")) {
        return VR_RadioBand_XM;
    }
    else if (!band.compare("DAB")) {
        return VR_RadioBand_DAB;
    }
    else {
        return VR_RadioBand_Undef;
    }
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioStartRadioTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGD_FUNC();
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(StartRadio);
    std::string sourceName = doc.select_node("//sourceName").node().child_value();
    VR_LOGI("\nRadioSourceName:[%s]", sourceName.c_str());
    VrStartRadio *pStartRadio = resultProto->mutable_start_radio();
    pStartRadio->set_radio_band_kind(ChangeBandToInt(sourceName));
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioChangeHDSubChannelTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    std::string subchannelNum = doc.select_node("/action[@op='changeHDSubChannel']/subchannelNum").node().child_value();

    VR_CREATE_RECOGRESULT_PROTO(ChangeHDSubChannel);

    VrChangeHDSubChannel *changeHDSubChannel = resultProto->mutable_change_hd_subchannel();
    changeHDSubChannel->set_fm_sps_number(atoi(subchannelNum.c_str()));
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioTuneFrequencyTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    std::string band = doc.select_node("/action[@op='tuneFrequency']/band").node().child_value();
    std::string frequency = doc.select_node("/action[@op='tuneFrequency']/frequency").node().child_value();

    VR_CREATE_RECOGRESULT_PROTO(TuneFrequency);

    VrTuneFrequency * tuneFrequency = resultProto->mutable_tune_frequency();
    int nBand = ChangeBandToInt(band);
    tuneFrequency->set_radio_band_kind(nBand);
    if (VR_RadioBand_AM == nBand) {
        VR_INT32 nAMFrequence_Hz = atoi(frequency.c_str()) * 1000; // trans from KHz to Hz
        tuneFrequency->set_frequency(nAMFrequence_Hz);
    }
    else if (VR_RadioBand_FM == nBand) {
        VR_INT32 nFMFrequency_Hz = atof(frequency.c_str()) * 1000 * 1000; // trans from MHz to Hz
        tuneFrequency->set_frequency(nFMFrequency_Hz);
    }
    else {
        tuneFrequency->set_frequency(-1);
        VR_LOGI("error band=[%s], frequency=[-1]", band.c_str());
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioTuneFMHDFrequencyTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    std::string frequency = doc.select_node("/action[@op='tuneFMHDFrequency']/frequency").node().child_value();
    std::string subchannelNum = doc.select_node("/action[@op='tuneFMHDFrequency']/subchannelNum").node().child_value();

    VR_CREATE_RECOGRESULT_PROTO(TuneFMHDFrequency);

    VrTuneFMHDFrequency * tuneFMHDFrequency = resultProto->mutable_tune_fmhd_frequency();
    VR_INT32 nFMFrequency_Hz = atof(frequency.c_str()) * 1000 * 1000; // trans from MHz to Hz
    tuneFMHDFrequency->set_frequency(nFMFrequency_Hz);
    tuneFMHDFrequency->set_fm_sps_number(atoi(subchannelNum.c_str()));
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioTuneLastSatelliteTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    VR_CREATE_RECOGRESULT_PROTO(TuneFrequencySatellite);

    resultProto->mutable_tune_frequency_satellite();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioTuneSatelliteChannelNumberTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    std::string subchannelNum = doc.select_node("/action[@op='tuneSatelliteChannelNumber']/subchannelNum").node().child_value();

    VR_CREATE_RECOGRESULT_PROTO(TuneSatelliteChannelNumber);

    VrTuneSatelliteChannelNumber * tuneSatelliteChannelNumber = resultProto->mutable_tune_satellite_channel_number();
    tuneSatelliteChannelNumber->set_ch_number(atoi(subchannelNum.c_str()));
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioTuneLastAMTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    VR_CREATE_RECOGRESULT_PROTO(TuneFrequencyAM);

    resultProto->mutable_tune_frequency_am();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioTuneLastFMTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    VR_CREATE_RECOGRESULT_PROTO(TuneFrequencyFM);

    resultProto->mutable_tune_frequency_fm();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioPlayByGenreTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    std::string band = doc.select_node("/action[@op='playByGenre']/band").node().child_value();
    std::string genreID = doc.select_node("/action[@op='playByGenre']/id").node().child_value();
    std::string genreName = doc.select_node("/action[@op='playByGenre']/genre").node().child_value();

    if (0 == band.compare("XM")) {
        VR_CREATE_RECOGRESULT_PROTO(TuneSatelliteGenre);
        VrTuneSatelliteGenre *tuneSatelliteGenreProto = resultProto->mutable_tune_satellite_genre();
        tuneSatelliteGenreProto->set_genre_name(genreName);
    }
    else if (0 == band.compare("FM")) {
        VR_CREATE_RECOGRESULT_PROTO(TuneFMGenre);
        VrTuneFMGenre *tuneFMGenreProto = resultProto->mutable_tune_fm_genre();
        tuneFMGenreProto->set_genre_id(atoi(genreID.c_str()));
    }
    else {
        VR_LOGI("error band=[%s]", band.c_str());
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioPlayByNameTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    std::string band = doc.select_node("/action[@op='playByName']/band").node().child_value();
    std::string name = doc.select_node("/action[@op='playByName']/name").node().child_value();
    std::string strId = doc.select_node("/action[@op='playByName']/id").node().child_value();

    if (0 == band.compare("DAB")) {
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq*  vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->set_request_id(TuneDABStationName);
        vrActiveInfo->mutable_tune_dab_station_name()->set_list_no(atoi(strId.c_str()));
        vrActiveInfo->mutable_tune_dab_station_name()->set_station_name(name);
    }
    else if (0 == band.compare("XM")) {
        VR_CREATE_RECOGRESULT_PROTO(TuneSatelliteChannelName);
        VrTuneSatelliteChannelName *tuneSatelliteChannelName = resultProto->mutable_tune_satellite_channel_name();
        tuneSatelliteChannelName->set_station_name(name);
    }
    else if (0 == band.compare("FM")) {
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq*  vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->set_request_id(TuneFMStationName);
        vrActiveInfo->mutable_tune_fm_station_name()->set_list_no(atoi(strId.c_str()));
        vrActiveInfo->mutable_tune_fm_station_name()->set_station_name(name);
    }
    else {
        VR_LOGI("error band=[%s]", band.c_str());
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioPlayByPresetTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    std::string presetNumber = doc.select_node("/action[@op='playByPreset']/number").node().child_value();
    std::string presetBand = doc.select_node("/action[@op='playByPreset']/band").node().child_value();

    VR_CREATE_RECOGRESULT_PROTO(TunePreset);
    VrTunePreset *tunePreset = resultProto->mutable_tune_preset();
    tunePreset->set_preset_number(atoi(presetNumber.c_str()));

    VR_INT32 nBand = ChangeBandToInt(presetBand);
    tunePreset->set_radio_band_kind(nBand);

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioQueryActiveFrequencyBandTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        proto->mutable_active_info_req()->set_request_id(RadioActiveBand);
        proto->mutable_active_info_req()->mutable_radio_active_band_req();
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioQueryRadioStatusTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);

    VR_CREATE_ADDITIONAL_PROTO(RadioState);
    AdditionalProto->mutable_radio_state_req();

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioQueryFreqBandStatusTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);

    std::string frequencyband = doc.select_node("/action[@op='queryFreqBandStatus']/band").node().child_value();
    VR_LOGI("band=[%s]", frequencyband.c_str());
    VR_CREATE_ADDITIONAL_PROTO(RadioBandState);
    VrRadioBandStateReq *radioBandState = AdditionalProto->mutable_radio_band_state_req();
    int nBand = ChangeBandToInt(frequencyband);
    radioBandState->set_radio_band_kind(nBand);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioQueryHDSubchannelAvailableTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);

    std::string hdsubchannel = doc.select_node("/action[@op='queryHDSubchannelAvailable']/hdsubchannel").node().child_value();
    std::string frequency = doc.select_node("/action[@op='queryHDSubchannelAvailable']/frequency").node().child_value();

    VR_CREATE_ADDITIONAL_PROTO(SpecifiedFMState);
    VrSpecifiedFMStateReq *specifiedFMState = AdditionalProto->mutable_specified_fm_state_req();
    VR_INT32 nFMFrequency_Hz = atof(frequency.c_str()) * 1000 * 1000; // trans from MHz to Hz
    specifiedFMState->set_frequency(nFMFrequency_Hz);
    specifiedFMState->set_multicast_channel(atoi(hdsubchannel.c_str()));
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioQueryCurrentHDSubchannelAvailableTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);

    std::string hdsubchannel = doc.select_node("/action[@op='queryCurrentHDSubchannelAvailable']/hdsubchannel").node().child_value();

    VR_CREATE_ADDITIONAL_PROTO(CurFMChannelState);
    VrCurFMChannelStateReq *curFMChannelState = AdditionalProto->mutable_cur_fm_channel_state_req();
    curFMChannelState->set_multicast_channel(atoi(hdsubchannel.c_str()));
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioQueryPresetsAvailableTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);
    VR_CREATE_ADDITIONAL_PROTO(PresetState);

    AdditionalProto->mutable_preset_state_req();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioQueryPresetDefinedTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);
    VR_CREATE_ADDITIONAL_PROTO(SpecifiedPresetState);

    std::string band = doc.select_node("/action[@op='queryPresetDefined']/band").node().child_value();
    std::string number = doc.select_node("/action[@op='queryPresetDefined']/number").node().child_value();

    VrSpecifiedPresetStateReq *specifiedPresetState = AdditionalProto->mutable_specified_preset_state_req();
    specifiedPresetState->set_preset_num(atoi(number.c_str()));

    VR_INT32 nBand = ChangeBandToInt(band);
    specifiedPresetState->set_radio_band_kind(nBand);

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::RadioQueryFMGenreAvailableTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_ADDITIONAL_INFO_REQ);
    VR_CREATE_ADDITIONAL_PROTO(GenreStationState);

    std::string sId = doc.select_node("/action[@op='queryFMGenreAvailable']/id").node().child_value();
    std::string genre = doc.select_node("/action[@op='queryFMGenreAvailable']/genre").node().child_value();

    VrGenreStationStateReq *genreStationState = AdditionalProto->mutable_genre_station_state_req();
    genreStationState->set_genre_id(atoi(sId.c_str()));
    genreStationState->set_genre_name(genre);
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::LaunchAppTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    std::string appData = "";
    std::string appId = doc.select_node("/action[@op='launchApp']/appid").node().child_value();
    std::string appDataSize = doc.select_node("/action[@op='launchApp']/dataSize").node().child_value();
    if ("0" != appDataSize) {
        pugi::xml_node appNode  = doc.select_node("//data/*").node();
        std::ostringstream tmp;
        appNode.print(tmp);
        appData = tmp.str();
    }
    VR_LOGI("appId=[%s],appDataSize=[%s], appData=[%s]", appId.c_str(), appDataSize.c_str(), appData.c_str());

    VR_CREATE_RECOGRESULT_PROTO(StartApps);

    VrStartApps * startApps = resultProto->mutable_start_apps();
    startApps->set_application_id(atoi(appId.c_str()));
    VR_INT32 nLen = atoi(appDataSize.c_str());
    startApps->set_data_size(nLen);
    if (0 != nLen) {
        startApps->set_data_content(appData);
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultButtonPressedTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VrEngineControlMessage::default_instance().New());
        proto->set_type(DMTaskMessageType_RepOperCommand);
        const std::string event_type =
                doc.select_node("/event-result[@name='buttonPressed']/keycode/@value").attribute().value();
        proto->mutable_reply_command()->set_type(event_type);
        proto->mutable_reply_command()->set_arg1(0);
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultStartAgentTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        const VR_INT32 ResultOK = 0;
        proto.reset(VrEngineControlMessage::default_instance().New());
        proto->set_type(DMTaskMessageType_StartEnd);
        INT errorCode = doc.child("startAgent").attribute("errcode").as_int();
        bool bRet = (ResultOK == errorCode) ? true : false;
        proto->mutable_start_end()->set_result(bRet);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultPrepareTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VrEngineControlMessage::default_instance().New());
        proto->set_type(DMTaskMessageType_prepare);
        INT errorCode = doc.child("prepare").attribute("errcode").as_int();
        proto->mutable_prepareresult()->set_errcode(errorCode);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultCancelTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VrEngineControlMessage::default_instance().New());
        proto->set_type(DMTaskMessageType_CancelEnd);
        pugi::xml_node msgNode = doc.select_node("//event-result").node();
        if (NULL == msgNode) {
            proto->mutable_cancel_end()->set_cancel_option(VrCancelOptionType_Default);
        }
        else {
            std::string name  = msgNode.attribute("option").name();
            VR_LOGI("name=[%s]", name.c_str());
            std::string value = msgNode.attribute("option").value();
            VR_LOGI("value=[%s]", value.c_str());
            if (("option" == name) && ("usbSwitch" == value)) {
                proto->mutable_cancel_end()->set_cancel_option(VrCancelOptionType_UsbSwitch);
            }
            else if (("option" == name) && ("diagCopy" == value)) {
                proto->mutable_cancel_end()->set_cancel_option(VrCancelOptionType_DiagCopy);
            }
            else if (("option" == name) && ("diagDelete" == value)) {
                proto->mutable_cancel_end()->set_cancel_option(VrCancelOptionType_DiagDelete);
            }
            else if (("option" == name) && ("allQuit" == value)) {
                proto->mutable_cancel_end()->set_cancel_option(VrCancelOptionType_CloseSession);
            }
            else {
                proto->mutable_cancel_end()->set_cancel_option(VrCancelOptionType_Default);
            }
        }
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PlayTtsTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_TTS_COMMAND_REQ);
        proto->mutable_tts_req()->set_func_code(TTSProcessType_Play);
        proto->mutable_tts_req()->set_data_type(TSDataType_Normal);
        pugi::xpath_node_set textNodes = doc.select_nodes("/action[@op='playTts']/text");
        for (pugi::xpath_node_set::iterator it = textNodes.begin(); it != textNodes.end(); ++it) {
            proto->mutable_tts_req()->add_tts_texts(it->node().child_value());
        }
        proto->mutable_tts_req()->set_tts_language(doc.select_node("/action[@op='playTts']/language").node().child_value());
        std::string str = doc.select_node("/action[@op='playTts']/bargein").node().child_value();
        bool isbargein = (str == "true") ? true: false;
        proto->mutable_tts_req()->set_isbargein(isbargein);
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::StopTtsTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_TTS_COMMAND_REQ);
        proto->mutable_tts_req()->set_func_code(TTSProcessType_Stop);

        proto->mutable_tts_req()->set_data_type(TSDataType_Normal);
        proto->mutable_tts_req()->set_tts_text("");
        proto->mutable_tts_req()->set_tts_language("en-us");
        proto->mutable_tts_req()->set_isbargein(true);
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PlayBeepTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_BEEP_COMMAND_REQ);
        proto->mutable_beep_req()->set_func_code(BeepProcessType_Play);
        proto->mutable_beep_req()->set_beep_file_path(doc.select_node("/action[@op='playBeep']/beepFile").node().child_value());
        std::string str = doc.select_node("/action[@op='playBeep']/bargein").node().child_value();

        proto->mutable_beep_req()->set_isbargein(str == "true"?true:false);
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::StopBeepTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_BEEP_COMMAND_REQ);
        proto->mutable_beep_req()->set_func_code(BeepProcessType_Stop);
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PlayTtsListTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_TTS_LIST_REQ);
        proto->mutable_tts_list_req()->set_func_code(VrTtsListCommandReq_VrTtsListType_play);
        proto->mutable_tts_list_req()->set_language(doc.select_node("/action[@op='playTtsList']/language").node().child_value());
        pugi::xml_node firstNode = doc.root().first_child().first_child();
        while (firstNode) {
            VR_String strName = firstNode.name();
            if ("beepFile" == strName) {
                VrTtsListPhrase* vrTtsListPhrase = proto->mutable_tts_list_req()->add_tts_phrase();
                vrTtsListPhrase->set_type(VrTtsListPhrase_VrPhraseType_soundName);
                vrTtsListPhrase->set_beep_path(firstNode.child_value());
            }
            else if ("text" == strName) {
                VrTtsListPhrase* vrTtsListPhrase = proto->mutable_tts_list_req()->add_tts_phrase();
                vrTtsListPhrase->set_type(VrTtsListPhrase_VrPhraseType_text);
                vrTtsListPhrase->set_tts_text(firstNode.child_value());
            }
            firstNode = firstNode.next_sibling();
        }
    }

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::StopTtsListTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_TTS_LIST_REQ);
        proto->mutable_tts_list_req()->set_func_code(VrTtsListCommandReq_VrTtsListType_stop);
    }

    return proto.release();
}

// for hvac begin showscreen
VR_ProtoMessageBase*  VR_TransformXmlToProto::ShowScreenTransform(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id=[%d]", id);
    static const VoiceMap<std::string, VrActionID>::type showProtoMap = boost::assign::map_list_of
            ("CONCIERGE", HVACLexusConciergeScreen)
            ("FRONT", HVACFrontScreen)
            ("FRONT_SEAT", HVACFrontSeatScreen)
            ("FRONT_SEAT_VENTILATION", FrontSeatVentilationScreen)
            ("REAR", HVACRearScreen)
            ("REAR_SEAT", HVACRearSeatScreen)
            ("REAR_SEAT_VENTILATION", RearSeatVentilationScreen)
            ("SEAT_OPERATION", HVACSeatOperationScreen)
            ("STEERING", HVACSteeringScreen);

    if (doc.empty()) {
        return NULL;
    }
    std::string strShowType = doc.select_node("//type").node().child_value();
    VoiceMap<std::string, VrActionID>::type::const_iterator citer;
    citer = showProtoMap.find(strShowType);
    if (citer == showProtoMap.end()) {
        return NULL;
    }
    VrActionID actionType = citer->second;
    std::unique_ptr<VrActionDE2DM> proto;
    proto.reset(VR_new VrActionDE2DM());
    proto->set_id(id);
    proto->set_vr_dm_action_type(ActionType_RECOG_RESULT);
    VrRecogResult* resultProto = proto->mutable_recog_result();
    switch (actionType) {
    VR_ACTION_CASE(HVACLexusConciergeScreen, mutable_hvac_lexus_concierge_screen);
    VR_ACTION_CASE(HVACFrontScreen, mutable_hvac_front_screen);
    VR_ACTION_CASE(HVACFrontSeatScreen, mutable_hvac_front_seat_screen);
    VR_ACTION_CASE(FrontSeatVentilationScreen, mutable_front_seat_ventilation_screen);
    VR_ACTION_CASE(HVACRearScreen, mutable_hvac_rear_screen);
    VR_ACTION_CASE(HVACRearSeatScreen, mutable_hvac_rear_seat_screen);
    VR_ACTION_CASE(RearSeatVentilationScreen, mutable_rear_seat_ventilation_screen);
    VR_ACTION_CASE(HVACSeatOperationScreen, mutable_hvac_seat_operation_screen);
    VR_ACTION_CASE(HVACSteeringScreen, mutable_hvac_steering_screen);
    default: break;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::QueryStatusTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    if (doc.empty()) {
        return NULL;
    }

    static const VoiceMap<std::string, VrRequestID>::type StateProtoMap = boost::assign::map_list_of
            ("AUTO_AIRCONDITION", AirConditionState)
            ("AC", ACState)
            ("DUAL_MODE", DualMode)
            ("WIPER_DEICER", WiperDeicer)
            ("REAR_DEFOGGER", RearDefogger)
            ("CONCIERGE_MODE", ClimateConciergeMode);
    std::string strQueryType = doc.select_node("//type").node().child_value();
    VoiceMap<std::string, VrRequestID>::type::const_iterator citer;
    citer = StateProtoMap.find(strQueryType);
    if (citer == StateProtoMap.end()) {
        return NULL;
    }

    VrRequestID  requestType = citer->second;
    std::unique_ptr<VrActionDE2DM> proto;
    proto.reset(VR_new VrActionDE2DM());
    proto->set_id(id);
    proto->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_REQ);
    VrAdditionalInfoReq* pAdditionalProto = proto->mutable_additional_info_req();
    switch (requestType) {
    VR_REQUEST_CASE(AirConditionState, mutable_air_condition_state_req);
    VR_REQUEST_CASE(ACState, mutable_ac_state_req);
    VR_REQUEST_CASE(DualMode, mutable_dual_mode_req);
    VR_REQUEST_CASE(WiperDeicer, mutable_wiper_deicer_req);
    VR_REQUEST_CASE(RearDefogger, mutable_rear_defogger_state_req);
    VR_REQUEST_CASE(ClimateConciergeMode, mutable_climate_concierge_mode_req);
    default: break;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::TemperatureTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
         proto.reset(VR_new VrActionDE2DM());
         proto->set_id(id);
         proto->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_REQ);
         VrAdditionalInfoReq* pAdditionalProto = proto->mutable_additional_info_req();
         pAdditionalProto->set_request_id(Temperature);
         pAdditionalProto->mutable_temperature_req();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::TemperatureRangeTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
         proto.reset(VR_new VrActionDE2DM());
         proto->set_id(id);
         proto->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_REQ);
         VrAdditionalInfoReq* pAdditionalProto = proto->mutable_additional_info_req();
         pAdditionalProto->set_request_id(TemperatureRange);
         VrTemperatureRangeReq* pTempRange = pAdditionalProto->mutable_temperature_range_req();
         pTempRange->set_temperature_unit(0); // default value , we may not use .
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::FanSpeedTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
         proto.reset(VR_new VrActionDE2DM());
         proto->set_id(id);
         proto->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_REQ);
         VrAdditionalInfoReq* pAdditionalProto = proto->mutable_additional_info_req();
         pAdditionalProto->set_request_id(FanSpeed);
         pAdditionalProto->mutable_fan_speed_state_req();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::BlowerModeTransfrom(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
         proto.reset(VR_new VrActionDE2DM());
         proto->set_id(id);
         proto->set_vr_dm_action_type(ActionType_ADDITIONAL_INFO_REQ);
         VrAdditionalInfoReq* pAdditionalProto = proto->mutable_additional_info_req();
         pAdditionalProto->set_request_id(BlowerMode);
         pAdditionalProto->mutable_blower_mode_req();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::TurnClimateTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    if (doc.empty()) {
        return NULL;
    }
    static const VoiceMap<std::string, VrActionID>::type TurnOnMap = boost::assign::map_list_of
            ("AUTO_AIRCONDITION", AutoAirConditionerON)
            ("AC", ACON)
            ("DUAL_MODE", DualModeON)
            ("WIPER_DEICER", WiperDeicerON)
            ("REAR_DEFOGGER", RearDefoggerON)
            ("REAR_AUTO_AIRCONDITION", RearSeatAirConditionerON)
            ("CONCIERGE_MODE", ClimateConciergeModeON);
    static const VoiceMap<std::string, VrActionID>::type TurnOffMap = boost::assign::map_list_of
            ("AUTO_AIRCONDITION", AutoAirConditionerOFF)
            ("AC", ACOFF)
            ("DUAL_MODE", DualModeOFF)
            ("WIPER_DEICER", WiperDeicerOFF)
            ("REAR_DEFOGGER", RearDefoggerOFF)
            ("REAR_AUTO_AIRCONDITION", RearSeatAirConditionerOFF);
    std::string strTurnState = doc.select_node("//status").node().child_value();
    std::string strTurnType = doc.select_node("//type").node().child_value();
    bool bTurnOn = (strTurnState == "ON") ? true : false;
    VoiceMap<std::string, VrActionID>::type::const_iterator citer;
    if (bTurnOn) {
        citer = TurnOnMap.find(strTurnType);
        if (citer == TurnOnMap.end()) {
            return NULL;
        }
    }
    else {
        citer = TurnOffMap.find(strTurnType);
        if (citer == TurnOffMap.end()) {
            return NULL;
        }
    }
    VrActionID actionType = citer->second;
    std::unique_ptr<VrActionDE2DM> proto;
    proto.reset(VR_new VrActionDE2DM());
    proto->set_id(id);
    proto->set_vr_dm_action_type(ActionType_RECOG_RESULT);
    VrRecogResult* resultProto = proto->mutable_recog_result();
    switch (actionType) {
    VR_ACTION_CASE(AutoAirConditionerON, mutable_auto_air_conditioner_on);
    VR_ACTION_CASE(AutoAirConditionerOFF, mutable_auto_air_conditioner_off);
    VR_ACTION_CASE(ACON, mutable_ac_on);
    VR_ACTION_CASE(ACOFF, mutable_ac_off);
    VR_ACTION_CASE(DualModeON, mutable_dual_mode_on);
    VR_ACTION_CASE(DualModeOFF, mutable_dual_mode_off);
    VR_ACTION_CASE(WiperDeicerON, mutable_wiper_deicer_on);
    VR_ACTION_CASE(WiperDeicerOFF, mutable_wiper_deicer_off);
    VR_ACTION_CASE(RearDefoggerON, mutable_rear_defogger_on);
    VR_ACTION_CASE(RearDefoggerOFF, mutable_rear_defogger_off);
    VR_ACTION_CASE(RearSeatAirConditionerON, rear_seat_air_conditioner_on);
    VR_ACTION_CASE(RearSeatAirConditionerOFF, mutable_rear_seat_air_conditioner_off);
    VR_ACTION_CASE(ClimateConciergeModeON, mutable_climate_concierge_mode_on);
    default: break;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::ControlTempTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    if (doc.empty()) {
        return NULL;
    }
    const static VoiceMap<std::string, VrActionID>::type ControlTempMap = boost::assign::map_list_of
            ("INC", IncreaseTemperature)
            ("DEC", DecreaseTemperature)
            ("INC_LITTLE", AFewWarmer)
            ("DEC_LITTLE", AFewCooler);
    std::string strControlType = doc.select_node("//type").node().child_value();
    VoiceMap<std::string, VrActionID>::type::const_iterator citer;
    citer = ControlTempMap.find(strControlType);
    if (citer == ControlTempMap.end()) {
        return NULL;
    }
    VrActionID actionType = citer->second;
    std::unique_ptr<VrActionDE2DM> proto;
    proto.reset(VR_new VrActionDE2DM());
    proto->set_id(id);
    proto->set_vr_dm_action_type(ActionType_RECOG_RESULT);
    VrRecogResult* resultProto = proto->mutable_recog_result();
    switch (actionType) {
    case IncreaseTemperature:
        {
            resultProto->set_action_id(IncreaseTemperature);
            VrIncreaseTemperature * pTemperature = resultProto->mutable_increase_temperature();
            pTemperature->set_temperature_unit(0); // this value we always not use. but must set value. for required item.
            pTemperature->set_temperature(2); // default value. not for use
            break;
        }
    case DecreaseTemperature:
        {
            resultProto->set_action_id(DecreaseTemperature);
            VrDecreaseTemperature* pTemperature = resultProto->mutable_decrease_temperature();
            pTemperature->set_temperature_unit(0); // this value we always not use, but must set value, for required item
            pTemperature->set_temperature(2); // default value. not for use
            break;
        }
    case AFewWarmer:
        {
            resultProto->set_action_id(AFewWarmer);
            VrAFewWarmer * pTemperature = resultProto->mutable_afew_warmer();
            pTemperature->set_temperature_unit(0); // this value we always not use, but must set value, for required item
            pTemperature->set_temperature(1); // default value. not for use
            break;
        }
    case AFewCooler:
        {
            resultProto->set_action_id(AFewCooler);
            VrAFewCooler* pTemperature = resultProto->mutable_afew_cooler();
            pTemperature->set_temperature_unit(0);  // this value we always not use, but must set value, for required item
            pTemperature->set_temperature(1); // default value. not for use
            break;
        }
    default: break;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::ChangeTemperatureTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM>  proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_RECOG_RESULT);
        VrRecogResult* resultProto = proto->mutable_recog_result();
        if (NULL == resultProto) {
            return NULL;
        }
        std::string strValue = doc.select_node("//degree").node().child_value();
        int nStep(-1);
        if (strValue == "max" || strValue == "MAX") {
            resultProto->set_action_id(TemperatureMax);
            resultProto->mutable_temperature_max();
        }
        else if (strValue == "min" || strValue == "MIN") {
            resultProto->set_action_id(TemperatureMin);
            resultProto->mutable_temperature_min();
        }
        else {
            double fTempValue = atof(strValue.c_str());
            if (fTempValue >=  VR_CELSIUS && fTempValue <= VR_CELSIUS_MAX) {
                // this may be cause the point temperature is ignore
                nStep = VR_CEL_STEP_START + (int)((fTempValue - VR_CELSIUS) * VR_CEL_REVERSE_RATE);
            }
            else if (fTempValue >= VR_FAHRENHEIT && fTempValue <= VR_FAHRENHEIT_MAX) {
                nStep = VR_FAH_STEP_START + (int)(fTempValue - VR_FAHRENHEIT);
            }
            else {
            }
            resultProto->set_action_id(ChangeTemperature);
            VrChangeTemperature* pTemperature = resultProto->mutable_change_temperature();
            pTemperature->set_temperature(nStep);
        }
    }
    return proto.release();
}

VR_ProtoMessageBase*  VR_TransformXmlToProto::ChangeFanSpeedTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_RECOG_RESULT);
        VrRecogResult* resultProto = proto->mutable_recog_result();
        std::string strFanSpeed = doc.select_node("//speed").node().child_value();
        if (strFanSpeed == "Max" || strFanSpeed == "MAX") {
            resultProto->set_action_id(FanSpeedMax);
            resultProto->mutable_fan_speed_max();
        }
        else if (strFanSpeed == "Min" || strFanSpeed == "MIN") {
            resultProto->set_action_id(FanSpeedMin);
            resultProto->mutable_fan_speed_min();
        }
        else {
            int nFanSpeed = atoi(strFanSpeed.c_str());
            resultProto->set_action_id(ChangeFanSpeed);
            VrChangeFanSpeed* pFanSpeed = resultProto->mutable_change_fan_speed();
            pFanSpeed->set_fan_speed(nFanSpeed);
        }
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::ChangeFanModeTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_RECOG_RESULT);
        VrRecogResult* resultProto = proto->mutable_recog_result();
        std::string  strFanMode = doc.select_node("//mode").node().child_value();
        if (strFanMode == "FACE") {
            resultProto->set_action_id(BlowerModeFace);
            resultProto->mutable_blower_mode_face();
        }
        else if (strFanMode == "FOOT") {
            resultProto->set_action_id(BlowerModeFoot);
            resultProto->mutable_blower_mode_foot();
        }
        else if (strFanMode == "FACE_TO_FOOT") {
            resultProto->set_action_id(BlowerModeFaceAndFoot);
            resultProto->mutable_blower_mode_face_foot();
        }
        else if (strFanMode == "WINDOW_TO_FOOT") {
            resultProto->set_action_id(BlowerModeWindowAndFoot);
            resultProto->mutable_blower_mode_window_foot();
        }
        else {
        }
    }
    return proto.release();
}

/////// for vbt get msg
VR_ProtoMessageBase* VR_TransformXmlToProto::GetValidTemperature(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq* vrActiveInfo = proto->mutable_active_info_req();
        VrValidTemperature * pValidTemprature = vrActiveInfo->mutable_temperature_valid_req();
        vrActiveInfo->set_request_id(ValidTemperature);
        std::string strInfoType = doc.select_node("/action[@op='getValidTemperature']/type").node().child_value();
        if (strInfoType == "increase") {
            pValidTemprature->set_type(ValidTemperatureType_Increase);
        }
        else if (strInfoType == "decrease") {
            pValidTemprature->set_type(ValidTemperatureType_Decrease);
        }
        else if (strInfoType == "max") {
            pValidTemprature->set_type(ValidTemperatureType_Max);
        }
        else if (strInfoType == "min") {
            pValidTemprature->set_type(validTemperatureTeyp_Min);
        }
        else {
        }
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::CheckCurrTemperature(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq* vrActiveInfo = proto->mutable_active_info_req();
        VrCheckTemperature* pCheckTemp = vrActiveInfo->mutable_temperature_check_req();
        vrActiveInfo->set_request_id(CheckTemperature);
        std::string strDegree = doc.select_node("/action[@op='checkTemperature']/degree").node().child_value();
        float fTemperature = (float)atof(strDegree.c_str());
        pCheckTemp->set_degree(fTemperature);
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::GetValidFanSpeed(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq* vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->set_request_id(ValidFanSpeed);
        VrValidFanSpeed* pFanSpeed = vrActiveInfo->mutable_fan_speed_valid_req();
        std::string  strFanType = doc.select_node("/action[@op='getValidFanSpeed']/type").node().child_value();
        if (strFanType == "increase") {
            pFanSpeed->set_type(ValidFanSpeedType_Increase);
        }
        else if (strFanType == "decrease") {
            pFanSpeed->set_type(ValidFanSpeedType_Decrease);
        }
        else {
        }
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::CheckCurrFanSpeed(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq* vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->set_request_id(CheckFanSpeed);
        VrCheckFanSpeed* pFanSpeed = vrActiveInfo->mutable_fan_speed_check_req();
        std::string strFanSpeed = doc.select_node("/action[@op='checkFanSpeed']/speed").node().child_value();
        int nSpeed = atoi(strFanSpeed.c_str());
        pFanSpeed->set_speed(nSpeed);
    }
    return proto.release();
}
/////// for hvac end
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int VR_TransformXmlToProto::ChangeVoiceTagResult(const VR_String& errCode, const int funcCode) const
{
    VR_LOGI("errCode=[%s], funcCode=[%d]", errCode.c_str(), funcCode);
    if (errCode.empty()) {
        return VrNotifyRegisterResult_VrVoiceTagResultCode_Failure;
    }

    switch (atoi(errCode.c_str())) {
    case VOICETAG_OK :
        {
            if ((VrVoiceTagFunc_AddNewRecord == funcCode) || (VrVoiceTagFunc_EditRecord == funcCode)) {
               return VrNotifyRegisterResult_VrVoiceTagResultCode_Recorded;
            }
            else if (VrVoiceTagFunc_Register == funcCode) {
               return VrNotifyRegisterResult_VrVoiceTagResultCode_Registered;
            }
            else if ((VrVoiceTagFunc_Delete == funcCode) || (VrVoiceTagFunc_DeleteAll == funcCode)) {
               return VrNotifyRegisterResult_VrVoiceTagResultCode_Deleted;
            }
            else if (VrVoiceTagFunc_CancelRecord == funcCode) {
               return VrNotifyRegisterResult_VrVoiceTagResultCode_RecordCanceled;
            }
            else if (VrVoiceTagFunc_CancelRegister == funcCode) {
               return VrNotifyRegisterResult_VrVoiceTagResultCode_RegisterCanceled;
            }
            else {
                return VrNotifyRegisterResult_VrVoiceTagResultCode_Failure;
            }
        }
    case VOICETAG_FAILURE :
        {
            return VrNotifyRegisterResult_VrVoiceTagResultCode_Failure;
        }
    default :
        return VrNotifyRegisterResult_VrVoiceTagResultCode_Failure;
    }
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultAddVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
        VrNotifyVoiceTagResult* notifyVoiceTagResult = proto->mutable_notify_voice_tag_result();
        notifyVoiceTagResult->set_sender("DE2DM");
        notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_AddNewRecord);
        VR_String strErrCode = doc.root().first_child().attribute("errcode").value();
        int nErrCode = ChangeVoiceTagResult(strErrCode, VrVoiceTagFunc_AddNewRecord);
        VR_String strVoiceTagID = doc.select_node("/event-result[@name='addRecordVoiceTag']/voiceTagId").node().child_value();
        VrNotifyRegisterResult* registerResult = notifyVoiceTagResult->mutable_registerresult();
        registerResult->add_voicetagid(atoi(strVoiceTagID.c_str()));
        registerResult->set_result((VrNotifyRegisterResult_VrVoiceTagResultCode)nErrCode);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultEditVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
        VrNotifyVoiceTagResult* notifyVoiceTagResult = proto->mutable_notify_voice_tag_result();
        notifyVoiceTagResult->set_sender("DE2DM");
        notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_EditRecord);
        VR_String strErrCode = doc.root().first_child().attribute("errcode").value();
        int nErrCode = ChangeVoiceTagResult(strErrCode, VrVoiceTagFunc_EditRecord);
        VR_String strVoiceTagID = doc.select_node("/event-result[@name='editRecordVoiceTag']/voiceTagId").node().child_value();
        VrNotifyRegisterResult* registerResult = notifyVoiceTagResult->mutable_registerresult();
        registerResult->add_voicetagid(atoi(strVoiceTagID.c_str()));
        registerResult->set_result((VrNotifyRegisterResult_VrVoiceTagResultCode)nErrCode);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultSaveVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
        VrNotifyVoiceTagResult* notifyVoiceTagResult = proto->mutable_notify_voice_tag_result();
        notifyVoiceTagResult->set_sender("DE2DM");
        notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_Register);
        VR_String strErrCode = doc.root().first_child().attribute("errcode").value();
        int nErrCode = ChangeVoiceTagResult(strErrCode, VrVoiceTagFunc_Register);
        VR_String strVoiceTagID = doc.select_node("/event-result[@name='saveVoiceTag']/voiceTagId").node().child_value();
        VrNotifyRegisterResult* registerResult = notifyVoiceTagResult->mutable_registerresult();
        registerResult->add_voicetagid(atoi(strVoiceTagID.c_str()));
        registerResult->set_result((VrNotifyRegisterResult_VrVoiceTagResultCode)nErrCode);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultDeleteVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
        VrNotifyVoiceTagResult* notifyVoiceTagResult = proto->mutable_notify_voice_tag_result();
        notifyVoiceTagResult->set_sender("DE2DM");
        notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_Delete);
        VR_String strErrCode = doc.root().first_child().attribute("errcode").value();
        int nErrCode = ChangeVoiceTagResult(strErrCode, VrVoiceTagFunc_Delete);
        VR_String strVoiceTagID = doc.select_node("/event-result[@name='deleteVoiceTag']/voiceTagId").node().child_value();
        VrNotifyRegisterResult* registerResult = notifyVoiceTagResult->mutable_registerresult();
        if (!strVoiceTagID.empty()) {
            registerResult->add_voicetagid(atoi(strVoiceTagID.c_str()));
        }
        registerResult->set_result((VrNotifyRegisterResult_VrVoiceTagResultCode)nErrCode);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultPlayVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
        VrNotifyVoiceTagResult* notifyVoiceTagResult = proto->mutable_notify_voice_tag_result();
        notifyVoiceTagResult->set_sender("DE2DM");
        notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_Play);
        VR_String strErrCode = doc.root().first_child().attribute("errcode").value();
        VrNotifyPlayResult* playResult = notifyVoiceTagResult->mutable_playresult();
        if (VOICETAG_OK == (VR_VoiceTagResult)atoi(strErrCode.c_str())) {
            playResult->set_result(true);
        }
        else {
            playResult->set_result(false);
        }
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultStopVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
        VrNotifyVoiceTagResult* notifyVoiceTagResult = proto->mutable_notify_voice_tag_result();
        notifyVoiceTagResult->set_sender("DE2DM");
        notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_Stop);
        VR_String strErrCode = doc.root().first_child().attribute("errcode").value();
        VrNotifyPlayResult* playResult = notifyVoiceTagResult->mutable_playresult();
        if (VOICETAG_OK == (VR_VoiceTagResult)atoi(strErrCode.c_str())) {
            playResult->set_result(true);
        }
        else {
            playResult->set_result(false);
        }
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultCancelRecoedVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
        VrNotifyVoiceTagResult* notifyVoiceTagResult = proto->mutable_notify_voice_tag_result();
        notifyVoiceTagResult->set_sender("DE2DM");
        notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_CancelRecord);
        VR_String strErrCode = doc.root().first_child().attribute("errcode").value();
        int nErrCode = ChangeVoiceTagResult(strErrCode, VrVoiceTagFunc_CancelRecord);
        VrNotifyRegisterResult* registerResult = notifyVoiceTagResult->mutable_registerresult();
        registerResult->set_result((VrNotifyRegisterResult_VrVoiceTagResultCode)nErrCode);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultCancelSaveVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
        VrNotifyVoiceTagResult* notifyVoiceTagResult = proto->mutable_notify_voice_tag_result();
        notifyVoiceTagResult->set_sender("DE2DM");
        notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_CancelRegister);
        VR_String strErrCode = doc.root().first_child().attribute("errcode").value();
        int nErrCode = ChangeVoiceTagResult(strErrCode, VrVoiceTagFunc_CancelRegister);
        VrNotifyRegisterResult* registerResult = notifyVoiceTagResult->mutable_registerresult();
        registerResult->set_result((VrNotifyRegisterResult_VrVoiceTagResultCode)nErrCode);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultSyncVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_VoiceTag);
        VR_String strDeviceAddress = doc.select_node("/event-result[@name='syncVoiceTag']/deviceAddress").node().child_value();
        proto->mutable_syncvoicetagresult()->set_device_id(atoi(strDeviceAddress.c_str()));
        VR_String strVoiceTags = doc.select_node("/event-result[@name='syncVoiceTag']/voiceTagIds").node().child_value();
        VR_LOGI("deviceAddress=[%s],voiceTagIds=[%s]", strDeviceAddress.c_str(), strVoiceTags.c_str());
        if (!strVoiceTags.empty()) {
            size_t startPos = 0;
            size_t pos = strVoiceTags.find(",");
            while (std::string::npos != pos) {
                proto->mutable_syncvoicetagresult()->add_voicetag_id(atoi(strVoiceTags.substr(startPos, pos - startPos).c_str()));
                startPos = pos + 1;
                pos = strVoiceTags.find(",", startPos);
            }
            proto->mutable_syncvoicetagresult()->add_voicetag_id(atoi(strVoiceTags.substr(startPos).c_str()));
        }
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultTslTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_TransmitDataToTSLNotify);
        VrTransmitDataToTSLNotify* transmitDataToTSLNotify = proto->mutable_transmitdatatotslnotify();
        std::stringstream stream;
        doc.save(stream);
        transmitDataToTSLNotify->set_data(stream.str());
        int length = stream.str().length();
        transmitDataToTSLNotify->set_data_length(length);
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::ActionNotifyTslTransform(int id, pugi_vr::xml_document &doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_TransmitDataToTSLNotify);
        VrTransmitDataToTSLNotify* transmitDataToTSLNotify = proto->mutable_transmitdatatotslnotify();
        std::stringstream stream;
        doc.save(stream);
        transmitDataToTSLNotify->set_data(stream.str());
        int length = stream.str().length();
        transmitDataToTSLNotify->set_data_length(length);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::CloseSessionTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_CloseSession);
        proto->mutable_sessiontype()->set_sessiontype(Session_VoiceTag);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::CloseVoiceTagTtsSessionTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_CloseSession);
        proto->mutable_sessiontype()->set_sessiontype(Session_VoiceTagTts);
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::EventResultStartServiceConditonTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_StartService);
        VrStartServiceResult* startResult =  proto->mutable_start_service_conditon();
        VR_String strState = doc.select_node("/event-result[@name='getResourceState']/state").node().child_value();
        int nState = atoi(strState.c_str());
        startResult->set_code(StartService_dic_lang);
        startResult->set_result(nState);
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::EventResultChangeLanguageTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_ChangeLanguage);
        VrChangeLangueResult *changeLanguage = proto->mutable_change_language_result();
        VR_String strLanguage = doc.select_node("/event-result[@name='changeLanguage']/language").node().child_value();
        changeLanguage->set_language(strLanguage);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultSmartAgentTransform(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrEngineControlMessage> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_SmartAgent);
        VrSmartAgentResult * agentResult = proto->mutable_smartagentresult();
        agentResult->set_type(smartagent_end);
        agentResult->set_userid(0x00);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::EventResultInitialPersonalDataTransform(int id, pugi_vr::xml_document& doc) const
{
    VR_LOGD("id = [%d]", id);

    std::unique_ptr<VrEngineControlMessage>  proto;
    if (!doc.empty()) {
        proto.reset(VR_new  VrEngineControlMessage);
        proto->set_type(DMTaskMessageType_InitPersonalData);
        VrPersonalDataResult * personalData = proto->mutable_personalresult();
        personalData->set_datatype(persondata_end);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneGetMessageDetailTransform(int id, pugi::xml_document &doc) const
{
    std::unique_ptr<VrActionDE2DM> proto;

    if (!doc.empty()) {

        VR_String instanceId = doc.select_node("//instanceId").node().child_value();
        VR_String messageId = doc.select_node("//messageId").node().child_value();
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_MESSAGE_INFO_REQ);
        proto->mutable_hfd_message_info_req()->set_message_no(0);
        proto->mutable_hfd_message_info_req()->set_instance_id(atoi(instanceId.c_str()));
        proto->mutable_hfd_message_info_req()->set_message_id(messageId);
        proto->mutable_hfd_message_info_req()->set_service_id(atoi(instanceId.c_str()));
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneAddVoiceTagTransform(int id, pugi::xml_document &doc) const
{
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(AddVoiceTag)
    resultProto->mutable_add_voice_tag();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneShowRecentCallTransform(int id, pugi::xml_document &doc) const
{
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(CallHistory)
    resultProto->mutable_call_history();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneListAllContactTransform(int id, pugi::xml_document &doc) const
{
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(ListAllContact)

    resultProto->mutable_list_all_contact();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneListFavourContactTransform(int id, pugi::xml_document &doc) const
{
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(PhoneListFavourites)

    resultProto->mutable_phone_list_favour();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneSendMessageTransform(int id, pugi::xml_document &doc) const
{
    VR_String messageType = doc.select_node("//messageType").node().child_value();
    VR_String address = doc.select_node("//number").node().child_value();
    VR_String body = doc.select_node("//messageBody").node().child_value();
    VR_String name = doc.select_node("//name").node().child_value();

    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(SendMessage)

    VrSendMessage* sendMsg = resultProto->mutable_send_message();
    sendMsg->set_message_type(atoi(messageType.c_str()));
    sendMsg->set_address_length(address.length());
    sendMsg->set_title_length(name.length());
    sendMsg->set_message_body_length(body.length());
    sendMsg->set_address(address.c_str());
    sendMsg->set_title(name.c_str());
    sendMsg->set_message_body(body.c_str());

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneSendDTMFToneTransform(int id, pugi::xml_document &doc) const
{
    VR_String tones = doc.select_node("//tones").node().child_value();

    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(SendDTMF)

    VrSendDTMF* sendTone = resultProto->mutable_send_dtmf();
    sendTone->set_tone_data_length(tones.length());
    sendTone->set_tone_data(tones.c_str());

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneMuteOffTransform(int id, pugi::xml_document &doc) const
{
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(MUTEOFF)

    resultProto->mutable_mute_off();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneMuteOnTransform(int id, pugi::xml_document &doc) const
{
    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);
    VR_CREATE_RECOGRESULT_PROTO(MUTEON)

    resultProto->mutable_mute_on();
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneDialTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGD_FUNC();
    static const VoiceMap<VR_String, VrActionID>::type dialMap = boost::assign::map_list_of
            ("CALL",           Call)
            ("REDIA",          Redial)
            ("CALL_NUMBER",    CallWithNo)
            ("CALL_RECENT",    CallRecent)
            ("CALL_SHORTRING", CallShortRing);

    VR_String dialType    = doc.select_node("//dialType").node().child_value();
    VR_String phoneNum    = doc.select_node("//number").node().child_value();
    VR_String phoneType   = doc.select_node("//phoneType").node().child_value();
    VR_INT32  phoneKind   = atoi(phoneType.c_str());
    VR_String contactName = doc.select_node("//contactName").node().child_value();
    VR_LOGI("dialType is[%s], number is[%s], kind is[%d], name is[%s]",
            dialType.c_str(), phoneNum.c_str(), phoneKind, contactName.c_str());
    VoiceMap<VR_String, VrActionID>::type::const_iterator cDialMap;
    cDialMap = dialMap.find(dialType);
    if (cDialMap == dialMap.end()) {
        VR_LOGE("");
        // return NULL;
    }
    VrActionID enDialType = cDialMap->second;

    VR_CREATE_DE2DM_PROTO(ActionType_RECOG_RESULT);

    VrRecogResult *resultProto = proto->mutable_recog_result();

    switch (enDialType) {
    case Call :
        {
            resultProto->set_action_id(Call);
            VrPhoneCall * playAlbum = resultProto->mutable_phone_call();

            playAlbum->set_name_length(contactName.length());
            playAlbum->set_phone_number_length(phoneNum.length());
            playAlbum->set_name(contactName.c_str());
            phoneKind = PhoneTypeTransUnit2AVC(phoneKind);
            playAlbum->set_phone_kind(phoneKind);
            playAlbum->set_phone_number(phoneNum.c_str());
        }
        break;
    case CallWithNo :
        {
            resultProto->set_action_id(CallWithNo);
            VrPhoneCallWithNo * playAlbum = resultProto->mutable_phone_call_no();

            playAlbum->set_phone_number_length(phoneNum.length());
            phoneKind = PhoneTypeTransUnit2AVC(phoneKind);
            playAlbum->set_phone_kind(phoneKind);
            playAlbum->set_phone_number(phoneNum.c_str());
        }
        break;
    case CallRecent :
        {
            resultProto->set_action_id(CallRecent);
            VrPhoneCallRecent * playAlbum = resultProto->mutable_phone_call_recent();

            playAlbum->set_name_length(contactName.length());
            playAlbum->set_phone_number_length(phoneNum.length());
            playAlbum->set_name(contactName.c_str());
            phoneKind = PhoneTypeTransUnit2AVC(phoneKind);
            playAlbum->set_phone_kind(phoneKind);
            playAlbum->set_phone_number(phoneNum.c_str());
        }
        break;
    case CallShortRing :
        {
            resultProto->set_action_id(CallShortRing);
            VrPhoneCallShortRing * playAlbum = resultProto->mutable_phone_call_short_ring();

            playAlbum->set_name_length(contactName.length());
            playAlbum->set_phone_number_length(phoneNum.length());
            playAlbum->set_name(contactName.c_str());
            phoneKind = PhoneTypeTransUnit2AVC(phoneKind);
            playAlbum->set_phone_kind(phoneKind);
            playAlbum->set_phone_number(phoneNum.c_str());
        }
        break;
    case Redial :
        {
            resultProto->set_action_id(Redial);
            VrPhoneRedial * playAlbum = resultProto->mutable_phone_redial();

            playAlbum->set_name_length(contactName.length());
            playAlbum->set_phone_number_length(phoneNum.length());
            playAlbum->set_name(contactName.c_str());
            phoneKind = PhoneTypeTransUnit2AVC(phoneKind);
            playAlbum->set_phone_kind(phoneKind);
            playAlbum->set_phone_number(phoneNum.c_str());
        }
        break;
    default:
        break;
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneGetMsgListByTypeTransform(int id, pugi::xml_document &doc) const
{
    static const ULONG EMAIL_INDEX = 1;
    static const ULONG SMS_INDEX = 5;
    static const ULONG MMS_INDEX = 4;

    VR_String reqCount = doc.select_node("//count").node().child_value();
    VR_String msgType  = doc.select_node("//messageType").node().child_value();

    ULONG messageType = 0;
    if (VR_String::npos != msgType.find("EMAIL")) {
        messageType = EMAIL_INDEX;
    }

    if (VR_String::npos != msgType.find("SMS")) {
        messageType = SMS_INDEX;
    }

    if (VR_String::npos != msgType.find("MMS")) {
        messageType = MMS_INDEX;
    }
    VR_LOGD("MessageType From DE:[%d]; UNKNOW(0)-EMAIL(1)-SMS(2|3)-MMS(4)", messageType);

    VR_CREATE_DE2DM_PROTO(ActionType_ACTIVE_INFO_REQ);
    VR_CREATE_ACTIVEINFOREQ_PROTO(RecentMessageListByType)

    VrRecentMessageListByTypeReq* msgTypeList = activeInfoProto->mutable_recent_type_msg_list_req();
    msgTypeList->set_count(atoi(reqCount.c_str()));
    msgTypeList->set_type(messageType);

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneGetRecentMsgListTransform(int id, pugi::xml_document &doc) const
{
    VR_String msgCount    = doc.select_node("//count").node().child_value();

    VR_CREATE_DE2DM_PROTO(ActionType_ACTIVE_INFO_REQ);
    VR_CREATE_ACTIVEINFOREQ_PROTO(RecentMessageList)

    VrRecentMessageListReq* msgList = activeInfoProto->mutable_recent_msg_list_req();
    msgList->set_count(atoi(msgCount.c_str()));

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneGetOutGoingCallListTransform(int id, pugi::xml_document &doc) const
{
    VR_CREATE_DE2DM_PROTO(ActionType_ACTIVE_INFO_REQ);
    VR_CREATE_ACTIVEINFOREQ_PROTO(RecentOutgoingCall)

    activeInfoProto->mutable_outgoing_call_list_req();

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneGetInComingCallListTransform(int id, pugi::xml_document &doc) const
{
    VR_CREATE_DE2DM_PROTO(ActionType_ACTIVE_INFO_REQ);
    VR_CREATE_ACTIVEINFOREQ_PROTO(RecentIncomingCall)

    activeInfoProto->mutable_incoming_call_list_req();

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneGetRecentCallListTransform(int id, pugi::xml_document &doc) const
{
    VR_String callCount    = doc.select_node("//count").node().child_value();

    VR_CREATE_DE2DM_PROTO(ActionType_ACTIVE_INFO_REQ);
    VR_CREATE_ACTIVEINFOREQ_PROTO(RecentCallList)

    VrRecentCallListReq* callList = activeInfoProto->mutable_recent_call_list_req();
    callList->set_count(atoi(callCount.c_str()));

    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::PhoneVehicleMontionTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq* vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->mutable_phone_vehicle_in_motion();
        vrActiveInfo->set_request_id(PhoneVehicleInMontion);
    }
    return proto.release();
}

// // vehicle motion
VR_ProtoMessageBase* VR_TransformXmlToProto::QueryVehicleMontionTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq* vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->mutable_vehicle_in_motion();
        vrActiveInfo->set_request_id(VehicleInMotion);
    }
    return proto.release();
}

// prompt level
VR_ProtoMessageBase* VR_TransformXmlToProto::SmartAgentPromptLevelTransform(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrActionDE2DM> proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq* vrActiveInfo = proto->mutable_active_info_req();
        std::string strLevel = doc.select_node("//level").node().child_value();
        vrActiveInfo->mutable_smart_prompt_level()->set_promptlevel(strLevel);
        vrActiveInfo->set_request_id(SmartAgentPromptLevel);
        VR_LOGI("strLevel = [%s]", strLevel.c_str());
    }
    return proto.release();
}
// gbook status check
VR_ProtoMessageBase* VR_TransformXmlToProto::GBookCheckStatusTransform(int id, pugi::xml_document& doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrActionDE2DM>  proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq*  vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->mutable_gbook_state_check();
        vrActiveInfo->set_request_id(GbookStateCheck);
    }
    return proto.release();
}

// gbook check error status
VR_ProtoMessageBase*  VR_TransformXmlToProto::GBookCheckErrorTransform(int id, pugi::xml_document & doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrActionDE2DM>  proto;
    if (!doc.empty()) {
        proto.reset(VR_new   VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq*  vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->mutable_gbook_call_assit();     // gbook_call_assit we use, because define , not use
        vrActiveInfo->set_request_id(GbookCallAssit);
    }
    return  proto.release();
}

// de status
VR_ProtoMessageBase*   VR_TransformXmlToProto::RecourceStateTransform(int id, pugi_vr::xml_document& doc) const
{
    VR_LOGI("id = [%d]", id);
    std::unique_ptr<VrActionDE2DM>   proto;
    if (!doc.empty()) {
        proto.reset(VR_new  VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq*  vrActiveInfo = proto->mutable_active_info_req();
        vrActiveInfo->set_request_id(DEStatusResult);
        VrDEStatusResult* deStatus = vrActiveInfo->mutable_destatus_result();
        VR_String strType   = doc.select_node("//resourceStateType").node().child_value();
        if (strType.empty()) {
            strType = "unknow";
        }
        deStatus->set_type(strType);
        VR_String strResult = doc.select_node("//resourceState").node().child_value();
        int       nResult   = atoi(strResult.c_str());
        deStatus->set_result(nResult);
    }
    return  proto.release();
}

// start finish notify
VR_ProtoMessageBase*   VR_TransformXmlToProto::StartFinishNotifyTransform(int id, pugi_vr::xml_document& doc) const
{
    VR_LOGI("id = [%d]", id);

    std::unique_ptr<VrActionDE2DM>   proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq*  VrActionInfo = proto->mutable_active_info_req();
        VrActionInfo->set_request_id(StartFinishNotify);
        VrActionInfo->mutable_startfinish_notify();
    }
    return proto.release();
}

// gbook center url
VR_ProtoMessageBase*  VR_TransformXmlToProto::GBookCenterUrlTransform(int id, pugi_vr::xml_document& doc) const
{
    VR_LOGI("id = [%d]",  id);

    std::unique_ptr<VrActionDE2DM>   proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ACTIVE_INFO_REQ);
        VrActiveInfoReq*  VrActionInfo = proto->mutable_active_info_req();
        VrActionInfo->set_request_id(GbookCenterUrl);
        VrActionInfo->mutable_gbook_center_url();
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::AwakenTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrActionDE2DM>   proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_SESSION_REQ);
        VrSessionReq*  pVrSessionReq = proto->mutable_session_req();
        pVrSessionReq->set_func_code(VrSessionReq_VrSessionType_open);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::QuitTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrActionDE2DM>   proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_SESSION_REQ);
        VrSessionReq*  pVrSessionReq = proto->mutable_session_req();
        pVrSessionReq->set_func_code(VrSessionReq_VrSessionType_close);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::ActionTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ALEXA_ACTION);
        proto->mutable_alexa_action()->set_msg(msg);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::AwakenWithOutStartTransform(int id, pugi::xml_document &doc) const
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrActionDE2DM>   proto;
    if (!doc.empty()) {
        proto.reset(VR_new VrActionDE2DM);
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_SESSION_REQ);
        VrSessionReq*  pVrSessionReq = proto->mutable_session_req();
        pVrSessionReq->set_func_code(VrSessionReq_VrSessionType_open_without_start);
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::DirveScoreTransform(int id, const std::string &msg) const
{
    VR_LOGD_FUNC();

    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_ALEXA_ACTION);
        proto->mutable_alexa_action()->set_msg(msg);
    }
    return proto.release();
}

// navi
VR_ProtoMessageBase* VR_TransformXmlToProto::NaviReqDfltInfoTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryDefaultInfo);
        *(proto->mutable_voice_navi_req()->mutable_query_default_info_req()) = msg;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryStateInfoTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryStateInfo);
        // for china address's state search
        if (doc.select_node("/action[@detype='cnde']")) {
            proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryStateInfoCN);
        }
        *(proto->mutable_voice_navi_req()->mutable_query_state_info_req()) = msg;
        VR_LOGI("XML2PROTO Query State Info Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryCityInfoTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryCityInfo);
        // for china address's city search
        if (doc.select_node("/action[@detype='cnde']")) {
            proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryCityInfoCN);
        }
        *(proto->mutable_voice_navi_req()->mutable_query_city_info_req()) = msg;
        VR_LOGI("XML2PROTO Query City Info Success!");
        return proto.release();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryStreetInfoTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryStreetInfo);
        // for china address's street search
        if (doc.select_node("/action[@detype='cnde']")) {
            proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryStreetInfoCN);
        }
        *(proto->mutable_voice_navi_req()->mutable_query_street_info_req()) = msg;
        VR_LOGI("XML2PROTO Query Street Info Success!");
        return proto.release();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryAddressInfoTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryHouseNoInfo);
        // for china address's houseno search
        if (doc.select_node("/action[@detype='cnde']")) {
            proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryHouseNoInfoCN);
        }
        *(proto->mutable_voice_navi_req()->mutable_query_houseno_info_req()) = msg;
        VR_LOGI("XML2PROTO Query HouseNo Info Success!");
        return proto.release();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryOneShotInfoTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryAddressInfoCN); // china only
        *(proto->mutable_voice_navi_req()->mutable_query_address_info_req()) = msg;
        VR_LOGI("XML2PROTO Query Address Info Success!");
        return proto.release();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryDistrictInfoTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryDistrictInfoCN); // china only
        *(proto->mutable_voice_navi_req()->mutable_query_district_info_req()) = msg;
        VR_LOGI("XML2PROTO Query District Info Success!");
        return proto.release();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryShowPOIIconNoTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryShowingPOIIconNumber);
        *(proto->mutable_voice_navi_req()->mutable_query_showing_poi_icon_number_req()) = msg;
        VR_LOGI("XML2PROTO Query Showing POI Icon Number Success!");
        return proto.release();
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryPOINearbyTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryPOICollectionNearby);
        *(proto->mutable_voice_navi_req()->mutable_query_poi_collection_nearby_req()) = msg;
        VR_LOGI("XML2PROTO Query POI Nearby Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryPOINearDestinationTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryPOICollectionNearDestination);
        *(proto->mutable_voice_navi_req()->mutable_query_poi_collection_near_destination_req()) = msg;
        VR_LOGI("XML2PROTO Query POI Near Destination Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryPOIAlongRouteTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryPOICollectionAlongRoute);
        *(proto->mutable_voice_navi_req()->mutable_query_poi_collection_along_route_req()) = msg;
        VR_LOGI("XML2PROTO Query POI Along Route Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryPointCollectionTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryPointCollection);
        *(proto->mutable_voice_navi_req()->mutable_query_point_collection_req()) = msg;
        VR_LOGI("XML2PROTO Query Point Collection Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryDestHistoryTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryDestinationHistory);
        *(proto->mutable_voice_navi_req()->mutable_query_destination_history_req()) = msg;
        VR_LOGI("XML2PROTO Query Destination History Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryRouteExistTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryRouteExist);
        *(proto->mutable_voice_navi_req()->mutable_query_route_exist_req()) = msg;
        VR_LOGI("XML2PROTO Query Route Exist Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryAllDestListTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryAllDestinationList);
        *(proto->mutable_voice_navi_req()->mutable_query_all_destination_list()) = msg;
        VR_LOGI("XML2PROTO Query All Destination List Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryUnpassedDestListTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryUnpassedDestinationList);
        *(proto->mutable_voice_navi_req()->mutable_query_unpassed_destination_list_req()) = msg;
        VR_LOGI("XML2PROTO Query Unpassed Destination List Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryPointTypeTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryPointType);
        *(proto->mutable_voice_navi_req()->mutable_query_point_type_req()) = msg;
        VR_LOGI("XML2PROTO Query Point Type Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryNextDestTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryNextDestination);
        *(proto->mutable_voice_navi_req()->mutable_query_next_destination_req()) = msg;
        VR_LOGI("XML2PROTO Query Next Destination Success!");
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryPointDateAreaTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML %s", msg.c_str());
    std::unique_ptr<VrActionDE2DM> proto;
    pugi::xml_document doc;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryPointDataArea);
        *(proto->mutable_voice_navi_req()->mutable_query_point_data_area_req()) = msg;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviShowPOIIconTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_ShowPoiIcon);
        *(proto->mutable_voice_navi_req()->mutable_show_poi_icon_req()) = msg;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviHidePOIIconTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_hidePOIIcon);
        *(proto->mutable_voice_navi_req()->mutable_hide_poi_icon_req()) = msg;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviAddToRouteTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_addToRoute);
        *(proto->mutable_voice_navi_req()->mutable_add_to_route()) = msg;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviDelDestinationTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_deleteDestination);
        *(proto->mutable_voice_navi_req()->mutable_delete_destination_req()) = msg;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviCalcRouteTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_calculateRoute);
        *(proto->mutable_voice_navi_req()->mutable_calculate_route_req()) = msg;
    }
    return proto.release();
}

VR_ProtoMessageBase* VR_TransformXmlToProto::NaviQueryCurrentPosTransform(int id, const std::string& msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryCurrentPosition);
        *(proto->mutable_voice_navi_req()->mutable_query_current_position_req()) = msg;
    }
    return proto.release();
}

VR_ProtoMessageBase *VR_TransformXmlToProto::NaviQueryPOIByNameTransform(int id, const std::string &msg) const
{
    VR_LOGI("NAVI::XML IS [%s]", msg.c_str());
    pugi::xml_document doc;
    std::unique_ptr<VrActionDE2DM> proto;
    if (doc.load(msg.c_str())) {
        proto.reset(VrActionDE2DM::default_instance().New());
        proto->set_id(id);
        proto->set_vr_dm_action_type(ActionType_Navi_Action_REQ);
        proto->mutable_voice_navi_req()->set_actiontype(NAVI_ACTION_queryPOINameNearby);
        *(proto->mutable_voice_navi_req()->mutable_query_poi_name_nearby_req()) = msg;
        VR_LOGI("XML2PROTO Query POI By Name Success!");
    }
    return proto.release();
}

VR_INT32 VR_TransformXmlToProto::PhoneTypeTransUnit2AVC(VR_INT32 typeValue) const
{
    auto itr = VrHfdPhoneTypeTrans.find(typeValue);
    if (itr != VrHfdPhoneTypeTrans.end()) {
        VR_LOGI("[-- HfdPhoneTypeTransform: itr->second(%d) --]", itr->second);
        return itr->second;
    }

    VR_LOGI("[-- HfdPhoneTypeTransform: type(%d) --]", typeValue);
    return VR_AVC_PHONE_TYPE_HIDE_ICON;
}

/* EOF */



