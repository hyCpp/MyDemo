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
 * @file VR_MusicDBHelperManager.h
 * @brief Declaration file of class VR_MusicDBHelperManager.
 *
 * This file includes the declaration of class VR_MusicDBHelperManager, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */
#ifndef VR_MUSIC_DB_HELPER_MANAGER_H
#define VR_MUSIC_DB_HELPER_MANAGER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include "VR_MusicDBHelper.h"

#define VR_MUSIC_DATA_PATH "voicerecog"
#define VR_MUSIC_ENGINE_DB_NAME "VrProxy_Music.db"

namespace navi
{
namespace dataprovider
{

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_MusicDBHelper);

/**
 * @brief The VR_MusicDBHelperManager class
 *
 * class declaration.
 */
class VR_MusicDBHelperManager
{
public:
    static void Destory();
    static VR_MusicDBHelperManager* GetInstance();

public:
    bool Initialize();

    virtual VR_String GetMediaDBPath();
    virtual bool      OnNotifyUpdate(tagMusicDBHelperItem& item);
    bool      GetFavoriableGenreList(std::list<VR_String>& list);
    VR_String GetEngineDBPath(int type);
    void      ClearEngineDBPath(int type);

private:
    VR_MusicDBHelperManager();
    virtual ~VR_MusicDBHelperManager();
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_MusicDBHelperManager);

    bool InitWorkDirectory();
    void SetEngineDBPath(VR_String& dbPath, int type);
    void sendMessageToNATP(VR_String& mesg);
    bool SyncMediaDatabase(MusicDBHelperItem& item);
    bool GenerateVoiceDBPath(int sourceId, VR_String& dbPath);

private:
    VR_String m_voiceDir;
    VR_String m_mediaDir;
    VR_String m_engineDBPath_usb;
    VR_String m_engineDBPath_ipod;
    static VR_MusicDBHelperManager* m_instance;
};

}
}
#endif /* VR_MUSIC_DB_HELPER_MANAGER_H */
/* EOF */
