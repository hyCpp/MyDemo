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

#include <typeinfo>

#include "VP_Event.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "stdafx.h"
#include "NC_SubAudioManager_mock.h"
#include "VP_SoundPlayer_mock.h"
#include "VC_Player_mock.h"
#include "VP_PhraseImp_mock.h"
#include "VP_Channel.h"
#include "VP_ChannelManager_mock.h"

using namespace std;
using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;

/**
 * This is a test class for VP_PlayerStoppedEvent
 *
 * This class is used to execute a unit test for class
 * VP_PlayerStoppedEvent
 */
class VP_PlayerStoppedEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_channel = make_shared<VP_Channel>("VPTest", audioMgr);
        m_event = make_shared<VP_PlayerStoppedEvent>(m_channel);
        VP_Factory::Instance()->m_pSoundPlayer = player;
    }

    virtual void TearDown() 
    {
        VP_Factory::Instance()->m_pSoundPlayer = nullptr;
    }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_Channel> m_channel;
    shared_ptr<VP_PlayerStoppedEvent> m_event;
};

/**
 * This is a test class for VP_PlayerStopEvent
 *
 * This class is used to execute a unit test for class
 * VP_PlayerStopEvent
 */
class VP_PlayerStopEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_channel = make_shared<VP_Channel>("VPTest", audioMgr);
        m_event = make_shared<VP_PlayerStopEvent>(m_channel);
        VP_Factory::Instance()->m_pSoundPlayer = player;
    }

    virtual void TearDown() 
    {
        VP_Factory::Instance()->m_pSoundPlayer = nullptr; 
    }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_Channel> m_channel;
    shared_ptr<VP_PlayerStopEvent> m_event;
};

/**
 * This is a test class for VP_ChannelOpenEvent
 *
 * This class is used to execute a unit test for class
 * VP_ChannelOpenEvent
 */
class VP_ChannelOpenEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();

        m_channel = make_shared<VP_Channel>("VPTest", audioMgr);
        m_event = make_shared<VP_ChannelOpenEvent>(m_channel, nullptr);
        VP_Factory::Instance()->m_pSoundPlayer = player;
    }

    virtual void TearDown() 
    {
        VP_Factory::Instance()->m_pSoundPlayer = nullptr;
    }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_Channel> m_channel;
    shared_ptr<VP_ChannelOpenEvent> m_event;
};

/**
 * This is a test class for VP_ChannelOpenedEvent
 *
 * This class is used to execute a unit test for class
 * VP_ChannelOpenedEvent
 */
class VP_ChannelOpenedEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_channel = make_shared<VP_Channel>("VPTest", audioMgr);
        m_event = make_shared<VP_ChannelOpenedEvent>(m_channel);
        VP_Factory::Instance()->m_pSoundPlayer = player;
    }

   virtual void TearDown() 
   {
        VP_Factory::Instance()->m_pSoundPlayer = nullptr;
   }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_Channel> m_channel;
    shared_ptr<VP_ChannelOpenedEvent> m_event;
};

/**
 * This is a test class for VP_ChannelClosedEvent
 *
 * This class is used to execute a unit test for class
 * VP_ChannelClosedEvent
 */
class VP_ChannelClosedEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_channel = make_shared<VP_Channel>("VPTest", audioMgr);
        m_event = make_shared<VP_ChannelClosedEvent>(m_channel);
        VP_Factory::Instance()->m_pSoundPlayer = player;
    }

    virtual void TearDown()
    {
        VP_Factory::Instance()->m_pSoundPlayer = nullptr;
    }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_Channel> m_channel;
    shared_ptr<VP_ChannelClosedEvent> m_event;
};

/**
 * This is a test class for VP_ChannelWaitingEvent
 *
 * This class is used to execute a unit test for class
 * VP_ChannelWaitingEvent
 */
class VP_ChannelWaitingEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_channel = make_shared<VP_Channel>("VPTest", audioMgr);
        m_event = make_shared<VP_ChannelWaitingEvent>(m_channel);
        VP_Factory::Instance()->m_pSoundPlayer = player;
    }

    virtual void TearDown()
    {
        VP_Factory::Instance()->m_pSoundPlayer = nullptr;
    }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_Channel> m_channel;
    shared_ptr<VP_ChannelWaitingEvent> m_event;
};

/**
 * This is a test class for VP_ChannelStopEvent
 *
 * This class is used to execute a unit test for class
 * VP_ChannelStopEvent
 */
class VP_ChannelStopEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_channel = make_shared<VP_Channel>("VPTest", audioMgr);
        m_event = make_shared<VP_ChannelStopEvent>(m_channel);
        VP_Factory::Instance()->m_pSoundPlayer = player;
    }

    virtual void TearDown()
    {
        VP_Factory::Instance()->m_pSoundPlayer = nullptr;
    }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_Channel> m_channel;
    shared_ptr<VP_ChannelStopEvent> m_event;
};

/**
 * This is a test class for VP_SetMuteEvent
 *
 * This class is used to execute a unit test for class
 * VP_SetMuteEvent
 */
class VP_SetMuteEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();
        m_channel = make_shared<VP_Channel>("VPTest", audioMgr);
        channelMgr = make_shared<MockVP_ChannelManager>();
        playControl = make_shared<VP_VoicePlayControl>(nullptr, player);

        playControl->m_channelMgr = channelMgr;
        VP_Factory::Instance()->m_pControl = playControl;
        m_event = make_shared<VP_SetMuteEvent>();
    }

    virtual void TearDown()
    {
        m_channel->m_audioMgr = nullptr;
        audioMgr = nullptr;
        m_channel = nullptr;
        playControl->m_pSoundPlayer = nullptr;
        playControl = nullptr;
    }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<VP_VoicePlayControl> playControl;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<MockVP_ChannelManager> channelMgr;
    shared_ptr<VP_Channel> m_channel;
    shared_ptr<VP_SetMuteEvent> m_event;
};

/**
 * This is a test class for VP_ChannelConfirmedEvent
 *
 * This class is used to execute a unit test for class
 * VP_ChannelConfirmedEvent
 */
class VP_ChannelConfirmedEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        channelMgr = make_shared<MockVP_ChannelManager>();
        playControl = make_shared<VP_VoicePlayControl>(nullptr, nullptr);

        playControl->m_channelMgr = channelMgr;
        VP_Factory::Instance()->m_pControl = playControl;
        m_event = make_shared<VP_ChannelConfirmedEvent>("VPTest", "VPTest", false);
    }

    virtual void TearDown()
    {
        playControl->m_channelMgr = nullptr;
        channelMgr = nullptr;
        playControl = nullptr;
    }

private:
    shared_ptr<VP_VoicePlayControl> playControl;
    shared_ptr<MockVP_ChannelManager> channelMgr;
    shared_ptr<VP_ChannelConfirmedEvent> m_event;
};

/**
 * This is a test class for VP_ChannelQueryStatusEvent
 *
 * This class is used to execute a unit test for class
 * VP_ChannelQueryStatusEvent
 */
class VP_ChannelStatusQueryEvent_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        channelMgr = make_shared<MockVP_ChannelManager>();
        playControl = make_shared<VP_VoicePlayControl>(nullptr, nullptr);

        playControl->m_channelMgr = channelMgr;
        VP_Factory::Instance()->m_pControl = playControl;
        m_event = make_shared<VP_ChannelStatusQueryEvent>("VPTest");
    }

    virtual void TearDown()
    {
        playControl->m_channelMgr = nullptr;
        channelMgr = nullptr;
        playControl = nullptr;
    }

private:
    shared_ptr<VP_VoicePlayControl> playControl;
    shared_ptr<MockVP_ChannelManager> channelMgr;
    shared_ptr<VP_ChannelStatusQueryEvent> m_event;
};

/**
 * This is a test class for VP_PlayListEvent
 *
 * This class is used to execute a unit test for class
 * VP_PlayListEvent
 */
class VP_PlayListEvent_Test : public ::testing::Test
{
public:
   virtual void SetUp()
   {
        VoiceList<std::shared_ptr<VP_PhraseImp>>::type pList;
        audioMgr = make_shared<MockNC_SubAudioManager>();

        m_event = make_shared<VP_PlayListEvent>(pList, VP_SS_DEFAULT_ID, "Guide", 0);
   }

   virtual void TearDown()
   {
        audioMgr = nullptr;
   }

private:
    shared_ptr<VP_PlayListEvent> m_event;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
};

/**
 * This is a test class for VP_GetDurationEvent
 *
 * This class is used to execute a unit test for class
 * VP_GetDurationEvent
 */
class VP_GetDurationEvent_Test : public ::testing::Test
{
public:
   virtual void SetUp()
   {
        VoiceList<std::shared_ptr<VP_PhraseImp>>::type pList;
        m_event = make_shared<VP_GetDurationEvent>(pList, "Guide", 0);
   }

   virtual void TearDown()
   {
   }

private:
    shared_ptr<VP_GetDurationEvent> m_event;
};

/**
 * This is a test class for VP_LanguageChangeEvent
 *
 * This class is used to execute a unit test for class
 * VP_LanguageChangeEvent
 */
class VP_LanguageChangeEvent_Test : public ::testing::Test
{
public:
   virtual void SetUp()
   {
        player = make_shared<MockVP_SoundPlayer>();
        audioMgr = make_shared<MockNC_SubAudioManager>();
        channelMgr = make_shared<VP_ChannelManager>(audioMgr);
        playControl = make_shared<VP_VoicePlayControl>(nullptr, player);
        playControl->m_channelMgr = channelMgr;

        VP_Factory::Instance()->m_pControl = playControl;
        m_event = make_shared<VP_LanguageChangeEvent>();
   }

   virtual void TearDown()
   {
        playControl->m_channelMgr = nullptr;
        playControl->m_pSoundPlayer = nullptr;
        channelMgr->m_audioMgr = nullptr;
        player = nullptr;
        audioMgr = nullptr;
        channelMgr = nullptr;
        playControl = nullptr;
   }

private:
    shared_ptr<MockVP_SoundPlayer> player;
    shared_ptr<VP_ChannelManager> channelMgr;
    shared_ptr<VP_VoicePlayControl> playControl;
    shared_ptr<MockNC_SubAudioManager> audioMgr;
    shared_ptr<VP_LanguageChangeEvent> m_event;
};

TEST_F(VP_PlayListEvent_Test, SetPhraseInfos_WithEmptyList)
{
    m_event->m_phraseList.clear();
    CL_ERROR err = m_event->SetPhraseInfos();
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VP_PlayListEvent_Test, SetPhraseInfos_WithNonExistedPhrase)
{
    shared_ptr<MockVP_PhraseImp> phrase = make_shared<MockVP_PhraseImp>();
    m_event->m_phraseList.clear();
    m_event->m_phraseList.push_back(phrase);

    EXPECT_CALL(*phrase, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_VP_PHRASE_NOTEXIST));
    CL_ERROR error = m_event->SetPhraseInfos();
    EXPECT_EQ(error, APL_ERR_VP_PHRASE_NOTEXIST);
}

TEST_F(VP_PlayListEvent_Test, SetPhraseInfos_WithPhraseNotCanceled)
{
    m_event->m_phraseList.clear();
    shared_ptr<MockVP_PhraseImp> phrase1 = make_shared<MockVP_PhraseImp>();
    shared_ptr<MockVP_PhraseImp> phrase2 = make_shared<MockVP_PhraseImp>();
    m_event->m_phraseList.push_back(phrase1);
    m_event->m_phraseList.push_back(phrase2);

    EXPECT_CALL(*phrase1, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*phrase2, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_VP_PHRASE_NOTEXIST));
    EXPECT_CALL(*phrase2, CanBeCancelled()).WillOnce(Return(CL_FALSE));
    CL_ERROR error = m_event->SetPhraseInfos();
    EXPECT_EQ(error, APL_ERR_VP_PHRASE_NOTEXIST);
}

TEST_F(VP_PlayListEvent_Test, SetPhraseInfos_WithPhraseCanceled)
{
    m_event->m_phraseList.clear();
    shared_ptr<MockVP_PhraseImp> phrase1 = make_shared<MockVP_PhraseImp>();
    shared_ptr<MockVP_PhraseImp> phrase2 = make_shared<MockVP_PhraseImp>();
    m_event->m_phraseList.push_back(phrase1);
    m_event->m_phraseList.push_back(phrase2);

    EXPECT_CALL(*phrase1, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*phrase2, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_VP_PHRASE_NOTEXIST));
    EXPECT_CALL(*phrase2, CanBeCancelled()).WillOnce(Return(CL_TRUE));
    CL_ERROR error = m_event->SetPhraseInfos();
    EXPECT_EQ(error, APL_ERR_VP_PHRASE_NOTEXIST);
}

TEST_F(VP_PlayListEvent_Test, SetPhraseInfos_WithPhraseOtherErrors)
{
    m_event->m_phraseList.clear();
    shared_ptr<MockVP_PhraseImp> phrase1 = make_shared<MockVP_PhraseImp>();
    shared_ptr<MockVP_PhraseImp> phrase2 = make_shared<MockVP_PhraseImp>();
    m_event->m_phraseList.push_back(phrase1);
    m_event->m_phraseList.push_back(phrase2);

    EXPECT_CALL(*phrase1, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*phrase2, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_VP_INVALID_PARAM));
    CL_ERROR error = m_event->SetPhraseInfos();
    EXPECT_EQ(error, APL_ERR_VP_INVALID_PARAM);
}

TEST_F(VP_PlayListEvent_Test, WrapNullDataAccessor)
{
    VoiceVector<shared_ptr<VP_DataAccessor>>::type pList;
    shared_ptr<MockVP_PhraseImp> phrase = make_shared<MockVP_PhraseImp>();
    m_event->m_phraseList.clear();
    m_event->m_phraseList.push_back(phrase);
    EXPECT_CALL(*phrase, CreateDataAccessor()).WillOnce(Return(nullptr));
    EXPECT_CALL(*phrase, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_NO_ERROR));

    bool result = m_event->WrapDataAccessor(pList);
    EXPECT_FALSE(result);
}

TEST_F(VP_PlayListEvent_Test, WrapCancelledDataAccessor)
{
    VoiceVector<shared_ptr<VP_DataAccessor>>::type pList;
    shared_ptr<MockVP_PhraseImp> phrase = make_shared<MockVP_PhraseImp>();
    m_event->m_phraseList.clear();
    m_event->m_phraseList.push_back(phrase);
    phrase->SetCancelled();
    EXPECT_CALL(*phrase, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_NO_ERROR));

    bool result = m_event->WrapDataAccessor(pList);
    EXPECT_TRUE(result);
}

TEST_F(VP_PlayListEvent_Test, DoActionWithMute)
{
    shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(nullptr, nullptr);
    shared_ptr<MockVP_ChannelManager> channelMgr = make_shared<MockVP_ChannelManager>();
    playControl->m_channelMgr = channelMgr;
    VP_Factory::Instance()->m_pControl = playControl;

    playControl->setMute(true);
    m_event->DoAction();
}

TEST_F(VP_PlayListEvent_Test, DoAction)
{
    shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(nullptr, nullptr);
    shared_ptr<MockVP_ChannelManager> channelMgr = make_shared<MockVP_ChannelManager>();
    playControl->m_channelMgr = channelMgr;
    VP_Factory::Instance()->m_pControl = playControl;
    shared_ptr<MockVP_PhraseImp> phrase = make_shared<MockVP_PhraseImp>();

    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));
    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(make_shared<VP_Channel>("VPTest", audioMgr)));
    EXPECT_CALL(*phrase, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*phrase, CreateDataAccessor()).WillOnce(Return(make_shared<VP_DataAccessorTts>("Tts", "ENG")));

    m_event->m_phraseList.clear();
    m_event->m_phraseList.push_back(phrase);
    m_event->DoAction();

    playControl->m_channelMgr = nullptr;
    channelMgr = nullptr;
    playControl = nullptr;
    phrase = nullptr;
}

TEST_F(VP_PlayListEvent_Test, DoActionWithNullChannel)
{
    shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(nullptr, nullptr);
    shared_ptr<MockVP_ChannelManager> channelMgr = make_shared<MockVP_ChannelManager>();
    playControl->m_channelMgr = channelMgr;
    VP_Factory::Instance()->m_pControl = playControl;

    EXPECT_CALL(*channelMgr, getChannel(_)).WillOnce(Return(nullptr));
    m_event->DoAction();

    playControl->m_channelMgr = nullptr;
    channelMgr = nullptr;
    playControl = nullptr;
}

TEST_F(VP_PlayerStoppedEvent_Test, DoAction)
{
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(pList);
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    EXPECT_CALL(*player, Play(_)).Times(AtLeast(1));

    shared_ptr<VP_Event> event = m_event->DoAction();
}

TEST_F(VP_PlayerStoppedEvent_Test, DoAction_NullList)
{
    m_channel->setWaitPlayList(nullptr);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);

    m_event->DoAction();
}

TEST_F(VP_PlayerStoppedEvent_Test, DoAction_PassiveCloseChannel)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_CLOSED);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(player->getPlayerStatus(), PLAYER_STATUS_IDLE);
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_IDLE);
}

TEST_F(VP_PlayerStoppedEvent_Test, DoActionWithOtherStatus)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    m_event->DoAction();
}

TEST_F(VP_PlayerStopEvent_Test, DoAction)
{
    EXPECT_CALL(*player, Stop()).Times(AtLeast(1));
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(event, nullptr);
}

TEST_F(VP_PlayerStopEvent_Test, DoAction2)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    player->setPlayerStatus(PLAYER_STATUS_IDLE);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(event, nullptr);
}

TEST_F(VP_PlayerStopEvent_Test, DoAction3)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    player->setPlayerStatus(PLAYER_STATUS_STOPPING);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(event, nullptr);
}

TEST_F(VP_PlayerStopEvent_Test, DoAction4)
{
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(pList);

    m_channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    player->setPlayerStatus(PLAYER_STATUS_IDLE);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(event, nullptr);
}

TEST_F(VP_ChannelOpenEvent_Test, DoAction)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(event, nullptr);
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_OPENING);
}

TEST_F(VP_ChannelOpenEvent_Test, DoAction2)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    player->setPlayerStatus(PLAYER_STATUS_PLAYING);
    EXPECT_CALL(*player, Stop());

    m_event->DoAction();
}

TEST_F(VP_ChannelOpenEvent_Test, DoAction3)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(event, nullptr);
}

TEST_F(VP_ChannelOpenEvent_Test, DoAction4)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    player->setPlayerStatus(PLAYER_STATUS_UNKNOWN);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(event, nullptr);
}

TEST_F(VP_ChannelOpenEvent_Test, DoAction5)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    player->setPlayerStatus(PLAYER_STATUS_READY);
    EXPECT_CALL(*player, Stop());

    m_event->DoAction();
}

TEST_F(VP_ChannelOpenedEvent_Test, DoAction1)
{
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(pList);
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENING);

    EXPECT_CALL(*player, Play(_)).Times(AtLeast(1));
    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_OPENED);
}

TEST_F(VP_ChannelOpenedEvent_Test, DoAction2)
{
    m_channel->setWaitPlayList(nullptr);
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));

    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_CLOSING);
}


TEST_F(VP_ChannelOpenedEvent_Test, DoAction3)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));

    m_event->DoAction();
}

TEST_F(VP_ChannelOpenedEvent_Test, DoAction4)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_CLOSING);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));

    m_event->DoAction();
}

TEST_F(VP_ChannelOpenedEvent_Test, DoAction5)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);

    m_event->DoAction();
}

TEST_F(VP_ChannelOpenedEvent_Test, resumePlay)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_WAITING);
    shared_ptr<VP_DataAccessorList> playList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(playList);
    EXPECT_CALL(*player, Play(_));

    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_OPENED);
}

TEST_F(VP_ChannelOpenedEvent_Test, resumePlayWithoutPlayList)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_WAITING);
    m_channel->setWaitPlayList(nullptr);
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(0));

    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_CLOSING);
}

TEST_F(VP_ChannelClosedEvent_Test, DoActionWithChannelOpening)
{
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(pList);

    m_channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_IDLE);
}

TEST_F(VP_ChannelClosedEvent_Test, DoActionWithChannelClosing)
{
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(pList);

    m_channel->setChannelStatus(CHANNEL_STATUS_CLOSING);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_IDLE);
    EXPECT_TRUE(event != nullptr);
}

TEST_F(VP_ChannelClosedEvent_Test, DoActionWithChannelClosingAndNullPlayList)
{
    m_channel->setWaitPlayList(nullptr);

    m_channel->setChannelStatus(CHANNEL_STATUS_CLOSING);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_IDLE);
    EXPECT_EQ(event, nullptr);
}

TEST_F(VP_ChannelClosedEvent_Test, DoActionWithChannelOpened)
{
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(pList);
    EXPECT_CALL(*player, Stop());

    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_IDLE);
}

TEST_F(VP_ChannelClosedEvent_Test, DoActionWithChannelOpenedAndPlayerStopping)
{
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(pList);
    EXPECT_CALL(*player, Stop());

    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    player->setPlayerStatus(PLAYER_STATUS_STOPPING);
    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_CLOSED);
}

TEST_F(VP_ChannelClosedEvent_Test, DoActionWithOtherStatus)
{
    shared_ptr<VP_DataAccessorList> pList = make_shared<VP_DataAccessorList>();
    m_channel->setWaitPlayList(pList);

    m_channel->setChannelStatus(CHANNEL_STATUS_CLOSED);
    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_CLOSED);
    pList = nullptr;
}

TEST_F(VP_ChannelWaitingEvent_Test, DoAction_WithChannelOpening)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    shared_ptr<VP_DataAccessorList> playList;
    m_channel->setWaitPlayList(playList);
    m_event->DoAction();
    EXPECT_EQ(m_channel->getWaitPlayList(), playList);
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_WAITING);
}

TEST_F(VP_ChannelWaitingEvent_Test, DoAction_WithChannelOpened)
{
    shared_ptr<VP_DataAccessorList> playList = make_shared<VP_DataAccessorList>();
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    player->setPlayerStatus(PLAYER_STATUS_PLAYING);
    player->m_playList = playList;
    EXPECT_CALL(*player, Stop());

    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_WAITING);
}

TEST_F(VP_ChannelWaitingEvent_Test, DoAction_WithChannelOpenedAndPlayerReady)
{
    shared_ptr<VP_DataAccessorList> playList = make_shared<VP_DataAccessorList>();
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    player->setPlayerStatus(PLAYER_STATUS_READY);
    player->m_playList = playList;
    EXPECT_CALL(*player, Stop());

    m_event->DoAction();
    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_WAITING);
}

TEST_F(VP_ChannelWaitingEvent_Test, DoAction_WithChannelClosed)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_CLOSED);
    shared_ptr<VP_Event> event = m_event->DoAction();
    EXPECT_EQ(event, nullptr);
}

TEST_F(VP_ChannelStopEvent_Test, DoAction)
{
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENING);
    shared_ptr<VP_DataAccessorList> playList;
    m_channel->setWaitPlayList(playList);

    m_event->DoAction();
}

TEST_F(VP_ChannelStopEvent_Test, DoAction_WithChannelOpened)
{
    EXPECT_CALL(*player, Stop());
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);

    m_event->DoAction();
}

TEST_F(VP_SetMuteEvent_Test, DoAction)
{
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*player, Stop());

    channelMgr->m_channels_list.push_back(m_channel);
    m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
    player->setPlayerStatus(PLAYER_STATUS_PLAYING);
    m_event->DoAction();
    channelMgr->m_channels_list.pop_back();

    EXPECT_EQ(m_channel->getChannelStatus(), CHANNEL_STATUS_CLOSING);
}

TEST_F(VP_ChannelConfirmedEvent_Test, DoAction)
{
    EXPECT_CALL(*channelMgr, confirmAudioStatus(_, _, _));
    m_event->DoAction();
}

TEST_F(VP_ChannelStatusQueryEvent_Test, DoAction)
{
    EXPECT_CALL(*channelMgr, queryAudioStatus(_));
    m_event->DoAction();
}

TEST_F(VP_GetDurationEvent_Test, GetDurationWithEmptyList)
{
    m_event->m_phraseList.clear();
    m_event->GetDuration();
}

TEST_F(VP_GetDurationEvent_Test, GetDurationWithWrongPhrase)
{
    shared_ptr<MockVP_PhraseImp> phrase = make_shared<MockVP_PhraseImp>();
    m_event->m_phraseList.push_back(phrase);
    EXPECT_CALL(*phrase, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_VP_INVALID_VALUE));

    int dwDuration = m_event->GetDuration();
    EXPECT_EQ(dwDuration, 0);
    phrase = nullptr;
}

TEST_F(VP_GetDurationEvent_Test, GetDuration)
{
    shared_ptr<MockVP_PhraseImp> phrase = make_shared<MockVP_PhraseImp>();
    m_event->m_phraseList.push_back(phrase);
    EXPECT_CALL(*phrase, SetPhraseInfo(_)).WillOnce(Return(APL_ERR_NO_ERROR));
    EXPECT_CALL(*phrase, GetDuration()).WillOnce(Return(100));

    m_event->GetDuration();
    phrase = nullptr;
}

TEST_F(VP_LanguageChangeEvent_Test, DoAction)
{
    EXPECT_CALL(*audioMgr, reqAudioSource(_, _)).WillRepeatedly(Return(CL_TRUE));
    m_event->DoAction();
}

/* EOF */
