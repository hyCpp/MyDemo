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

#include "VP_SoundFile.h"
#include "VP_PhraseImp.h"
#include "BL_File.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using namespace std;

/**
 * Summary for class VP_SoundFile_Test
 * 
 * This is a detail description for class VP_SoundFile_Test
 *
 */
class VP_SoundFile_Test:public testing::Test
{
public:
    virtual void SetUp()
    {
        m_pVP_SoundFile = make_shared<VP_SoundFile>();
        m_pVP_SoundFile->soundFile = make_shared<BL_File>(BL_FILE_PREFIX_TMP);

        header = new(MEM_Voice)VP_Header();
        header->phraseCount = 1;
        header->nameIdCount = 1;
    }

    virtual void TearDown() 
    {
        if (nullptr != header) {
            delete header;
            header = nullptr;
        }
    }

    void write_fixed_data();
    void write_fixed_seek_failed_data();
    void write_fixed_data_without_phrase();
            
private:
    VP_Header *header;
    shared_ptr<VP_SoundFile> m_pVP_SoundFile;
};

ACTION_P(cast_type, param) 
{
    *static_cast<VP_Header *>(arg0) = param;
}

void VP_SoundFile_Test::write_fixed_data()
{
    unsigned char fixed_data[] = { 0x53, 0x50, 0x53, 0x44, 0x30, 0x31, 0x30, 0x31,
        0x32, 0x30, 0x31, 0x35, 0x31, 0x31, 0x31, 0x38, 0x31, 0x32, 0x32, 0x35,
        0x30, 0x33, 0x30, 0x30, 0x30, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0,
        0x01, 0x10, 0x30, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x0, 0x57, 0x29,
        0x0, 0x0, 0x58, 0x06, 0x0, 0x0 };
    FILE *fp = fopen("/tmp/vp_test", "wb");
    if (nullptr != fp) {
        int length = sizeof(fixed_data) / sizeof(char);
        int bytes = fwrite(fixed_data, sizeof(char), length, fp);

        fclose(fp);
    }
}

void VP_SoundFile_Test::write_fixed_seek_failed_data()
{
    unsigned char fixed_data[] = { 0x53, 0x50, 0x53, 0x44, 0x30, 0x31, 0x30, 0x31,
        0x32, 0x30, 0x31, 0x35, 0x31, 0x31, 0x31, 0x38, 0x31, 0x32, 0x32, 0x35,
        0x30, 0x33, 0x30, 0x30, 0xFF, 0xFF, 0xFF, 0xFF, 0x1, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0,
        0x01, 0x10, 0x30, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x0, 0x57, 0x29,
        0x0, 0x0, 0x58, 0x06, 0x0, 0x0 };
    FILE *fp = fopen("/tmp/vp_test", "wb");
    if (nullptr != fp) {
        int length = sizeof(fixed_data) / sizeof(char);
        fwrite(fixed_data, sizeof(char), length, fp);

        fclose(fp);
    }
}

void VP_SoundFile_Test::write_fixed_data_without_phrase()
{
    unsigned char fixed_data[] = { 0x53, 0x50, 0x53, 0x44, 0x30, 0x31, 0x30, 0x31,
        0x32, 0x30, 0x31, 0x35, 0x31, 0x31, 0x31, 0x38, 0x31, 0x32, 0x32, 0x35,
        0x30, 0x33, 0x30, 0x30, 0x30, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0 };
    FILE *fp = fopen("/tmp/vp_test", "wb");
    if (nullptr != fp) {
        int length = sizeof(fixed_data) / sizeof(char);
        fwrite(fixed_data, sizeof(char), length, fp);

        fclose(fp);
    }
}

TEST_F(VP_SoundFile_Test, Initialize)
{
    write_fixed_data();

    CL_ERROR err = m_pVP_SoundFile->Initialize("vp_test");
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VP_SoundFile_Test, Initialize_WithOpenFailed)
{
    CL_ERROR err = m_pVP_SoundFile->Initialize("vp_test_none");
    EXPECT_EQ(APL_ERR_VP_FILE_OPEN, err);
}

TEST_F(VP_SoundFile_Test, Initialize_WithReadHeaderFailed)
{
    FILE *fp = fopen("/tmp/vp_test", "wb");

    if (nullptr != fp) {
        CL_ERROR err = m_pVP_SoundFile->Initialize("vp_test");
        EXPECT_EQ(APL_ERR_VP_FILE_READ, err);

        fclose(fp);
    }
}

TEST_F(VP_SoundFile_Test, Initialize_WithSeekPhraseFailed)
{
    write_fixed_seek_failed_data();

    CL_ERROR err = m_pVP_SoundFile->Initialize("vp_test");
    EXPECT_EQ(APL_ERR_VP_FILE_READ, err);
}

TEST_F(VP_SoundFile_Test, Initialize_WithReadPhraseFailed)
{
    write_fixed_data_without_phrase();
    CL_ERROR err = m_pVP_SoundFile->Initialize("vp_test");
    EXPECT_EQ(APL_ERR_VP_FILE_READ, err);
}

TEST_F(VP_SoundFile_Test, GetPhraseInfo_NullCheck)
{
    DWORD phraseId = 0;
    m_pVP_SoundFile->m_pPhrases = NULL;
    const VP_Phrase* pPhrase = m_pVP_SoundFile->GetPhraseInfo(phraseId);
    EXPECT_TRUE(NULL == pPhrase);
}

TEST_F(VP_SoundFile_Test, GetPhraseInfo_SameNameFlag)
{
    DWORD phraseId = 3;
    VP_Phrase phrase = { phraseId, 0, 0, 0, 0 };
    m_pVP_SoundFile->m_pPhrases = &phrase;
    m_pVP_SoundFile->m_phraseCount = 1;
    const VP_Phrase* pPhrase = m_pVP_SoundFile->GetPhraseInfo(phraseId);
    EXPECT_TRUE(NULL == pPhrase);
    m_pVP_SoundFile->m_pPhrases = NULL;
}

TEST_F(VP_SoundFile_Test, GetPhraseInfo_InvalidValue)
{
    DWORD phraseId = 2;

    VP_Phrase phrase = { 3, 0, 0, 0, 0 };
    m_pVP_SoundFile->m_pPhrases = &phrase;
    m_pVP_SoundFile->m_phraseCount = 1;
    const VP_Phrase* pPhrase = m_pVP_SoundFile->GetPhraseInfo(phraseId);
    EXPECT_TRUE(NULL == pPhrase);
    m_pVP_SoundFile->m_pPhrases = NULL;
}

TEST_F(VP_SoundFile_Test, GetPhraseInfo)
{
    DWORD phraseId = 3;
    VP_Phrase phrase = { phraseId, -1, 0, 0, 0 };
    m_pVP_SoundFile->m_pPhrases = &phrase;
    m_pVP_SoundFile->m_phraseCount = 1;
    const VP_Phrase* pPhrase = m_pVP_SoundFile->GetPhraseInfo(phraseId);
    EXPECT_TRUE(NULL != pPhrase);
    m_pVP_SoundFile->m_pPhrases = NULL;
}

TEST_F(VP_SoundFile_Test, GetPhrase)
{
    DWORD phraseId = 0;
    m_pVP_SoundFile->GetPhraseObject(phraseId);

}

TEST_F(VP_SoundFile_Test, GetPhraseWithWrongId)
{
    DWORD phraseId = -1;
    VP_Phrase phrase = { 0, -1, 0, 0, 0 };
    m_pVP_SoundFile->m_pPhrases = &phrase;
    m_pVP_SoundFile->m_phraseCount = 1;
    const VP_Phrase* pPhrase = m_pVP_SoundFile->GetPhraseObject(phraseId);

    EXPECT_EQ(NULL, pPhrase);
    m_pVP_SoundFile->m_pPhrases = NULL;
}

/* EOF */
