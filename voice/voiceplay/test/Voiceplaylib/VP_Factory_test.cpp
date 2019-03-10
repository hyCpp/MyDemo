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

#include "VP_Factory.h"
#include "VC_CommonIF.h"
#include "VC_CommonDef.h"
#include "VC_AudioSession.h"
#include "VP_LanguageSetting.h"
#include "VP_DataManager.h"
#include "VP_SoundPlayer.h"

using namespace std;

/**
 * Summary for class VP_Factory_Test
 *
 * This is a detail description for class VP_Factory_Test
 *
 */
class VP_Factory_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_Factory = VP_Factory::Instance();
    }

    virtual void TearDown() {}

public:
    shared_ptr<VP_Factory> m_pVP_Factory;
};

TEST_F(VP_Factory_Test, Instance)
{
    shared_ptr<VP_Factory> pInstance = VP_Factory::Instance();
    EXPECT_FALSE(nullptr == pInstance);
    EXPECT_EQ(pInstance, VP_Factory::s_pInstance);
}

// test VP_Factory AcquireDataMgr function
TEST_F(VP_Factory_Test, AcquireDataMgr)
{
    shared_ptr<VP_DataManager> pDataManager = m_pVP_Factory->AcquireDataMgr();
    // EXPECT_TRUE(nullptr == pDataManager);  // comment this statement, this result depends on the existence of snd file
}

TEST_F(VP_Factory_Test, AcquireDataMgrTwice)
{
    shared_ptr<VP_DataManager> pDataManager = m_pVP_Factory->AcquireDataMgr();
    pDataManager = m_pVP_Factory->AcquireDataMgr();
    // EXPECT_TRUE(nullptr == pDataManager); // same as above
}

// test VP_Factory AcquireDataMgr function
TEST_F(VP_Factory_Test, AcquirePlayControl)
{
    shared_ptr<VP_VoicePlayControl> pControl = m_pVP_Factory->AcquirePlayControl();
    EXPECT_FALSE(nullptr == pControl);
}

// conditional coverage
TEST_F(VP_Factory_Test, AcquirePlayControlTwice)
{
    shared_ptr<VP_VoicePlayControl> pControl = m_pVP_Factory->AcquirePlayControl();
    pControl = m_pVP_Factory->AcquirePlayControl();
    EXPECT_FALSE(nullptr == pControl);
}

TEST_F(VP_Factory_Test, AcquireSoundPlayer)
{
    shared_ptr<VP_SoundPlayer> pSoundPlayer = m_pVP_Factory->AcquireSoundPlayer();
    EXPECT_FALSE(nullptr == pSoundPlayer);
}

TEST_F(VP_Factory_Test, AcquireSoundPlayerTwice)
{
    shared_ptr<VP_SoundPlayer> pSoundPlayer = m_pVP_Factory->AcquireSoundPlayer();
    pSoundPlayer = m_pVP_Factory->AcquireSoundPlayer();
    EXPECT_FALSE(nullptr == pSoundPlayer);
}

TEST_F(VP_Factory_Test, AcquireDurationManager)
{
    shared_ptr<VP_DurationManager> pDurationManager = m_pVP_Factory->AcquireDurationManager();
    EXPECT_FALSE(nullptr == pDurationManager);
}

TEST_F(VP_Factory_Test, AcquireDurationManagerTwice)
{
    shared_ptr<VP_DurationManager> pDurationManager = m_pVP_Factory->AcquireDurationManager();
    pDurationManager = m_pVP_Factory->AcquireDurationManager();
    EXPECT_FALSE(nullptr == pDurationManager);
}

/* EOF */
