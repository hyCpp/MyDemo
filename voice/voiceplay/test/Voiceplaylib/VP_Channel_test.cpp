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
#include "VP_SoundPlayer.h"
#include "NC_SubAudioManager_mock.h"
#include "VP_SoundPlayer_mock.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;
using ::testing::Return;
using ::testing::_;

/**
 * This is a test class for VP_Channel_Test
 *
 * This class is used to execute a unit test for class
 * VP_Channel_Test
 */
class VP_Channel_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_pVpChannel = make_shared<VP_Channel>("VPTest", audioMgr);
    }

    virtual void TearDown() {}

private:
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_Channel> m_pVpChannel;
};

TEST_F(VP_Channel_Test, reqAudioSource)
{
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));
    m_pVpChannel->reqAudioSource(false);
}

TEST_F(VP_Channel_Test, reqAudioSourceWithNullMgr)
{
    m_pVpChannel->m_audioMgr = nullptr;
    m_pVpChannel->reqAudioSource(false);
}

TEST_F(VP_Channel_Test, setChannelStatus)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_IDLE);
}

TEST_F(VP_Channel_Test, getChannelStatus)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_IDLE);
    VP_CHANNEL_STATUS status = m_pVpChannel->getChannelStatus();
    EXPECT_EQ(status, CHANNEL_STATUS_IDLE);
}

TEST_F(VP_Channel_Test, getChannelName)
{
    string name = m_pVpChannel->getChannelName();
    EXPECT_EQ(name, "VPTest");
}

TEST_F(VP_Channel_Test, queryAudioStatusWithChannelOpened)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_OPENED);
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(0));
    m_pVpChannel->queryAudioStatus();
}

TEST_F(VP_Channel_Test, queryAudioStatusWithChannelOpening)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_OPENING);
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(0));
    m_pVpChannel->queryAudioStatus();

    EXPECT_EQ(m_pVpChannel->m_selfStatus, CHANNEL_STATUS_OPENED);
}

TEST_F(VP_Channel_Test, queryAudioStatusWithChannelWaiting)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_WAITING);
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(0));
    m_pVpChannel->queryAudioStatus();

    EXPECT_EQ(m_pVpChannel->m_selfStatus, CHANNEL_STATUS_OPENED);
}
    
TEST_F(VP_Channel_Test, queryAudioStatusWithChannelClosing)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_CLOSING);
    EXPECT_CALL(*audioMgr, cfmAudioStatus(_, _)).WillOnce(Return(0));
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0)); 
    m_pVpChannel->queryAudioStatus();
}

TEST_F(VP_Channel_Test, queryAudioStatusWithChannelIdle)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_IDLE);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0)); 
    m_pVpChannel->queryAudioStatus();
}

TEST_F(VP_Channel_Test, resetChannelStatusWithChannelOpeningOrOpened)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_OPENING);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));
    m_pVpChannel->resetChannelStatus();
    EXPECT_EQ(m_pVpChannel->m_selfStatus, CHANNEL_STATUS_OPENING);

    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_OPENED);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));
    m_pVpChannel->resetChannelStatus();
    EXPECT_EQ(m_pVpChannel->m_selfStatus, CHANNEL_STATUS_OPENING);
}

TEST_F(VP_Channel_Test, resetChannelStatusWithChannelClosing)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_CLOSING);
    m_pVpChannel->resetChannelStatus();
    EXPECT_EQ(m_pVpChannel->m_selfStatus, CHANNEL_STATUS_IDLE);
}

TEST_F(VP_Channel_Test, resetChannelStatusWithOtherStatus)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_WAITING);
    m_pVpChannel->resetChannelStatus();
    EXPECT_EQ(m_pVpChannel->m_selfStatus, CHANNEL_STATUS_WAITING);
}

TEST_F(VP_Channel_Test, confirmAudioStatusWithOtherName)
{
    m_pVpChannel->m_selfStatus = CHANNEL_STATUS_IDLE;
    m_pVpChannel->confirmAudioStatus("VPTest1", true);
    m_pVpChannel->confirmAudioStatus("VPTest1", false);
}

TEST_F(VP_Channel_Test, confirmAudioStatusWithChannelIdleOrClosing)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_IDLE);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));
    m_pVpChannel->confirmAudioStatus("VPTest", true);

    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_CLOSING);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));
    m_pVpChannel->confirmAudioStatus("VPTest", true);
}

TEST_F(VP_Channel_Test, confirmAudioStatusWithChannelOpeningOrWaiting)
{
    shared_ptr<MockVP_SoundPlayer> player = make_shared<MockVP_SoundPlayer>();
    VP_Factory::Instance()->m_pSoundPlayer = player;
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_OPENING);
    EXPECT_CALL(*player, Play(_));
    m_pVpChannel->confirmAudioStatus("VPTest", true);

    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_WAITING);
    EXPECT_CALL(*player, Play(_));
    m_pVpChannel->confirmAudioStatus("VPTest", true);

    VP_Factory::Instance()->m_pSoundPlayer = nullptr;
}

TEST_F(VP_Channel_Test, confirmAudioStatusWithOtherStatus)
{
    m_pVpChannel->setChannelStatus(CHANNEL_STATUS_OPENED);
    m_pVpChannel->confirmAudioStatus("VPTest", true);
}

/* EOF */
