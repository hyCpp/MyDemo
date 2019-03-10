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
#include "TTS_WaveFile.h"
#include <errno.h>

const WORD WAVE_FORMAT_PCM = 1;

const CHAR* TTS_WAVEFILE_CHUNK_RIFF_ID = "RIFF";
const CHAR* TTS_WAVEFILE_CHUNK_FMT_ID = "fmt ";
const CHAR* TTS_WAVEFILE_CHUNK_FACT_ID = "fact";
const CHAR* TTS_WAVEFILE_CHUNK_DATA_ID = "data";

TTS_WaveFile::TTS_WaveFile()
    : m_sRiffHeader(),
      m_sFormatHeader(),
      m_dataLen(0),
      m_dataPos(0),
      m_file(NULL)
{

}

TTS_WaveFile::~TTS_WaveFile()
{
    if (NULL != m_file) {
        fclose(m_file);
        m_file = NULL;
    }
}

bool TTS_WaveFile::ReadFile(const std::string& fileName)
{
    m_file = fopen(fileName.c_str(), "rb");
    if (!m_file) {
        TTS_LOGE("fopen: %s fail, err code is %d", fileName.c_str(), errno);
        return false;
    }

    if (!GetRiffHeaderFromFile()) {
        TTS_LOGE("Getriff Err: %s", fileName.c_str());
        return false;
    }

    if (!GetSubChunks()) {
        TTS_LOGE("SubChunks Err: %s", fileName.c_str());
        return false;
    }

    return true;
}

bool TTS_WaveFile::ReadData(char* data, const size_t begin, const size_t len)
{
    if (m_file == NULL || data == NULL) {
        return false;
    }

    if (0 != fseek(m_file, m_dataPos + begin, SEEK_SET)) {
        TTS_LOGE("fseek,pos=%d,need=%d", m_dataPos, begin);
        return false;
    }

    size_t readlen = fread(data, sizeof(char), len, m_file);
    if (len != readlen) {
        TTS_LOGE("read,need=%d,read=%d", len, readlen);
        return false;
    }

    return true; 
}

bool TTS_WaveFile::GetSubChunks()
{
    CHUNK_HEADER header;

    size_t readlen = fread(&header, sizeof(char), sizeof(header),  m_file);

    bool bFmtHeader = false;
    bool bDataChunk = false;

    while (readlen == sizeof(header)) {
       std::string str;
       str.append(header.szHeader, 4);

       TTS_LOGI("head is %s", str.c_str());
        
       if (str == TTS_WAVEFILE_CHUNK_FMT_ID) {
           if (!GetFormatHeaderFromFile(header.dwChunkSize)) {
               TTS_LOGE("Format Header error!");
               return false;
           }

           bFmtHeader = true;
       } 
       else if (str == TTS_WAVEFILE_CHUNK_DATA_ID) {
            m_dataLen = header.dwChunkSize;
            m_dataPos = ftell(m_file);

            if (0 != fseek(m_file, header.dwChunkSize, SEEK_CUR)) {
               return false;
            }

            bDataChunk = true;
       }
       else {
           
           if (str.empty()) {
               break;
           }
           
           if (header.dwChunkSize <= 0) {
               break;
           }

           if (0 != fseek(m_file, header.dwChunkSize, SEEK_CUR)) {
               break;
           }
       }

       readlen = fread(&header, sizeof(char), sizeof(header), m_file);
    }

    return bFmtHeader && bDataChunk;
}

bool TTS_WaveFile::GetRiffHeaderFromFile()
{
    size_t dwRead = fread(&m_sRiffHeader, sizeof(m_sRiffHeader), 1, m_file);
    if (dwRead != 1) {
        TTS_LOGE("GetRiff fread fail");
        TTS_LOGE("m_RifHead=%d,Read=%d", sizeof(m_sRiffHeader), dwRead);
        return false;
    }
    
    if (0 != _strnicmp(m_sRiffHeader.szRiffID, "RIFF", 4)) {
        TTS_LOGE("can not find RIFF");
        return false;
    }
    
    if (0 != _strnicmp(m_sRiffHeader.szRiffFormat, "WAVE", 4)) {
        TTS_LOGE("can not find WAVE");
        return false;
    }

    return true;
}

bool TTS_WaveFile::GetFormatHeaderFromFile(const size_t headerSize)
{
    size_t dwRead = fread(&m_sFormatHeader, sizeof(m_sFormatHeader), 1, m_file);

    TTS_LOGI("m_FormatHead=%d", sizeof(m_sFormatHeader));
    TTS_LOGI("Read=%d,headSize=%d", dwRead, headerSize);

    if (headerSize > sizeof(m_sFormatHeader)) {
        if (0 != fseek(m_file, headerSize - sizeof(m_sFormatHeader), SEEK_CUR)) {
            return false;
        }
    }

    if (dwRead != 1) {
        TTS_LOGE("dwRead Err=%d", dwRead);
        return false; 
    }

    // PCM format
    if (WAVE_FORMAT_PCM != m_sFormatHeader.wFormatTag) {
        TTS_LOGE("formtTag wrong: %d", m_sFormatHeader.wFormatTag);
        return false;
    }

    // 16 bits samples per second
    if (16 != m_sFormatHeader.wBitsPerSample) {
        TTS_LOGE("bits per wrong=%d", m_sFormatHeader.wBitsPerSample);
        return false;
    }


    return true;
}
/* EOF */
