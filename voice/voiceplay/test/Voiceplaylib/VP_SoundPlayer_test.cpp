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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#ifndef CXX_BL_ERROR_H
#	include "BL_Error.h"
#endif

#include "MEM_list.h"
#include "MEM_vector.h"
#include "VP_Define.h"
#include "VP_SoundPlayer.h"
#include "VP_DataAccessorList.h"
#include "VC_CommonIF.h"
#include "VC_Player.h"
#include "VP_DataAccessor_mock.h"
#include "VC_Player_mock.h"
#include "VP_ChannelManager_mock.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using namespace std;

/**
 * Summary for class VP_SoundPlayer_Test
 * 
 * This is a detail description for class VP_SoundPlayer_Test
 *
 */
class VP_SoundPlayer_Test:public testing::Test
{
public:
    virtual void SetUp()
    {
        player = new(MEM_Voice) MockVC_Player();
        m_pVP_SoundPlayer = make_shared<VP_SoundPlayer>(player);

        pList = make_shared<MockVP_DataAccessorList>();
        pList->sender = "Guide";
        m_pVP_SoundPlayer->m_playList = pList;

        shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(nullptr, nullptr);
        channelMgr = make_shared<MockVP_ChannelManager>();
        playControl->m_channelMgr = channelMgr;
        VP_Factory::Instance()->m_pControl = nullptr;
        VP_Factory::Instance()->m_pControl = playControl;
    }

    virtual void TearDown() 
    {
        if (NULL != player) {
            delete player;
            player = NULL;
        }
        VP_Factory::Instance()->m_pControl = nullptr;
    }
        
public:
    MockVC_Player *player;
    shared_ptr<MockVP_ChannelManager> channelMgr;
    shared_ptr<VP_SoundPlayer> m_pVP_SoundPlayer;
    shared_ptr<MockVP_DataAccessorList> pList;
};

TEST_F(VP_SoundPlayer_Test, Initialize)
{
    CL_ERROR err = m_pVP_SoundPlayer->Initialize();
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VP_SoundPlayer_Test, CreateStopMsg)
{
    bool ret = m_pVP_SoundPlayer->CreateStopMsg();
    EXPECT_TRUE(ret);
}

TEST_F(VP_SoundPlayer_Test, Stop)
{
    EXPECT_CALL(*player, Stop()).Times(AtLeast(1));
    m_pVP_SoundPlayer->m_state = PLAYER_STATUS_PLAYING;
    m_pVP_SoundPlayer->m_bIsTtsPlay = CL_FALSE;
    m_pVP_SoundPlayer->Stop();

    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_STOPPING);
}

TEST_F(VP_SoundPlayer_Test, Stop_WithTTS)
{
    EXPECT_CALL(*player, Stop()).Times(AtLeast(1));
    m_pVP_SoundPlayer->m_state = PLAYER_STATUS_PLAYING;
    m_pVP_SoundPlayer->m_bIsTtsPlay = CL_TRUE;
    m_pVP_SoundPlayer->Stop();

    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_STOPPING);
}

TEST_F(VP_SoundPlayer_Test, StopNullPlayList)
{
    m_pVP_SoundPlayer->m_playList = nullptr;
    m_pVP_SoundPlayer->m_state = PLAYER_STATUS_PLAYING;
    m_pVP_SoundPlayer->Stop();

    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_PLAYING);
}

TEST_F(VP_SoundPlayer_Test, StopAbortedPlayList)
{
    m_pVP_SoundPlayer->m_playList->setAbort(true);
    m_pVP_SoundPlayer->m_state = PLAYER_STATUS_PLAYING;
    m_pVP_SoundPlayer->Stop();

    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_PLAYING); 
}

TEST_F(VP_SoundPlayer_Test, StopWrongState)
{
    m_pVP_SoundPlayer->m_state = PLAYER_STATUS_STOPPING;
    m_pVP_SoundPlayer->Stop();

    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_STOPPING);
}

TEST_F(VP_SoundPlayer_Test, StopWithReadyState)
{
    m_pVP_SoundPlayer->m_state = PLAYER_STATUS_READY;
    m_pVP_SoundPlayer->Stop();

    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_STOPPING);
}

TEST_F(VP_SoundPlayer_Test, Play)
{
    EXPECT_CALL(*pList, Play(_, _, _, _, _)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*pList, HasNext()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));

    m_pVP_SoundPlayer->Play();
    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_STOPPING);
}

TEST_F(VP_SoundPlayer_Test, PlayError)
{
    EXPECT_CALL(*pList, Play(_, _, _, _, _)).WillOnce(Return(APL_ERR_VP_AUDIO_PLAY));
    EXPECT_CALL(*pList, HasNext()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));

    m_pVP_SoundPlayer->Play();
    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_STOPPING);
}

TEST_F(VP_SoundPlayer_Test, PlayEmptyList)
{
    EXPECT_CALL(*pList, HasNext()).WillOnce(Return(false));
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));
            
    m_pVP_SoundPlayer->Play();
    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_STOPPING);
}

TEST_F(VP_SoundPlayer_Test, PlayNullList)
{
    m_pVP_SoundPlayer->m_playList = nullptr;
    m_pVP_SoundPlayer->Play();

    EXPECT_EQ(m_pVP_SoundPlayer->m_state, PLAYER_STATUS_IDLE);
}

TEST_F(VP_SoundPlayer_Test, PlayAbortedList)
{
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));
    
    m_pVP_SoundPlayer->m_playList->setAbort(true);
    m_pVP_SoundPlayer->Play();
}    

TEST_F(VP_SoundPlayer_Test, PlayFailed)
{
    EXPECT_CALL(*pList, Play(_, _, _, _, _)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*pList, HasNext()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));

    m_pVP_SoundPlayer->m_bResultState = CL_FALSE;
    m_pVP_SoundPlayer->Play();
}

TEST_F(VP_SoundPlayer_Test, PlayFailed1)
{
    EXPECT_CALL(*pList, Play(_, _, _, _, _)).WillOnce(Return(APL_ERR_VP_AUDIO_PLAY));
    EXPECT_CALL(*pList, HasNext()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));

    m_pVP_SoundPlayer->m_bResultState = CL_FALSE;
    m_pVP_SoundPlayer->Play();
}

TEST_F(VP_SoundPlayer_Test, notifyStoppedState)
{
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", nullptr)));
    m_pVP_SoundPlayer->notifyStoppedState(m_pVP_SoundPlayer->m_playList);
}

TEST_F(VP_SoundPlayer_Test, notifyStoppedStateWithNullChannel)
{
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(nullptr));
    m_pVP_SoundPlayer->notifyStoppedState(m_pVP_SoundPlayer->m_playList);
}

TEST_F(VP_SoundPlayer_Test, OnPlayComplete)
{
    m_pVP_SoundPlayer->OnPlayComplete(CL_TRUE);
}

TEST_F(VP_SoundPlayer_Test, OnPlayCompleteTts)
{
    m_pVP_SoundPlayer->OnPlayComplete(CL_TRUE, 0);
}

TEST_F(VP_SoundPlayer_Test, OnPlayComplete_SameID)
{
    m_pVP_SoundPlayer->m_currentReqId = 1;
    m_pVP_SoundPlayer->OnPlayComplete(CL_TRUE, 1);
}

TEST_F(VP_SoundPlayer_Test, Init)
{
    EXPECT_CALL(*player, Init());
    m_pVP_SoundPlayer->Init();
}

TEST_F(VP_SoundPlayer_Test, Init_NullCheck)
{
    m_pVP_SoundPlayer->m_pPlayer = nullptr;
    m_pVP_SoundPlayer->Init();
}

TEST_F(VP_SoundPlayer_Test, ReqIdCreater)
{
    VP_SoundPlayer::m_reqId = 0;
    EXPECT_EQ(m_pVP_SoundPlayer->ReqIdCreater(), 0);
}

TEST_F(VP_SoundPlayer_Test, StopPlayerThread)
{
    EXPECT_CALL(*player, StopPlayer());
    m_pVP_SoundPlayer->StopPlayerThread();
}

TEST_F(VP_SoundPlayer_Test, ReadPlayerStatus)
{
    m_pVP_SoundPlayer->setPlayerStatus(PLAYER_STATUS_IDLE);
    EXPECT_EQ(m_pVP_SoundPlayer->getPlayerStatus(), PLAYER_STATUS_IDLE);
}

TEST_F(VP_SoundPlayer_Test, postResult)
{
    VoiceVector<shared_ptr<VP_DataAccessor>>::type dataList;
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>(dataList, "VPTest", 0);
    VP_PLAY_RESULT playResult = VP_PLAYRESULT_OK;
    m_pVP_SoundPlayer->postResult(pList, playResult);
}

TEST_F(VP_SoundPlayer_Test, postNullResult)
{
    m_pVP_SoundPlayer->postResult(nullptr, VP_PLAYRESULT_OK);
}

TEST_F(VP_SoundPlayer_Test, SetTtsPlayEnd)
{
    m_pVP_SoundPlayer->m_currentReqId = -1;
    m_pVP_SoundPlayer->onTtsServiceDown();

    m_pVP_SoundPlayer->m_currentReqId = 0;
    m_pVP_SoundPlayer->m_bIsTtsPlay = false;
    m_pVP_SoundPlayer->onTtsServiceDown();

    m_pVP_SoundPlayer->m_bIsTtsPlay = true;
    m_pVP_SoundPlayer->onTtsServiceDown();
}

/* EOF */
