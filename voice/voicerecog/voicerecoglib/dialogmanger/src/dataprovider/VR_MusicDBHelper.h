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
 * @file VR_MusicDBHelper.h
 * @brief Declaration file of class VR_MusicDBHelper.
 *
 * This file includes the declaration of class VR_MusicDBHelper, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */
#ifndef VR_MUSIC_DB_HELPER_H
#define VR_MUSIC_DB_HELPER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

namespace navi
{
namespace dataprovider
{

enum
{
    OPEN_READWRITE          = 0x00000000,
    OPEN_READONLY           = 0x00000001,
    OPEN_READ_MASK          = 0x00000001,
    NO_LOCALIZED_COLLATORS  = 0x00000010,
    CREATE_IF_NECESSARY     = 0x10000000
};

typedef enum tagMusicDeviceType
{
    MUSIC_DEVICE_TYPE_UNKOWN = 0,
    MUSIC_DEVICE_TYPE_USB,
    MUSIC_DEVICE_TYPE_IPOD,
    MUSIC_DEVICE_TYPE_ENGINE
    
}MusicDeviceType;

typedef enum tagMusicDriverType
{
    MUSIC_DRIVER_TYPE_UNKOWN = 0,
    MUSIC_DRIVER_TYPE_ENGINE,
    MUSIC_DRIVER_TYPE_IPOD_IAP1,
    MUSIC_DRIVER_TYPE_IPOD_IAP2,
    MUSIC_DRIVER_TYPE_USB_GENERAL,
    
}MusicDriverType;

typedef struct tagMusicDeviceItem
{
    VR_String         DevName;
    MusicDeviceType   DevType;
    MusicDriverType   DevDriverType;

    tagMusicDeviceItem()
    {
        DevType = MUSIC_DEVICE_TYPE_UNKOWN;
        DevDriverType = MUSIC_DRIVER_TYPE_UNKOWN;
    }

}MusicDeviceItem;

typedef enum tagMusicDBDataSyncStatus
{
    MUSIC_DB_DATA_SYNC_STATUS_UNKOWN = 0,
    MUSIC_DB_DATA_SYNC_STATUS_SUCCESS,
    MUSIC_DB_DATA_SYNC_STATUS_FAIL

}MusicDBDataSyncStatus;

// music database info
typedef struct tagMusicDBItem
{
    VR_String             Path;         // Absoluate path
    MusicDBDataSyncStatus SyncStatus;

    tagMusicDBItem()
    {
        SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_UNKOWN;
    }
    
}MusicDBItem;

// db helper info
typedef struct tagMusicDBHelperItem
{
    MusicDBItem       DBItem;
    MusicDeviceItem   DeviceItem;
    
}MusicDBHelperItem;

/**
 * @brief The VR_MusicDBHelper class
 *
 * class declaration
 */
class VR_MusicDBHelper
{
public:
    MusicDBHelperItem m_Info;

public:
    VR_MusicDBHelper(const MusicDBHelperItem& item);
    virtual ~VR_MusicDBHelper();

public:
    virtual bool SyncMusicDB(VR_MusicDBHelper* target);
    virtual nutshell::NCList<VR_String>* CreateDBTableSQL();
    virtual nutshell::NCList<VR_String>* MergeDBTableSQL();

    static VR_MusicDBHelper* CreateDBHelper(const MusicDBHelperItem& item);

private:
    bool CreateFullTable();

private:
    VR_MusicDBHelper(const VR_MusicDBHelper&);
    VR_MusicDBHelper& operator =(const VR_MusicDBHelper&);
};

}
}
#endif /* VR_MUSIC_DB_HELPER_H */
/* EOF */
