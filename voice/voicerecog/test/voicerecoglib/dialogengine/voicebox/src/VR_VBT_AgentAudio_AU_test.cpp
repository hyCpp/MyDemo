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

#include "VR_VoiceBoxAgentAudio_AU.h"
#include "VR_VoiceBoxCatalogAudio.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_AgentAudio_AU_Test
*
* The class is just for VR_VoiceBoxAgentAudio_AU test.
*/
class VR_VBT_AgentAudio_AU_Test : public testing::Test
{
public:
    VR_VBT_AgentAudio_AU_Test() : m_agentAudioAU(NULL)
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
    VR_VoiceBoxAgentAudio_AU* m_agentAudioAU;
    VR_VoiceBoxCatalogAudio* m_pCatalogAudio;

private:
    VR_VBT_AgentAudio_AU_Test(const VR_VBT_AgentAudio_AU_Test& other);
    void operator= (const VR_VBT_AgentAudio_AU_Test& other);
};

void
VR_VBT_AgentAudio_AU_Test::SetUp()
{
    m_agentAudioAU = new VR_VoiceBoxAgentAudio_AU(
                m_engine,
                m_command,
                m_callback);
    m_pCatalogAudio = new VR_VoiceBoxCatalogAudio(
                m_engine,
                m_command,
                m_callback);
}

void
VR_VBT_AgentAudio_AU_Test::TearDown()
{
    delete m_agentAudioAU;
    m_agentAudioAU = NULL;
    delete m_pCatalogAudio;
    m_pCatalogAudio = NULL;
}

TEST_F(VR_VBT_AgentAudio_AU_Test, Initialize)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
    EXPECT_TRUE(ret);
    m_agentAudioAU->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ReplyQueryInfo)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<action name=\"startNextAgent\"><op>getCurrentAlbum</op></action>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_TRUE(m_agentAudioAU->ReplyQueryInfo(parser));
    m_agentAudioAU->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, QueryAgentSupport_False)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
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
    HRESULT result = m_agentAudioAU->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(VBT_FALSE == bSupported);
    m_agentAudioAU->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, QueryAgentSupport_Fail)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
    if (!ret) {
        FAIL();
    }

    VBT_CSTR szAgent = _T("Apps");
    HRESULT result;
    result = m_agentAudioAU->QueryAgentSupport(szAgent, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAudioAU->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, QueryAgentSupport_True)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
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
    result = m_agentAudioAU->QueryAgentSupport(szAgent, &bSupported);
    EXPECT_EQ(S_OK, result);
    EXPECT_TRUE(bSupported);
    m_agentAudioAU->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, OnAgentResultMessage_Fail)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentAudioAU->OnAgentResultMessage(NULL, NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAudioAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentAudio_AU_Test, OnAgentResultMessage_OK)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
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
    HRESULT result = m_agentAudioAU->OnAgentResultMessage(&m_pTrans, &m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentAudioAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentAudio_AU_Test, OnAgentDriverMessage_Fail)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
    if (!ret) {
        FAIL();
    }

    HRESULT result = m_agentAudioAU->OnAgentDriverMessage(NULL);
    EXPECT_EQ(E_FAIL, result);
    m_agentAudioAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentAudio_AU_Test, OnAgentDriverMessage_OK)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
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
    HRESULT result = m_agentAudioAU->OnAgentDriverMessage(&m_pResultMsg);
    EXPECT_EQ(S_OK, result);
    m_agentAudioAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentAudio_AU_Test, ProcessAgentMessage_NULL)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
    if (!ret) {
        FAIL();
    }

    bool result = m_agentAudioAU->ProcessAgentMessage(NULL);
    EXPECT_FALSE(result);
    m_agentAudioAU->UnInitialize();
}
/*
TEST_F(VR_VBT_AgentAudio_AU_Test, ProcessAgentMessage_FALSE)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
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
    bool result = m_agentAudioAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_FALSE(result);
    m_agentAudioAU->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ProcessAgentMessage_ActionType_True)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
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
    bool result = m_agentAudioAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAudioAU->UnInitialize();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ProcessAgentMessage_ListType_True)
{
    if (NULL == m_agentAudioAU) {
        FAIL();
    }

    bool ret = m_agentAudioAU->Initialize();
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
    bool result = m_agentAudioAU->ProcessAgentMessage(&m_pResultMsg);
    EXPECT_TRUE(result);
    m_agentAudioAU->UnInitialize();
}
*/
TEST_F(VR_VBT_AgentAudio_AU_Test, SendMusicPlay)
{
    std::string strSourceID = _T("Source ID");
    std::string strPlayType = _T("PLAY_ALBUM");
    std::string strID = _T("Album ID");
    std::string strName = _T("Name");
    std::string strShuffle = _T("false");
    std::string strSubID = _T("Episode ID");

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentAudioAU->SendMusicPlay(strSourceID, strPlayType, strID, strName, strShuffle, strSubID);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayAlbum)
{
    EXPECT_TRUE(m_agentAudioAU->PlayAlbum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayArtist)
{
    EXPECT_TRUE(m_agentAudioAU->PlayArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayPlaylist)
{
    EXPECT_TRUE(m_agentAudioAU->PlayPlaylist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlaySong)
{
    EXPECT_TRUE(m_agentAudioAU->PlaySong(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, BuildScreenDisplayFront_1)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    m_pCatalogAudio->m_bIpodActive = false;
    std::string strContent = _T("please balabala");
    std::string strPromtValue = _T("please balabala");
    m_agentAudioAU->BuildScreenDisplayFront(xmlBulder, strContent, strPromtValue);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, BuildScreenDisplayFront_2)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    m_pCatalogAudio->m_bIpodActive = true;
    std::string strContent = _T("please balabala");
    std::string strPromtValue = _T("please balabala");
    m_agentAudioAU->BuildScreenDisplayFront(xmlBulder, strContent, strPromtValue);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, BuildScreenDisplayAll)
{
    std::string strContent = _T("please balabala");
    std::string strPromtValue = _T("please balabala");
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentAudioAU->BuildScreenDisplayAll(strContent, strPromtValue);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, BuildListHeader)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node nodeList;
    m_agentAudioAU->BuildListHeader(&m_pResultMsg, xmlBulder, nodeList);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, BuildMusicInfo)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node nodeList;
    std::string strName = _T("Name");
    std::string strSourceId = _T("SourceId");
    std::string strId = _T("Id");

    m_agentAudioAU->BuildMusicInfo(xmlBulder, nodeList, strName, strSourceId, strId);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, BuildListInfo)
{
    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node nodeList;
    std::string strNamePara = _T("name");
    std::string strIDPara = _T("ID");
    std::string strSourceID = _T("sourceid");
    VBT_ULONG ulSize = 1;

    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(ulSize), Return(S_OK)));
    m_agentAudioAU->BuildListInfo(&m_pResultMsg, xmlBulder, nodeList, strNamePara,
                                strIDPara, strSourceID);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayAlbums)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayAlbums(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayDisambiguateAlbumArtist_1)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Message"), 8);
    VBT_STR strAlbumName = VeciAllocString(_T("AlbumName"), 10);
    VBT_STR strSourceID = VeciAllocString(_T("SourceId"), 9);
    VBT_STR strStartIndex = VeciAllocString(_T("2"), 2);
    VBT_STR strListCount = VeciAllocString(_T("4"), 2);
    VBT_ULONG uSize = 0;
    VBT_STR strListType = VeciAllocString(_T("ListType"), 9);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strAlbumName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strSourceID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strStartIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetNBestScreenFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayDisambiguateAlbumArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayDisambiguateAlbumArtist_2)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayDisambiguateAlbumArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayDisambiguateSongAlbum_1)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Message"), 8);
    VBT_STR strSongName = VeciAllocString(_T("SongName"), 9);
    VBT_STR strSourceID = VeciAllocString(_T("SourceId"), 9);
    VBT_STR strStartIndex = VeciAllocString(_T("2"), 2);
    VBT_STR strListCount = VeciAllocString(_T("4"), 2);
    VBT_ULONG uSize = 0;
    VBT_STR strListType = VeciAllocString(_T("ListType"), 9);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strSongName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strSourceID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strStartIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetNBestScreenFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayDisambiguateSongAlbum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayDisambiguateSongAlbum_2)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayDisambiguateSongAlbum(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayDisambiguateSongArtist_1)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Message"), 8);
    VBT_STR strSongName = VeciAllocString(_T("SongName"), 9);
    VBT_STR strSourceID = VeciAllocString(_T("SourceId"), 9);
    VBT_STR strStartIndex = VeciAllocString(_T("2"), 2);
    VBT_STR strListCount = VeciAllocString(_T("4"), 2);
    VBT_ULONG uSize = 0;
    VBT_STR strListType = VeciAllocString(_T("ListType"), 9);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(4).WillOnce(
                DoAll(SetArgPointee<1>(strSongName), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strSourceID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strStartIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetNBestScreenFlg(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayDisambiguateSongArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayDisambiguateSongArtist_2)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayDisambiguateSongArtist(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayPlaylists_1)
{
    VBT_STR xmlMsg = VeciAllocString(_T("Message"), 8);
    VBT_STR strSourceID = VeciAllocString(_T("SourceId"), 9);
    VBT_STR strStartIndex = VeciAllocString(_T("2"), 2);
    VBT_STR strListCount = VeciAllocString(_T("4"), 2);
    VBT_ULONG uSize = 0;
    VBT_STR strListType = VeciAllocString(_T("ListType"), 9);

    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).Times(3).WillOnce(
                DoAll(SetArgPointee<1>(strSourceID), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strStartIndex), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<1>(strListCount), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strListType), Return(S_OK)));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayPlaylists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayPlaylists_2)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayPlaylists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayArtists)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlayArtists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlaySongs)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PlaySongs(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, AudioHome)
{
    EXPECT_FALSE(m_agentAudioAU->AudioHome(NULL));

    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->AudioHome(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, AudioMenu)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->AudioMenu(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, BusyUpdatingGrammar)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->BusyUpdatingGrammar(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ChangeSource)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->ChangeSource(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ChangeSource_USB)
{
    EXPECT_FALSE(m_agentAudioAU->ChangeSource(NULL));

    VBT_STR strVBTValue = VeciAllocString(_T("USB"), 4);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, ChangeAudioSourceName(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->ChangeSource(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ChangeSource_FMRadio)
{
    VBT_STR strVBTValue = VeciAllocString(_T("FM Radio"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->ChangeSource(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ChangeSource_else)
{
    VBT_STR strVBTValue = VeciAllocString(_T("else"), 5);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->ChangeSource(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ChangeSourceNotConnected_USB2)
{
    EXPECT_FALSE(m_agentAudioAU->ChangeSourceNotConnected(NULL));

    VBT_STR strVBTValue = VeciAllocString(_T("USB2"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Change Source - Not Connected"), 30);

    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->ChangeSourceNotConnected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ChangeSourceNotConnected_iPod2)
{
    VBT_STR strVBTValue = VeciAllocString(_T("iPod2"), 6);
    VBT_STR strActionType = VeciAllocString(_T("Change Source - Not Connected"), 30);

    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->ChangeSourceNotConnected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ChangeSourceNotConnected_BluetoothAudio)
{
    VBT_STR strVBTValue = VeciAllocString(_T("Bluetooth Audio"), 16);
    VBT_STR strActionType = VeciAllocString(_T("Change Source - Not Connected"), 30);

    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->ChangeSourceNotConnected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, ChangeSourceNotConnected_else)
{
    VBT_STR strVBTValue = VeciAllocString(_T("else"), 5);
    VBT_STR strActionType = VeciAllocString(_T("Change Source - Not Connected"), 30);

    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strVBTValue), Return(S_OK)));
    EXPECT_FALSE(m_agentAudioAU->ChangeSourceNotConnected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, NoAlbums)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->NoAlbums(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, NoArtists)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->NoArtists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, NoAudioSourceConnected)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->NoAudioSourceConnected(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, NoPlaylists)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->NoPlaylists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, NoSongs)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->NoSongs(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PlayPlaylistNoSongs)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->PlayPlaylistNoSongs(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TooManyAlbums)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->TooManyAlbums(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TooManyArtists)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->TooManyArtists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TooManyPlaylists)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->TooManyPlaylists(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TooManySongs)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->TooManySongs(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, SourceConnectedRequest_AM_Radio)
{
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    VBT_STR strSourceName = VeciAllocString(_T("AM Radio"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strSourceName), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudioAU->SourceConnectedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, SourceConnectedRequest_FM_Radio)
{
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    VBT_STR strSourceName = VeciAllocString(_T("FM Radio"), 9);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strSourceName), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_FALSE(m_agentAudioAU->SourceConnectedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, SourceConnectedRequest_else)
{
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    VBT_STR strSourceName = VeciAllocString(_T("Radio"), 6);
    EXPECT_CALL(m_pResultMsg, GetActionParameterValue(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strSourceName), Return(S_OK)));
    std::string strAudioConnected = "False";
    EXPECT_CALL(m_callback, GetAudioConnected(_)).WillOnce(Return(strAudioConnected));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudioAU->SourceConnectedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, OnInfoRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    m_agentAudioAU->OnInfoRequest(_T("Back"));
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, GetNameByID)
{
    std::string strName = "name";
    int nCount = 1;
    char* pValue[] = { "name", "" };
    char** pName = NULL;
    EXPECT_EQ(0, m_agentAudioAU->GetNameByID((void*)&strName, nCount, pValue, pName));
}


TEST_F(VR_VBT_AgentAudio_AU_Test, AudioNBest)
{
    VBT_STR xmlMsg = NULL;
    VBT_LONG uSize = 1;
    EXPECT_CALL(m_pResultMsg, GetString(_)).WillOnce(
                DoAll(SetArgPointee<0>(xmlMsg), Return(S_OK)));
    VBT_STR strCommand = VeciAllocString(_T("Play Album"), 11);
    EXPECT_CALL(m_pResultMsg, GetValueByKey(_, _)).WillOnce(
                DoAll(SetArgPointee<1>(strCommand), Return(S_OK)));


    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());

    EXPECT_CALL(m_pResultMsg, GetListSize(_)).WillOnce(
                DoAll(SetArgPointee<0>(uSize), Return(S_OK)));
    EXPECT_CALL(m_pResultMsg, GetListItemParameterValue(_, _, _)).Times(2).WillOnce(
                Return(S_OK)).WillOnce(
                Return(S_OK));
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->AudioNBest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PresetDefinedRequest)
{
    EXPECT_CALL(m_callback, OnInfoQueryMessage(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->PresetDefinedRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PresetDefinedReply)
{
    std::string xml = "<available>False</available>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudioAU->PresetDefinedReply(parser));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PresetsAvailableRequest)
{
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudioAU->PresetsAvailableRequest(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PresetsAvailableReply)
{
    std::string xml = "<available>False</available>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_command, DriverActionReply(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_agentAudioAU->PresetsAvailableReply(parser));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TunePresetCatch)
{
    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->TunePresetCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TuneFrequencyCatch)
{
    EXPECT_FALSE(m_agentAudioAU->TuneFrequencyCatch(NULL));

    EXPECT_CALL(m_callback, GetPromptByScreenId(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->TuneFrequencyCatch(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TuneFrequency)
{
    std::string strBand = _T("Band");
    std::string strFrequency = _T("Frequency");
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    m_agentAudioAU->TuneFrequency(strBand, strFrequency);
    SUCCEED();
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TuneAMFrequency)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->TuneAMFrequency(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TuneFMFrequency)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->TuneFMFrequency(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TunePreset)
{
    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->TunePreset(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, TaskComplete)
{
    VBT_STR strActionType = VeciAllocString(_T("Tune"), 14);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->TaskComplete(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, AudioBack)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->AudioBack(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, AudioCancel)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->AudioCancel(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, AudioHelp)
{
    EXPECT_TRUE(m_agentAudioAU->AudioHelp(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, AudioStartOver)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->AudioStartOver(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, AudioStartPause)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->AudioStartPause(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, AudioStartRepeat)
{
    EXPECT_CALL(m_callback, OnRequest(_)).WillOnce(Return());
    EXPECT_TRUE(m_agentAudioAU->AudioStartRepeat(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PresetNotDefined)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->PresetNotDefined(&m_pResultMsg));
}

TEST_F(VR_VBT_AgentAudio_AU_Test, PresetsNotAvailable)
{
    VBT_STR strActionType = VeciAllocString(_T(""), 1);
    EXPECT_CALL(m_pResultMsg, GetActionType(_)).WillOnce(
                DoAll(SetArgPointee<0>(strActionType), Return(S_OK)));
    EXPECT_TRUE(m_agentAudioAU->PresetsNotAvailable(&m_pResultMsg));
}

/* EOF */
