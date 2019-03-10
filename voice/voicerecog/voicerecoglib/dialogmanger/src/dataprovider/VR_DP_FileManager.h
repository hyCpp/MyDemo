/**
 * Copyright @ 2013 - 2014 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_DP_FileManager.h
 * @brief Declaration file of class VR_DP_FileManager.
 *
 * This file includes the declaration of class VR_DP_FileManager, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */
#ifndef VR_DP_FILE_MANAGER_H
#define VR_DP_FILE_MANAGER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "ncore/NCTypesDefine.h"
#include "ncore/NCString.h"
#include "BL_SyncObject.h"
#include "BL_AutoSync.h"

namespace navi
{
namespace dataprovider
{
#define VR_DPCACHE_FILENAME_SUFFIX         ".temp"
#define VR_DPCACHE_PATH_MAXLEN             192
#define VR_DPCACHE_FILENAME_MAXLEN         64
#define VR_DPCACHE_FILEPATH_MAXLEN         256

// DP File Node
typedef struct tagDPFileNode
{
    nutshell::INT    index;
    nutshell::INT    accessCount;
    nutshell::NCCHAR path[VR_DPCACHE_FILEPATH_MAXLEN + 4];

}DPFileNode;

// DP File Head Node
typedef struct tagDPFileHeadNode
{
    nutshell::INT    key;
    nutshell::NCCHAR name[VR_DPCACHE_FILENAME_MAXLEN + 4];
    nutshell::NCCHAR cachePath[VR_DPCACHE_PATH_MAXLEN + 4];
    nutshell::INT    nodeMaxLen;
    nutshell::INT    nodeLen;
    nutshell::INT    virtualNodeLen;
    tagDPFileNode*   nodeList;

}DPFileHead;

/**
 * @brief The VR_DP_FileManager class
 *
 * class declaration.
 */
class VR_DP_FileManager
{
public:
    static VR_DP_FileManager* GetInstance();
    static VOID Dispose();

    bool SetFileCache(const nutshell::INT&    fileKey,
                      const nutshell::NCCHAR* fileName,
                      const nutshell::NCCHAR* cachePath,
                      const nutshell::INT&    cacheSize);

public:
    bool PushBack(const nutshell::INT& fileKey, const nutshell::NCCHAR* filePath);
    bool Front(const nutshell::INT& fileKey, nutshell::NCCHAR* filePath,
               nutshell::NCCHAR* fileName);
    bool PopFront(const nutshell::INT& fileKey);
    bool Clear(const nutshell::INT& fileKey);

private:
    VR_DP_FileManager();
    ~VR_DP_FileManager();
    VR_DP_FileManager(const VR_DP_FileManager&);
    VR_DP_FileManager& operator=(const VR_DP_FileManager&);

    typedef VoiceMap<nutshell::INT, nutshell::INT64>::type DPFileMap;
    DPFileMap                 m_fileMap;
    BL_SyncObject             m_lock;
    static VR_DP_FileManager* m_fileManager;
};
}
}
#endif /* VR_DP_FILE_MANAGER_H */
/* EOF */
