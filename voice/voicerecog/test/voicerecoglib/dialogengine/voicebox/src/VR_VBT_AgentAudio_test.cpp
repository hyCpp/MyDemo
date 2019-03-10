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
#include "VR_VBT_VECIListItems_mock.h"

#include "VR_VoiceBoxCatalogAudio.h"
#include "VR_VoiceBoxAgentAudio.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_AgentAudio_Test
*
* The class is just for VR_VoiceBoxAgentAudio test.
*/
class VR_VBT_AgentAudio_Test : public testing::Test
{
public:
    VR_VBT_AgentAudio_Test() : m_agentAudio(NULL)
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
    VR_VoiceBoxAgentAudio* m_agentAudio;

    VR_VoiceBoxCatalogAudio*   m_catalogAudio;

private:
    VR_VBT_AgentAudio_Test(const VR_VBT_AgentAudio_Test& other);
    void operator= (const VR_VBT_AgentAudio_Test& other);
};

void
VR_VBT_AgentAudio_Test::SetUp()
{
    m_agentAudio = new VR_VoiceBoxAgentAudio(
                m_engine,
                m_command,
                m_callback);

    m_catalogAudio = new VR_VoiceBoxCatalogAudio(
                m_engine,
                m_command,
                m_callback);
}

void
VR_VBT_AgentAudio_Test::TearDown()
{
    delete m_agentAudio;
    m_agentAudio = NULL;
    delete m_catalogAudio;
    m_catalogAudio = NULL;
}

TEST_F(VR_VBT_AgentAudio_Test, Initialize)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    EXPECT_TRUE(ret);
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, ReplyQueryInfo)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<action name=\"startNextAgent\"><op>getCurrentAlbum</op></action>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->ReplyQueryInfo(parser));
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    MockVR_VBT_VECIString mockString;
    EXPECT_CALL(mockString, SetString(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(mockString, IsEqual(_, _, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(VBT_FALSE), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateString(_)).WillOnce(
                DoAll(SetArgPointee<0>(&mockString), Return(S_OK)));

    VBT_CSTR szAgent = _T("Apps");
    VBT_BOOL bSupported = VBT_FALSE;
    HRESULT result = m_agentAudio->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(VBT_FALSE == bSupported);
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Apps");
    HRESULT result;
    result = m_agentAudio->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, QueryAgentSupport_True)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
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
    HRESULT result;
    result = m_agentAudio->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, OnAgentResultMessage_Fail)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentAudio->OnAgentResultMessage(NULL, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAudio->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentAudio_Test, OnAgentResultMessage_OK)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
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
    HRESULT result = m_agentAudio->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentAudio->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentAudio_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentAudio->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAudio->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentAudio_Test, OnAgentDriverMessage_OK)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
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
    HRESULT result = m_agentAudio->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentAudio->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentAudio_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAudio->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentAudio->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentAudio_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
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
    bool result = m_agentAudio->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, ProcessAgentMessage_ActionType_True)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
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
    bool result = m_agentAudio->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, ProcessAgentMessage_ListType_True)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
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
    bool result = m_agentAudio->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAudio->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentAudio_Test, SendMusicPlay)
{
    std::string strSourceID = _T("Source ID");
    std::string strPlayType = _T("PLAY_ALBUM");
    std::string strID = _T("Album ID");
    std::string strShuffle = _T("false");
    std::string strSubID = _T("Episode ID");

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentAudio->SendMusicPlay(strSourceID, strPlayType, strID, strShuffle, strSubID);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, PlayAlbum)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayAlbum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ShufflePlayAlbum)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ShufflePlayAlbum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayAll)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayAll(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayArtist)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ShufflePlayArtist)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ShufflePlayArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayAudioBook)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayAudioBook(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayComposer)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayComposer(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ShufflePlayComposer)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ShufflePlayComposer(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayGenre)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayGenre(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayPlaylist)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayPlaylist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ShufflePlayPlaylist)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ShufflePlayPlaylist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayPodcast)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayPodcast(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayPodcastItem)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayPodcastItem(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlaySong)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlaySong(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioOn)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioOn(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioOff)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioOff(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, BuildScreenDisplayFront)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    std::string strContent = _T("please balabala");
    std::string strPromtValue = _T("please balabala");
    m_agentAudio->BuildScreenDisplayFront(xmlBulder, strContent, strPromtValue);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, BuildScreenDisplayAll)
{
    std::string strContent = _T("please balabala");
    std::string strPromtValue = _T("please balabala");
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentAudio->BuildScreenDisplayAll(strContent, strPromtValue);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, BuildListHeader)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node nodeList;
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    m_agentAudio->BuildListHeader(&m_pResultMsg, xmlBulder, nodeList);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, BuildMusicInfo)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node nodeList;
    std::string strName = _T("Name");
    std::string strSourceId = _T("SourceId");
    std::string strId = _T("Id");

    m_agentAudio->BuildMusicInfo(xmlBulder, nodeList, strName, strSourceId, strId);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, BuildListInfo)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node nodeList;
    std::string strNamePara = _T("name");
    std::string strIDPara = _T("ID");
    std::string strSourceID = _T("sourceid");
    VBT_ULONG ulSize = 1;

    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    m_agentAudio->BuildListInfo(&m_pResultMsg, xmlBulder, nodeList, strNamePara,
                                strIDPara, strSourceID);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, BrowseAlbum)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->BrowseAlbum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnAlbums)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnAlbums(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, BrowseArtist)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->BrowseArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, BrowseArtistSongs)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->BrowseArtistSongs(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, BrowseComposer)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->BrowseComposer(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnDisambiguateAlbumArtist)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnDisambiguateAlbumArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, BrowseGenre)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->BrowseGenre(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, BrowsePlaylist)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->BrowsePlaylist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, BrowsePodcast)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->BrowsePodcast(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ChangeSourceCatch)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ChangeSourceCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnCatch)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnComposers)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnComposers(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayDisambiguateSongAlbum)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayDisambiguateSongAlbum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayDisambiguateSongArtist)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PlayDisambiguateSongArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnGenres)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnGenres(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnPlaylists)
{
    VBT_STR xmlMsg = NULL;
    VBT_STR strListType = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnPlaylists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnPodcasts)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnPodcasts(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnArtists)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnArtists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnAudiobooks)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnAudiobooks(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnSongs)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->OnSongs(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, MusicMenu)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->MusicMenu(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ChangeSource)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ChangeSource(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, MusicKeyboardDictation)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->MusicKeyboardDictation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ChangeSource_USB)
{
    VBT_STR strVBTValue = VeciAllocString(_T("USB"), 4);
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    m_catalogAudio->m_bIPodUSB2 = 1;

    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ChangeSource(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ChangeSource_iPod)
{
    VBT_STR strVBTValue = VeciAllocString(_T("iPod"), 5);
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    m_catalogAudio->m_bUSBIPod2 = 1;

    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ChangeSource(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ChangeSource_AM)
{
    VBT_STR strVBTValue = VeciAllocString(_T("AM"), 3);

    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ChangeSource(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioNBest_Music)
{
    VBT_STR strAgentName = VeciAllocString(_T("Music"), 6);
    EXPECT_CALL(m_pResultMsg, GetAgentName(_)).WillOnce(
                DoAll(SetArgPointee<0>(strAgentName), Return(S_OK)));
    VBT_STR xmlMsg = NULL;
    VBT_LONG uSize = 0;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioNBest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioNBest_Radio)
{
    VBT_STR strAgentName = VeciAllocString(_T("Radio"), 6);
    EXPECT_CALL(m_pResultMsg, GetAgentName(_)).WillOnce(
                DoAll(SetArgPointee<0>(strAgentName), Return(S_OK)));
    VBT_STR xmlMsg = NULL;
    VBT_LONG uSize = 0;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetValueByKey(_, _)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioNBest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, MusicNBest)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                 DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    VBT_ULONG uSize = 1;
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->MusicNBest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, OnInfoRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    m_agentAudio->OnInfoRequest(_T("Back"));
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentAlbumRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->GetCurrentAlbumRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentArtistRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->GetCurrentArtistRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentComposerRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->GetCurrentComposerRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentGenreRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->GetCurrentGenreRequest(&m_pResultMsg));
}

// TEST_F(VR_VBT_AgentAudio_Test, GetEpisodesByPodcast)
// {
//     VoiceMap<std::string, std::string>::type  mapTmp;
//     char *pValue[2] = { "PodcaseID", "SourceID" };
//     EXPECT_EQ(0, m_agentAudio->GetEpisodesByPodcast(reinterpret_cast<void*>(&mapTmp), 0,
//                                                     reinterpret_cast<char**>(pValue), NULL));
// }

TEST_F(VR_VBT_AgentAudio_Test, GetPodcastEpisodesRequest_NULL)
{
    EXPECT_FALSE(m_agentAudio->GetPodcastEpisodesRequest(NULL));
}

TEST_F(VR_VBT_AgentAudio_Test, GetPodcastEpisodesRequest_True)
{
    MockVR_VBT_VECIParameterSet spItem;
    MockVR_VBT_VECIListItems spListItems;
    VBT_STR strVBTValue = VeciAllocString(_T("play"), 5);

    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&spListItems), Return(S_OK)));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetPodcastEpisodesRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, GetPodcastEpisodesRequest_False)
{
    MockVR_VBT_VECIParameterSet spItem;
    MockVR_VBT_VECIListItems spListItems;
    VBT_STR strVBTValue = VeciAllocString(_T("play"), 5);

    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&spListItems), Return(S_OK)));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentAudio->GetPodcastEpisodesRequest(&m_pResultMsg));
}

// TEST_F(VR_VBT_AgentAudio_Test, GetPodcastEpisodesRequest_OpenSqlFail)
// {
//     EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
//                 Return(S_OK)).WillOnce(Return(S_OK));
//     EXPECT_FALSE(m_agentAudio->GetPodcastEpisodesRequest(&m_pResultMsg));
// }

// TEST_F(VR_VBT_AgentAudio_Test, GetPodcastEpisodesRequest_ExecSqlFail)
// {

// }

// TEST_F(VR_VBT_AgentAudio_Test, GetPodcastEpisodesRequest_True)
// {

// }

// TEST_F(VR_VBT_AgentAudio_Test, GetNameByID)
// {
//     std::string strName;
//     strName.clear();
//     char* pValue[1] = { _T("PodCast") };
//     EXPECT_EQ(0, m_agentAudio->GetNameByID(reinterpret_cast<void*>(&strName), 0,
//                                             reinterpret_cast<char**>(pValue), NULL));
// }

// TEST_F(VR_VBT_AgentAudio_Test, GetNameByTableNameAndID_OpenSqlFail)
// {
//     std::string strTableName = _T("");
//     std::string strID = _T("");
//     std::string strName = _T("");
//     std::string strSourceID = _T("");
//     m_agentAudio->GetNameByTableNameAndID(strTableName, strID, strName, strSourceID);
//     SUCCEED();
// }

// TEST_F(VR_VBT_AgentAudio_Test, GetNameByTableNameAndID_ExecSqlFail)
// {

// }

// TEST_F(VR_VBT_AgentAudio_Test, GetNameByTableNameAndID_True)
// {

// }

TEST_F(VR_VBT_AgentAudio_Test, GetStatusByErrCode_empty)
{
    std::string strErrCode = "";
    EXPECT_EQ("", m_agentAudio->GetStatusByErrCode(strErrCode));
}

TEST_F(VR_VBT_AgentAudio_Test, GetStatusByErrCode_0)
{
    std::string strErrCode = "0";
    EXPECT_EQ("Success", m_agentAudio->GetStatusByErrCode(strErrCode));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentAlbum_Success)
{
    std::string xml = "<errcode>0</errcode>"
                      "<songid>22</songid>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));

    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetCurrentAlbum(parser));
}

// TEST_F(VR_VBT_AgentAudio_Test, GetCurrentAlbum_Errcode0)
// {
//     std::string xml = "<errcode>0</errcode>";
//     VR_VoiceBoxXmlParser parser(xml);
//     EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
//                 DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
//     EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
//     EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
//     EXPECT_TRUE(m_agentAudio->GetCurrentAlbum(parser));
// }

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentAlbum_Errcode20)
{
    std::string xml = "<errcode>20</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetCurrentAlbum(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentAlbum_Errcode30)
{
    std::string xml = "<errcode>30</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentAudio->GetCurrentAlbum(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentArtist_Success)
{
    std::string xml = "<errcode>0</errcode>";
                      "<songid>22</songid>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetCurrentArtist(parser));
}

// TEST_F(VR_VBT_AgentAudio_Test, GetCurrentArtist_Errcode0)
// {
//     std::string xml = "<errcode>0</errcode>";
//     VR_VoiceBoxXmlParser parser(xml);
//     EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
//                 DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
//     EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
//     EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
//     EXPECT_TRUE(m_agentAudio->GetCurrentArtist(parser));
// }

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentArtist_Errcode20)
{
    std::string xml = "<errcode>20</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetCurrentArtist(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentArtist_Errcode30)
{
    std::string xml = "<errcode>30</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentAudio->GetCurrentArtist(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentComposer_Success)
{
    std::string xml = "<errcode>0</errcode>";
                      "<songid>22</songid>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetCurrentComposer(parser));
}

// TEST_F(VR_VBT_AgentAudio_Test, GetCurrentComposer_Errcode0)
// {
//     std::string xml = "<errcode>0</errcode>";
//     VR_VoiceBoxXmlParser parser(xml);
//     EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
//                 DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
//     EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
//     EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
//     EXPECT_TRUE(m_agentAudio->GetCurrentComposer(parser));
// }

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentComposer_Errcode20)
{
    std::string xml = "<errcode>20</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetCurrentComposer(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentComposer_Errcode30)
{
    std::string xml = "<errcode>30</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentAudio->GetCurrentComposer(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentGenre_Success)
{
    std::string xml = "<errcode>0</errcode>";
                      "<songid>22</songid>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetCurrentGenre(parser));
}

// TEST_F(VR_VBT_AgentAudio_Test, GetCurrentGenre_Errcode0)
// {
//     std::string xml = "<errcode>0</errcode>";
//     VR_VoiceBoxXmlParser parser(xml);
//     EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
//                 DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
//     EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
//     EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
//     EXPECT_TRUE(m_agentAudio->GetCurrentGenre(parser));
// }

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentGenre_Errcode20)
{
    std::string xml = "<errcode>20</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->GetCurrentGenre(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, GetCurrentGenre_Errcode30)
{
    std::string xml = "<errcode>30</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentAudio->GetCurrentGenre(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, ActiveFrequencyBandRequest)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ActiveFrequencyBandRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ActiveFrequencyBandReply)
{
    std::string xml = "<band>XM</band>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->ActiveFrequencyBandReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, FrequencyBandStatusRequest)
{
    VBT_STR strVBTValue = VeciAllocString(_T("Satellite"), 10);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->FrequencyBandStatusRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, FrequencyBandStatusReply_True)
{
    std::string xml = "<active>True</active>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->FrequencyBandStatusReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, FrequencyBandStatusReply_XM)
{
    std::string xml = "<active>False</active><band>XM</band>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->FrequencyBandStatusReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, FrequencyBandStatusReply_Else)
{
    std::string xml = "<active>False</active><band>XL</band>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentAudio->FrequencyBandStatusReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, HDSubChannelAvailforCurFreRequest)
{
    VBT_STR strFrequencyBand = VeciAllocString(_T("FM"), 3);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strFrequencyBand), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->HDSubChannelAvailforCurFreRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, HDSubChannelAvailforCurFreReply)
{
    std::string xml = "<available>False</available>"
                      "<frequency>87.7</frequency>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->HDSubChannelAvailforCurFreReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, HDSubchannelAvailableforFreRequest)
{
    VBT_STR strHDSubchannel = VeciAllocString(_T("HD"), 3);
    VBT_STR strFrequency = VeciAllocString(_T("87"), 3);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                DoAll(SetArgPointee<1>(strHDSubchannel), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strFrequency), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->HDSubchannelAvailableforFreRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, HDSubchannelAvailableforFreReply)
{
    std::string xml = "<available>False</available>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->HDSubchannelAvailableforFreReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, PresetDefinedRequest)
{
    VBT_STR strFrequencyBand = VeciAllocString(_T("FM"), 3);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strFrequencyBand), Return(S_OK)));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PresetDefinedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PresetDefinedReply)
{
    std::string xml = "<available>False</available>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->PresetDefinedReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, PresetsAvailableRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->PresetsAvailableRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PresetsAvailableReply)
{
    std::string xml = "<available>False</available>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->PresetsAvailableReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, RadioStatusRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->RadioStatusRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, RadioStatusReply_True)
{
    std::string xml = "<active>True</active>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->RadioStatusReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, RadioStatusReply_True_XM)
{
    std::string xml = "<active>False</active><band>XM</band>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->RadioStatusReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, RadioStatusReply_False_Else)
{
    std::string xml = "<active>False</active><band>XL</band>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentAudio->RadioStatusReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, StationsAvailableinFMGenreRequest)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->StationsAvailableinFMGenreRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, StationsAvailableinFMGenreReply)
{
    std::string xml = "<active>False</active><band>XM</band>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudio->StationsAvailableinFMGenreReply(parser));
}

TEST_F(VR_VBT_AgentAudio_Test, GoToRadio)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->GoToRadio(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneRadioCatch)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneRadioCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFrequencyBandAM)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFrequencyBandAM(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFrequencyBandFM)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFrequencyBandFM(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TunePresetCatch)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TunePresetCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFMGenreCatch)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFMGenreCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneSatelliteGenreCatch)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneSatelliteGenreCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFrequencyBandSatellite)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFrequencyBandSatellite(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFrequencyCatch)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFrequencyCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ActivateRadio_FM)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    VBT_STR strFrequencyBand = VeciAllocString(_T("FM"), 3);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strFrequencyBand), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ActivateRadio(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ActivateRadio_AM)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    VBT_STR strFrequencyBand = VeciAllocString(_T("AM"), 3);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strFrequencyBand), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ActivateRadio(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ActivateRadio_Satellite)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    VBT_STR strFrequencyBand = VeciAllocString(_T("Satellite"), 10);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strFrequencyBand), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ActivateRadio(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ActivateRadio_Else)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    VBT_STR strFrequencyBand = VeciAllocString(_T("else"), 10);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strFrequencyBand), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ActivateRadio(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, ChangeHDSubChannel)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->ChangeHDSubChannel(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFrequency)
{
    std::string strBand = _T("Band");
    std::string strFrequency = _T("Frequency");
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentAudio->TuneFrequency(strBand, strFrequency);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, TuneAMFrequency)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneAMFrequency(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFMFrequency)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFMFrequency(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFMGenre)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFMGenre(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneSatelliteChannelName)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneSatelliteChannelName(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneSatelliteChannelNumber)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneSatelliteChannelNumber(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, PlayByGenre)
{
    std::string strBand = _T("AM");
    std::string strGenre = _T("Tune");
    std::string strID = _T("100.1");
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentAudio->PlayByGenre(strBand, strGenre, strID);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_Test, TuneSatelliteGenre)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneSatelliteGenre(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, VBTSendCommand)
{
    EXPECT_FALSE(m_agentAudio->VBTSendCommand(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFMHDFrequency)
{
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFMHDFrequency(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TunePreset)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TunePreset(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFrequencyBandSatelliteStation)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFrequencyBandSatelliteStation(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFrequencyBandAMFrequency)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFrequencyBandAMFrequency(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, TuneFrequencyBandFMFrequency)
{
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->TuneFrequencyBandFMFrequency(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, RadioNBest_FM)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    VBT_ULONG uSize = 1;
    VBT_STR strCommand = VeciAllocString(_T("Tune FM Genre"), 14);
    VBT_STR strIDValue = VeciAllocString(_T("IDValue"), 6);
    VBT_STR strAliasValue = VeciAllocString(_T("Alias"), 6);
    VBT_STR strFormalNameValue = VeciAllocString(_T("Formal"), 7);
    EXPECT_CALL(m_pResultMsg, GetValueByKey(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strCommand), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<2>(strIDValue), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strAliasValue), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strFormalNameValue), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->RadioNBest(&m_pResultMsg));
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, RadioNBest_Channel)
{
    if (NULL == m_agentAudio) {
        FAIL();
    }

    bool ret = m_agentAudio->Initialize();
    VBT_STR xmlMsg = NULL;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    VBT_ULONG uSize = 1;
    VBT_STR strCommand = VeciAllocString(_T("Tune Channel Name"), 18);
    VBT_STR strIDValue = VeciAllocString(_T("IDValue"), 8);
    VBT_STR strAliasValue = VeciAllocString(_T("Alias"), 6);
    VBT_STR strFormalNameValue = VeciAllocString(_T("Formal"), 7);
    printf("1231131313");
    EXPECT_CALL(m_pResultMsg, GetValueByKey(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strCommand), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<2>(strIDValue), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strAliasValue), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<2>(strFormalNameValue), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->RadioNBest(&m_pResultMsg));
    m_agentAudio->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_Test, TaskComplete)
{
    VBT_STR strActionType = VeciAllocString(_T("Tune"), 14);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudio->TaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioBack)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioBack(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioCancel)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioCancel(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioHelp)
{
    EXPECT_TRUE(m_agentAudio->AudioHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioStartOver)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioStartOver(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioStartPause)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioStartPause(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, AudioStartRepeat)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->AudioStartRepeat(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_Test, EscalatingError_NULL)
{
    EXPECT_FALSE(m_agentAudio->EscalatingError(NULL));
}

TEST_F(VR_VBT_AgentAudio_Test, EscalatingError_NotNULL)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudio->EscalatingError(&m_pResultMsg));
}

/* EOF */
