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
 * @file TTS_PlatformAccessor.h
 * @brief Declaration file of class TTS_PlatformAccessor.
 *
 * This file includes the declaration of TTS_PlatformAccessor.
 *
 * @attention used for C++ only.
 */

#ifndef CXX_TTS_PLATFORM_ACCESSOR_H
#define CXX_TTS_PLATFORM_ACCESSOR_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_BL_List_H
#   include "BL_List.h"
#endif

#ifndef CXX_TTS_COMM_H
#   include "TTS_Comm.h"
#endif

// Forward class declaration
class TTS_File;

/**
 * class TTS_PlatformAccessor
 *
 * Base File Accessor, it will forward file request to TTS_File.
 * It simulate a fake file memory mapping interface.
 *
 */
class TTS_PlatformAccessor
{
public:
    // Bits order: cache flag(1 bit) | position(left bits) | length(var bits)
    // flag: 1 cache, 0 non - cache
    static const DWORD KEY_LEN = 1;
    static const DWORD WORK_BUFF_SIZE = 4096;

    /**
     * class WorkBuffer
     *
     * memory buffer
     *
     */
    class WorkBuffer
    {
        public:
            WorkBuffer();
            ~WorkBuffer();

            BYTE* GetInBuffer(const DWORD& offset, const DWORD& len);
            BYTE* GetBuffer();

            void UnMap();
            void ResetBuffer(const DWORD& offset);

            DWORD GetMapped() const 
            {
                return m_mapped;
            }

            bool CheckInBuffer(const void* pAddr) const 
            {
                return pAddr >= m_buffer && pAddr < (m_buffer + WORK_BUFF_SIZE);
            }

        private:
            bool  m_bInit;
            DWORD m_fileOffset;
            DWORD m_mapped;
            BYTE  m_buffer[WORK_BUFF_SIZE];

            WorkBuffer(const WorkBuffer&);
            WorkBuffer& operator = (const WorkBuffer&);
    };
   
#if 0
    /**
     * class Item
     *
     * Cache item
     * Recent added item will be place to list header
     */
    class Item{
    public:
        explicit Item(DWORD dwLen);
        ~Item();
        DWORD* GetAddr() const { return pAddr + KEY_LEN; }

        Item* next;
    private:
        DWORD* pAddr;

    private:
        Item(const Item&);
        Item& operator=(const Item&);
    };

#endif

    TTS_PlatformAccessor(const XCHAR* pszName, TTS_File* pFile, DWORD dwMaxItemSize);
    virtual ~TTS_PlatformAccessor();

    /**
     * Map resource file data to memory
     *
     * @param dwOffset [IN] file position offset
     * @param pdwElemBytes [IN/OUT] data length in bytes
     * @return memory address if the function succeeds, else return NULL.
     */
    virtual const VOID* Map(DWORD dwOffset, DWORD pdwBytes);

    /**
     * Unmap memory of resource file data
     *
     * @param pAddr [IN] data memory address
     * @return CL_TRUE if the function succeeds, else return CL_FALSE.
     */
    virtual CL_BOOL Unmap(const VOID* pAddr);

    /**
     * Freeze memory to avoid be swapped out
     *
     * @return CL_FALSE always return.
     */
    // virtual CL_BOOL Freeze();

    CL_BOOL Exist(const XCHAR* pszName);

    inline DWORD GetSize()
    {
        return m_dwFileSize;
    }

protected:
    inline INT IncRef()
    {
        return m_nRefCount++;
    }

    inline INT DecRef()
    {
        return m_nRefCount--;
    }

    // const DWORD m_dwMaxItemSize;

    // File open refer count
    INT m_nRefCount;
    XCHAR* m_pszName;
    TTS_File* m_pFile;
    DWORD  m_dwFileSize;

    // Temp work buffer
    WorkBuffer m_workBuff;

    friend class TTS_PlatformFileManager;

    // Only for test
#ifdef _ENABLE_CACHE_TEST
    DWORD dwTestCacheHit_;
    DWORD dwTestCacheMiss_;
# define CACHE_INIT() dwTestCacheHit_ = 0; dwTestCacheMiss_ = 0
# define CACHE_HIT() (++dwTestCacheHit_)
# define CACHE_MISS() (++dwTestCacheMiss_)
#endif

private:
    TTS_PlatformAccessor(const TTS_PlatformAccessor&);
    TTS_PlatformAccessor& operator=(const TTS_PlatformAccessor&);
};

DefineObjList(TTS_PlatformAccessor);

#endif // CXX_TTS_PLATFORM_ACCESSOR_H
/* EOF */
