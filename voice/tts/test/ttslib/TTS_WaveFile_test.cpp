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

#ifndef CXX_BL_FILEPUBLICDEF_H
#    include "BL_FilePublicDef.h"
#endif

#ifndef CXX_TTS_WAVEFILE_H
#    include "TTS_WaveFile.h"
#endif


/**
* TTS_WaveFile_Test
*
* The class is just for TTS_WaveFile test.
*/
class TTS_WaveFile_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

public:
};


TEST_F(TTS_WaveFile_Test, GoodWaveFile)
{
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
    data.dwChunkSize = 4096 * 2;

    fwrite(&data, sizeof(data), 1, file);

    char ch = '0';
    for (int i = 0; i < 4096 * 2; i++) {
        fwrite(&ch, sizeof(char), 1, file);
    }

    TTS_WaveFile::CHUNK_HEADER fact;
    strncpy(fact.szHeader, "fact", 4);
    data.dwChunkSize = 4;

    fwrite(&fact, sizeof(fact), 1, file);

    fclose(file);


    TTS_WaveFile ttsFile;

    EXPECT_TRUE(ttsFile.ReadFile("/data/ttstest.wav"));
}


TEST_F(TTS_WaveFile_Test, Wave_ERROR_RiffHeader)
{
    FILE *file = fopen("/data/ttsriff.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RICF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttsriff.wav"));
}

TEST_F(TTS_WaveFile_Test, Wave_ERROR_SubChunks)
{
    FILE *file = fopen("/data/ttschunk.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);
    
    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "erro", 4);
    format.dwChunkSize = sizeof(format);

    fwrite(&format, sizeof(format), 1, file);
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttschunk.wav"));
}

TEST_F(TTS_WaveFile_Test, GetSubChunks_empty_subchunk)
{
    FILE *file = fopen("/data/ttstest.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);
    
    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "\0", 1);
    format.dwChunkSize = sizeof(format);

    fwrite(&format, sizeof(format), 1, file);
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}

TEST_F(TTS_WaveFile_Test, GetSubChunks_error_size)
{
    FILE *file = fopen("/data/ttstest.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);
    
    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "fmt ", 4);
    format.dwChunkSize = 0;

    fwrite(&format, sizeof(format), 1, file);
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}

TEST_F(TTS_WaveFile_Test, GetSubChunks_error_size_2)
{
    FILE *file = fopen("/data/ttstest.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);
    
    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "fmt ", 4);
    format.dwChunkSize = 10;

    fwrite(&format, sizeof(format), 1, file);
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}

TEST_F(TTS_WaveFile_Test, GetRiffHeaderFromfile_erros_size)
{
    FILE *file = fopen("/data/ttstest.wav", "wb");

    char ch = 'c';
    fwrite(&ch, sizeof(ch), 1, file);
    
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}

TEST_F(TTS_WaveFile_Test, GetRiffHeaderFromfile_erros_FIFF)
{
    FILE *file = fopen("/data/ttstest.wav", "wb");

    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RICF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);
    
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}

TEST_F(TTS_WaveFile_Test, GetRiffHeaderFromfile_erros_WAVE)
{
    FILE *file = fopen("/data/ttstest.wav", "wb");

    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAEE", 4);

    fwrite(&riff, sizeof(riff), 1, file);
    
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}


TEST_F(TTS_WaveFile_Test, GetFormatHeaderFromFile_err_long)
{
    FILE *file = fopen("/data/ttstest.wav", "wb");
    TTS_WaveFile:: RIFF_HEADER riff;
    strncpy(riff.szRiffID, "RIFF", 4);
    strncpy(riff.szRiffFormat, "WAVE", 4);

    fwrite(&riff, sizeof(riff), 1, file);
    
    TTS_WaveFile::CHUNK_HEADER format;
    strncpy(format.szHeader, "fmt ", 4);
    format.dwChunkSize = sizeof(TTS_WaveFile::WAVE_FORMAT) + 10;

    fwrite(&format, sizeof(format), 1, file);
    
    TTS_WaveFile::WAVE_FORMAT wavFormat;
    wavFormat.wFormatTag = 1;
    wavFormat.wBitsPerSample = 16;

    fwrite(&wavFormat, sizeof(wavFormat), 1, file);
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}

TEST_F(TTS_WaveFile_Test, GetFormatHeaderFromFile_err_tag)
{
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
    wavFormat.wFormatTag = 2;
    wavFormat.wBitsPerSample = 16;

    fwrite(&wavFormat, sizeof(wavFormat), 1, file);
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}

TEST_F(TTS_WaveFile_Test, GetFormatHeaderFromFile_err_sample)
{
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
    wavFormat.wBitsPerSample = 10;

    fwrite(&wavFormat, sizeof(wavFormat), 1, file);
    fclose(file);

    TTS_WaveFile ttsFile;

    EXPECT_FALSE(ttsFile.ReadFile("/data/ttstest.wav"));
}

TEST_F(TTS_WaveFile_Test, ReadData_file_null) 
{
    TTS_WaveFile ttsFile;
    EXPECT_FALSE(ttsFile.ReadData(NULL, 1, 1));
}


TEST_F(TTS_WaveFile_Test, ReadData_with_file)
{
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
    data.dwChunkSize = 4096 * 2;

    fwrite(&data, sizeof(data), 1, file);

    char ch = '0';
    for (int i = 0; i < 4096 * 2; i++) {
        fwrite(&ch, sizeof(char), 1, file);
    }

    TTS_WaveFile::CHUNK_HEADER fact;
    strncpy(fact.szHeader, "fact", 4);
    data.dwChunkSize = 4;

    fwrite(&fact, sizeof(fact), 1, file);

    fclose(file);


    TTS_WaveFile ttsFile;

    EXPECT_TRUE(ttsFile.ReadFile("/data/ttstest.wav"));
    
    EXPECT_FALSE(ttsFile.ReadData(NULL, 1, 1));
   
    char buffer[100]; 

    EXPECT_FALSE(ttsFile.ReadData(buffer, 4096 * 2 + 10, 100));
    
    EXPECT_FALSE(ttsFile.ReadData(buffer, 4096 * 2 - 50, 100));
    
    EXPECT_TRUE(ttsFile.ReadData(buffer, 0, 100));
}

/* EOF */
