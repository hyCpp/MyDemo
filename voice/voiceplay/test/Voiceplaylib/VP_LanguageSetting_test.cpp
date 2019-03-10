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

#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VP_LanguageSetting.h"

using ::testing::Return;
using ::testing::AtLeast;
using namespace std;

/**
 * Summary for class VP_LanguageSetting_test
 * 
 * This is a detail description for class VP_LanguageSetting_test
 *
 */
class VP_LanguageSetting_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_LanguageSetting = make_shared<VP_LanguageSetting>();
    }

    virtual void TearDown() {}

private:
    shared_ptr<VP_LanguageSetting> m_pVP_LanguageSetting;
};

TEST_F(VP_LanguageSetting_Test, getLanguage)
{
   m_pVP_LanguageSetting->getLanguage();
} 

TEST_F(VP_LanguageSetting_Test, GetGuideSoundFileName)
{
    m_pVP_LanguageSetting->m_guideSoundFileName = "gtest";
    BL_String str = m_pVP_LanguageSetting->getGuideSoundFileName();
    EXPECT_EQ(m_pVP_LanguageSetting->m_guideSoundFileName, str);
}

TEST_F(VP_LanguageSetting_Test, loadLanguage)
{
    m_pVP_LanguageSetting->loadLanguage();
}

TEST_F(VP_LanguageSetting_Test, loadLanguage_WithEmptyTable)
{
    m_pVP_LanguageSetting->m_languageTable.clear();
    m_pVP_LanguageSetting->loadLanguage();
}

TEST_F(VP_LanguageSetting_Test, loadGender)
{
    m_pVP_LanguageSetting->loadGender();
}

TEST_F(VP_LanguageSetting_Test, setGender)
{
    m_pVP_LanguageSetting->setGender(0);
    m_pVP_LanguageSetting->setGender(1);
    m_pVP_LanguageSetting->setGender(2);
}
    
/* EOF */
