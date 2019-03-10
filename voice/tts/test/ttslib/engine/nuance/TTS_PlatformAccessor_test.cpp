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
#include "stdafx.h"
#include "TTS_PlatformAccessor.h"
#include "TTS_File.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "TTS_file_test.h"
#include <stdio.h>

/**
* TTS_PlatformAccessor_test
*
* The class is just for TTS_PlatformAccessor_test.
*/
class TTS_PlatformAccessor_test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_file = new TTS_File();
        m_file->Open("/tmp/tts_access.txt", "w+");
        std::string strcontent = "this is a test.";
        m_file->Write(strcontent.c_str(), 1, strcontent.size());
        m_access = new TTS_PlatformAccessor("suntec", m_file, 10);
    }

    virtual void TearDown()
    {
        if (NULL != m_access) {
            delete m_access;
            m_access = NULL;
        }
    }

private:
    TTS_PlatformAccessor*    m_access;
    TTS_File*                m_file;
};

TEST_F(TTS_PlatformAccessor_test, destruct_p_fileNULL)
{
    m_access->m_pFile->Close();
    delete m_access->m_pFile;

    m_access->m_pFile = NULL;
}

TEST_F(TTS_PlatformAccessor_test, Exist_bothNULL)
{
    if (NULL != m_access->m_pszName) {
        delete m_access->m_pszName;
        m_access->m_pszName = NULL;
    }

    bool bret = m_access->Exist(NULL);
    EXPECT_FALSE(bret);

}

TEST_F(TTS_PlatformAccessor_test, Exist_m_pszNameNULL)
{
    if (NULL != m_access->m_pszName) {
        delete m_access->m_pszName;
        m_access->m_pszName = NULL;
    }

    bool bret = m_access->Exist("hello");
    EXPECT_FALSE(bret);

}

TEST_F(TTS_PlatformAccessor_test, Exist_False)
{
    bool bret = m_access->Exist("hello");
    EXPECT_FALSE(bret);
}

TEST_F(TTS_PlatformAccessor_test, Exist_pszNameNULL)
{
    // if (NULL != m_access->m_pszName) {
    //     delete m_access->m_pszName;
    //     m_access->m_pszName = NULL;
    // }

    bool bret = m_access->Exist(NULL);
    EXPECT_FALSE(bret);

}

TEST_F(TTS_PlatformAccessor_test, Exist)
{
    // if (NULL != m_access->m_pszName) {
    //     delete m_access->m_pszName;
    //     m_access->m_pszName = NULL;
    // }

    bool bret = m_access->Exist("suntec");
    EXPECT_TRUE(bret);

}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesZero_m_dwFileSizeSmaller_pAddrNULL)
{
    m_access->m_dwFileSize = 0;
    m_access->m_workBuff.m_bInit = false;
    m_access->m_pFile->Close();

    m_access->Map(1, 0);
}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesZero_m_dwFileSizeBigger_pAddrNULL)
{
    m_access->m_dwFileSize = 10;
    m_access->m_workBuff.m_bInit = false;
    m_access->m_pFile->Close();

    m_access->Map(1, 0);
}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesNotZero_m_dwFileSizeBigger_pAddrNULL)
{
    m_access->m_dwFileSize = 10;
    m_access->m_workBuff.m_bInit = false;
    m_access->m_pFile->Close();

    m_access->Map(1, 1);
}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesNotZero_m_dwFileSizeSmaller_pAddrNULL)
{
    m_access->m_dwFileSize = 0;
    m_access->m_workBuff.m_bInit = false;
    m_access->m_pFile->Close();

    m_access->Map(1, 1);
}

TEST_F(TTS_PlatformAccessor_test, Map_pAddrNULL_offsetBigger_addBigger)
{
    m_access->m_dwFileSize = 100;
    m_access->m_workBuff.m_bInit = true;
    m_access->m_workBuff.m_fileOffset = 10;
    m_access->m_pFile->Close();

    m_access->Map(1, 1);
}

TEST_F(TTS_PlatformAccessor_test, Map_pAddrNULL_offsetsmaller_addsmaller)
{
    m_access->m_dwFileSize = 10001;
    m_access->m_workBuff.m_bInit = true;
    m_access->m_workBuff.m_fileOffset = 0;
    m_access->m_pFile->Close();

    m_access->Map(5000, 5000);
}


TEST_F(TTS_PlatformAccessor_test, Map_SeekFalse)
{
    m_access->m_dwFileSize = 100;
    m_access->m_workBuff.m_bInit = false;
    m_access->m_workBuff.m_fileOffset = 0;

    m_access->m_pFile->Close();


    m_access->Map(1, 1);
}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesSmaller_GetMappedZero_ReadTrue)
{
    m_access->m_dwFileSize = 100;
    m_access->m_workBuff.m_bInit = false;
    m_access->m_workBuff.m_fileOffset = 0;

    // m_access->m_pFile->Close();


    m_access->Map(1, 1);
}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesSmaller_GetMappedZero_ReadFalse)
{
    m_access->m_dwFileSize = 100;
    m_access->m_workBuff.m_bInit = true;
    m_access->m_workBuff.m_fileOffset = 0;

    size_t iread = 0;
    FUNC_FAKE_SINGRET_SET(fread, iread);

    m_access->Map(1, 1);
    FUNC_FAKE_RESET1(fread);
}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesBigger_GetMappedZero)
{
    m_access->m_dwFileSize = 10000;
    m_access->m_workBuff.m_bInit = false;

    m_access->Map(10, 5000);
}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesSmaller_GetMappedNotZero)
{
    m_access->m_dwFileSize = 10000;
    m_access->m_workBuff.m_bInit = false;
    m_access->m_workBuff.m_mapped = 1;

    m_access->Map(10, 10);
}

TEST_F(TTS_PlatformAccessor_test, Map_pdwBytesSmaller_GetMappedNotZero_ReadFailed)
{
    m_access->m_dwFileSize = 10000;
    m_access->m_workBuff.m_bInit = false;
    m_access->m_workBuff.m_mapped = 1;

    size_t iread = 0;

    FUNC_FAKE_SINGRET_SET(fread, iread);

    m_access->Map(10, 10);
    FUNC_FAKE_RESET1(fread);
}

TEST_F(TTS_PlatformAccessor_test, Unmap_CheckInBufferTrue)
{
    unsigned char* buffer= m_access->m_workBuff.m_buffer + 2;

    m_access->Unmap(buffer);
}

#if 0
// not safe, so noted
TEST_F(TTS_PlatformAccessor_test, Unmap_CheckInBufferFalse)
{
    unsigned char* buffer = m_access->m_workBuff.m_buffer - 2;

    m_access->Unmap(buffer);
}

#endif

TEST_F(TTS_PlatformAccessor_test, Unmap_GetBufferNULL)
{
    m_access->m_workBuff.m_mapped = 10;

    m_access->m_workBuff.GetBuffer();
}

/* EOF */
