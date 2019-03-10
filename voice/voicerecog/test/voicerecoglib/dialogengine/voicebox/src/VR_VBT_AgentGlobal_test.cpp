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
#include "VR_VoiceBoxXmlParser.h"
#include "VR_VoiceBoxAgentGlobal.h"
#include "VR_VoiceBoxCatalogPhone.h"
#include "VECIOutStr.h"
#include "VBTSystemNotificationCodes.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;
/**
* VR_VBT_AgentGlobal_Test
*
* The class is just for VR_VoiceBoxAgentGlobal test.
*/
class VR_VBT_AgentGlobal_Test : public testing::Test
{
public:
    VR_VBT_AgentGlobal_Test() : m_agentGlobal(NULL)
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

    VR_VoiceBoxAgentGlobal*    m_agentGlobal;
    VR_VoiceBoxCatalogPhone*   m_catalogPhone;

private:
    VR_VBT_AgentGlobal_Test(const VR_VBT_AgentGlobal_Test& other);
    void operator= (const VR_VBT_AgentGlobal_Test& other);
};

void
VR_VBT_AgentGlobal_Test::SetUp()
{
    m_agentGlobal = new VR_VoiceBoxAgentGlobal(
                m_engine,
                m_command,
                m_callback);

    m_catalogPhone = new VR_VoiceBoxCatalogPhone(
                m_engine,
                m_callback);
}

void
VR_VBT_AgentGlobal_Test::TearDown()
{
    delete m_agentGlobal;
    m_agentGlobal = NULL;
    delete m_catalogPhone;
    m_catalogPhone = NULL;
}

TEST_F(VR_VBT_AgentGlobal_Test, Initialize)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    EXPECT_TRUE(ret);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, ReplyQueryInfo)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<action><op>queryVehicleinMotion</op></action>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->ReplyQueryInfo(parser));
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, ProcessMessage_True)
{
    std::string message = _T("<Message source=\"VBT Button Down\""
                            "class=\"System\" ref=\"Client\"></Message>");
    EXPECT_TRUE(m_agentGlobal->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentGlobal_Test, ProcessMessage_NativeVR)
{
    std::string message = _T("<Message source=\"startNativeVR\""
                            "class=\"System\" ref=\"Client\"></Message>");
    EXPECT_TRUE(m_agentGlobal->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentGlobal_Test, ProcessMessage_clientRenderDone)
{
    std::string message = _T("<Message source=\"clientRenderDone\""
                            "class=\"System\" ref=\"Client\"></Message>");
    EXPECT_TRUE(m_agentGlobal->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentGlobal_Test, ProcessMessage_False)
{
    std::string message = _T("<Message source=\"VBT Cancel\" class=\"System\""
                            "ref=\"Client\"></Message>");
    EXPECT_TRUE(m_agentGlobal->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentGlobal_Test, ProcessMessage_Else)
{
    std::string message = _T("<Message source=\"VBT\" class=\"System\" ref=\"Client\"></Message>");
    EXPECT_TRUE(m_agentGlobal->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentGlobal_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Global");
    HRESULT result;
    result = m_agentGlobal->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, QueryAgentSupport_S_OK)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string message = _T("<Message source=\"VBT Button Down\""
                            "class=\"System\" ref=\"Client\"></Message>");
    m_agentGlobal->ProcessMessage(message);

    VBT_CSTR szAgent = _T("System");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result = m_agentGlobal->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(VBT_TRUE == bSupported);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, QueryAgentSupport_True_System)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).Times(1).WillRepeatedly(
                DoAll(SetArgPointee<2>(VBT_TRUE), Return(S_OK)));

    VBT_CSTR szAgent = _T("System");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentGlobal->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, QueryAgentSupport_True_Help)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(VBT_TRUE), Return(S_OK)));

    VBT_CSTR szAgent = _T("Help");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentGlobal->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, QueryAgentSupport_True_Speaker_Adaptation)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(VBT_TRUE), Return(S_OK)));

    VBT_CSTR szAgent = _T("Speaker Adaptation");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentGlobal->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, QueryAgentSupport_True_Root)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(VBT_TRUE),  Return(S_OK)));

    VBT_CSTR szAgent = _T("Root");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentGlobal->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }

    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).Times(4).WillRepeatedly(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK)));

    VBT_CSTR szAgent = _T("System");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentGlobal->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_FALSE(bSupported);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, OnProcessStatus_Fail)
{
    EXPECT_EQ(E_FAIL, m_agentGlobal->OnProcessStatus(NULL));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnProcessStatus_S_OK)
{
    EXPECT_EQ(S_OK, m_agentGlobal->OnProcessStatus(_T("Global")));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_szMessageNULL)
{
    VBT_STR szMessage = NULL;
    m_agentGlobal->m_bTslState = true;

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_RECOGNIZED, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_StateProcessing)
{
    VBT_STR szMessage = VeciAllocString(_T(""), 1);
    m_agentGlobal->m_bTslState = false;

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK)));
    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_RECOGNIZED, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_S_OK)
{
    VBT_STR szMessage = VeciAllocString(_T("VBT"), 4);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());

    std::string message = _T("<Message source=\"VBT Button Down\""
                            "class=\"System\" ref=\"Client\"></Message>");
    m_agentGlobal->ProcessMessage(message);
    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(1, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_CaseOne)
{
    VBT_STR szMessage = VeciAllocString(_T("VBT"), 4);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK)));

    EXPECT_CALL(m_callback, SendVRState(_, _, _, _)).WillOnce(Return());

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_ASR_LISTENING,
                                                        &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_CaseTwo)
{
    VBT_STR szMessage = VeciAllocString(_T("VBT"), 4);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK)));
    EXPECT_CALL(m_callback, OnBeep(VR_BeepType_Confirmation)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_ASR_NOT_LISTENING,
                                                        &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_CaseThree)
{
    VBT_STR szMessage = VeciAllocString(_T("VBT"), 4);
    VBT_STR szAsrType = VeciAllocString(_T("Type"), 5);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(szAsrType), Return(S_OK)));

    EXPECT_CALL(m_callback, GetNBestScreenFlg()).WillOnce(Return(true));
    EXPECT_CALL(m_callback, SendVRState(_, _, _, _)).WillOnce(Return());

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_RECOGNIZED, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_OSD)
{
    VBT_STR szMessage = VeciAllocString(_T("1"), 2);
    VBT_STR szAsrType = VeciAllocString(_T("OSD"), 4);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(szAsrType), Return(S_OK)));

    EXPECT_CALL(m_callback, GetNBestScreenFlg()).WillOnce(Return(true));
    EXPECT_CALL(m_callback, SendVRState(_, _, _, _)).WillOnce(Return());

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_RECOGNIZED, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_Asrtype_NULL)
{
    VBT_STR szMessage = VeciAllocString(_T("VBT"), 4);
    VBT_STR szAsrType = VeciAllocString(_T(""), 1);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(szAsrType), Return(S_OK)));

    EXPECT_CALL(m_callback, GetNBestScreenFlg()).WillOnce(Return(true));
    EXPECT_CALL(m_callback, SendVRState(_, _, _, _)).WillOnce(Return());

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_RECOGNIZED, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_CaseFour)
{
    VBT_STR szMessage = VeciAllocString(_T("VBT"), 4);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK)));

    EXPECT_CALL(m_callback, SendVRState(_, _, _, _)).WillOnce(Return());

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_NOT_RECOGNIZED, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_CaseFive_1)
{
    VBT_STR szMessage = VeciAllocString(_T("3"), 2);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK)));

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_ASR_GRAMMAR_DYNAMIC_ID_ERROR, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_CaseFive_2)
{
    VBT_STR szMessage = VeciAllocString(_T("3"), 2);
    m_agentGlobal->m_iGrammarErrCount = 5;

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK)));

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_ASR_GRAMMAR_DYNAMIC_ID_ERROR, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_CaseSix)
{
    VBT_STR szMessage = VeciAllocString(_T("Message"), 8);
    VBT_STR szUtteranceStartTime = VeciAllocString(_T("4"), 2);

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(szUtteranceStartTime), Return(S_OK)));

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_ASR_BEGINNING_OF_UTTERANCE, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_CaseSixNULL)
{
    VBT_STR szMessage = VeciAllocString(_T("Message"), 8);
    VBT_STR szUtteranceStartTime = NULL;

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(szUtteranceStartTime), Return(S_OK)));

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_ASR_BEGINNING_OF_UTTERANCE, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSystemNotification_Asrtype_else)
{
    VBT_STR szMessage = VeciAllocString(_T("VBT"), 4);
    VBT_STR szAsrType = NULL;

    EXPECT_CALL(m_paraSet, GetParameterInfo(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(szMessage), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(szAsrType), Return(S_OK)));

    EXPECT_CALL(m_callback, GetNBestScreenFlg()).WillOnce(Return(true));
    EXPECT_CALL(m_callback, SendVRState(_, _, _, _)).WillOnce(Return());

    EXPECT_EQ(S_OK, m_agentGlobal->OnSystemNotification(VBT_NOTIFICATION_RECOGNIZED, &m_paraSet));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnAgentResultMessage_State)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR xmlMsg = VeciAllocString(_T("xmlMsg"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());

    std::string message = _T("<Message source=\"VBT Button Down\""
                            "class=\"System\" ref=\"Client\"></Message>");
    m_agentGlobal->ProcessMessage(message);
    HRESULT result = m_agentGlobal->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentGlobal->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentGlobal_Test, OnAgentResultMessage)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    HRESULT result = m_agentGlobal->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentGlobal->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentGlobal_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentGlobal->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, OnAgentDriverMessage_State)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR xmlMsg = VeciAllocString(_T("xmlMsg"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());

    std::string message = _T("<Message source=\"VBT Button Down\""
                            "class=\"System\" ref=\"Client\"></Message>");
    m_agentGlobal->ProcessMessage(message);
    HRESULT result = m_agentGlobal->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentGlobal->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentGlobal_Test, OnAgentDriverMessage)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    HRESULT result = m_agentGlobal->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentGlobal->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentGlobal_Test, OnSpeak_NULL)
{
    EXPECT_EQ(E_FAIL, m_agentGlobal->OnSpeak(NULL, NULL));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSpeak)
 {
    VBT_ULONG uSize = 1;
    VBT_STR str = _T("Speak");
    EXPECT_CALL(m_stringSet, GetSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_stringSet, GetItem(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(str), Return(S_OK)));
    EXPECT_CALL(m_callback, OnSpeak(_, _)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnSpeak(&m_pTrans, &m_stringSet));
 }

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete)
{
    VBT_STR str = VeciAllocString(_T("Grammar"), 8);
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(str), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_NULL)
{
    EXPECT_EQ(E_POINTER, m_agentGlobal->OnCommandComplete(NULL));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_music_1)
{
    VBT_STR strTrans = VeciAllocString(_T("AddGrammar"), 11);
    VBT_ULONG ulErrorCode = S_OK;

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "music";
    m_agentGlobal->m_bGrammarErr = false;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_callback, PostCatalogStatus(_)).Times(2).WillRepeatedly(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, ConfirmMusicUpdateGrammar(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetWaitResumeGrammar(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_music_2)
{
    VBT_STR strTrans = VeciAllocString(_T("AddGrammar"), 11);
    VBT_ULONG ulErrorCode = S_OK;

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "music";
    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_callback, ResetGrammarUpdate()).WillOnce(Return(false));
    EXPECT_CALL(m_callback, PostCatalogStatus(_)).Times(2).WillRepeatedly(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, ConfirmMusicUpdateGrammar(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetWaitResumeGrammar(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_hfd)
{
    VBT_STR strTrans = VeciAllocString(_T("ReloadGrammar"), 14);
    VBT_ULONG ulErrorCode = S_OK;

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "hfd";
    m_agentGlobal->m_bGrammarErr = false;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_callback, PostCatalogStatus(_)).Times(2).WillRepeatedly(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetWaitResumeGrammar(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_RemoveGrammar)
{
    VBT_STR strTrans = VeciAllocString(_T("RemoveGrammar"), 14);
    VBT_ULONG ulErrorCode = S_OK;

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "music";
    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_callback, ResetGrammarUpdate()).WillOnce(Return(false));
    EXPECT_CALL(m_callback, PostCatalogStatus(_)).Times(2).WillRepeatedly(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, ConfirmMusicUpdateGrammar(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetWaitResumeGrammar(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_RemoveAllGrammars)
{
    VBT_STR strTrans = VeciAllocString(_T("RemoveAllGrammars"), 18);
    VBT_ULONG ulErrorCode = S_OK;

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "hfd";
    m_agentGlobal->m_bGrammarErr = false;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_callback, PostCatalogStatus(_)).Times(2).WillRepeatedly(Return());

    VoiceMap<std::string, std::string>::type mapSaveTransactionId;
    mapSaveTransactionId.insert(std::make_pair("RemoveAllGrammars", "Call"));
    m_catalogPhone->m_mapSaveTransactionId = mapSaveTransactionId;

    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetWaitResumeGrammar(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_RemoveGrammar_hfd_else)
{
    VBT_STR strTrans = VeciAllocString(_T("RemoveGrammar"), 14);
    VBT_ULONG ulErrorCode = S_OK;

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "hfd";
    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_callback, ResetGrammarUpdate()).WillOnce(Return(false));
    EXPECT_CALL(m_callback, PostCatalogStatus(_)).Times(2).WillRepeatedly(Return());

    VoiceMap<std::string, std::string>::type mapSaveTransactionId;
    mapSaveTransactionId.insert(std::make_pair("RemoveGrammar", "Call"));
    m_catalogPhone->m_mapSaveTransactionId = mapSaveTransactionId;

    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetWaitResumeGrammar(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_PauseGrammar_1)
{
    VBT_STR strTrans = VeciAllocString(_T("PauseGrammar"), 13);
    VBT_ULONG ulErrorCode = S_OK;

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "music";
    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, SetWaitResumeGrammar(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_PauseGrammar_2)
{
    VBT_STR strTrans = VeciAllocString(_T("PauseGrammar"), 13);
    VBT_ULONG ulErrorCode = S_FALSE;

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "music";
    pairAgent2TransId.second = "song";
    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_callback, PostCatalogStatus(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_PauseGrammar_3)
{
    VBT_STR strTrans = VeciAllocString(_T("PauseGrammar"), 13);
    VBT_ULONG ulErrorCode = S_PAUSED;

    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));

    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, PostCatalogStatus(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_ResumeGrammar)
{
    VBT_STR strTrans = VeciAllocString(_T("ResumeGrammar"), 14);
    VBT_ULONG ulErrorCode = S_PAUSED;

    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, SetWaitResumeGrammar(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, PostCatalogStatus(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_RemoveGrammar_PAUSED)
{
    VBT_STR strTrans = VeciAllocString(_T("RemoveGrammar"), 14);
    VBT_ULONG ulErrorCode = S_PAUSED;
    std::pair<std::string, std::string> pairAgent2TransId;

    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_SetDataActiveSource)
{
    VBT_STR strTrans = VeciAllocString(_T("SetDataActiveSource"), 20);
    VBT_ULONG ulErrorCode = S_OK;

    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetOpActiveSouce(_, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_SetDataSynchronized)
{
    VBT_STR strTrans = VeciAllocString(_T("SetDataSynchronized"), 20);
    VBT_ULONG ulErrorCode = S_OK;
    std::string strDataSyncTransId = "SetDataSynchronized";
    m_agentGlobal->m_bGrammarErr = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, GetPhoneDataSyncTransId()).WillOnce(Return(strDataSyncTransId));
    EXPECT_CALL(m_callback, SetPhoneReloadDataFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, IsBeingVRSession()).WillOnce(Return(false));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCommandComplete_TslState)
{
    VBT_STR strTrans = VeciAllocString(_T("Grammar"), 8);
    VBT_ULONG ulErrorCode = S_OK;
    m_agentGlobal->m_bTslState = true;
    EXPECT_CALL(m_pTrans, GetResultErrorCode(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulErrorCode), Return(S_OK)));
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));

    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnCommandComplete(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSendXMLMessage_NULL)
{
    EXPECT_FALSE(m_agentGlobal->OnSendXMLMessage(NULL));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSendXMLMessage_SetPropertyFail)
{
    EXPECT_CALL(m_engine, SetProperty(_, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentGlobal->OnSendXMLMessage(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSendXMLMessage_SetPropertyTrueFail)
{
    EXPECT_CALL(m_engine, SetProperty(_, _)).Times(2).WillOnce(Return(S_OK)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentGlobal->OnSendXMLMessage(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSendXMLMessage_StartRecoSessionFail)
{
    EXPECT_CALL(m_engine, SetProperty(_, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, StartRecoSession(_, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(E_FAIL)));
    EXPECT_FALSE(m_agentGlobal->OnSendXMLMessage(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSendXMLMessage_GetTransactionIdFail)
{
    VBT_STR strTrans = VeciAllocString(_T("Xml"), 4);
    EXPECT_CALL(m_engine, SetProperty(_, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, StartRecoSession(_, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(E_FAIL)));
    EXPECT_FALSE(m_agentGlobal->OnSendXMLMessage(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnSendXMLMessage)
{
    VBT_STR strTrans = VeciAllocString(_T("Xml"), 4);
    EXPECT_CALL(m_engine, SetProperty(_, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, StartRecoSession(_, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->OnSendXMLMessage(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnStartRecoSession_NULL)
{
    EXPECT_FALSE(m_agentGlobal->OnStartRecoSession(NULL));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnStartRecoSession_SetPropertyFail)
{
    EXPECT_CALL(m_engine, SetProperty(_, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentGlobal->OnStartRecoSession(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnStartRecoSession_SetPropertyTrueFail)
{
    EXPECT_CALL(m_engine, SetProperty(_, _)).Times(2).WillOnce(Return(S_OK)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentGlobal->OnStartRecoSession(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnStartRecoSession_GetResultMessageFail)
{
    EXPECT_CALL(m_engine, SetProperty(_, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_pTrans, GetResultMessage(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pResultMsg), Return(E_FAIL)));
    EXPECT_FALSE(m_agentGlobal->OnStartRecoSession(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnStartRecoSession_GetStringFail)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Session"), 8);
    EXPECT_CALL(m_engine, SetProperty(_, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_pTrans, GetResultMessage(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pResultMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(E_FAIL)));
    EXPECT_FALSE(m_agentGlobal->OnStartRecoSession(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnStartRecoSession)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Session"), 8);
    EXPECT_CALL(m_engine, SetProperty(_, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_pTrans, GetResultMessage(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pResultMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->OnStartRecoSession(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCancelSpecific_NULL)
{
    EXPECT_FALSE(m_agentGlobal->OnCancelSpecific(NULL));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCancelSpecific_GetResultMessageFail)
{
    EXPECT_CALL(m_pTrans, GetResultMessage(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pResultMsg), Return(E_FAIL)));
    EXPECT_FALSE(m_agentGlobal->OnCancelSpecific(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCancelSpecific_GetStringFail)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Specific"), 9);
    EXPECT_CALL(m_pTrans, GetResultMessage(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pResultMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(E_FAIL)));
    EXPECT_FALSE(m_agentGlobal->OnCancelSpecific(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnCancelSpecific)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Specific"), 9);
    EXPECT_CALL(m_pTrans, GetResultMessage(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pResultMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->OnCancelSpecific(&m_pTrans));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnBeepSync)
{
    EXPECT_CALL(m_callback, OnBeep(VR_BeepType_Listening)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnBeepSync());
}

TEST_F(VR_VBT_AgentGlobal_Test, OnAudioLevelUpdate)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_EQ(S_OK, m_agentGlobal->OnAudioLevelUpdate(1));
}

TEST_F(VR_VBT_AgentGlobal_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentGlobal->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentGlobal->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentGlobal_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    bool result = m_agentGlobal->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, ProcessAgentMessage_ActionType_True)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    bool result = m_agentGlobal->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentGlobal->UnInitialize();
}

TEST_F(VR_VBT_AgentGlobal_Test, ProcessAgentMessage_ListType_True)
{
    if (NULL == m_agentGlobal) {
        FAIL();
    }

    bool ret = m_agentGlobal->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    bool result = m_agentGlobal->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentGlobal->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentGlobal_Test, SystemBack)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SystemBack(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemBeginOfList)
{
    EXPECT_FALSE(m_agentGlobal->SystemBeginOfList(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemCancel)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SystemCancel(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, HintFirstPage)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->HintFirstPage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, HintLastPage)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->HintLastPage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, HintPrevPage)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->HintPrevPage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, HintNextPage)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->HintNextPage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemCSVRComplete)
{
    EXPECT_FALSE(m_agentGlobal->SystemCSVRComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemEndOfList)
{
    EXPECT_FALSE(m_agentGlobal->SystemEndOfList(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemErrorLimitReached)
{
    EXPECT_FALSE(m_agentGlobal->SystemErrorLimitReached(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemFirstPage)
{
    EXPECT_FALSE(m_agentGlobal->SystemFirstPage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemGenericConfirm)
{
    EXPECT_FALSE(m_agentGlobal->SystemGenericConfirm(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemGenericReject)
{
    EXPECT_FALSE(m_agentGlobal->SystemGenericReject(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemGreeting)
{
    EXPECT_FALSE(m_agentGlobal->SystemGreeting(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemHelp)
{
    EXPECT_TRUE(m_agentGlobal->SystemHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemLastPage)
{
    EXPECT_FALSE(m_agentGlobal->SystemLastPage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemNextPage)
{
    EXPECT_FALSE(m_agentGlobal->SystemNextPage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemNoReco)
{
    EXPECT_FALSE(m_agentGlobal->SystemNoReco(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemConnectError)
{
    EXPECT_FALSE(m_agentGlobal->SystemConnectError(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemPreviousPage)
{
    EXPECT_FALSE(m_agentGlobal->SystemPreviousPage(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemStartOver)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_FALSE(m_agentGlobal->SystemStartOver(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemPause)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_FALSE(m_agentGlobal->SystemPause(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemRepeat)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_FALSE(m_agentGlobal->SystemRepeat(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemVBTError)
{
    EXPECT_FALSE(m_agentGlobal->SystemVBTError(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemSendCommand)
{
    EXPECT_FALSE(m_agentGlobal->SystemSendCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemVersionInfo)
{
    EXPECT_FALSE(m_agentGlobal->SystemVersionInfo(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemWhatCanISay)
{
    EXPECT_FALSE(m_agentGlobal->SystemWhatCanISay(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step2)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 01 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step3)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 02 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step4)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 03 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step5)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 04 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step6)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 05 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step7)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 06 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step8)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 07 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step9)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 08 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Step10)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 09 Completed"), 24);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Complete)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 10 Completed"), 24);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptationCommand_Error)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strActionType = VeciAllocString(_T("Command 11 Completed"), 24);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_FALSE(m_agentGlobal->SpeakerAdaptationCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, StartSpeakerAdaptation)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    VBT_STR strPhrase = VeciAllocString(_T("phrase"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->StartSpeakerAdaptation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, AddressEntryHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->AddressEntryHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, AddressEntryHelpNotEnabled)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->AddressEntryHelpNotEnabled(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, AppsHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->AppsHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, AppsHelpNotEnabled)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->AppsHelpNotEnabled(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, ConfirmTutorialConfirm)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Global"), 7);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->ConfirmTutorialConfirm(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, ConfirmTutorialReject)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->ConfirmTutorialReject(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, DestinationHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->DestinationHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, DestinationHelpNotEnabled)
{
    VBT_STR xmlMsg = VeciAllocString(_T("DestinationHelpNotEnabled"), 64);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->DestinationHelpNotEnabled(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemExit)
{
    VBT_STR strActionType = VeciAllocString(_T("Exit"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->SystemExit(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, HVACHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->HVACHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, InformationHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->InformationHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, InformationHelpNotEnabled)
{
    VBT_STR xmlMsg = VeciAllocString(_T("InformationHelpNotEnabled"), 64);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->InformationHelpNotEnabled(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, GenericKeyboardDictation)
{
    EXPECT_FALSE(m_agentGlobal->GenericKeyboardDictation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, LexusInsiderCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("LexusInsiderCatch"), 64);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->LexusInsiderCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, MessagingHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->MessagingHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemMoreHints)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SystemMoreHints(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, MusicHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->MusicHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, PhoneHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->PhoneHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, POIHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->POIHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, POIHelpNotEnabled)
{
    VBT_STR xmlMsg = VeciAllocString(_T("POIHelpNotEnabled"), 64);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->POIHelpNotEnabled(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, RadioHelp)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->RadioHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SystemSettings)
{
    VBT_STR xmlMsg = VeciAllocString(_T("SystemSettings"), 64);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SystemSettings(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, SpeakerAdaptionCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("SpeakerAdaptionCatch"), 64);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->SpeakerAdaptionCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, GlobalTaskComplete)
{
    VBT_STR strActionType = VeciAllocString(_T("GlobalTaskComplete"), 64);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->GlobalTaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, TutorialCatch)
{
    VBT_STR xmlMsg = VeciAllocString(_T("SpeakerAdaptionCatch"), 64);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));

    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->TutorialCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, GettingStartedWithVoice)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->GettingStartedWithVoice(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, HintFristPageRequest)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->HintFristPageRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, HintLastPageRequest)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->HintLastPageRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, VehicleInMotionRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->VehicleInMotionRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, EscalatingError)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->EscalatingError(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, FirstPageReply_False)
{
    VBT_STR xml = _T("<value>false</value>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentGlobal->FirstPageReply(parser));
}

TEST_F(VR_VBT_AgentGlobal_Test, FirstPageReply_True)
{
    VBT_STR xml = _T("<value>true</value>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentGlobal->FirstPageReply(parser));
}

TEST_F(VR_VBT_AgentGlobal_Test, LastPageReply_False)
{
    VBT_STR xml = _T("<value>false</value>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentGlobal->LastPageReply(parser));
}

TEST_F(VR_VBT_AgentGlobal_Test, LastPageReply_True)
{
    VBT_STR xml = _T("<value>true</value>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentGlobal->LastPageReply(parser));
}

TEST_F(VR_VBT_AgentGlobal_Test, VehicleInMotionReply_False)
{
    VBT_STR xml = _T("<motion>false</motion>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentGlobal->VehicleInMotionReply(parser));
}

TEST_F(VR_VBT_AgentGlobal_Test, VehicleInMotionReply_True)
{
    VBT_STR xml = _T("<motion>true</motion>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentGlobal->VehicleInMotionReply(parser));
}

TEST_F(VR_VBT_AgentGlobal_Test, VehicleInMotionReply_Else)
{
    VBT_STR xml = _T("<motion>else</motion>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_TRUE(m_agentGlobal->VehicleInMotionReply(parser));
}

TEST_F(VR_VBT_AgentGlobal_Test, NullOperation)
{
    VBT_STR xmlMsg = VeciAllocString(_T("NullOperation"), 64);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_FALSE(m_agentGlobal->NullOperation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, GlobalBackAlreadyAtRoot)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentGlobal->GlobalBackAlreadyAtRoot(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentGlobal_Test, OnExtendedEvent_NULL)
{
    VeciExtendedEvent type = ExEventEngineMaintenanceReport;
    EXPECT_EQ(E_FAIL, m_agentGlobal->OnExtendedEvent(type, NULL));
}

/* EOF */
