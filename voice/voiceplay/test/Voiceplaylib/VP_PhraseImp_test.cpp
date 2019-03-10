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

#ifndef CXX_BL_ERROR_H
#	include "BL_Error.h"
#endif

#include "VP_PhraseImp.h"
#include "VP_DataManager_mock.h"

using ::testing::Return;
using ::testing::_;
using namespace std;

/**
 * Summary for class VP_WaveIdPhrase_Test
 * 
 * This is a detail description for class VP_WaveIdPhrase_Test
 *
 */
class VP_WaveIdPhrase_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_WaveIdPhrase = make_shared<VP_WaveIdPhrase>(VP_WAVE_ID_SPEEDCAMERA);
    }

    virtual void TearDown() {}

public:
    shared_ptr<VP_WaveIdPhrase> m_pVP_WaveIdPhrase;
};

/**
 * Summary for class VP_WaveIdPhrase_Test
 * 
 * This is a detail description for class VP_WaveIdPhrase_Test
 *
 */
class VP_TtsPhrase_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_TtsPhrase = make_shared<VP_TtsPhrase>("Gtest");
    }

    virtual void TearDown() {}

public:
    shared_ptr<VP_TtsPhrase> m_pVP_TtsPhrase;
};

TEST_F(VP_WaveIdPhrase_Test, SetPhraseInfo)
{
    MockVP_DataManager mockDataManager;
    EXPECT_CALL(mockDataManager, GetGuidePhrase(_, _, _)).WillOnce(Return(APL_ERR_NO_ERROR));
    CL_ERROR err = m_pVP_WaveIdPhrase->SetPhraseInfo(mockDataManager);
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VP_WaveIdPhrase_Test, SetPhraseInfo_PhraseNotExist)
{
    MockVP_DataManager mockDataManager;
    EXPECT_CALL(mockDataManager, GetGuidePhrase(_, _, _)).WillOnce(Return(APL_ERR_VP_PHRASE_NOTEXIST));
    CL_ERROR err = m_pVP_WaveIdPhrase->SetPhraseInfo(mockDataManager);
    EXPECT_EQ(APL_ERR_VP_PHRASE_NOTEXIST, err);
}

TEST_F(VP_WaveIdPhrase_Test, CreateDataAccessor)
{
    shared_ptr<VP_DataAccessor> pDataAccessor = m_pVP_WaveIdPhrase->CreateDataAccessor();
    EXPECT_FALSE(nullptr == pDataAccessor);
}

TEST_F(VP_WaveIdPhrase_Test, CanBeCancelled_True)
{
    VP_WaveIdPhrase cancelPhrase_first(VP_WAVE_ID_CANCEL_1);
    EXPECT_TRUE(cancelPhrase_first.CanBeCancelled());

    VP_WaveIdPhrase cancelPhrase_second(VP_WAVE_ID_CANCEL_2);
    EXPECT_TRUE(cancelPhrase_second.CanBeCancelled());

    VP_WaveIdPhrase cancelPhrase_third(VP_WAVE_ID_CANCEL_3);
    EXPECT_TRUE(cancelPhrase_third.CanBeCancelled());
}

TEST_F(VP_WaveIdPhrase_Test, CanBeCancelled_False)
{
    EXPECT_FALSE(m_pVP_WaveIdPhrase->CanBeCancelled());
}

TEST_F(VP_WaveIdPhrase_Test, GetCancelledDuration)
{
    m_pVP_WaveIdPhrase->SetCancelled();
    EXPECT_TRUE(m_pVP_WaveIdPhrase->GetDuration() == 0);
}

TEST_F(VP_WaveIdPhrase_Test, GetDuration)
{
    m_pVP_WaveIdPhrase->m_phraseInfo.Time = 60;
    EXPECT_EQ(m_pVP_WaveIdPhrase->GetDuration(), 60);
}

TEST_F(VP_TtsPhrase_Test, GetDuration)
{
    EXPECT_LT(DWORD(0), m_pVP_TtsPhrase->GetDuration());
}

TEST_F(VP_TtsPhrase_Test, GetDataAccessor_NullCheck)
{
    shared_ptr<VP_DataAccessor> pDataAccessor = m_pVP_TtsPhrase->CreateDataAccessor();
    EXPECT_TRUE(nullptr != pDataAccessor);
}

TEST_F(VP_TtsPhrase_Test, GetDataAccessor)
{
    shared_ptr<VP_DataAccessor> pDataAccessor = m_pVP_TtsPhrase->CreateDataAccessor();
    EXPECT_TRUE(nullptr != pDataAccessor);
}

/* EOF */
