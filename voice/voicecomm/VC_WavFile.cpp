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
 * @file VC_WavFile.cpp
 * @brief
 *
 * This file includes the define of class VR_InterpreterImpl.
 *
 * @attention used for C++ only.
 */
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "stdafx.h"

#include "VC_Log.h"
#include "VC_WavFile.h"
#include <errno.h> 

// header of wav file
struct WAV_HDR
{
    char rID[4];                // 'RIFF'
    int32_t rLen;               // member description for rLen
    char wID[4];                // 'WAVE'
    char fId[4];                // 'fmt '
    int32_t pcm_header_len;     // varies...
    int16_t wFormatTag;         // member description for wFormatTag
    int16_t nChannels;          // 1, 2 for stereo data is (l, r) pairs
    int32_t nSamplesPerSec;     // member description for nSamplesPerSec
    int32_t nAvgBytesPerSec;    // member description for nAvgBytesPerSec
    int16_t nBlockAlign;        // member description for nBlockAlign
    int16_t nBitsPerSample;     // member description for nBitsPerSample
};

bool check_wav_header(const WAV_HDR& header)
{
    if ((memcmp(&header.rID, "RIFF", 4) != 0)
        && (memcmp(&header.wID, "WAVE", 4) != 0)
        && (memcmp(&header.fId, "fmt ", 4) != 0)) {
        VC_LOGI("rID=[%c %c%c %c](RIFF)", header.rID[0], header.rID[1], header.rID[2], header.rID[3]);
        VC_LOGI("wID=[%c %c%c %c](WAVE)", header.wID[0], header.wID[1], header.wID[2], header.wID[3]);
        VC_LOGI("fId=[%c %c%c %c](fmt )", header.fId[0], header.fId[1], header.fId[2], header.fId[3]);
        RETURN(false);
    }

    return true;
}

// header of chunk
struct CHUNK_HDR
{
    char dId[4];            // 'data' or 'fact'
    long int dLen;          // data length
//  unsigned char *data;
};


bool check_chunk_header(const CHUNK_HDR& header, const char* chunk_tag)
{
    if (memcmp(&header.dId, chunk_tag, 4) == 0) {
        return true;
    }
    else {
        VC_LOGI("dId=[%c %c%c %c](data)", header.dId[0], header.dId[1], header.dId[2], header.dId[3]);
        RETURN(false);
    }
}

VC_WavFile::VC_WavFile()
    :   m_pFile(NULL)
    ,   m_iDataPos(0)
    ,   m_iDataLen(0)
{
}

VC_WavFile::~VC_WavFile()
{
}

bool VC_WavFile::Open(const std::string &filename, int sampleRate, bool bwrite)
{
    VC_LOGD_FUNC();
    if (bwrite) {
        return OpenW(filename, sampleRate);
    }
    else {
        return OpenR(filename, sampleRate);
    }
}

bool VC_WavFile::OpenW(const std::string &filename, int sampleRate)
{
    VC_LOGD_FUNC();
    if (NULL != m_pFile) {
        fclose(m_pFile);
        m_pFile = NULL;
    }

    m_pFile = fopen(filename.c_str(), "wb");
    if (NULL != m_pFile) {
        WriteHeader(sampleRate);
        m_iDataPos = 0;
        m_iDataLen = 0;
        return true;
    }
    else {
        VC_LOGE("Open file failed , filename=[%s]", filename.c_str());
        RETURN(false);
    }
}

bool VC_WavFile::OpenR(const std::string &filename, int sampleRate)
{
    VC_LOGD_FUNC();
    m_pFile = fopen(filename.c_str(), "rb");
    if (NULL == m_pFile) {
        VC_LOGE("Open file failed , filename=[%s],errnum=[%s]", filename.c_str(),  strerror(errno));
        RETURN(false);
    } 
    else {
        VC_LOGI("filename=%s,sampleRate=%d", filename.c_str(), sampleRate);
    }

    fseek(m_pFile, 0, SEEK_END);
    int size = ftell(m_pFile);
    fseek(m_pFile, 0, SEEK_SET);

    WAV_HDR hdr;
    int count = fread(&hdr, sizeof(hdr), 1, m_pFile);
    size -= sizeof(hdr);

    if ((count < 1) || (!check_wav_header(hdr))
        || sampleRate != hdr.nSamplesPerSec) {
        VC_LOGE("sampleRate req/real=%d/%d", sampleRate, hdr.nSamplesPerSec);
        fclose(m_pFile);
        m_pFile = NULL;
        RETURN(false);
    }

    CHUNK_HDR chdr;
    count = fread(&chdr, sizeof(chdr), 1, m_pFile);
    size -= sizeof(chdr);
    if ((1 == count) && check_chunk_header(chdr, "fact")) {
        fseek(m_pFile, chdr.dLen, SEEK_CUR);
        size -= (chdr.dLen + sizeof(chdr));

        fread(&chdr, sizeof(chdr), 1, m_pFile);
    } 

    if  (!check_chunk_header(chdr, "data")) {
        VC_LOGE("chunk has no data section");
        return false;
    }

    if (size != chdr.dLen) {
        VC_LOGE("data chunk size is incorrect, data.size=%d, file size=%d", chdr.dLen, size);
    }

    m_iDataLen = size;
    m_iDataPos = 0;

    VC_LOGI("Open wav file ok, data len=%d", size);
    return true;
}

int VC_WavFile::Write(void *pData, int iLen)
{
    VC_LOGD_FUNC();
    if (NULL == m_pFile || NULL == pData || iLen < 1) {
        VC_LOGI("invalid param, m_pFile=[%p], pData=[%p], len=[%d]", m_pFile, pData, iLen);
        return 0;
    }

    int write_len = fwrite(pData, iLen, 1, m_pFile);
    if (1 == write_len) {
        m_iDataPos += iLen;
    }

    return write_len;
}

void VC_WavFile::Close()
{
    VC_LOGD_FUNC();
    if (NULL != m_pFile) {
        if (0 == m_iDataLen) { // for write file
            WriteHeaderLen();
        }
        fclose(m_pFile);
        m_pFile = NULL;

        m_iDataPos = 0;
        m_iDataLen = 0;
    }
}

void VC_WavFile::WriteHeader(int sampleRate)
{
    VC_LOGD_FUNC();
    if (NULL == m_pFile) {
        return;
    }
    WAV_HDR hdr;
    memcpy(&hdr.rID, "RIFF", 4);
    hdr.rLen = sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = sampleRate;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels*sampleRate*(hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels*(hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), m_pFile);

    CHUNK_HDR chdr;
    memcpy(&chdr.dId, "data", 4);
    chdr.dLen = 0;
    fwrite(&chdr, 1, sizeof(chdr), m_pFile);
}

void VC_WavFile::WriteHeaderLen()
{
    VC_LOGD_FUNC();
    if (NULL == m_pFile) {
        return;
    }

    DWORD dwTotal = m_iDataPos + sizeof(WAV_HDR) - sizeof(char) * 4 - sizeof(long int) + sizeof(CHUNK_HDR);
    fseek(m_pFile, 4, SEEK_SET);
    fwrite(&dwTotal, 1, 4, m_pFile);

    fseek(m_pFile, 40, SEEK_SET);
    fwrite(&m_iDataPos, 1, 4, m_pFile);
}

int VC_WavFile::GetAvailableLen()
{
    VC_LOGD_FUNC();
    return m_iDataLen - m_iDataPos;
}

int VC_WavFile::Read(void *buffer, int len)
{
    VC_LOGD_FUNC();
    if ((NULL == buffer) || (0 == len)) {
        VC_LOGE("buffer=%p, len=%d", buffer, len);
        return 0;
    }

    if (m_iDataPos >= m_iDataLen) { // No more data
        return -1;
    }

    len = fread(buffer, 1, MIN(len, m_iDataLen - m_iDataPos), m_pFile);
    if (len > 0) {
        m_iDataPos += len;
    }

    return len;
}

/* EOF */
