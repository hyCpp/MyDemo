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
#include "TTS_Platform.h"
#include "TTS_File.h"
#include "CL_SyncObj.h"
#include "BL_FullFile.h"
#include <stdio.h>
#include <pthread.h>
#include <limits.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "TTS_PlatformFileManager.h"
#include <algorithm>

#include "MEM_vector.h"
#include "CL_NewTypesDefine.h"

using std::string;

/* handle to the data cached in a data entry (stream or mapping handle) */
typedef void * VPLATFORM_DATA_HDATA;

#define TTS_NUANCE_DATA_DIR     "/speech/components/"

/* ******************************************************************
**  DEFINITIONS
** ***************************************************************** */
/* linked list of all *.dat files in the install directories */
typedef struct VPLATFORM_FILELIST_S
{
    NUAN_U8                  * szFullPathName; /* full path name to the file */
    char                        * szFileName;     /* short file name (converted broker string) */
    VPLATFORM_DATA_HDATA          hData;          /* data handle */
    VPLATFORM_FILELIST_S        * pNext;        /* next node */
} VPLATFORM_FILELIST;

/* Class structure for the data interface */
typedef struct VPLATFORM_DATA_CLASS_S {
    void*       hHeap;  // heap
    // char                 * szBrokerInfo;
    VPLATFORM_FILELIST       * pFileList;   // filelist
} VPLATFORM_DATA_CLASS_T;

extern "C" {
static void* vplatform_heap_Malloc(void* hHeap, size_t cBytes)
{
    return malloc(cBytes);
}

static void* vplatform_heap_Calloc(void* hHeap, size_t cElements, size_t cElementBytes)
{
    return calloc(cElements, cElementBytes); 
}

static void* vplatform_heap_Realloc(void* hHeap, void* pData, size_t cBytes)
{
    return realloc(pData, cBytes);
}

static void vplatform_heap_Free(void* hHeap, void* pData)
{
    free(pData);
}

static const VE_HEAP_INTERFACE s_IHeap = {
    vplatform_heap_Malloc,
    vplatform_heap_Calloc,
    vplatform_heap_Realloc,
    vplatform_heap_Free
};

/* ------------------------------------------------------------------
**  Stream interface implementation
** ----------------------------------------------------------------- */

static CL_BOOL TranslateFileName(CONST CHAR* szSrcFileName, XCHAR* szDstFileName, DWORD dwSize)
{
    unsigned int ulLen = strlen(szSrcFileName);
    if (ulLen + 4 >= dwSize) {
        return CL_FALSE;
    }

    for (ULONG i = 0; i < ulLen; ++i) {
        if (((szSrcFileName[i] >= XTEXT('a')) && (szSrcFileName[i] <= XTEXT('z')))
            || ((szSrcFileName[i] >= XTEXT('0')) && (szSrcFileName[i] <= XTEXT('9')))) {
                szDstFileName[i] = szSrcFileName[i];
        }
        else if (XTEXT('/') == szSrcFileName[i]) {
            szDstFileName[i] = XTEXT('_');
        }
        else {
            szDstFileName[i] = XTEXT('-');
        }
    }
    xcscpy(szDstFileName + ulLen, XTEXT(".dat"));
    return CL_TRUE;
}

NUAN_ERROR vplatform_stream_Open(void* hDataClass,
                                 void* hHeap,
                                 const char* szName,
                                 const char* szMode,
                                 void** phStream)
{

    TTS_LOGI("platform Open=%s", szName);

    if ((!szName) || (!szMode) || (!hDataClass)) {
        TTS_LOGE("Name Mode Class NULL");
        return NUAN_E_INVALIDARG;
    }
    
    char szFileName[CL_MAX_PATH] = { 0 };
    if (!TranslateFileName(szName, szFileName, CL_MAX_PATH)) {
        return NUAN_E_BUFFERTOOSMALL;
    }
    // TTS_LOGD("file2=%s", szFileName);
    VoiceVector<TTS_Platform::TTS_EngineDataFile>::type *dataFiles = (VoiceVector<TTS_Platform::TTS_EngineDataFile>::type *)hDataClass;
    if (!dataFiles) {
        return NUAN_E_COULDNOTOPENFILE;
    }


    auto dataFile = std::find(dataFiles->begin(), dataFiles->end(), szFileName);

    if (dataFile == dataFiles->end()) {
        TTS_LOGE("vplatform_stream_Open can not find file");
        return NUAN_E_COULDNOTOPENFILE;
    }

    // TTS_LOGD("open file=%s", szFileName);
    TTS_File *pFile = new(MEM_Voice) TTS_File();
    if (nullptr == pFile) {
        return NUAN_E_COULDNOTOPENFILE;
    }

    string fileName = dataFile->m_dirPath  + "/" + szFileName;
    BL_String fileFullPath = fileName.c_str();
    bool ret = false;
    if ((szMode[0] != 'r') || (strchr(szName, '.') != NULL)) {
        ret = pFile->Open(fileFullPath, szMode);
    }
    else {
        ret = pFile->Open(fileFullPath, "rb");
    }

    if (ret) {
        *phStream = pFile;
        return NUAN_OK;
    }
    else {
        *phStream = NULL;
        TTS_LOGE("open %s fail", fileName.c_str());
        return NUAN_E_COULDNOTOPENFILE;
    }
}

NUAN_ERROR vplatform_stream_Close(void* hStream)
{
    if (NULL == hStream) {
        return NUAN_E_INVALIDARG;
    }

    TTS_File *pFile = reinterpret_cast<TTS_File*>(hStream);
    if (CL_FALSE == pFile->Close()) {
        return NUAN_E_FILECLOSE;
    }
    delete pFile;
    return NUAN_OK;
}

size_t vplatform_stream_Read(void* pBuffer,
                               size_t cElementBytes,
                               size_t cElements,
                               void* hStream)
{
    if ((NULL == pBuffer) || (NULL == hStream)) {
        return NUAN_E_INVALIDARG;
    }

    TTS_LOGD("read file size: %d", cElementBytes * cElements);
    TTS_File *pFile = reinterpret_cast<TTS_File*>(hStream);
    int count = pFile->Read(pBuffer, cElementBytes, cElements);
    return  (0 <= count) ? cElements : 0xFFFFFFFF;
}

NUAN_ERROR vplatform_stream_Seek(void* hStream,
                                 size_t u32Offset,
                                 VE_STREAM_ORIGIN eOrigin,
                                 VE_STREAM_DIRECTION eDirection)
{
    TTS_LOGD("seek file offset: %d", u32Offset);
    if (NULL == hStream) {
        return NUAN_E_INVALIDARG;
    }
    long int nSeekOpt;
    LONG lPos = static_cast<LONG>(u32Offset);
    TTS_File *pFile = reinterpret_cast<TTS_File*>(hStream);
    const LONG lSize = static_cast<LONG>(pFile->GetFileSize());
    switch (eOrigin) {
    case VE_STREAM_SEEK_SET:
    {
        nSeekOpt = SEEK_SET;
        if (VE_STREAM_BACKWARD == eDirection) {
            lPos = lSize - lPos;
        }
        break;
    }
    case VE_STREAM_SEEK_CUR:
    {
        nSeekOpt = SEEK_CUR;
        if (VE_STREAM_BACKWARD == eDirection) {
            lPos = -lPos;
        }
        break;
    }
    case VE_STREAM_SEEK_END:
    {
        nSeekOpt = SEEK_END;
        if (VE_STREAM_BACKWARD == eDirection) {
            lPos -= lSize;
        }
        break;
    }
    default:
        return NUAN_E_INVALIDARG;
    }

    bool ok = pFile->Seek(lPos, (BL_FileSeekOrigin)nSeekOpt);
    return ok ? NUAN_OK : NUAN_E_FILESEEK;
}

size_t vplatform_stream_GetSize(void* hStream)
{
    if (NULL == hStream) {
        return NUAN_E_INVALIDARG;
    }

    TTS_File *pFile = reinterpret_cast<TTS_File*>(hStream);
    return pFile->GetFileSize();
}

NUAN_ERROR vplatform_stream_Error(void*  hStream)
{
    return NUAN_OK;
}

size_t vplatform_stream_Write(const void* pBuffer,
                                size_t cElementBytes,
                                size_t cElements,
                                void* hStream)
{
    if (NULL == hStream) {
        return NUAN_E_INVALIDARG;
    }
    TTS_LOGD("platform_stream_Write...");
    TTS_File *pFile = reinterpret_cast<TTS_File*>(hStream);
    BOOL bOK = pFile->Write(const_cast<void *>(pBuffer), cElementBytes, cElements);
    return (TRUE == bOK) ? cElements : 0;
}

static const VE_DATA_STREAM_INTERFACE s_IStream = {
    vplatform_stream_Open,
    vplatform_stream_Close,
    vplatform_stream_Read,
    vplatform_stream_Seek,
    vplatform_stream_GetSize,
    vplatform_stream_Error,
    vplatform_stream_Write
};

/* struct CRITSEC_S */
typedef struct CRITSEC_S
{
    pthread_mutex_t     CriticalSection;    // CriticalSection
    pthread_t           ThreadId;           // ThreadId
    size_t            LevelCount;         // LevelCount
    void*               hHeap;              // hHeap
} CRITSEC_T;

// ////////////////////////////////////////////////////////////////////////////////////
static NUAN_ERROR vplatform_lock_Open(void* hCCritSec,
                                      void* hHeap,
                                      void** phCritSec)
{
    CRITSEC_T * pCritSec = NULL;
    // Dummy reference unused args
    (void)hCCritSec;

    // Validate args
    if (!phCritSec) {
        return NUAN_E_INVALIDARG;
    }

    // Init output arg
    *phCritSec = NULL;

    /* Allocate the structure */
    pCritSec = (CRITSEC_T*)vplatform_heap_Calloc(hHeap, 1, sizeof(CRITSEC_T));
    if (!pCritSec) {
        return NUAN_E_OUTOFMEMORY;
    }
      pCritSec->hHeap = hHeap;

    pthread_mutex_init(&pCritSec->CriticalSection, NULL);
    pCritSec->LevelCount = 0;
    pCritSec->ThreadId = 0;

    // Set output arg
    *phCritSec = pCritSec;
    return NUAN_OK;
}

static NUAN_ERROR vplatform_lock_Close(void* hCritSec)
{
    CRITSEC_T * pCritSec = (CRITSEC_T *)hCritSec;
    pthread_mutex_destroy(&pCritSec->CriticalSection);
    vplatform_heap_Free(pCritSec->hHeap, pCritSec);

    return NUAN_OK;
}

static NUAN_ERROR vplatform_lock_Start(void* hCritSec)
{
    CRITSEC_T * pCritSec = (CRITSEC_T *)hCritSec;
      // Validate args
    if (pCritSec == NULL) {
        return NUAN_OK;
    }

    if (pCritSec->ThreadId != pthread_self()) {
        pthread_mutex_lock(&pCritSec->CriticalSection);
        pCritSec->ThreadId = pthread_self();
        pCritSec->LevelCount++;
    }
    else {
        pCritSec->LevelCount++;
    }

    return NUAN_OK;
}

static NUAN_ERROR vplatform_lock_End(void* hCritSec)
{
    CRITSEC_T * pCritSec = (CRITSEC_T *)hCritSec;
    if (pCritSec == NULL) {
        return NUAN_OK;
    }

    if (pCritSec->ThreadId == pthread_self()) { // Safety check against bad programming practice
        if (pCritSec->LevelCount != 0) {
            pCritSec->LevelCount--;
            if (pCritSec->LevelCount == 0) {
                pCritSec->ThreadId = 0;
                pthread_mutex_unlock(&pCritSec->CriticalSection);
              }
        }
      }

    return NUAN_OK;
}

static const VE_CRITSEC_INTERFACE s_ILock = {
    vplatform_lock_Open,
    vplatform_lock_Close,
    vplatform_lock_Start,
    vplatform_lock_End
};

NUAN_ERROR vplatform_mapping_Open(void*      hDataClass,
                                  void*      hHeap,
                                  const char          * szName,
                                  void** phMapping)
{
    // TTS_LOGD("vplatform_mappint file1=%s", szName);
    char szFileName[CL_MAX_PATH] = { 0 };
    if (!TranslateFileName(szName, szFileName, CL_MAX_PATH)) {
        return NUAN_E_BUFFERTOOSMALL;
    }
    // TTS_LOGD("file2=%s", szFileName);
    VoiceVector<TTS_Platform::TTS_EngineDataFile>::type *dataFiles = (VoiceVector<TTS_Platform::TTS_EngineDataFile>::type *)hDataClass;
    if (!dataFiles) {
        return NUAN_E_COULDNOTOPENFILE;
    }

    auto dataFile = std::find(dataFiles->begin(), dataFiles->end(), szFileName);

    if (dataFile == dataFiles->end()) {
        return NUAN_E_COULDNOTOPENFILE;
    }

    string fileName = dataFile->m_dirPath  + szFileName;

    TTS_PlatformFileManager* pInst = TTS_PlatformFileManager::Instance();
    if (NULL == pInst) {
        // TTS_LOGD("pInst is null");
        return NUAN_E_OUTOFMEMORY;
    }

    TTS_PlatformAccessor* pFile = pInst->Acquire(fileName.c_str(), "r");
    if (NULL != pFile) {
        // TTS_LOGD("pFile not null");
        *phMapping = pFile;
        return NUAN_OK;
    }
    // TTS_LOGD("pFile is null");
    *phMapping = NULL;
    return NUAN_E_MAPPING;
}

NUAN_ERROR vplatform_mapping_Close(void*  hMapping)
{
    TTS_PlatformAccessor* pFile = reinterpret_cast<TTS_PlatformAccessor*>(hMapping);
    // TTS_LOGI("Data mapping close, handle=%p", pFile);

    TTS_PlatformFileManager* pInst = TTS_PlatformFileManager::Instance();
    if (NULL == pInst) {
        return NUAN_E_OUTOFMEMORY;
    }
    pInst->Release(pFile);
    return NUAN_OK;
}

NUAN_ERROR vplatform_mapping_Map(void*  hMapping,
                                 size_t               u32Offset,
                                 size_t             * pcBytes,
                                 const void          ** ppData)
{
    TTS_PlatformAccessor* pFile = reinterpret_cast<TTS_PlatformAccessor*>(hMapping);
    // TTS_LOGI("Data mapping map, handle=%p, offset=%u, len=%u", pFile, u32Offset, *pcBytes);
    *ppData = pFile->Map(u32Offset, *pcBytes);
    if (NULL == *ppData) {
        return NUAN_E_MAPPING;
    }
    return NUAN_OK;
}

NUAN_ERROR vplatform_mapping_Unmap(void*  hMapping,
                                   const void           * pData)
{
    TTS_PlatformAccessor* pFile = reinterpret_cast<TTS_PlatformAccessor*>(hMapping);
    // TTS_LOGI("Data mapping unmap, handle=%p, addr=%p", pFile, pData);
    if (!pFile->Unmap(pData)) {
        return NUAN_E_MAPPING;
    }
    return NUAN_OK;
}

NUAN_ERROR vplatform_mapping_Freeze(void*  hMapping)
{
    // TTS_PlatformAccessor* pFile = reinterpret_cast<TTS_PlatformAccessor*>(hMapping);
    // TTS_LOGI("Data mapping freeze, handle=%p", pFile);
    return NUAN_OK;
}

static const VE_DATA_MAPPING_INTERFACE s_IMapping = {
    vplatform_mapping_Open,
    vplatform_mapping_Close,
    vplatform_mapping_Map,
    vplatform_mapping_Unmap,
    vplatform_mapping_Freeze
};
};

TTS_Platform::TTS_Platform()
{

}

TTS_Platform::~TTS_Platform()
{

}

NUAN_ERROR TTS_Platform::GetInterface(VE_INSTALL *pInstall, VoiceVector<TTS_EngineDataFile>::type *dataFile)
{
    pInstall->pIHeap = &s_IHeap;
    pInstall->hHeap = NULL;
    pInstall->pICritSec = &s_ILock;
    pInstall->hCSClass = NULL;
    pInstall->hDataClass = (void*)dataFile;
    pInstall->pIDataStream = &s_IStream;
    pInstall->pIDataMapping =  &s_IMapping;
    pInstall->pILog = NULL;

    return NUAN_OK;
}

NUAN_ERROR TTS_Platform::ReleaseInterface(VE_INSTALL * pInstall)
{
    return NUAN_OK;
}

/* EOF */
