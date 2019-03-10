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

#include "VR_VoiceBoxCatalogPoi.h"
#include "VECIOutStr.h"
#include "VR_Log.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_CatalogPoi_Test
*
* The class is just for VR_VoiceBoxCatalogPhone test.
*/
class VR_VBT_CatalogPoi_Test : public testing::Test
{
public:
    VR_VBT_CatalogPoi_Test() : m_catalogPoi(NULL)
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
    MockVR_VBT_VECIAgentDataCommand m_dataCommandPoi;
    MockVR_VBT_VECIListItems m_listItems;

    VR_VoiceBoxCatalogPoi* m_catalogPoi;

private:
    VR_VBT_CatalogPoi_Test(const VR_VBT_CatalogPoi_Test& other);
    void operator= (const VR_VBT_CatalogPoi_Test& other);
};

void
VR_VBT_CatalogPoi_Test::SetUp()
{
    m_catalogPoi = new VR_VoiceBoxCatalogPoi(
                m_engine,
                m_callback);
}

void
VR_VBT_CatalogPoi_Test::TearDown()
{
    delete m_catalogPoi;
    m_catalogPoi = NULL;
}

TEST_F(VR_VBT_CatalogPoi_Test, Initialize)
{
    // EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).WillOnce(
    //             DoAll(SetArgPointee<0>(&m_dataCommandPoi), Return(S_OK)));
    // EXPECT_CALL(m_dataCommandPoi, Init(_)).WillOnce(Return(S_OK));
    // EXPECT_TRUE(m_catalogPoi->Initialize());
}

TEST_F(VR_VBT_CatalogPoi_Test, Cataloguing)
{
    // EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).WillOnce(
    //             DoAll(SetArgPointee<0>(&m_dataCommandPoi), Return(S_OK)));
    // EXPECT_CALL(m_dataCommandPoi, Init(_)).WillOnce(Return(S_OK));
    // m_catalogPoi->Initialize();
    // VR_LOGI("123");
    // EXPECT_CALL(m_dataCommandPoi, SetDataSynchronized(_, _, _, _, _)).WillOnce(
    //             DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));

    // EXPECT_CALL(m_engine, CreateListItems(_)).WillOnce(
    //             DoAll(SetArgPointee<0>(&m_listItems), Return(S_OK)));
    // EXPECT_CALL(m_dataCommandPoi, ReloadData(_, _, _, _)).WillOnce(Return(S_OK));
    // EXPECT_CALL(m_dataCommandPoi, SetDataSynchronized(_, _, _, _, _)).WillOnce(
    //             DoAll(SetArgPointee<0>(&m_pTrans), Return(S_OK)));
    // EXPECT_CALL(m_dataCommandPoi, ReloadGrammar(_, _, _)).WillOnce(Return(S_OK));
    // VBT_STR strTrans = VeciAllocString(_T("111"), 4);
    // EXPECT_CALL(m_pTrans, GetTransactionId(_)).WillOnce(
    //             DoAll(SetArgPointee<0>(strTrans), Return(S_OK)));
    // std::string xml = "<grammar_new>"
    //                     "<category name=\"audiosource\" value=\"audio\">"
    //                         "<formal name=\"forma\" grammarid=\"off\" id=\"22\">"
    //                             "<alias name=\"alia\" tts=\"oo\">"
    //                             "</alias>"
    //                         "</formal>"
    //                     "</category>"
    //                 "</grammar_new>";
    // VR_VoiceBoxXmlParser parser(xml);
    // EXPECT_TRUE(m_catalogPoi->Cataloguing(parser));
}

TEST_F(VR_VBT_CatalogPoi_Test, ReadPoiData)
{

}

/* EOF */
