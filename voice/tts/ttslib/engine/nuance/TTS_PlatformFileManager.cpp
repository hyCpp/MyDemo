/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
#include "TTS_PlatformFileManager.h"
#include "TTS_File.h"
#include "ve_ttsapi.h"
#include <string>

using namespace std;


TTS_PlatformFileManager* TTS_PlatformFileManager::s_pInstance = NULL;

TTS_PlatformFileManager* TTS_PlatformFileManager::Instance()
{
    if (NULL != s_pInstance) {
        return s_pInstance;
    }
    s_pInstance = new(MEM_Voice) TTS_PlatformFileManager();
    return s_pInstance;
}

VOID TTS_PlatformFileManager::Destroy()
{
    if (NULL != s_pInstance) {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

TTS_PlatformFileManager::TTS_PlatformFileManager()
    :m_dwPathLen(0)
{
    m_szPath[0] = 0;
    m_szLang[0] = 0;
}

TTS_PlatformFileManager::~TTS_PlatformFileManager()
{
    Clear();
}

CL_BOOL TTS_PlatformFileManager::SetPath(const XCHAR* pszPath)
{
    if (NULL != pszPath) {
        m_dwPathLen = xcslen(pszPath);
        xcscpy(m_szPath, pszPath);
    }
    else {
        m_dwPathLen = 0;
        m_szPath[0] = 0;
    }
    return CL_TRUE;
}

CL_BOOL TTS_PlatformFileManager::SetLang(const XCHAR* pszLang)
{
    Clear();
    if (NULL != pszLang) {
        xcscpy(m_szLang, pszLang);
    }
    else {
        m_szLang[0] = 0;
    }
    return CL_TRUE;
}
 
TTS_PlatformAccessor* TTS_PlatformFileManager::Acquire(const XCHAR* pszName, const char* pszMode)
{
    if (NULL == pszName) {
        return NULL;
    }

    TTS_PlatformAccessor* pAccessor = GetAccessor(pszName);
    if (NULL != pAccessor) {
        pAccessor->IncRef();
        return pAccessor;
    }

    TTS_File* pFile = OpeTTS_File(pszName, pszMode);
    if (NULL == pFile) {
        return NULL;
    }

    pAccessor = NewAccessor(pszName, pFile);
    if (NULL != pAccessor) {
        m_fileList.Add(pAccessor);
    } 
    else {
        pFile->Close();
    }
    RETURN(pAccessor);
}

TTS_File* TTS_PlatformFileManager::OpeTTS_File(const XCHAR* pszName, const char* pszMode) 
{
    // Composite file full path name
    DWORD dwLangLen = 0;
    xcscpy(m_szPath, pszName);
    TTS_File* pFile = new(MEM_Voice) TTS_File();
    if (NULL == pFile) {
        return NULL;
    }

    if (FALSE == pFile->Open(m_szPath, const_cast<char*>(pszMode))) {
        delete pFile;
        return NULL;
    }

    return pFile;
}


TTS_PlatformAccessor* TTS_PlatformFileManager::NewAccessor(const XCHAR* pszName, TTS_File* pFile)
{
    // TTS_LOGD("NewAccessor.....");
    TTS_PlatformAccessor* pAccessor = NULL;
    // string patten0 = s_cahcePaternNames[0];
    // string patten1 = s_cahcePaternNames[1];
    // string fileName = pszName;
    
    // if (isSubstr1(fileName, patten1)) {
        // TTS_LOGD("new AccessorCache");
       //  pAccessor = new(MEM_Voice) AccessorCache(pszName, pFile, s_cacheConfigs[1][0], s_cacheConfigs[1][1], s_cacheConfigs[1][2]);
    // }
    // else {
        // TTS_LOGD("new TTS_PlatformAccessor");
        pAccessor = new(MEM_Voice) TTS_PlatformAccessor(pszName, pFile, 0);
    // }

    return pAccessor;
}

VOID TTS_PlatformFileManager::Release(TTS_PlatformAccessor* pAccessor)
{
    if (NULL != pAccessor) {
        if (1 == pAccessor->DecRef()) {
            m_fileList.Remove(pAccessor);
            delete pAccessor;
        }
    }
}
 
VOID TTS_PlatformFileManager::Clear()
{
    m_fileList.DeleteData();
}

TTS_PlatformAccessor* TTS_PlatformFileManager::GetAccessor(const XCHAR* pszName)
{
    DECL_OBJ_LIST_ITER(TTS_PlatformAccessor) iter(m_fileList);
    TTS_PlatformAccessor *it = iter();
    for (; NULL != it; it = iter()) {
        if (it->Exist(pszName)) {
            return it;
        }
    }
    return NULL;
}

/* EOF */
