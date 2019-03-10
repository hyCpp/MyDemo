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
* @file  VR_VoiceTagHandler.cpp
* @brief Declaration file of class VR_VoiceTagHandler.
*
* This file includes the declaration of class VR_VoiceTagHandler.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "boost/format.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_VoiceTagHandler.h"
#include "settingservice/NCSetting.h"
#include "VR_RunableThread.h"
#include "settingservice/NCSettingEventDefine.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "commu/NDBtPhonebook.pb.h"
#include <contentresolver/NCContentResolver.h>
#include "VR_PhoneBookXMLFormat.h"
#include "VR_CommonTool.h"

const nutshell::NCString BT_SPACE     =  nutshell::NCString("btservice.pb");
const nutshell::NCString BT_IDENTITY  =  nutshell::NCString("NCBtPb_SyncVoiceTags");
const nutshell::NCString BT_UPDATE_IDENTITY  =  nutshell::NCString("NCBtPb_UpdateVoiceTag");
const nutshell::NCString VR_SPACE     =  nutshell::NCString("vrservice.pb");
const nutshell::NCString VR_IDENTITY  =  nutshell::NCString("NCVrPb_SyncVoiceTags");
const VR_INT32 MAX_VOICE_TAG_COUNT = 50;
const VR_INT32 TOTAL_COUNT = 100;

using namespace navi::VoiceRecog;
using namespace navi::dataprovider;
using namespace nutshell;

VR_VoiceTagEventHandler::VR_VoiceTagEventHandler(VR_VoiceTagHandler *pVoiceTagHandle, const nutshell::NCRunnableLooper &looper)
    : nutshell::NCEventHandler(looper),
      m_pVoiceTag(pVoiceTagHandle)
{

}

VR_VoiceTagEventHandler::~VR_VoiceTagEventHandler()
{

}

void VR_VoiceTagEventHandler::handleEvent(const nutshell::NCEvent &event)
{
    VR_LOGD_FUNC();
    nutshell::NCString  evtSpace = event.space();
    if (-1 == evtSpace.find(BT_SPACE.getString())) {
        VR_LOGI("Event space error : [%s]", evtSpace.getString());
        return;
    }

    nutshell::NCString  ncIdentity("");
    event.identity(&ncIdentity);

    if (BT_IDENTITY == ncIdentity) {
        VR_String startXml = "<event name=\"syncVoiceTag\">"
                                  "<deviceAddress>%1%</deviceAddress>"
                                  "<voiceTagIds>%2%</voiceTagIds>"
                                  "<contactIds>%3%</contactIds>"
                              "</event>";
        VR_String strVoiceTags;
        VR_String strContacts;
        VR_String strVoiceTagIds;
        VR_String strContactIds;
        VR_INT32 nDeviceId = event.arg1();
        VR_INT32 nCount = event.arg2();
        VR_LOGI("nDeviceId=[%d],nCount=[%d]", nDeviceId, nCount);
        VR_INT32 nVoiceTagCount = (nCount > MAX_VOICE_TAG_COUNT) ? MAX_VOICE_TAG_COUNT : nCount;
        VR_INT32 nVoiceTagBuf[TOTAL_COUNT];
        event.getExtraData(nVoiceTagBuf, sizeof(nVoiceTagBuf));
        for (int i = 0; i < nVoiceTagCount; i++) {
            VR_String tmp = std::to_string(nVoiceTagBuf[i]);
            strVoiceTags = strVoiceTags + tmp + ",";
        }
        if (!strVoiceTags.empty()) {
            int len = strVoiceTags.length();
            strVoiceTagIds = strVoiceTags.substr(0, len - 1);
        }
        for (int i = MAX_VOICE_TAG_COUNT; i < MAX_VOICE_TAG_COUNT + nVoiceTagCount; i++) {
            VR_String tmp = std::to_string(nVoiceTagBuf[i]);
            strContacts = strContacts + tmp + ",";
        }
        if (!strContacts.empty()) {
            int len = strContacts.length();
            strContactIds = strContacts.substr(0, len - 1);
        }
        VR_String xmlResult = (boost::format(startXml) % nDeviceId % strVoiceTagIds % strContactIds).str();
        VR_LOGI("xmlResult=[%s]", xmlResult.c_str());

        std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_DM2DE);
        loopMessage->mutable_m_vrsendmsgdm2de()->set_msg(xmlResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    else if (BT_UPDATE_IDENTITY == ncIdentity) {
        VR_LOGI("BT_UPDATE_IDENTITY == ncIdentity");
        nutshell::commu::NIBtVoiceTagEntry  btVoiceTagEntry;
        size_t size = event.extraDataSize();
        char* buff = VR_new char[size + 1];
        if (NULL == buff) {
            VR_LOGI("new buff failed");
            return;
        }
        memset(buff, 0x00, size + 1);
        VR_INT32 realSize = event.getExtraData(buff, size);
        if (realSize <= 0) {
            VR_LOGI("get buff size[%u], real size[%d]", size, realSize);
            delete[] buff;
            buff = NULL;
            return;
        }
        if (!btVoiceTagEntry.ParseFromArray(buff, size)) {
            VR_LOGI("ParseFromString failed.");
            delete[] buff;
            buff = NULL;
            return;
        }
        VR_String xml = "<event name=\"updateVoiceTag\">"
                                        "<voiceTagId>%1%</voiceTagId>"
                                        "<contact name=\"%2%\">"
                                            "<phone_item type=\"%3%\" number=\"%4%\"/>"
                                            "<phone_item type=\"%5%\" number=\"%6%\"/>"
                                            "<phone_item type=\"%7%\" number=\"%8%\"/>"
                                            "<phone_item type=\"%9%\" number=\"%10%\"/>"
                                         "</contact>"
                                     "</event>";
        VR_String resultXml = (boost::format(xml) % btVoiceTagEntry.voicetag() % VR_CommonTool::Encode(btVoiceTagEntry.name())
                     % btVoiceTagEntry.numbertype1() % btVoiceTagEntry.number1()
                     % btVoiceTagEntry.numbertype2() % btVoiceTagEntry.number2()
                     % btVoiceTagEntry.numbertype3() % btVoiceTagEntry.number3()
                     % btVoiceTagEntry.numbertype4() % btVoiceTagEntry.number4()).str();
        VR_LOGI("resultXml=[%s]", resultXml.c_str());

        std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_DM2DE);
        loopMessage->mutable_m_vrsendmsgdm2de()->set_msg(resultXml);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());

        delete[] buff;
        buff = NULL;
    }
    else {
        VR_LOGI("Not Find space identity: [%s]", ncIdentity.getString());
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// VR_VoiceTagHandler
VR_VoiceTagHandler::VR_VoiceTagHandler(spVR_RunableThread  spRunnableThread, spVR_DialogEngineIF dialogEngine)
    : m_spRunnableThread(spRunnableThread)
    , m_spDialogEngine(dialogEngine)
    , m_pcEventHandler(NULL)
{
    m_filter.clear();
}

VR_VoiceTagHandler::~VR_VoiceTagHandler()
{

}

spVR_DialogEngineIF VR_VoiceTagHandler::GetDialogEngine()
{
    return m_spDialogEngine;
}

void VR_VoiceTagHandler::Start()
{
    VR_LOGD_FUNC();
    m_filter.push_back(m_context.buildEventFullName(BT_SPACE, nutshell::NCString(BT_IDENTITY)));
    m_filter.push_back(m_context.buildEventFullName(BT_SPACE, nutshell::NCString(BT_UPDATE_IDENTITY)));
    m_pcEventHandler = VR_new VR_VoiceTagEventHandler(this, m_spRunnableThread->getRunableLooper());
    m_context.registerHandler(m_pcEventHandler, m_filter);

    std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_GetVoiceTagInfo);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

void VR_VoiceTagHandler::Stop()
{
    VR_LOGD_FUNC();
    m_context.unregisterHandler(m_pcEventHandler, m_filter);
    m_filter.clear();
}

void VR_VoiceTagHandler::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGI("msg.GetTypeName()=[%s]", msg.GetTypeName().c_str());
    if ("navi.VoiceRecog.VrEngineControlMessage.VrGetVoiceTagInfo" == msg.GetTypeName()) {
        SendVoiceTagInfo();
    }
    else if ("navi.VoiceRecog.VrSyncVoiceTagResult" == msg.GetTypeName()) {
        const VrSyncVoiceTagResult& voiceTagResult = (const VrSyncVoiceTagResult&)msg;
        if (!voiceTagResult.has_device_id()) {
            VR_LOGI("!voiceTagResult.has_device_id()");
            return;
        }
        VR_INT32 device_id = voiceTagResult.device_id();
        VR_INT32 voiceTagCount = voiceTagResult.voicetag_id_size();
        VR_LOGI("voiceTagCount=[%d]", voiceTagCount);
        if (0 == voiceTagCount) {
            return;
        }
        VR_INT32 nVoiceTagBuf[MAX_VOICE_TAG_COUNT];
        memset(nVoiceTagBuf, 0, sizeof(nVoiceTagBuf));
        VR_INT32 nVoiceTagCount = (voiceTagCount > MAX_VOICE_TAG_COUNT) ? MAX_VOICE_TAG_COUNT : voiceTagCount;
        for (int i = 0; i < nVoiceTagCount; i++) {
            nVoiceTagBuf[i] = voiceTagResult.voicetag_id(i);
        }
        nutshell::NCEvSysContext context;
        nutshell::NCEvMessageIF* eventSender = context.queryEventSender();
        nutshell::NCEvent* event = context.obtainEvent(VR_SPACE, VR_IDENTITY, device_id, nVoiceTagCount);
        if ((NULL != event) && (NULL != eventSender)) {
            event->setExtraData(nVoiceTagBuf, sizeof(nVoiceTagBuf));
            if (nutshell::NC_FALSE == eventSender->sendEvent(event)) {
                VR_LOGI("send event error!");
            }
        }
        else {
            VR_LOGI("event error!");
        }
        context.releaseEvent(event);
    }
}

void VR_VoiceTagHandler::SendVoiceTagInfo()
{
    // NCString where;
    // NCObjList<NCString> whereArgs;
    // NCString sortOrder;
    // NCContentResolver resolver;
    // NCList<NCString> columns;
    // VR_String strSyncVoiceTagXml = "<event name=\"syncVoiceTag\">"
    //                                                           "<deviceAddress>%1%</deviceAddress>"
    //                                                           "<voiceTagIds>%2%</voiceTagIds>"
    //                                                           "<contactIds>%3%</contactIds>"
    //                                                       "</event>";
    // VR_String strUpdateVoiceTagXml = "<event name=\"updateVoiceTag\">"
    //                                                                 "<voiceTagId>%1%</voiceTagId>"
    //                                                                 "<contact name=\"%2%\">"
    //                                                                     "<phone_item type=\"%3%\" number=\"%4%\"/>"
    //                                                                     "<phone_item type=\"%5%\" number=\"%6%\"/>"
    //                                                                     "<phone_item type=\"%7%\" number=\"%8%\"/>"
    //                                                                     "<phone_item type=\"%9%\" number=\"%10%\"/>"
    //                                                                  "</contact>"
    //                                                              "</event>";


    // columns << &VR_PB_QUERY_CONTACTS_INFO_INDEX__ID
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_CONTACT_ID
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_F_NAME
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_L_NAME
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_1
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_1
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_1
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_2
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_2
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_2
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_3
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_3
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_3
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_TYPE_4
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_INDEX_4
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_NUM_4
    //         << &VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_VOICE_TAG;

    // whereArgs << VR_new NCString("-1");
    // where.format("%s<>?", VR_PB_QUERY_CONTACTS_INFO_INDEX_PHONE_VOICE_TAG.getString());

    // NCCursor* cursor = resolver.query(NCUri(VR_PB_CONTACT_DATA_INFO_URI), columns
    //                                   , where, whereArgs, sortOrder);
    // if (NULL == cursor) {
    //     VR_LOGI("[query url[%s] error, cursor = null", VR_PB_CONTACT_DATA_INFO_URI.getString());
    //     return;
    // }

    // VR_INT32 nDeviceId = GetDeviceId();
    // if (0 == cursor->getCount()) {
    //     if (0 != nDeviceId) {
    //         VR_String strResult = (boost::format(strSyncVoiceTagXml) % nDeviceId % "" % "").str();
    //         SendMsg2DE(strResult);
    //     }
    // }
    // else if (cursor->getCount() > 0 && cursor->moveToFirst()) {
    //     VR_LOGI(" count : %d", cursor->getCount());
    //     VR_String strVoiceTagID = "";
    //     VR_String strContactID = "";
    //     do {
    //         INT nContactId = cursor->getInt64(CONTACT_TABLE_FIELD_CONTACT_ID);
    //         NCString strFirstName = cursor->getString(CONTACT_TABLE_FIELD_FIRSTNAME);
    //         NCString strLastName = cursor->getString(CONTACT_TABLE_FIELD_LASTNAME);
    //         INT nPhoneType1 = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE1_TYPE);
    //         INT nPhoneIndex1 = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE1_INDEX);
    //         NCString strPhoneNum1 = cursor->getString(CONTACT_TABLE_FIELD_PHONE1_NUMBER);
    //         INT nPhoneType2 = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE2_TYPE);
    //         INT nPhoneIndex2 = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE2_INDEX);
    //         NCString strPhoneNum2 = cursor->getString(CONTACT_TABLE_FIELD_PHONE2_NUMBER);
    //         INT nPhoneType3 = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE3_TYPE);
    //         INT nPhoneIndex3 = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE3_INDEX);
    //         NCString strPhoneNum3 = cursor->getString(CONTACT_TABLE_FIELD_PHONE3_NUMBER);
    //         INT nPhoneType4 = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE4_TYPE);
    //         INT nPhoneIndex4 = cursor->getInt64(CONTACT_TABLE_FIELD_PHONE4_INDEX);
    //         NCString strPhoneNum4 = cursor->getString(CONTACT_TABLE_FIELD_PHONE4_NUMBER);
    //         INT nVoiceTagId = cursor->getInt64(CONTACT_TABLE_FIELD_VOICE_TAG_ID);
    //         VR_LOGI("nPhoneType1=[%d],nPhoneType2=[%d],nPhoneType3=[%d],nPhoneType4=[%d],nPhoneIndex1=[%d],"
    //             "nPhoneIndex2=[%d],nPhoneIndex3=[%d],nPhoneIndex4=[%d]", nPhoneType1, nPhoneType2, nPhoneType3,
    //             nPhoneType4, nPhoneIndex1, nPhoneIndex2, nPhoneIndex3, nPhoneIndex4);
    //         VR_LOGI(" strFirstName=[%s], strLastName=[%s]", strFirstName.getString(), strLastName.getString());
    //         VR_String strContactName;
    //         if ((0 != strFirstName.getLength()) && (0 != strLastName.getLength())) {
    //             strContactName = strFirstName + " " + strLastName;
    //         }
    //         else {
    //             strContactName = strFirstName + strLastName;
    //         }
    //         VR_String strResult = (boost::format(strUpdateVoiceTagXml) % nVoiceTagId % VR_CommonTool::Encode(strContactName)
    //             % TransformPhoneType(nPhoneType1, nPhoneIndex1) % strPhoneNum1 % TransformPhoneType(nPhoneType2, nPhoneIndex2)  % strPhoneNum2
    //             % TransformPhoneType(nPhoneType3, nPhoneIndex3) % strPhoneNum3 % TransformPhoneType(nPhoneType4, nPhoneIndex4) % strPhoneNum4).str();
    //         SendMsg2DE(strResult);

    //         strVoiceTagID +=  "," + std::to_string(nVoiceTagId);
    //         strContactID +=  "," + std::to_string(nContactId);
    //     } while (cursor->moveToNext());

    //     if (0 != nDeviceId) {
    //         strVoiceTagID = strVoiceTagID.substr(1, strVoiceTagID.length() - 1);
    //         strContactID = strContactID.substr(1, strContactID.length() - 1);
    //         VR_String strResult = (boost::format(strSyncVoiceTagXml) % nDeviceId % strVoiceTagID % strContactID).str();
    //         SendMsg2DE(strResult);
    //     }
    // }
    // delete cursor;
    // cursor = NULL;
}

VR_INT32 VR_VoiceTagHandler::GetDeviceId()
{
    VR_INT32 nDeviceId = 0;
    NCString deviceIdValue;
    NCUri uri("content://system/info?bt/hfp-curdev");
    NCContentResolver resolver;
    ncsp<NCCursor>::sp queryResult = resolver.query(uri, NCList<NCString>(), NCString(),
                                                    NCList<NCString>(), NCString());
    if ((queryResult != NULL) && (1 == queryResult->getCount()) && (queryResult->moveToFirst())) {
        deviceIdValue = queryResult->getString(0);
    }

    nDeviceId = atoi(deviceIdValue.getString());
    VR_LOGI("nDeviceId=[%d]", nDeviceId);

    return nDeviceId;
}

VR_INT32 VR_VoiceTagHandler::TransformPhoneType(VR_INT32 nPhoneType, VR_INT32 nPhoneIndex)
{
    if (-1 != nPhoneType) {
        nPhoneType = nPhoneType + nPhoneIndex * 4;
    }
    VR_LOGI("nPhoneType=[%d]", nPhoneType);

    return nPhoneType;
}

void VR_VoiceTagHandler::SendMsg2DE(VR_String &strXml)
{
    VR_LOGI("strXml=[%s]", strXml.c_str());
    if (GetDialogEngine()) {
        GetDialogEngine()->SendMessage(strXml);
    }
}
/* EOF */
