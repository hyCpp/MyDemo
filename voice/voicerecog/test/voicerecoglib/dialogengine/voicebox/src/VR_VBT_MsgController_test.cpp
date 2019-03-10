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
#include "VBTExternalErrors.h"
#include "VR_VBT_EngineIF_mock.h"
#include "VR_VoiceBoxMsgController.h"

/**
* VR_VBT_MsgController_Test
*
* The class is just for VR_VoiceBoxMsgController test.
*/
class VR_VBT_MsgController_Test : public testing::Test
{
public:
    VR_VBT_MsgController_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

private:
    VR_VoiceBoxMsgController* m_msgController;
};

void
VR_VBT_MsgController_Test::SetUp()
{
    m_msgController = new VR_VoiceBoxMsgController();
}

void
VR_VBT_MsgController_Test::TearDown()
{
    delete m_msgController;
    m_msgController = NULL;
}

TEST_F(VR_VBT_MsgController_Test, Initialize)
{
    MockVR_VBT_EngineIF pcEngine;
    EXPECT_FALSE(m_msgController->Initialize(NULL));
    EXPECT_TRUE(m_msgController->Initialize(&pcEngine));
}

TEST_F(VR_VBT_MsgController_Test, Start_Stop)
{
    m_msgController->Start();
    m_msgController->Stop();
    SUCCEED();
}

TEST_F(VR_VBT_MsgController_Test, PostMessage)
{
    MockVR_VBT_EngineIF pcEngine;
    std::string message = _T("Start");
    EXPECT_TRUE(m_msgController->Initialize(&pcEngine));
    EXPECT_TRUE(m_msgController->PostMessage(message));
}

TEST_F(VR_VBT_MsgController_Test, PostMessage_False)
{
    MockVR_VBT_EngineIF pcEngine;
    std::string message = _T("Start");
    EXPECT_FALSE(m_msgController->PostMessage(message));
}

TEST_F(VR_VBT_MsgController_Test, Run)
{
    MockVR_VBT_EngineIF pcEngine;
    std::string message = _T("Start");
    EXPECT_TRUE(m_msgController->Initialize(&pcEngine));
    m_msgController->Start();
    EXPECT_TRUE(m_msgController->PostMessage(message));
    usleep(1000);
    m_msgController->Stop();
}

TEST_F(VR_VBT_MsgController_Test, ClearMessage)
{
    m_msgController->ClearMessage();
    SUCCEED();
}

TEST_F(VR_VBT_MsgController_Test, GetCurrentMessage)
{
    EXPECT_EQ("", m_msgController->GetCurrentMessage());
}

TEST_F(VR_VBT_MsgController_Test, PostStatusMsg)
{
    std::string message = _T("Start");
    EXPECT_TRUE(m_msgController->PostStatusMsg(message));
}

TEST_F(VR_VBT_MsgController_Test, PushFrontMessage)
{
    MockVR_VBT_EngineIF pcEngine;
    std::string message = _T("Start");
    EXPECT_TRUE(m_msgController->Initialize(&pcEngine));
    EXPECT_TRUE(m_msgController->PushFrontMessage(message));
}

TEST_F(VR_VBT_MsgController_Test, PushFrontMessage_False)
{
    MockVR_VBT_EngineIF pcEngine;
    std::string message = _T("Start");
    EXPECT_FALSE(m_msgController->PushFrontMessage(message));
}

/* EOF */
