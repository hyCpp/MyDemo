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
#include "VC_CommonDef.h"
#include "VC_WavFileWriter.h"
#include "VC_FileTest_fff.h"


using ::testing::Return;
using ::testing::_;

/**
 * Summary for class VC_WavFileWriter_test
 * 
 * This is a detail description for class VC_WavFileWriter_test
 *
 */
class VC_WavFileWriter_test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_writer = new VC_WavFileWriter();
    }

    virtual void TearDown()
    {
        if (NULL != m_writer) {
            delete m_writer;
            m_writer = NULL;
        }
    }

private:
    VC_WavFileWriter*    m_writer;
};

TEST_F(VC_WavFileWriter_test, Openm_pFileNotNULL)
{
    VC_WAVEFORMATEX format;
    std::string filename = "/tmp/mengfanbing.txt";
    m_writer->m_pFile = fopen(filename.c_str(), "wb");

    EXPECT_FALSE(m_writer->Open(filename, format));
}

TEST_F(VC_WavFileWriter_test, Openm_pFileOpenFailed)
{
    std::string filename = "/NO/NoExist.txt";
    VC_WAVEFORMATEX format;

    EXPECT_FALSE(m_writer->Open(filename, format));
}

TEST_F(VC_WavFileWriter_test, OpenSuccess)
{
    std::string filename = "/tmp/suntec.txt";
    VC_WAVEFORMATEX format;

    // why?
    EXPECT_FALSE(m_writer->Open(filename, format));
}

TEST_F(VC_WavFileWriter_test, OpenfilenameiFalse)
{
    std::string filename = "/tmp/suntec.txt";
    m_writer->m_pFile = fopen(filename.c_str(), "wb");

    EXPECT_FALSE(m_writer->Open(filename));
}

TEST_F(VC_WavFileWriter_test, Openfilenameisuccess)
{
    std::string filename = "/tmp/suntec.txt";

    EXPECT_TRUE(m_writer->Open(filename));
}

TEST_F(VC_WavFileWriter_test, Writem_pFileNULL)
{
    m_writer->m_pFile = NULL;
    int i = 10;
    int len = 4;

    EXPECT_EQ(-1, m_writer->Write(&i, len));
}

TEST_F(VC_WavFileWriter_test, WritedataNULL)
{
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    int* p_i = NULL;
    int len = 4;

    EXPECT_EQ(-1, m_writer->Write(p_i, len));
}

TEST_F(VC_WavFileWriter_test, Writelensmallerthanzero)
{
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    int i = 11;
    int len = -1;

    EXPECT_EQ(-1, m_writer->Write(&i, len));
}

TEST_F(VC_WavFileWriter_test, WriteSuccess)
{
    m_writer->m_pFile = fopen("tmp/suntec.txt", "wb");
    int i = 11;
    int len = 4;

    EXPECT_TRUE(m_writer->Write(&i, len) > 0);
}

TEST_F(VC_WavFileWriter_test, Closem_pFileNULL)
{
    m_writer->m_pFile = NULL;

    EXPECT_FALSE(m_writer->Close());
}

TEST_F(VC_WavFileWriter_test, CloseSuccess)
{
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    m_writer->m_lDataLen = 11;
    m_writer->m_lDataLenSub = 0;

    EXPECT_TRUE(m_writer->Close());
}

TEST_F(VC_WavFileWriter_test, Close_WriteWaveLenFalse)
{
    m_writer->m_pFile = fopen("/tmp/Close_WriteWaveLenFalse.txt", "wb");
    m_writer->m_lDataLenSub = 0;
    m_writer->m_lDataLen = 11;

    // FUNC_FAKE_SEQRET_SET(fwrite, size_t, myReturnVals, 20, 10, 0);
    FUNC_FAKE_SINGRET_SET(fwrite, 1);

    bool bret = m_writer->Close();
    EXPECT_FALSE(bret);
    FUNC_FAKE_RESET1(fwrite);
}

TEST_F(VC_WavFileWriter_test, Close_fcloseFalse)
{
    m_writer->m_pFile = fopen("/tmp/Close_fcloseFalse.txt", "wb");
    m_writer->m_lDataLenSub = 0;
    m_writer->m_lDataLen = 11;

    FUNC_FAKE_SINGRET_SET(fclose, 1);
    bool bret = m_writer->Close();
    EXPECT_FALSE(bret);

    FUNC_FAKE_RESET1(fclose);
}

TEST_F(VC_WavFileWriter_test, WriteWaveLen_SecondFalse)
{
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    m_writer->m_lDataLenSub = 0;
    m_writer->m_lDataLen = 11;

    FUNC_FAKE_SEQRET_SET(fwrite, size_t, myReturnVals, 4, 10);
    EXPECT_FALSE(m_writer->WriteWaveLen());
    FUNC_FAKE_RESET1(fwrite);
}

TEST_F(VC_WavFileWriter_test, WriteWaveLen_ThirdFalse)
{
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    m_writer->m_lDataLenSub = 0;
    m_writer->m_lDataLen = 11;

    FUNC_FAKE_SEQRET_SET(fwrite, size_t, myReturnVals, 4, 1, 11);
    EXPECT_FALSE(m_writer->WriteWaveLen());
    FUNC_FAKE_RESET1(fwrite);
}

TEST_F(VC_WavFileWriter_test, WriteWaveRiffInfofwritefalse)
{
    FUNC_FAKE_SINGRET_SET(fwrite, 1);
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    EXPECT_FALSE(m_writer->WriteWaveRiffInfo());
    FUNC_FAKE_RESET1(fwrite);
}

TEST_F(VC_WavFileWriter_test, WriteWaveHeader_WriteWaveRiffInfoFalse)
{
    VC_WAVEFORMATEX format;
    memcpy(format.fId, "fmt ", 4);
    format.pcm_header_len = 16;
    format.wFormatTag = 1;
    format.nChannels = 1;
    format.nSamplesPerSec = 16000;
    format.nBitsPerSample = 16;
    format.nAvgBytesPerSec = format.nChannels * format.nSamplesPerSec * (format.nBitsPerSample / 8);
    format.nBlockAlign = format.nChannels*(format.nBitsPerSample / 8);

    FUNC_FAKE_SINGRET_SET(fwrite, 1);
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    EXPECT_FALSE(m_writer->WriteWaveHeader(format));
    FUNC_FAKE_RESET1(fwrite);

}

TEST_F(VC_WavFileWriter_test, WriteWaveHeader_WriteWaveDataInfoFalse)
{
    VC_WAVEFORMATEX format;
    memcpy(format.fId, "fmt ", 4);
    format.pcm_header_len = 16;
    format.wFormatTag = 1;
    format.nChannels = 1;
    format.nSamplesPerSec = 16000;
    format.nBitsPerSample = 16;
    format.nAvgBytesPerSec = format.nChannels * format.nSamplesPerSec * (format.nBitsPerSample / 8);
    format.nBlockAlign = format.nChannels*(format.nBitsPerSample / 8);

    FUNC_FAKE_SEQRET_SET(fwrite, size_t, myReturnVals, 12, 24, 1);
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    EXPECT_FALSE(m_writer->WriteWaveHeader(format));
    FUNC_FAKE_RESET1(fwrite);

}

TEST_F(VC_WavFileWriter_test, WriteWaveFmtInfofwriteFalse)
{
    VC_WAVEFORMATEX format;
    memcpy(format.fId, "fmt ", 4);
    format.pcm_header_len = 16;
    format.wFormatTag = 1;
    format.nChannels = 1;
    format.nSamplesPerSec = 16000;
    format.nBitsPerSample = 16;
    format.nAvgBytesPerSec = format.nChannels * format.nSamplesPerSec * (format.nBitsPerSample / 8);
    format.nBlockAlign = format.nChannels*(format.nBitsPerSample / 8);

    FUNC_FAKE_SINGRET_SET(fwrite, 1);
    m_writer->m_pFile = fopen("/tmp/suntec.txt", "wb");
    EXPECT_FALSE(m_writer->WriteWaveFmtInfo(format));
    FUNC_FAKE_RESET1(fwrite);

}

/* EOF */