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
 * @file TTS_PlatformFileManager.h 
 * @brief Declaration file of class TTS_PlatformFileManager.                  
 *
 * This file includes the declaration of class TTS_PlatformFileManager and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef CXX_TTS_PLATFORM_FILEMANAGER_H
#define CXX_TTS_PLATFORM_FILEMANAGER_H

#ifndef CXX_TTS_PLATFORM_ACCESSOR_H
#	include "TTS_PlatformAccessor.h"
#endif

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "CL_NewTypesDefine.h"

/**
 * TTS_PlatformFileManager
 *
 * Platform read - only resource file accessor manager
 * It use file open refer count avoid to reopen same file
 */
class TTS_PlatformFileManager
{
public:

    static TTS_PlatformFileManager* Instance();
    static VOID Destroy();
    static CONST XCHAR* TranslateName(CONST CHAR* szSrcFileName, XCHAR* szDstFileName);

    /**
     * Set engine data path
     *
     * @param pszPath [IN] engine data file path
     * @return CL_TRUE if the function succeeds, else return CL_FALSE.
     */
    CL_BOOL SetPath(const XCHAR* pszPath);

    /**
     * Set engine language dependent data path
     *
     * @param pszLang [IN] language short name(such as ENG, THT, PTB)
     * @return CL_TRUE if the function succeeds, else return CL_FALSE.
     */
    CL_BOOL SetLang(const XCHAR* pszLang);

    /**
     * Acquire engine data file accessor
     *
     * @param pszName [IN] file name
     * @param pszMode [IN] file open mode
     * @return accessor if the function succeeds, else return NULL.
     */
    TTS_PlatformAccessor* Acquire(const XCHAR* pszName, const char* pszMode);

    /**
     * Release engine data file accessor
     *
     * @param pAccessor [IN] file accessor pointer
     * @return none.
     */
    VOID Release(TTS_PlatformAccessor* pAccessor);

    /**
     * Open engine data file with TTS_File
     *
     * @param pszName [IN] file name
     * @param pszMode [IN] file open mode
     * @return TTS_File pointer if the function succeeds, else return NULL.
     * @attention this method will not be used
     */
    TTS_File* OpeTTS_File(const XCHAR* pszName, const char* pszMode);

private:
    TTS_PlatformFileManager();
    ~TTS_PlatformFileManager();
    TTS_PlatformAccessor* NewAccessor(const XCHAR* pszName, TTS_File* pFile);
    TTS_PlatformAccessor* GetAccessor(const XCHAR* pszName);
    VOID Clear();

    static TTS_PlatformFileManager* s_pInstance;

    DWORD m_dwPathLen;
    XCHAR m_szPath[CL_MAX_PATH];
    XCHAR m_szLang[4];
    DECL_OBJ_LIST(TTS_PlatformAccessor) m_fileList;
};


#endif // CXX_TTS_PLATFORM_FILEMANAGER_H
/* EOF */
