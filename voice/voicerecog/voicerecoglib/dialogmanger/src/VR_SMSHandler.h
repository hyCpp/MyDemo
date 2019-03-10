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
 * @file VR_SMSHandler.h
 * @brief Declaration file of class VR_SMSHandler.
 *
 * This file includes the declaration of class VR_SMSHandler.
 *
 * @attention used for C++ only.
 */

#ifndef VR_SMSHANDLER_H
#define VR_SMSHANDLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "ncore/NCDir.h"
#include "ncore/NCFile.h"
#include "VR_PhoneBookXMLFormat.h"
#include "CL_SyncObj.h"

/**
 * @brief The VR_SMSHandler class
 *
 * class declaration
 */
class VR_SMSHandler
{
public:
    VR_SMSHandler();
    virtual ~VR_SMSHandler();

public:
    void GetSMSMessage(const VR_String& strXml);
    void ReponseMsgBody(const VR_String& strMessage);

private:
    int GetSMSMessageInfo();
    void GetTodayInfo();

    bool GetMessageCondition(nutshell::NCString& where, nutshell::NCObjList<nutshell::NCString>& whereArgs);

    void GetMessageConditionAll(nutshell::NCString& where, nutshell::NCObjList<nutshell::NCString>& whereArgs);
    void GetMessageConditionAll(nutshell::NCString& where, nutshell::NCObjList<nutshell::NCString>& whereArgs, bool bRead);
    void GetMessageConditionByContact(nutshell::NCString& where, nutshell::NCObjList<nutshell::NCString>& whereArgs, bool bRead);
    void GetMessageConditionByNumber(nutshell::NCString& where, nutshell::NCObjList<nutshell::NCString>& whereArgs, bool bRead);

    void ImproveMessageInfo(navi::dataprovider::VR_MessageInfo& msgInfo, const VR_String& strDateTime);
    void FindContactId(const nutshell::NCString& number, nutshell::INT64& iContactId);
    void FindContactIdWithAreaCode(const VR_String& strNumber, nutshell::INT64& iContactId);

    bool ResponseMessageInfo(bool bSuccessful, const VR_String& strMsgBody = "");
    void UpdateData();
    bool CheckNumber(const VR_String& strNum, const VR_String& strContactNum);
    bool ChangedQueryNumber(VR_String& strNumChanged);
    void RemoveAreaCode(nutshell::NCString& strNumber);
    void SendMesgNATP(const VR_String& strMessage);

    void RequestMsgBody();

private:
    VR_String   m_strId;
    VR_String   m_strSMSType;
    VR_String   m_strContactName;
    VR_String   m_strContactId;
    VR_String   m_strPhoneNumber;
    VR_String   m_strDate;
    int         m_iIndex;
    bool        m_bToday;

    VoiceVector<navi::dataprovider::VR_MessageInfo>::type m_lstMsgInfo;
    VoiceList<navi::dataprovider::VR_MessageInfo>::type m_lstQueryMsgInfo;
    navi::dataprovider::VR_MessageInfo m_msgInfo;
    CL_SyncObj m_cSync;
};

#endif // VR_VOICETAGHANDLER_H
/* EOF */
