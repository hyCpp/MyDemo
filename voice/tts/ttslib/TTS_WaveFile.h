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

/**
 * @file TTS_WaveFile.h 
 * @brief Declaration file of class TTS_WaveFile. 
 *
 * This file includes the declaration of class TTS_WaveFile and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef CXX_TTS_WAVEFILE_H
#define CXX_TTS_WAVEFILE_H

#include "BL_FullFile.h"
#include <stdio.h>
#include <string>
// class TTS_SoundData;

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* TTS_WaveFile 
*
* The class is just for TTS_WaveFile.
*/
class TTS_WaveFile
{
public:
    TTS_WaveFile();
    virtual ~TTS_WaveFile();

    bool ReadFile(const std::string& fileName);
    bool ReadData(char* data, const size_t begin, const size_t len);

    DWORD GetSampleRate() const
    {
        return m_sFormatHeader.dwSamplesPerSec;
    }

    WORD  GetChannel() const 
    {
        return m_sFormatHeader.wChannels;
    }

    DWORD  GetDataLen() const
    {
        return m_dataLen; 
    }

private:
    
    struct RIFF_HEADER
    {
        char szRiffID[4];		// 'R', 'I', 'F', 'F'
        DWORD dwRiffSize;
        char szRiffFormat[4];	// 'W', 'A', 'V', 'E'
    };

    struct WAVE_FORMAT
    {
        WORD wFormatTag;
        WORD wChannels;
        DWORD dwSamplesPerSec;
        DWORD dwAvgBytesPerSec;
        WORD wBlockAlign;
        WORD wBitsPerSample;
    };

    struct FMT_BLOCK
    {
        char  szFmtID[4];		// 'f', 'm', 't', ' '
        DWORD  dwFmtSize;
        WAVE_FORMAT wavFormat;
    };

    struct CHUNK_HEADER
    {
        char  szHeader[4];
        DWORD  dwChunkSize;
    };

private:
    TTS_WaveFile(const TTS_WaveFile&);
    TTS_WaveFile & operator = (const TTS_WaveFile&);

    bool GetRiffHeaderFromFile();
    bool GetFormatHeaderFromFile(const size_t headerSize);

    bool GetSubChunks();

private:
    RIFF_HEADER				m_sRiffHeader;
    WAVE_FORMAT             m_sFormatHeader;
    size_t                  m_dataLen;
    size_t                  m_dataPos;
    FILE				    *m_file;
};

#endif
/* EOF */
