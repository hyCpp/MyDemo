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

#include "VR_VoiceBoxCatalogPhone.h"
#include "VECIOutStr.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_CatalogPhone_Test
*
* The class is just for VR_VoiceBoxCatalogPhone test.
*/
class VR_VBT_CatalogPhone_Test : public testing::Test
{
public:
    VR_VBT_CatalogPhone_Test() : m_catalogPhone(NULL)
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

    VR_VoiceBoxCatalogPhone* m_catalogPhone;

private:
    VR_VBT_CatalogPhone_Test(const VR_VBT_CatalogPhone_Test& other);
    void operator= (const VR_VBT_CatalogPhone_Test& other);
};

void
VR_VBT_CatalogPhone_Test::SetUp()
{
    m_catalogPhone = new VR_VoiceBoxCatalogPhone(
                m_engine,
                m_callback);
}

void
VR_VBT_CatalogPhone_Test::TearDown()
{
    delete m_catalogPhone;
    m_catalogPhone = NULL;
}

TEST_F(VR_VBT_CatalogPhone_Test, Initialize)
{
    EXPECT_CALL(m_engine, CreateAgentDispatcher(_)).WillOnce(
                DoAll(SetArgPointee<0>(&m_dataCommand), Return(S_OK)));
    EXPECT_CALL(m_dataCommand, Init(_)).WillOnce(Return(S_OK));
    EXPECT_TRUE(m_catalogPhone->Initialize());
}

TEST_F(VR_VBT_CatalogPhone_Test, UnInitialize)
{
    m_catalogPhone->UnInitialize();
}

TEST_F(VR_VBT_CatalogPhone_Test, CreateVoiceTag)
{
    m_catalogPhone->CreateVoiceTag();
}

TEST_F(VR_VBT_CatalogPhone_Test, CheckTempVoiceTag)
{
    std::string deviceAddress = "";
    m_catalogPhone->CheckTempVoiceTag(deviceAddress);
}

TEST_F(VR_VBT_CatalogPhone_Test, SetDeviceAddress)
{
    m_catalogPhone->SetDeviceAddress("test");
}

/* EOF */
