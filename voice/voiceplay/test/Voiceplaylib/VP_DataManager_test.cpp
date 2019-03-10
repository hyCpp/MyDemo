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

#include <string>
#include "VP_DataManager.h"
#include "VP_LanguageSetting.h"
#include "VP_SoundFile_mock.h"
#include "BL_SystemLanguage_DEF.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;
using namespace std;

/**
 * Summary for class VP_DataManager_Test
 * 
 * This is a detail description for class VP_DataManager_Test
 *
 */
class VP_DataManager_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        soundFile = make_shared<MockVP_SoundFile>();
        shared_ptr<VP_LanguageSetting> languageSetting = make_shared<VP_LanguageSetting>();
        m_pVP_DataManager = make_shared<VP_DataManager>(languageSetting, soundFile);
    }

    virtual void TearDown() {}

private:
    shared_ptr<MockVP_SoundFile> soundFile;
    shared_ptr<VP_DataManager> m_pVP_DataManager;
};

TEST_F(VP_DataManager_Test, Constructor_WithNullLanguageSetting)
{
    shared_ptr<VP_DataManager> dataMgr = make_shared<VP_DataManager>(nullptr, nullptr);
    EXPECT_EQ(dataMgr->m_languageSetting, nullptr);
}

TEST_F(VP_DataManager_Test, Initialize)
{
    EXPECT_CALL(*soundFile, Initialize(_)).WillOnce(Return(APL_ERR_NO_ERROR));
    CL_ERROR err = m_pVP_DataManager->Initialize();
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VP_DataManager_Test, Initialize_WithError)
{
    EXPECT_CALL(*soundFile, Initialize(_)).WillOnce(Return(APL_ERR_VP_FILE_READ));
    CL_ERROR err = m_pVP_DataManager->Initialize();
    EXPECT_EQ(APL_ERR_VP_FILE_READ, err);
}

TEST_F(VP_DataManager_Test, GetGuidePhrase)
{
    EXPECT_CALL(*soundFile, GetPhraseInfo(0)).WillOnce(Return(new VP_Phrase()));

    VP_Phrase phrase;
    BL_String fileName;
    CL_ERROR err = m_pVP_DataManager->GetGuidePhrase(0, phrase, fileName);
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VP_DataManager_Test, GetNullGuidePhrase)
{
    EXPECT_CALL(*soundFile, GetPhraseInfo(0)).WillOnce(Return(nullptr));

    VP_Phrase phrase;
    BL_String fileName;
    CL_ERROR err = m_pVP_DataManager->GetGuidePhrase(0, phrase, fileName);
    EXPECT_EQ(err, APL_ERR_VP_PHRASE_NOTEXIST);
}

TEST_F(VP_DataManager_Test, GetLanguage)
{
    string language = m_pVP_DataManager->GetLanguage();
}

/* EOF */
