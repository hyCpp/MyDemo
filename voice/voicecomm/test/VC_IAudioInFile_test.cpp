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
#include "VC_IAudioInFile.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "VC_Log.h"
#include "VC_WavFileWriter.h"
#include "VC_FileTest_fff.h"

/**
 * Summary for class VC_IAudioInFile_test
 * 
 * This is a detail description for class VC_IAudioInFile_test
 *
 */
class VC_IAudioInFile_test  : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_audioinfile = new(MEM_Voice) VC_IAudioInFile();
    }

    virtual void TearDown()
    {
        delete m_audioinfile;
        m_audioinfile = NULL;
    }

private:
    VC_IAudioInFile*    m_audioinfile;
};

TEST_F(VC_IAudioInFile_test, Opensizezero)
{
    bool bret = m_audioinfile->Open(10, 20);

    EXPECT_FALSE(bret);
}

TEST_F(VC_IAudioInFile_test, Open)
{
    m_audioinfile->sequences.push_back(std::string("mengfanbing"));
    m_audioinfile->sequences.push_back(std::string("suntec"));

    m_audioinfile->index = 3;

}

// TEST_F(VC_IAudioInFile_test, reloadfile)
// {
//     std::string strPackFileName = "/udisk/vr_rec_sequences.txt";
//     std::ofstream packfile(strPackFileName.c_str(), std::ios::out | std::ios::binary);
//     if (packfile.is_open()) {
//         packfile.write ((const char*)packetBuff, extProData->size + PACKEXTAVCHEADSIZE);

//     }
// }

TEST_F(VC_IAudioInFile_test, Close)
{
    bool bret = m_audioinfile->Close();

    EXPECT_TRUE(bret);
}

TEST_F(VC_IAudioInFile_test, Stop)
{
    bool bret = m_audioinfile->Stop();

    EXPECT_TRUE(bret);
}

TEST_F(VC_IAudioInFile_test, Read)
{
    char buf[20] = { 0 };
    int size = 0;

    int iret = m_audioinfile->Read(buf, size);

    EXPECT_EQ(iret, 0);
}

TEST_F(VC_IAudioInFile_test, Opensequencessizezero)
{
    bool bret = m_audioinfile->Open(0, 9);

    EXPECT_FALSE(bret);
}

TEST_F(VC_IAudioInFile_test, Opensequencessmallerthanindex)
{
    m_audioinfile->sequences.push_back(std::string("mengfanbing"));
    m_audioinfile->index = 10;

    bool bret = m_audioinfile->Open(0, 0);
    EXPECT_FALSE(bret);
}

TEST_F(VC_IAudioInFile_test, Opensequencesbiggerthanindex)
{
    m_audioinfile->sequences.push_back(std::string("mengfanbing"));
    m_audioinfile->index = 0;

    bool bret = m_audioinfile->Open(0, 0);
    EXPECT_FALSE(bret);
}

TEST_F(VC_IAudioInFile_test, GetParam)
{
    int type = 1;
    int* p_type = &type;

    EXPECT_FALSE(m_audioinfile->GetParam(type, (void**)&p_type, p_type));
}

TEST_F(VC_IAudioInFile_test, reloadis_open_sequencessizeBiggerZerodirNotNULLtxt)
{
    FILE* pfile = fopen("/tmp/VC_IAudioInFile.txt", "wb");
    if (NULL == pfile) {
        std::cout << "VC_IAudioInFile_test fopen VC_IAudioInFile.txt failed." << std::endl;
        return;
    }

    std::string name1("mengfanbing");
    int ret = fwrite(name1.c_str(), name1.size(), 1, pfile);
    fclose(pfile);

    m_audioinfile->strRecListPath = "/tmp/VC_IAudioInFile.txt";
    m_audioinfile->strRecListDir = "/tmp/";

    m_audioinfile->reload();
}

TEST_F(VC_IAudioInFile_test, reloadis_open_sequencessizeBiggerZerodirNotNULLwav)
{
    FILE* pfile = fopen("/tmp/VC_IAudioInFile.wav", "wb");
    if (NULL == pfile) {
        std::cout << "VC_IAudioInFile_test fopen VC_IAudioInFile.txt failed." << std::endl;
        return;
    }

    std::string name1("mengfanbing");
    int ret = fwrite(name1.c_str(), name1.size(), 1, pfile);
    fclose(pfile);

    m_audioinfile->strRecListPath = "/tmp/VC_IAudioInFile_Wrong.txt";
    m_audioinfile->strRecListDir = "/tmp/";

    m_audioinfile->reload();
}

TEST_F(VC_IAudioInFile_test, reloadis_open_sequencessizeBiggerZerodirNotNULLwavCloseDirFailed)
{
    FILE* pfile = fopen("/tmp/VC_IAudioInFile.wav", "wb");
    if (NULL == pfile) {
        std::cout << "VC_IAudioInFile_test fopen VC_IAudioInFile.txt failed." << std::endl;
        return;
    }

    std::string name1("mengfanbing");
    int ret = fwrite(name1.c_str(), name1.size(), 1, pfile);
    fclose(pfile);

    m_audioinfile->strRecListPath = "/tmp/VC_IAudioInFile_Wrong.txt";
    m_audioinfile->strRecListDir = "/tmp/";

    FUNC_FAKE_SINGRET_SET(closedir, 1);
    m_audioinfile->reload();
    FUNC_FAKE_RESET1(closedir);
}

/* EOF */