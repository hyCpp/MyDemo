/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

#include <string>
#include "gtest/gtest.h"
#include "VR_VBT_VECIEngineClient_mock.h"
#include "VR_VBT_VECIEngineCommand_mock.h"
#include "VR_VBT_VECIString_mock.h"
#include "VR_VBT_EngineCallback_mock.h"
#include "VR_VBT_VECIParsedMessage_mock.h"
#include "VR_VBT_VECITransaction_mock.h"
#include "VR_VBT_VECIStringSet_mock.h"
#include "VR_VBT_VECIParameterSet_mock.h"
#include "VR_VBT_VECIListItems_mock.h"

#include "VR_VoiceBoxXmlParser.h"
#include "VR_VoiceBoxAgentPhone.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;
/**
* VR_VBT_AgentPhone_Test
*
* The class is just for VR_VoiceBoxAgentPhone test.
*/
class VR_VBT_AgentPhone_Test : public testing::Test
{
public:
    VR_VBT_AgentPhone_Test() : m_agentPhone(NULL)
    {
    }

    virtual void SetUp();
    virtual void TearDown();

public:
    MockVR_VBT_VECIStringSet m_stringSet;
    MockVR_VBT_VECIEngineClient m_engine;
    MockVR_VBT_VECIEngineCommand m_command;
    MockVR_VBT_EngineCallback m_callback;
    MockVR_VBT_VECIParsedMessage m_pResultMsg;
    MockVR_VBT_VECITransaction m_pTrans;
    MockVR_VBT_VECIParameterSet m_paraSet;
    MockVR_VBT_VECIListItems m_listItems;

    VR_VoiceBoxAgentPhone* m_agentPhone;

private:
    VR_VBT_AgentPhone_Test(const VR_VBT_AgentPhone_Test& other);
    void operator= (const VR_VBT_AgentPhone_Test& other);
};

void
VR_VBT_AgentPhone_Test::SetUp()
{
    m_agentPhone = new VR_VoiceBoxAgentPhone(
                m_engine,
                m_command,
                m_callback);
}

void
VR_VBT_AgentPhone_Test::TearDown()
{
    delete m_agentPhone;
    m_agentPhone = NULL;
}

TEST_F(VR_VBT_AgentPhone_Test, Initialize)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    EXPECT_TRUE(ret);
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, ReplyQueryInfo)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<action><op>setMessasgeAvailable</op></action>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentPhone->ReplyQueryInfo(parser));
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Navi");
    HRESULT result;
    result = m_agentPhone->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, QueryAgentSupport_True)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }
    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(VBT_TRUE), Return(S_OK)));

    VBT_CSTR szAgent = _T("HFD");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentPhone->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }

    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK)));

    VBT_CSTR szAgent = _T("System");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentPhone->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_FALSE(bSupported);
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, OnAgentResultMessage_NULL)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentPhone->OnAgentResultMessage(NULL, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhone->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentPhone_Test, OnAgentResultMessage_Fail)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Phone"), 6);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));

    HRESULT result = m_agentPhone->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, OnAgentResultMessage_S_OK)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strMsgClassName = VeciAllocString(_T("Phone"), 6);
    VBT_STR strActionType = VeciAllocString(_T("HFD Keyboard Dictation"), 23);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, setActionResultValue(_)).WillOnce(Return());

    HRESULT result = m_agentPhone->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentPhone->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentPhone_Test, OnAgentDriverMessage_NULL)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentPhone->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhone->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentPhone_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Phone"), 6);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));

    HRESULT result = m_agentPhone->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, OnAgentDriverMessage_S_OK)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strMsgClassName = VeciAllocString(_T("Phone"), 6);
    VBT_STR strActionType = VeciAllocString(_T("HFD Keyboard Dictation"), 23);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, setActionResultValue(_)).WillOnce(Return());

    HRESULT result = m_agentPhone->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentPhone->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentPhone_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentPhone->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentPhone->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentPhone_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Phone"), 6);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));

    bool result = m_agentPhone->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, ProcessAgentMessage_ActionType_True)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strMsgClassName = NULL;
    VBT_STR strActionType = VeciAllocString(_T("HFD Keyboard Dictation"), 23);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, setActionResultValue(_)).WillOnce(Return());
    bool result = m_agentPhone->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentPhone->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_Test, ProcessAgentMessage_ListType_True)
{
    if (NULL == m_agentPhone) {
        FAIL();
    }

    bool ret = m_agentPhone->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = NULL;
    VBT_STR strListType = VeciAllocString(_T("HFD Keyboard Dictation"), 23);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, setActionResultValue(_)).WillOnce(Return());
    bool result = m_agentPhone->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentPhone->UnInitialize();
}
*/

TEST_F(VR_VBT_AgentPhone_Test, SendMessageInfo)
{
    std::string xml = "<id>23</id>"
                      "<messageType>SMS</messageType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentPhone->SendMessageInfo(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, ChangeCallType)
{
    std::string xml = "<change>callType</change>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentPhone->ChangeCallType(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, QuitVRInit_InstanceId)
{
    std::string xml = "<quit>VR</quit>";
    m_agentPhone->m_strInstanceId = "3";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentPhone->QuitVRInit(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, QuitVRInit_MessageType)
{
    std::string xml = "<quit>VR</quit>";
    m_agentPhone->m_strMessageType = "message";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentPhone->QuitVRInit(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, SetIncomingMessageInfo)
{
    std::string xml = "<instanceId>2</instanceId>"
                      "<messageId>3</messageId>"
                      "<phoneType>phone</phoneType>"
                      "<contactID>65</contactID>"
                      "<messageType>message</messageType>"
                      "<subject>meeting</subject>"
                      "<phoneNumber>18947628593</phoneNumber>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentPhone->SetIncomingMessageInfo(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, HFDNBest_Name)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_ULONG uSize = 1;
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    VBT_STR strDigits = VeciAllocString(_T("66"), 3);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<2>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strDigits), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    bool result = m_agentPhone->HFDNBest(&m_pResultMsg);
    EXPECT_TRUE(result);
}

TEST_F(VR_VBT_AgentPhone_Test, HFDNBest_Number)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_ULONG uSize = 1;
    VBT_STR strContact = NULL;
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    VBT_STR strDigits = VeciAllocString(_T("66"), 3);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<2>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strDigits), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    bool result = m_agentPhone->HFDNBest(&m_pResultMsg);
    EXPECT_TRUE(result);
}

TEST_F(VR_VBT_AgentPhone_Test, HFDNBest_Digits)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_ULONG uSize = 1;
    VBT_STR strContact = NULL;
    VBT_STR strPhoneNumber = NULL;
    VBT_STR strDigits = VeciAllocString(_T("66"), 3);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<2>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strDigits), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    bool result = m_agentPhone->HFDNBest(&m_pResultMsg);
    EXPECT_TRUE(result);
}

TEST_F(VR_VBT_AgentPhone_Test, AppendDigitsAlreadyAtMax)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->AppendDigitsAlreadyAtMax(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, AppendDigitsCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->AppendDigitsCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, PhoneTaskComplete)
{
    VBT_STR strActionType = VeciAllocString(_T("Task"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentPhone->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallBackConfirmed)
{
    m_agentPhone->m_strContactID = "";
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallBackConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallBackConfirmed_ContactIDNotNull)
{
    m_agentPhone->m_strContactID = "2";
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallBackConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallContact)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneType = VeciAllocString(_T("2"), 2);
    VBT_STR strContactID = VeciAllocString(_T("1"), 2);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallContact(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallContact_tempPhoneTypeNull)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T(""), 1);
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneType = VeciAllocString(_T(""), 1);
    VBT_STR strContactID = VeciAllocString(_T(""), 1);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallContact(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallContactCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());

    EXPECT_TRUE(m_agentPhone->CallContactCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallContactConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    VBT_STR strPhoneTypeID = VeciAllocString(_T("2"), 2);
    VBT_STR strContactID = VeciAllocString(_T("1"), 1);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneTypeID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallContactConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallContactConfirmed_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContact = NULL;
    VBT_STR strPhoneNumber = NULL;
    // VBT_STR strPhoneTypeID = NULL;
    VBT_STR strPhoneTypeID = VeciAllocString(_T("2"), 2);
    VBT_STR strContactID = NULL;

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneTypeID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallContactConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallCurrentMsgConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallCurrentMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallCurrentMsgConfirmed_PhoneType_1)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    m_agentPhone->m_strPhoneType = "-1";
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallCurrentMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallCurrentMsgConfirmed_PhoneType_2)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    m_agentPhone->m_strPhoneType = "2";
    m_agentPhone->m_strContactID = "1";
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallCurrentMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallCurrentMsgConfirmed_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContact = NULL;
    VBT_STR strPhoneNumber = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallCurrentMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallIncomingMsgConfirmed_PhoneNumberNotNULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = NULL;
    VBT_STR strPhoneNumber = VeciAllocString(_T("13250354284"), 12);
    std::string strPath = "/Phonepath/";
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(strPath));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallIncomingMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallIncomingMsgConfirmed_ContactNameNotNULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13250354284"), 12);
    std::string strPath = "/Phonepath/";
    m_agentPhone->m_incomingMessageInfo.contactID = "2";
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(strPath));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallIncomingMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallIncomingMsgConfirmed_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = NULL;
    VBT_STR strPhoneNumber = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallIncomingMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallIncomingMsgConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = NULL;
    VBT_STR strPhoneNumber = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallIncomingMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallNumber)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallNumber_PhoneNumberNotNULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13250354284"), 12);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallNumberCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallNumberCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallNumberConfirmed_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = NULL;
    VBT_STR strPhoneNumber = NULL;
    VBT_STR strContactId = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactId), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallNumberConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallNumberConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    VBT_STR strContactId = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactId), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallNumberConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallSelectedMsgConfirmed_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListNumber = VeciAllocString(_T("ListNumber"), 11);
    VBT_STR strContactName = NULL;
    VBT_STR strPhoneNumber = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallSelectedMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallSelectedMsgConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListNumber = VeciAllocString(_T("ListNumber"), 11);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallSelectedMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallSelectedMsgConfirmed_ListNumber)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListNumber = VeciAllocString(_T("3"), 2);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallSelectedMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CallSelectedRecentConfirm)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListNumber = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CallSelectedRecentConfirm(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CannotAddVoiceTag)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CannotAddVoiceTag(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, CannotReplytoCurrentMessage)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->CannotReplytoCurrentMessage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ChooseVoiceTagContact)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ChooseVoiceTagContact(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ConfirmSMSRecipient)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    VBT_STR strPhoneType = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactID = VeciAllocString(_T("3"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));

    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ConfirmSMSRecipient(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, DigitsCorrection)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->DigitsCorrection(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, HFDHome)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->HFDHome(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, HFDKeyboardDictation)
{
    EXPECT_TRUE(m_agentPhone->HFDKeyboardDictation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, BuildListHeader)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node xmlNode = xmlBulder.buildDisplayElement("Common", "ScreenDisplay");
    xmlBulder.buildGivenElement(xmlNode, "agent", "phone", "", "");
    xmlBulder.buildGivenElement(xmlNode, "content", "na_phone_list_messages", "", "");
    pugi::xml_node selectListNode = xmlBulder.buildGivenElement(xmlNode, "selectList", "", "", "");
    pugi::xml_node nodeList = xmlBulder.buildGivenElement(selectListNode, "list", "", "", "");
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strPageIndex = VeciAllocString(_T("5"), 2);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPageIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK)));
    m_agentPhone->BuildListHeader(&m_pResultMsg, xmlBulder, nodeList);
    SUCCEED();
}

TEST_F(VR_VBT_AgentPhone_Test, ListMessages)
{
    VBT_ULONG uSize = 1;
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);

    VBT_STR strPhoneNumber = VeciAllocString(_T("PhoneNumber"), 12);
    VBT_STR strListIndex = VeciAllocString(_T("List"), 5);
    VBT_STR strEntry = VeciAllocString(_T("Entry"), 6);
    VBT_STR strSender = VeciAllocString(_T("Sender"), 7);
    VBT_STR strTimeStamp = VeciAllocString(_T("Time"), 5);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(5).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strListIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strEntry), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strSender), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strTimeStamp), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());

    EXPECT_TRUE(m_agentPhone->ListMessages(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ListPartialNameMatches)
{
    VBT_ULONG uSize = 1;
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);

    VBT_STR strPhoneNum = VeciAllocString(_T("PhoneNumber"), 12);
    VBT_STR strPhoneType = VeciAllocString(_T("List"), 5);
    VBT_STR strPhoneTypeID = VeciAllocString(_T("Entry"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Sender"), 7);
    VBT_STR strContactID = VeciAllocString(_T("Time"), 5);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(5).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNum), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneTypeID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ListPartialNameMatches(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ListPartialNameMatches_NULL)
{
    VBT_ULONG uSize = 1;
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);

    VBT_STR strPhoneNum = NULL;
    VBT_STR strPhoneType = NULL;
    VBT_STR strPhoneTypeID = NULL;
    VBT_STR strContactName = VeciAllocString(_T("Sender"), 7);
    VBT_STR strContactID = VeciAllocString(_T("Time"), 5);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(5).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNum), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneTypeID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ListPartialNameMatches(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ListPhoneTypes)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_ULONG uSize = 1;
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    VBT_STR strContactID = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strContactID), Return(S_OK)));

    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ListPhoneTypes(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ListRecentCalls)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_ULONG uSize = 1;
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);
    VBT_STR strListIndex = VeciAllocString(_T("Index"), 6);
    VBT_STR strCallType = VeciAllocString(_T("CallType"), 9);
    VBT_STR strEntry = VeciAllocString(_T("Entry"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<2>(strListIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strEntry), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strTimeStamp), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ListRecentCalls(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ListRecentCalls_CallTypeNULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_ULONG uSize = 1;
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);
    VBT_STR strListIndex = VeciAllocString(_T("Index"), 6);
    VBT_STR strCallType = NULL;
    VBT_STR strEntry = VeciAllocString(_T("Entry"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<2>(strListIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strEntry), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strTimeStamp), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ListRecentCalls(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ListRecentCalls_CallTypeOutGoing)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_ULONG uSize = 1;
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);
    VBT_STR strListIndex = VeciAllocString(_T("Index"), 6);

    VBT_STR strCallType = VeciAllocString(_T("OutGoing"), 9);
    m_agentPhone->m_strOutGoing = "OutGoing";

    VBT_STR strEntry = VeciAllocString(_T("Entry"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<2>(strListIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strEntry), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strTimeStamp), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ListRecentCalls(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ListRecentCalls_CallTypeIncoming)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_ULONG uSize = 1;
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);
    VBT_STR strListIndex = VeciAllocString(_T("Index"), 6);

    VBT_STR strCallType = VeciAllocString(_T("Incoming"), 9);
    m_agentPhone->m_strIncoming = "Incoming";

    VBT_STR strEntry = VeciAllocString(_T("Entry"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<2>(strListIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strEntry), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strTimeStamp), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ListRecentCalls(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ListRecentCalls_CallTypeMiss)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_ULONG uSize = 1;
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);
    VBT_STR strListIndex = VeciAllocString(_T("Index"), 6);

    VBT_STR strCallType = VeciAllocString(_T("Miss"), 5);
    m_agentPhone->m_strMiss = "Miss";

    VBT_STR strEntry = VeciAllocString(_T("Entry"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<2>(strListIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strEntry), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strTimeStamp), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ListRecentCalls(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, MessageDictation)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Name"), 5);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->MessageDictation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, MessageDictation_ContactNameNULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = NULL;
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->MessageDictation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, MuteCall)
{
    VBT_STR strActionType = VeciAllocString(_T("HFD Keyboard Dictation"), 23);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->MuteCall(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, MuteOff)
{
    VBT_STR strActionType = VeciAllocString(_T("HFD Keyboard Dictation"), 23);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->MuteOff(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, NoContactNumbers)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhone->NoContactNumbers(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, NoContactsFound)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhone->NoContactsFound(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, NoQuickReplyMessages)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->NoQuickReplyMessages(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, OffboardDictation)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Name"), 5);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->OffboardDictation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, OneCallSelectedCallConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->OneCallSelectedCallConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, OneCallSelectedCallConfirmed_ContactID)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    m_agentPhone->m_strContactID = "3";
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->OneCallSelectedCallConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReadIncomingMessage)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhone->ReadIncomingMessage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReadSelectedMessage)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strSender = VeciAllocString(_T("Sender"), 7);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13826408264"), 12);
    VBT_STR strBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strListNumber = VeciAllocString(_T("3"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<1>(strSender), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strTimeStamp), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, GetSendMessageAvailableState()).WillOnce(Return(true));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReadSelectedMessage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReadSelectedMessage_SenderNULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strSender = NULL;
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13826408264"), 12);
    VBT_STR strBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strListNumber = VeciAllocString(_T("5"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<1>(strSender), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strTimeStamp), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, GetSendMessageAvailableState()).WillOnce(Return(false));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReadSelectedMessage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReadSelectMsgInvalidListNum)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReadSelectMsgInvalidListNum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, RedialConfirmed)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->RedialConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, RedialConfirmed_ContactID)
{
    m_agentPhone->m_strContactID = "2";
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->RedialConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithDictatedMessage)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Name"), 5);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithDictatedMessage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithDictatedMessage_ContactNameNULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = NULL;

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithDictatedMessage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithDictatedMsgConfirm)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    m_agentPhone->m_strInstanceId = "2";
    m_agentPhone->m_strMessageType = "1";

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithDictatedMsgConfirm(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithDictatedMsgConfirm_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    m_agentPhone->m_strInstanceId = "";
    m_agentPhone->m_strMessageType = "";

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithDictatedMsgConfirm(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithDictatedMsgReject)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Name"), 5);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithDictatedMsgReject(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithQuickReplyMessage)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Name"), 5);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithQuickReplyMessage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithQuickReplyMessage_ContactName)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T(""), 1);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithQuickReplyMessage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithQuickMsgConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    m_agentPhone->m_strInstanceId = "2";
    m_agentPhone->m_strMessageType = "1";

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithQuickMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, ReplywithQuickMsgConfirmed_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    m_agentPhone->m_strInstanceId = "";
    m_agentPhone->m_strMessageType = "";

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->ReplywithQuickMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SelectQuickReply)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("list"), 5);
    VBT_ULONG uSize = 1;
    VBT_STR strListIndex = VeciAllocString(_T("3"), 2);
    VBT_STR strMessageBody = VeciAllocString(_T("Message"), 8);

    VBT_STR strListNumber = VeciAllocString(_T("5"), 2);
    VBT_STR strListCount = VeciAllocString(_T("3"), 2);
    VBT_STR strVBTValue = VeciAllocString(_T("5"), 2);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));

    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));            

    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(strListIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strMessageBody), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SelectQuickReply(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SelectRecCallNoContactInfo)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Body"), 5);
    VBT_STR strCallType = VeciAllocString(_T("CallType"), 9);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("PhoneType"), 10);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    VBT_STR strContactID = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strTimeStamp), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SelectRecCallNoContactInfo(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SelectRecCallNoContactInfo_CallTypeNULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Body"), 5);
    VBT_STR strCallType = VeciAllocString(_T(""), 1);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("PhoneType"), 10);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    VBT_STR strContactID = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strTimeStamp), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SelectRecCallNoContactInfo(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SelectRecCallNoContactInfo_CallTypeOutGoing)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Body"), 5);
    VBT_STR strCallType = VeciAllocString(_T("OutGoing"), 9);
    m_agentPhone->m_strOutGoing = "OutGoing";
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("PhoneType"), 10);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    VBT_STR strContactID = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strTimeStamp), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SelectRecCallNoContactInfo(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SelectRecCallNoContactInfo_CallTypeIncoming)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Body"), 5);
    VBT_STR strCallType = VeciAllocString(_T("Incoming"), 9);
    m_agentPhone->m_strIncoming = "Incoming";
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("PhoneType"), 10);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    VBT_STR strContactID = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strTimeStamp), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SelectRecCallNoContactInfo(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SelectRecCallNoContactInfo_CallTypeMiss)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Body"), 5);
    VBT_STR strCallType = VeciAllocString(_T("Miss"), 5);
    m_agentPhone->m_strMiss = "Miss";
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strPhoneType = VeciAllocString(_T("PhoneType"), 10);
    VBT_STR strTimeStamp = VeciAllocString(_T("Stamp"), 6);
    VBT_STR strContactID = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(6).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strCallType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strTimeStamp), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SelectRecCallNoContactInfo(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SendDictatedMessageConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);

    m_agentPhone->m_strInstanceId = "4";
    m_agentPhone->m_strMessageType = "1";

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SendDictatedMessageConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SendDictatedMessageConfirmed_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);

    m_agentPhone->m_strInstanceId = "";
    m_agentPhone->m_strMessageType = "";

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SendDictatedMessageConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SendDTMF)
{
    VBT_STR strActionType = VeciAllocString(_T("Phone"), 6);
    VBT_STR strDigits = VeciAllocString(_T("1101"), 5);

    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strDigits), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SendDTMF(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SendDTMFCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SendDTMFCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SendQuickReplyMsgConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);

    m_agentPhone->m_strInstanceId = "4";
    m_agentPhone->m_strMessageType = "1";

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SendQuickReplyMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SendQuickReplyMsgConfirmed_NULL)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strMessageBody = VeciAllocString(_T("Body"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);

    m_agentPhone->m_strInstanceId = "";
    m_agentPhone->m_strMessageType = "";

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strMessageBody), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SendQuickReplyMsgConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SendSMSNoChangePhoneTypes)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhone->SendSMSNoChangePhoneTypes(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SMSNotAvailable)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SMSNotAvailable(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, SMSRecipientRejected)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->SMSRecipientRejected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, VoiceTagSIDError)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhone->VoiceTagSIDError(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, StartVoiceTag)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhone->StartVoiceTag(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, GetNextMessageRequest)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    m_agentPhone->m_listNumber = 0;

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->GetNextMessageRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, GetPreviousMessageRequest)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->GetPreviousMessageRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, GetPreviousMessageRequest_ListNumber)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    m_agentPhone->m_listNumber = 2;

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->GetPreviousMessageRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, GetSelectedMessageRequest)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListNumber = VeciAllocString(_T("1"), 2);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->GetSelectedMessageRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, IncomingCallInfoRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->IncomingCallInfoRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, IsDrivingRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->IsDrivingRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, OutgoingCallInfoRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->OutgoingCallInfoRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentCallsRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->RecentCallsRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesByTypeRequest)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListNumber = VeciAllocString(_T("1"), 2);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strListNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->RecentMessagesByTypeRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->RecentMessagesRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesReply_ErrCode0_messageType1)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<errcode>0</errcode>"
            "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>1</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender>sender</sender>"
            "<timeStamp>20151021</timeStamp>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesReply_ErrCode0_messageType1_senderNULL)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<errcode>0</errcode>"
            "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>1</messageType>"
            "<phoneNumber></phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender></sender>"
            "<timeStamp>20151021</timeStamp>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesReply_ErrCode0_messageType23)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<errcode>0</errcode>"
            "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>2</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender>sender</sender>"
            "<timeStamp>20151021</timeStamp>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesReply_ErrCode0_messageType4)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<errcode>0</errcode>"
            "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>4</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender>sender</sender>"
            "<timeStamp>20151021</timeStamp>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesReply_ErrCode0_messageType5)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<errcode>0</errcode>"
            "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>5</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender>sender</sender>"
            "<timeStamp>20151021</timeStamp>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesReply_ErrCodeERR_PHONE_NO_MESSAGE_LIST)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<errcode>ERR_PHONE_NO_MESSAGE_LIST</errcode>"
            "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>5</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender>sender</sender>"
            "<timeStamp>20151021</timeStamp>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, GetSelectedMessageReply_GetNextMessageRequest)
{
    std::string xml = "<errcode>0</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);
    m_agentPhone->m_requestType = _T("GetNextMessageRequest");
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(9).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->GetSelectedMessageReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, GetSelectedMessageReply_GetPreviousMessageRequest)
{
    std::string xml = "<errcode>0</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);
    m_agentPhone->m_requestType = _T("GetPreviousMessageRequest");
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(9).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->GetSelectedMessageReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, GetSelectedMessageReply_GetSelectedMessageRequest)
{
    std::string xml = "<errcode>0</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);
    m_agentPhone->m_requestType = _T("GetSelectedMessageRequest");
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->GetSelectedMessageReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, GetSelectedMessageReply_errcode_ERR_PHONE_QUERY_MESSAGE_DETAIL)
{
    std::string xml = "<errcode>ERR_PHONE_QUERY_MESSAGE_DETAIL</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);
    m_agentPhone->m_requestType = _T("GetNextMessageRequest");
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_FALSE(m_agentPhone->GetSelectedMessageReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, GetSelectedMessageReply_GetSelectedMessageRequest_E_FAIL)
{
    std::string xml = "<errcode>0</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);
    m_agentPhone->m_requestType = _T("GetSelectedMessageRequest");
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhone->GetSelectedMessageReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, GetMessageBodyError_errcode_ERR_PHONE_QUERY_MESSAGE_DETAIL)
{
    std::string xml = "<errcode>ERR_PHONE_QUERY_MESSAGE_DETAIL</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    m_agentPhone->GetMessageBodyError(parser);
    SUCCEED();
}

TEST_F(VR_VBT_AgentPhone_Test, GetMessageBodyError_errcode_ALREADY_ON_LAST_MESSAGE)
{
    std::string xml = "<errcode>ALREADY_ON_LAST_MESSAGE</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    m_agentPhone->GetMessageBodyError(parser);
    SUCCEED();
}

TEST_F(VR_VBT_AgentPhone_Test, GetMessageBodyError_errcode_ALREADY_ON_FIRST_MESSAGE)
{
    std::string xml = "<errcode>ALREADY_ON_FIRST_MESSAGE</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    m_agentPhone->GetMessageBodyError(parser);
    SUCCEED();
}

TEST_F(VR_VBT_AgentPhone_Test, GetMessageBodyError_errcode_ELSE)
{
    std::string xml = "<errcode>ELSE</errcode>"
            "<messagebody>reply</messagebody>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    m_agentPhone->GetMessageBodyError(parser);
    SUCCEED();
}

#define private public

TEST_F(VR_VBT_AgentPhone_Test, IncomingCallInfoReply_0)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>15037562852</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>phone</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhone->IncomingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, IncomingCallInfoReply_ERR_PHONE_NO_INCOMMING)
{
    std::string xml = "<errcode>ERR_PHONE_NO_INCOMMING</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->IncomingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, OutgoingCallInfoReply_0)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>15037562852</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>phone</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhone->OutgoingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, OutgoingCallInfoReply_ERR_PHONE_NO_OUTGOING)
{
    std::string xml = "<errcode>ERR_PHONE_NO_OUTGOING</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->OutgoingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentCallsReply_0_phoneKindMinus1)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<phoneInfo>"
            "<contactId>1</contactId>"
            "<phoneKind>-1</phoneKind>"
            "<contactName></contactName>"
            "<phoneType>phone</phoneType>"
            "<number></number>"
            "<timeStamp>stamp</timeStamp>"
            "<errcode>0</errcode>"
            "</phoneInfo>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));

    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));

    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhone->RecentCallsReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentCallsReply_0_phoneKind0)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<phoneInfo>"
            "<contactId>1</contactId>"
            "<phoneKind>0</phoneKind>"
            "<contactName>John</contactName>"
            "<phoneType>phone</phoneType>"
            "<number>13917485397</number>"
            "<timeStamp>stamp</timeStamp>"
            "<errcode>0</errcode>"
            "</phoneInfo>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));

    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));

    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentCallsReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentCallsReply_0_phoneKind1)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<phoneInfo>"
            "<contactId>1</contactId>"
            "<phoneKind>1</phoneKind>"
            "<contactName></contactName>"
            "<phoneType>phone</phoneType>"
            "<number>13917485397</number>"
            "<timeStamp>stamp</timeStamp>"
            "<errcode>0</errcode>"
            "</phoneInfo>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));

    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));

    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentCallsReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentCallsReply_0_phoneKind2)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<phoneInfo>"
            "<contactId>1</contactId>"
            "<phoneKind>2</phoneKind>"
            "<contactName></contactName>"
            "<phoneType>phone</phoneType>"
            "<number></number>"
            "<timeStamp>stamp</timeStamp>"
            "<errcode>0</errcode>"
            "</phoneInfo>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));

    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));

    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhone->RecentCallsReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentCallsReply_0_phoneKind3)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<phoneInfo>"
            "<contactId>1</contactId>"
            "<phoneKind>3</phoneKind>"
            "<contactName></contactName>"
            "<phoneType>phone</phoneType>"
            "<number></number>"
            "<timeStamp>stamp</timeStamp>"
            "<errcode>0</errcode>"
            "</phoneInfo>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));

    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));

    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhone->RecentCallsReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentCallsReply_ERR_PHONE_NO_RECENT_CALL_LIST)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<phoneInfo>"
            "<contactId>1</contactId>"
            "<phoneKind>0</phoneKind>"
            "<contactName>John</contactName>"
            "<phoneType>phone</phoneType>"
            "<number>13917485397</number>"
            "<timeStamp>stamp</timeStamp>"
            "<errcode>ERR_PHONE_NO_RECENT_CALL_LIST</errcode>"
            "</phoneInfo>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));

    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK)); 
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentCallsReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesByTypeReply_0_messageType1)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>1</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender>sender</sender>"
            "<timeStamp>20151021</timeStamp>"
            "<errcode>0</errcode>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesByTypeReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesByTypeReply_0_messageType2)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>2</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender></sender>"
            "<timeStamp>20151021</timeStamp>"
            "<errcode>0</errcode>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesByTypeReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesByTypeReply_0_messageType4)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>4</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender></sender>"
            "<timeStamp>20151021</timeStamp>"
            "<errcode>0</errcode>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesByTypeReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesByTypeReply_0_messageType5)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>5</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender></sender>"
            "<timeStamp>20151021</timeStamp>"
            "<errcode>0</errcode>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(spParamsList, AddParameter(_, _, _, _)).Times(6).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(pListItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->RecentMessagesByTypeReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, RecentMessagesByTypeReply_ERR_PHONE_NO_MESSAGE_LIST)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<messageInfo>"
            "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>1</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender>sender</sender>"
            "<timeStamp>20151021</timeStamp>"
            "<errcode>ERR_PHONE_NO_MESSAGE_LIST</errcode>"
            "</messageInfo>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&spParams), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&spParamsList), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&pListItems), Return(S_OK)));
    EXPECT_CALL(spParams, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhone->RecentMessagesByTypeReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, IsDrivingReply_False)
{
    std::string xml = _T("<motion>false</motion>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->IsDrivingReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, IsDrivingReply_True)
{
    std::string xml = _T("<motion>true</motion>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhone->IsDrivingReply(parser));
}

TEST_F(VR_VBT_AgentPhone_Test, PhoneBack)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->PhoneBack(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, PhoneCancel)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->PhoneCancel(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, PhoneHelp)
{
    EXPECT_TRUE(m_agentPhone->PhoneHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, PhoneStartOver)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->PhoneStartOver(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, PhoneStartPause)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->PhoneStartPause(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, PhoneStartRepeat)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->PhoneStartRepeat(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_Test, EscalatingError)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhone->EscalatingError(&m_pResultMsg));
}

/* EOF */
