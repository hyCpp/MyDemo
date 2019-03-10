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
#include "TTS_PlayElementSoundName.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "VC_AudioBufferGeneral.h"
#include "TTS_EngineManager.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::ReturnPointee;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;



/**
* TestTTS_PlayElementSoundName
*
* The class is just for TTS_PlayElementSoundName test. 
*/
class TestTTS_PlayElementSoundName : public testing::Test
{
public:
    TestTTS_PlayElementSoundName() {}
    virtual ~TestTTS_PlayElementSoundName() {}

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

public:
};

/**
* MockEngineBuffers 
*
* The class is just for TTS_PlayElementSoundName test. 
*/
class MockEngineBuffers : public TTS_EngineBuffers
{
    public:
        MOCK_METHOD1(GetWriteBuffer, BYTE*(int& len));
        MOCK_METHOD0(SetWriteLen, void());
};

TEST_F(TestTTS_PlayElementSoundName, DoPlay_NULL)
{
    TTS_PlayElementSoundName element;
    element.DoPlay(NULL);
}

TEST_F(TestTTS_PlayElementSoundName, DoPlay_invalid_file)
{
    TTS_PlayElementSoundName element;
    element.m_bValidfile = false;
    TTS_EngineBuffers buff;
    element.DoPlay(&buff);
}

TEST_F(TestTTS_PlayElementSoundName, DoPlay_empty_file)
{
    TTS_PlayElementSoundName element;

    TTS_EngineBuffers buff;
    element.DoPlay(&buff);
}

TEST_F(TestTTS_PlayElementSoundName, DoPlay_no_buffer)
{
    TTS_PlayElementSoundName element;
    MockEngineBuffers buff;

    FILE *file = fopen("/data/ttstest.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);

    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "fmt ", 4);
    format.dwChunkSize = sizeof(TTS_WaveFile::WAVE_FORMAT);

    fwrite(&format, sizeof(format), 1, file);


    TTS_WaveFile::WAVE_FORMAT wavFormat;
    wavFormat.wFormatTag = 1;
    wavFormat.wBitsPerSample = 16;

    fwrite(&wavFormat, sizeof(wavFormat), 1, file);

    TTS_WaveFile::CHUNK_HEADER data;
    strncpy(data.szHeader, "data", 4);
    data.dwChunkSize = 100;

    fwrite(&data, sizeof(data), 1, file);

    char ch = '0';
    for (int i = 0; i < 100; i++) {
        fwrite(&ch, sizeof(char), 1, file);
    }

    TTS_WaveFile::CHUNK_HEADER fact;
    strncpy(fact.szHeader, "fact", 4);
    data.dwChunkSize = 4;

    fwrite(&fact, sizeof(fact), 1, file);

    fclose(file);


    element.m_waveFile.ReadFile("/data/ttstest.wav");


    EXPECT_CALL(buff, GetWriteBuffer(_)).WillOnce(ReturnNull());
    element.DoPlay(&buff);
}

TEST_F(TestTTS_PlayElementSoundName, DoPlay_no_buffer_2)
{
    TTS_PlayElementSoundName element;
    MockEngineBuffers buff;
    
    FILE *file = fopen("/data/ttstest.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);

    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "fmt ", 4);
    format.dwChunkSize = sizeof(TTS_WaveFile::WAVE_FORMAT);

    fwrite(&format, sizeof(format), 1, file);


    TTS_WaveFile::WAVE_FORMAT wavFormat;
    wavFormat.wFormatTag = 1;
    wavFormat.wBitsPerSample = 16;

    fwrite(&wavFormat, sizeof(wavFormat), 1, file);

    TTS_WaveFile::CHUNK_HEADER data;
    strncpy(data.szHeader, "data", 4);
    data.dwChunkSize = 100;

    fwrite(&data, sizeof(data), 1, file);

    char ch = '0';
    for (int i = 0; i < 100; i++) {
        fwrite(&ch, sizeof(char), 1, file);
    }

    TTS_WaveFile::CHUNK_HEADER fact;
    strncpy(fact.szHeader, "fact", 4);
    data.dwChunkSize = 4;

    fwrite(&fact, sizeof(fact), 1, file);

    fclose(file);


    element.m_waveFile.ReadFile("/data/ttstest.wav");


    BYTE buffer;
    EXPECT_CALL(buff, GetWriteBuffer(_)).WillOnce(DoAll(SetArgReferee<0>(0), Return(&buffer)));
    element.DoPlay(&buff);
}

TEST_F(TestTTS_PlayElementSoundName, DoPlay_file_short)
{
    TTS_PlayElementSoundName element;
    MockEngineBuffers buff;
    
    FILE *file = fopen("/data/ttstest.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);

    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "fmt ", 4);
    format.dwChunkSize = sizeof(TTS_WaveFile::WAVE_FORMAT);

    fwrite(&format, sizeof(format), 1, file);


    TTS_WaveFile::WAVE_FORMAT wavFormat;
    wavFormat.wFormatTag = 1;
    wavFormat.wBitsPerSample = 16;

    fwrite(&wavFormat, sizeof(wavFormat), 1, file);

    TTS_WaveFile::CHUNK_HEADER data;
    strncpy(data.szHeader, "data", 4);
    data.dwChunkSize = 100;

    fwrite(&data, sizeof(data), 1, file);

    char ch = '0';
    for (int i = 0; i < 100; i++) {
        fwrite(&ch, sizeof(char), 1, file);
    }

    TTS_WaveFile::CHUNK_HEADER fact;
    strncpy(fact.szHeader, "fact", 4);
    data.dwChunkSize = 4;

    fwrite(&fact, sizeof(fact), 1, file);

    fclose(file);


    element.m_waveFile.ReadFile("/data/ttstest.wav");


    BYTE buffer[100]; 
    EXPECT_CALL(buff, GetWriteBuffer(_)).WillOnce(DoAll(SetArgReferee<0>(4096), Return(buffer)));
    element.DoPlay(&buff);
}

TEST_F(TestTTS_PlayElementSoundName, DoPlay_file_right)
{
    TTS_PlayElementSoundName element;
    MockEngineBuffers buff;
    
    FILE *file = fopen("/data/ttstest.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);

    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "fmt ", 4);
    format.dwChunkSize = sizeof(TTS_WaveFile::WAVE_FORMAT);

    fwrite(&format, sizeof(format), 1, file);


    TTS_WaveFile::WAVE_FORMAT wavFormat;
    wavFormat.wFormatTag = 1;
    wavFormat.wBitsPerSample = 16;

    fwrite(&wavFormat, sizeof(wavFormat), 1, file);

    TTS_WaveFile::CHUNK_HEADER data;
    strncpy(data.szHeader, "data", 4);
    data.dwChunkSize = 100;

    fwrite(&data, sizeof(data), 1, file);

    char ch = '0';
    for (int i = 0; i < 100; i++) {
        fwrite(&ch, sizeof(char), 1, file);
    }

    TTS_WaveFile::CHUNK_HEADER fact;
    strncpy(fact.szHeader, "fact", 4);
    data.dwChunkSize = 4;

    fwrite(&fact, sizeof(fact), 1, file);

    fclose(file);


    element.m_waveFile.ReadFile("/data/ttstest.wav");


    BYTE buffer[100]; 
    EXPECT_CALL(buff, GetWriteBuffer(_)).WillOnce(DoAll(SetArgReferee<0>(100), Return(buffer)));
    element.DoPlay(&buff);
}

TEST_F(TestTTS_PlayElementSoundName, DoPlay_file_fail)
{
    TTS_PlayElementSoundName element;
    MockEngineBuffers buff;

    element.m_waveFile.ReadFile("/data/ttstesterror.wav");
    element.m_waveFile.m_dataLen = 100;

    BYTE buffer[100]; 
    EXPECT_CALL(buff, GetWriteBuffer(_)).WillOnce(DoAll(SetArgReferee<0>(100), Return(buffer)));
    element.DoPlay(&buff);
}

/* EOF */
