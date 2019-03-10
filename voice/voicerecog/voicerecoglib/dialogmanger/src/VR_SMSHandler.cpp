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

#include <sstream>
#include <time.h>
#include <ncore/NCUri.h>
#include "stdafx.h"
#include "boost/format.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "VR_SMSHandler.h"
#include "VR_RunableThread.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "EV_EventSender.h"
#include "VR_PhoneBookListener.h"

#include <contentresolver/NCContentResolver.h>
#include "VR_XmlStrParse.h"
#include "VR_DataProviderComm.h"
#include <boost/format.hpp>
#include "VR_Indentifier.hpp"
#include "CL_AutoSync.h"

using namespace navi::VoiceRecog;
using namespace navi::dataprovider;
using namespace nutshell;

const std::string cstAreaCode = "+86";
const std::string SMS_BODY_XML =
        "<action agent=\"phone\" op=\"getMessageDetail\">"\
            "<instanceId>%1%</instanceId>"\
            "<messageId>%2%</messageId>"\
        "</action>";

VR_SMSHandler::VR_SMSHandler()
    : m_iIndex(0)
    , m_bToday(false)
{
}

VR_SMSHandler::~VR_SMSHandler()
{
}

void VR_SMSHandler::GetSMSMessage(const VR_String& strXml)
{
    VR_LOGD_FUNC();
    VR_LOGD("%s", strXml.c_str());
    CL_AutoSync cAutoSync(m_cSync);

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    m_iIndex = atoi(xmlStrParse.GetXMLValue("msg_index").c_str());
    if (0 != m_iIndex) {
        VR_LOGD("get message body by index[%d]", m_iIndex);
        RequestMsgBody();
        return;
    }
    m_lstQueryMsgInfo.clear();
    m_lstMsgInfo.clear();

    m_strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    m_strSMSType = xmlStrParse.GetXMLValue("sms_type");
    m_strContactName = xmlStrParse.GetXMLValue("contact_name");
    m_strContactId = xmlStrParse.GetXMLValue("contact_id");
    m_strPhoneNumber = xmlStrParse.GetXMLValue("phone_number");
    m_bToday = ("true" == xmlStrParse.GetXMLValue("is_today")) ? true : false;

    GetTodayInfo();
    int iReuslt = GetSMSMessageInfo();
    if (iReuslt < 0) {
        ResponseMessageInfo(false);
    }
    else if (iReuslt == 0) {
        ResponseMessageInfo(true);
    }
    else {
        VR_LOGD("Wait for BT to get message body");
        RequestMsgBody();
    }
}

void VR_SMSHandler::GetTodayInfo()
{
    VR_LOGD_FUNC();
    m_strDate = "";

    if (m_bToday) {
        timespec time;
        clock_gettime(CLOCK_REALTIME, &time);
        tm nowTime;
        localtime_r(&time.tv_sec, &nowTime);
        char current[16] = {};
        snprintf(current, sizeof(current), "%04d%02d%02d", nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday);
        m_strDate = current;
    }
}

int VR_SMSHandler::GetSMSMessageInfo()
{
    VR_LOGD_FUNC();
    NCString where;
    NCObjList<NCString> whereArgs;
    if (!GetMessageCondition(where, whereArgs)) {
        return -1;
    }

    NCList<NCString> recentMsgList;
    recentMsgList
            << &VR_PB_QUERY_MSGHIST_INFO_INDEX_HANDLE
            << &VR_PB_QUERY_MSGHIST_INFO_INDEX_INSTANCEID
            << &VR_PB_QUERY_MSGHIST_INFO_INDEX_TIME
            << &VR_PB_QUERY_MSGHIST_INFO_INDEX_ADDRESS
            << &VR_PB_QUERY_MSGHIST_INFO_INDEX_NAME;

    NCString sortOrder("time desc");
    NCContentResolver resolver;
    NCCursor* cursor = resolver.query(NCUri(VR_PHONE_SMS_MESSAGE_URI), recentMsgList,
                                            where, whereArgs, sortOrder);

    if (NULL == cursor) {
        VR_LOGD("query error");
        return -1;
    }

    INT rowNum = cursor->getCount();
    VR_LOGD("the number of messages is %d", rowNum);
    if (0 == rowNum) {
        VR_LOGD("message size is 0");
        return 0;
    }
    else {
        if (NC_FALSE == cursor->moveToFirst()) {
            VR_LOGD("cursor->moveToFirst() size is false");
            delete cursor;
            cursor = NULL;
            return -1;
        }

        VR_MessageInfo msgInfo;
        for (INT i = 0; i < rowNum; ++i) {
            msgInfo.strMessageId = cursor->getString(MESSAGE_TABLE_FIELD_HANDLE);
            msgInfo.iInstanceId = cursor->getInt64(MESSAGE_TABLE_FIELD_INSTANCEID);
            if (VR_SMS_TYPE_NUMBER_UNREAD == m_strSMSType || VR_SMS_TYPE_NUMBER_READ == m_strSMSType) {
                msgInfo.strNumber = m_strPhoneNumber.c_str();
            }
            else {
                msgInfo.strNumber = cursor->getString(MESSAGE_TABLE_FIELD_ADDRESS);
            }
            RemoveAreaCode(msgInfo.strNumber);
            msgInfo.strName = cursor->getString(MESSAGE_TABLE_FIELD_NAME);

            // improve the information about date and time and contact
            ImproveMessageInfo(msgInfo, cursor->getString(MESSAGE_TABLE_FIELD_TIME).getString());

            m_lstMsgInfo.push_back(msgInfo);

            cursor->moveToNext();
        }
    }
    delete cursor;
    cursor = NULL;

    if (VR_SMS_TYPE_CONTACT_UNREAD == m_strSMSType || VR_SMS_TYPE_CONTACT_READED == m_strSMSType) {
        UpdateData();
    }

    return (int)m_lstMsgInfo.size();
}

void VR_SMSHandler::UpdateData()
{
    myPersonInfo personInfo;

    // get the numbers by contact id
    VR_PhoneBookListener::GetInstance()->getPhoneNumber(atoi(m_strContactId.c_str()), personInfo);

    std::string strNumber;
    std::string strContactNum;
    auto iterMsg = m_lstMsgInfo.begin();
    while (iterMsg != m_lstMsgInfo.end()) {
        strNumber = iterMsg->strNumber.getString();
        bool bExit = false;

        // check the number wether in the vector
        for (auto iterNumber = personInfo.lstPhoneNum.cbegin();
             iterNumber != personInfo.lstPhoneNum.cend();
             ++iterNumber) {
            strContactNum = iterNumber->number.getString();
            if (CheckNumber(strNumber, strContactNum)) {
                bExit = true;
                break;
            }
        }

        if (bExit) {
            // the number is exit
            ++iterMsg;
        }
        else {
            // the number is not the contact's, remove the message info
            m_lstMsgInfo.erase(iterMsg++);
        }
    }
}

bool VR_SMSHandler::CheckNumber(const VR_String& strNum, const VR_String& strContactNum)
{
    if (strNum.length() == 0) {
        return false;
    }

    VR_LOGD("Address: %s; phonenumber:%s", strNum.c_str(), strContactNum.c_str());
    if (strContactNum == strNum) {
        return true;
    }
    else {
        return (VR_String(cstAreaCode.c_str() + strNum) == strContactNum);
    }
}

bool VR_SMSHandler::GetMessageCondition(NCString& where, NCObjList<NCString>& whereArgs)
{
    VR_LOGD_FUNC();
    if (VR_SMS_TYPE_ALL == m_strSMSType) {
        GetMessageConditionAll(where, whereArgs);
    }
    else if (VR_SMS_TYPE_ALL_READED == m_strSMSType) {
        GetMessageConditionAll(where, whereArgs, true);
    }
    else if (VR_SMS_TYPE_ALL_UNREADED == m_strSMSType) {
        GetMessageConditionAll(where, whereArgs, false);   
    }
    else if (VR_SMS_TYPE_CONTACT_READED == m_strSMSType) {
        GetMessageConditionByContact(where, whereArgs, true);  
    }
    else if (VR_SMS_TYPE_CONTACT_UNREAD == m_strSMSType) {
        GetMessageConditionByContact(where, whereArgs, false);
    }
    else if (VR_SMS_TYPE_NUMBER_READ == m_strSMSType) {
        GetMessageConditionByNumber(where, whereArgs, true);
    }
    else if (VR_SMS_TYPE_NUMBER_UNREAD == m_strSMSType) {
        GetMessageConditionByNumber(where, whereArgs, false);
    }
    else {
        return false;
    }
    return true;
}

void VR_SMSHandler::GetMessageConditionAll(NCString& where, NCObjList<NCString>& whereArgs)
{
    VR_LOGD_FUNC();
    NCString* ncStrType1 = VR_new NCString();
    ncStrType1->format("%d", 2);
    NCString* ncStrType2 = VR_new NCString();
    ncStrType2->format("%d", 3);
    if (m_bToday) {
        where.format("%s like ? and (%s=? or %s=?)", "time", "type", "type");
        NCString* ncStrTime = VR_new NCString();
        VR_LOGD("%s", m_strDate.c_str());
        ncStrTime->format("%s%%", m_strDate.c_str());
        whereArgs << ncStrTime << ncStrType1 << ncStrType2;
    }
    else {
        where.format("(%s=? or %s=?) ", "type", "type");
        whereArgs << ncStrType1 << ncStrType2;
    }
}

void VR_SMSHandler::GetMessageConditionAll(NCString& where, NCObjList<NCString>& whereArgs, bool bRead)
{
    VR_LOGD_FUNC();
    NCString* ncStrType1 = VR_new NCString();
    ncStrType1->format("%d", 2);
    NCString* ncStrType2 = VR_new NCString();
    ncStrType2->format("%d", 3);
    NCString* ncStrStatus = VR_new NCString();
    if (bRead) {
        ncStrStatus->format("%d", 0);
    }
    else {
        ncStrStatus->format("%d", 1);
    }
    
    if (m_bToday) {
        where.format("%s like ? and %s=? and (%s=? or %s=?)", "time", "status", "type", "type");
        NCString* ncStrTime = VR_new NCString();
        ncStrTime->format("%s%%", m_strDate.c_str());
        whereArgs << ncStrTime << ncStrStatus << ncStrType1 << ncStrType2;
    }
    else {
        where.format("%s=? and (%s=? or %s=?)", "status", "type", "type");
        whereArgs << ncStrStatus << ncStrType1 << ncStrType2;
    }
}

void VR_SMSHandler::GetMessageConditionByContact(NCString& where, NCObjList<NCString>& whereArgs, bool bRead)
{
    VR_LOGD_FUNC();
    NCString* ncStrType1 = VR_new NCString();
    ncStrType1->format("%d", 2);
    NCString* ncStrType2 = VR_new NCString();
    ncStrType2->format("%d", 3);
    NCString* ncStrStatus = VR_new NCString();
    if (bRead) {
        ncStrStatus->format("%d", 0);
    }
    else {
        ncStrStatus->format("%d", 1);
    }
    NCString* ncStrContact = VR_new NCString();
    ncStrContact->format("%s", m_strContactName.c_str());
    
    if (m_bToday) {
        where.format("%s like ? and %s=? and %s=? and (%s=? or %s=?)", "time", "name", "status", "type", "type");
        NCString* ncStrTime = VR_new NCString();
        ncStrTime->format("%s%%", m_strDate.c_str());
        whereArgs << ncStrTime << ncStrContact << ncStrStatus << ncStrType1 << ncStrType2;
    }
    else {
        where.format("%s=? and %s=? and (%s=? or %s=?) ", "name", "status", "type", "type");
        whereArgs << ncStrContact << ncStrStatus << ncStrType1 << ncStrType2;
    }
}

void VR_SMSHandler::GetMessageConditionByNumber(NCString& where, NCObjList<NCString>& whereArgs, bool bRead)
{
    VR_LOGD_FUNC();
    NCString* ncStrType1 = VR_new NCString();
    ncStrType1->format("%d", 2);
    NCString* ncStrType2 = VR_new NCString();
    ncStrType2->format("%d", 3);
    NCString* ncStrStatus = VR_new NCString();
    if (bRead) {
        ncStrStatus->format("%d", 0);
    }
    else {
        ncStrStatus->format("%d", 1);
    }

    NCString* ncStrNumber = VR_new NCString();
    ncStrNumber->format("%s", m_strPhoneNumber.c_str());
    VR_String strNumber;
    if (ChangedQueryNumber(strNumber)) {

        // query by number (xxxx and +86xxxxx)
        VR_LOGD("Number (%s and %s)", m_strPhoneNumber.c_str(), strNumber.c_str());
        NCString* ncStrNumberChanged = VR_new NCString();
        ncStrNumberChanged->format("%s", strNumber.c_str());
        if (m_bToday) {
            where.format("%s like ? and (%s=? or %s=?) and %s=? and (%s=? or %s=?)", "time", "address", "address", "status", "type", "type");
            NCString* ncStrTime = VR_new NCString();
            ncStrTime->format("%s%%", m_strDate.c_str());
            whereArgs << ncStrTime << ncStrNumber << ncStrNumberChanged << ncStrStatus << ncStrType1 << ncStrType2;
        }
        else {
            where.format("(%s=? or %s=?) and %s=? and (%s=? or %s=?)", "address", "address", "status", "type", "type");
            whereArgs << ncStrNumber << ncStrNumberChanged << ncStrStatus << ncStrType1 << ncStrType2;
        }
    }
    else {
        VR_LOGD("Number (%s and %s)", m_strPhoneNumber.c_str(), strNumber.c_str());
        if (m_bToday) {
            where.format("%s like ? and %s=? and %s=? and (%s=? or %s=?)", "time", "address", "status", "type", "type");
            NCString* ncStrTime = VR_new NCString();
            ncStrTime->format("%s%%", m_strDate.c_str());
            whereArgs << ncStrTime << ncStrNumber << ncStrStatus << ncStrType1 << ncStrType2;
        }
        else {
            where.format("%s=? and %s=? and (%s=? or %s=?)", "address", "status", "type", "type");
            whereArgs << ncStrNumber << ncStrStatus << ncStrType1 << ncStrType2;
        }
    }
}

bool VR_SMSHandler::ChangedQueryNumber(VR_String& strNumChanged)
{
    VR_String::size_type iPos;
    iPos = m_strPhoneNumber.find_first_of(cstAreaCode);
    if (iPos == 0) {
        strNumChanged = m_strPhoneNumber;
    }
    else {
        strNumChanged = VR_String(cstAreaCode.c_str()) + m_strPhoneNumber.c_str();
    }

    return !(strNumChanged == m_strPhoneNumber);
}

void VR_SMSHandler::RemoveAreaCode(nutshell::NCString& strNumber)
{
    // If the number has area code, remove area code
    VR_String strNumberChanged = strNumber.getString();
    VR_String::size_type iPos;
    iPos = strNumberChanged.find_first_of(cstAreaCode);
    if (iPos == 0) {
        strNumberChanged = strNumberChanged.substr(cstAreaCode.length());
    }
    else {
        return;
    }
    VR_LOGD("number changed (%s -> %s)", strNumber.getString(), strNumberChanged.c_str());
    strNumber = strNumberChanged.c_str();
}

void VR_SMSHandler::ImproveMessageInfo(VR_MessageInfo& msgInfo, const VR_String& strDateTime)
{
    VR_LOGD_FUNC();

    // set contact id
    FindContactId(msgInfo.strNumber, msgInfo.iContactId);

    // set date and time
    if (15 != strDateTime.size()) {
        VR_LOGD("current dataTime is [%s]", strDateTime.c_str());
        return;
    }
    msgInfo.strYear = strDateTime.substr(0, 4).c_str();
    msgInfo.strMonth = strDateTime.substr(4, 2).c_str();
    msgInfo.strDay = strDateTime.substr(6, 2).c_str();
    msgInfo.strHour = strDateTime.substr(9, 2).c_str();
    msgInfo.strMinute = strDateTime.substr(11, 2).c_str();
}

void VR_SMSHandler::FindContactId(const nutshell::NCString& number, nutshell::INT64& iContactId)
{
    VR_LOGD_FUNC();
    if (VR_SMS_TYPE_CONTACT_READED == m_strSMSType || VR_SMS_TYPE_CONTACT_UNREAD == m_strSMSType) {
        iContactId = atoi(m_strContactId.c_str());
        return;
    }
    else {
    }

    VR_String strNumberAreaCode = VR_String(cstAreaCode.c_str()) + VR_String(number.getString());
    iContactId = -1;
    NCObjList<NCString> columns;
    NCString sortOrder;
    NCContentResolver resolver;
    NCCursor* cursor = resolver.query(NCUri(VR_PHONE_CONTACTID_BY_NUMBER_URI), columns, number, columns, sortOrder);
    if (NULL == cursor) {
        VR_LOGD("Find contact id error");
        FindContactIdWithAreaCode(strNumberAreaCode, iContactId);
        return;
    }

    INT rowNum = cursor->getCount();
    if (0 == rowNum) {
        VR_LOGD("No contact id");
        delete cursor;
        cursor = NULL;
        FindContactIdWithAreaCode(strNumberAreaCode, iContactId);
        return;
    }

    NC_BOOL Result = cursor->moveToFirst();
    if (NC_FALSE == Result) {
        VR_LOGD("cursor->moveToFirst() is error");
        delete cursor;
        cursor = NULL;
        FindContactIdWithAreaCode(strNumberAreaCode, iContactId);
        return;
    }

    iContactId = cursor->getInt64(0);

    delete cursor;
    cursor = NULL;
}

void VR_SMSHandler::FindContactIdWithAreaCode(const VR_String& strNumber, nutshell::INT64& iContactId)
{
    VR_LOGD_FUNC();

    NCObjList<NCString> columns;
    NCString sortOrder;
    NCContentResolver resolver;
    NCCursor* cursor = resolver.query(NCUri(VR_PHONE_CONTACTID_BY_NUMBER_URI), columns, NCString(strNumber.c_str()), columns, sortOrder);
    if (NULL == cursor) {
        VR_LOGD("Find contact id error");
        return;
    }

    INT rowNum = cursor->getCount();
    if (0 == rowNum) {
        VR_LOGD("No contact id");
        delete cursor;
        cursor = NULL;
        return;
    }

    NC_BOOL Result = cursor->moveToFirst();
    if (NC_FALSE == Result) {
        VR_LOGD("cursor->moveToFirst() is error");
        delete cursor;
        cursor = NULL;
        return;
    }

    iContactId = cursor->getInt64(0);

    delete cursor;
    cursor = NULL;
}

bool VR_SMSHandler::ResponseMessageInfo(bool bSuccessful, const VR_String& strMsgBody)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_DATA_SMS_RESPGETMSG);
    if (!res) {
        VR_LOGD("xml invalid");
        return false;
    }

    do {
        res = parser.SetNodeText("//action_result/id", m_strId.c_str());
        if (!res) {
            VR_LOGD("set id error");
            break;
        }

        res = parser.SetNodeText("//action_result/action_id", VR_IdGenerator().GetID());
        if (!res) {
            VR_LOGD("set action_id error");
            break;
        }

        if (!bSuccessful) {
            res = false;
            break;
        }

        res = parser.SetNodeText("//action_result/error_no", VR_PHONE_RESULT_OK);
        if (!res) {
            VR_LOGD("set error_no error");
            break;
        }

        res = parser.SetNodeText("//action_result/total_num", (int)m_lstMsgInfo.size());
        if (!res) {
            VR_LOGD("set total_num error");
            break;
        }

        if (m_lstMsgInfo.empty()) {
            VR_LOGD("there is no message");
            break;
        }

        VR_XmlStrParse parserMessageInfo;
        VR_String strMessageInfo;
        res = parserMessageInfo.LoadString(VR_DATA_MESSAGE_INFO);
        if (!res) {
            VR_LOGD("set message_info error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/message_content", strMsgBody);
        if (!res) {
            VR_LOGD("set message_content error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/message_id", m_msgInfo.strMessageId.getString());
        if (!res) {
            VR_LOGD("set message_id error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/instance_id", (int)m_msgInfo.iInstanceId);
        if (!res) {
            VR_LOGD("set instance_id error");
            break;
        }

        if (-1 != m_msgInfo.iContactId) {
            res = parserMessageInfo.SetNodeText("//message_info/sender_id", (int)m_msgInfo.iContactId);
            if (!res) {
                VR_LOGD("set sender_id error");
                break;
            }
        }

        res = parserMessageInfo.SetNodeText("//message_info/sender_name", m_msgInfo.strName.getString());
        if (!res) {
            VR_LOGD("set sender_name error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/sender_number", m_msgInfo.strNumber.getString());
        if (!res) {
            VR_LOGD("set sender_number error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/minute", m_msgInfo.strMinute.getString());
        if (!res) {
            VR_LOGD("set minute error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/hour", m_msgInfo.strHour.getString());
        if (!res) {
            VR_LOGD("set hour error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/day", m_msgInfo.strDay.getString());
        if (!res) {
            VR_LOGD("set day error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/month", m_msgInfo.strMonth.getString());
        if (!res) {
            VR_LOGD("set month error");
            break;
        }

        res = parserMessageInfo.SetNodeText("//message_info/year", m_msgInfo.strYear.getString());
        if (!res) {
            VR_LOGD("set year error");
            break;
        }

        parserMessageInfo.GetXmlString(strMessageInfo);
        res = parser.AddNode("//action_result/message_info_list", strMessageInfo);
        if (!res) {
            VR_LOGD("add message_info_list error");
            break;
        }
        parserMessageInfo.Reset();

    } while (0);

    if (!res) {
        parser.SetNodeText("//action_result/error_no", VR_PHONE_RESULT_ERROR);
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("MESG: %s", mesg.c_str());
    SendMesgNATP(mesg);
    return true;
}

void VR_SMSHandler::SendMesgNATP(const VR_String& strMessage)
{
    VR_LOGD_FUNC();
    using namespace navi::VoiceRecog;
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(strMessage.c_str());
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

void VR_SMSHandler::RequestMsgBody()
{
    int iSize = m_lstMsgInfo.size();
    if (m_iIndex >= iSize) {
        ResponseMessageInfo(false);
        return;
    }

    VR_MessageInfo msgInfo = m_lstMsgInfo.at(m_iIndex);
    m_lstQueryMsgInfo.push_back(msgInfo);

    VR_String resultXml;
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(atoi(m_strId.c_str()));
    notifyAction->set_actionid(-1);
    resultXml = (boost::format(SMS_BODY_XML) % msgInfo.iInstanceId % msgInfo.strMessageId.getString()).str();

    VR_LOGD("[-- Send msg-req to VRFC: %s --]", resultXml.c_str());

    *(notifyAction->mutable_actioncontent()) = resultXml;
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());
}

void VR_SMSHandler::ReponseMsgBody(const VR_String& strMessage)
{
    CL_AutoSync cAutoSync(m_cSync);
    VR_LOGD("message body is [%s]", strMessage.c_str());
    if (m_lstQueryMsgInfo.empty()) {
        VR_LOGE("There is no query infomation");
        return;
    }

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strMessage)) {
        VR_LOGE("load_string error");
        ResponseMessageInfo(false);
        return;
    }

    auto iter = m_lstQueryMsgInfo.begin();
    m_msgInfo = *(iter);
    m_lstQueryMsgInfo.erase(iter);

    int iInstanceId = atoi(xmlStrParse.GetXMLValue("instanceId").c_str());
    VR_String strMsgId = xmlStrParse.GetXMLValue("messageId");
    VR_String strMsgBody = xmlStrParse.GetXMLValue("messagebody");

    if (iInstanceId != m_msgInfo.iInstanceId
        || strMsgId != m_msgInfo.strMessageId.getString()) {
        VR_LOGD("intanceID(%d,%d) messageID(%s, %s)", iInstanceId
                , m_msgInfo.iInstanceId
                , strMsgId.c_str()
                , m_msgInfo.strMessageId.getString());
        return;
    }
    ResponseMessageInfo(true, strMsgBody);
}

/* EOF */
