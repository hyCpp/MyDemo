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

#include "VR_VoiceBoxCatalogManager.h"
#include "VR_VoiceBoxCatalogAudio.h"
#include "VR_VoiceBoxCatalogPhone.h"

#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_CatalogManager_Test
*
* The class is just for VR_VoiceBoxCatalogManager test.
*/
class VR_VBT_CatalogManager_Test : public testing::Test
{
public:
    VR_VBT_CatalogManager_Test() : m_catalogManager(NULL)
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

    MockVR_VBT_VECIAgentDataCommand m_agentDataCommandApps;
    MockVR_VBT_VECIAgentDataCommand m_agentDataCommandRadio;
    MockVR_VBT_VECIAgentDataCommand m_agentDataCommandMusic;
    MockVR_VBT_VECIAgentDataCommand m_agentDataCommandPhone;
    MockVR_VBT_VECIAgentDataCommand m_agentDataCommandPoi;
    MockVR_VBT_VECIListItems m_listItems;

    VR_VoiceBoxCatalogManager* m_catalogManager;
    VR_VoiceBoxCatalogAudio*   m_catalogAudio;
    VR_VoiceBoxCatalogPhone*   m_catalogPhone;

private:
    VR_VBT_CatalogManager_Test(const VR_VBT_CatalogManager_Test& other);
    void operator= (const VR_VBT_CatalogManager_Test& other);
};

void
VR_VBT_CatalogManager_Test::SetUp()
{
    m_catalogManager = new VR_VoiceBoxCatalogManager(
                m_engine,
                m_command,
                m_callback);

    m_catalogAudio = new VR_VoiceBoxCatalogAudio(
                m_engine,
                m_command,
                m_callback);

    m_catalogPhone = new VR_VoiceBoxCatalogPhone(
                m_engine,
                m_callback);
}

void
VR_VBT_CatalogManager_Test::TearDown()
{
    delete m_catalogManager;
    m_catalogManager = NULL;
    delete m_catalogAudio;
    m_catalogAudio = NULL;
    delete m_catalogPhone;
    m_catalogPhone = NULL;

}

TEST_F(VR_VBT_CatalogManager_Test, Initialize)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    EXPECT_TRUE(m_catalogManager->Initialize());
}
/*
TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_init)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_init name=\"grammar_init\">"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                                "<songitemcount>1460</songitemcount>"
                                "<otheritemcount>50</otheritemcount>"
                            "</grammar_init>");
    EXPECT_CALL(m_command, SetPreference(_, _, _, _)).Times(4).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, ReloadGrammar(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    VBT_STR strTrans = VeciAllocString(_T("444"), 4);
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    EXPECT_CALL(m_callback, InsertMusicUpdateGrammar(_, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, InsertGrammarHandle(_, _)).WillOnce(Return());

    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}
*/
TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_active_else)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_active name=\"grammar_active\">"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                                "<songitemcount>1460</songitemcount>"
                                "<otheritemcount>50</otheritemcount>"
                            "</grammar_active>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());

    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_disactive_else)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_disactive name=\"grammar_disactive\">"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_disactive>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());

    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_init_media)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_init name=\"grammar_init\">"
                                "<agent>media</agent>"
                            "</grammar_init>");

    m_catalogAudio->m_agentDataCommandMusic = NULL;
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_diff_poi)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_diff name=\"grammar_diff\">"
                                "<agent>poi</agent>"
                            "</grammar_diff>");

    EXPECT_CALL(m_agentDataCommandPoi, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_quickreplymessage)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"quickreplymessage\">"
                                "<message id=\"2\" name =\"Be x minutes late\">"
                            "</category>");

    m_catalogPhone->m_agentDataCommand = NULL;
    EXPECT_CALL(m_agentDataCommandPhone, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_fmgenre)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"fmgenre\">"
                                "<formal id = \"1\" name = \"News\">"
                                    "<alias name = \"News\" pron = \"nuz\"/>"
                                    "<alias name = \"\" pron = \"\"/>"
                                "</formal>"
                            "</category>");

    EXPECT_CALL(m_agentDataCommandRadio, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_satchannelnumber)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"satchannelnumber\">"
                                "<formal id = \"1\" name = \"2\"/>"
                            "</category>");

    EXPECT_CALL(m_agentDataCommandRadio, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_init_phone)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_init name=\"grammar_init\">"
                                "<agent>phone</agent>"
                            "</grammar_init>");
    EXPECT_CALL(m_callback, SetPhoneReloadDataFlg(_)).Times(2).WillRepeatedly(Return());
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());

    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_init_poi)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_init name=\"grammar_init\">"
                                "<agent>poi</agent>"
                            "</grammar_init>");

    EXPECT_CALL(m_agentDataCommandPoi, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_diff_else)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_diff name=\"grammar_diff\">"
                                "<agent>apps</agent>"
                            "</grammar_diff>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_phonetype)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"phonetype\">"
                                "<agent>hfd</agent>"
                            "</category>");

    EXPECT_CALL(m_agentDataCommandPhone, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_messagetype)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"messagetype\">"
                                "<agent>hfd</agent>"
                            "</category>");

    EXPECT_CALL(m_agentDataCommandPhone, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_audiosource)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"audiosource\">"
                                "<agent>hfd</agent>"
                            "</category>");

    EXPECT_CALL(m_agentDataCommandMusic, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_satchannelname)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"satchannelname\">"
                                "<formal id = \"1\" name = \"2\"/>"
                            "</category>");

    EXPECT_CALL(m_agentDataCommandRadio, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_satgenre)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"satgenre\">"
                                "<formal id = \"1\" name = \"2\"/>"
                            "</category>");

    EXPECT_CALL(m_agentDataCommandRadio, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_add)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"add\">"
                                "<formal id = \"1\" name = \"2\"/>"
                                "<add>"
                                "<formal id = \"2\" name = \"3\"/>"
                                "</add>"
                            "</category>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_delete)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"delete\">"
                                "<formal id = \"1\" name = \"2\"/>"
                                "<delete>"
                                "<formal id = \"2\" name = \"3\"/>"
                                "</delete>"
                            "</category>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_category_else)
{
    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<category name=\"music\">"
                                "<formal id = \"1\" name = \"2\"/>"
                            "</category>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_active_phone_true)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_active name=\"grammar_active\">"
                                "<agent>phone</agent>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                                "<songitemcount>1460</songitemcount>"
                                "<otheritemcount>50</otheritemcount>"
                            "</grammar_active>");
    m_catalogManager->m_isUpdateGrammar = true;

    EXPECT_CALL(m_callback, OnRecognized(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_active_phone_false)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_active name=\"grammar_active\">"
                                "<agent>phone</agent>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog</path>"
                                "<songitemcount>1460</songitemcount>"
                                "<otheritemcount>50</otheritemcount>"
                            "</grammar_active>");
    m_catalogManager->m_isUpdateGrammar = false;

    EXPECT_CALL(m_callback, SetPhoneReloadDataFlg(_)).Times(2).WillRepeatedly(Return());
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_active_media)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string SourceId = "33";
    std::string message = _T("<grammar_active name=\"grammar_active\">"
                                "<agent>media</agent>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                                "<reply>true</reply>"
                                "<songitemcount>1460</songitemcount>"
                                "<otheritemcount>50</otheritemcount>"
                            "</grammar_active>");

    EXPECT_CALL(m_callback, GetMDActiveSourceId()).WillOnce(Return(SourceId));
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_disactive_phone)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_disactive name=\"grammar_disactive\">"
                                "<agent>phone</agent>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_disactive>");

    EXPECT_CALL(m_agentDataCommandPhone, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_disactive_media)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_disactive name=\"grammar_disactive\">"
                                "<agent>media</agent>"
                                "<reply>true</reply>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_disactive>");
    std::string SourceId = "6";

    EXPECT_CALL(m_callback, GetMDActiveSourceId()).WillOnce(Return(SourceId));
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_voicetag)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_voicetag name=\"grammar_voicetag\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_voicetag>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_audiosource_oc)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_audiosource_oc name=\"grammar_audiosource_oc\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<withdisc>disc</withdisc>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_audiosource_oc>");

    EXPECT_CALL(m_agentDataCommandMusic, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_delete)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_delete name=\"grammar_delete\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<withdisc>disc</withdisc>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_delete>");

    EXPECT_CALL(m_agentDataCommandPoi, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_reloadgrammar)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_reloadgrammar name=\"grammar_reloadgrammar\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<withdisc>disc</withdisc>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_reloadgrammar>");

    EXPECT_CALL(m_agentDataCommandPhone, ReloadGrammar(_, _, _)).WillOnce(Return(E_FAIL));
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_else)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(true));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_else name=\"grammar_else\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<withdisc>disc</withdisc>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_else>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_notstart_grammar_init_media)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(false));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_init name=\"grammar_init\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<withdisc>disc</withdisc>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_init>");
    
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_notstart_grammar_init_phone)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(false));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_init name=\"grammar_init\">"
                                "<agent>phone</agent>"
                                "<op>grammar</op>"
                                "<withdisc>disc</withdisc>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_init>");
    
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_notstart_grammar_active_phone)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(false));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_active name=\"grammar_active\">"
                                "<agent>phone</agent>"
                                "<op>grammar</op>"
                                "<withdisc>disc</withdisc>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_active>");
    
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_notstart_grammar_active_media)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(false));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_active name=\"grammar_active\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<reply>true</reply>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_active>");
    
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_notstart_grammar_disactive_phone)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(false));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_disactive name=\"grammar_disactive\">"
                                "<agent>phone</agent>"
                                "<sender>DM</sender>"
                                "<reply>true</reply>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_disactive>");
    
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_notstart_grammar_disactive_media)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(false));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_disactive name=\"grammar_disactive\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<reply>true</reply>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_disactive>");
    
    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ProcessMessage_grammar_notstart_else)
{

    EXPECT_CALL(m_callback, SetNeedGrammarResult(_)).WillOnce(Return());
    EXPECT_CALL(m_callback, GetEngineStatus()).WillOnce(Return(false));

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string message = _T("<grammar_else name=\"grammar_else\">"
                                "<agent>media</agent>"
                                "<op>grammar</op>"
                                "<reply>true</reply>"
                                "<grammarid>22</grammarid>"
                                "<path>//server/sql/MusicCatalog/mql.db</path>"
                            "</grammar_else>");

    EXPECT_CALL(m_callback, SetUpdateGammarFlg(_)).WillOnce(Return());
    m_catalogManager->ProcessMessage(message);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, GetCatalogPhone)
{
    m_catalogManager->GetCatalogPhone();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, GetCatalogAudio)
{
    m_catalogManager->GetCatalogAudio();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, PauseGrammarUpdate_music)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("music");
    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "musicTran";

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_agentDataCommandMusic, PauseGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->PauseGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, PauseGrammarUpdate_radio)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("radio");
    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "radioTran";

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_agentDataCommandRadio, PauseGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->PauseGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, PauseGrammarUpdate_hfd)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("hfd");
    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "hfdTran";

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_agentDataCommandPhone, PauseGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->PauseGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, PauseGrammarUpdate_apps)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("apps");
    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "appsTran";

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_agentDataCommandApps, PauseGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->PauseGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, PauseGrammarUpdate_poi)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("poi");
    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "poiTran";

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_agentDataCommandPoi, PauseGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->PauseGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ResumeGrammarUpdate_music)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("music");

    EXPECT_CALL(m_agentDataCommandMusic, ResumeGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->ResumeGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ResumeGrammarUpdate_radio)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("radio");

    EXPECT_CALL(m_agentDataCommandRadio, ResumeGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->ResumeGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ResumeGrammarUpdate_hfd)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("hfd");

    EXPECT_CALL(m_agentDataCommandPhone, ResumeGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->ResumeGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ResumeGrammarUpdate_apps)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("apps");

    EXPECT_CALL(m_agentDataCommandApps, ResumeGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->ResumeGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, ResumeGrammarUpdate_poi)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("poi");

    EXPECT_CALL(m_agentDataCommandPoi, ResumeGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->ResumeGrammarUpdate(strAgent);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, SendPhoneGrammarResult_active)
{
    std::string strPhoneGrammar = _T("<grammar_active agent=\"phone\">");

    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    m_catalogManager->SendPhoneGrammarResult(strPhoneGrammar);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, SendPhoneGrammarResult_disactive)
{
    std::string strPhoneGrammar = _T("<grammar_disactive agent=\"phone\">");

    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    m_catalogManager->SendPhoneGrammarResult(strPhoneGrammar);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, SendPhoneGrammarResult_grammar)
{
    std::string strPhoneGrammar = _T("<grammar_init agent=\"phone\">");

    EXPECT_CALL(m_callback, SendGrammarResult(_, _, _, _)).WillOnce(Return());
    m_catalogManager->SendPhoneGrammarResult(strPhoneGrammar);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, GetCurrentTransaction)
{
    std::pair<std::string, std::string> pairTransaction;
    pairTransaction.first = "Agent";
    pairTransaction.second = "TransId";
    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairTransaction));
    m_catalogManager->GetCurrentTransaction();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, CancelGrammarUpdate_music)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("music");
    bool bSync = 1;

    EXPECT_CALL(m_agentDataCommandMusic, CancelGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->CancelGrammarUpdate(strAgent, bSync);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, CancelGrammarUpdate_radio)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("radio");
    bool bSync = 1;

    EXPECT_CALL(m_agentDataCommandRadio, CancelGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->CancelGrammarUpdate(strAgent, bSync);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, CancelGrammarUpdate_hfd)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("hfd");
    bool bSync = 1;

    EXPECT_CALL(m_agentDataCommandPhone, CancelGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->CancelGrammarUpdate(strAgent, bSync);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, CancelGrammarUpdate_apps)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("apps");
    bool bSync = 1;

    EXPECT_CALL(m_agentDataCommandApps, CancelGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->CancelGrammarUpdate(strAgent, bSync);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, CancelGrammarUpdate_poi)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    std::string strAgent = _T("poi");
    bool bSync = 1;

    EXPECT_CALL(m_agentDataCommandPoi, CancelGrammarUpdate(_)).WillOnce(Return(E_FAIL));
    m_catalogManager->CancelGrammarUpdate(strAgent, bSync);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, InitialPersonData_Phone_not_NULL)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    m_catalogManager->m_catalogPhone != NULL;
    m_catalogManager->m_catalogAudio = NULL;
    EXPECT_CALL(m_agentDataCommandPhone, SetDataSynchronized(_, _, _, _, _)).WillOnce(Return(E_FAIL));
    m_catalogManager->InitialPersonData();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, InitialPersonData_Audio_not_NULL)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).Times(5).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandApps), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandMusic), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandRadio), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPhone), Return(S_OK))).WillOnce(
                DoAll(SetArgPointee<0>(&m_agentDataCommandPoi), Return(S_OK)));
    EXPECT_CALL(m_agentDataCommandApps, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandMusic, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandRadio, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPhone, Init(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_agentDataCommandPoi, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogManager->Initialize();
    if (!ret) {
        FAIL();
    }

    m_catalogManager->m_catalogPhone = NULL;
    m_catalogManager->m_catalogAudio != NULL;
    EXPECT_CALL(m_agentDataCommandMusic, SetDataSynchronized(_, _, _, _, _)).Times(4).WillRepeatedly(Return(E_FAIL));
    m_catalogManager->InitialPersonData();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, SetNeedPauseGrammarFlg)
{
    bool bNeedPauseGrammarFlg = 1;
    m_catalogManager->SetNeedPauseGrammarFlg(bNeedPauseGrammarFlg);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, GetNeedPauseGrammarFlg)
{
    m_catalogManager->m_bNeedPauseGrammar = 1;
    EXPECT_TRUE(m_catalogManager->GetNeedPauseGrammarFlg());
}

TEST_F(VR_VBT_CatalogManager_Test, ClearCurrentTransaction)
{
    EXPECT_CALL(m_callback, SetCurrentTransaction(_, _)).WillOnce(Return());
    m_catalogManager->ClearCurrentTransaction();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, GetVBTErrorFlg)
{
    m_catalogManager->m_bVBTError = 1;
    EXPECT_TRUE(m_catalogManager->GetVBTErrorFlg());
}

TEST_F(VR_VBT_CatalogManager_Test, HandleGrammarAfterVDATAUpdate_1)
{
   bool isVDataUpdate = true;
   bool isNeedInstallApp = true;
   m_catalogManager->HandleGrammarAfterVDATAUpdate(isVDataUpdate, isNeedInstallApp);
   SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, HandleGrammarAfterVDATAUpdate_2)
{
   bool isVDataUpdate = false;
   bool isNeedInstallApp = true;
   m_catalogManager->HandleGrammarAfterVDATAUpdate(isVDataUpdate, isNeedInstallApp);
   SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, RequestGrammarExceptApp_True)
{
    bool isVDataUpdate = true;
    EXPECT_CALL(m_callback, SendRequestGrammar(_, _)).Times(4).WillRepeatedly(Return());
    m_catalogManager->RequestGrammarExceptApp(isVDataUpdate);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, RequestGrammarExceptApp_False)
{
    bool isVDataUpdate = false;

    VoiceMultiMap<std::string, std::string>::type MapUndoGrammar;
    MapUndoGrammar.insert(std::pair<std::string, std::string>("media", "1"));
    MapUndoGrammar.insert(std::pair<std::string, std::string>("media", "2"));
    MapUndoGrammar.insert(std::pair<std::string, std::string>("media", "3"));
    MapUndoGrammar.insert(std::pair<std::string, std::string>("media", "4"));

    m_catalogManager->m_MapUndoGrammar = MapUndoGrammar;

    EXPECT_CALL(m_callback, SendRequestGrammar(_, _)).Times(4).WillRepeatedly(Return());
    m_catalogManager->RequestGrammarExceptApp(isVDataUpdate);
    SUCCEED();
}

TEST_F(VR_VBT_CatalogManager_Test, UpdateGrammarFinishedStatus_0)
{
    std::string strGrammarName = "app";
    std::string strGrammarID = "";
    EXPECT_TRUE(m_catalogManager->UpdateGrammarFinishedStatus(strGrammarName, strGrammarID));
}

TEST_F(VR_VBT_CatalogManager_Test, UpdateGrammarFinishedStatus_1)
{
    std::string strGrammarName = "media";
    std::string strGrammarID = "1";
    EXPECT_TRUE(m_catalogManager->UpdateGrammarFinishedStatus(strGrammarName, strGrammarID));
}

TEST_F(VR_VBT_CatalogManager_Test, UpdateGrammarFinishedStatus_2)
{
    std::string strGrammarName = "media";
    std::string strGrammarID = "2";
    EXPECT_TRUE(m_catalogManager->UpdateGrammarFinishedStatus(strGrammarName, strGrammarID));
}

TEST_F(VR_VBT_CatalogManager_Test, UpdateGrammarFinishedStatus_3)
{
    std::string strGrammarName = "media";
    std::string strGrammarID = "3";
    EXPECT_TRUE(m_catalogManager->UpdateGrammarFinishedStatus(strGrammarName, strGrammarID));
}

TEST_F(VR_VBT_CatalogManager_Test, UpdateGrammarFinishedStatus_4)
{
    std::string strGrammarName = "media";
    std::string strGrammarID = "4";
    EXPECT_TRUE(m_catalogManager->UpdateGrammarFinishedStatus(strGrammarName, strGrammarID));
}

TEST_F(VR_VBT_CatalogManager_Test, UpdateGrammarFinishedStatus_else)
{
    std::string strGrammarName = "media";
    std::string strGrammarID = "5";
    EXPECT_FALSE(m_catalogManager->UpdateGrammarFinishedStatus(strGrammarName, strGrammarID));
}

TEST_F(VR_VBT_CatalogManager_Test, GetGrammarUndoStatus)
{
    VoiceMultiMap<std::string, std::string>::type mapUndoGrammar;
    mapUndoGrammar.insert(std::pair<std::string, std::string>("navi", "1"));
    EXPECT_FALSE(m_catalogManager->GetGrammarUndoStatus(mapUndoGrammar));
}

/* EOF */
