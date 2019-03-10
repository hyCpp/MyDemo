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

#include "VP_Define.h"
#include "VP_DataAccessorSound.h"
#include "VC_Player.h"
#include "VC_PlayerGeneral.h"
#include "VC_Player_mock.h"
#include "VP_VoicePlayControl.h"

using ::testing::_;
using ::testing::Return;
using namespace std;

/**
 * Summary for class VP_DataAccessorID_Test
 * 
 * This is a detail description for class VP_DataAccessorID_Test
 *
 */
class VP_DataAccessorID_Test:public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_DataAccessorID = make_shared<VP_DataAccessorID>("VPTest", 0, 0);
    }

    virtual void TearDown() {}

public:
    shared_ptr<VP_DataAccessorID>  m_pVP_DataAccessorID;
};

/**
 * Summary for class VP_DataAccessorBuf_Test
 * 
 * This is a detail description for class VP_DataAccessorBuf_Test
 *
 */
class VP_DataAccessorBuf_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_DataAccessorBuf = make_shared<VP_DataAccessorBuf>(nullptr, 0);
    }

    virtual void TearDown() {}

public:
    shared_ptr<VP_DataAccessorBuf> m_pVP_DataAccessorBuf;
};

/**
 * Summary for class VP_DataAccessorTts_Test
 * 
 * This is a detail description for class VP_DataAccessorTts_Test
 *
 */
class VP_DataAccessorTts_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_DataAccessorTts = make_shared<VP_DataAccessorTts>("VPTest", "ENG");
    }

    virtual void TearDown() {}

public:
    shared_ptr<VP_DataAccessorTts>  m_pVP_DataAccessorTts;
};

TEST_F(VP_DataAccessorID_Test, Play)
{
   MockVC_Player mockPlayer;
   EXPECT_CALL(mockPlayer, Play(_, _, _, _, _, _)).WillOnce(Return(CL_TRUE));

   CL_BOOL isTtsPlay;
   CL_ERROR err = m_pVP_DataAccessorID->Play(&mockPlayer, nullptr, isTtsPlay, 0, 0);
   EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VP_DataAccessorID_Test, Play_FALSE)
{
   MockVC_Player mockPlayer;
   EXPECT_CALL(mockPlayer, Play(_, _, _, _, _, _)).WillOnce(Return(CL_FALSE));

   CL_BOOL isTtsPlay;
   CL_ERROR err = m_pVP_DataAccessorID->Play(&mockPlayer, nullptr, isTtsPlay, 0, 0);
   EXPECT_EQ(APL_ERR_VP_AUDIO_PLAY, err);
}

TEST_F(VP_DataAccessorBuf_Test, Play)
{
   MockVC_Player mockPlayer;
   EXPECT_CALL(mockPlayer, Play(nullptr, _, _, _, _)).WillOnce(Return(CL_TRUE));
   
   CL_BOOL isTtsPlay;
   CL_ERROR err = m_pVP_DataAccessorBuf->Play(&mockPlayer, nullptr, isTtsPlay, 0, 0);
   EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VP_DataAccessorBuf_Test, Play_FALSE)
{
   MockVC_Player mockPlayer;
   EXPECT_CALL(mockPlayer, Play(nullptr, _, _, _, _)).WillOnce(Return(CL_FALSE));

   CL_BOOL isTtsPlay;
   CL_ERROR err = m_pVP_DataAccessorBuf->Play(&mockPlayer, nullptr, isTtsPlay, 0, 0);
   EXPECT_EQ(APL_ERR_VP_AUDIO_PLAY, err);
}

TEST_F(VP_DataAccessorTts_Test, Play)
{
   MockVC_Player mockPlayer;
   CL_BOOL isTtsPlay = CL_FALSE;
   shared_ptr<VP_VoicePlayControl> playControl = make_shared<VP_VoicePlayControl>(nullptr, nullptr);
   m_pVP_DataAccessorTts->setPlayControl(playControl);
   
   playControl->setTtsServiceStatus(true);
   CL_ERROR err = m_pVP_DataAccessorTts->Play(&mockPlayer, nullptr, isTtsPlay, 0, 0);
   EXPECT_EQ(APL_ERR_NO_ERROR, err);

   playControl->setTtsServiceStatus(false);
   err = m_pVP_DataAccessorTts->Play(&mockPlayer, nullptr, isTtsPlay, 0, 0);
   EXPECT_EQ(APL_ERR_VP_AUDIO_PLAY, err);
}

TEST_F(VP_DataAccessorTts_Test, CreateMessage)
{
    EXPECT_TRUE(m_pVP_DataAccessorTts->CreateMessage(0, 0) != nullptr);
}

TEST_F(VP_DataAccessorTts_Test, SendMessage)
{
   m_pVP_DataAccessorTts->SendMessage(EV_EVENT_TTS_PLAYTEXT, NULL);
}
/* EOF */
