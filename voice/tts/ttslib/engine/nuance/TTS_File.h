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
 * @file TTS_File.h 
 * @brief Declaration file of class TTS_File.                  
 *
 * This file includes the declaration of class TTS_File and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef _CXX_TTS_FILE_H_
#define _CXX_TTS_FILE_H_

#include "BL_String.h"
#include <stdio.h>
#include <string>
#include "BL_FilePublicDef.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* TTS_File
*
* The class is just for TTS_File.
*/
class TTS_File
{
public:
    TTS_File();
    ~TTS_File();

    bool Open(const XCHAR* strFileName, const XCHAR* mode);
    size_t GetFileSize();
    size_t Read(void *buffer, int size, int count);
    bool Close();
    bool Seek(long lOffset, BL_FileSeekOrigin eOrigin = BL_FILE_SEEK_SET);
    size_t Write(const void *buf, int size, int count);

private:
    TTS_File(const TTS_File&);
    TTS_File& operator = (const TTS_File&);
    FILE			*m_file;
    std::string		m_path;

};

#endif /* _CXX_TTS_FILE_H_ */
/* EOF */
