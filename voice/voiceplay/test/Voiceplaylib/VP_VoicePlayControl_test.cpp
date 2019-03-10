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

#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
#    include "gtest/gtest.h"
#endif

#ifndef GMOCK_INCLUDE_GMOCK_GMOCK_H_
#    include "gmock/gmock.h"
#endif

#include "stdafx.h"
#include "VP_SoundPlayer_mock.h"
#include "NC_SubAudioManager_mock.h"
#include "VP_VoicePlayControl.h"
#include "VP_ChannelManager_mock.h"
#include "VP_Define.h"
#include "VP_DataManager_mock.h"
#include "VP_LanguageSetting.h"

using namespace std;
using ::testing::_;
using ::testing::Return;

/**
 * Class VP_VoicePlayControl_Test.
 *
 * Class for test VP_VoicePlayControl
 *
 */
class VP_VoicePlayControl_Test : public testing::Test
{
public:
    VP_VoicePlayControl_Test() : m_pVP_VoicePlayControl(NULL) 
    {
    }

    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        dataMgr = make_shared<MockVP_DataManager>();
        audioMgr = make_shared<MockNC_SubAudioManager>();

        channelMgr = make_shared<MockVP_ChannelManager>();
        channelMgr->m_audioMgr = audioMgr;

        m_pVP_VoicePlayControl = make_shared<VP_VoicePlayControl>(dataMgr, player);
    }
    
    virtual void TearDown() {}

public:
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<MockVP_DataManager> dataMgr;
    shared_ptr<MockVP_ChannelManager> channelMgr;
    shared_ptr<VP_VoicePlayControl> m_pVP_VoicePlayControl;
};

TEST_F(VP_VoicePlayControl_Test, Initialize)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    CL_ERROR err = m_pVP_VoicePlayControl->Initialize(channelMgr);
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VP_VoicePlayControl_Test, Initialize_NullPlayer)
{
   shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(nullptr, nullptr);
   CL_ERROR err = playControl->Initialize(channelMgr);
   EXPECT_EQ(err, APL_ERR_VP_INVALID_OP);
} 

TEST_F(VP_VoicePlayControl_Test, Initialize_NullDataMgr)
{
    shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(nullptr, player);
    CL_ERROR err = playControl->Initialize(nullptr);
    EXPECT_EQ(err, APL_ERR_VP_INVALID_OP);
}

TEST_F(VP_VoicePlayControl_Test, Initialize_NullChannelMgr)
{
    EXPECT_CALL(*player, Init());
    CL_ERROR err = m_pVP_VoicePlayControl->Initialize(nullptr);
    EXPECT_EQ(m_pVP_VoicePlayControl->m_channelMgr, nullptr);
}  

TEST_F(VP_VoicePlayControl_Test, PostEvent)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    CL_ERROR err = m_pVP_VoicePlayControl->Initialize(channelMgr);

    shared_ptr<VP_Event> event = make_shared<VP_PlayerStopEvent>(nullptr);
    err = m_pVP_VoicePlayControl->PostEvent(event);
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
    EXPECT_EQ(m_pVP_VoicePlayControl->m_eventQueue.Size(), 1);
}

TEST_F(VP_VoicePlayControl_Test, PostNullEvent)
{
    CL_ERROR err = m_pVP_VoicePlayControl->PostEvent(nullptr);
    EXPECT_EQ(err, APL_ERR_VP_INVALID_PARAM);
}

TEST_F(VP_VoicePlayControl_Test, PostEventAfterQuit)
{
    m_pVP_VoicePlayControl->m_quited = CL_TRUE;
    shared_ptr<VP_Event> event = make_shared<VP_PlayerStopEvent>(nullptr);
    CL_ERROR err = m_pVP_VoicePlayControl->PostEvent(event);
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
    EXPECT_EQ(m_pVP_VoicePlayControl->m_eventQueue.Size(), 0);
}

TEST_F(VP_VoicePlayControl_Test, RunWithoutQuit)
{
    m_pVP_VoicePlayControl->m_quited = CL_FALSE;
    m_pVP_VoicePlayControl->Run();
}

TEST_F(VP_VoicePlayControl_Test, getChannelManager)
{
    m_pVP_VoicePlayControl->getChannelManager();
}

TEST_F(VP_VoicePlayControl_Test, getChannelWithNullChannelManager)
{
    m_pVP_VoicePlayControl->m_channelMgr = nullptr;
    shared_ptr<VP_Channel> channel = m_pVP_VoicePlayControl->getChannel("VPTest");
    EXPECT_EQ(channel, nullptr);
}

TEST_F(VP_VoicePlayControl_Test, getDefaultChannel)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    m_pVP_VoicePlayControl->Initialize(channelMgr);

    shared_ptr<VP_Channel> channel = m_pVP_VoicePlayControl->getChannel("VPTest");
    EXPECT_EQ(channel, nullptr);
} 

TEST_F(VP_VoicePlayControl_Test, getChannel_ForPoint)
{

    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    m_pVP_VoicePlayControl->Initialize(channelMgr);

    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("0x58.NAVNotification", nullptr)));
    shared_ptr<VP_Channel> channel = m_pVP_VoicePlayControl->getChannel("Point");
    EXPECT_EQ(channel->getChannelName(), "0x58.NAVNotification");
}

TEST_F(VP_VoicePlayControl_Test, getChannel_ForGuide)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    m_pVP_VoicePlayControl->Initialize(channelMgr);

    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("0x58.RouteGuidance", nullptr)));
    shared_ptr<VP_Channel> channel = m_pVP_VoicePlayControl->getChannel("Guide");
    EXPECT_EQ(channel->getChannelName(), "0x58.RouteGuidance");
} 

TEST_F(VP_VoicePlayControl_Test, getChannel_ForUI)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    m_pVP_VoicePlayControl->Initialize(channelMgr);

    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("0x58.NAVNotification", nullptr)));
    shared_ptr<VP_Channel> channel = m_pVP_VoicePlayControl->getChannel("UI");
    EXPECT_EQ(channel->getChannelName(), "0x58.NAVNotification");
} 

TEST_F(VP_VoicePlayControl_Test, getChannel_ForTraffic)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    m_pVP_VoicePlayControl->Initialize(channelMgr);

    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("0x58.TrafficInformationGuidanceMode", nullptr)));
    shared_ptr<VP_Channel> channel = m_pVP_VoicePlayControl->getChannel("Traffic");
    EXPECT_EQ(channel->getChannelName(), "0x58.TrafficInformationGuidanceMode");
}

TEST_F(VP_VoicePlayControl_Test, getChannel_ForPath)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    m_pVP_VoicePlayControl->Initialize(channelMgr);

    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("0x58.RouteGuidance", nullptr)));
    shared_ptr<VP_Channel> channel = m_pVP_VoicePlayControl->getChannel("Path");
    EXPECT_EQ(channel->getChannelName(), "0x58.RouteGuidance");
}

TEST_F(VP_VoicePlayControl_Test, getChannel_WithEmptySender)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    m_pVP_VoicePlayControl->Initialize(channelMgr);

    shared_ptr<VP_Channel> channel = m_pVP_VoicePlayControl->getChannel("");
    EXPECT_EQ(channel, nullptr);
}

TEST_F(VP_VoicePlayControl_Test, StopAllWithEmptyChannelList)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());

    m_pVP_VoicePlayControl->Initialize(channelMgr);
    player->setPlayerStatus(PLAYER_STATUS_IDLE);
    m_pVP_VoicePlayControl->StopAll();
}

TEST_F(VP_VoicePlayControl_Test, StopAllWithChannelClosed)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());

    m_pVP_VoicePlayControl->Initialize(channelMgr);
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", audioMgr);
    channelMgr->m_channels_list.push_back(channel);
    channel->setChannelStatus(CHANNEL_STATUS_CLOSED);
    player->setPlayerStatus(PLAYER_STATUS_IDLE);
    m_pVP_VoicePlayControl->StopAll();
    channelMgr->m_channels_list.pop_back();
}

TEST_F(VP_VoicePlayControl_Test, StopAllWithChannelOpened)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    m_pVP_VoicePlayControl->Initialize(channelMgr);
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", audioMgr);
    channelMgr->m_channels_list.push_back(channel);
    channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    player->setPlayerStatus(PLAYER_STATUS_IDLE);
    m_pVP_VoicePlayControl->StopAll();
    channelMgr->m_channels_list.pop_back();
}

TEST_F(VP_VoicePlayControl_Test, StopAllWithChannelOpening)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));

    m_pVP_VoicePlayControl->Initialize(channelMgr);
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", audioMgr);
    channelMgr->m_channels_list.push_back(channel);
    channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    player->setPlayerStatus(PLAYER_STATUS_IDLE);
    m_pVP_VoicePlayControl->StopAll();
    channelMgr->m_channels_list.pop_back();
}

TEST_F(VP_VoicePlayControl_Test, StopAllWithChannelWaiting)
{
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*player, Init());
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*player, Stop());

    m_pVP_VoicePlayControl->Initialize(channelMgr);
    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>("VPTest", audioMgr);
    channelMgr->m_channels_list.push_back(channel);
    channel->setChannelStatus(CHANNEL_STATUS_WAITING);
    player->setPlayerStatus(PLAYER_STATUS_PLAYING);
    m_pVP_VoicePlayControl->StopAll();
    channelMgr->m_channels_list.pop_back();
}

/* EOF */
