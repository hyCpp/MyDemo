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
#include "TTS_PlatformAccessor.h"
#include "TTS_File.h"

TTS_PlatformAccessor::WorkBuffer::WorkBuffer()
    : m_bInit(false), m_fileOffset(0), m_mapped(0)
{
}

TTS_PlatformAccessor::WorkBuffer::~WorkBuffer()
{
}

BYTE* TTS_PlatformAccessor::WorkBuffer::GetInBuffer(const DWORD& offset, const DWORD& len)
{
    if (!m_bInit) {
        return NULL;
    }

    if (m_fileOffset <= offset && (m_fileOffset + WORK_BUFF_SIZE) >= (offset + len)) {
        m_mapped++;
        return m_buffer + (offset - m_fileOffset);
    }

    return NULL;
}

BYTE* TTS_PlatformAccessor::WorkBuffer::GetBuffer()
{
    if (m_mapped != 0) {
        return NULL;
    }

    m_bInit = false;

    return m_buffer;
}

void TTS_PlatformAccessor::WorkBuffer::UnMap()
{
    m_mapped--;
}

void TTS_PlatformAccessor::WorkBuffer::ResetBuffer(const DWORD& offset)
{
    m_bInit = true;
    m_fileOffset = offset;
    m_mapped = 1;
}

TTS_PlatformAccessor::TTS_PlatformAccessor(const XCHAR* pszName, TTS_File* pFile, DWORD dwMaxItemSize)
: m_nRefCount(1), m_pFile(pFile)
{
    // CACHE_INIT();
    // TTS_LOGD(XTEXT("[%s] Open <<<"), pszName);

    m_pszName = new(MEM_Voice) XCHAR[xcslen(pszName) + 1];
    if (NULL != m_pszName) {
        xcscpy(m_pszName, pszName);
    }

    m_dwFileSize = pFile->GetFileSize();
}

TTS_PlatformAccessor::~TTS_PlatformAccessor()
{
    if (NULL != m_pFile) {
        m_pFile->Close();
        delete m_pFile;
    }

    if (NULL != m_pszName) {
        delete[] m_pszName;
    }
}

CL_BOOL TTS_PlatformAccessor::Exist(const XCHAR* pszName)
{
    if (NULL == m_pszName || NULL == pszName) {
        return CL_FALSE;
    }
    return  (0 == xcscmp(pszName, m_pszName)) ? CL_TRUE : CL_FALSE;
}

const VOID* TTS_PlatformAccessor::Map(DWORD dwOffset, DWORD pdwBytes)
{
    // TTS_LOGD("[%s] Map , offset=%d, len=%d", m_pszName, dwOffset, pdwBytes);
    if (0 == pdwBytes || (dwOffset + pdwBytes) > m_dwFileSize) {
        pdwBytes = m_dwFileSize - dwOffset;
    }

    void* pAddr = m_workBuff.GetInBuffer(dwOffset, pdwBytes);
    if (pAddr != NULL) {
        // TTS_LOGD("[%s] Map in WorkBuffer, offset=%d, len=%d", m_pszName, dwOffset, pdwBytes);
        return pAddr;
    }

    // First seek file
    if (FALSE == m_pFile->Seek(dwOffset, BL_FILE_SEEK_SET)) {
        // TTS_LOGD("m_pFile->Seek error");
        return NULL;
    }

    // Fixed work buffer
    // TTS_LOGD("this=%p, m_bUsed=%d", this, m_bUsed);
    if (pdwBytes <= WORK_BUFF_SIZE && m_workBuff.GetMapped() == 0) { // Avoid to allocate and release frequently
        pAddr = m_workBuff.GetBuffer();
        if (FALSE == m_pFile->Read(pAddr, 1, WORK_BUFF_SIZE)) {
            // TTS_LOGD("m_pFile->read error");
            return NULL;
        }

        m_workBuff.ResetBuffer(dwOffset);
        // CACHE_MISS();
        return pAddr;
    }

    char *pNewAddr = new(MEM_Voice) char[pdwBytes];

    if (!m_pFile->Read(pNewAddr, 1, pdwBytes)) {
        delete[] pNewAddr;
        return NULL;
    }

    return pNewAddr;
}

CL_BOOL TTS_PlatformAccessor::Unmap(const VOID* pAddr)
{
    if (m_workBuff.CheckInBuffer(pAddr)) {
        // TTS_LOGD("UnMap in WorkBuffer pAddr is %0x, ", pAddr);
        m_workBuff.UnMap();
    }
    else {
        // TTS_LOGD("UnMap free pAddr is %0x, ", pAddr);
        char* p = static_cast<char*>(const_cast<void*>(pAddr));
        delete[] p;
    }
    return CL_TRUE;
}

#if 0
CL_BOOL TTS_PlatformAccessor::Freeze()
{
    return CL_FALSE;
}

#endif

/* EOF */
