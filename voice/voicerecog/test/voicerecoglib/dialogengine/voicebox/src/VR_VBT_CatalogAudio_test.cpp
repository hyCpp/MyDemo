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
#include "VR_VBT_VECIAgentDataCommand_mock.h"
#include "VR_VBT_VECIListItems_mock.h"

#include "VR_VoiceBoxCatalogAudio.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_CatalogAudio_Test
*
* The class is just for VR_VoiceBoxCatalogAudio test.
*/
class VR_VBT_CatalogAudio_Test : public testing::Test
{
public:
    VR_VBT_CatalogAudio_Test() : m_catalogAudio(NULL)
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
    MockVR_VBT_VECIAgentDataCommand m_dataCommandMusic;
    MockVR_VBT_VECIAgentDataCommand m_dataCommandRadio;
    MockVR_VBT_VECIListItems m_listItems;

    VR_VoiceBoxCatalogAudio* m_catalogAudio;

private:
    VR_VBT_CatalogAudio_Test(const VR_VBT_CatalogAudio_Test& other);
    void operator= (const VR_VBT_CatalogAudio_Test& other);
};

void
VR_VBT_CatalogAudio_Test::SetUp()
{
    m_catalogAudio = new VR_VoiceBoxCatalogAudio(
                m_engine,
                m_command,
                m_callback);
}

void
VR_VBT_CatalogAudio_Test::TearDown()
{
    delete m_catalogAudio;
    m_catalogAudio = NULL;
}

TEST_F(VR_VBT_CatalogAudio_Test, Initialize)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    EXPECT_TRUE(ret);
}

TEST_F(VR_VBT_CatalogAudio_Test, Cataloguing_SetupMusicAudioSources_LoadAudioSource)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<grammar_new>"
                        "<category name=\"audiosource\" value=\"audio\">"
                            "<formal name=\"forma\" grammarid=\"off\" id=\"22\">"
                                "<alias name=\"alia\" tts=\"oo\">"
                                "</alias>"
                            "</formal>"
                        "</category>"
                    "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandMusic, SetDataSynchronized(_, _, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandMusic, ReloadData(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandMusic, ReloadGrammar(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));

    VBT_STR strTrans = VeciAllocString(_T("12306"), 6);
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    EXPECT_CALL(m_callback, InsertMusicUpdateGrammar(_, _)).WillOnce(Return());
    // EXPECT_TRUE(m_catalogAudio->Cataloguing(parser));
}
/*
TEST_F(VR_VBT_CatalogAudio_Test, Cataloguing_SetupRadioData_Commongenre_LoadFormalPronInfo)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<grammar_new>"
                        "<category name=\"commongenre\" value=\"audio\">"
                            "<formal name=\"forma\" id=\"22\">"
                                "<alias name=\"alia\" pron=\"oo\">"
                                "</alias>"
                            "</formal>"
                        "</category>"
                    "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandRadio, SetDataSynchronized(_, _, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, ReloadGrammar(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    VBT_STR strTrans = VeciAllocString(_T("33"), 3);
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    EXPECT_CALL(m_callback, InsertGrammarHandle(_, _));
    EXPECT_TRUE(m_catalogAudio->Cataloguing(parser));
}
*/
TEST_F(VR_VBT_CatalogAudio_Test, Cataloguing_SetupRadioData_Fmgenre_LoadFormalPronInfo)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<grammar_new>"
                        "<category name=\"fmgenre\" value=\"audio\">"
                            "<formal name=\"forma\" id=\"22\">"
                                "<alias name=\"alia\" pron=\"oo\">"
                                "</alias>"
                            "</formal>"
                        "</category>"
                    "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandRadio, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, ReloadData(_, _, _, _)).WillOnce(Return(E_FAIL));
    // EXPECT_TRUE(m_catalogAudio->Cataloguing(parser));
}

TEST_F(VR_VBT_CatalogAudio_Test, Cataloguing_SetupRadioData_Satchannelname_LoadFormalPronInfo)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<grammar_new>"
                        "<category name=\"satchannelname\" value=\"audio\">"
                            "<formal name=\"forma\" id=\"22\">"
                                "<alias name=\"alia\" pron=\"oo\">"
                                "</alias>"
                            "</formal>"
                        "</category>"
                    "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandRadio, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, ReloadData(_, _, _, _)).WillOnce(Return(E_FAIL));
    // EXPECT_TRUE(m_catalogAudio->Cataloguing(parser));
}

TEST_F(VR_VBT_CatalogAudio_Test, Cataloguing_SetupRadioData_Satchannelnumber_LoadSatChannelNumber)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<grammar_new>"
                        "<category name=\"satchannelnumber\" value=\"audio\">"
                            "<formal name=\"forma\" id=\"22\">"
                            "</formal>"
                        "</category>"
                    "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandRadio, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, ReloadData(_, _, _, _)).WillOnce(Return(E_FAIL));
    // EXPECT_TRUE(m_catalogAudio->Cataloguing(parser));
}

TEST_F(VR_VBT_CatalogAudio_Test, Cataloguing_SetupRadioData_SatGenre_LoadFormalPronInfo)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<grammar_new>"
                        "<category name=\"satgenre\" value=\"audio\">"
                            "<formal name=\"forma\" id=\"22\">"
                                "<alias name=\"alia\" pron=\"oo\">"
                                "</alias>"
                            "</formal>"
                        "</category>"
                    "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandRadio, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, ReloadData(_, _, _, _)).WillOnce(Return(E_FAIL));
    // EXPECT_TRUE(m_catalogAudio->Cataloguing(parser));
}

TEST_F(VR_VBT_CatalogAudio_Test, Cataloguing_SetupRadioData_Hdsubchannel_LoadHdSubChannel)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<grammar_new>"
                        "<category name=\"hdsubchannel\" value=\"audio\">"
                            "<formal name=\"forma\" intval=\"2\" id=\"22\">"
                                "<alias name=\"alia\" pron=\"oo\">"
                                "</alias>"
                            "</formal>"
                        "</category>"
                    "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandRadio, SetDataSynchronized(_, _, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, ReloadData(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, ReloadGrammar(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    VBT_STR strTrans = NULL;
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    // EXPECT_TRUE(m_catalogAudio->Cataloguing(parser));
}
/*
TEST_F(VR_VBT_CatalogAudio_Test, MusicGrammarActive)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string xml = "<grammarid>22</grammarid>"
                "<path>//server/sql/MusicCatalog/mql.db</path>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_command, SetPreference(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandMusic, SetDataActiveSource(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_catalogAudio->MusicGrammarActive(parser));
}

TEST_F(VR_VBT_CatalogAudio_Test, MusicGrammarDisActive)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }
    std::string xml = "<grammarid>22</grammarid>"
                "<path>//server/sql/MusicCatalog/mql.db</path>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandMusic, SetDataActiveSource(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_catalogAudio->MusicGrammarDisActive(parser));
}

TEST_F(VR_VBT_CatalogAudio_Test, SetupMusicData_1500)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string xml = "<grammarid>22</grammarid>"
            "<path>//server/sql/MusicCatalog/mql.db</path>"
            "<songitemcount>1460</songitemcount>"
            "<otheritemcount>50</otheritemcount>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_command, SetPreference(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_dataCommandMusic, ReloadGrammar(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    VBT_STR strTrans = VeciAllocString(_T("444"), 4);
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    EXPECT_CALL(m_callback, InsertMusicUpdateGrammar(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, InsertGrammarHandle(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_catalogAudio->SetupMusicData(parser));
}

TEST_F(VR_VBT_CatalogAudio_Test, SetupMusicData_1400)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string xml = "<grammarid>22</grammarid>"
            "<path>//server/sql/MusicCatalog/mql.db</path>"
            "<songitemcount>1360</songitemcount>"
            "<otheritemcount>40</otheritemcount>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_command, SetPreference(_, _, _, _)).Times(3).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_dataCommandMusic, ReloadGrammar(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    VBT_STR strTrans = NULL;
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    EXPECT_FALSE(m_catalogAudio->SetupMusicData(parser));
}

TEST_F(VR_VBT_CatalogAudio_Test, MusicDiffUpdate)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string xml = "<grammar_diff>"
                        "<category name=\"hdsubchannel\" grammarid=\"11\" value=\"audio\">"
                            "<delete name=\"forma\" id=\"22\">"
                            "</delete>"
                            "<add id=\"1\" name=\"alia\" pron=\"oo\" pronid=\"1\">"
                            "</add>"
                        "</category>"
                    "</grammar_diff>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_dataCommandMusic, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_catalogAudio->MusicDiffUpdate(parser));
}
*/
TEST_F(VR_VBT_CatalogAudio_Test, GetTableHandlerName_Song)
{
    std::string strGrammarName = _T("song");
    std::string strTableName;
    std::string strHandlerName;
    m_catalogAudio->GetTableHandlerName(strGrammarName, strTableName, strHandlerName);
    EXPECT_STREQ(_T("MusicAgentSongs"), strTableName.c_str());
    EXPECT_STREQ(_T("songs"), strHandlerName.c_str());
}

TEST_F(VR_VBT_CatalogAudio_Test, GetTableHandlerName_Album)
{
    std::string strGrammarName = _T("album");
    std::string strTableName;
    std::string strHandlerName;
    m_catalogAudio->GetTableHandlerName(strGrammarName, strTableName, strHandlerName);
    EXPECT_STREQ(_T("MusicAgentAlbums"), strTableName.c_str());
    EXPECT_STREQ(_T("albums"), strHandlerName.c_str());
}

TEST_F(VR_VBT_CatalogAudio_Test, GetTableHandlerName_Artist)
{
    std::string strGrammarName = _T("artist");
    std::string strTableName;
    std::string strHandlerName;
    m_catalogAudio->GetTableHandlerName(strGrammarName, strTableName, strHandlerName);
    EXPECT_STREQ(_T("MusicAgentArtists"), strTableName.c_str());
    EXPECT_STREQ(_T("artists"), strHandlerName.c_str());
}

TEST_F(VR_VBT_CatalogAudio_Test, GetTableHandlerName_Genre)
{
    std::string strGrammarName = _T("genre");
    std::string strTableName;
    std::string strHandlerName;
    m_catalogAudio->GetTableHandlerName(strGrammarName, strTableName, strHandlerName);
    EXPECT_STREQ(_T("MusicAgentGenres"), strTableName.c_str());
    EXPECT_STREQ(_T("genres"), strHandlerName.c_str());
}

TEST_F(VR_VBT_CatalogAudio_Test, GetTableHandlerName_Audiobook)
{
    std::string strGrammarName = _T("audiobook");
    std::string strTableName;
    std::string strHandlerName;
    m_catalogAudio->GetTableHandlerName(strGrammarName, strTableName, strHandlerName);
    EXPECT_STREQ(_T("MusicAgentAudiobooks"), strTableName.c_str());
    EXPECT_STREQ(_T("audiobooks"), strHandlerName.c_str());
}

TEST_F(VR_VBT_CatalogAudio_Test, GetTableHandlerName_Composer)
{
    std::string strGrammarName = _T("composer");
    std::string strTableName;
    std::string strHandlerName;
    m_catalogAudio->GetTableHandlerName(strGrammarName, strTableName, strHandlerName);
    EXPECT_STREQ(_T("MusicAgentComposers"), strTableName.c_str());
    EXPECT_STREQ(_T("composers"), strHandlerName.c_str());
}

TEST_F(VR_VBT_CatalogAudio_Test, GetTableHandlerName_Playlist)
{
    std::string strGrammarName = _T("playlist");
    std::string strTableName;
    std::string strHandlerName;
    m_catalogAudio->GetTableHandlerName(strGrammarName, strTableName, strHandlerName);
    EXPECT_STREQ(_T("MusicAgentPlaylists"), strTableName.c_str());
    EXPECT_STREQ(_T("playlists"), strHandlerName.c_str());
}

TEST_F(VR_VBT_CatalogAudio_Test, GetTableHandlerName_Podcast)
{
    std::string strGrammarName = _T("podcast");
    std::string strTableName;
    std::string strHandlerName;
    m_catalogAudio->GetTableHandlerName(strGrammarName, strTableName, strHandlerName);
    EXPECT_STREQ(_T("MusicAgentPodcasts"), strTableName.c_str());
    EXPECT_STREQ(_T("podcasts"), strHandlerName.c_str());
}

TEST_F(VR_VBT_CatalogAudio_Test, PauseGrammarMusic)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    std::pair<std::string, std::string> pairAgent2TransId;
    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_dataCommandMusic, PauseGrammarUpdate(_)).WillOnce(Return(S_OK));
    m_catalogAudio->PauseGrammarMusic();
}

TEST_F(VR_VBT_CatalogAudio_Test, PauseGrammarRadio)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    std::pair<std::string, std::string> pairAgent2TransId;
    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_dataCommandRadio, PauseGrammarUpdate(_)).WillOnce(Return(S_OK));
    m_catalogAudio->PauseGrammarRadio();
}

TEST_F(VR_VBT_CatalogAudio_Test, ResumeGrammarMusic)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_dataCommandMusic, ResumeGrammarUpdate(_)).WillOnce(Return(S_OK));
    m_catalogAudio->ResumeGrammarMusic();
}

TEST_F(VR_VBT_CatalogAudio_Test, ResumeGrammarRadio)
{
    if (NULL == m_catalogAudio) {
        FAIL();
    }

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(2).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommandRadio), Return(S_OK)));
    EXPECT_CALL(m_dataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogAudio->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_dataCommandRadio, ResumeGrammarUpdate(_)).WillOnce(Return(S_OK));
    m_catalogAudio->ResumeGrammarRadio();
}

/* EOF */
