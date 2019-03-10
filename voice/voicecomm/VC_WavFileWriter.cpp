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
 * @file VC_WavFileWrite.cpp
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

#include "VC_WavFileWriter.h"
using namespace std;


VC_WavFileWriter::VC_WavFileWriter()
    :   m_pFile(NULL)
    ,   m_lDataLenSub(0)
    ,   m_lDataLen(0)
{
}

VC_WavFileWriter::~VC_WavFileWriter()
{
    if (NULL != m_pFile) {
        Close();
    }
        
}

bool VC_WavFileWriter::Open(const std::string &filename, VC_WAVEFORMATEX &format)
{
    if (NULL != m_pFile) {
        VC_LOGI("please close the previou file!");
        RETURN(false);
    }

    m_pFile = fopen(filename.c_str(), "wb");
    if (NULL == m_pFile) {
        VC_LOGE("open file[%s] failed!", filename.c_str());
        RETURN(false);
    } 
    
    m_lDataLenSub = 0;
    m_lDataLen = 0;
    
    if (!WriteWaveHeader(format)) {
        VC_LOGI("write head info failed!");
        Close();
        RETURN(false);
    }

    return true;
}

bool VC_WavFileWriter::Open(const std::string &filename)
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

    return Open(filename, format);
}

int VC_WavFileWriter::Write(void *pData, int iLen)
{
    if (NULL == m_pFile || NULL == pData || iLen < 1) {
        VC_LOGI("the len is lt 0 or buffer is not existe!");
        return -1;
    }

    int write_len = fwrite(pData, 1, iLen, m_pFile);
    m_lDataLen += write_len;
    return write_len;
}

bool VC_WavFileWriter::Close()
{
    if (NULL == m_pFile) {
        VC_LOGI("the file is closed!");
        RETURN(false);
    }

    if (!WriteWaveLen()) {
        VC_LOGI("Write len failed!");
        RETURN(false);
    }

    if (fclose(m_pFile)) {
        VC_LOGI("close file failed!");
        RETURN(false);
    }

    m_pFile = NULL;
    return true;
}

bool VC_WavFileWriter::WriteWaveLen()
{
    int const HEAD_LEN = 4; 
    int nRet = 0;
    fseek(m_pFile, m_lDataLenSub, SEEK_SET);
    nRet = fwrite(&m_lDataLen, sizeof(char), HEAD_LEN, m_pFile);
    if (nRet != HEAD_LEN) {
        RETURN(false);
    }
        

    char ch = '\0';
    if ((m_lDataLen%2) != 0) {
        nRet = fwrite(&ch, sizeof(char), 1, m_pFile);
        if (nRet != 1) {
            RETURN(false);
        }
            
        m_lDataLen++;
    }
    
    long int lWaveLen;
    lWaveLen = m_lDataLen + m_lDataLenSub - HEAD_LEN;

    fseek(m_pFile, HEAD_LEN, SEEK_SET);
    nRet = fwrite(&lWaveLen, sizeof(char), HEAD_LEN, m_pFile);
    if (nRet != HEAD_LEN) {
        RETURN(false);
    }
        
    return true;
}

bool VC_WavFileWriter::WriteWaveRiffInfo()
{
    const int RIFF_SIZE = 12;
    int nRet = 0;
    char cWrite[RIFF_SIZE] = { 0 };
    memcpy(cWrite, "RIFF0000WAVE", RIFF_SIZE);
    nRet = fwrite(cWrite, sizeof(char), RIFF_SIZE, m_pFile);
    if (nRet != RIFF_SIZE) {
        RETURN(false);
    }
        
    m_lDataLenSub += RIFF_SIZE;
    return true;
}

bool VC_WavFileWriter::WriteWaveFmtInfo(VC_WAVEFORMATEX format)
{
    int len = format.pcm_header_len;
    len += 8;
    if ((len != 26) && (len != 24)) {
        RETURN(false);
    }
        
    int nRet = 0;
    nRet = fwrite(&format, sizeof(char), len, m_pFile);
    if (nRet != len) {
        RETURN(false);
    }
        
    m_lDataLenSub += len;
    return true;
}

bool VC_WavFileWriter::WriteWaveDataInfo()
{
    const int READ_SIZE = 8;
    char cRead[READ_SIZE] = { 0 };
    memcpy(cRead, "data", 4);
    int nRet = 0;
    nRet = fwrite(cRead, sizeof(char), READ_SIZE, m_pFile);
    if (nRet != READ_SIZE) {
        RETURN(false);
    }
        
    m_lDataLenSub += 4;
    return true;
}

bool VC_WavFileWriter::WriteWaveHeader(VC_WAVEFORMATEX format)
{
    if (!WriteWaveRiffInfo()) {
        RETURN(false);
    }

    if (!WriteWaveFmtInfo(format)) {
        RETURN(false);
    }

    if (!WriteWaveDataInfo()) {
        RETURN(false);
    }

    return true;
}
/* EOF */
