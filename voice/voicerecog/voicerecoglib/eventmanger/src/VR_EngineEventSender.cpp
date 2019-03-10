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
 * @file VR_EngineEventSender.cpp
 * @brief Declaration file of class VR_EngineEventSender.
 *
 * This file includes the declaration of class VR_EngineEventSender.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"
#include "VR_ConfigureIF.h"
#include "VR_EngineEventSender.h"

#include <boost/assign/list_of.hpp>
#include <memory>

#include "pugixml.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"

#include "VR_TransformXmlToXml.h"
#include "VR_TransformXmlToProto.h"
#include "VR_ActionSenderDef.h"
#include "navi/Voice/VoiceRecog/VrNotifyRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyEngineStatus.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyMMVr.pb.h"
#include "MEM_map.h"
#include "BL_StringLibIF.h"
#include "BL_String.h"
#include "VR_CommonTool.h"
#include "VR_XmlStrParse.h"
#include "VR_JsonParse.h"

using namespace navi::VoiceRecog;

struct xml_string_writer : pugi::xml_writer // xml to string
{
    std::string result; // string store xml-string
    virtual void write(const void* data, size_t size) // write xml to string
    {
        result.assign(static_cast<const char*>(data), size);
    }
};

typedef std::pair<std::string, std::string>Msg_Key;
typedef VoiceMap<Msg_Key, VR_EngineEventSender::Msg_type>::type Msg_Map;
const Msg_Map  msg_Map = boost::assign::map_list_of
    /// music
//#if defined(VR_DM_TARGET_VRUNIT)
//    (std::make_pair("media", "turnAudioOff"),                      VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "turnAudioOn"),                       VR_EngineEventSender::MSG_DE_DM)
//#else
//    (std::make_pair("media", "changeSource"),                      VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("media", "turnAudioOff"),                      VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("media", "turnAudioOn"),                       VR_EngineEventSender::MSG_DE_FC)
//#endif
//    (std::make_pair("media", "changeSourceByName"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "play"),                              VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "playPodcastEpisode"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "playByFilter"),                      VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "showMediaScreen"),                   VR_EngineEventSender::MSG_DE_DM)

//    (std::make_pair("media", "getCurrentAlbum"),                   VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "getCurrentArtist"),                  VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "getCurrentComposer"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "getCurrentGenre"),                   VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "grammar"),                           VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "active"),                            VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "disactive"),                         VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "querySourceStatus"),                 VR_EngineEventSender::MSG_DE_DM)
//    /// apps
//    (std::make_pair("apps", "launchApp"),                          VR_EngineEventSender::MSG_DE_DM)
//    /// radio
//    (std::make_pair("media", "startRadio"),                        VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "changeHDSubChannel"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "tuneFrequency"),                     VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "tuneFMHDFrequency"),                 VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "tuneLastSatellite"),                 VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "tuneSatelliteChannelNumber"),        VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "tuneLastAM"),                        VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "tuneLastFM"),                        VR_EngineEventSender::MSG_DE_DM)

//    (std::make_pair("media", "playByGenre"),                       VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "playByName"),                        VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "playByPreset"),                      VR_EngineEventSender::MSG_DE_DM)

//    (std::make_pair("media", "queryActiveFrequencyBand"),          VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "queryFreqBandStatus"),               VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "queryRadioStatus"),                  VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "queryHDSubchannelAvailable"),        VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "queryCurrentHDSubchannelAvailable"), VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "queryPresetDefined"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "queryPresetsAvailable"),             VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("media", "queryFMGenreAvailable"),             VR_EngineEventSender::MSG_DE_DM)
//    /// phone avc-lan
//#if defined(VR_DM_TARGET_VRUNIT)
//    (std::make_pair("phone", "dial"),                              VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "showRecentCallList"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "sendDTMFTones"),                     VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "showContactList"),                   VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "showFavoriteContactList"),           VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "sendMessage"),                       VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "muteCall"),                          VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "muteOff"),                           VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "showAddVoiceTagUI"),                 VR_EngineEventSender::MSG_DE_DM)
//#else
//    /// phone
//    (std::make_pair("phone", "dial"),                              VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "showRecentCallList"),                VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "sendDTMFTones"),                     VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "showContactList"),                   VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "showFavoriteContactList"),           VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "sendMessage"),                       VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "muteCall"),                          VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "muteOff"),                           VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "showAddVoiceTagUI"),                 VR_EngineEventSender::MSG_DE_FC)
//#endif
//    (std::make_pair("phone", "queryIncomingCall"),                 VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "queryOutgoingCall"),                 VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "queryRecentCallList"),               VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "queryRecentMessage"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "queryRecentMessageByType"),          VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "queryVehicleinMotion"),              VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "grammar"),                           VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "active"),                            VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "disactive"),                         VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "getMessageDetail"),                  VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("phone", "getMsgDetailStatusNoChange"),        VR_EngineEventSender::MSG_DE_FC)
//    /// grammar request
//    (std::make_pair("media", "requestGrammar"),                    VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("phone", "requestGrammar"),                    VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("tuner", "requestGrammar"),                    VR_EngineEventSender::MSG_DE_DM)
//    /// climate
//    (std::make_pair("climate", "showScreen"),                      VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "queryStatus"),                     VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "turn"),                            VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "queryTemperature"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "queryTemperatureRange"),           VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "controlTemperature"),              VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "changeTemperature"),               VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "queryFanSpeed"),                   VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "queryFanSpeedRange"),              VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "changeFanSpeed"),                  VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "changeFanMode"),                   VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "getFanMode"),                      VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "getValidTemperature"),             VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "checkTemperature"),                VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "checkFanSpeed"),                   VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("climate", "getValidFanSpeed"),                VR_EngineEventSender::MSG_DE_DM)
//    /// prompt
   (std::make_pair("prompt", "playTts"),                          VR_EngineEventSender::MSG_DE_DM)
   (std::make_pair("prompt", "stopTts"),                          VR_EngineEventSender::MSG_DE_DM)
   (std::make_pair("prompt", "playBeep"),                         VR_EngineEventSender::MSG_DE_DM)
   (std::make_pair("prompt", "stopBeep"),                         VR_EngineEventSender::MSG_DE_DM)
   (std::make_pair("prompt", "playTtsList"),                      VR_EngineEventSender::MSG_DE_DM)
   (std::make_pair("prompt", "stopTtsList"),                      VR_EngineEventSender::MSG_DE_DM)
//    /// vehicle
//    (std::make_pair("help", "queryVehicleinMotion"),               VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("help", "showVoiceSettingsUI"),                VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("help", "showLexusInsiderUI"),                 VR_EngineEventSender::MSG_DE_FC)
//    (std::make_pair("help", "showTutorialsUI"),                    VR_EngineEventSender::MSG_DE_FC)
//    /// smartagent
//    (std::make_pair("smartagent", "setPromptLevel"),               VR_EngineEventSender::MSG_DE_DM)
//    /// navi msg
//    (std::make_pair("navi", "queryStateInfo"),                     VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryCityInfo"),                      VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryDistrictInfo"),                  VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryStreetInfo"),                    VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryAddressInfo"),                   VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryOneShotInfo"),                   VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryShowingPOIIconNumber"),          VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryPOICollectionNearby"),           VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryPOICollectionNearDestination"),  VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryPOICollectionAlongRoute"),       VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryPointCollection"),               VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryDestinationHistory"),            VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryRouteExist"),                    VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryAllDestinationList"),            VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryUnpassedDestinationList"),       VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryPointType"),                     VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryNextDestination"),               VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryPointDataArea"),                 VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "showPOIIcon"),                        VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "hidePOIIcon"),                        VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "addToRoute"),                         VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "deleteDestination"),                  VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "calculateRoute"),                     VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryCurrentPosition"),               VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "requestDefaultInfo"),                 VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "queryPOIByName"),                     VR_EngineEventSender::MSG_DE_DM)
//    /// navi gbook
//    (std::make_pair("navi", "queryDestinationAssistAvailable"),    VR_EngineEventSender::MSG_DE_DM)
//    (std::make_pair("navi", "callDestinationAssist"),              VR_EngineEventSender::MSG_DE_FC)
//    /// training end
//    (std::make_pair("help", "showTrainingEndMsg"),                 VR_EngineEventSender::MSG_DE_FC)
//    /// gbook communication
//    (std::make_pair("communication", "queryGBookStatus"),          VR_EngineEventSender::MSG_DE_DM)
    (std::make_pair("destatus", "notifyResourceState"),            VR_EngineEventSender::MSG_DE_DM)
//    /// start finish notify
//    (std::make_pair("destatus", "notifyStartFinish"),              VR_EngineEventSender::MSG_DE_DM)
//    /// gbook center url
//    (std::make_pair("communication", "queryCenterVRURL"),          VR_EngineEventSender::MSG_DE_DM)
    // amazon
    (std::make_pair("system", "awake"),                            VR_EngineEventSender::MSG_DE_DM)
    (std::make_pair("system", "quit"),                             VR_EngineEventSender::MSG_DE_DM)
    (std::make_pair("system", "awakewithoutstart"),                VR_EngineEventSender::MSG_DE_DM)
    (std::make_pair("system", "DirveScore"),                VR_EngineEventSender::MSG_DE_DM)
    (std::make_pair("vehicle", "sunroofControl"),                  VR_EngineEventSender::MSG_DE_FC)
    (std::make_pair("carManual", "showScreen"),                    VR_EngineEventSender::MSG_DE_FC)
    (std::make_pair("phone", "showPhoneCall"),                     VR_EngineEventSender::MSG_DE_FC)
    (std::make_pair("navi", "showPOI"),                            VR_EngineEventSender::MSG_DE_FC)
    (std::make_pair("phone", "dial"),                              VR_EngineEventSender::MSG_DE_FC);

VR_EngineEventSender::VR_EngineEventSender()
{
    VR_LOGD_FUNC();
}

void VR_EngineEventSender::SendEvent(int taskId, int id, const VR_String &msg)
{
    VR_LOGI("taskId=[%d],id=[%d],msg=[%s]", taskId, id, msg.c_str());
    pugi::xml_document doc;
    doc.load_string(msg.c_str());
//    pugi::xml_node msgNode = doc.select_node("//display").node();
//    if (msgNode != NULL) {
//        SendNotifyEvent(taskId, msg);
//        return;
//    }

    pugi::xml_node msgNode = doc.select_node("//event-result").node();
    if (msgNode != NULL) {
        pugi::xpath_node node;
        node = doc.select_node("/event-result[@name='startAgent']");
        if (node == NULL) {
            SendReplyEvent(id, msg);
            return;
        }
    }

    msgNode = doc.select_node("//action").node();
    if (msgNode != NULL) {
        if (SendReqEvent(taskId, id, msg)) {
            return;
        }
    }

    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("Alexa");
    emVriAutoProxyType emType = VriAutoProxyType_Unknown;
    VR_String strXml = VR_TransformXmlToXml().Transform(id, msg, emType);
    if (strXml.empty()) {
        return;
    }
    if (VriAutoProxyType_System_SetState == emType) {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVRUpdateData);
        noitfyProperty->mutable_vrupdatedata()->set_updatedata(strXml);
    }
    else if (VriAutoProxyType_Vehicle_Control == emType) {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(strXml);
    }
    else if (VriAutoProxyType_HMI_ShowList == emType) {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyUpdateHintsAction);
        noitfyProperty->mutable_updatehintsaction()->set_hintscontent(strXml);
    }
    else if (VriAutoProxyType_Suggestion_QuitScreen == emType) {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(msg);
    }
    else if (VriAutoProxyType_Dictation_SetState == emType) {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyDictationVrServiceStatus);
        if ("idle" == strXml) {
            noitfyProperty->mutable_vrservicestatus()->set_status(VrNotifyDictationVrServiceStatus_VrDictationVrServiceStatus_Idle);
        }
        else if ("listening" == strXml) {
            noitfyProperty->mutable_vrservicestatus()->set_status(VrNotifyDictationVrServiceStatus_VrDictationVrServiceStatus_Listening);
        }
        else if ("processing" == strXml) {
            noitfyProperty->mutable_vrservicestatus()->set_status(VrNotifyDictationVrServiceStatus_VrDictationVrServiceStatus_Processing);
        }
        else {
            noitfyProperty->mutable_vrservicestatus()->set_status(VrNotifyDictationVrServiceStatus_VrDictationVrServiceStatus_Invalid);
        }
    }
    else if (VriAutoProxyType_Dictation_RecResult == emType) {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyDictationVoiceRecogResult);
        bool bSucess = false;
        VR_String strResult;
        GetDictationResult(strXml, bSucess, strResult);

        VR_String Key_Status = "RecognitionStatus";
        VR_String Key_Text = "DisplayText";
        VR_String recognitionstatus;
        VR_String DisplayText;
        VR_JsonParse aJsonParse;
        recognitionstatus = aJsonParse.getStringFromJson(strResult, Key_Status);
        if ("Success" == recognitionstatus) {
            DisplayText = aJsonParse.getStringFromJson(strResult, Key_Text);
            bSucess = true;
        }
        else {
            DisplayText = "";
            bSucess = false;
        }

        noitfyProperty->mutable_vrdictationrecogresult()->set_issuccess(bSucess);
        noitfyProperty->mutable_vrdictationrecogresult()->set_voicerecogresult(DisplayText);
        VR_ConfigureIF::Instance()->setEngineCurrentStatus(VR_Engine_Idle);
        VR_LOGD("getEngineCurrentStatus [%d]", VR_ConfigureIF::Instance()->getEngineCurrentStatus());

    }
    else {
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(msg);
    }
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());

    VR_LOGI("unhandle message !");
}

void VR_EngineEventSender::SendNotifyEvent(int taskId, const VR_String &msg)
{
    VR_LOGI("msg=[%s], taskId=%d", (msg.c_str()), taskId);

    pugi::xml_document doc;
    doc.load_string(msg.c_str());
    pugi::xml_node msgNode = doc.root().first_child();

    if (std::string("VRState") == msgNode.attribute("content").value()
        && std::string("normal") == msgNode.attribute("vrmode").value()) {
        std::unique_ptr<VrNotifyEngineStatus> protoMsg(VrNotifyEngineStatus::default_instance().New());
        protoMsg->set_sender("vr_s");
        protoMsg->set_taskid(taskId);
        protoMsg->set_enginestatus(msg);

        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYENGINESTATUS, protoMsg.release());
        VR_LOGI("Send Event Display IconState(VrNotifyEngineStatus)");
    }
    else if (std::string("VRState") == msgNode.attribute("content").value()
        && std::string("mmvr") == msgNode.attribute("vrmode").value()) {
        std::unique_ptr<VrNotifyMMVr>  protoMsg(VrNotifyMMVr::default_instance().New());
        protoMsg->set_sender("dm");
        protoMsg->set_funccode(VrNotifyMMVr_VrNotifyMMVrFunc_VrNotifyMMVrFunc_EngineStatusChanged);
        VrNotifyMMEngineStatusChanged* engineStatus = protoMsg->mutable_enginestatuschanged();
        engineStatus->set_taskid(taskId);
        engineStatus->set_recogenginestatus(msg);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYMMVR, protoMsg.release());
    }
    else if (std::string("Volume") == msgNode.attribute("content").value()
        && std::string("mmvr") == msgNode.attribute("vrmode").value()) {
        std::unique_ptr<VrNotifyMMVr> protoMsg(VrNotifyMMVr::default_instance().New());
        protoMsg->set_sender("dm");
        protoMsg->set_funccode(VrNotifyMMVr_VrNotifyMMVrFunc_VrNotifyMMVrFunc_VoiceRecogResult);
        VrNotifyMMVoiceRecogResult* recogResult = protoMsg->mutable_voicerecogresult();
        recogResult->set_taskid(taskId);
        recogResult->set_recogresult(msg);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYMMVR, protoMsg.release());
    }
    else if (std::string("QuitVRApp") == msgNode.attribute("content").value()
        && std::string("mmvr") == msgNode.attribute("vrmode").value()) {
        std::unique_ptr<VrNotifyMMVr>  protoMsg(VrNotifyMMVr::default_instance().New());
        protoMsg->set_sender("dm");
        protoMsg->set_funccode(VrNotifyMMVr_VrNotifyMMVrFunc_VrNotifyMMVrFunc_VoiceRecogQuit);
        VrNotifyMMVoiceRecogQuit* recogQuit = protoMsg->mutable_voicerecogquit();
        recogQuit->set_errorcode(MMVr_ErrorCode_Undefine);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYMMVR, protoMsg.release());
    }
    else if (std::string("HintsDisplay") == msgNode.attribute("content").value()) {
        std::unique_ptr<VrNotifyCommonProperty> notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
        notifyCommonProperty->set_sender("dm");
        notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyInitialCondition);
        VrNotifyInitialCondition* startCondition = notifyCommonProperty->mutable_initialcondition();
        startCondition->set_hintsflag(true);
        startCondition->set_hints(msg);

        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
        VR_LOGI("Send Event Display ******hints*******(VrNotifyCommonProperty)");
    }
    else {
        std::unique_ptr<VrNotifyRecogResult> protoMsg(VrNotifyRecogResult::default_instance().New());
        protoMsg->set_sender("vr_s");
        protoMsg->set_taskid(taskId);
        protoMsg->set_recogresult(msg);

        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYRECOGRESULT, protoMsg.release());
        VR_LOGI("Send Event Display Other(VrNotifyRecogResult)");

    }
}

bool VR_EngineEventSender::SendReqEvent(int taskId, int id, const VR_String &msg)
{
    VR_LOGI("msg=[%s], seqId=%d", (msg.c_str()), id);
    // VR_LOGP("DM: Recive Action From DE->(msg:[%s], seqId:[%d])", (msg.c_str()), id);
    bool bRet = false;
    pugi::xml_document doc;
    doc.load_string(msg.c_str());
    pugi::xml_node msgNode = doc.root().first_child();
    std::string agent    = msgNode.attribute("agent").value();
    std::string op       = msgNode.attribute("op").value();
    std::string name     = msgNode.attribute("name").name();
    std::string avalue   = msgNode.attribute("name").value();

    if (name == "name" && agent.empty() && op.empty()) {
        // this always like action notify, like tsl
        VR_LOGI("Message Node Name:[%s]", (msgNode.name()));
        VR_LOGI("Attribute of name is:[%s]", (avalue.c_str()));
        SendReplyEvent(id, msg);
        return bRet;
    }

    if ((0 == strcmp("grammar_result", msgNode.name())) && op.empty()) {
        op = "grammar";
    }

    VR_LOGI("Message Node Name:[%s]\n", (msgNode.name()));
    VR_LOGI("Agent: [%s]", (agent.c_str()));
    VR_LOGI("Op: [%s]", (op.c_str()));
    Msg_Map::const_iterator iter;
    iter = msg_Map.find(std::make_pair(agent, op));

    if (iter != msg_Map.end()) {
        Msg_type type = iter->second;
        if (MSG_DE_DM == type) {
            // VR_LOGP("ActionType:[MSG_DE_DM], Begin Do Action");
            std::unique_ptr<VR_ProtoMessageBase> protoToDM(VR_TransformXmlToProto().Transform(id, msg));
            if (protoToDM.get()) {
                std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
                loopMessage->set_type(VrLoopBack_MSG_Type_MSG_DE2DM);
                loopMessage->mutable_m_de2dm()->MergeFrom(*(static_cast<VrActionDE2DM*>(protoToDM.get())));
                EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
                bRet = true;
                // VR_LOGP("Send Action TO DM-self END, EventTag:[EV_EVENT_VOICERECOG_VRLOOPBACK]");
            }
            else {
                VR_LOGI("unhandle message !");
            }
        }
    }
    return bRet;
}

void VR_EngineEventSender::SendReplyEvent(int id, const VR_String &msg)
{
    VR_LOGI("msg=[%s], seqId=%d", (msg.c_str()), id);

    std::unique_ptr<VR_ProtoMessageBase> protoMsg(VR_TransformXmlToProto().Transform(id, msg));
    if (protoMsg) {
        std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        loopMessage->mutable_m_controlmessage()->MergeFrom(*(static_cast<VrEngineControlMessage*>(protoMsg.get())));
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    else {
        VR_LOGI("unhandle event !");
    }
}

void VR_EngineEventSender::GetDictationResult(const VR_String &strXml, bool bSucess, VR_String &strResult)
{
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    VR_String strSuccess;
    xmlStrParse.GetNodeText("//recognizeResult", strResult);
    xmlStrParse.GetNodeText("//recognizeSuccess", strSuccess);

    bSucess = ("true" == strSuccess) ? true : false;

    VR_LOGI("strResult=[%s], bSucess=%d", (strResult.c_str()), bSucess);
}
/* EOF */
