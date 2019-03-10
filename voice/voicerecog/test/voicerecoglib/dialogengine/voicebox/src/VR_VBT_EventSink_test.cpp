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
#include <string>
#include "gtest/gtest.h"

#include "VR_VBT_VECIEngineClient_mock.h"
#include "VR_VBT_VECIEngineCommand_mock.h"
#include "VR_VBT_VECIString_mock.h"
#include "VR_VBT_EngineCallback_mock.h"
#include "VR_VBT_EventSink_mock.h"
#include "VR_VBT_VECIParameterSet_mock.h"
#include "VR_VBT_VECIStringSet_mock.h"
#include "VR_VBT_VECITransaction_mock.h"
#include "VR_VBT_VECIParsedMessage_mock.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_EventSink_Test
*
* The class is just for VR_VoiceBoxEventSink test.
*/
class VR_VBT_EventSink_Test : public testing::Test
{
public:
    VR_VBT_EventSink_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

private:
    MockVR_VBT_VECIEngineClient m_engine;
    MockVR_VBT_VECIEngineCommand m_command;
    MockVR_VBT_EngineCallback m_callback;
    VR_VoiceBoxEventSink* m_eventSink;
};

void
VR_VBT_EventSink_Test::SetUp()
{
    m_eventSink = new MockVR_VBT_EventSink(m_engine, m_command, m_callback);
}

void
VR_VBT_EventSink_Test::TearDown()
{
    delete m_eventSink;
    m_eventSink = NULL;
}

TEST_F(VR_VBT_EventSink_Test, Initialize)
{
    EXPECT_TRUE(m_eventSink->Initialize());
}

TEST_F(VR_VBT_EventSink_Test, UnInitialize)
{
    m_eventSink->UnInitialize();
    SUCCEED();
}

TEST_F(VR_VBT_EventSink_Test, QueryInterface_IID_IUnknown)
{
    IUnknown* p;
    REFIID riid = IID_IUnknown;
    EXPECT_EQ(S_OK, m_eventSink->QueryInterface(riid, (void**)&p));
}

TEST_F(VR_VBT_EventSink_Test, QueryInterface_IID_IVECIEventSink)
{
    IUnknown* p;
    REFIID riid = IID_IVECIEventSink;
    EXPECT_EQ(S_OK, m_eventSink->QueryInterface(riid, (void**)&p));
}

TEST_F(VR_VBT_EventSink_Test, QueryInterface_E_NOINTERFACE)
{
    IUnknown* p;
    REFIID riid = IID_IVECIWorkItemHandler;
    EXPECT_EQ(E_NOINTERFACE, m_eventSink->QueryInterface(riid, (void**)&p));
}

TEST_F(VR_VBT_EventSink_Test, QueryInterface_E_POINTER)
{
    IUnknown* p = NULL;
    REFIID riid = IID_IVECIEventSink;
    EXPECT_EQ(E_POINTER, m_eventSink->QueryInterface(riid, (void**)NULL));
}

TEST_F(VR_VBT_EventSink_Test, Realase_Fail)
{
    EXPECT_EQ(-1, m_eventSink->Release());
}

TEST_F(VR_VBT_EventSink_Test, OnProcessStatus_S_OK)
{
    EXPECT_EQ(S_OK, m_eventSink->OnProcessStatus("status"));
    EXPECT_EQ(E_FAIL, m_eventSink->OnProcessStatus(NULL));
}

TEST_F(VR_VBT_EventSink_Test, OnSystemNotification)
{
    MockVR_VBT_VECIParameterSet pParameterSet;
    EXPECT_EQ(S_OK, m_eventSink->OnSystemNotification(1, &pParameterSet));
    EXPECT_EQ(E_FAIL, m_eventSink->OnSystemNotification(1, NULL));
}

TEST_F(VR_VBT_EventSink_Test, OnSpeak)
{
    MockVR_VBT_VECITransaction pTrans;
    MockVR_VBT_VECIStringSet pSpeakList;
    EXPECT_EQ(S_OK, m_eventSink->OnSpeak(&pTrans, &pSpeakList));
    EXPECT_EQ(E_FAIL, m_eventSink->OnSpeak(NULL, NULL));
}

TEST_F(VR_VBT_EventSink_Test, OnCommandComplete)
{
    MockVR_VBT_VECITransaction pTrans;
    EXPECT_EQ(S_OK, m_eventSink->OnCommandComplete(&pTrans));
    EXPECT_EQ(E_FAIL, m_eventSink->OnCommandComplete(NULL));
}

TEST_F(VR_VBT_EventSink_Test, OnBeepSync)
{
    EXPECT_EQ(S_OK, m_eventSink->OnBeepSync());
}

TEST_F(VR_VBT_EventSink_Test, OnExtendedEvent)
{
    IUnknown* p = static_cast<IUnknown*>(m_eventSink);
    EXPECT_EQ(S_OK, m_eventSink->OnExtendedEvent(ExEventQAData, p));
    EXPECT_EQ(E_FAIL, m_eventSink->OnExtendedEvent(ExEventQAData, NULL));
}

TEST_F(VR_VBT_EventSink_Test, OnClientMessage)
{
    MockVR_VBT_VECIParsedMessage pClientMsg;
    EXPECT_EQ(S_OK, m_eventSink->OnClientMessage(&pClientMsg));
    EXPECT_EQ(E_FAIL, m_eventSink->OnClientMessage(NULL));
}

TEST_F(VR_VBT_EventSink_Test, OnKeywordActivation)
{
    EXPECT_EQ(S_OK, m_eventSink->OnKeywordActivation());
}

TEST_F(VR_VBT_EventSink_Test, OnAudioLevelUpdate)
{
    EXPECT_EQ(S_OK, m_eventSink->OnAudioLevelUpdate(1));
}

TEST_F(VR_VBT_EventSink_Test, ProcessMessage)
{
    std::string msg = _T("Back");
    EXPECT_TRUE(m_eventSink->ProcessMessage(msg));
}

TEST_F(VR_VBT_EventSink_Test, GetActionParaByName)
{
    EXPECT_EQ("", m_eventSink->GetActionParaByName(NULL, ""));

    MockVR_VBT_VECIParsedMessage pcMsg;
    VBT_STR strActionParaName = _T("Agent");
    VBT_STR strValue = VeciAllocString(_T("Apps"), 5);
    EXPECT_CALL(pcMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strValue), Return(S_OK)));
    EXPECT_STREQ("Apps", m_eventSink->GetActionParaByName(&pcMsg, strActionParaName).c_str());
}

TEST_F(VR_VBT_EventSink_Test, GetListParaByName)
{
    std::string ListParaName = "";
    VBT_STR strActionParaName = _T("Agent");
    EXPECT_EQ("", m_eventSink->GetListParaByName(NULL, 1, ListParaName));
    MockVR_VBT_VECIParsedMessage pcMsg;

    VBT_STR strValue = VeciAllocString(_T("Apps"), 5);
    EXPECT_CALL(pcMsg, GetListItemParameterValue(_, _, _)).WillOnce(
                DoAll(SetArgPointee<2>(strValue), Return(S_OK)));
    EXPECT_STREQ("Apps", m_eventSink->GetListParaByName(&pcMsg, 1, strActionParaName).c_str());
}

TEST_F(VR_VBT_EventSink_Test, GetActionParameter)
{
    MockVR_VBT_VECIParsedMessage pcMsg;
    VoiceVector<VR_VoiceBoxEventSink::NodeInfo>::type vecActionParameter;
    EXPECT_FALSE(m_eventSink->GetActionParameter(NULL, vecActionParameter));

    MockVR_VBT_VECIParameterSet paraSet;
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&paraSet), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetActionParameters(_)).WillOnce(
                DoAll(SetArgPointee<0>(&paraSet), Return(S_OK)));
    VBT_LONG uSize = 1;
    EXPECT_CALL(paraSet, GetSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    VBT_STR strName = VeciAllocString(_T("Name"), 5);
    VBT_STR strValue = VeciAllocString(_T("Value"), 6);
    EXPECT_CALL(paraSet, GetParameter(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(strName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strValue), Return(S_OK)));
    EXPECT_TRUE(m_eventSink->GetActionParameter(&pcMsg, vecActionParameter));
}

TEST_F(VR_VBT_EventSink_Test, GetActionParameter_Value_NULL)
{
    MockVR_VBT_VECIParsedMessage pcMsg;
    VoiceVector<VR_VoiceBoxEventSink::NodeInfo>::type vecActionParameter;
    EXPECT_FALSE(m_eventSink->GetActionParameter(NULL, vecActionParameter));

    MockVR_VBT_VECIParameterSet paraSet;
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&paraSet), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetActionParameters(_)).WillOnce(
                DoAll(SetArgPointee<0>(&paraSet), Return(S_OK)));
    VBT_LONG uSize = 1;
    EXPECT_CALL(paraSet, GetSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    VBT_STR strName = VeciAllocString(_T("Name"), 5);
    VBT_STR strValue = NULL;
    EXPECT_CALL(paraSet, GetParameter(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(strName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strValue), Return(S_OK)));
    EXPECT_TRUE(m_eventSink->GetActionParameter(&pcMsg, vecActionParameter));
}

TEST_F(VR_VBT_EventSink_Test, GetListParameter)
{
    MockVR_VBT_VECIParsedMessage pcMsg;
    VR_VoiceBoxEventSink::ListNodeInfo listParameter;
    EXPECT_FALSE(m_eventSink->GetListParameter(NULL, listParameter));

    VBT_LONG uSize = 1;
    EXPECT_CALL(pcMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    VBT_STR strListType = VeciAllocString(_T("ListType"), 9);
    EXPECT_CALL(pcMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    MockVR_VBT_VECIParameterSet paraSet;
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&paraSet), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetListItem(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(&paraSet), Return(S_OK)));
    VBT_LONG uValueSize = 1;
    EXPECT_CALL(paraSet, GetSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uValueSize), Return(S_OK)));
    VBT_STR strName = VeciAllocString(_T("Name"), 5);
    VBT_STR strValue = VeciAllocString(_T("Value"), 6);
    EXPECT_CALL(paraSet, GetParameter(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(strName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strValue), Return(S_OK)));
    EXPECT_TRUE(m_eventSink->GetListParameter(&pcMsg, listParameter));
}

TEST_F(VR_VBT_EventSink_Test, ReStartRecoSession)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_eventSink->ReStartRecoSession());
}

TEST_F(VR_VBT_EventSink_Test, GetStartIndex)
{
    MockVR_VBT_VECIParsedMessage pcMsg;
    EXPECT_STREQ(_T("0"), m_eventSink->GetStartIndex(NULL).c_str());

    VBT_STR strValue = VeciAllocString(_T("2"), 2);
    EXPECT_CALL(pcMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strValue), Return(S_OK)));
    EXPECT_STREQ(_T("5"), m_eventSink->GetStartIndex(&pcMsg).c_str());
}

TEST_F(VR_VBT_EventSink_Test, SendActionMessage)
{
    MockVR_VBT_VECIParsedMessage pcMsg;
    VBT_STR cstrAgentName = VeciAllocString(_T("Agent"), 6);
    VBT_STR cstrActionClass = VeciAllocString(_T("Action"), 7);
    VBT_STR cstrActionType = VeciAllocString(_T("Type"), 5);
    VBT_STR cstrListType = VeciAllocString(_T("List"), 5);

    EXPECT_CALL(pcMsg, GetAgentName(_)).WillOnce(
                DoAll(SetArgPointee<0>(cstrAgentName), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(cstrActionClass), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(cstrActionType), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(cstrListType), Return(S_OK)));
    EXPECT_CALL(m_callback, NotifyActionClass(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_eventSink->SendActionMessage(&pcMsg));
}

TEST_F(VR_VBT_EventSink_Test, SendActionMessage_NULL)
{
    MockVR_VBT_VECIParsedMessage pcMsg;
    VBT_STR cstrAgentName = NULL;
    VBT_STR cstrActionClass = NULL;
    VBT_STR cstrActionType = NULL;
    VBT_STR cstrListType = NULL;

    EXPECT_CALL(pcMsg, GetAgentName(_)).WillOnce(
                DoAll(SetArgPointee<0>(cstrAgentName), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetActionClass(_)).WillOnce(
                DoAll(SetArgPointee<0>(cstrActionClass), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(cstrActionType), Return(S_OK)));
    EXPECT_CALL(pcMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(cstrListType), Return(S_OK)));
    EXPECT_FALSE(m_eventSink->SendActionMessage(&pcMsg));
}

/* EOF */
