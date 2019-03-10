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
 * @file VR_WavFile.h
 * @brief inteface
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef _Wave_File_h__
#define _Wave_File_h__

#include <stdio.h>
#include <string>
#include <cstdio>
/**
 * Summary for class VC_WavFile
 *
 * There should be a detail description for
 * class VC_WavFile
 */
class C_Wav_File
{
public:
    C_Wav_File();

    // for read/write wav file
    bool Open(const std::string &filenamei, int sampleRate, bool write = false);

    /**
     * Get wave available raw data length
     */
    int GetAvailableLen();
    int Read(void *buffer, int len);

    int Write(void *pData, int iLen);

    void Close();

private:
    // Disable copy/assign constructor
    C_Wav_File(const C_Wav_File& o);
    C_Wav_File& operator= (const C_Wav_File& o);

    void WriteHeader(int sampleRate);
    void WriteHeaderLen();
    bool OpenW(const std::string &filenamei, int sampleRate);
    bool OpenR(const std::string &filenamei, int sampleRate);

private:
    FILE *m_pFile;
    long long  m_iDataPos;
    long long  m_iDataLen;
};

#endif // _Wave_File_h__
/* EOF */