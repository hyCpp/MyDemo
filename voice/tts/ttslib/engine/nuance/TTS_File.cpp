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
#include "TTS_File.h"
#include <stdio.h>

TTS_File::TTS_File()
: m_file(NULL), m_path()
{

}

TTS_File::~TTS_File()
{

}

bool TTS_File::Open(const XCHAR* strFileName, const XCHAR* mode)
{
    m_path = strFileName;
    m_file = fopen(strFileName, mode);
    if (!m_file) {
        TTS_LOGI("Open %s fail", m_path.c_str());
        return false;
    }
    
    return true;
}

size_t TTS_File::GetFileSize()
{
    fseek(m_file, 0, BL_FILE_SEEK_END);
    long len = ftell(m_file);
    fseek(m_file, 0, BL_FILE_SEEK_SET);
    return len;
}

size_t TTS_File::Read(void *buffer, int size, int count)
{
    size_t len = fread(buffer, size, count, m_file);
    int errCode = ferror(m_file);
    if (0 != errCode) {
        TTS_LOGE("errCode=%d,size=%d", errCode, size);
        TTS_LOGE("count=%d,len=%d", count, len);
        TTS_LOGE("filePath=%s", m_path.c_str());
        clearerr(m_file);
    }
    return len;
}

bool TTS_File::Close()
{
    m_path = "";
    if (m_file) {
        fclose(m_file);
        m_file = NULL;
    }

    return true;
}

bool TTS_File::Seek(long lOffset, BL_FileSeekOrigin eOrigin)
{
    if (m_file) {
        int ret = fseek(m_file, lOffset, eOrigin);
        return (0 == ret);
    }

    return false;
}

size_t TTS_File::Write(const void *buf, int size, int count)
{
    if (m_file) {
        return fwrite(buf, size, count, m_file);
    }

    return 0;
}

/* EOF */
