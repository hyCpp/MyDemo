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

#include <memory>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "VR_EventSenderIF.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_ActionEventSender.h"
#include "VR_VoiceTagAction.h"
#include "VR_ActionTtsPlay.h"
#include "VR_ActionBeepPlay.h"
#include "VR_ConfigureIF.h"

#include "navi/Voice/VoiceRecog/VrCommonDef.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"
#include "navi/Voice/VoiceRecog/RespVrVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyVoiceTagResult.pb.h"
#include "navi/Voice/TTS/TtsStop.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "VR_CommonTool.h"

using namespace navi::VoiceRecog;

const VR_String ADD_RECORD_VOICETAG    = VR_String("addRecordVoiceTag");
const VR_String EDIT_RECORD_VOICETAG   = VR_String("editRecordVoiceTag");
const VR_String SAVE_VOICETAG          = VR_String("saveVoiceTag");
const VR_String DELETE_VOICETAG        = VR_String("deleteVoiceTag");
const VR_String DELETE_ALL_VOICETAG    = VR_String("deleteAllVoiceTag");
const VR_String PLAY_VOICETAG          = VR_String("playVoiceTag");
const VR_String STOP_VOICETAG          = VR_String("stopVoiceTag");
const VR_String CANCEL_RECORD_VOICETAG = VR_String("cancelRecordVoiceTag");
const VR_String CANCEL_SAVE_VOICETAG   = VR_String("cancelSaveVoiceTag");
const VR_INT32  INVALID_VALUE          = -1;

VoiceList<int>::type *VR_VoiceTagQueue::GetVoiceTagActionIDs()
{
    return &m_VoiceTagActionIDs;
}

void VR_VoiceTagQueue::RemoveAction(const VR_INT32 actionId)
{
    Remove(actionId);
}

VR_VoiceTagAction::VR_VoiceTagAction(spVR_ActionEventSender sender, const EV_EventContext &context, const RequestVrVoiceTag &msg)
    : VR_ActionBase(sender)
    , m_spContext(VR_EventSenderIF::CopyEventContext(&context))
    , m_ReqMsg(msg.New())
    , m_ReqMsgNotify(VrNotifyVoiceTagResult::default_instance().New())
    , m_sessionManger()
    , m_spVoiceTagQueue(VR_new VR_VoiceTagQueue())
{
    VR_LOGD_FUNC();
    m_ReqMsg->CheckTypeAndMergeFrom(msg);
    InitVoiceTagInfo();
}

VR_VoiceTagAction::~VR_VoiceTagAction()
{
    VR_LOGI("m_nFuncCode=[%d]", m_nFuncCode);
    VoiceVector<int>::type().swap(m_vecVoiceTagID);
    VoiceVector<VoiceTagPhoneInfo>::type().swap(m_vecVoiceTagPhoneInfo);
}

void VR_VoiceTagAction::InitVoiceTagInfo()
{
    VR_LOGD_FUNC();
    m_nFuncCode = -1;
    m_nDeleteCount = 0;
    m_nDelectCount = 0;
    m_nDelectAccumulationCount = 0;
    m_nClassification = 0;
    m_nDeviceAddressCharset = 0;
    m_nNameDataCharset = 0;
    m_nVoiceTagIDCount = 0;
    m_nVoiceTagPhoneInfoCount = 0;
    m_voiceTagResultCode = -1;
    const RequestVrVoiceTag* voiceTag = m_ReqMsg.get();
    if (voiceTag->has_funccode()) {
        m_nFuncCode = voiceTag->funccode();
    }
    m_nVoiceTagIDCount = voiceTag->voicetagid_size();
    VR_LOGI("m_nVoiceTagIDCount=[%d]", m_nVoiceTagIDCount);
    for (VR_INT32 i = 0; i < m_nVoiceTagIDCount; i++) {
        m_vecVoiceTagID.push_back(voiceTag->voicetagid(i));
    }
    if (voiceTag->has_voicetag()) {
        ReqVrVoiceTagInfo voiceTagInfo = voiceTag->voicetag();
        if (voiceTagInfo.has_classification()) {
            m_nClassification = voiceTagInfo.classification();
        }
        if (voiceTagInfo.has_delect_accumulation_count()) {
            m_nDelectAccumulationCount = voiceTagInfo.delect_accumulation_count();
        }
        if (voiceTagInfo.has_delect_count()) {
            m_nDelectCount = voiceTagInfo.delect_count();
        }
        if (voiceTagInfo.has_device_address()) {
            m_strDeviceAddress = voiceTagInfo.device_address();
        }
        if (voiceTagInfo.has_device_address_charset()) {
            m_nDeviceAddressCharset = voiceTagInfo.device_address_charset();
        }
        if (voiceTagInfo.has_name_data()) {
            m_strNameData = voiceTagInfo.name_data();
        }
        if (voiceTagInfo.has_name_data_charset()) {
            m_nNameDataCharset = voiceTagInfo.name_data_charset();
        }
        m_nVoiceTagPhoneInfoCount = voiceTagInfo.phone_infos_size();
        for (VR_UINT32 i = 0; i < m_nVoiceTagPhoneInfoCount; i++) {
            VoiceTagPhoneInfo voiceTagPhoneInfo;
            if (voiceTagInfo.phone_infos(i).has_phone_number()) {
                voiceTagPhoneInfo.phone_number = voiceTagInfo.phone_infos(i).phone_number();
            }
            if (voiceTagInfo.phone_infos(i).has_phone_number_len()) {
                voiceTagPhoneInfo.phone_number_len = voiceTagInfo.phone_infos(i).phone_number_len();
            }
            else {
                voiceTagPhoneInfo.phone_number_len = 0;
            }
            if (voiceTagInfo.phone_infos(i).has_phone_type()) {
                voiceTagPhoneInfo.phone_type = voiceTagInfo.phone_infos(i).phone_type();
            }
            else {
                voiceTagPhoneInfo.phone_type = 0;
            }
            m_vecVoiceTagPhoneInfo.push_back(voiceTagPhoneInfo);
        }
    }
}

void VR_VoiceTagAction::StopVoice(const VR_INT32 id)
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<navi::TTS::TtsStop> msg(navi::TTS::TtsStop::default_instance().New());
    msg->set_reqid(id);
    msg->mutable_base()->set_sender(EV_MODULE_VOICERECOG);
    msg->mutable_base()->set_onresponse(true);
    VR_EventSenderIF::GetSender()->SendEvent(EV_EVENT_TTS_STOP, msg.release());
}

void VR_VoiceTagAction::StopCurTtsAndBeep()
{
    VR_INT32 ttsId = VR_ActionTtsPlay::getLastActionId();
    VR_INT32 beepId = VR_ActionBeepPlay::getLastActionId();
    VR_LOGI("ttsId=[%d],beepId=[%d]", ttsId, beepId);
    StopVoice(ttsId);
    StopVoice(beepId);
}

void VR_VoiceTagAction::NotifyDeleteAction(const VR_INT32 id)
{
    VR_LOGI("id=[%d]", id);
    std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_DE2DM);
    loopMessage->mutable_m_de2dm()->set_id(id);
    loopMessage->mutable_m_de2dm()->set_vr_dm_action_type(ActionType_NOTIFY_VOICE_TAG_RESULT);
    VrNotifyVoiceTagResult* notifyVoiceTagResult = loopMessage->mutable_m_de2dm()->mutable_notify_voice_tag_result();
    notifyVoiceTagResult->set_sender("DM2DM");
    notifyVoiceTagResult->set_funccode(VrVoiceTagFunc_Faile);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

void VR_VoiceTagAction::Run()
{
    VR_String xml = "";
    switch (m_nFuncCode) {
    case VrVoiceTagFunc_AddNewRecord :
        {
            VR_LOGI("case VrVoiceTagFunc_AddNewRecord");
            m_sessionManger->CreateSession(m_strSession);
            int id = GetActionId();
            m_spVoiceTagQueue->GetVoiceTagActionIDs()->push_back(id);
            xml = VoiceTagRecordXml(ADD_RECORD_VOICETAG);
            m_sessionManger->SetCheckVoiceTagSession(m_spContext, m_strSession, xml, m_sender, m_nFuncCode, id);
            m_sessionManger->OpenSession(m_strSession);
        }
        break;
    case VrVoiceTagFunc_EditRecord :
        {
            VR_LOGI("case VrVoiceTagFunc_EditRecord");
            m_sessionManger->CreateSession(m_strSession);
            int id = GetActionId();
            m_spVoiceTagQueue->GetVoiceTagActionIDs()->push_back(id);
            xml = VoiceTagRecordXml(EDIT_RECORD_VOICETAG);
            m_sessionManger->SetCheckVoiceTagSession(m_spContext, m_strSession, xml, m_sender, m_nFuncCode, id);
            m_sessionManger->OpenSession(m_strSession);
        }
        break;
    case VrVoiceTagFunc_Register :
        {
            VR_LOGI("case VrVoiceTagFunc_Register");
            int id = GetActionId();
            m_spVoiceTagQueue->GetVoiceTagActionIDs()->push_back(id);
            ReplyVoiceTagResult(true);
            xml = VoiceTagSaveXml();
            SendMessage2Engine(xml);
        }
        break;
    case VrVoiceTagFunc_Delete :
        {
            VR_LOGI("case VrVoiceTagFunc_Delete");
            ReplyVoiceTagResult(true);
            for (VR_INT32 i = 0; i < m_nVoiceTagIDCount; ++i) {
                xml = VoiceTagDeleteXml(m_vecVoiceTagID.at(i));
                SendMessage2Engine(xml);
            }
        }
        break;
    case VrVoiceTagFunc_DeleteAll :
        {
            VR_LOGI("case VrVoiceTagFunc_DeleteAll");
            ReplyVoiceTagResult(true);
            xml = VoiceTagDeleteXml(INVALID_VALUE);
            SendMessage2Engine(xml);
        }
        break;
    case VrVoiceTagFunc_Play :
        {
            VR_LOGI("case VrVoiceTagFunc_Play");
            m_sessionManger->CreateSession(m_strSession);
            int id = GetActionId();
            m_spVoiceTagQueue->GetVoiceTagActionIDs()->push_back(id);
            int count = m_spVoiceTagQueue->GetVoiceTagActionIDs()->size();
            VR_LOGI("VrVoiceTagFunc_Play, VoiceTagActionID count=[%d]", count);
            xml = VoiceTagPlayBackXml(PLAY_VOICETAG);
            m_sessionManger->SetCheckVoiceTagSession(m_spContext, m_strSession, xml, m_sender, m_nFuncCode, id);
            m_sessionManger->OpenSession(m_strSession);
        }
        break;
    case VrVoiceTagFunc_Stop :
        {
            VR_LOGI("case VrVoiceTagFunc_Stop");
            ReplyVoiceTagResult(true);
            xml = VoiceTagPlayBackXml(STOP_VOICETAG);
            SendMessage2Engine(xml);
            if (VR_SessionManger::VR_SourceStatus_Openning == m_sessionManger->GetSourceStatus()) {
                m_sessionManger->SetVoiceTagCancelStatus(true);
            }
        }
        break;
    case VrVoiceTagFunc_CancelRecord :
        {
            VR_LOGI("case VrVoiceTagFunc_CancelRecord");
            ReplyVoiceTagResult(true);
            xml = VoiceTagCancelXml(CANCEL_RECORD_VOICETAG);
            SendMessage2Engine(xml);
            if (VR_SessionManger::VR_SourceStatus_Openning == m_sessionManger->GetSourceStatus()) {
                m_sessionManger->SetVoiceTagCancelStatus(true);
            }
        }
        break;
    case VrVoiceTagFunc_CancelRegister :
        {
            VR_LOGI("case VrVoiceTagFunc_CancelRegister");
            ReplyVoiceTagResult(true);
            xml = VoiceTagCancelXml(CANCEL_SAVE_VOICETAG);
            SendMessage2Engine(xml);
        }
        break;
    default :
        {
            VR_LOGI("default");
            ReplyVoiceTagResult(false);
            NotifyDeleteAction(GetActionId());
        }
        break;
    }
}

void VR_VoiceTagAction::OnReceive(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();
    ReplyVoiceTagNotification(msg);
}

bool VR_VoiceTagAction::ReplyRequest(VR_ProtoMessageBase* respMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VR_ProtoMessageBase> msg(respMsg);
    if (NULL != m_spContext) {
        EV_EventSender().Reply(*m_spContext, msg.release());
    }
    return (NULL != m_spContext);
}

void VR_VoiceTagAction::SetSessionManager(spVR_SessionManger spSessionManager)
{
    m_sessionManger = spSessionManager;
}

void VR_VoiceTagAction::SendMessage2Engine(const std::string &msg)
{
    VR_LOGI("msg=[%s]", msg.c_str());

    m_sender->sendEngineReply(-1, msg);
}

VR_String VR_VoiceTagAction::VoiceTagRecordXml(const VR_String& xml)
{
    VR_String strXml = "<event name=\"%1%\">"
                           "<actionId>%2%</actionId>"
                           "<deviceAddress>%3%</deviceAddress>"
                           "<voiceTagId>%4%</voiceTagId>"
                           "%5%"
                       "</event>";
    VR_String contactNameXml = "<contact name=\"%1%\">"
                                   "%2%"
                               "</contact>";
    VR_String phoneItemXml = "<phone_item type=\"%1%\" number=\"%2%\"/>";

    VR_String phoneItem = "";
    for (VR_UINT32 i = 0; i < m_nVoiceTagPhoneInfoCount; ++i) {
        VR_UINT32 phoneItemType = m_vecVoiceTagPhoneInfo.at(i).phone_type;
        VR_String phoneItemNumber = m_vecVoiceTagPhoneInfo.at(i).phone_number;
        phoneItem += (boost::format(phoneItemXml) % phoneItemType % phoneItemNumber).str();
    }

    VR_String contactName = (boost::format(contactNameXml) % VR_CommonTool::Encode(m_strNameData) % phoneItem).str();
    int id = GetActionId();
    VR_String voiceTagInfo = (boost::format(strXml) % xml % id % VR_CommonTool::Encode(m_strDeviceAddress) % m_vecVoiceTagID.at(0) % contactName).str();

    VR_LOGI("VoiceTagRecordXml=[%s]", voiceTagInfo.c_str());
    return voiceTagInfo;
}

VR_String VR_VoiceTagAction::VoiceTagSaveXml()
{
    VR_String strXml = "<event name=\"saveVoiceTag\">"
                           "<actionId>%1%</actionId>"
                           "<voiceTagId>%2%</voiceTagId>"
                       "</event>";

    int id = GetActionId();
    VR_String voiceTagInfo = (boost::format(strXml) % id % m_vecVoiceTagID.at(0)).str();

    VR_LOGI("VoiceTagSaveXml=[%s]", voiceTagInfo.c_str());
    return voiceTagInfo;
}

VR_String VR_VoiceTagAction::VoiceTagDeleteXml(const VR_INT32& voiceTagID)
{
    VR_String voiceTagInfo = "";
    VR_String strXml = "<event name=\"deleteVoiceTag\">"
                           "<actionId>%1%</actionId>"
                           "<deviceAddress>%2%</deviceAddress>"
                           "<voiceTagId>%3%</voiceTagId>"
                       "</event>";

    int id = GetActionId();
    if (INVALID_VALUE == voiceTagID) {
        voiceTagInfo = (boost::format(strXml) % id % VR_CommonTool::Encode(m_strDeviceAddress) % "").str();
    }
    else {
        voiceTagInfo = (boost::format(strXml) % id % VR_CommonTool::Encode(m_strDeviceAddress) % voiceTagID).str();
    }

    VR_LOGI("VoiceTagDeleteXml=[%s]", voiceTagInfo.c_str());
    return voiceTagInfo;
}

VR_String VR_VoiceTagAction::VoiceTagPlayBackXml(const VR_String& voiceTagFunc)
{
    VR_String strXml = "<event name=\"%1%\">"
                           "<actionId>%2%</actionId>"
                           "<voiceTagId>%3%</voiceTagId>"
                       "</event>";

    int id = GetActionId();
    VR_String voiceTagInfo = (boost::format(strXml) % voiceTagFunc % id % m_vecVoiceTagID.at(0)).str();

    VR_LOGI("VoiceTagPlayBackXml=[%s]", voiceTagInfo.c_str());
    return voiceTagInfo;
}

VR_String VR_VoiceTagAction::VoiceTagCancelXml(const VR_String& voiceTagFunc)
{
    VR_String strXml = "<event name=\"%1%\">"
                           "<actionId>%2%</actionId>"
                       "</event>";

    int id = GetActionId();
    VR_String voiceTagInfo = (boost::format(strXml) % voiceTagFunc % id).str();

    VR_LOGI("VoiceTagCancelXml=[%s]", voiceTagInfo.c_str());
    return voiceTagInfo;
}

void VR_VoiceTagAction::ReplyVoiceTagResult(bool bResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrVoiceTagResult> voiceTagResult(RespVrVoiceTagResult::default_instance().New());
    voiceTagResult->set_sender("DM2FC");
    voiceTagResult->set_funccode((VrVoiceTagFunc)m_nFuncCode);
    voiceTagResult->set_result(bResult);
    ReplyRequest(voiceTagResult.release());
}

void VR_VoiceTagAction::ReplyVoiceTagNotification(const VR_ProtoMessageBase& message)
{
    VR_LOGI("funccode=[%d]", m_nFuncCode);
    m_ReqMsgNotify->set_sender("DM2FC");

    const VrNotifyVoiceTagResult& msg = (const VrNotifyVoiceTagResult&) message;
    if (VrVoiceTagFunc_Faile == msg.funccode()) {
        SetActionComplete();
        return;
    }

    if (!m_ReqMsgNotify->has_funccode()) {
        m_ReqMsgNotify->set_funccode((VrVoiceTagFunc)m_nFuncCode);
    }

    if (msg.has_playresult()) {
        VR_LOGI("msg.has_playresult()");
        if (msg.playresult().has_result()) {
            VR_LOGI("playresult=[%d]", msg.playresult().result());
            m_ReqMsgNotify->mutable_playresult()->set_result(msg.playresult().result());
        }
        else {
            m_ReqMsgNotify->mutable_playresult()->set_result(false);
        }

        if ((VrVoiceTagFunc_Play == m_nFuncCode) && msg.playresult().result() && (Session_VoiceTag == m_sessionManger->GetSessionType())) {
            m_sessionManger->SetVoiceTagStatus(-1, true);
        }
        else {
            m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYVOICETAGRESULT, m_ReqMsgNotify.release());
        }

        SetActionComplete();
    }
    else if (msg.has_registerresult()) {
        VR_LOGI("msg.has_registerresult()");
        if ((VrVoiceTagFunc_DeleteAll != m_nFuncCode)
            && (VrVoiceTagFunc_CancelRecord != m_nFuncCode)
            && (VrVoiceTagFunc_CancelRegister != m_nFuncCode)) {
            VR_LOGI("voicetagID=[%d]", msg.registerresult().voicetagid(0));
            m_ReqMsgNotify->mutable_registerresult()->add_voicetagid(msg.registerresult().voicetagid(0));
        }
        VR_LOGI("msg.registerresult().result()=[%d]", msg.registerresult().result());
        m_voiceTagResultCode = msg.registerresult().result();

        VR_LOGI("idSize=[%d],m_nVoiceTagIDCount=[%d]", m_ReqMsgNotify->mutable_registerresult()->voicetagid_size(), m_nVoiceTagIDCount);
        if ((VrVoiceTagFunc_AddNewRecord == m_nFuncCode) || (VrVoiceTagFunc_EditRecord == m_nFuncCode)) {
            if ((m_voiceTagResultCode == VrNotifyRegisterResult_VrVoiceTagResultCode_Recorded) && (Session_VoiceTag == m_sessionManger->GetSessionType())) {
                m_sessionManger->SetVoiceTagStatus(msg.registerresult().voicetagid(0), true);
            }
            else {
                m_ReqMsgNotify->mutable_registerresult()->set_result((VrNotifyRegisterResult_VrVoiceTagResultCode)m_voiceTagResultCode);
                m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYVOICETAGRESULT, m_ReqMsgNotify.release());
            }
            SetActionComplete();
        }
        else {
            m_ReqMsgNotify->mutable_registerresult()->set_result((VrNotifyRegisterResult_VrVoiceTagResultCode)m_voiceTagResultCode);
            m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYVOICETAGRESULT, m_ReqMsgNotify.release());
            SetActionComplete();
        }
    }
    else {
        VR_LOGI("VrNotifyVoiceTagResult info error");
        m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYVOICETAGRESULT, m_ReqMsgNotify.release());
        SetActionComplete();
    }
}

void VR_VoiceTagAction::DeleteVoiceTagAction()
{
    int count = m_spVoiceTagQueue->GetVoiceTagActionIDs()->size();
    VR_LOGI("before delete, VoiceTagActionID count=[%d]", count);
    for (int i = 0; i < count; ++i) {
        int id = m_spVoiceTagQueue->GetVoiceTagActionIDs()->back();
        if (id != GetActionId()) {
            NotifyDeleteAction(id);
        }
    }
    m_spVoiceTagQueue->GetVoiceTagActionIDs()->clear();
    VR_LOGI("after delete, VoiceTagActionID count=[%d]", m_spVoiceTagQueue->GetVoiceTagActionIDs()->size());
}
/* EOF */
