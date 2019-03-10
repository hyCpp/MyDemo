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

#ifndef CXX_BL_STRING_H
#    include "BL_String.h"
#endif

#ifndef CXX_BL_FULLFILE_H
#    include "BL_FullFile.h"
#endif

#ifndef CXX_TTS_PLATFORM_FILEMANAGER_H
#    include "TTS_PlatformFileManager.h"
#endif

#include "TTS_File.h"

static const XCHAR* const TTS_PFM_PATH_TEST = XTEXT("SD/.kanavijp/ndata/Sounds/tts");
static const XCHAR* const TTS_PFM_PATH_EMPTY = XTEXT("");

static const XCHAR* const TTS_PFM_LANG_TEST = XTEXT("ENG");
static const XCHAR* const TTS_PFM_LANG_EMPTY = XTEXT("");

static const XCHAR* const TTS_PFM_DAT_CLC = XTEXT("SD/.kanavijp/ndata/Sounds/tts/Nuance/engine_data/ENG/clc_eng_serena_cfg3.dat");
static const XCHAR* const TTS_PFM_DAT_SYNTH = XTEXT("SD/.kanavijp/ndata/Sounds/tts/Nuance/engine_data/ENG/synth_serena_dri80_1175mrf22_1175_02.dat");
static const XCHAR* const TTS_PFM_DAT_DCT = XTEXT("SD/.kanavijp/ndata/Sounds/tts/Nuance/engine_data/ENG/sysdct_eng_serena_cfg3.dat");
static const XCHAR* const TTS_PFM_DAT_EMPTY = XTEXT("");

static const CHAR* const TTS_FILE_MODE_R   = "r";
static const CHAR* const TTS_FILE_MODE_W   = "w";
static const CHAR* const TTS_FILE_MODE_RW  = "rw";

/**
* TTS_PlatformFileManager_Test
*
* The class is just for TTS_PlatformFileManager test.
*/
class TTS_PlatformFileManager_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
        BL_FullFile dataFile;
        BL_String cDatSynthTest(TTS_PFM_DAT_SYNTH);
        dataFile.MakePhysicalFullPath(cDatSynthTest, m_strSynthDat);
        BL_String cDatClcTest(TTS_PFM_DAT_CLC);
        dataFile.MakePhysicalFullPath(cDatClcTest, m_strClcDat);
        BL_String cDatDctTest(TTS_PFM_DAT_DCT);
        dataFile.MakePhysicalFullPath(cDatDctTest, m_strDctDat);
    }

    virtual void TearDown()
    {
    }

public:
    BL_String       m_strSynthDat;
    BL_String       m_strClcDat;
    BL_String       m_strDctDat;
};


TEST_F(TTS_PlatformFileManager_Test, Instance)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    EXPECT_EQ(pInstance, pInstance->Instance());
}

TEST_F(TTS_PlatformFileManager_Test, Destroy)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    pInstance->Destroy();
    EXPECT_EQ(NULL, TTS_PlatformFileManager::s_pInstance);
}

TEST_F(TTS_PlatformFileManager_Test, SetPath)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    CL_BOOL ret = pInstance->SetPath(TTS_PFM_PATH_TEST);
    EXPECT_TRUE(ret);
    EXPECT_STREQ(TTS_PFM_PATH_TEST, pInstance->m_szPath);
}

TEST_F(TTS_PlatformFileManager_Test, SetPath_NullPath)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    CL_BOOL ret = pInstance->SetPath(NULL);
    EXPECT_TRUE(ret);
    EXPECT_EQ((DWORD)0, pInstance->m_dwPathLen);
}

TEST_F(TTS_PlatformFileManager_Test, SetLang)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    CL_BOOL ret = pInstance->SetLang(TTS_PFM_LANG_TEST);
    EXPECT_TRUE(ret);
    EXPECT_STREQ(TTS_PFM_LANG_TEST, pInstance->m_szLang);
}

TEST_F(TTS_PlatformFileManager_Test, SetLang_NullLang)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    CL_BOOL ret = pInstance->SetLang(NULL);
    EXPECT_TRUE(ret);
    EXPECT_EQ((DWORD)0, pInstance->m_dwPathLen);
}

TEST_F(TTS_PlatformFileManager_Test, Acquire)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    pInstance->Acquire(m_strSynthDat, TTS_FILE_MODE_R);
}

TEST_F(TTS_PlatformFileManager_Test, Acquire_NullDat)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    TTS_PlatformAccessor *pAccessor = pInstance->Acquire(NULL, TTS_FILE_MODE_R);
    EXPECT_EQ(NULL, pAccessor);
}

TEST_F(TTS_PlatformFileManager_Test, Acquire_Repeat)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    TTS_PlatformAccessor *pAccessor = pInstance->Acquire(m_strSynthDat, TTS_FILE_MODE_R);
    TTS_PlatformAccessor *pAccessorRepeat = pInstance->Acquire(m_strSynthDat, TTS_FILE_MODE_R);
    EXPECT_EQ(pAccessor, pAccessorRepeat);
}

TEST_F(TTS_PlatformFileManager_Test, Acquire_EmptyDat)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    TTS_PlatformAccessor *pAccessor = pInstance->Acquire(TTS_PFM_DAT_EMPTY, TTS_FILE_MODE_R);
    EXPECT_EQ(NULL, pAccessor);
}

TEST_F(TTS_PlatformFileManager_Test, OpeTTS_File)
{
    TTS_PlatformFileManager ttsPlatformFileManager;
    ttsPlatformFileManager.OpeTTS_File(m_strSynthDat, TTS_FILE_MODE_R);
}

TEST_F(TTS_PlatformFileManager_Test, OpeTTS_File_WrongMode)
{
    TTS_PlatformFileManager ttsPlatformFileManager;
    TTS_File *pFile = ttsPlatformFileManager.OpeTTS_File(m_strSynthDat, "");
    EXPECT_EQ(NULL, pFile);
}

TEST_F(TTS_PlatformFileManager_Test, Release)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    TTS_PlatformAccessor *pAccessor = pInstance->Acquire(m_strSynthDat, TTS_FILE_MODE_R);
    pInstance->Release(pAccessor);
}

TEST_F(TTS_PlatformFileManager_Test, Clear)
{
    TTS_PlatformFileManager *pInstance = TTS_PlatformFileManager::Instance();
    pInstance->Clear();
}

TEST_F(TTS_PlatformFileManager_Test, Destroy_NULL)
{
    TTS_PlatformFileManager* pInstance = TTS_PlatformFileManager::Instance();
    TTS_PlatformFileManager::Destroy();
    TTS_PlatformFileManager::Destroy();
}

TEST_F(TTS_PlatformFileManager_Test, OpeTTS_File_OpenTrue)
{
    TTS_PlatformFileManager* pInstance = TTS_PlatformFileManager::Instance();

    TTS_File* file =  pInstance->OpeTTS_File("/tmp/filemanager.txt", "w+");
    file->Write("mengfanbing", 1, 11);
    file->Close();
    delete file;
}

TEST_F(TTS_PlatformFileManager_Test, Release_DecRefNotOne)
{
    TTS_PlatformFileManager* pInstance = TTS_PlatformFileManager::Instance();

    TTS_File* p_file = new TTS_File();
    p_file->Open("/tmp/filemanager.txt", "r+");

    TTS_PlatformAccessor* accessor = pInstance->NewAccessor("suntec", p_file);
    accessor->m_nRefCount = 0;
    pInstance->Release(accessor);
}

TEST_F(TTS_PlatformFileManager_Test, Release_DecRefEqualsOne)
{
    TTS_PlatformFileManager* pInstance = TTS_PlatformFileManager::Instance();

    TTS_File* p_file = new TTS_File();
    p_file->Open("/tmp/filemanager.txt", "r+");

    TTS_PlatformAccessor* accessor = pInstance->NewAccessor("suntec", p_file);
    accessor->m_nRefCount = 1;
    pInstance->Release(accessor);
    TTS_PlatformFileManager::Destroy();
}

TEST_F(TTS_PlatformFileManager_Test, GetAccessor_ReturnNotExist)
{
    TTS_PlatformFileManager::Destroy();
    TTS_PlatformFileManager* pInstance = TTS_PlatformFileManager::Instance();

    TTS_File* p_file = new TTS_File();
    p_file->Open("/tmp/filemanager.txt", "r+");

    TTS_PlatformAccessor* accessor = pInstance->NewAccessor("suntec", p_file);
    pInstance->m_fileList.Add(accessor);
    pInstance->GetAccessor("testcp");

    p_file = NULL;
}

TEST_F(TTS_PlatformFileManager_Test, GetAccessor_ReturnIterator)
{
    TTS_PlatformFileManager* pInstance = TTS_PlatformFileManager::Instance();

    TTS_File* p_file = new TTS_File();
    p_file->Open("/tmp/filemanager.txt", "r+");

    TTS_PlatformAccessor* accessor = pInstance->NewAccessor("suntec", p_file);
    pInstance->m_fileList.Add(accessor);
    pInstance->GetAccessor("suntec");

    p_file = NULL;
}

TEST_F(TTS_PlatformFileManager_Test, Acquire_ReturnAccessor)
{
    TTS_PlatformFileManager* pInstance = TTS_PlatformFileManager::Instance();
    pInstance->Acquire("suntec", "r+");
}

TEST_F(TTS_PlatformFileManager_Test, Acquire_ReturnAccessor2)
{
    TTS_PlatformFileManager::Destroy();
    TTS_PlatformFileManager* pInstance = TTS_PlatformFileManager::Instance();
    memset(pInstance->m_szPath, 0, strlen(pInstance->m_szPath));

    TTS_File* file =  pInstance->OpeTTS_File("/tmp/Acquire.txt", "w+");
    file->Write("mengfanbing", 1, 11);
    file->Close();

    pInstance->Acquire("/tmp/Acquire.txt", "r+");
}


/* EOF */
