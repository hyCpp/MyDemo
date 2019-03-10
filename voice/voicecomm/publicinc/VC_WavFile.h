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

#ifndef _VC_WAV_FILE_H_
#define _VC_WAV_FILE_H_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <stdio.h>
#include <string>
#include "VC_CommonDef.h"

/**
 * Summary for class VC_WavFile
 *
 * There should be a detail description for
 * class VC_WavFile
 */
class VC_API VC_WavFile
{
public:
    VC_WavFile();
    virtual ~VC_WavFile();

    // for read/write wav file
    bool Open(const std::string &filename, int sampleRate, bool bwrite = false);

    /**
     * Get wave available raw data length
     */
    int  GetAvailableLen();
    int  Read(void *buffer, int len);

    int Write(void *pData, int iLen);

    void Close();

private:
    // Disable copy/assign constructor
    VC_WavFile(const VC_WavFile& o);
    VC_WavFile& operator= (const VC_WavFile& o);

    void WriteHeader(int sampleRate);
    void WriteHeaderLen();
    bool OpenW(const std::string &filename, int sampleRate);
    bool OpenR(const std::string &filename, int sampleRate);

private:
    FILE *m_pFile;
    int  m_iDataPos;
    int  m_iDataLen;
};

#endif // _VC_WAV_FILE_H_
/* EOF */
