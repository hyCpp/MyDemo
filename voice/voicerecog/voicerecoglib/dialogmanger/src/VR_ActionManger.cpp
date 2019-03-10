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
#include <sstream>
#include <boost/format.hpp>
#include <pugixml.hpp>

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrMessageTrans.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyActionResult.pb.h"
#include "navi/Traffic/WeatherInfoforVRRes.pb.h"
#include "navi/Map/MapScreenInfo.pb.h"
#include "navi/Map/MapNeighborName.pb.h"

#include "navi/Location/LocInfoUpdate.pb.h"

// #include "VR_Common_VrUnitAgent.h"

#if defined(VR_DM_TARGET_VRUNIT)
#include "navi/Search/DefaultInfoResult.pb.h"
#include "navi/Search/RespCityInfoVRResult.pb.h"
#include "navi/Search/RespStateInfoVRResult.pb.h"
#include "navi/Search/RespStreetInfoVRResult.pb.h"
#include "navi/Search/RespHouseNoInfoVRResult.pb.h"
#include "navi/Search/RespCategorySearchVRResult.pb.h"
#include "navi/Point/PointResponse.pb.h"
#include "navi/Path/GuideStatus.pb.h"
#include "navi/Path/NotifyRouteStatus.pb.h"
#include "navi/Path/PathPointList.pb.h"
#include "navi/Path/NearLinkResult.pb.h"

#include "navi/Search/RespStateInfoVRResultCN.pb.h"
#include "navi/Search/RespCityInfoVRResultCN.pb.h"
#include "navi/Search/RespDistrictInfoVRResultCN.pb.h"
#include "navi/Search/RespStreetInfoVRResultCN.pb.h"
#include "navi/Search/RespHouseNoInfoVRResultCN.pb.h"
#include "navi/Search/RespAddressInfoVRResultCN.pb.h"
#include "navi/Search/RespPOINameListVRResultCN.pb.h"

#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include   "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#endif

#include "VR_ActionInnerParameterDef.h"
#include "VR_ActionManger.h"
#include "action/VR_ActionFactoryIF.h"
#include "action/VR_ActionEventSender.h"
#include "action/VR_RequestQueue.h"
#include "action/VR_VoiceTagAction.h"
#include "VR_ConfigureIF.h"
#include "VR_TransformProtoToXml.h"
#include "VR_ActionSenderDef.h"
#include "ncore/NCTypesDefine.h"
#include "VR_DataProvider.h"
#include "VR_PhoneBookXMLFormat.h"
#include "VR_PB_XmlBuildHelper.h"

template<class T> void toString(std::string& result, const T& param)
{
    std::stringstream ss;
    ss << param;
    result = ss.str();
}

using namespace navi::VoiceRecog;
using namespace navi::Location;

#if defined(VR_DM_TARGET_VRUNIT)
using namespace navi::Search;
using namespace navi::Path;
using namespace navi::Point;
using namespace navi::Location;
#endif

VR_ActionManger::VR_ActionManger(spVR_ActionFactoryIF factory, spVR_ActionEventSender sender,
                                 spVR_SessionManger spSessionManager)
    : m_pActionFactory(factory)
    , m_pRequestQueue(VR_new VR_RequestQueue())
    , m_spVR_ActionEventSender(sender)
    , m_spSessionManager(spSessionManager)
  //  , m_spDataProvider(dp)
    , m_bDemoOn(false)
    , m_bIsPhoneConFromFC(false)
{
    VR_LOGD_FUNC();
}

VR_ActionManger::~VR_ActionManger()
{
    VR_LOGD_FUNC();
}

void VR_ActionManger::OnMessage(const VrActionDE2DM &proto)
{
    int actionId = proto.id();
    VrDMActionType  actionType = proto.vr_dm_action_type();
    VR_LOGI("nActionId=[%d],actionType=[%d]", actionId, actionType);
    switch (actionType) {
    case ActionType_ADDITIONAL_INFO_REQ :
        VR_LOGI("case ActionType_ADDITIONAL_INFO_REQ");
        if (proto.has_additional_info_req()) {
            AdditionalInfoAction(actionId, proto.additional_info_req());
        }
        else {
            VR_LOGE("proto.has_additional_info_req() = [%d]", (proto.has_additional_info_req()));
        }
       break;
    case  ActionType_RECOG_RESULT:
        VR_LOGI("case ActionType_RECOG_RESULT");
        if (proto.has_recog_result()) {
           RecogResultAction(actionId, proto.recog_result());
        }
        else {
            VR_LOGE("proto.has_recog_result() = [%d]", (proto.has_recog_result()));
        }
        break;
    case  ActionType_ACTIVE_INFO_REQ:
        VR_LOGI("case ActionType_ACTIVE_INFO_REQ");
        if (proto.has_active_info_req()) {
           ActiveInfoAction(actionId, proto.active_info_req());
        }
        else {
            VR_LOGE("proto.has_active_info_req() = [%d]", (proto.has_active_info_req()));
        }
        break;
    case  ActionType_TTS_COMMAND_REQ:
        VR_LOGI("case ActionType_TTS_COMMAND_REQ");
        if (proto.has_tts_req()) {
            TtsAction(actionId, proto.tts_req());
        }
        else {
            VR_LOGE("proto.has_tts_req() = [%d]", (proto.has_recog_result()));
        }
        break;
    case  ActionType_BEEP_COMMAND_REQ:
        VR_LOGI("case ActionType_BEEP_COMMAND_REQ");
        if (proto.has_beep_req()) {
            BeepAction(actionId, proto.beep_req());
        }
        else {
            VR_LOGE("proto.has_tts_req() = [%d]", (proto.has_recog_result()));
        }
        break;
    case  ActionType_ACTION_TEST_MSG:
        VR_LOGI("case ActionType_ACTION_TEST_MSG");
        if (proto.has_action_test_msg()) {
            TestActionMsg(actionId, proto.action_test_msg());
        }
        else {
            VR_LOGE("proto.has_action_test_msg() = [%d]", (proto.has_action_test_msg()));
        }
        break;
    case  ActionType_NOTIFY_VOICE_TAG_RESULT:
        VR_LOGI("case ActionType_NOTIFY_VOICE_TAG_RESULT");
        if (proto.has_notify_voice_tag_result()) {
            m_pRequestQueue->OnMessage(actionId, proto.notify_voice_tag_result());
        }
        else {
            VR_LOGE("proto.has_notify_voice_tag_result() = [%d]", (proto.has_notify_voice_tag_result()));
        }
        break;
    case ActionType_MESSAGE_INFO_REQ:
        VR_LOGI("case ActionType_MESSAGE_INFO_REQ");
        if (proto.has_hfd_message_info_req()) {
            MessageDetailAction(actionId, proto.hfd_message_info_req());
        }
        else {
            VR_LOGE("proto.has_hfd_message_info_req() = [%d]", (proto.has_hfd_message_info_req()));
        }
        break;
    case  ActionType_Navi_Action_REQ:
        VR_LOGI("case ActionType_Navi_Action_REQ");
        if (proto.has_voice_navi_req()) {
            if (proto.voice_navi_req().actiontype() == NAVI_ACTION_queryCurrentPosition) {
#if defined(VR_DM_TARGET_VRUNIT)
                QueryCurrentPosAction(actionId);
#endif
            }
            else {
                NaviActionHandle(actionId, proto.voice_navi_req());
            }
        }
        else {
            VR_LOGE("proto.has_voice_navi_req() = [%d]", (proto.has_voice_navi_req()));
        }
        break;
    case  ActionType_VOICET_TAG_REGISTER_RESP:
        VR_LOGI("case ActionType_VOICET_TAG_REGISTER_RESP");
        if (proto.has_voice_tag_register_resp()) {
            m_pRequestQueue->OnMessage(actionId, proto.voice_tag_register_resp());
        }
        else {
            VR_LOGE("proto.has_voice_tag_register_resp() = [%d]", (proto.has_voice_tag_register_resp()));
        }
        break;
    case  ActionType_VOICET_TAG_PLAY_BACK_RESP:
        VR_LOGI("case ActionType_VOICET_TAG_PLAY_BACK_RESP");
        if (proto.has_voice_tag_play_back_resp()) {
            m_pRequestQueue->OnMessage(actionId, proto.voice_tag_play_back_resp());
        }
        else {
            VR_LOGE("proto.has_voice_tag_play_back_resp() = [%d]", (proto.has_voice_tag_play_back_resp()));
        }
        break;
    case  ActionType_TTS_LIST_REQ:
        VR_LOGI("case ActionType_TTS_LIST_REQ");
        if (proto.has_tts_list_req()) {
            TtsListAction(actionId, proto.tts_list_req());
        }
        else {
            VR_LOGE("proto.has_tts_list_req() = [%d]", (proto.has_tts_list_req()));
        }
        break;
    default:
        {
            VR_LOGI("default");
            VR_LOGE("proto.vr_dm_action_type() = [%d]", (actionType));
        }
       break;
   }
}

void VR_ActionManger::OnMessage(const VrActionFC2DM &proto)
{
    int actionId = proto.id();
    VrDMActionType  actionType = proto.vr_dm_action_type();
    VR_LOGI("actionId=[%d],action type=[%d]", actionId, actionType);
    VR_LOGP("Recive Reply From FC actionId:[%d], ActionType:[%d]", actionId, actionType);
    switch (actionType) {
    case ActionType_ADDITIONAL_INFO_RSP :
        VR_LOGI("case ActionType_ADDITIONAL_INFO_RSP");
        if (proto.has_additional_info_rsp()) {
           m_pRequestQueue->OnMessage(actionId, proto.additional_info_rsp());
        }
        else {
            VR_LOGE("proto.has_additional_info_rsp() = [%d]", (proto.has_additional_info_rsp()));
        }
        break;
    case  ActionType_RECOG_RESULT_CONFIRM :
        VR_LOGI("case ActionType_RECOG_RESULT_CONFIRM");
        if (proto.has_recog_result_confirm()) {
           m_pRequestQueue->OnMessage(actionId, proto.recog_result_confirm());
           VR_LOGP("Send RecogResultConfirm TO DE");
        }
        else {
            VR_LOGE("proto.has_recog_result_confirm() = [%d]", (proto.has_recog_result_confirm()));
        }
        break;
    case  ActionType_TTS_COMMAND_RESULT:
        VR_LOGI("case ActionType_TTS_COMMAND_RESULT");
        if (proto.has_tts_result()) {
            m_pRequestQueue->OnMessage(actionId, proto.tts_result(), true);
        }
        else {
            VR_LOGE("proto.has_tts_result() = [%d]", (proto.has_tts_result()));
        }
        break;
    case ActionType_MESSAGE_INFO_RESP:
        VR_LOGI("case ActionType_MESSAGE_INFO_REQ");
        if (proto.has_hfd_message_info_resp()) {
            m_pRequestQueue->OnMessage(actionId, proto.hfd_message_info_resp());
        }
        else {
            VR_LOGE("proto.has_hfd_message_info_resp() = [%d]", (proto.has_hfd_message_info_resp()));
        }
        break;
    default:
        {
            VR_LOGI("default");
            VR_LOGE("proto.vr_dm_action_type() = [%d]", (actionType));
        }
        break;
    }

}

void VR_ActionManger::OnReplyActoinResult(const VR_ProtoMessageBase &msg)
{
    VR_LOGP("Recive ActionResult From FC");
    const VrNotifyActionResult&  actionResult = (const VrNotifyActionResult &)(msg);
    VR_String strResult = actionResult.actionresult();
    int actionId = actionResult.taskid();
    VR_LOGI("strResult=[%s]", strResult.c_str());
    bool bConnect = VR_ConfigureIF::Instance()->getInnerAvcLanConnectStatus();

    if (bConnect) {
#ifdef VR_DM_TARGET_VRAGENT
        if (std::string::npos != strResult.find("getMessageDetail")) {
            VR_LOGI("getMessageDetail");
            pugi::xml_document doc;
            doc.load(strResult.c_str());

            if (doc.empty()) {
                VR_LOGD("[-- msgdetail unavailable --]");
                return;
            }
            strResult = doc.select_node("//messagebody").node().child_value();

            std::unique_ptr<VrMessageAcquisitionResp> msgResult(VrMessageAcquisitionResp::default_instance().New());
            msgResult->set_op_result(true);
            msgResult->set_service_id(0);
            msgResult->set_len_addr(0);
            msgResult->set_address("");
            msgResult->set_len_title(0);
            msgResult->set_title("");
            msgResult->set_len_body(strlen(strResult.c_str()));
            msgResult->set_body(strResult);
            m_pRequestQueue->OnMessage(actionId, *msgResult);
        }
        else if (std::string::npos != strResult.find("getMessageState")) {
            VR_LOGI("AvcLan-Agent--------------getMessageState");
            OnReceiveMsgAvaliableState(strResult);
        }
        else if (std::string::npos != strResult.find("arriveTimeNotify")) {
            VR_REGION_TYPE emContryType = VR_ConfigureIF::Instance()->getVRContry();
            if (VR_REGION_US == emContryType) {
                OnReceiveArriveTimeNotify(strResult);
            }
        }
        else {
            VR_LOGI("other");
            std::unique_ptr<VrRecogResultConfirm>  resultProto(VrRecogResultConfirm::default_instance().New());
            int actionType = actionResult.actionid();
            resultProto->set_action_id(actionType);
            if ("success" == strResult) {
                resultProto->set_processing_result(true);
            }
            else {
                resultProto->set_processing_result(false);
            }
            m_pRequestQueue->OnMessage(actionId, *resultProto);
        }
#else
        if (std::string::npos != strResult.find("queryShowingPOIIconNumber")) {
            VR_LOGI("queryShowingPOIIconNumber");
            m_spVR_ActionEventSender->sendEngineReply(actionId, strResult);
        }
        else if (std::string::npos != strResult.find("PHONE_STATE_CONNECTED")) {
            OnReceiveTelConnectedState(strResult);
        }
        else {
            VR_LOGI("navi other");
            bool bState = false;
            int actionType = actionResult.actionid();
            if ("success" == strResult) {
                bState = true;
            }
            else {
                bState = false;
            }
            strResult = VR_TransformProtoToXml().Transform(actionType, bState);
            m_spVR_ActionEventSender->sendEngineReply(actionId, strResult);
        }
#endif
    }
    else {
        VR_LOGI("connect=[false]");
        int actionType = actionResult.actionid();
        if (std::string::npos != strResult.find("IncomingMessageInfo")) {
            VR_LOGD("IncomingMessageInfo");
            m_spVR_ActionEventSender->sendEngineReply(-1, strResult);
        }
        else if (std::string::npos != strResult.find("getMessageDetail")) {
            VR_LOGI("getMessageDetail");
            m_spVR_ActionEventSender->sendEngineReply(actionId, strResult);
        }
        else if (std::string::npos != strResult.find("getMsgDetailStatusNoChange")) {
            VR_LOGI("getMsgDetailStatusNoChange");
            m_spVR_ActionEventSender->sendEngineReply(actionId, strResult);
        }
        else if (std::string::npos != strResult.find("getMessageState")) {
            VR_LOGI("getMessageState");
            VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
            if (NULL != pConfig) {
                pConfig->setUpdateState(VR_PHONE_GETMESSAGESTATE, strResult);
            }
            m_spVR_ActionEventSender->sendEngineReply(-1, strResult);
        }
        else if (std::string::npos != strResult.find("MAPProtocol")) {
            VR_LOGI("[--- MAPProtocol ---]");
            VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
            if (NULL != pConfig) {
                pConfig->setUpdateState(VR_PHONE_MAPPROTOCOL, strResult);
            }
            m_spVR_ActionEventSender->sendEngineReply(-1, strResult);
        }
        else if (std::string::npos != strResult.find("getMessageSupportType")) {
            VR_LOGI("getMessageSupportType");
            OnReceiveSupportMsgTypeInfo(strResult);
        }
        else if ((ACTION_TYPE_SHOW_VOICE_SETTINGS_UI == actionType) || (ACTION_TYPE_SHOW_LEXUS_INSIDER_UI == actionType)
            || (ACTION_TYPE_SHOW_TUTORIALS_UI == actionType) || (ACTION_TYPE_SHOW_TRAINING_END_MSG == actionType)
            || (ACTION_TYPE_PLAY_BY_FILTER == actionType) || (ACTION_TYPE_SHOW_MEDIA_SCREEN == actionType)) {
            bool bState = false;
            if ("success" == strResult) {
                bState = true;
            }
            else {
                bState = false;
            }
            strResult = VR_TransformProtoToXml().TransformSpecialType(actionType, bState);
            m_spVR_ActionEventSender->sendEngineReply(actionId, strResult);
        }
        else {
            std::unique_ptr<VrRecogResultConfirm>  resultProto(VrRecogResultConfirm::default_instance().New());
            resultProto->set_action_id(actionType);
            if ("success" == strResult) {
                resultProto->set_processing_result(true);
            }
            else {
                resultProto->set_processing_result(false);
            }
            strResult = VR_TransformProtoToXml().Transform(*resultProto);
            m_spVR_ActionEventSender->sendEngineReply(actionId, strResult);
        }
    }
}

void VR_ActionManger::RecogResultAction(int actionId, const VrRecogResult &proto)
{
    VR_LOGI("actionId=[%d]", actionId);
    spVR_ActionBase recogResultAction = m_pActionFactory->CreateAction(actionId, proto);
    m_pRequestQueue->Push(recogResultAction);
}

void VR_ActionManger::AdditionalInfoAction(int actionId, const VrAdditionalInfoReq &proto)
{
    VR_LOGI("actionId=[%d]", actionId);
    spVR_ActionBase infoAction = m_pActionFactory->CreateAction(actionId, proto);
    m_pRequestQueue->Push(infoAction);
}

void VR_ActionManger::ActiveInfoAction(int actionId, const VrActiveInfoReq &proto)
{
    VR_LOGI("actionId=[%d]", actionId);
    spVR_ActionBase infoAction = m_pActionFactory->CreateAction(actionId, proto);
    m_pRequestQueue->Push(infoAction);
}

void VR_ActionManger::TtsAction(int actionId, const VrTTSCommandReq &proto)
{
    VR_LOGI("actionId=[%d]", actionId);
    spVR_ActionBase ttsAction = m_pActionFactory->CreateAction(actionId, proto);
    m_pRequestQueue->Push(ttsAction, true);
}

void VR_ActionManger::BeepAction(int actionId, const VrBeepCommandReq &proto)
{
    VR_LOGI("actionId=[%d]", actionId);
    spVR_ActionBase beepAction = m_pActionFactory->CreateAction(actionId, proto);
    m_pRequestQueue->Push(beepAction, true);

}

void VR_ActionManger::TestActionMsg(int actionId, const VrActionTestMsg &proto)
{
    VR_LOGI("actionId=[%d]", actionId);
    m_spVR_ActionEventSender->sendEngineReply(actionId, proto.msg());
}

void VR_ActionManger::MessageDetailAction(int actionId, const VrMessageAcquisitionReq &proto)
{
    VR_LOGI("actionId=[%d]", actionId);
    spVR_ActionBase infoAction = m_pActionFactory->CreateAction(actionId, proto);
    m_pRequestQueue->Push(infoAction);
}

void VR_ActionManger::VoiceTagAction(const EV_EventContext &context, const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();

    spVR_ActionBase voiceTagAction = m_pActionFactory->CreateAction(context, (RequestVrVoiceTag&)proto);
    voiceTagAction->SetSessionManager(m_spSessionManager);
    m_pRequestQueue->Push(voiceTagAction);
}

void VR_ActionManger::NaviActionHandle(int actionId, const VrActionNavi &proto)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    spVR_ActionBase naviAction = m_pActionFactory->CreateAction(actionId, proto);
    m_pRequestQueue->Push(naviAction);
#endif
}

void VR_ActionManger::TtsListAction(int actionId, const VrTtsListCommandReq &proto)
{
    VR_LOGI("actionId=[%d]", actionId);
    spVR_ActionBase ttsAction = m_pActionFactory->CreateAction(actionId, proto);
    m_pRequestQueue->Push(ttsAction, true);
}

void VR_ActionManger::OnMessageNaviReqDfltInfoResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#ifdef VR_DM_TARGET_VRUNIT
    const DefaultInfoResult &proto = (const DefaultInfoResult &)msg;
    int actionid = proto.senderid();
    spVR_ActionBase spReq = m_pRequestQueue->Find(actionid);
    if (spReq) {
        m_pRequestQueue->OnMessage(actionid, proto);
    }
    else {
        int errorCode = proto.errorcode();
        if ((errorCode == 0) && proto.has_deafultarea()) {
            nutshell::INT32 i32CountryID = proto.deafultarea().countryid();
            VR_ConfigureIF::Instance()->setCountryIDForVBT(i32CountryID);
            // send xml
            std::string countryId = std::to_string(i32CountryID);
            std::string xmlStart = "<event name=\"changeCountry\"><countryID>%1%</countryID></event>";
            std::string xmlResult = (boost::format(xmlStart) % countryId).str();
            VR_LOGD("xmlResult = %s", xmlResult.c_str());
            m_spVR_ActionEventSender->sendEngineReply(-1, xmlResult);
        }
    }

#endif
}

void VR_ActionManger::OnMessageNaviSearchStateResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespStateInfoVRResult &proto = (const RespStateInfoVRResult &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviSearchCityResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespCityInfoVRResult &proto = (const RespCityInfoVRResult &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviSearchStreetResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespStreetInfoVRResult &proto = (const RespStreetInfoVRResult &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviSearchHouseNoResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespHouseNoInfoVRResult &proto = (const RespHouseNoInfoVRResult &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviSearchPoiInfoResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespCategorySearchVRResult &proto = (const RespCategorySearchVRResult &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviQueryPointCollectionResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const PointResponse &proto = (const PointResponse &)msg;
    int actionid = proto.respinfo().requestid();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviQueryDestHistoryResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const PointResponse &proto = (const PointResponse &)msg;
    int actionid = proto.respinfo().requestid();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviQueryRouteExistResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const GuideStatus &proto = (const GuideStatus &)msg;
    int actionid = proto.sender_id();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviQueryAllDestListResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const PathPointList &proto = (const PathPointList &)msg;
    int actionid = proto.obj_key().req_id();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviPathPointListNotify(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    this->UpdateNaviPathPointList(msg);
#endif
}

void  VR_ActionManger::UpdateNaviPathPointList(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_pActionFactory->UpdateNaviPathPointList(msg);
}


void VR_ActionManger::OnMessageNaviQueryUnpassedDestListResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const PathPointList &proto = (const PathPointList &)msg;
    int actionid = proto.obj_key().req_id();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviQueryPointTypeResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const NearLinkResult &proto = (const NearLinkResult &)msg;
    if (PathDef_RequestObjectType_OBJECT_TYPE_VOICE != proto.req_key().req_obj()) {
        return;
    }
    int actionid = proto.req_key().req_id();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviQueryNextDestResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    // const GudReplyVRETAInfo &proto = (const GudReplyVRETAInfo &)msg;
    // int actionid = proto.senderid();
    // m_pRequestQueue->OnMessage(actionid, proto);
    const PathPointList &proto = (const PathPointList &)msg;
    int actionid = proto.obj_key().req_id();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviQueryCurrentPosResp(const VR_ProtoMessageBase &msg)
{
    // VR_LOGD_FUNC();
// #if defined(VR_DM_TARGET_VRUNIT)
    const LocInfoUpdate &proto = (const LocInfoUpdate &)msg;
    const LocInfoUpdate_LDM_LocInformation &cLocInfo = proto.m_clocinfo();
    const LocInfoUpdate_LDM_RoadDataInfo &roadDataInfo = cLocInfo.m_croaddatainfo();
    nutshell::INT32 wroadkind = roadDataInfo.m_wroadkind();

    // get demo flag
 //   const LocInfoUpdate_LDM_LocStatus &locStatus = cLocInfo.m_clocstatus();
      bool bDemoOn = false;
 //   bool bPrevDemoOn = VR_ConfigureIF::Instance()->getDemoOnFlg();

    const LocInfoUpdate_LDM_LocInfoNormal &locInfoNormal = cLocInfo.m_clocinfonormal();
    const LocInfoUpdate_LocLonLat &locLonlat = locInfoNormal.m_cpos();
    nutshell::INT32 ilon = locLonlat.ilon();
    nutshell::INT32 ilat = locLonlat.ilat();

    VR_ConfigureIF::Instance()->setLocateInfo(wroadkind, ilon, ilat, bDemoOn);

    // Demo 状态变更， 同步更新
//    if (bDemoOn != bPrevDemoOn) {
//        std::unique_ptr<VrLoopBack>    loopMessage(VrLoopBack::default_instance().New());
//        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
//        VrEngineControlMessage*  controlMessage = loopMessage->mutable_m_controlmessage();
//        controlMessage->set_type(DMTaskMessageType_StartStatusResult);
//        controlMessage->mutable_startstatusresult()->set_starttype(Navigation_Demo);
//        controlMessage->mutable_startstatusresult()->set_startreuslt(bDemoOn);
//        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
//    }

// #endif
}

void VR_ActionManger::OnMessageNaviMapScreenInfo(const VR_ProtoMessageBase &msg)
{
    const navi::Map::MapScreenInfo &proto = (const navi::Map::MapScreenInfo &)msg;
    if (proto.has_car_position_mode()) {
        VR_ConfigureIF::Instance()->setCursorFlg(!proto.car_position_mode()); // (car_position_mode == true) is no cursor
    }
}

void VR_ActionManger::OnMessageNaviMapNeighborName(const VR_ProtoMessageBase &msg)
{
    const navi::Map::MapNeighborName &proto = (const navi::Map::MapNeighborName &)msg;
    if (proto.has_neighbor_roadname()) {
        VR_ConfigureIF::Instance()->setStrRoadName(proto.neighbor_roadname());
    }
    else {
        VR_ConfigureIF::Instance()->setStrRoadName("");
    }
}

void VR_ActionManger::OnMessageNaviQueryPOIByNameRespCN(const VR_ProtoMessageBase &msg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    const RespPOINameListVRResultCN &proto = (const RespPOINameListVRResultCN &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, proto);
#endif
}

void VR_ActionManger::OnMessageNaviSearchStateRespCN(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespStateInfoVRResultCN &proto = (const RespStateInfoVRResultCN &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, msg);
#endif
}

void VR_ActionManger::OnMessageNaviSearchCityRespCN(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespCityInfoVRResultCN &proto = (const RespCityInfoVRResultCN &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, msg);
#endif

}

void VR_ActionManger::OnMessageNaviSearchDistrictRespCN(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespDistrictInfoVRResultCN &proto = (const RespDistrictInfoVRResultCN &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, msg);
#endif
}

void VR_ActionManger::OnMessageNaviSearchStreetRespCN(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespStreetInfoVRResultCN &proto = (const RespStreetInfoVRResultCN &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, msg);
#endif
}

void VR_ActionManger::OnMessageNaviSearchHouseNoRespCN(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespHouseNoInfoVRResultCN &proto = (const RespHouseNoInfoVRResultCN &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, msg);
#endif
}

void VR_ActionManger::OnMessageNaviSearchAddressInfoCN(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const RespAddressInfoVRResultCN &proto = (const RespAddressInfoVRResultCN &)msg;
    int actionid = proto.senderid();
    m_pRequestQueue->OnMessage(actionid, msg);
#endif
}

void VR_ActionManger::OnMessageNaviNotifyRouteStatus(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    const NotifyRouteStatus &rNotifyRouteStatus = (const NotifyRouteStatus &)msg;
   PathDef_GuideStatus eGuideStatus =  rNotifyRouteStatus.guide_status();
   std::string strStatus = "";

   if ((PathDef_GuideStatus_GUIDESTATUS_GUIDE_STOP == eGuideStatus) || (PathDef_GuideStatus_GUIDESTATUS_GUIDE_ON == eGuideStatus)) {
        strStatus = "true";
   }
   else {
        strStatus = "false";
   }

   std::string resultXml = (boost::format(NaviNotifyRouteStatusXML) % strStatus).str();
   VR_LOGD("resultXml == %s", resultXml.c_str());
   m_spVR_ActionEventSender->sendEngineReply(-1, resultXml);
#endif
}

void VR_ActionManger::ClearTtsAction()
{
    VR_LOGD_FUNC();
    m_pRequestQueue->ClearTtsAction();
}

#if defined(VR_DM_TARGET_VRUNIT)
void VR_ActionManger::QueryCurrentPosAction(int actionId)
{
    VR_LOGD_FUNC();
    std::string op = "queryCurrentPosition";
    std::string errCode = "0";
    std::string displayGuideType1 = "";
    std::string longitude2 = "";
    std::string latitude3 = "";
    std::string count4 = "0";
    std::string guideLon5 = "";
    std::string guideLan6 = "";
    std::string linkId7 = "0";
    std::string guideType = "";
    std::string dispName = "";
    std::string guidLinkId = "0";

    nutshell::INT32 wroadkind = 0xFFFFFFFF;
    nutshell::INT32 ilon = 0xFFFFFFFF;
    nutshell::INT32 ilat = 0xFFFFFFFF;
    bool bDemoOn = false;
    VR_ConfigureIF::Instance()->getLocateInfo(wroadkind, ilon, ilat, bDemoOn);

    toString(displayGuideType1, wroadkind);
    toString(longitude2, ilon);
    toString(latitude3, ilat);

    std::string guidePtItemXml = (boost::format(NaviGuidePointItem) % guideType % guideLon5 % guideLan6 % guidLinkId).str();
    std::string guidePtXml = (boost::format(NaviGuidePointXml) % displayGuideType1 % longitude2 % latitude3 % count4 % guidePtItemXml % linkId7 % dispName).str();
    std::string resultXml = (boost::format(NaviConnectXML) % op % errCode % guidePtXml).str();

    m_spVR_ActionEventSender->sendEngineReply(actionId, resultXml);
    VR_LOGI("current_pos_xml = [%s]", resultXml.c_str());
}

#endif

void VR_ActionManger::OnMessageWeatherInfoResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const navi::Traffic::WeatherInfoforVRRes &proto = (const navi::Traffic::WeatherInfoforVRRes &)msg;
    int actionid = proto.action_id();
    m_pRequestQueue->OnMessage(actionid, msg);
}


void VR_ActionManger::OnReceiveMsgAvaliableState(const VR_String &strMsgState)
{
    VR_LOGD_FUNC();

    bool bAvcLanCon = VR_ConfigureIF::Instance()->getAvcLanConnectStatus();

    if (bAvcLanCon) {
        VR_REGION_TYPE emContryType = VR_ConfigureIF::Instance()->getVRContry();
        VR_LOGD("[--- vr-contry[%d] ---]", emContryType);

        if (VR_REGION_US == emContryType) {

            if (strMsgState.empty()) {
                VR_LOGD("[----OnReceiveMsgAvaliableState: strMsgState.empty----]");
                return;
            }

            pugi::xml_document doc;
            doc.load(strMsgState.c_str());
            if (doc.empty()) {
                VR_LOGD("[----OnReceiveMsgAvaliableState: doc.empty----]");
                return;
            }

            std::string strMsgType;
            std::string strOutboxValue = "0";
            pugi::xpath_node_set nodeSet = doc.select_nodes("//instanceId");

            for (auto it = nodeSet.begin(); nodeSet.end() != it; ++it) {
                pugi::xml_node paramNode = it->node();
                for (pugi::xml_attribute attr = paramNode.first_attribute(); attr; attr = attr.next_attribute()) {
                    if (0 == strcmp("messageType", attr.name())) {
                        strMsgType = attr.value();
                    }
                    if (0 == strcmp("outbox", attr.name())) {
                        strOutboxValue = attr.value();
                        VR_UNUSED_VAR(strOutboxValue);
                    }
                }

                long int MsgTypeId = std::strtol(strMsgType.c_str(), NULL, 10);
                VR_LOGI("MsgTypeId : %ld", MsgTypeId);

                const int SMS_GSM = 2;
                const int SMS_CDMA = 4;
                if ((SMS_GSM & MsgTypeId) || (SMS_CDMA & MsgTypeId)) {
                    break;
                }
            }

           //  m_spDataProvider->OnReceiveMsgAvaliableState(strOutboxValue);
        }
        else {
            VR_LOGD("[--- VR contry is not US ---]");
        }
    }
    else {
        VR_LOGD("[--- avclan-con-state[%d] ---]", bAvcLanCon);
    }
}

void VR_ActionManger::OnReceiveSupportMsgTypeInfo(const VR_String &strMsgType)
{
    VR_LOGD_FUNC();
}

void VR_ActionManger::OnReceiveTelConnectedState(const VR_String &strConState)
{
    VR_LOGD_FUNC();
    if (strConState.empty()) {
        VR_LOGD("[----OnReceivePhoneConnectState: strMsgState.empty----]");
        return;
    }

    pugi::xml_document doc;
    doc.load(strConState.c_str());
    if (doc.empty()) {
        VR_LOGD("[----OnReceivePhoneConnectState: doc.empty----]");
        return;
    }

    pugi::xml_node itemNode = doc.select_node("//item").node();
    if (itemNode.empty()) {
        VR_LOGD("[----OnReceivePhoneConnectState: itemNode.empty----]");
        return;
    }

    VR_String value = itemNode.attribute("value").value();
    bool lastConStateValue = m_bIsPhoneConFromFC;
    VR_String strConValue("disconnected");

    // value is "0" or "1" mean "false" or "true"
    if ("1" == value || "connected" == value || "true" == value) {
        strConValue = "connected";
        m_bIsPhoneConFromFC = true;
    //    m_spVR_ActionEventSender->sendEngineReply(-1, VR_PB_GRAMMAR_ACTIVE);
    }
    else {
        m_bIsPhoneConFromFC = false;
    }

    VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
    if (NULL != pConfig) {
        pConfig->setUpdateState(VR_PHONE_STATE_CONNECTED, strConValue);
   //     pConfig->setUpdateGrammar(VR_PHONE_GRAMMAR_ACTIVE, VR_PB_GRAMMAR_ACTIVE);
        pConfig->setAgentStatus(telephone_connection_state, m_bIsPhoneConFromFC);
        VR_LOGD("[---- telephone_connection_state: <%d> ----]", m_bIsPhoneConFromFC);
    }
    else {
        VR_LOGD("[---- pConfig = NULL: telephone_connection_state ----]");
    }

    VR_LOGD("[---- last-value-from-fc[%d] new-value-from-fc[%d] ----]", lastConStateValue, m_bIsPhoneConFromFC);
    if (lastConStateValue != m_bIsPhoneConFromFC) {
        if (lastConStateValue) {
    //        m_spVR_ActionEventSender->sendEngineReply(-1, VR_PB_DP_CANCELXML);
            if (NULL != pConfig) {
                pConfig->setUpdateGrammar(VR_PHONE_GRAMMAR_INIT, "");
                pConfig->setUpdateGrammar(VR_PHONE_GRAMMAR_ACTIVE, "");
                pConfig->setUpdateGrammar(VR_PHONE_GRAMMAR_NEW_MESSAGETYPE, "");
            }
        }
    }
}

void VR_ActionManger::OnReceiveArriveTimeNotify(const VR_String &strArriveTime)
{
    VR_LOGD_FUNC();
    if (strArriveTime.empty()) {
        VR_LOGD("[----OnReceiveArriveTimeNotify: strArriveTime.empty----]");
        return;
    }

    pugi::xml_document doc;
    doc.load(strArriveTime.c_str());
    if (doc.empty()) {
        VR_LOGD("[----OnReceiveArriveTimeNotify: doc.empty----]");
        return;
    }

    pugi::xml_node timeNode = doc.select_node("//time").node();
    if (timeNode.empty()) {
        VR_LOGD("[----OnReceiveArriveTimeNotify: time.empty----]");
        return;
    }

    VR_String value = timeNode.attribute("value").value();
  //  m_spDataProvider->OnReceiveArriveTimeNotify(value);
}

/* EOF */
