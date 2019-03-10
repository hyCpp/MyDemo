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
#include "gtest/gtest.h"
#define DWORD S_DOWRD
#include "VR_VoiceBoxCatalogController.h"
#undef DWORD
#include "VR_VBT_VECIEngineClient_mock.h"
#include "VR_VBT_VECIEngineCommand_mock.h"
#include "VR_VBT_EngineCallback_mock.h"

#include "VR_VoiceBoxCatalogManager.h"
#include "VR_VBT_EngineIF_mock.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_CatalogController_Test
*
* The class is just for VR_VoiceBoxCatalogController test.
*/

class VR_VBT_CatalogController_Test : public testing::Test
{
public:
    VR_VBT_CatalogController_Test() : m_catalogController(NULL)
    {
        m_pcCatalogManager = new VR_VoiceBoxCatalogManager(
                    m_engine,
                    m_command,
                    m_callback);
    }

    ~VR_VBT_CatalogController_Test()
    {
        delete m_pcCatalogManager;
        m_pcCatalogManager = NULL;
    }

    virtual void SetUp();
    virtual void TearDown();
private:
    MockVR_VBT_VECIEngineClient m_engine;
    MockVR_VBT_VECIEngineCommand m_command;
    MockVR_VBT_EngineCallback m_callback;
    MockVR_VBT_EngineIF m_engineIF;

    VR_VoiceBoxCatalogManager* m_pcCatalogManager;
    VR_VoiceBoxCatalogController* m_catalogController;
};

void VR_VBT_CatalogController_Test::SetUp()
{
    m_catalogController = new VR_VoiceBoxCatalogController();
}

void VR_VBT_CatalogController_Test::TearDown()
{
    delete m_catalogController;
    m_catalogController = NULL;
}

TEST_F(VR_VBT_CatalogController_Test, Initialize)
{
    EXPECT_CALL(m_engineIF, GetCatalogManager()).WillOnce(Return(m_pcCatalogManager));
    EXPECT_TRUE(m_catalogController->Initialize(&m_engineIF));
}

TEST_F(VR_VBT_CatalogController_Test, Start)
{
    m_catalogController->Start();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogController_Test, Stop)
{
    m_catalogController->Stop();
    SUCCEED();
}

TEST_F(VR_VBT_CatalogController_Test, PostMessage)
{
    if (NULL == m_catalogController) {
        FAIL();
    }

    EXPECT_CALL(m_engineIF, GetCatalogManager()).WillOnce(Return(m_pcCatalogManager));
    bool ret = m_catalogController->Initialize(&m_engineIF);
    if (!ret) {
        FAIL();
    }

    std::string message = _T("Controller");
    EXPECT_TRUE(m_catalogController->PostMessage(message));
}

TEST_F(VR_VBT_CatalogController_Test, Run)
{
    // while(true);
}

/* EOF */
