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

#include "VR_VoiceBoxCatalogApps.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_CatalogApps_Test
*
* The class is just for VR_VoiceBoxCatalogApps test.
*/
class VR_VBT_CatalogApps_Test : public testing::Test
{
public:
    VR_VBT_CatalogApps_Test() : m_catalogApps(NULL)
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
    MockVR_VBT_VECIAgentDataCommand m_dataCommand;
    MockVR_VBT_VECIListItems m_listItems;

    VR_VoiceBoxCatalogApps* m_catalogApps;

private:
    VR_VBT_CatalogApps_Test(const VR_VBT_CatalogApps_Test& other);
    void operator= (const VR_VBT_CatalogApps_Test& other);
};

void
VR_VBT_CatalogApps_Test::SetUp()
{
    m_catalogApps = new VR_VoiceBoxCatalogApps(
                m_engine,
                m_callback);
}

void
VR_VBT_CatalogApps_Test::TearDown()
{
    delete m_catalogApps;
    m_catalogApps = NULL;
}

TEST_F(VR_VBT_CatalogApps_Test, Initialize)
{
    if (NULL == m_catalogApps) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommand), Return(S_OK)));
    EXPECT_CALL(m_dataCommand, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogApps->Initialize();
    EXPECT_TRUE(ret);
}

TEST_F(VR_VBT_CatalogApps_Test, Cataloguing)
{
    if (NULL == m_catalogApps) {
        FAIL();
    }
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommand), Return(S_OK)));
    EXPECT_CALL(m_dataCommand, Init(_)).WillOnce(Return(S_OK));

    bool ret = m_catalogApps->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_dataCommand, SetDataSynchronized(_, _, _, _, _)).Times(2).WillRepeatedly(Return(S_OK));

    std::string xml = "<category name=\"apps\" value=\"cate\"></category>"
                    "<formal name=\"forma\" value=\"off\"></formal>"
                    "<alias name=\"alia\" value=\"oo\"></alias>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    EXPECT_CALL(m_engine, CreateParameterSet(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_paraSet), Return(S_OK)));
    EXPECT_CALL(m_paraSet, AddParameter(_, _, _, _)).Times(5).WillRepeatedly(Return(S_OK));
    EXPECT_CALL(m_listItems, AddItem(_)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommand, ReloadData(_, _, _, _)).WillOnce(Return(S_OK));
    EXPECT_CALL(m_dataCommand, ReloadGrammar(_, _, _)).WillOnce(
                DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));

    VBT_STR strTrans = VeciAllocString("2", 2);
    EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
                DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    EXPECT_CALL(m_callback, SetCurrentTransaction(_, _)).WillOnce(Return());
    EXPECT_TRUE(m_catalogApps->Cataloguing(parser));
}

TEST_F(VR_VBT_CatalogApps_Test, PauseGrammarApps)
{
    if (NULL == m_catalogApps) {
        FAIL();
    }

    std::pair<std::string, std::string> pairAgent2TransId;
    pairAgent2TransId.first = "apps";

    EXPECT_CALL(m_callback, GetCurrentTransaction()).WillOnce(Return(pairAgent2TransId));
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommand), Return(S_OK)));
    EXPECT_CALL(m_dataCommand, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogApps->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_dataCommand, PauseGrammarUpdate(_)).WillOnce(Return(S_OK));
    m_catalogApps->PauseGrammarApps();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogApps_Test, ResumeGrammarApps)
{
    if (NULL == m_catalogApps) {
        FAIL();
    }

    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommand), Return(S_OK)));
    EXPECT_CALL(m_dataCommand, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogApps->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_dataCommand, ResumeGrammarUpdate(_)).WillOnce(Return(S_OK));
    m_catalogApps->ResumeGrammarApps();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogApps_Test, CancelGrammarUpdate)
{
    if (NULL == m_catalogApps) {
        FAIL();
    }
    bool bSync = 1;
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommand), Return(S_OK)));
    EXPECT_CALL(m_dataCommand, Init(_)).WillOnce(Return(S_OK));
    bool ret = m_catalogApps->Initialize();
    if (!ret) {
        FAIL();
    }

    EXPECT_CALL(m_dataCommand, CancelGrammarUpdate(_)).WillOnce(Return(S_OK));
    m_catalogApps->CancelGrammarUpdate(bSync);
    SUCCEED();
}

/* EOF */
