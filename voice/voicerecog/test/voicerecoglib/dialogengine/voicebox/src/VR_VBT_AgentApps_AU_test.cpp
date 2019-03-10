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
#include "VR_VBT_VECIStringSet_mock.h"

#include "VR_VoiceBoxXmlBuilder.h"
#include "VR_VoiceBoxAgentApps_AU.h"
#include "VR_VoiceBoxLog.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_AgentApps_AU_Test
*
* The class is just for VR_VoiceBoxAgentApps_AU test.
*/

class VR_VBT_AgentApps_AU_Test : public testing::Test
{
public:
    VR_VBT_AgentApps_AU_Test() : m_agentAppsAU(NULL)
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
    MockVR_VBT_VECIStringSet m_mockStringSet;
    VR_VoiceBoxAgentApps_AU* m_agentAppsAU;

private:
    VR_VBT_AgentApps_AU_Test(const VR_VBT_AgentApps_AU_Test& other);
    void operator= (const VR_VBT_AgentApps_AU_Test& other);
};

void
VR_VBT_AgentApps_AU_Test::SetUp()
{
    m_agentAppsAU = new VR_VoiceBoxAgentApps_AU(
                m_engine,
                m_command,
                m_callback);
}

void
VR_VBT_AgentApps_AU_Test::TearDown()
{
    delete m_agentAppsAU;
    m_agentAppsAU = NULL;
}

TEST_F(VR_VBT_AgentApps_AU_Test, Initialize)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    EXPECT_TRUE(ret);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, QueryAgentSupport_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(VBT_TRUE), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));

    VBT_CSTR szAgent = _T("Apps");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result = m_agentAppsAU->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(VBT_TRUE == bSupported);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));

    VBT_CSTR szAgent = _T("Apps");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result = m_agentAppsAU->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(VBT_FALSE == bSupported);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Apps");
    HRESULT result = m_agentAppsAU->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, OnAgentResultMessage_Fail)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentAppsAU->OnAgentResultMessage(NULL, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAppsAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentApps_AU_Test, OnAgentResultMessage_OK)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    HRESULT result = m_agentAppsAU->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentAppsAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentApps_AU_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentAppsAU->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAppsAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentApps_AU_Test, OnAgentDriverMessage_OK)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    HRESULT result = m_agentAppsAU->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentAppsAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentApps_AU_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentApps_AU_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    bool result = m_agentAppsAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, ProcessAgentMessage_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    bool result = m_agentAppsAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, ProcessAgentMessage_False_NoSupport)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Apps"), 5);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillRepeatedly(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    bool result = m_agentAppsAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentApps_AU_Test, AppsHome_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppsHome(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppsHome_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    bool result = m_agentAppsAU->AppsHome(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, LaunchApp_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->LaunchApp(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, LaunchApp_True)
{
    //    if (NULL == m_agentAppsAU) {
    //        FAIL();
    //    }

    //    bool ret = m_agentAppsAU->Initialize();
    //    if (!ret) {
    //        FAIL();
    //    }
    //    //CVECIPtr<IVECIParameterSet> parmeterSet;
    //    VR_VBT_LOG("1:%p\n",parmeterSet);
    //    m_agentAppsAU->m_client.CreateParameterSet(&parmeterSet);
    //    VR_VBT_LOG("2:%p\n",parmeterSet);
    //    //parmeterSet->AddParameter("Status", "Success", "", "");
    //    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    //    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
    //        DoAll(SetArgPointee<0>(parmeterSet),Return(S_OK)));
    //    EXPECT_CALL(m_pResultMsg, GetActionParameters(_)).WillOnce(
    //        DoAll(SetArgPointee<0>(parmeterSet), Return(S_OK)));
    //    bool result = m_agentAppsAU->LaunchApp(&m_pResultMsg);
    //    EXPECT_TRUE(result);
    //    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, ListApps_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->ListApps(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, ListApps_True)
{
    //    if (NULL == m_agentAppsAU) {
    //        FAIL();
    //    }

    //    bool ret = m_agentAppsAU->Initialize();
    //    if (!ret) {
    //        FAIL();
    //    }

    //    bool result = m_agentAppsAU->ListApps(&m_pResultMsg);
    //    EXPECT_FALSE(result);
    //    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, NBest_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->NBest(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, NBest_True)
{
    //    if (NULL == m_agentAppsAU) {
    //        FAIL();
    //    }

    //    bool ret = m_agentAppsAU->Initialize();
    //    if (!ret) {
    //        FAIL();
    //    }

    //    bool result = m_agentAppsAU->NBest(&m_pResultMsg);
    //    EXPECT_FALSE(result);
    //    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppNotInstalled_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppNotInstalled(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppNotInstalled_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppNotInstalled(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppsNotAvailable_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppsNotAvailable(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppsNotAvailable_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppsNotAvailable(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}


TEST_F(VR_VBT_AgentApps_AU_Test, NoAppsAvailable_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->NoAppsAvailable(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, NoAppsAvailable_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->NoAppsAvailable(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppBack_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppBack(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppBack_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    bool result = m_agentAppsAU->AppBack(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppCancel_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppCancel(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppCancel_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    bool result = m_agentAppsAU->AppCancel(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppHelp_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppHelp(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppHelp_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppHelp(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppStartOver_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppStartOver(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppStartOver_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    bool result = m_agentAppsAU->AppStartOver(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppStartPause_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppStartPause(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppStartPause_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    bool result = m_agentAppsAU->AppStartPause(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppStartRepeat_False)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAppsAU->AppStartRepeat(NULL);
    EXPECT_FALSE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, AppStartRepeat_True)
{
    if (NULL == m_agentAppsAU) {
        FAIL();
    }

    bool ret = m_agentAppsAU->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    bool result = m_agentAppsAU->AppStartRepeat(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAppsAU->UnInitialize();
}

TEST_F(VR_VBT_AgentApps_AU_Test, ProcessMessage_InstallAgent)
{
    std::string message = "<event-result name=\"install\" errcode=\"0\">"
            "<TSLINFO>"
            "<Action type=\"Install Agent\">"
            "<Parameter name=\"Uninstall Previous Agent\" value=\"False\"/>"
            "</Action>"
            "</TSLINFO>"
            "</event-result>";
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAppsAU->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentApps_AU_Test, ProcessMessage_UninstallAgent)
{
    std::string message = "<event-result name=\"install\" errcode=\"0\">"
            "<TSLINFO>"
            "<Action type=\"Uninstall Agent\">"
            "<Parameter name=\"Agent\" value=\"Apps\"/>"
            "</Action>"
            "</TSLINFO>"
            "</event-result>";
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAppsAU->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentApps_AU_Test, ProcessMessage_UninstallAllAgent)
{
    std::string message = "<event-result name=\"install\" errcode=\"0\">"
            "<TSLINFO>"
            "<Action type=\"Uninstall All Agent\">"
            "<Parameter name=\"Agent\" value=\"Apps\"/>"
            "</Action>"
            "</TSLINFO>"
            "</event-result>";
    EXPECT_CALL(m_command, UninstallAllDownloadableAgent(_, _, _)).WillOnce(
                Return(E_FAIL));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAppsAU->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentApps_AU_Test, ProcessMessage_GetSupportLanguage)
{
    std::string message = "<event-result name=\"install\" errcode=\"0\">"
            "<TSLINFO>"
            "<Action type=\"Get Supported Language\">"
            "<Parameter name=\"Language\" value=\"es\"/>"
            "</Action>"
            "</TSLINFO>"
            "</event-result>";
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAppsAU->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentApps_AU_Test, ProcessMessage_False)
{
    std::string message = "<event-result name=\"install\" errcode=\"0\">"
            "<TSLINFO>"
            "<Action type=\"Language\">"
            "<Parameter name=\"Language\" value=\"es\"/>"
            "</Action>"
            "</TSLINFO>"
            "</event-result>";
    EXPECT_FALSE(m_agentAppsAU->ProcessMessage(message));
}

TEST_F(VR_VBT_AgentApps_AU_Test, DoInstall_UninstallPre)
{
    std::string xml = "<event-result name=\"install\" errcode=\"0\">"
            "<TSLINFO>"
            "<Action type=\"install Agent\">"
            "<Parameter name=\"Uninstall Previous Agent\" value=\"False\"/>"
            "<Parameter name=\"Path\" value=\"/usr/path\"/>"
            "<Parameter name=\"Language\" value=\"fr\"/>"
            "</Action>"
            "</TSLINFO>"
            "</event-result>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateStringSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_mockStringSet), Return(S_OK)));
    EXPECT_CALL(m_mockStringSet, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, InstallDownloadableAgent(_, _, _, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    EXPECT_CALL(m_pTrans, WaitForCompletion(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(VBT_FALSE), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());

    EXPECT_TRUE(m_agentAppsAU->DoInstall(parser));
}

TEST_F(VR_VBT_AgentApps_AU_Test, DoInstall_NoUninstallPre)
{
    EXPECT_CALL(m_command, UninstallAllDownloadableAgent(_, _, _)).WillOnce(Return(E_FAIL));
    std::string xml = "<event-result name=\"install\" errcode=\"0\">"
            "<TSLINFO>"
            "<Action type=\"install Agent\">"
            "<Parameter name=\"Uninstall Previous Agent\" value=\"True\"/>"
            "<Parameter name=\"Path\" value=\"/usr/path\"/>"
            "<Parameter name=\"Language\" value=\"fr\"/>"
            "</Action>"
            "</TSLINFO>"
            "</event-result>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateStringSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_mockStringSet), Return(S_OK)));
    EXPECT_CALL(m_mockStringSet, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, InstallDownloadableAgent(_, _, _, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    EXPECT_CALL(m_pTrans, WaitForCompletion(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(VBT_FALSE), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());

    EXPECT_TRUE(m_agentAppsAU->DoInstall(parser));
}

TEST_F(VR_VBT_AgentApps_AU_Test, DoUninstall_Agent)
{
    std::string xml = "<event-result name=\"uninstall\" errcode=\"0\">"
            "<TSLINFO>"
            "<Action type=\"Uninstalled Agent\">"
            "<Parameter name=\"Agent\" value=\"Apps\"/>"
            "<Parameter name=\"Language\" value=\"en\"/>"
            "</Action>"
            "</TSLINFO>"
            "</event-result>";
    VR_VoiceBoxXmlParser parser(xml);

    EXPECT_CALL(m_engine, CreateStringSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_mockStringSet), Return(S_OK)));
    EXPECT_CALL(m_mockStringSet, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, UninstallDownloadableAgent(_, _, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    EXPECT_CALL(m_pTrans, WaitForCompletion(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(VBT_FALSE), Return(S_OK)));
    EXPECT_TRUE(m_agentAppsAU->DoUninstall(parser));
}

TEST_F(VR_VBT_AgentApps_AU_Test, DoUninstallAll_Fail)
{
    EXPECT_CALL(m_command, UninstallAllDownloadableAgent(_, _, _)).WillOnce(
                Return(E_FAIL));
    EXPECT_FALSE(m_agentAppsAU->DoUninstallAll());
}

TEST_F(VR_VBT_AgentApps_AU_Test, DoUninstallAll_True)
{
    EXPECT_CALL(m_command, UninstallAllDownloadableAgent(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    EXPECT_CALL(m_pTrans, WaitForCompletion(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(VBT_FALSE), Return(S_OK)));
    EXPECT_TRUE(m_agentAppsAU->DoUninstallAll());
}

TEST_F(VR_VBT_AgentApps_AU_Test, DoUninstallAll_Wait_Fail)
{
    EXPECT_CALL(m_command, UninstallAllDownloadableAgent(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    EXPECT_CALL(m_pTrans, WaitForCompletion(_, _)).WillOnce(
                Return(E_FAIL));
    EXPECT_FALSE(m_agentAppsAU->DoUninstallAll());
}

TEST_F(VR_VBT_AgentApps_AU_Test, GetCulture)
{
    std::string language = _T("en");
    EXPECT_STREQ(_T("en-US"), m_agentAppsAU->GetCulture(language).c_str());
    language = _T("fr");
    EXPECT_STREQ(_T("fr-CA"), m_agentAppsAU->GetCulture(language).c_str());
    language = _T("es");
    EXPECT_STREQ(_T("en-MX"), m_agentAppsAU->GetCulture(language).c_str());
    language = _T("");
    EXPECT_STREQ(_T(""), m_agentAppsAU->GetCulture(language).c_str());
}

/* EOF */
