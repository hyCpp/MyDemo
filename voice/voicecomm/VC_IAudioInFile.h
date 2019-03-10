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
 * @file VC_IAudioInFile.h
 * @brief inteface
 *
 *
 * @attention used for C++ only.
 */
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#ifndef _VC_IAUDIOIN_FILE_H_
#define _VC_IAUDIOIN_FILE_H_

#include "VC_IAudioIn.h"
#include "VC_WavFile.h"

#include <vector>

#ifndef CXX_BL_THREAD_H
#    include "BL_Thread.h"
#endif

/**
 * Summary for class VC_IAudioInFile
 * 
 * This is a detail description for class VC_IAudioInFile
 *
 */
class VC_IAudioInFile: public VC_IAudioIn, protected BL_Thread
{
public:
    VC_IAudioInFile();
    virtual ~VC_IAudioInFile();
    virtual bool Open(int sampleRate, int mode);
    virtual bool GetParam(int type, void **buf, int* size);

    virtual int Read(VOID* buf, int size);
    virtual bool Stop();
    virtual bool Close();

private:
    bool reload();
    VC_IAudioInFile(const VC_IAudioInFile&);
    VC_IAudioInFile &operator=(const VC_IAudioInFile& other);
    void VC_IAudioInCheckFilePath();

    VC_WavFile wavFile;
    std::vector<std::string> sequences;
    size_t index;
    std::string   strRecListPath;
    std::string   strRecListDir;

    char * fileData;
    int fileSize;
    char* m_eBuffer;
    bool m_isStart;
    int m_sampleRate;
};

#endif // _VC_IAUDIOIN_FILE_H_
/* EOF */
