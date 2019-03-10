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
#include <sstream>

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "pugixml.hpp"
#include "VR_HAVCTypedefine.h"
#include "VR_TransformProtoToXml.h"
#include "VR_ActionInnerParameterDef.h"
#include "VR_ActionSenderDef.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrUnitStatus.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrMessageTrans.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"
#include "VR_CommonTool.h"

using namespace navi::VoiceRecog;

inline const char* int2str(int intData)
{
    std::stringstream ss;
    std::string str;
    ss << intData;
    ss >> str;
    const char* resultStr = str.c_str();
    return resultStr;
}


VR_TransformProtoToXml::VR_TransformProtoToXml()
{
    VR_LOGD_FUNC();
}

VR_TransformProtoToXml::~VR_TransformProtoToXml()
{
    VR_LOGD_FUNC();
}

VR_String VR_TransformProtoToXml::Transform(const VrMessageAcquisitionResp &msgInfo)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"phone\" op=\"getMessageDetail\" errcode=\"0\">"\
                                          "<messagebody>%1%</messagebody>"\
                                      "</action-result>";
    if (msgInfo.has_op_result()) {
        VR_String body = msgInfo.body();
        resultXml = (boost::format(startXml) % body).str();
    }

    return resultXml;
}

const VR_String &VR_TransformProtoToXml::MsgTypeEnumToXML(const int& type)
{
    VR_LOGI("type=[%d]", type);
    static const VR_String& FAILED_STR = "";
    static const VoiceMap<int, VR_String>::type recogResultTypeMap = boost::assign::map_list_of
            (AudioOFF,                   "turnAudioOff")
            (AudioON,                    "turnAudioOn")
            (SwitchSource,               "changeSourceByName")
            (PlayAlbum,                  "play")
            (PlayAll,                    "play")
            (PlayArtist,                 "play")
            (PlayAudioBook,              "play")
            (PlayComposer,               "play")
            (PlayGenre,                  "play")
            (PlayPlaylist,               "play")
            (PlayPodcast,                "play")
            (PlayPodcastItem,            "play")
            (PlaySong,                   "play")
            (StartRadio,         "startRadio")
            (ChangeHDSubChannel,         "changeHDSubChannel")
            (TuneFrequency,              "tuneFrequency")
            (TuneFMHDFrequency,          "tuneFMHDFrequency")
            (TuneFrequencyAM,            "tuneLastAM")
            (TuneFrequencyFM,            "tuneLastFM")
            (TuneFrequencySatellite,     "tuneLastSatellite")
            (TunePreset,                 "playByPreset")
            (TuneSatelliteChannelName,   "playByName")
            (TuneSatelliteChannelNumber, "tuneSatelliteChannelNumber")
            (TuneDABChannelName,         "playByName")
            (TuneFMChannelName,          "playByName")
            (TuneFMGenre,                "playByGenre")
            (TuneSatelliteGenre,         "playByGenre")
            // apps
            (StartApps,                  "launchApp")
            // hvac action
            (AutoAirConditionerON,       "turn")
            (AutoAirConditionerOFF,      "turn")
            (ACON,                       "turn")
            (ACOFF,                      "turn")
            (DualModeON,                 "turn")
            (DualModeOFF,                "turn")
            (WiperDeicerON,              "turn")
            (WiperDeicerOFF,             "turn")
            (RearSeatAirConditionerON,   "turn")
            (RearSeatAirConditionerOFF,  "turn")
            (RearDefoggerON,             "turn")
            (RearDefoggerOFF,            "turn")
            (ClimateConciergeModeON,     "turn")
            (DecreaseTemperature,        "controlTemperature")
            (IncreaseTemperature,        "controlTemperature")
            (AFewWarmer,                 "controlTemperature")
            (AFewCooler,                 "controlTemperature")
            (TemperatureMax,             "changeTemperature")
            (TemperatureMin,             "changeTemperature")
            (ChangeTemperature,          "changeTemperature")
            (ChangeFanSpeed,             "changeFanSpeed")
            (FanSpeedMax,                "changeFanSpeed")
            (FanSpeedMin,                "changeFanSpeed")
            (BlowerModeFace,             "changeFanMode")
            (BlowerModeFoot,             "changeFanMode")
            (BlowerModeFaceAndFoot,      "changeFanMode")
            (BlowerModeWindowAndFoot,    "changeFanMode")
            (HVACFrontScreen,            "showScreen")
            (HVACRearScreen,             "showScreen")
            (HVACSteeringScreen,         "showScreen")
            (HVACFrontSeatScreen,        "showScreen")
            (HVACRearSeatScreen,         "showScreen")
            (HVACLexusConciergeScreen,   "showScreen")
            (HVACSeatOperationScreen,    "showScreen")
            (FrontSeatVentilationScreen, "showScreen")
            (RearSeatVentilationScreen,  "showScreen")
            // HFD ActionId
            (Call,                       "dial")
            (CallWithNo,                 "dial")
            (CallRecent,                 "dial")
            (MUTEON,                     "muteCall")
            (MUTEOFF,                    "muteOff")
            (CallShortRing,              "dial")
            (Redial,                     "dial")
            (SendFixedMessage,           "sendMessage")
            (SendDTMF,                   "sendDTMFTones")
            (SendMessage,                "sendMessage")
            (PhoneListFavourites,        "showFavoriteContactList")
            (ListAllContact,             "showContactList")
            (AddVoiceTag,                "showAddVoiceTagUI")
            (CallHistory,                "showRecentCallList");

    VoiceMap<int, VR_String>::const_iterator itr = recogResultTypeMap.find(type);
    if (itr != recogResultTypeMap.end()) {
        VR_LOGI("xml op=[%s]", (itr->second).c_str());
        return itr->second;
    }
    else {
        VR_LOGI("not find xml op");
        return FAILED_STR;
    }
}

VR_String VR_TransformProtoToXml::TransformVrRecogResultConfirm(const VR_String& op, const int& type, const VR_String& errCode)
{
    VR_String retv = "";
    VR_String state = MsgTypeEnumToXML(type);

    if (!state.empty()) {
        const VR_String updateXml = "<action-result agent=\"%1%\""
                                               " op=\"%2%\" errcode=\"%3%\" >"
                                           "</action-result>";
        retv = (boost::format(updateXml) % op % state % errCode).str();
    }

    VR_LOGI("action-result xml=[%s]", retv.c_str());
    return retv;
}

VR_String VR_TransformProtoToXml::Transform(const VrRecogResultConfirm& resultProto)
{
    VrActionID  type = (VrActionID)resultProto.action_id();
    bool state = resultProto.processing_result();
    VR_String strState(state ? "true" : "false");
    VR_String result;
    static const VR_String mediaAgent("media");
    static const VR_String havcAgent("climate");
    static const VR_String phoneAgent("phone");
    static const VR_String appsAgent("apps");

    VR_LOGI("action type=[%d]", type);
    switch (type) {
    case AudioOFF :
    case AudioON :
    case SwitchSource :
    case PlayAlbum :
    case PlayAll :
    case PlayArtist :
    case PlayAudioBook :
    case PlayComposer :
    case PlayGenre :
    case PlayPlaylist :
    case PlayPodcast :
    case PlayPodcastItem :
    case PlaySong :
    case StartRadio :
    case ChangeHDSubChannel :
    case TuneFrequency :
    case TuneFMHDFrequency :
    case TuneFrequencyAM :
    case TuneFrequencyFM :
    case TuneFrequencySatellite :
    case TunePreset :
    case TuneSatelliteChannelName :
    case TuneSatelliteChannelNumber :
    case TuneDABChannelName :
    case TuneFMGenre :
    case TuneSatelliteGenre :
    case TuneFMChannelName :
        {
            result = TransformVrRecogResultConfirm(mediaAgent, type, strState);
        }
        break;
    case AutoAirConditionerON:
    case AutoAirConditionerOFF:
    case ACON:
    case ACOFF:
    case DualModeON:
    case DualModeOFF:
    case WiperDeicerON:
    case WiperDeicerOFF:
    case RearSeatAirConditionerON:
    case RearSeatAirConditionerOFF:
    case RearDefoggerON:
    case RearDefoggerOFF:
    case DecreaseTemperature:
    case IncreaseTemperature:
    case ChangeTemperature:
    case AFewWarmer:
    case AFewCooler:
    case TemperatureMax:
    case TemperatureMin:
    case ChangeFanSpeed:
    case FanSpeedMax:
    case FanSpeedMin:
    case BlowerModeFace:
    case BlowerModeFoot:
    case BlowerModeFaceAndFoot:
    case BlowerModeWindowAndFoot:
    case ClimateConciergeModeON:
    case HVACFrontScreen:
    case HVACRearScreen:
    case HVACSteeringScreen:
    case HVACFrontSeatScreen:
    case HVACRearSeatScreen:
    case HVACLexusConciergeScreen:
    case HVACSeatOperationScreen:
    case FrontSeatVentilationScreen:
    case RearSeatVentilationScreen:
        {
            result = TransformVrRecogResultConfirm(havcAgent, type, strState);
        }
        break;
    case Call:
    case CallWithNo:
    case CallRecent:
    case MUTEON:
    case MUTEOFF:
    case CallShortRing:
    case Redial:
    case SendFixedMessage:
    case SendDTMF:
    case SendMessage:
    case PhoneListFavourites:
    case ListAllContact:
    case AddVoiceTag:
    case CallHistory:
        {
            result = TransformVrRecogResultConfirm(phoneAgent, type, strState);
        }
        break;
    case StartApps:
        {
            result = TransformVrRecogResultConfirm(appsAgent, type, strState);
        }
        break;
    default:
        break;
    }
    VR_LOGI("result=[%s]", result.c_str());

    return result;
}

const VR_String &VR_TransformProtoToXml::MsgNaviTypeEnumToXML(const int& type)
{
    VR_LOGI("type=[%d]", type);
    static const VR_String& FAILED_STR = "";
    static const VoiceMap<int, VR_String>::type recogResultTypeMap = boost::assign::map_list_of
            // Navi action
            (NAVI_ACTION_queryDefaultInfo,                  "queryDefaultInfo")
            (NAVI_ACTION_queryStateInfo,                    "queryStateInfo")
            (NAVI_ACTION_queryCityInfo,                     "queryCityInfo")
            (NAVI_ACTION_queryStreetInfo,                   "queryStreetInfo")
            (NAVI_ACTION_queryHouseNoInfo,                  "queryAddressInfo")
            (NAVI_ACTION_queryPOICollectionNearby,          "queryPOICollectionNearby")
            (NAVI_ACTION_queryPOICollectionNearDestination, "queryPOICollectionNearDestination")
            (NAVI_ACTION_queryPOICollectionAlongRoute,      "queryPOICollectionAlongRoute")
            (NAVI_ACTION_queryRouteExist,                   "queryRouteExist")
            (NAVI_ACTION_queryUnpassedDestinationList,      "queryUnpassedDestinationList")
            (NAVI_ACTION_queryAllDestinationList,           "queryAllDestinationList")
            (NAVI_ACTION_queryPointType,                    "queryPointType")
            (NAVI_ACTION_queryNextDestination,              "queryNextDestination")
            (NAVI_ACTION_queryPointCollection,              "queryPointCollection")
            (NAVI_ACTION_queryDestinationHistory,           "queryDestinationHistory")
            (NAVI_ACTION_queryPointDataArea,                "queryPointDataArea")
            (NAVI_ACTION_ShowPoiIcon,                       "showPoiIcon")
            (NAVI_ACTION_hidePOIIcon,                       "hidePOIIcon")
            (NAVI_ACTION_addToRoute,                        "addToRoute")
            (NAVI_ACTION_deleteDestination,                 "deleteDestination")
            (NAVI_ACTION_calculateRoute,                    "calculateRoute")
            (NAVI_ACTION_queryStateInfoCN,                  "queryStateInfo")
            (NAVI_ACTION_queryCityInfoCN,                   "queryCityInfo")
            (NAVI_ACTION_queryDistrictInfoCN,               "queryDistrictInfo")
            (NAVI_ACTION_queryStreetInfoCN,                 "queryStreetInfo")
            (NAVI_ACTION_queryHouseNoInfoCN,                "queryAddressInfo")
            (NAVI_ACTION_queryAddressInfoCN,                "queryOneShotInfo")
            (NAVI_ACTION_queryPOINameNearby,                "queryPOIByName")
            ((navi::VoiceRecog::VrNaviActionType)ACTION_TYPE_SHOW_CALLDESTINATION_UI,     "callDestinationAssist");

    VoiceMap<int, VR_String>::const_iterator itr = recogResultTypeMap.find(type);
    if (itr != recogResultTypeMap.end()) {
        VR_LOGI("xml op=[%s]", (itr->second).c_str());
        return itr->second;
    }
    else {
        VR_LOGI("not find xml op");
        return FAILED_STR;
    }
}

const VR_String &VR_TransformProtoToXml::MsgSpecialTypeEnumToXML(const int& type)
{
    VR_LOGI("type=[%d]", type);
    static const VR_String& FAILED_STR = "";
    static const VoiceMap<int, VR_String>::type recogResultTypeMap = boost::assign::map_list_of
            // Navi action
            (ACTION_TYPE_SHOW_VOICE_SETTINGS_UI,                  "showVoiceSettingsUI")
            (ACTION_TYPE_SHOW_LEXUS_INSIDER_UI,                   "showLexusInsiderUI")
            (ACTION_TYPE_SHOW_TUTORIALS_UI,                       "showTutorialsUI")
            (ACTION_TYPE_SHOW_TRAINING_END_MSG,                   "showTrainingEndMsg");

    VoiceMap<int, VR_String>::const_iterator itr = recogResultTypeMap.find(type);
    if (itr != recogResultTypeMap.end()) {
        VR_LOGI("xml op=[%s]", (itr->second).c_str());
        return itr->second;
    }
    else {
        VR_LOGI("not find xml op");
        return FAILED_STR;
    }
}

VR_String VR_TransformProtoToXml::Transform(const int type, bool bState)
{
    VR_String strState(bState ? "true" : "false");
    VR_String result;
    // static const VR_String appsAgent("navi");

    VR_LOGI("action type=[%d]", type);
    switch (type) {
    case NAVI_ACTION_queryDefaultInfo :
    case NAVI_ACTION_queryStateInfo :
    case NAVI_ACTION_queryCityInfo :
    case NAVI_ACTION_queryStreetInfo :
    case NAVI_ACTION_queryHouseNoInfo :
    case NAVI_ACTION_queryPOICollectionNearby :
    case NAVI_ACTION_queryPOICollectionNearDestination :
    case NAVI_ACTION_queryPOICollectionAlongRoute :
    case NAVI_ACTION_queryRouteExist :
    case NAVI_ACTION_queryUnpassedDestinationList :
    case NAVI_ACTION_queryAllDestinationList :
    case NAVI_ACTION_queryPointType :
    case NAVI_ACTION_queryNextDestination :
    case NAVI_ACTION_queryPointCollection :
    case NAVI_ACTION_queryDestinationHistory :
    case NAVI_ACTION_queryPointDataArea :
    case NAVI_ACTION_ShowPoiIcon :
    case NAVI_ACTION_hidePOIIcon :
    case NAVI_ACTION_addToRoute :
    case NAVI_ACTION_deleteDestination :
    case NAVI_ACTION_calculateRoute :
    case NAVI_ACTION_queryStateInfoCN:
    case NAVI_ACTION_queryCityInfoCN:
    case NAVI_ACTION_queryDistrictInfoCN:
    case NAVI_ACTION_queryStreetInfoCN:
    case NAVI_ACTION_queryHouseNoInfoCN:
    case NAVI_ACTION_queryAddressInfoCN:
    case NAVI_ACTION_queryPOINameNearby:
    case ACTION_TYPE_SHOW_CALLDESTINATION_UI:
        {
            VR_String state = MsgNaviTypeEnumToXML(type);
            const VR_String updateXml = "<action-result agent=\"navi\""
                                                   " op=\"%1%\" errcode=\"%2%\" >"
                                               "</action-result>";
            result = (boost::format(updateXml) % state % strState).str();
        }
        break;
    default:
        break;
    }
    VR_LOGI("result=[%s]", result.c_str());

    return result;
}

VR_String VR_TransformProtoToXml::TransformSpecialType(const int type, bool bState)
{
    VR_String strState(bState ? "true" : "false");
    VR_String result;

    VR_LOGI("action type=[%d]", type);
    switch (type) {
    case ACTION_TYPE_SHOW_VOICE_SETTINGS_UI :
    case ACTION_TYPE_SHOW_LEXUS_INSIDER_UI :
    case ACTION_TYPE_SHOW_TUTORIALS_UI :
    case ACTION_TYPE_SHOW_TRAINING_END_MSG :
        {
            VR_String state = MsgSpecialTypeEnumToXML(type);
            const VR_String updateXml = "<action-result agent=\"help\""
                                                   " op=\"%1%\" errcode=\"%2%\" >"
                                               "</action-result>";
            result = (boost::format(updateXml) % state % strState).str();
        }
        break;
    case ACTION_TYPE_PLAY_BY_FILTER :
        {
            const VR_String updateXml = "<action-result agent=\"media\""
                                                   " op=\"playByFilter\" errcode=\"%1%\" >"
                                               "</action-result>";
            result = (boost::format(updateXml) % strState).str();
        }
        break;
    case ACTION_TYPE_SHOW_MEDIA_SCREEN :
        {
            const VR_String updateXml = "<action-result agent=\"media\""
                                                   " op=\"showMediaScreen\" errcode=\"%1%\" >"
                                               "</action-result>";
            result = (boost::format(updateXml) % strState).str();
        }
        break;
    default:
        break;
    }
    VR_LOGI("result=[%s]", result.c_str());

    return result;
}

VR_String VR_TransformProtoToXml::Transform(const VrAdditionalInfoRsp& additionalProto)
{
    VR_String  resultXml;
    VrRequestID type = (VrRequestID)additionalProto.response_id();

    VR_LOGI("action type=[%d]", type);
    switch (type) {
    case USBDeviceInfo :
        {
            resultXml = TransformUSBDeviceInfo(additionalProto);
        }
        break;
    case RadioBandState :
        {
            resultXml = TransformRadioBandState(additionalProto);
        }
        break;
    case CurFMChannelState :
        {
            resultXml = TransformCurFMChannelState(additionalProto);
        }
        break;
    case SpecifiedFMState :
        {
            resultXml = TransformSpecifiedFMState(additionalProto);
        }
        break;
    case PresetState :
        {
            resultXml = TransformPresetState(additionalProto);
        }
        break;
    case SpecifiedPresetState :
        {
            resultXml = TransformSpecifiedPresetState(additionalProto);
        }
        break;
    case RadioState :
        {
            resultXml = TransformRadioState(additionalProto);
        }
        break;
    case GenreStationState :
        {
            resultXml = TransformGenreStationState(additionalProto);
        }
        break;
    case AirConditionState:
    case ACState:
    case DualMode:
    case WiperDeicer:
    case RearDefogger:
    case ClimateConciergeMode:
        {
            resultXml = TransfromQueryStatus(additionalProto);
        }
        break;
    case Temperature:
        {
            resultXml = TransfromTemperature(additionalProto);
        }
        break;
    case TemperatureRange:
        {
            resultXml = TransfromTemperatureRang(additionalProto);
        }
        break;
    case FanSpeed:
        {
            resultXml = TransfromFanSpeed(additionalProto);
        }
        break;
    case BlowerMode:
        {
            resultXml = TransfromFanMode(additionalProto);
        }
        break;
    default:
        break;
    }
    return resultXml;
}

VR_String VR_TransformProtoToXml::Transform(const VrActiveInfoRsp& activeInfo)
{
    VR_String  resultXml;
    VrActiveInfoId  type = (VrActiveInfoId)activeInfo.response_id();

    switch (type) {
    case RadioActiveBand :
        {
            resultXml = TransformRadioActiveBandState(activeInfo);
        }
        break;
    case ValidTemperature :
        {
        }
        break;
    case CheckTemperature :
        {
        }
        break;
    case ValidFanSpeed :
        {
        }
        break;
    case CheckFanSpeed :
        {
        }
        break;
    case RecentCallList :
        {
            resultXml = TransformGetRenctCallListInfo(activeInfo);
        }
        break;
    case RecentIncomingCall :
        {
            resultXml = TransformGetInComingCallListInfo(activeInfo);
        }
        break;
    case RecentOutgoingCall :
        {
            resultXml = TransformGetOutGoingCallListInfo(activeInfo);
        }
        break;
    case RecentMessageList :
        {
            resultXml = TransformGetRenctMsgListInfo(activeInfo);
        }
        break;
    case RecentMessageListByType :
        {
            resultXml = TransformGetMsgListByTypeInfo(activeInfo);
        }
        break;
    default:
        break;
    }
    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformUSBDeviceInfo(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto)
{
    VR_LOGD_FUNC();
    const int playing = 0;
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"%1%\" errcode=\"%2%\">"
                                          "<%3%>%4%</%3%>"
                                      "</action-result>";
    VR_String result;
    VR_String op;
    VR_String errcode;
    // VR_String status;
    VR_String actionName;
    VR_String actionType;

    if (!additionalProto.has_usb_device_info_resp()) {
        errcode = "22";
    }
    else {
        VrUSBDeviceInfoResp infoProto = additionalProto.usb_device_info_resp();
        GetType type = (GetType)infoProto.response_id();
        switch (type) {
        case GetCurrentAlbum :
            {
                op = "getCurrentAlbum";
                actionType = "songid";
                VR_LOGI("playing %d", infoProto.play_status());
                if (infoProto.play_status() == playing) {
                    if (infoProto.has_song_id()) {
                        // status = "Success";
                        actionName = infoProto.song_id();
                        errcode = "0";
                        VR_LOGI("actionproName%s", infoProto.song_id().c_str());
                        VR_LOGI("actionName%s", actionName.c_str());
                    }
                    else {
                        VR_LOGI("--------actionproName%s", infoProto.song_id().c_str());
                        VR_LOGI("---------actionName%s", actionName.c_str());
                        // status = "Information Not Available";
                        actionName = "";
                        errcode = "21";
                    }
                }
                else {
                    errcode = "20";
                    // status = "No Song Playing";
                }
            }
            break;
        case GetCurrentArtist :
            {
                op = "getCurrentArtist";
                actionType = "songid";
                if (infoProto.play_status() == playing) {
                    if (infoProto.has_song_id()) {
                        // status = "Success";
                        actionName = infoProto.song_id();
                        errcode = "0";
                    }
                    else {
                        // status = "Information Not Available";
                        actionName = "";
                        errcode = "21";
                    }
                }
                else {
                    errcode = "20";
                    // status = "No Song Playing";
                }
            }
            break;
        case GetCurrentComposer :
            {
                op = "getCurrentComposer";
                actionType = "songid";
                if (infoProto.play_status() == playing) {
                    if (infoProto.has_song_id()) {
                        // status = "Success";
                        actionName = infoProto.song_id();
                        errcode = "0";

                    }
                    else {
                        // status = "Information Not Available";
                        actionName = "";
                        errcode = "21";
                    }
                }
                else {
                    errcode = "20";
                    // status = "No Song Playing";
                }
            }
            break;
        case GetCurrentGenre :
            {
                op = "getCurrentGenre";
                actionType = "songid";
                if (infoProto.play_status() == playing) {
                    if (infoProto.has_song_id()) {
                        // status = "Success";
                        actionName = infoProto.song_id();
                        errcode = "0";
                    }
                    else {
                        // status = "Information Not Available";
                        actionName = "";
                        errcode = "21";
                    }
                }
                else {
                    errcode = "20";
                    // status = "No Song Playing";
                }
            }
            break;
        default:
            break;
        }
    }
    result = (boost::format(startXml) % op % errcode % actionType % actionName).str();
    VR_LOGI("result=[%s]", result.c_str());
    return result;
}

VR_String VR_TransformProtoToXml::ChangeBand2Str(const VR_INT32& nBand)
{
    VR_LOGI("nBand=[%d]", nBand);
    VR_String str = "";
    switch (nBand) {
    case BandAM :
        {
            str = VR_VOICE_ACTION_MEDIA_RADIO_BAND_AM;
        }
        break;
    case BandFM :
        {
            str = VR_VOICE_ACTION_MEDIA_RADIO_BAND_FM;
        }
        break;
    case BandXM :
        {
            str = VR_VOICE_ACTION_MEDIA_RADIO_BAND_XM;
        }
        break;
    case BandDAB :
        {
            str = VR_VOICE_ACTION_MEDIA_RADIO_BAND_DAB;
        }
        break;
    default:
        break;
    }

    return str;
}

VR_String VR_TransformProtoToXml::TransformRadioBandState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"queryFreqBandStatus\" errcode=\"0\" >"
                                          "<band>%1%</band>"
                                          "<stationName>%2%</stationName>"
                                          "<frequency>%3%</frequency>"
                                          "<active>%4%</active>"
                                      "</action-result>";
    if (!additionalProto.has_radio_band_state_resp()) {
        resultXml = (boost::format(startXml) % "" % "" % "" % "").str();
    }
    else {
        VR_INT32 nBand         = additionalProto.radio_band_state_resp().radio_band_kind();
        VR_String stationName   = additionalProto.radio_band_state_resp().station_name();
        VR_INT32 frequency     = additionalProto.radio_band_state_resp().frequency();
        VR_INT32 active        = additionalProto.radio_band_state_resp().radio_band_state();
        VR_String strBand      = ChangeBand2Str(nBand);
        VR_String strActive    = active ? "False" : "True";
        VR_String strFrequency = "";
        if (BandFM == nBand) {
            VR_LOGI("FM");
            strFrequency = (boost::format("%1%") % ((float)frequency / 1000 / 1000)).str(); // trans from Hz to MHz
        }
        else if (BandAM == nBand) {
            VR_LOGI("AM");
            strFrequency = (boost::format("%1%") % (frequency / 1000)).str(); // trans from Hz to KHz
        }

        resultXml = (boost::format(startXml) % strBand % VR_CommonTool::Encode(stationName) % strFrequency % strActive).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformCurFMChannelState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"queryCurrentHDSubchannelAvailable\" errcode=\"0\" >"
                                          "<frequency>%1%</frequency>"
                                          "<available>%2%</available>"
                                      "</action-result>";

    if (!additionalProto.has_cur_fm_channel_state_resp()) {
        resultXml = (boost::format(startXml) % "" % "").str();
    }
    else {
        VR_INT32 frequency     = additionalProto.cur_fm_channel_state_resp().frequency();
        VR_INT32 available     = additionalProto.cur_fm_channel_state_resp().multicast_channel_state();
        VR_String stravailable = available ? "False" : "True";
        VR_String strFrequency = (boost::format("%1%") % ((float)frequency / 100)).str();

        resultXml = (boost::format(startXml) % strFrequency % stravailable).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformSpecifiedFMState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"queryHDSubchannelAvailable\" errcode=\"0\" >"
                                          "<available>%1%</available>"
                                      "</action-result>";
    if (!additionalProto.has_specified_fm_state_resp()) {
        resultXml = (boost::format(startXml) % "").str();
    }
    else {
        VR_INT32 available     = additionalProto.specified_fm_state_resp().radio_band_state();
        VR_String stravailable = available ? "False" : "True";

        resultXml = (boost::format(startXml) % stravailable).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformPresetState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"queryPresetsAvailable\" errcode=\"0\" >"
                                          "<available>%1%</available>"
                                      "</action-result>";

    if (!additionalProto.has_preset_state_resp()) {
        resultXml = (boost::format(startXml) % "False").str();
    }
    else {
        VR_INT32 available     = additionalProto.preset_state_resp().preset_state();
        VR_String stravailable = available ? "False" : "True";

        resultXml = (boost::format(startXml) % stravailable).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformSpecifiedPresetState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"queryPresetDefined\" errcode=\"0\" >"
                                          "<defined>%1%</defined>"
                                      "</action-result>";

    if (!additionalProto.has_specified_preset_state_resp()) {
        resultXml = (boost::format(startXml) % "False").str();
    }
    else {
        VR_INT32 available     = additionalProto.specified_preset_state_resp().preset_num_state();
        VR_String stravailable = available ? "False" : "True";

        resultXml = (boost::format(startXml) % stravailable).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformRadioState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"queryRadioStatus\" errcode=\"0\" >"
                                          "<band>%1%</band>"
                                          "<stationName>%2%</stationName>"
                                          "<frequency>%3%</frequency>"
                                          "<active>%4%</active>"
                                      "</action-result>";

    if (!additionalProto.has_radio_state_resp()) {
        resultXml = (boost::format(startXml) % "" % "" % "" % "").str();
    }
    else {
        VR_INT32 nBand         = additionalProto.radio_state_resp().radio_band_kind();
        VR_String stationName   = additionalProto.radio_state_resp().station_name();
        VR_INT32 frequency     = additionalProto.radio_state_resp().frequency();
        VR_INT32 active        = additionalProto.radio_state_resp().radio_band_state();
        VR_String strBand      = ChangeBand2Str(nBand);
        VR_String strActive    = active ? "False" : "True";
        VR_String strFrequency = "";
        if (BandFM == nBand) {
            strFrequency = (boost::format("%1%") % ((float)frequency / 1000 / 1000)).str(); // trans from Hz to MHz
        }
        else if (BandAM == nBand) {
            strFrequency = (boost::format("%1%") % (frequency / 1000)).str(); // trans from Hz to KHz
        }

        resultXml = (boost::format(startXml) % strBand % VR_CommonTool::Encode(stationName) % strFrequency % strActive).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformGenreStationState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"queryFMGenreAvailable\" errcode=\"0\" >"
                                          "<available>%1%</available>"
                                      "</action-result>";

    if (!additionalProto.has_genre_station_state_resp()) {
        resultXml = (boost::format(startXml) % "").str();
    }
    else {
        VR_INT32 available     = additionalProto.genre_station_state_resp().genre_state();
        VR_String stravailable = available ? "False" : "True";

        resultXml = (boost::format(startXml) % stravailable).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformRadioActiveBandState(const navi::VoiceRecog::VrActiveInfoRsp& activeInfo)
{
    VR_String  resultXml = "";
    const VR_String startXml = "<action-result agent=\"media\""
                                          " op=\"queryActiveFrequencyBand\" errcode=\"0\" >"
                                          "<band>%1%</band>"
                                      "</action-result>";
    if (!activeInfo.has_radio_active_band_resp()) {
        resultXml = (boost::format(startXml) % "").str();
    }
    else {
        VR_INT32 nBand         = activeInfo.radio_active_band_resp().radio_band_kind();
        VR_String strBand      = ChangeBand2Str(nBand);

        resultXml = (boost::format(startXml) % strBand).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VR_String VR_TransformProtoToXml::TransfromQueryStatus(const VrAdditionalInfoRsp &additionalProto)
{
    VR_String  resultXml = "";
    VR_String  startXml =  "<action-result agent=\"climate\""
                                       " op=\"queryStatus\" errcode=\"0\" >"
                                       "<type>%1%</type>"
                                       "<status>%2%</status>"
                                   "</action-result>";
    static const VoiceMap<VrRequestID, std::string>::type requestMap = boost::assign::map_list_of
            (AirConditionState, "AUTO_AIRCONDITION")
            (ACState, "AC")
            (DualMode, "DUAL_MODE")
            (WiperDeicer, "WIPER_DEICER")
            (RearDefogger, "REAR_DEFOGGER")
            (ClimateConciergeMode, "CONCIERGE_MODE");
    VrRequestID requestId = (VrRequestID)additionalProto.response_id();
    int nState(-1);
    VR_String  strType = "";
    VoiceMap<VrRequestID, std::string>::type::const_iterator citer;
    citer = requestMap.find(requestId);
    if (citer == requestMap.end()) {
        return resultXml;
    }
    strType = citer->second;
    switch (requestId) {
    case AirConditionState:
        {
            const VrAirConditionStateResp& airConState = additionalProto.air_condition_state_resp();
            nState = airConState.air_condition_state();
            break;
        }
    case ACState:
        {
            const VrACStateResp& acState = additionalProto.ac_state_resp();
            nState = acState.ac_state();
            break;
        }
    case DualMode:
        {
            const VrDualModeResp& dualState = additionalProto.dual_mode_resp();
            nState = dualState.dual_mode_state();
            break;
        }
    case WiperDeicer:
        {
            const VrWiperDeicerResp& wiperState = additionalProto.wiper_deicer_resp();
            nState = wiperState.wiper_deicer_state();
            break;
        }
    case RearDefogger:
        {
            const VrRearDefoggerStateResp& rearDefState = additionalProto.rear_defogger_state_resp();
            nState = rearDefState.rear_defogger_state();
            break;
        }
    case ClimateConciergeMode:
        {
            const VrClimateConciergeModeResp& climateState = additionalProto.climate_concierge_mode_resp();
            nState = climateState.climate_concierge_mode();
            break;
        }
    default: break;
    }
    VR_String  strState = (nState == 1) ? "ON" : "OFF";
    resultXml = (boost::format(startXml) % strType % strState).str();
    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransfromTemperature(const VrAdditionalInfoRsp &additionalProto)
{
    VR_String  resultXml = "";
    VR_String  startXml =  "<action-result agent=\"climate\""
                                       " op=\"queryTemperature\" errcode=\"0\" >"
                                       "<degree>%1%</degree>"
                                   "</action-result>";

    if (!additionalProto.has_temperature_resp()) { // if i get a handle error... set the degree to error. make fault handler.
        resultXml = (boost::format(startXml) % "error").str(); // this for suntect engine.
    }
    else {
        const VrTemperatureResp& temperature = additionalProto.temperature_resp();
        int nTempStep = temperature.temperature_state();
        float fTemp(VR_TEMP_INVALID);
        VR_String strTemp = "";
        if (nTempStep >= VR_CEL_STEP_START && nTempStep <= VR_CEL_STEP_END) {
            fTemp = VR_CELSIUS + (nTempStep - VR_CEL_STEP_START) * VR_CEL_RATE;
        }
        else if (nTempStep >= VR_FAH_STEP_START && nTempStep <= VR_FAH_STEP_END) {
            fTemp = VR_FAHRENHEIT + (nTempStep - VR_FAH_STEP_START) * VR_TEMP_FLOAT;
        }
        else if (nTempStep == VR_TEMP_MIN) {
            strTemp = "MIN";
        }
        else if (nTempStep == VR_TEMP_MAX) {
            strTemp = "MAX";
        }
        else {
        }
        if (fTemp != VR_TEMP_INVALID) {
            strTemp = (boost::format("%1%") % fTemp).str();
        }
        // in meu-dcu , we think strTemp is empty , we set it "error"
        if (strTemp.empty()) {
            strTemp = "error";
        }
        resultXml = (boost::format(startXml) % strTemp).str();
    }
    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransfromTemperatureRang(const VrAdditionalInfoRsp &additionalProto)
{
    VR_String  resultXml = "";
    VR_String  startXml =  "<action-result agent=\"climate\""
                                       " op=\"queryTemperatureRange\" errcode=\"0\" >"
                                       "<mindegree>%1%</mindegree>"
                                       "<maxdegree>%2%</maxdegree>"
                                   "</action-result>";

    if (!additionalProto.has_temperature_range_resp()) {
        resultXml = (boost::format(startXml) % "" % "").str();
    }
    else {
        const VrTemperatureRangeResp& tempRang = additionalProto.temperature_range_resp();
        int  nLowTemp = tempRang.min_temperature();
        int  nHighTemp = tempRang.max_temperature();
        resultXml = (boost::format(startXml) % nLowTemp % nHighTemp).str();
    }
    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransfromFanSpeed(const VrAdditionalInfoRsp &additionalProto)
{
    VR_String  resultXml = "";
    VR_String  startXml =  "<action-result agent=\"climate\""
                                       " op=\"queryFanSpeed\" errcode=\"0\" >"
                                       "<degree>%1%</degree>"
                                   "</action-result>";

    if (!additionalProto.has_fan_speed_state_resp()) {
        resultXml = (boost::format(startXml) % "error").str();    // default fault handler
    }
    else {
        const VrFanSpeedStateResp& fanSpeed = additionalProto.fan_speed_state_resp();
        int nFanSpeed = fanSpeed.fan_speed_state();
        resultXml = (boost::format(startXml) % nFanSpeed).str();
    }
    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransfromFanMode(const VrAdditionalInfoRsp &additionalProto)
{
    VR_String resultXml = "";
    VR_String startXml =  "<action-result agent=\"climate\""
                                       " op=\"getFanMode\" errcode=\"0\" >"
                                       "<mode>%1%</mode>"
                                 "</action-result>";
    VR_String strMode = "";
    if (!additionalProto.has_blower_mode_resp()) {
        resultXml = (boost::format(startXml) % "error").str();
    }
    else {
        const VrBlowerModeResp& modeResp = additionalProto.blower_mode_resp();
        int nBlowerMode = modeResp.blower_mode_state();
        if (nBlowerMode == VR_BLOWER_MODE_FACE) {
            strMode = "FACE";
        }
        else if (nBlowerMode == VR_BLOWER_MODE_FOOT) {
            strMode = "FOOT";
        }
        else if (nBlowerMode == VR_BLOWER_MODE_FACE_FOOT) {
            strMode = "FACE TO FOOT";
        }
        else if (nBlowerMode == VR_BLOWER_MODE_WIND_FOOT) {
            strMode = "WINDOW TO FOOT";
        }
        else {
        }
        // in meu-dcu , we think strTemp is empty , we set it "error"
        if (strMode.empty()) {
            strMode = "error";
        }
        resultXml = (boost::format(startXml) % strMode).str();
    }
    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_TransformProtoToXml::TransformGetRenctCallListInfo(const VrActiveInfoRsp &activeInfo)
{
    const char* const RECENT_CALL_XML  = "<action-result agent=\"phone\" op=\"queryRecentCallList\" errcode=\"0\">"\
                                             "<list>""</list>"\
                                         "</action-result>";
    const char* const RECENT_CALL_NULL = "<action-result agent=\"phone\" op=\"queryRecentCallList\" errcode=\"ERR_PHONE_NO_RECENT_CALL_LIST\">"\
                                         "</action-result>";

    pugi::xml_document doc;
    doc.load_string(RECENT_CALL_XML);

    VrRecentCallListRsp recentCallList = activeInfo.recent_call_list_rsp();

    if (recentCallList.has_phone_avaialble()) {
        pugi::xml_node category = doc.child("action-result").child("list");
        int count = recentCallList.phone_info_size();

        for (int i = 0; i < count; ++i) {
            VrPhoneInfo pInfo = recentCallList.phone_info(i);
            pugi::xml_node phoneInfo = category.append_child("phoneInfo");

            phoneInfo.append_attribute("contactId") = pInfo.contact_id();

            VR_String fName = pInfo.first_name();
            VR_String lName = pInfo.last_name();
            VR_String name  = fName + lName;

            pugi::xml_attribute contactName = phoneInfo.append_attribute("contactName");
            pugi::xml_attribute phoneKind   = phoneInfo.append_attribute("phoneKind");
            pugi::xml_attribute phoneType   = phoneInfo.append_attribute("phoneType");
            pugi::xml_attribute number      = phoneInfo.append_attribute("number");

            contactName.set_value(name.c_str());
            phoneKind.set_value(pInfo.phone_kind());
            phoneType.set_value(pInfo.phone_type());
            number.set_value(pInfo.phone_number().c_str());
        }
        return RECENT_CALL_XML;
    }
    else {
        return RECENT_CALL_NULL;
    }
}

VR_String VR_TransformProtoToXml::TransformGetInComingCallListInfo(const VrActiveInfoRsp &activeInfo)
{
    const char* const INCOMING_XML  = "<action-result agent=\"phone\" op=\"queryIncomingCall\" errcode=\"0\">"\
                                          "<list>""</list>"\
                                      "</action-result>";
    const char* const INCOMING_NULL = "<action-result agent=\"phone\" op=\"queryIncomingCall\" errcode=\"ERR_PHONE_NO_INCOMMING\">"\
                                      "</action-result>";

     pugi::xml_document doc;
     doc.load_string(INCOMING_XML);


     VrRecentIncomingCallRsp recentCallList = activeInfo.incoming_call_list_rsp();

     if (recentCallList.has_phone_avaialble()) {
         pugi::xml_node category = doc.child("action-result").child("list");
         VrPhoneInfo pInfo = recentCallList.phone_info();

         pugi::xml_node phoneInfo = category.append_child("phoneInfo");

         phoneInfo.append_attribute("contactId") = int2str(pInfo.contact_id());

         VR_String fName = pInfo.first_name();
         VR_String lName = pInfo.last_name();
         VR_String name = fName + lName;

         pugi::xml_attribute contactName = phoneInfo.append_attribute("contactName");
         pugi::xml_attribute phoneKind   = phoneInfo.append_attribute("phoneKind");
         pugi::xml_attribute phoneType   = phoneInfo.append_attribute("phoneType");
         pugi::xml_attribute number      = phoneInfo.append_attribute("number");

         contactName.set_value(name.c_str());
         phoneKind.set_value(pInfo.phone_kind());
         phoneType.set_value(pInfo.phone_type());
         number.set_value(pInfo.phone_number().c_str());

         return INCOMING_XML;
     }
     else {
         return INCOMING_NULL;
     }
}

VR_String VR_TransformProtoToXml::TransformGetOutGoingCallListInfo(const VrActiveInfoRsp &activeInfo)
{
    const char* const OUTGOING_XML  = "<action-result agent=\"phone\" op=\"queryOutgoingCall\" errcode=\"0\">"\
                                          "<list>""</list>"\
                                      "</action-result>";
    const char* const OUTGOING_NULL = "<action-result agent=\"phone\" op=\"queryOutgoingCall\" errcode=\"ERR_PHONE_NO_OUTGOING\">"\
                                      "</action-result>";

    pugi::xml_document doc;
    doc.load_string(OUTGOING_XML);


    VrRecentOutgoingCallRsp recentCallList = activeInfo.outgoing_call_list_rsp();

    if (recentCallList.has_phone_avaialble()) {
        pugi::xml_node category = doc.child("action-result").child("list");
        VrPhoneInfo pInfo = recentCallList.phone_info();

        pugi::xml_node phoneInfo = category.append_child("phoneInfo");

        phoneInfo.append_attribute("contactId") = int2str(pInfo.contact_id());

        VR_String fName = pInfo.first_name();
        VR_String lName = pInfo.last_name();
        VR_String name = fName + lName;

        pugi::xml_attribute contactName = phoneInfo.append_attribute("contactName");
        pugi::xml_attribute phoneKind   = phoneInfo.append_attribute("phoneKind");
        pugi::xml_attribute phoneType   = phoneInfo.append_attribute("phoneType");
        pugi::xml_attribute number      = phoneInfo.append_attribute("number");

        contactName.set_value(name.c_str());
        phoneKind.set_value(pInfo.phone_kind());
        phoneType.set_value(pInfo.phone_type());
        number.set_value(pInfo.phone_number().c_str());

        return OUTGOING_XML;
    }
    else {
        return OUTGOING_NULL;
    }
}

VR_String VR_TransformProtoToXml::TransformGetRenctMsgListInfo(const VrActiveInfoRsp &activeInfo)
{
    const char* const RECENT_MSG_XML  = "<action-result agent=\"phone\" op=\"queryRecentMessage\" errcode=\"0\">"\
                                            "<list>""</list>"\
                                        "</action-result>";
    const char* const RECENT_MSG_NULL = "<action-result agent=\"phone\" op=\"queryRecentMessage\" errcode=\"ERR_PHONE_NO_MESSAGE_LIST\">"\
                                        "</action-result>";

    pugi::xml_document doc;
    doc.load_string(RECENT_MSG_XML);

    VrRecentMessageListRsp recentMsgList = activeInfo.recent_msg_list_rsp();
    if (recentMsgList.has_msg_avaialble()) {
        pugi::xml_node category = doc.child("action-result").child("list");
        int count = recentMsgList.message_info_size();

        for (int i = 0; i < count; ++i) {
            VrMessageInfo mInfo = recentMsgList.message_info(i);
            pugi::xml_node phoneInfo = category.append_child("messageInfo");

            pugi::xml_attribute instanceId  = phoneInfo.append_attribute("instanceId");
            pugi::xml_attribute messageId   = phoneInfo.append_attribute("messageId");
            pugi::xml_attribute phoneNumber = phoneInfo.append_attribute("phoneNumber");
            pugi::xml_attribute sender      = phoneInfo.append_attribute("sender");
            pugi::xml_attribute phoneType   = phoneInfo.append_attribute("phoneType");
            pugi::xml_attribute timestamp   = phoneInfo.append_attribute("timestamp");
            pugi::xml_attribute status      = phoneInfo.append_attribute("status");

            instanceId.set_value(mInfo.instance_id());
            messageId.set_value(mInfo.message_id().c_str());
            phoneNumber.set_value(mInfo.address().c_str());
            sender.set_value(mInfo.sender_name().c_str());
            phoneType.set_value(mInfo.phone_type());
            timestamp.set_value(mInfo.time().c_str());
            status.set_value(mInfo.status());
        }
        return RECENT_MSG_XML;
    }
    else {
        return RECENT_MSG_NULL;
    }
}

VR_String VR_TransformProtoToXml::TransformGetMsgListByTypeInfo(const VrActiveInfoRsp &activeInfo)
{
    const char* const MSG_TYPE_XML  = "<action-result agent=\"phone\" op=\"queryRecentMessageByType\" errcode =\"0\">"\
                                          "<list>""</list>"\
                                      "</action-result>";
    const char* const MSG_TYPE_NULL = "<action-result agent=\"phone\" op=\"queryRecentMessageByType\" errcode=\"ERR_PHONE_NO_MESSAGE_LIST\">"\
                                      "</action-result>";

    pugi::xml_document doc;
    doc.load_string(MSG_TYPE_XML);


    VrRecentMessageListByTypeRsp recentMsgList = activeInfo.recent_type_msg_list_rsp();
    if (recentMsgList.has_msg_avaialble()) {
        pugi::xml_node category = doc.child("action-result").child("list");
        int count = recentMsgList.message_info_size();

        for (int i = 0; i < count; ++i) {
            VrMessageInfo mInfo = recentMsgList.message_info(i);
            pugi::xml_node phoneInfo = category.append_child("messageInfo");

            pugi::xml_attribute instanceId  = phoneInfo.append_attribute("instanceId");
            pugi::xml_attribute messageId   = phoneInfo.append_attribute("messageId");
            pugi::xml_attribute phoneNumber = phoneInfo.append_attribute("phoneNumber");
            pugi::xml_attribute sender      = phoneInfo.append_attribute("sender");
            pugi::xml_attribute phoneType   = phoneInfo.append_attribute("phoneType");
            pugi::xml_attribute timestamp   = phoneInfo.append_attribute("timestamp");
            pugi::xml_attribute status      = phoneInfo.append_attribute("status");

            instanceId.set_value(mInfo.instance_id());
            messageId.set_value(mInfo.message_id().c_str());
            phoneNumber.set_value(mInfo.address().c_str());
            sender.set_value(mInfo.sender_name().c_str());
            phoneType.set_value(mInfo.phone_type());
            timestamp.set_value(mInfo.time().c_str());
            status.set_value(mInfo.status());
        }

        return MSG_TYPE_XML;
    }
    else {
        return MSG_TYPE_NULL;
    }
}

VR_String VR_TransformProtoToXml::Transform(const VrMetMMStatus &metMMStatus)
{
    VR_String  resultXml = "";
    VR_String  status = "";
    const VR_String vrStateXml = "<display agent=\"Common\" content=\"VRState\">"
                                             "<state>%1%</state>"
                                             "<tag>%2%</tag>"
                                             "<prompt>%3%</prompt>"
                                             "<result>%4%</result>"
                                         "</display>";

    VR_INT32 nStatus = metMMStatus.status();
    VR_String tag = metMMStatus.vr_fb_title();
    BL_String tag_Dst;
    size_t tag_Strlen = tag.length();
    bool bResult = VR_CommonTool::ParseUCS2_BE(tag.c_str(), static_cast<INT>(tag_Strlen), tag_Dst);
    if (!bResult) {
        tag_Dst = "";
    }

    VR_String result = metMMStatus.vr_fb_text();
    BL_String result_Dst;
    size_t result_Strlen = result.length();
    bResult = VR_CommonTool::ParseUCS2_BE(result.c_str(), static_cast<INT>(result_Strlen), result_Dst);
    if (!bResult) {
        result_Dst = "";
    }

    VR_String prompt = metMMStatus.vr_guide_text();
    BL_String prompt_Dst;
    size_t prompt_Strlen = prompt.length();
    bResult = VR_CommonTool::ParseUCS2_BE(prompt.c_str(), static_cast<INT>(prompt_Strlen), prompt_Dst);
    if (!bResult) {
        prompt_Dst = "";
    }

    switch (nStatus) {
    case Wait :
        {
            status = "quit";
        }
        break;
    case Operate :
        {
            status = "promptPlaying";
        }
        break;
    case Receive :
        {
            status = "listening";
        }
        break;
    case Process :
        {
            status = "processing";
        }
        break;
    default:
        {
            status = "idle";
        }
        break;
    }

    resultXml = (boost::format(vrStateXml) % status % tag_Dst.GetString() % prompt_Dst.GetString() % result_Dst.GetString()).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

/* EOF */

