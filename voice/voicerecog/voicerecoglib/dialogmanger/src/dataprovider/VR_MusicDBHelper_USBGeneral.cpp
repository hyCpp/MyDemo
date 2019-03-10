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
#include "VR_Log.h"
#include "VR_MusicDBHelper_USBGeneral.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{

const char* VR_MusicDBHelper_USBGeneral::g_MergeSQLList[] = {
    VR_MUSIC_USB_MERGE_DEV_INFO,
    VR_MUSIC_USB_MERGE_SONGS,
    VR_MUSIC_USB_MERGE_ALBUM,
    VR_MUSIC_USB_MERGE_ARTIST,
    VR_MUSIC_USB_MERGE_GENRE,
    VR_MUSIC_USB_MERGE_ALBUMSONGS,
    VR_MUSIC_USB_MERGE_ARTISTSONGS,
    VR_MUSIC_USB_MERGE_GENRESONGS,
    VR_MUSIC_USB_MERGE_TYPE
};

VR_MusicDBHelper_USBGeneral::VR_MusicDBHelper_USBGeneral(const MusicDBHelperItem& item)
    : VR_MusicDBHelper(item)
    , m_MergeSQLList(nullptr)
{
    VR_LOGD_FUNC();
}

VR_MusicDBHelper_USBGeneral::~VR_MusicDBHelper_USBGeneral()
{
    VR_LOGD_FUNC();
    delete m_MergeSQLList;
    m_MergeSQLList = nullptr;
}

NCList<VR_String>* VR_MusicDBHelper_USBGeneral::CreateDBTableSQL()
{
    VR_LOGD_FUNC();
    return nullptr;
}

NCList<VR_String>* VR_MusicDBHelper_USBGeneral::MergeDBTableSQL()
{
    VR_LOGD_FUNC();

    if (nullptr != m_MergeSQLList) {
        return m_MergeSQLList;
    }

    m_MergeSQLList = VR_new NCList<VR_String>();
    if (nullptr == m_MergeSQLList) {
        VR_LOGD("new list failed");
        return nullptr;
    }

    for (int i = 0; i < VR_MUSIC_USB_MERGE_NUM; i++) {
        VR_String* str = VR_new VR_String(g_MergeSQLList[i]);
        if (nullptr == str) {
            VR_LOGD("new str failed");
            continue;
        }

        *m_MergeSQLList << str;
    }

    return m_MergeSQLList;
}

}
}
/* EOF */
