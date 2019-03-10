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

#include "VP_Define.h"
#include "VP_Channel.h"
#include "NC_SubAudioManager_mock.h"
#include "VP_ChannelManager_mock.h"
#include "VP_DataManager_mock.h"
#include "VP_VoicePlayControl.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;
using ::testing::Return;
using ::testing::_;

/**
 * This is a test class for VP_ChannelManager_Test
 *
 * This class is used to execute a unit test for class
 * VP_ChannelManager_Test
 */
class VP_ChannelManager_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        m_audioMgr = make_shared<MockNC_SubAudioManager>();
        m_pChannelMgr = make_shared<VP_ChannelManager>(m_audioMgr);
    }

    virtual void TearDown()
    {
    }

private:
    shared_ptr<VP_ChannelManager> m_pChannelMgr;
    shared_ptr<MockNC_SubAudioManager> m_audioMgr;
};

/**
 * This is a test class for VP_AudioManagerListener
 *
 * This class is used to execute a unit test for class
 * VP_AudioManagerListener
 */
class VP_AudioManagerListener_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        shared_ptr<MockVP_DataManager> dataMgr = make_shared<MockVP_DataManager>();
        shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(dataMgr, nullptr);
        channelMgr = make_shared<MockVP_ChannelManager>();
        playControl->m_channelMgr = channelMgr;
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_pListener = make_shared<VP_AudioManagerListener>(audioMgr);

        m_pListener->m_pControl = nullptr;
        m_pListener->m_pControl = playControl;
    }

    virtual void TearDown()
    {
    }

private:
    shared_ptr<MockVP_ChannelManager> channelMgr;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_AudioManagerListener> m_pListener;
};

TEST_F(VP_ChannelManager_Test, registerChannel)
{
    EXPECT_CALL(*m_audioMgr, registerAvm(_, _)).WillOnce(Return(true));
    EXPECT_CALL(*m_audioMgr, unregister(_)).WillOnce(Return(true));
    EXPECT_TRUE(m_pChannelMgr->registerChannel("VPTest"));
}

TEST_F(VP_ChannelManager_Test, registerExistedChannel)
{
    EXPECT_CALL(*m_audioMgr, registerAvm(_, _)).WillOnce(Return(true));
    EXPECT_CALL(*m_audioMgr, unregister(_)).WillOnce(Return(true));
    EXPECT_TRUE(m_pChannelMgr->registerChannel("VPTest"));
    EXPECT_FALSE(m_pChannelMgr->registerChannel("VPTest"));
}

TEST_F(VP_ChannelManager_Test, unregisterChannel)
{
    EXPECT_CALL(*m_audioMgr, unregister(_)).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_audioMgr, registerAvm(_, _)).WillOnce(Return(true));

    EXPECT_TRUE(m_pChannelMgr->registerChannel("VPTest"));
    EXPECT_TRUE(m_pChannelMgr->unregisterChannel("VPTest"));
}

TEST_F(VP_ChannelManager_Test, unregisterNonExistedChannel)
{
    EXPECT_CALL(*m_audioMgr, unregister(_)).Times(0);

    EXPECT_FALSE(m_pChannelMgr->unregisterChannel("VPTest"));
}

TEST_F(VP_ChannelManager_Test, unregisterChannles)
{
    m_pChannelMgr->unregisterChannels();
}

TEST_F(VP_ChannelManager_Test, getChannel)
{
    EXPECT_CALL(*m_audioMgr, registerAvm(_, _)).WillOnce(Return(true));
    EXPECT_CALL(*m_audioMgr, unregister(_)).WillOnce(Return(true));

    EXPECT_TRUE(m_pChannelMgr->registerChannel("VP_Test"));
    EXPECT_TRUE(m_pChannelMgr->getChannel("VP_Test") != nullptr);
}

TEST_F(VP_ChannelManager_Test, getNonExistedChannel)
{
    EXPECT_TRUE(m_pChannelMgr->getChannel("VP_Test") == nullptr);
}

TEST_F(VP_ChannelManager_Test, getChannels)
{
    m_pChannelMgr->getChannels();
}

TEST_F(VP_AudioManagerListener_Test, currentAudioSource)
{
    m_pListener->currentAudioSource("VPTest", "VPTest", true);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusOnWithChannelOpen)
{
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", nullptr);
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(channel));
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    NCString status = "on";
    channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusOnWithChannelWaiting)
{
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", nullptr);
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(channel));
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    NCString status = "on";
    channel->setChannelStatus(CHANNEL_STATUS_WAITING);
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusOnWithChannelClose)
{
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", nullptr);
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(channel));
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    NCString status = "on";
    channel->setChannelStatus(CHANNEL_STATUS_CLOSING);
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusOff)
{
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", nullptr);
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(channel));
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    NCString status = "off";
    channel->setChannelStatus(CHANNEL_STATUS_CLOSING);
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusWaiting)
{
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", nullptr);
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(channel));
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    NCString status = "wait";
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusResumed)
{
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", nullptr);
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(channel));
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    NCString status = "continue";
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusStop)
{
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", nullptr);
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(channel));
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    NCString status = "stop";
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusUnknown)
{
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", nullptr);
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(channel));
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    NCString status = "unknown";
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusChanged_WithNullChannel)
{
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(nullptr));

    NCString status = "unknown";
    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, notifyStatusChanged_WithNullChannelManager)
{
    shared_ptr<MockVP_DataManager> dataMgr = make_shared<MockVP_DataManager>();
    shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(dataMgr, nullptr);
    m_pListener->m_pControl = nullptr;
    playControl->m_channelMgr = nullptr;
    m_pListener->m_pControl = playControl;
    NCString status = "unknown";

    m_pListener->notifyStatusChanged("VPTest", status);
}

TEST_F(VP_AudioManagerListener_Test, askAudioStatus)
{
    m_pListener->askAudioStatus("VPTest");
}

TEST_F(VP_AudioManagerListener_Test, onConnected)
{
    m_pListener->onConnected();
}

/* EOF */
