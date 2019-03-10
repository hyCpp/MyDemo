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
 * @file VR_MusicListener.h
 * @brief music listener for content provider notify handle
 *
 * @attention used for C++ only.
 */

#ifndef VR_MUSIC_LISTENER_H
#define VR_MUSIC_LISTENER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include "ncore/NCUri.h"
#include "ncore/NCVariant.h"
#include "ncore/NCTypesDefine.h"
#include "VR_MusicDBHelperManager.h"

namespace navi
{
namespace dataprovider
{

typedef enum tagMusicNotifyMsgHandle
{
    MUSIC_NOTIFY_MSG_DROP    = 0,
    MUSIC_NOTIFY_MSG_FORWARD = 1

}MusicNotifyMsgHandle;

/**
 * @brief The VR_MusicListener class
 *
 * class declaration.
 */
class VR_MusicListener
{
public:
    static void Destory();
    static VR_MusicListener* GetInstance();

    bool onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);

private:
    bool Initialize();
    nutshell::INT32 HookUSBDataUpdate(nutshell::NCUri& uri, nutshell::NCVariant& variant);
    nutshell::INT32 HookIpodDataUpdate(nutshell::NCUri& uri, nutshell::NCVariant& variant);
    nutshell::INT32 HookInitMusicData(nutshell::NCUri& uri, nutshell::NCVariant& variant);
    bool GetIpodDeviceInfo(int& deviceStatus, MusicDriverType& deviceDriver, VR_String& name);
    bool GetUSBDeviceInfo(bool& deviceStatus);
    bool BackupMediaDB(VR_String& origin, VR_String& target);

private:
    VR_MusicListener();
    virtual ~VR_MusicListener();
    VR_MusicListener(const VR_MusicListener&);
    VR_MusicListener& operator =(const VR_MusicListener&);

    VR_MusicDBHelperManager* m_dbManager;
    static VR_MusicListener* m_instance;
};

}
}
#endif /* VR_MUSIC_LISTENER_H */
/* EOF */
