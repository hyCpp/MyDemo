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
#include "VR_Log.h"
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
#include "VR_VoiceBoxAgentNavi.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;
/**
* VR_VBT_AgentNavi_Test
*
* The class is just for VR_VoiceBoxAgentNavi test.
*/
class VR_VBT_AgentNavi_Test : public testing::Test
{
public:
    VR_VBT_AgentNavi_Test() : m_agentNavi(NULL)
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

    VR_VoiceBoxAgentNavi* m_agentNavi;

private:
    VR_VBT_AgentNavi_Test(const VR_VBT_AgentNavi_Test& other);
    void operator= (const VR_VBT_AgentNavi_Test& other);
};

void
VR_VBT_AgentNavi_Test::SetUp()
{
    VR_LOGI("In SetUp");
    m_agentNavi = new VR_VoiceBoxAgentNavi(
                m_engine,
                m_command,
                m_callback);
    VR_LOGI("In SetUp");
}

void
VR_VBT_AgentNavi_Test::TearDown()
{
    delete m_agentNavi;
    m_agentNavi = NULL;
}

TEST_F(VR_VBT_AgentNavi_Test, Initialize)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    EXPECT_TRUE(ret);
    m_agentNavi->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentNavi_Test, ReplyQueryInfo)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<action><op>queryUnpassedDestinationList</op></action>";
    VR_VoiceBoxXmlParser parser(xml);
    MockVR_VBT_VECIParameterSet* paraSet = NULL;
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(paraSet), Return(S_OK)));
    EXPECT_TRUE(m_agentNavi->ReplyQueryInfo(parser));
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Navi");
    HRESULT result;
    result = m_agentNavi->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, QueryAgentSupport_True_AddressEntry)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }
    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).Times(1).WillRepeatedly(
                DoAll(SetArgPointee<2>(VBT_TRUE), Return(S_OK)));

    VBT_CSTR szAgent = _T("Address Entry");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentNavi->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, QueryAgentSupport_True_Destination)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
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

    VBT_CSTR szAgent = _T("Destination");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentNavi->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, QueryAgentSupport_True_POI)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
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

    VBT_CSTR szAgent = _T("POI");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentNavi->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, QueryAgentSupport_True_Navigation)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
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

    VBT_CSTR szAgent = _T("Navigation");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result;
    result = m_agentNavi->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
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
    result = m_agentNavi->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_FALSE(bSupported);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, OnAgentResultMessage_NULL)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentNavi->OnAgentResultMessage(NULL, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, OnAgentResultMessage_Fail)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillRepeatedly(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));

    HRESULT result = m_agentNavi->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, OnAgentResultMessage_S_OK)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = VeciAllocString(_T("Call POI"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillRepeatedly(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK))).WillOnce(Return(S_OK));

    HRESULT result = m_agentNavi->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, OnAgentDriverMessage_NULL)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentNavi->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillRepeatedly(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));

    HRESULT result = m_agentNavi->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(E_FAIL, result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, OnAgentDriverMessage_S_OK)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = VeciAllocString(_T("Call POI"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillRepeatedly(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK))).WillOnce(Return(S_OK));

    HRESULT result = m_agentNavi->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentNavi->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }
    VBT_STR strActionClass = VeciAllocString(_T("Navi"), 5);
    VBT_STR strActionType = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK))).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));

    bool result = m_agentNavi->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, ProcessAgentMessage_ActionType_True)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strMsgClassName = VeciAllocString(_T("Driver"), 7);
    VBT_STR strActionClassName = VeciAllocString(_T("Navi"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Add Waypoint Request"), 25);
    VBT_STR strActionParameterValue = VeciAllocString(_T("Call POI"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionParameterValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    bool result = m_agentNavi->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, ProcessAgentMessage_ListType_True)
{
    if (NULL == m_agentNavi) {
        FAIL();
    }

    bool ret = m_agentNavi->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_STR strActionClass = NULL;
    VBT_STR strActionType = NULL;
    VBT_STR strListType = VeciAllocString(_T("Call POI"), 9);
    VBT_STR strActionParameterValue = NULL;
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillRepeatedly(
                DoAll(SetArgPointee<0>(strActionClass), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionParameterValue), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));

    bool result = m_agentNavi->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentNavi->UnInitialize();
}

TEST_F(VR_VBT_AgentNavi_Test, CallPOI)
{
    bool result = m_agentNavi->CallPOI(&m_pResultMsg);
    EXPECT_TRUE(result);
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmPOI)
{
    VBT_STR strWaypointAction = VeciAllocString(_T("POI"), 4);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strWaypointAction), Return(S_OK)));
    bool result = m_agentNavi->ConfirmPOI(&m_pResultMsg);
    EXPECT_TRUE(result);
}

TEST_F(VR_VBT_AgentNavi_Test, FindPOINoRouteActive)
{
    EXPECT_TRUE(m_agentNavi->FindPOINoRouteActive(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetAssignment)
{
    EXPECT_TRUE(m_agentNavi->GetAssignment(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsToPOI)
{
    EXPECT_TRUE(m_agentNavi->GetDirectionsToPOI(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetPOICategory)
{
    EXPECT_TRUE(m_agentNavi->GetPOICategory(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, HideAllIcons)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->HideAllIcons(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, InternetNotAvailable)
{
    EXPECT_TRUE(m_agentNavi->InternetNotAvailable(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ListPOIs)
{
    EXPECT_TRUE(m_agentNavi->ListPOIs(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, MaxIconsShown)
{
    EXPECT_TRUE(m_agentNavi->MaxIconsShown(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, MaxWaypoints)
{
    EXPECT_TRUE(m_agentNavi->MaxWaypoints(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, NoIconsShown)
{
    EXPECT_TRUE(m_agentNavi->NoIconsShown(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, NoPOIsAvailable)
{
    EXPECT_TRUE(m_agentNavi->NoPOIsAvailable(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, NoPOIsReturned)
{
    EXPECT_TRUE(m_agentNavi->NoPOIsReturned(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, NoSearchApp)
{
    EXPECT_TRUE(m_agentNavi->NoSearchApp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, OffboardPOISearch)
{
    EXPECT_TRUE(m_agentNavi->OffboardPOISearch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PhoneNotConnected)
{
    EXPECT_TRUE(m_agentNavi->PhoneNotConnected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PhoneNumberNotAvailable)
{
    EXPECT_TRUE(m_agentNavi->PhoneNumberNotAvailable(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, POIIconsNotAvailable)
{
    EXPECT_TRUE(m_agentNavi->POIIconsNotAvailable(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, RejectDestination)
{
    EXPECT_TRUE(m_agentNavi->RejectDestination(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, RejectPOINavigation)
{
    EXPECT_TRUE(m_agentNavi->RejectPOINavigation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SearchTheWeb)
{
    EXPECT_TRUE(m_agentNavi->SearchTheWeb(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ShowOffboardPOISearchResults)
{
    EXPECT_TRUE(m_agentNavi->ShowOffboardPOISearchResults(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ShowPOIIcons)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->ShowPOIIcons(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, POIAddWaypointRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->POIAddWaypointRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, IconsShownRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->IconsShownRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PhoneNumAvailableRequest)
{
    EXPECT_TRUE(m_agentNavi->PhoneNumAvailableRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, POIRequest_Nearby)
{
    VBT_STR strPhrase = VeciAllocString(_T("Nearby"), 7);
    VBT_STR strPOIID = VeciAllocString(_T("Poiid"), 6);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPOIID), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->POIRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, POIRequest_AlongRoute)
{
    VBT_STR strPhrase = VeciAllocString(_T("Along Route"), 12);
    VBT_STR strPOIID = VeciAllocString(_T("Poiid"), 6);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPOIID), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->POIRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, POIRequest_Destination)
{
    VBT_STR strPhrase = VeciAllocString(_T("Destination"), 15);
    VBT_STR strPOIID = VeciAllocString(_T("Poiid"), 6);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strPOIID), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->POIRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, POIRequest_City)
{
    VBT_STR strPhrase = VeciAllocString(_T("City"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_TRUE(m_agentNavi->POIRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, POIRequest_False)
{
    VBT_STR strPhrase = VeciAllocString(_T("false"), 6);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strPhrase), Return(S_OK)));
    EXPECT_FALSE(m_agentNavi->POIRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddPresetAsWaypoint)
{
    EXPECT_TRUE(m_agentNavi->AddPresetAsWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddPrevDestinationAsWaypoint)
{
    EXPECT_TRUE(m_agentNavi->AddPrevDestinationAsWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, CannotAddWaypoint)
{
    EXPECT_TRUE(m_agentNavi->CannotAddWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmDeleteWaypoint)
{
    EXPECT_TRUE(m_agentNavi->ConfirmDeleteWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmPreset)
{
    EXPECT_TRUE(m_agentNavi->ConfirmPreset(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmPresetReject)
{
    EXPECT_TRUE(m_agentNavi->ConfirmPresetReject(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmWaypointForDeletion)
{
    EXPECT_TRUE(m_agentNavi->ConfirmWaypointForDeletion(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, DeleteDestination)
{
    EXPECT_TRUE(m_agentNavi->DeleteDestination(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, DeleteWaypoint)
{
    EXPECT_TRUE(m_agentNavi->DeleteWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, DestAssCatch)
{
    EXPECT_TRUE(m_agentNavi->DestAssCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, DestAssConfirmCall)
{
    EXPECT_TRUE(m_agentNavi->DestAssConfirmCall(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, DestAssConfirmCallReject)
{
    EXPECT_TRUE(m_agentNavi->DestAssConfirmCallReject(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, DestAssNotAvailable)
{
    EXPECT_TRUE(m_agentNavi->DestAssNotAvailable(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, HomePresetNotDefined)
{
    EXPECT_TRUE(m_agentNavi->HomePresetNotDefined(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, MapDataNotInArea)
{
    EXPECT_TRUE(m_agentNavi->MapDataNotInArea(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, NavigationHome)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->NavigationHome(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PresetNotValid)
{
    EXPECT_TRUE(m_agentNavi->PresetNotValid(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PrevDestNotValid)
{
    EXPECT_TRUE(m_agentNavi->PrevDestNotValid(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PrevDestNotAvailable)
{
    EXPECT_TRUE(m_agentNavi->PrevDestNotAvailable(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SelectPresetAssignment)
{
    EXPECT_TRUE(m_agentNavi->SelectPresetAssignment(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SelectPrevDest)
{
    EXPECT_TRUE(m_agentNavi->SelectPrevDest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SelectPrevDestAssignment)
{
    EXPECT_TRUE(m_agentNavi->SelectPrevDestAssignment(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SelectWaypointForDeletion)
{
    EXPECT_TRUE(m_agentNavi->SelectWaypointForDeletion(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SetPresetAsDestination)
{
    EXPECT_TRUE(m_agentNavi->SetPresetAsDestination(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SetPrevDestAsDestination)
{
    EXPECT_TRUE(m_agentNavi->SetPrevDestAsDestination(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, WaypointDeletionCanceled)
{
    EXPECT_TRUE(m_agentNavi->WaypointDeletionCanceled(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, WaypointsNotAvailable)
{
    EXPECT_TRUE(m_agentNavi->WaypointsNotAvailable(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, IDMappingRequest)
{
    VBT_ULONG uListSize = 1;
    VBT_ULONG uValueSize = 2;
    VBT_STR strOut = VeciAllocString(_T("Map"), 4);
    VBT_STR strAttrName = VeciAllocString(_T("Type"), 5);
    VBT_STR strAttrValue = VeciAllocString(_T("State"), 6);
    VBT_STR strID = VeciAllocString(_T("ID"), 3);
    VBT_STR strIDValue = VeciAllocString(_T("10010"), 6);
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uListSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strOut), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItem(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, GetSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uValueSize), Return(S_OK)));
    EXPECT_CALL(m_paraSet, GetParameter(_, _, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<2>(strAttrName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strAttrValue), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strIDValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->IDMappingRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeValue_Hex)
{
    std::string strValue = _T("10010");
    m_agentNavi->ChangeValue(strValue, true);
    SUCCEED();
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeValue_Default)
{
    std::string strValue = _T("10010");
    m_agentNavi->ChangeValue(strValue, false);
    SUCCEED();
}

TEST_F(VR_VBT_AgentNavi_Test, CanAddWaypointRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->CanAddWaypointRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, HomePresetDefinedRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->HomePresetDefinedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PresetValidRequest)
{
    EXPECT_TRUE(m_agentNavi->PresetValidRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PresetsAvailableRequest)
{
    EXPECT_TRUE(m_agentNavi->PresetsAvailableRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PrevDestValidRequest)
{
    EXPECT_TRUE(m_agentNavi->PrevDestValidRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PrevDestsRequest)
{
    EXPECT_TRUE(m_agentNavi->PrevDestsRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, PrevStartPointDefinedRequest)
{
    EXPECT_TRUE(m_agentNavi->PrevStartPointDefinedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, WaypointRequest)
{
    EXPECT_TRUE(m_agentNavi->WaypointRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddAddressToActiveRoute)
{
    EXPECT_TRUE(m_agentNavi->AddAddressToActiveRoute(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddWaypoint)
{
    EXPECT_TRUE(m_agentNavi->AddWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddWaypointCarParked)
{
    EXPECT_TRUE(m_agentNavi->AddWaypointCarParked(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddWaypointRequest)
{
    EXPECT_TRUE(m_agentNavi->AddWaypointRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ActiveRounteConfirmation)
{
    EXPECT_TRUE(m_agentNavi->ActiveRounteConfirmation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddRouteConfirmArrivalPoint)
{
    EXPECT_TRUE(m_agentNavi->AddRouteConfirmArrivalPoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddRouteMaxWaypoint)
{
    EXPECT_TRUE(m_agentNavi->AddRouteMaxWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddRouteRequest)
{
    EXPECT_TRUE(m_agentNavi->AddRouteRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddressConfirmationBridge)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->AddressConfirmationBridge(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddressConfirmationFerry)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->AddressConfirmationFerry(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddressConfirmationFreeway)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->AddressConfirmationFreeway(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddressConfirmationTollRoad)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->AddressConfirmationTollRoad(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddressConfirmationTunnel)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->AddressConfirmationTunnel(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddressConfirmed)
{
    EXPECT_TRUE(m_agentNavi->AddressConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, AddressRejected)
{
    EXPECT_TRUE(m_agentNavi->AddressRejected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeAddress)
{
    EXPECT_TRUE(m_agentNavi->ChangeAddress(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeAddressCity)
{
    EXPECT_TRUE(m_agentNavi->ChangeAddressCity(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeAddressHouseNumber)
{
    EXPECT_TRUE(m_agentNavi->ChangeAddressHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeAddressState)
{
    EXPECT_TRUE(m_agentNavi->ChangeAddressState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeAddressStreet)
{
    EXPECT_TRUE(m_agentNavi->ChangeAddressStreet(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeCityOrState)
{
    EXPECT_TRUE(m_agentNavi->ChangeCityOrState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeCityOrStateCity)
{
    EXPECT_TRUE(m_agentNavi->ChangeCityOrStateCity(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeCityOrStateHouseNumber)
{
    EXPECT_TRUE(m_agentNavi->ChangeCityOrStateHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeCityOrStateState)
{
    EXPECT_TRUE(m_agentNavi->ChangeCityOrStateState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeCityOrStateStreet)
{
    EXPECT_TRUE(m_agentNavi->ChangeCityOrStateStreet(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeDefaultStateCatch)
{
    EXPECT_TRUE(m_agentNavi->ChangeDefaultStateCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeStateInNBestList)
{
    EXPECT_TRUE(m_agentNavi->ChangeStateInNBestList(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeStreetCityOrState)
{
    EXPECT_TRUE(m_agentNavi->ChangeStreetCityOrState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeStreetCityOrStateCity)
{
    EXPECT_TRUE(m_agentNavi->ChangeStreetCityOrStateCity(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeStreetCityOrStateHouseNumber)
{
    EXPECT_TRUE(m_agentNavi->ChangeStreetCityOrStateHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeStreetCityOrStateState)
{
    EXPECT_TRUE(m_agentNavi->ChangeStreetCityOrStateState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ChangeStreetCityOrStateStreet)
{
    EXPECT_TRUE(m_agentNavi->ChangeStreetCityOrStateStreet(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, CityWithNoStreetsRejected)
{
    EXPECT_TRUE(m_agentNavi->CityWithNoStreetsRejected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAddress)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAddress(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAddressCity)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAddressCity(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAddressHouseNumber)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAddressHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAddressState)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAddressState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAddressConfirmSuccess)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAddressConfirmSuccess(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAddressWithoutHouseNumber)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->ConfirmAddressWithoutHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmArrivalPointAddWaypoint)
{
    EXPECT_TRUE(m_agentNavi->ConfirmArrivalPointAddWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmArrivalPointRejectSuccess)
{
    EXPECT_TRUE(m_agentNavi->ConfirmArrivalPointRejectSuccess(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmArrivalPointSuccess)
{
    EXPECT_TRUE(m_agentNavi->ConfirmArrivalPointSuccess(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ReplaceRouteConfirmArrivalPoint)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->ReplaceRouteConfirmArrivalPoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAddressStreet)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAddressStreet(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAnywhere)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->ConfirmAnywhere(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAnywhereCity)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAnywhereCity(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAnywhereHouseNumber)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAnywhereHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAnywhereState)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAnywhereState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmAnywhereStreet)
{
    EXPECT_TRUE(m_agentNavi->ConfirmAnywhereStreet(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmCityWithNoStreets)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->ConfirmCityWithNoStreets(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmCityWithNoStreetsCity)
{
    EXPECT_TRUE(m_agentNavi->ConfirmCityWithNoStreetsCity(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ConfirmCityWithNoStreetsState)
{
    EXPECT_TRUE(m_agentNavi->ConfirmCityWithNoStreetsState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetCity)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->GetCity(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDefaultState)
{
    EXPECT_TRUE(m_agentNavi->GetDefaultState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsToAddress)
{
    EXPECT_TRUE(m_agentNavi->GetDirectionsToAddress(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsToCountrywide)
{
    EXPECT_TRUE(m_agentNavi->GetDirectionsToCountrywide(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsToCountrywideNoDefaultState)
{
    EXPECT_TRUE(m_agentNavi->GetDirectionsToCountrywideNoDefaultState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsToMultiStepStartingWithState)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->GetDirectionsToMultiStepStartingWithState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsToUnsupported)
{
    EXPECT_TRUE(m_agentNavi->GetDirectionsToUnsupported(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsViaCountrywide)
{
    EXPECT_TRUE(m_agentNavi->GetDirectionsViaCountrywide(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsViaMultiStepStartingWithInfo)
{
    EXPECT_TRUE(m_agentNavi->GetDirectionsViaMultiStepStartingWithInfo(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetDirectionsViaUnsupported)
{
    EXPECT_TRUE(m_agentNavi->GetDirectionsViaUnsupported(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetHouseNumber)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->GetHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetStreetBeforeHouseNumber)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->GetStreetBeforeHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, InvalidHouseNumber)
{
    EXPECT_TRUE(m_agentNavi->InvalidHouseNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, MultiStepStartingWithState)
{
    EXPECT_TRUE(m_agentNavi->MultiStepStartingWithState(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, NativeAddressConfirmCountrywide)
{
    EXPECT_TRUE(m_agentNavi->NativeAddressConfirmCountrywide(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, NativeAddressRejectedStateReplace)
{
    EXPECT_TRUE(m_agentNavi->NativeAddressRejectedStateReplace(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ReplaceWaypoint)
{
    EXPECT_TRUE(m_agentNavi->ReplaceWaypoint(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ReplaceWaypointRequest)
{
    EXPECT_TRUE(m_agentNavi->ReplaceWaypointRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SaveAddress)
{
    EXPECT_TRUE(m_agentNavi->SaveAddress(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SaveAddressIsFull)
{
    EXPECT_TRUE(m_agentNavi->SaveAddressIsFull(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SelectUserIntent)
{
    EXPECT_TRUE(m_agentNavi->SelectUserIntent(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ShowOnMap)
{
    EXPECT_TRUE(m_agentNavi->ShowOnMap(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, WaypointConfirmed)
{
    EXPECT_TRUE(m_agentNavi->WaypointConfirmed(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, CarParkedRequest)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->CarParkedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, SaveAddrRequest)
{
    EXPECT_TRUE(m_agentNavi->SaveAddrRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, ValidateHouseNum)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->ValidateHouseNum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, VerifyCityContainsStreets)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->VerifyCityContainsStreets(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, NullOperation)
{
    VBT_STR strMsg = VeciAllocString(_T("null"), 5);
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(strMsg), Return(S_OK)));
    EXPECT_FALSE(m_agentNavi->NullOperation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, HomePresetDefinedReply)
{
    std::string xml = _T("<exist>true</exist>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->HomePresetDefinedReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, AddWaypointReply_False)
{
    std::string xml = _T("<count>2</count>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_FALSE(m_agentNavi->AddWaypointReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, AddWaypointReply_Add_Max_Waypoints)
{
    VBT_STR strMsgClassName = VeciAllocString(_T("Driver"), 7);
    VBT_STR strActionClassName = VeciAllocString(_T("Navi"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Add Waypoint Request"), 25);
    VBT_STR strActionParameterValue = VeciAllocString(_T("Call POI"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionParameterValue), Return(S_OK)));
    m_agentNavi->ProcessAgentMessage(&m_pResultMsg);
    std::string xml = _T("<count>5</count>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->AddWaypointReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, AddWaypointReply_Add_Success)
{
    VBT_STR strMsgClassName = VeciAllocString(_T("Driver"), 7);
    VBT_STR strActionClassName = VeciAllocString(_T("Navi"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Add Waypoint Request"), 25);
    VBT_STR strActionParameterValue = VeciAllocString(_T("Call POI"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionParameterValue), Return(S_OK)));
    m_agentNavi->ProcessAgentMessage(&m_pResultMsg);
    std::string xml = _T("<count>1</count>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->AddWaypointReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, AddWaypointReply_Can_Add_True)
{
    VBT_STR strMsgClassName = VeciAllocString(_T("Driver"), 7);
    VBT_STR strActionClassName = VeciAllocString(_T("Navi"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Can Add Waypoint Request"), 30);
    VBT_STR strActionParameterValue = VeciAllocString(_T("Call POI"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionParameterValue), Return(S_OK)));
    m_agentNavi->ProcessAgentMessage(&m_pResultMsg);
    std::string xml = _T("<count>5</count>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->AddWaypointReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, AddWaypointReply_Can_Add_False)
{
    VBT_STR strMsgClassName = VeciAllocString(_T("Driver"), 7);
    VBT_STR strActionClassName = VeciAllocString(_T("Navi"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Can Add Waypoint Request"), 30);
    VBT_STR strActionParameterValue = VeciAllocString(_T("Call POI"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionClass(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strMsgClassName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionClassName), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(strActionParameterValue), Return(S_OK)));
    m_agentNavi->ProcessAgentMessage(&m_pResultMsg);
    std::string xml = _T("<count>5</count>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->AddWaypointReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, IconsShownReply_False)
{
    std::string xml = _T("<number>-1</number>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_FALSE(m_agentNavi->IconsShownReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, IconsShownReply_No_Icons_Shown)
{
    std::string xml = _T("<number>0</number>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->IconsShownReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, IconsShownReply_Icons_Shown)
{
    std::string xml = _T("<number>4</number>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->IconsShownReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, IconsShownReply_Max_Icons_Shown)
{
    std::string xml = _T("<number>6</number>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->IconsShownReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, POICollectionNearby)
{
    std::string xml = _T("<count>-1</count>"
                        "<id>1</id>"
                        "<distance>100</distance>"
                        "<unit>km</unit>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->POICollectionNearby(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, POICollectionAlongRoute)
{
    std::string xml = _T("<count>-1</count>"
                        "<id>1</id>"
                        "<distance>100</distance>"
                        "<unit>km</unit>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->POICollectionAlongRoute(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, POICollectionNearDestination)
{
    std::string xml = _T("<count>-1</count>"
                        "<id>1</id>"
                        "<distance>100</distance>"
                        "<unit>km</unit>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->POICollectionNearDestination(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, SavePOIInfo)
{
    std::string xml = _T("<id>1</id>"
                        "<distance>100</distance>"
                        "<unit>km</unit>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentNavi->SavePOIInfo(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, CarParked_Fail)
{
    std::string xml = _T("<parser>error</parser>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));

    EXPECT_FALSE(m_agentNavi->CarParked(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, CarParked_False)
{
    std::string xml = _T("<parser>false</parser>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->CarParked(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, CarParked_True)
{
    std::string xml = _T("<parser>true</parser>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->CarParked(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, HouseNumValidation_HouseNumberFound_False)
{
    std::string xml = _T("<houseNumberFound>false</houseNumberFound>"
                        "<housenumber>100</housenumber>"
                        "<houseNumberExist>true</houseNumberExist>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->HouseNumValidation(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, HouseNumValidation_HouseNumber_Valid)
{
    std::string xml = _T("<houseNumberFound>true</houseNumberFound>"
                        "<housenumber>A-100</housenumber>"
                        "<houseNumberExist>true</houseNumberExist>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->HouseNumValidation(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, HouseNumValidation_HouseNumber_Invalid)
{
    std::string xml = _T("<houseNumberFound>true</houseNumberFound>"
                        "<housenumber>A?-100</housenumber>"
                        "<houseNumberExist>false</houseNumberExist>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->HouseNumValidation(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, HouseNumValidation_Fail)
{
    std::string xml = _T("<houseNumberFound>error</houseNumberFound>"
                        "<housenumber>A?-100</housenumber>"
                        "<houseNumberExist>false</houseNumberExist>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));

    EXPECT_FALSE(m_agentNavi->HouseNumValidation(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, CityVerification_StreetExist_False)
{
    std::string xml = _T("<streetExist>false</streetExist>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->CityVerification(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, CityVerification_StreetExist_True)
{
    std::string xml = _T("<streetExist>true</streetExist>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->CityVerification(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, CityVerification_StreetExist_Fail)
{
    std::string xml = _T("<streetExist>error</streetExist>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));

    EXPECT_FALSE(m_agentNavi->CityVerification(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, GetHouseNumType_NumericOnly)
{
    std::string strHouseNumber = _T("4-111");
    std::string strHouseNumType;

    EXPECT_TRUE(m_agentNavi->GetHouseNumType(strHouseNumber, strHouseNumType));
    EXPECT_STREQ("NumericOnly", strHouseNumType.c_str());
}

TEST_F(VR_VBT_AgentNavi_Test, GetHouseNumType_AlphaNumeric)
{
    std::string strHouseNumber = _T("A-111");
    std::string strHouseNumType;

    EXPECT_TRUE(m_agentNavi->GetHouseNumType(strHouseNumber, strHouseNumType));
    EXPECT_STREQ("AlphaNumeric", strHouseNumType.c_str());
}

TEST_F(VR_VBT_AgentNavi_Test, POIReply_Available_False)
{
    std::string xml = _T("<count>-1</count>"
                        "<id>1</id>"
                        "<distance>100</distance>"
                        "<unit>km</unit>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentNavi->POIReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, POIReply_Available_True)
{
    //     std::string xml = _T("<count>1</count>"
    //                         "<id>1</id>"
    //                         "<distance>100</distance>"
    //                         "<unit>km</unit>");
    //     VR_VoiceBoxXmlParser parser(xml);
    //     EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
    //                 DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    //     EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    //     EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    //     EXPECT_TRUE(m_agentNavi->POIReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, IDMappingReply)
{
    std::string xml = _T("<stateId>156</stateId>"
                        "<country>China</country>"
                        "<state>HuaNan</state>"
                        "<statePron>''</statePron>"
                        "<cityId>5510</cityId>"
                        "<city>''</city>"
                        "<cityPron>Shanghai</cityPron>"
                        "<streetExist>true</streetExist>"
                        "<streetId>108</streetId>"
                        "<street>YiShan Road</street>"
                        "<streetPron>12</streetPron>"
                        "<houseNumberExist>true</houseNumberExist>"
                        "<houseNumber>1001</houseNumber>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).Times(3).WillRepeatedly(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(11).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->IDMappingReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, IDMappingReply_bSuccess_False)
{
    std::string xml = _T("<stateId>''</stateId>");
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_agentNavi->IDMappingReply(parser));
}

TEST_F(VR_VBT_AgentNavi_Test, SimplyInfoQueryMessage)
{
    std::string strOp = _T("SavePOIInfo");
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->SimplyInfoQueryMessage(strOp));
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenSpeakEntriesAddress)
{
    EXPECT_TRUE(m_agentNavi->JumpScreenSpeakEntriesAddress());
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenSpeakState)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->JumpScreenSpeakState());
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenSpeakCity)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->JumpScreenSpeakCity());
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenSpeakStreet)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->JumpScreenSpeakStreet());
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenSpeakHouseNum)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->JumpScreenSpeakHouseNum());
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenAddressConfirm)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->JumpScreenAddressConfirm());
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenSetRoute)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->JumpScreenSetRoute());
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenSetRouteConfirm)
{
    std::string strGuideType = _T("Navi");
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentNavi->JumpScreenSetRouteConfirm(strGuideType));
}

TEST_F(VR_VBT_AgentNavi_Test, JumpScreenPOIConfirm)
{
    EXPECT_TRUE(m_agentNavi->JumpScreenPOIConfirm(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentNavi_Test, GetAddress)
{
    std::string strKey = _T("city");
    EXPECT_STREQ("", m_agentNavi->GetAddress(strKey).c_str());
}
*/
/* EOF */
