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
#include "VR_VoiceBoxAgentPhone_AU.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;
/**
* VR_VBT_AgentPhone_AU_Test
*
* The class is just for VR_VoiceBoxAgentPhone_AU test.
*/
class VR_VBT_AgentPhone_AU_Test : public testing::Test
{
public:
    VR_VBT_AgentPhone_AU_Test() : m_agentPhoneAU(NULL)
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

    VR_VoiceBoxAgentPhone_AU* m_agentPhoneAU;

private:
    VR_VBT_AgentPhone_AU_Test(const VR_VBT_AgentPhone_AU_Test& other);
    void operator= (const VR_VBT_AgentPhone_AU_Test& other);
};

void
VR_VBT_AgentPhone_AU_Test::SetUp()
{
    m_agentPhoneAU = new VR_VoiceBoxAgentPhone_AU(
                m_engine,
                m_command,
                m_callback);
}

void
VR_VBT_AgentPhone_AU_Test::TearDown()
{
    delete m_agentPhoneAU;
    m_agentPhoneAU = NULL;
}

TEST_F(VR_VBT_AgentPhone_AU_Test, Initialize)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    EXPECT_TRUE(ret);
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, ReplyQueryInfo)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<action><op>setMessasgeAvailable</op></action>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentPhoneAU->ReplyQueryInfo(parser));
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Navi");
    HRESULT result;
    result = m_agentPhoneAU->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, QueryAgentSupport_True)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
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
    result = m_agentPhoneAU->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
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
    result = m_agentPhoneAU->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_FALSE(bSupported);
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OnAgentResultMessage_NULL)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentPhoneAU->OnAgentResultMessage(NULL, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhoneAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentPhone_AU_Test, OnAgentResultMessage_Fail)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
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

    HRESULT result = m_agentPhoneAU->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OnAgentResultMessage_S_OK)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strMsgClassName = VeciAllocString(_T("Phone"), 6);
    VBT_STR strActionType = VeciAllocString(_T("HFD Home"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, setActionResultValue(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    HRESULT result = m_agentPhoneAU->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentPhoneAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentPhone_AU_Test, OnAgentDriverMessage_NULL)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentPhoneAU->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhoneAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentPhone_AU_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
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

    HRESULT result = m_agentPhoneAU->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OnAgentDriverMessage_S_OK)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strMsgClassName = VeciAllocString(_T("Phone"), 6);
    VBT_STR strActionType = VeciAllocString(_T("HFD Home"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, setActionResultValue(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    HRESULT result = m_agentPhoneAU->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentPhoneAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentPhone_AU_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentPhoneAU->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentPhoneAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentPhone_AU_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
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

    bool result = m_agentPhoneAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, ProcessAgentMessage_ActionType_True)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strMsgClassName = NULL;
    VBT_STR strActionType = VeciAllocString(_T("HFD Home"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, setActionResultValue(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    bool result = m_agentPhoneAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentPhoneAU->UnInitialize();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, ProcessAgentMessage_ListType_True)
{
    if (NULL == m_agentPhoneAU) {
        FAIL();
    }

    bool ret = m_agentPhoneAU->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = NULL;
    VBT_STR strListType = VeciAllocString(_T("HFD Home"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, setActionResultValue(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    bool result = m_agentPhoneAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentPhoneAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentPhone_AU_Test, HFDNBest_Name)
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
    bool result = m_agentPhoneAU->HFDNBest(&m_pResultMsg);
    EXPECT_TRUE(result);
}

TEST_F(VR_VBT_AgentPhone_AU_Test, HFDNBest_Number)
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
    bool result = m_agentPhoneAU->HFDNBest(&m_pResultMsg);
    EXPECT_TRUE(result);
}

TEST_F(VR_VBT_AgentPhone_AU_Test, HFDNBest_Digits)
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
    bool result = m_agentPhoneAU->HFDNBest(&m_pResultMsg);
    EXPECT_TRUE(result);
}

TEST_F(VR_VBT_AgentPhone_AU_Test, AppendDigitsAlreadyAtMax)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhoneAU->AppendDigitsAlreadyAtMax(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneTaskComplete_NoPhoneRegistered)
{
    EXPECT_FALSE(m_agentPhoneAU->PhoneTaskComplete(NULL));

    VBT_STR strActionType = VeciAllocString(_T("No Phone Registered"), 20);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneTaskComplete_VBT_ACTION_TYPE_HFD_NO_PHONE_CONNECTED)
{
    EXPECT_FALSE(m_agentPhoneAU->PhoneTaskComplete(NULL));

    VBT_STR strActionType = VeciAllocString(_T("No Phone Connected"), 19);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneTaskComplete_VBT_ACTION_TYPE_HFD_NO_RECENT_CALLS)
{
    EXPECT_FALSE(m_agentPhoneAU->PhoneTaskComplete(NULL));

    VBT_STR strActionType = VeciAllocString(_T("No Recent Calls"), 16);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneTaskComplete_VBT_ACTION_TYPE_HFD_BUSY_UPDATING_GRAMMAR)
{
    EXPECT_FALSE(m_agentPhoneAU->PhoneTaskComplete(NULL));

    VBT_STR strActionType = VeciAllocString(_T("Busy Updating Grammar"), 22);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneTaskComplete_OutgoingCallHistoryEmpty)
{
    VBT_STR strActionType = VeciAllocString(_T("Outgoing Call History Empty"), 28);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneTaskComplete_IncomingCallHistoryEmpty)
{
    VBT_STR strActionType = VeciAllocString(_T("Incoming Call History Empty"), 28);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneTaskComplete_VBT_ACTION_TYPE_HFD_NO_REDIAL_NUMBER)
{
    VBT_STR strActionType = VeciAllocString(_T("No Redial Number"), 17);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneTaskComplete_VBT_ACTION_TYPE_HFD_NO_CALL_BACK_NUMBER)
{
    VBT_STR strActionType = VeciAllocString(_T("No Call Back Number"), 20);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallBackConfirmed)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->CallBackConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallContact)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContact = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    VBT_STR strPhoneType = VeciAllocString(_T("phone"), 6);
    VBT_STR strContactID = VeciAllocString(_T("2"), 2);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strContact), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactID), Return(S_OK)));

    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->CallContact(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallRecentNumber)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->CallRecentNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallContactCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->CallContactCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallContactConfirmed)
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
    EXPECT_TRUE(m_agentPhoneAU->CallContactConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallContactConfirmed_NULL)
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
    EXPECT_TRUE(m_agentPhoneAU->CallContactConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallNumber)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->CallNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallNumberCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->CallNumberCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallNumberConfirmed_ContactIDPhoneType)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    m_agentPhoneAU->m_strContactID = "2";
    m_agentPhoneAU->m_strPhoneType = "1";
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->CallNumberConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallNumberConfirmed_NULL)
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
    EXPECT_TRUE(m_agentPhoneAU->CallNumberConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, CallNumberConfirmed)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strContactName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->CallNumberConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, DigitsCorrection)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strPhoneNumber = VeciAllocString(_T("13037432236"), 12);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhoneNumber), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->DigitsCorrection(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, HFDHome)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->HFDHome(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, HFDMenu)
{
    EXPECT_FALSE(m_agentPhoneAU->HFDMenu(NULL));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->HFDMenu(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, BuildListHeader)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node xmlNode = xmlBulder.buildDisplayElement("Common", "ScreenDisplay");
    xmlBulder.buildGivenElement(xmlNode, "agent", "phone", "", "");
    xmlBulder.buildGivenElement(xmlNode, "content", "na_phone_list_messages", "", "");
    pugi::xml_node selectListNode = xmlBulder.buildGivenElement(xmlNode, "selectList", "", "", "");
    pugi::xml_node nodeList = xmlBulder.buildGivenElement(selectListNode, "list", "", "", "");
    VBT_STR strListCount = VeciAllocString(_T("5"), 2);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK)));
    m_agentPhoneAU->BuildListHeader(&m_pResultMsg, xmlBulder, nodeList);
    SUCCEED();
}

TEST_F(VR_VBT_AgentPhone_AU_Test, ListPartialNameMatches)
{
    VBT_ULONG uSize = 1;
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
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
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetNBestScreenFlg(_)).WillOnce(Return());
    
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(strContactName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strContactID), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strIndex), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->ListPartialNameMatches(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, ListPhoneTypes)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    VBT_STR strListType = VeciAllocString(_T("List"), 5);
    VBT_ULONG uSize = 1;
    VBT_STR strIndex = VeciAllocString(_T("5"), 2);
    VBT_STR strPhoneType = VeciAllocString(_T("Type"), 5);
    VBT_STR strPhoneNumber = VeciAllocString(_T("Number"), 7);
    VBT_STR strContactName = VeciAllocString(_T("Contact"), 8);
    VBT_STR strContactID = VeciAllocString(_T("3"), 2);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
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
    EXPECT_TRUE(m_agentPhoneAU->ListPhoneTypes(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, ListRecentCalls)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->ListRecentCalls(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, NoContactNumbers)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhoneAU->NoContactNumbers(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, NoContactsFound)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentPhoneAU->NoContactsFound(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, SendDTMF)
{
    VBT_STR strActionType = VeciAllocString(_T("SendDTMF"), 9);
    VBT_STR strDigits = VeciAllocString(_T("Digits"), 7);

    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strDigits), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->SendDTMF(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, SendDTMFCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Phone"), 6);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->SendDTMFCatch(&m_pResultMsg));
    EXPECT_FALSE(m_agentPhoneAU->SendDTMFCatch(NULL));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, SendDTMFConfirmed)
{
    VBT_STR strActionType = VeciAllocString(_T("SendDTMFConfirmed"), 18);
    VBT_STR strDigits = VeciAllocString(_T("Digits"), 7);

    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strDigits), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->SendDTMFConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, SendDTMFRejected)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->SendDTMFRejected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, IncomingCallInfoRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->IncomingCallInfoRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OutgoingCallInfoRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->OutgoingCallInfoRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, RecentCallsRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->RecentCallsRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, IncomingCallInfoReply_0_phoneType0)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>0</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhoneAU->IncomingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, IncomingCallInfoReply_0_phoneType1)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>1</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhoneAU->IncomingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, IncomingCallInfoReply_0_phoneType2)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>2</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhoneAU->IncomingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, IncomingCallInfoReply_0_phoneType3)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>3</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhoneAU->IncomingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, IncomingCallInfoReply_0_phoneType4)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>4</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhoneAU->IncomingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, IncomingCallInfoReply_ERR_PHONE_NO_INCOMMING)
{
    std::string xml = "<errcode>ERR_PHONE_NO_INCOMMING</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_command, DriverXmlReply(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhoneAU->IncomingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OutgoingCallInfoReply_0_phoneType0)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>0</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhoneAU->OutgoingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OutgoingCallInfoReply_0_phoneType1)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>1</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhoneAU->OutgoingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OutgoingCallInfoReply_0_phoneType2)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>2</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhoneAU->OutgoingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OutgoingCallInfoReply_0_phoneType3)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>3</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhoneAU->OutgoingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OutgoingCallInfoReply_0_phoneType4)
{
    std::string xml = "<errcode>0</errcode>"
                      "<contactName>contact</contactName>"
                      "<number>18934763862</number>"
                      "<contactId>23</contactId>"
                      "<phoneType>4</phoneType>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_callback, GetPhonBookDBPath()).WillOnce(Return(""));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentPhoneAU->OutgoingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, OutgoingCallInfoReply_ERR_PHONE_NO_OUTGOING)
{
    std::string xml = "<errcode>ERR_PHONE_NO_OUTGOING</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_command, DriverXmlReply(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentPhoneAU->OutgoingCallInfoReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, RecentCallsReply_0)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<phoneInfo>" 
            "<contactId>1</contactId>"
            "<phoneKind>android</phoneKind>"
            "<contactName>contact</contactName>"
            "<phoneType>type</phoneType>"
            "<number>1223</number>"
            "<timeStamp>1713</timeStamp>"
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
    EXPECT_TRUE(m_agentPhoneAU->RecentCallsReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, RecentCallsReply_ERR_PHONE_NO_RECENT_CALL_LIST)
{
    MockVR_VBT_VECIParameterSet spParams;
    MockVR_VBT_VECIListItems pListItems;
    MockVR_VBT_VECIParameterSet spParamsList;
    std::string xml = "<phoneInfo>" 
            "<contactId>1</contactId>"
            "<phoneKind>android</phoneKind>"
            "<contactName>contact</contactName>"
            "<phoneType>type</phoneType>"
            "<number>1223</number>"
            "<timeStamp>1713</timeStamp>"
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
    EXPECT_TRUE(m_agentPhoneAU->RecentCallsReply(parser));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneBack)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneBack(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneCancel)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneCancel(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneHelp)
{
    EXPECT_TRUE(m_agentPhoneAU->PhoneHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneStartOver)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneStartOver(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneStartPause)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneStartPause(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentPhone_AU_Test, PhoneStartRepeat)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentPhoneAU->PhoneStartRepeat(&m_pResultMsg));
}

/* EOF */

