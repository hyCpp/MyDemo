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
#include "VC_WavFile.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "VC_FileTest_fff.h"

using ::testing::Return;
using ::testing::_;

// header of wav file
struct WAV_HDR
{
    char rID[4];                // 'RIFF'
    long int rLen;              // member description for rLen
    char wID[4];                // 'WAVE'
    char fId[4];                // 'fmt '
    long int pcm_header_len;    // varies...
    short int wFormatTag;       // member description for wFormatTag
    short int nChannels;        // 1, 2 for stereo data is (l, r) pairs
    long int nSamplesPerSec;    // member description for nSamplesPerSec
    long int nAvgBytesPerSec;   // member description for nAvgBytesPerSec
    short int nBlockAlign;      // member description for nBlockAlign
    short int nBitsPerSample;   // member description for nBitsPerSample
};

// header of chunk
struct CHUNK_HDR
{
    char dId[4];            // 'data' or 'fact'
    long int dLen;          // data length
//  unsigned char *data;
};

/**
 * Summary for class VC_WavFile_test
 * 
 * This is a detail description for class VC_WavFile_test
 *
 */
class VC_WavFile_test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_wavfile = new VC_WavFile();
    }

    virtual void TearDown()
    {
        if (NULL != m_wavfile) {
            delete m_wavfile;
            m_wavfile = NULL;
        }
    }

private:
    VC_WavFile*    m_wavfile;
};

TEST_F(VC_WavFile_test, OpenbWriteTrue)
{
    std::string filename = "/tmp/suntec.txt";
    int sampleRate = 22050;
    bool bwrite = true;

    m_wavfile->Open(filename, sampleRate, bwrite);
}

TEST_F(VC_WavFile_test, OpenbWriteFalse)
{
    std::string filename = "/tmp/suntec.txt";
    int sampleRate = 22050;
    bool bwrite = false;

    m_wavfile->Open(filename, sampleRate, bwrite);
}

TEST_F(VC_WavFile_test, OpenWm_pFillNotNULL)
{
    m_wavfile->m_pFile = fopen("/tmp/meng.txt", "wb");
    bool bret = m_wavfile->OpenW(std::string("/tmp/suntec.txt"), 48000);

    EXPECT_TRUE(m_wavfile);
}

TEST_F(VC_WavFile_test, OpenWOpenFileFailed)
{
    bool bret = m_wavfile->OpenW("/meng/suntec.txt", 48000);

    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenROpenFileFailed)
{
    std::string filename = "/tmp/noExist.txt";
    bool bret = m_wavfile->OpenR(filename, 22050);

    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenROpenFileSuccessRateWrong)
{
    std::string filename = "/tmp/suntec.txt";

    bool bret = m_wavfile->OpenR(filename, 22050);

    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenROpenFileSuccessRateSuccess)
{
    std::string filename = "/tmp/suntec.txt";

    bool bret = m_wavfile->OpenR(filename, 48000);

    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenRCheckHeaderFailed)
{
    // CheckFailed, other two true
    // check all wrong
    std::string filename = "/tmp/VC_WavFile_CheckHeaderFailed.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RITT", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WATT", 4);
    memcpy(&hdr.fId, "fmtt", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    CHUNK_HDR chdr;
    memcpy(&chdr.dId, "data", 4);
    chdr.dLen = 0;
    fwrite(&chdr, 1, sizeof(chdr), p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4800);
    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenRCheckHeaderFalseSecondTrue)
{
    // Rate Wrong, other two true
    // check second true
    std::string filename = "/tmp/VC_WavFile_CheckHeaderFailed.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RITT", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    CHUNK_HDR chdr;
    memcpy(&chdr.dId, "data", 4);
    chdr.dLen = 0;
    fwrite(&chdr, 1, sizeof(chdr), p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4801);
    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenRCheckHeaderFalseThirdTrue)
{
    // Rate Wrong, other two true
    // check third true
    std::string filename = "/tmp/VC_WavFile_CheckHeaderFailed.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RITT", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WATT", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    CHUNK_HDR chdr;
    memcpy(&chdr.dId, "data", 4);
    chdr.dLen = 0;
    fwrite(&chdr, 1, sizeof(chdr), p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4801);
    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenRCheckHeaderTrueRateNotEqual)
{
    // Rate Wrong, other two true
    // check all true
    std::string filename = "/tmp/VC_WavFile_CheckRateFailed.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RIFF", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    CHUNK_HDR chdr;
    memcpy(&chdr.dId, "data", 4);
    chdr.dLen = 0;
    fwrite(&chdr, 1, sizeof(chdr), p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4801);
    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenRfreadHDR_check_chunk_headerfirstfactseconddatt)
{
    // Rate Wrong, other two true
    // check all true
    std::string filename = "/tmp/VC_WavFile_CheckChunckfactdatt.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RIFF", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    CHUNK_HDR chdr;
    memcpy(&chdr.dId, "fact", 4);
    chdr.dLen = 4;
    fwrite(&chdr, 1, sizeof(chdr), p_file);

    CHUNK_HDR chdrdata;
    memcpy(&chdrdata.dId, "datt", 4);
    chdrdata.dLen = 4;
    fwrite(&chdrdata, 1, sizeof(chdrdata), p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4800);
    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenRfreadHDR_check_chunk_headerfirstdatadLenZero)
{
    // Rate Wrong, other two true
    // check all true
    std::string filename = "/tmp/VC_WavFile_CheckChunckdataAllTrue.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RIFF", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    // CHUNK_HDR chdr;
    // memcpy(&chdr.dId, "fact", 4);
    // chdr.dLen = 4;
    // fwrite(&chdr, 1, sizeof(chdr), p_file);

    CHUNK_HDR chdrdata;
    memcpy(&chdrdata.dId, "data", 4);
    chdrdata.dLen = 0;
    fwrite(&chdrdata, 1, sizeof(chdrdata), p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4800);
    EXPECT_TRUE(bret);
}

TEST_F(VC_WavFile_test, OpenRfreadHDR_check_chunk_headerfirstdatadLenNotZero)
{
    // Rate Wrong, other two true
    // check all true
    std::string filename = "/tmp/VC_WavFile_CheckChunckdataLenWrong.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RIFF", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    // CHUNK_HDR chdr;
    // memcpy(&chdr.dId, "fact", 4);
    // chdr.dLen = 4;
    // fwrite(&chdr, 1, sizeof(chdr), p_file);

    CHUNK_HDR chdrdata;
    memcpy(&chdrdata.dId, "data", 4);
    chdrdata.dLen = 110;
    fwrite(&chdrdata, 1, sizeof(chdrdata), p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4800);
    EXPECT_TRUE(bret);
}

TEST_F(VC_WavFile_test, OpenRfreadHDR_check_chunk_headerfirstNotdataFalse)
{
    // Rate Wrong, other two true
    // check all true
    std::string filename = "/tmp/VC_WavFile_CheckChunckdataWrong.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RIFF", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    // CHUNK_HDR chdr;
    // memcpy(&chdr.dId, "fact", 4);
    // chdr.dLen = 4;
    // fwrite(&chdr, 1, sizeof(chdr), p_file);

    CHUNK_HDR chdrdata;
    memcpy(&chdrdata.dId, "hell", 4);
    chdrdata.dLen = 110;
    fwrite(&chdrdata, 1, sizeof(chdrdata), p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4800);
    EXPECT_FALSE(bret);
}

TEST_F(VC_WavFile_test, OpenRfreadHDR_check_chunk_headerCountZero)
{
    // Rate Wrong, other two true
    // check all true
    std::string filename = "/tmp/VC_WavFile_CheckChunckdataCountZero.txt";

    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "wb");

    WAV_HDR hdr;
    memcpy(&hdr.rID, "RIFF", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = 4800;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*4800*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), p_file);

    char buffer[] = "mengfanbingshandongshengtengzhoushi";

    fwrite(buffer, 36, 1, p_file);

    fclose(p_file);
    p_file = NULL;

    bool bret = m_wavfile->OpenR(filename, 4800);
    EXPECT_FALSE(bret);
}



TEST_F(VC_WavFile_test, Writem_pFileNULL)
{
    m_wavfile->m_pFile = NULL;
    char* data = "mengfanbing";
    int len = strlen(data);

    EXPECT_FALSE(m_wavfile->Write((void*)data, len));
}

TEST_F(VC_WavFile_test, WritedataNULL)
{
    m_wavfile->m_pFile = fopen("/tmp/writemeng.txt", "wb");
    char* data = NULL;
    int len = 1;

    EXPECT_FALSE(m_wavfile->Write((void*)data, len));
}

TEST_F(VC_WavFile_test, WriteLenbelowzero)
{
    m_wavfile->m_pFile = fopen("/tmp/writemeng.txt", "wb");
    char* data = "NULL";
    int len = -1;

    EXPECT_FALSE(m_wavfile->Write((void*)data, len));
}

TEST_F(VC_WavFile_test, Write)
{
    m_wavfile->m_pFile = fopen("/tmp/writemeng.txt", "wb");
    char* data = "mengfanbing";
    int len = strlen(data);

    EXPECT_TRUE(m_wavfile->Write((void*)data, len));
}

TEST_F(VC_WavFile_test, Write_write_len121)
{
    m_wavfile->m_pFile = fopen("/tmp/writemeng.txt", "wb");
    char* data = "mengfanbing";
    int len = strlen(data);

    FUNC_FAKE_SINGRET_SET(fwrite, 121);    
    EXPECT_TRUE(m_wavfile->Write((void*)data, len));
    FUNC_FAKE_RESET1(fwrite);
}


TEST_F(VC_WavFile_test, Closem_iDataLennotzero)
{
    m_wavfile->m_pFile = fopen("/tmp/writemeng.txt", "wb");
    m_wavfile->m_iDataLen = 1;

    m_wavfile->Close();
}

TEST_F(VC_WavFile_test, Closem_iDataLenzero)
{
    m_wavfile->m_pFile = fopen("/tmp/writemeng.txt", "wb");
    m_wavfile->m_iDataLen = 0;

    m_wavfile->Close();
}

TEST_F(VC_WavFile_test, Closem_pFileNULL)
{
    m_wavfile->m_pFile = NULL;
    m_wavfile->m_iDataLen = 0;

    m_wavfile->Close();
}

TEST_F(VC_WavFile_test, WriteHeaderLenm_pFileNULL)
{
    m_wavfile->m_pFile = NULL;

    m_wavfile->WriteHeaderLen();
}

TEST_F(VC_WavFile_test, GetAvailableLen)
{
    m_wavfile->GetAvailableLen();
}

TEST_F(VC_WavFile_test, ReadbufferNULL)
{
    char* buffer = NULL;
    int len = 10;

    EXPECT_EQ(0, m_wavfile->Read((void*)buffer, len));
}

TEST_F(VC_WavFile_test, ReadLenZero)
{
    char* buffer = "mengfanbing";
    int len = 0;

    EXPECT_EQ(0, m_wavfile->Read((void*)buffer, len));
}

TEST_F(VC_WavFile_test, ReadPosEqual)
{
    char* buffer = "mengfanbing";
    int len = 10;

    EXPECT_EQ(-1, m_wavfile->Read((void*)buffer, len));
}

TEST_F(VC_WavFile_test, Read)
{
    char buffer[20] = "mfb";
    memset(buffer, 0, 20);
    int len = 10;
    m_wavfile->m_iDataLen = 10;
    m_wavfile->m_iDataPos = 0;

    m_wavfile->m_pFile = fopen("/tmp/suntec.txt", "rb");

    EXPECT_FALSE(m_wavfile->Read(buffer, len) > 0);
}

TEST_F(VC_WavFile_test, ReadlenBiggerThanZero)
{
    char buffer[20] = "mfb";
    memset(buffer, 0, 20);

    int len = 10;
    m_wavfile->m_iDataLen = 10;
    m_wavfile->m_iDataPos = 0;

    FILE* p_file = fopen("/tmp/ReadLenBiggerZero.txt", "wb");
    fwrite("mengfanbing", 1, 11, p_file);
    fclose(p_file);
    p_file = NULL;

    m_wavfile->m_pFile = fopen("/tmp/ReadLenBiggerZero.txt", "rb");
    int iret = m_wavfile->Read(buffer, len);

    EXPECT_EQ(iret, 10);
}

TEST_F(VC_WavFile_test, WriteHeaderm_pFileNULL)
{
    m_wavfile->m_pFile = NULL;
    m_wavfile->WriteHeader(22050);
}


/* EOF */