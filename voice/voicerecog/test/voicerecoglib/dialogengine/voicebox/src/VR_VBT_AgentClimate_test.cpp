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
#include "VR_VBT_VECIParameterSet_mock.h"

#include "VR_VoiceBoxXmlParser.h"
#include "VR_VoiceBoxAgentClimate.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_AgentClimate_Test
*
* The class is just for VR_VoiceBoxAgentClimate test.
*/
class VR_VBT_AgentClimate_Test : public testing::Test
{
public:
    VR_VBT_AgentClimate_Test() : m_agentClimate(NULL)
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
    MockVR_VBT_VECIParameterSet m_paraSet;
    VR_VoiceBoxAgentClimate* m_agentClimate;

private:
    VR_VBT_AgentClimate_Test(const VR_VBT_AgentClimate_Test& other);
    void operator= (const VR_VBT_AgentClimate_Test& other);
};

void
VR_VBT_AgentClimate_Test::SetUp()
{
    m_agentClimate = new VR_VoiceBoxAgentClimate(
                m_engine,
                m_command,
                m_callback);
}

void
VR_VBT_AgentClimate_Test::TearDown()
{
    delete m_agentClimate;
    m_agentClimate = NULL;
}

TEST_F(VR_VBT_AgentClimate_Test, Initialize)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    EXPECT_TRUE(ret);
    m_agentClimate->UnInitialize();
}

TEST_F(VR_VBT_AgentClimate_Test, ReplyQueryInfo)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<action><op>queryStatus</op></action>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->ReplyQueryInfo(parser));
    m_agentClimate->UnInitialize();
}

TEST_F(VR_VBT_AgentClimate_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }

    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK)));

    VBT_CSTR szAgent = _T("Climate");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result = m_agentClimate->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(VBT_FALSE == bSupported);
    m_agentClimate->UnInitialize();
}

TEST_F(VR_VBT_AgentClimate_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Climate");
    HRESULT result;
    result = m_agentClimate->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentClimate->UnInitialize();
}

TEST_F(VR_VBT_AgentClimate_Test, QueryAgentSupport_True)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }
    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(VBT_TRUE), Return(S_OK)));

    VBT_CSTR szAgent = _T("Climate");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentClimate->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentClimate->UnInitialize();
}

TEST_F(VR_VBT_AgentClimate_Test, OnAgentResultMessage_Fail)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentClimate->OnAgentResultMessage(NULL, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentClimate->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentClimate_Test, OnAgentResultMessage_OK)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Climate"), 8);
    VBT_STR strActionType = VeciAllocString(_T("Back"), 5);
    VBT_STR xmlMsg = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    HRESULT result = m_agentClimate->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentClimate->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentClimate_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentClimate->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentClimate->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentClimate_Test, OnAgentDriverMessage_OK)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Climate"), 8);
    VBT_STR strActionType = VeciAllocString(_T("Back"), 5);
    VBT_STR xmlMsg = VeciAllocString(_T("Back"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(2).WillOnce(Return()).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    HRESULT result = m_agentClimate->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentClimate->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentClimate_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentClimate->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentClimate->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentClimate_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Climate"), 8);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).Times(1).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    bool result = m_agentClimate->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentClimate->UnInitialize();
}

TEST_F(VR_VBT_AgentClimate_Test, ProcessAgentMessage_ActionType_True)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Climate"), 8);
    VBT_STR strActionType = VeciAllocString(_T("AC Off"), 7);
    VBT_STR xmlmsg = VeciAllocString(_T("xml"), 4);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlmsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    bool result = m_agentClimate->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentClimate->UnInitialize();
}

TEST_F(VR_VBT_AgentClimate_Test, ProcessAgentMessage_ListType_True)
{
    if (NULL == m_agentClimate) {
        FAIL();
    }

    bool ret = m_agentClimate->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = VeciAllocString(_T("Climate"), 8);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = VeciAllocString(_T("AC Off"), 7);
    VBT_STR xmlmsg = VeciAllocString(_T("xml"), 4);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlmsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    bool result = m_agentClimate->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentClimate->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentClimate_Test, TurnClimate)
{
    std::string strType = _T("AC");
    std::string strStatus = _T("ON");

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentClimate->TurnClimate(strType, strStatus);
    SUCCEED();
}

TEST_F(VR_VBT_AgentClimate_Test, ACOn)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ACOn(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ACOff)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ACOff(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ShowScreen)
{
    std::string strType = _T("CONCIERGE");
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentClimate->ShowScreen(strType);
    SUCCEED();
}

TEST_F(VR_VBT_AgentClimate_Test, HVACConciergeScreen)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->HVACConciergeScreen(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, HVACFrontScreen)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->HVACFrontScreen(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, HVACFrontSeatScreen)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->HVACFrontSeatScreen(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, HVACRearScreen)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->HVACRearScreen(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, HVACRearSeatScreen)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->HVACRearSeatScreen(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, HVACSeatOperationScreen)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->HVACSeatOperationScreen(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, HVACSteeringScreen)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->HVACSteeringScreen(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ChangeFanSpeed)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ChangeFanSpeed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, DecreaseTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->DecreaseTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, IncreaseTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->IncreaseTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, SendChangeTemperature)
{
    std::string strReply = _T("Temperature");
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentClimate->SendChangeTemperature(strReply);
    SUCCEED();
}

TEST_F(VR_VBT_AgentClimate_Test, ChangeMinTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ChangeMinTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ChangeMaxTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ChangeMaxTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ChangeTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ChangeTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ConciergeModeOn)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ConciergeModeOn(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, TaskComplete)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->TaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ACAlreadyOff)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->ACAlreadyOff(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ACAlreadyOn)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ACAlreadyOn(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, AlreadyAtMaxTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->AlreadyAtMaxTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, AlreadyAtMinTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->AlreadyAtMinTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ChangeTemperatureAlreadyAtRequestedTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ChangeTemperatureAlreadyAtRequestedTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, GoToHVAC)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->GoToHVAC(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, DecTempAlreadyMinTemp)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->DecTempAlreadyMinTemp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, GetTempAlreadyMinTemp)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->GetTempAlreadyMinTemp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, GetTemperature)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_FALSE(m_agentClimate->GetTemperature(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, GetFanSpeed)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->GetFanSpeed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ACOnRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ACOnRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ConciergeModeOnRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ConciergeModeOnRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, QueryStatus_AC)
{
    std::string xml = _T("<type>AUTO_AIRCONDITION</type><status>ON</status>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->QueryStatus(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, QueryStatus_Concierge)
{
    std::string xml = _T("<type>CONCIERGE_MODE</type><status>OFF</status>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->QueryStatus(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, QueryStatus_Else)
{
    std::string xml = _T("<type>CONCIERGE_MODE</type><status>ABORT</status>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentClimate->QueryStatus(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, DecreaseFanSpeedRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->DecreaseFanSpeedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidFanSpeed)
{
    std::string strType = _T("CONCIERGE_MODE");
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    m_agentClimate->GetValidFanSpeed(strType);
    SUCCEED();
}

TEST_F(VR_VBT_AgentClimate_Test, DecreaseTemperatureRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->DecreaseTemperatureRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, FanSpeedValidRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->FanSpeedValidRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, IncreaseFanSpeedRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->IncreaseFanSpeedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidTemperature)
{
    std::string strType = _T("");
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    m_agentClimate->GetValidTemperature(strType);
    SUCCEED();
}

TEST_F(VR_VBT_AgentClimate_Test, IncreaseTemperatureRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->IncreaseTemperatureRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, TemperatureMaxRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->TemperatureMaxRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, TemperatureMinRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->TemperatureMinRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, TemperatureValidRequest)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->TemperatureValidRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckFanSpeed_0)
{
    std::string xml = "<status>0</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->CheckFanSpeed(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckFanSpeed_1)
{
    std::string xml = "<status>1</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->CheckFanSpeed(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckFanSpeed_2)
{
    std::string xml = "<status>2</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->CheckFanSpeed(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckFanSpeed_3)
{
    std::string xml = "<status>3</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentClimate->CheckFanSpeed(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckTemperature_0)
{
    std::string xml = "<status>0</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));

    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));

    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->CheckTemperature(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckTemperature_1)
{
    std::string xml = "<status>1</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));

    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));

    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->CheckTemperature(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckTemperature_2)
{
    std::string xml = "<status>2</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->CheckTemperature(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckTemperature_3)
{
    std::string xml = "<status>3</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->CheckTemperature(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, CheckTemperature_4)
{
    std::string xml = "<status>3</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentClimate->CheckTemperature(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidTemperatureReply_Decrease)
{
    std::string xml = "<type>decrease</type><status>0</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->GetValidTemperatureReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidTemperatureReply_increase)
{
    std::string xml = "<type>increase</type><status>1</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->GetValidTemperatureReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidTemperatureReply_Max)
{
    std::string xml = "<type>max</type><status>2</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->GetValidTemperatureReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidTemperatureReply_Min)
{
    std::string xml = "<type>min</type><status>3</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->GetValidTemperatureReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidTemperatureReply_Else)
{
    std::string xml = "<type>else</type><status>3</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentClimate->GetValidTemperatureReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidFanSpeedReply_0)
{
    std::string xml = "<speed>min</speed><status>0</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->GetValidFanSpeedReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidFanSpeedReply_1)
{
    std::string xml = "<speed>min</speed><status>1</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->GetValidFanSpeedReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidFanSpeedReply_2)
{
    std::string xml = "<speed>min</speed><status>2</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->GetValidFanSpeedReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, GetValidFanSpeedReply_3)
{
    std::string xml = "<speed>min</speed><status>3</status>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentClimate->GetValidFanSpeedReply(parser));
}

TEST_F(VR_VBT_AgentClimate_Test, ClimateBack)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ClimateBack(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ClimateCancel)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ClimateCancel(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ClimateHelp)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentClimate->ClimateHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ClimateStartOver)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ClimateStartOver(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ClimateStartPause)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ClimateStartPause(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, ClimateStartRepeat)
{
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->ClimateStartRepeat(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentClimate_Test, EscalatingError)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentClimate->EscalatingError(&m_pResultMsg));
}

/* EOF */
