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

#ifndef CXX_BL_STRING_H 
#    include "BL_String.h" 
#endif 

#include "BL_SystemLanguage.h"
#include "VP_DurationManager.h"

using namespace std;

/**
 * Summary for class VP_DurationManager_Test
 * 
 * This is a detail description for class VP_DurationManager_Test
 *
 */
class VP_DurationManager_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_DurationManager = new(MEM_Voice) VP_DurationManager();
    }

    virtual void TearDown()
    {
        if (NULL != m_pVP_DurationManager) {
            delete m_pVP_DurationManager;
            m_pVP_DurationManager = NULL;
        }
    }

public:
    VP_DurationManager  *m_pVP_DurationManager;
};

TEST_F(VP_DurationManager_Test, GetDuration)
{
    BL_String strContent;
    EXPECT_EQ(INT(0), m_pVP_DurationManager->GetDuration(strContent));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_MNC)
{
    EXPECT_LT(INT(0), m_pVP_DurationManager->GetDurationByLanguage(TEXT("test"), BL_LANGUAGE_TYPE_SIMPLIFIED_CHINESE));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_ENG)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_ENGLISH));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_UKENG)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_UKENGLISH));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_THT)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_THAI));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_IDI)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_INDONESIAN));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_PTP)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_PORTUGUESE));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_SPE)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_SPANISH));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_JPJ)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_JAPANESE));
}

TEST_F(VP_DurationManager_Test, GetDurationByLanguage_MLY)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_MALAY));
}

TEST_F(VP_DurationManager_Test, GetDurationByDefaultLanguage)
{
    EXPECT_LT(0, m_pVP_DurationManager->GetDurationByLanguage("test", BL_LANGUAGE_TYPE_DEFAULT));
}

TEST_F(VP_DurationManager_Test, CalcDuration_MNC)
{
    EXPECT_LT(INT(0), m_pVP_DurationManager->CalcDuration_MNC(TEXT("test")));
}

TEST_F(VP_DurationManager_Test, CalcDuration_MNC_WithOtherText)
{
    char str[] = { 0x80, 0x80, 0x80, 0 };
    EXPECT_EQ(INT(0), m_pVP_DurationManager->CalcDuration_MNC(TEXT(str)));
}

TEST_F(VP_DurationManager_Test, CalcDuration_MNC_Punctuation)
{
    EXPECT_LT(INT(0), m_pVP_DurationManager->CalcDuration_MNC(TEXT("[test, test.]")));
}

TEST_F(VP_DurationManager_Test, CalcDuration_ENG)
{
    EXPECT_LT(INT(0), m_pVP_DurationManager->CalcDuration_ENG(TEXT("test")));
}

TEST_F(VP_DurationManager_Test, CalcDuration_THT)
{
    EXPECT_LT(INT(0), m_pVP_DurationManager->CalcDuration_THT(BL_String("test")));
}

TEST_F(VP_DurationManager_Test, CalcDuration_IDI)
{
    EXPECT_LT(INT(0), m_pVP_DurationManager->CalcDuration_IDI(BL_String("test")));
}

TEST_F(VP_DurationManager_Test, CalcDuration_PTP)
{
    EXPECT_LT(INT(0), m_pVP_DurationManager->CalcDuration_PTP(BL_String("test")));
}

TEST_F(VP_DurationManager_Test, CalcDuration_SPE)
{
    EXPECT_LT(INT(0), m_pVP_DurationManager->CalcDuration_SPE(BL_String("test")));

}
/* EOF */
