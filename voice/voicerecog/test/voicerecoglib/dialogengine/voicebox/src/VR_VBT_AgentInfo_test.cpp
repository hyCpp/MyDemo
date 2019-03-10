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
#include "VR_VBT_VECITransaction_mock.h"
#include "VR_VBT_VECIParsedMessage_mock.h"

#include "VR_VoiceBoxXmlBuilder.h"
#include "VR_VoiceBoxAgentInfo.h"
#include "VR_VoiceBoxLog.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
/**
* VR_VBT_AgentInfo_Test
*
* The class is just for VR_VoiceBoxAgentInfo test.
*/

class VR_VBT_AgentInfo_Test : public testing::Test
{
public:
    VR_VBT_AgentInfo_Test() : m_agentInfo(NULL)
    {
    }

    virtual void SetUp();
    virtual void TearDown();

public:
    MockVR_VBT_VECIEngineClient m_engine;
    MockVR_VBT_VECIEngineCommand m_command;
    MockVR_VBT_EngineCallback m_callback;
    MockVR_VBT_VECIParsedMessage m_pResultMsg;
    MockVR_VBT_VECITransaction m_pTrans;

    VR_VoiceBoxAgentInfo* m_agentInfo;

private:
    VR_VBT_AgentInfo_Test(const VR_VBT_AgentInfo_Test& other);
    void operator= (const VR_VBT_AgentInfo_Test& other);
};

void
VR_VBT_AgentInfo_Test::SetUp()
{
    m_agentInfo = new VR_VoiceBoxAgentInfo(
                m_engine,
                m_command,
                m_callback);
}

void
VR_VBT_AgentInfo_Test::TearDown()
{
    delete m_agentInfo;
    m_agentInfo = NULL;
}

TEST_F(VR_VBT_AgentInfo_Test, Initialize)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    EXPECT_TRUE(ret);
    m_agentInfo->UnInitialize();
}

TEST_F(VR_VBT_AgentInfo_Test, ReplyQueryInfo)
{
    std::string xml = "<action><op></op></action>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentInfo->ReplyQueryInfo(parser));
}


TEST_F(VR_VBT_AgentInfo_Test, QueryAgentSupport_True)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }

    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(VBT_TRUE), Return(S_OK)));

    VBT_CSTR szAgent = _T("Apps");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result = m_agentInfo->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(VBT_TRUE == bSupported);
    m_agentInfo->UnInitialize();
}

TEST_F(VR_VBT_AgentInfo_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }

    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK)));

    VBT_CSTR szAgent = _T("Apps");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result = m_agentInfo->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(VBT_FALSE == bSupported);
    m_agentInfo->UnInitialize();
}

TEST_F(VR_VBT_AgentInfo_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Apps");
    HRESULT result = m_agentInfo->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentInfo->UnInitialize();
}

TEST_F(VR_VBT_AgentInfo_Test, OnAgentResultMessage_Fail)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentInfo->OnAgentResultMessage(NULL, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentInfo->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentInfo_Test, OnAgentResultMessage)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    HRESULT result = m_agentInfo->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentInfo->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentInfo_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentInfo->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentInfo->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentInfo_Test, OnAgentDriverMessage)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    HRESULT result = m_agentInfo->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentInfo->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentInfo_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentInfo->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentInfo->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentInfo_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    bool result = m_agentInfo->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentInfo->UnInitialize();
}

TEST_F(VR_VBT_AgentInfo_Test, ProcessAgentMessage_True)
{
    if (NULL == m_agentInfo) {
        FAIL();
    }

    bool ret = m_agentInfo->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    bool result = m_agentInfo->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentInfo->UnInitialize();
}
*/
/* EOF */
