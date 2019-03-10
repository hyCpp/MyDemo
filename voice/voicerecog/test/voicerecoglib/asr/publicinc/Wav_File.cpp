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

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#else
#endif

#include <string.h>

#include "Wav_File.h"
#include "Record_Log.h"

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

bool check_wav_header(const WAV_HDR& header)
{
    if ((memcmp(&header.rID, "RIFF", 4) != 0)
        && (memcmp(&header.wID, "WAVE", 4) != 0)
        && (memcmp(&header.fId, "fmt ", 4) != 0)) {
        Fn_Log("rID=[%c %c%c %c](RIFF)", header.rID[0], header.rID[1], header.rID[2], header.rID[3]);
        Fn_Log("rID=[%c %c%c %c](RIFF)", header.rID[0], header.rID[1], header.rID[2], header.rID[3]);
        Fn_Log("wID=[%c %c%c %c](WAVE)", header.wID[0], header.wID[1], header.wID[2], header.wID[3]);
        Fn_Log("fId=[%c %c%c %c](fmt )", header.fId[0], header.fId[1], header.fId[2], header.fId[3]);
        return false;
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

bool check_chunk_hander(const CHUNK_HDR& header)
{
    if ((memcmp(&header.dId, "data", 4) == 0)
        || (memcmp(&header.dId, "fact", 4) == 0)) {
        return true;
    }
    else {
        Fn_Log("dId=[%c %c%c %c](data)", header.dId[0], header.dId[1], header.dId[2], header.dId[3]);
        return false;
    }
}

C_Wav_File::C_Wav_File()
    :   m_pFile(NULL)
    ,   m_iDataPos(0)
    ,   m_iDataLen(0)
{
}

bool C_Wav_File::Open(const std::string &filename, int sampleRate, bool write)
{
    if (write) {
        return OpenW(filename, sampleRate);
    }
    else {
        return OpenR(filename, sampleRate);
    }
}

bool C_Wav_File::OpenW(const std::string &filename, int sampleRate)
{
    if (NULL != m_pFile) {
        fclose(m_pFile);
        m_pFile = NULL;
    }
    m_pFile = fopen(filename.c_str(), "wb");
    if (NULL == m_pFile) {
        Fn_Log("Open file failed , filename=[%s]", filename.c_str());
        return false;
    }

    WriteHeader(sampleRate);
    m_iDataPos = 0;
    m_iDataLen = 0;

    return true;
}

bool C_Wav_File::OpenR(const std::string &filename, int sampleRate)
{
    m_pFile = fopen(filename.c_str(), "rb");
    if (NULL == m_pFile) {
        Fn_Log("Open file failed, filename=[%s]", filename.c_str());
        return false;
    }

    fseek(m_pFile, 0, SEEK_END);
    long tot = ftell(m_pFile);
    fseek(m_pFile, 0, SEEK_SET);

    Fn_Log("size of long long :%ld", sizeof(long long));
    Fn_Log("size of long :%ld", sizeof(long));
    Fn_Log("size of long int :%ld", sizeof(long int));

    WAV_HDR hdr;
    Fn_Log("size of WAV_HDR :%ld", sizeof(WAV_HDR));
    size_t count = fread(&hdr, sizeof(hdr), 1, m_pFile);
    if ((count < 1)
        || (!check_wav_header(hdr))
        || sampleRate != hdr.nSamplesPerSec) {
        Fn_Log("sampleRate req real=%d %ld", sampleRate, hdr.nSamplesPerSec);
        fclose(m_pFile);
        m_pFile = NULL;
        return false;
    }

    CHUNK_HDR chdr;
    Fn_Log("size of CHUNK_HDR :%ld", sizeof(CHUNK_HDR));
    chdr.dLen = 0;
    count = fread(&chdr, sizeof(chdr), 1, m_pFile);
    if ((count > 0) && check_chunk_hander(chdr)) {
    }

    m_iDataLen = tot - sizeof(hdr) - sizeof(chdr);
    m_iDataPos = 0;
    Fn_Log("data total len=%d chdr.len=%ld", m_iDataLen, chdr.dLen);
    return true;
}

int C_Wav_File::Write(void *pData, int iLen)
{
    if (NULL == m_pFile || NULL == pData || iLen < 1) {
        Fn_Log("invalid param, m_pFile=[%p], pData=[%p], len=[%d]", m_pFile, pData, iLen);
        return 0;
    }

    int write_len = (int)fwrite(pData, iLen, 1, m_pFile);
    m_iDataPos += write_len;

    return write_len;
}

void C_Wav_File::Close()
{
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

void C_Wav_File::WriteHeader(int sampleRate)
{
    if (NULL == m_pFile) {
        return;
    }
    WAV_HDR hdr;
    memcpy(&hdr.rID, "RIFF", 4);
    hdr.rLen = sizeof(WAV_HDR) + sizeof(CHUNK_HDR);
    memcpy(&hdr.wID, "WAVE", 4);
    memcpy(&hdr.fId, "fmt ", 4);
    hdr.pcm_header_len = 16;
    hdr.wFormatTag = 1;
    hdr.nChannels = 1;
    hdr.nSamplesPerSec = sampleRate;
    hdr.nBitsPerSample = 16;
    hdr.nAvgBytesPerSec = hdr.nChannels * sampleRate * (hdr.nBitsPerSample >> 3);
    hdr.nBlockAlign = hdr.nChannels * (hdr.nBitsPerSample >> 3);
    fwrite(&hdr, 1, sizeof(hdr), m_pFile);

    CHUNK_HDR chdr;
    memcpy(&chdr.dId, "data", 4);
    chdr.dLen = 0;
    fwrite(&chdr, 1, sizeof(chdr), m_pFile);
}

void C_Wav_File::WriteHeaderLen()
{
    if (NULL == m_pFile) {
        return;
    }

    size_t dwTotal = m_iDataPos + sizeof(WAV_HDR);
    fseek(m_pFile, 4, SEEK_SET);
    fwrite(&dwTotal, 1, 4, m_pFile);

    fseek(m_pFile, 40, SEEK_SET);
    fwrite(&m_iDataPos, 1, 4, m_pFile);
}

int C_Wav_File::GetAvailableLen()
{
    return (int)(m_iDataLen - m_iDataPos);
}

int C_Wav_File::Read(void *buffer, int len)
{
    if ((NULL == buffer) || (0 == len)) {
        Fn_Log("buffer=[%p], len=[%d]", buffer, len);
        return 0;
    }
    if (m_iDataPos >= m_iDataLen) {
        return -1;
    }
    
    int dataLen = len;
    int i_diff = (int)(m_iDataLen - m_iDataPos);
    if (dataLen > i_diff) {
        dataLen = i_diff;
    }

    dataLen = (int)fread(buffer, 1, dataLen, m_pFile);
    if (dataLen > 0) {
        m_iDataPos += dataLen;
    }
    Fn_Log("read dataLen=[%d]", dataLen);

    return dataLen;
}

/* EOF */
