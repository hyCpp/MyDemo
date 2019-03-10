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
 * @file VR_MusicDBHelper_USBGeneral.h
 * @brief u-disk device dbhelper descriptor
 *
 * @attention used for C++ only.
 */

#ifndef VR_MUSIC_DB_HELPER_USB_GENERAL_H
#define VR_MUSIC_DB_HELPER_USB_GENERAL_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include "VR_MusicDBHelper.h"
#include "VR_MusicResource_Engine.h"

namespace navi
{
namespace dataprovider
{

#define VR_MUSIC_USB_MERGE_DEV_INFO \
    "INSERT INTO " VR_MUIFA_INFO_TBL_NAME" VALUES('usb', 1);"

#define VR_MUSIC_USB_MERGE_SONGS \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT F.ID AS id," VR_MUSIC_DESC_TYPE_SONG_ID" AS type_id," \
    " F.TITLE AS name, NULL AS pronunciation, -1 AS pronunciation_id" \
    " FROM REMOTE.TBL_FILES AS F WHERE IS_ACTIVE = 1 AND DEVICE = 'mediashare';"

#define VR_MUSIC_USB_MERGE_ALBUM \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT A.ID AS id," VR_MUSIC_DESC_TYPE_ALBUM_ID" AS type_id," \
    " A.NAME AS name, NULL AS pronunciation, -1 AS pronunciation_id" \
    " FROM REMOTE.TBL_ALBUMS AS A WHERE A.ID IN"\
    " (SELECT A.ALBUM_ID FROM REMOTE.TBL_TAGS AS A JOIN REMOTE.TBL_FILES AS B ON" \
    " A.FILE_ID = B.ID AND B.IS_ACTIVE = 1 AND B.DEVICE = 'mediashare');"

#define VR_MUSIC_USB_MERGE_ARTIST \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT A.ID AS id," VR_MUSIC_DESC_TYPE_ARTIST_ID" AS type_id," \
    " A.NAME AS name, NULL AS pronunciation, -1 AS pronunciation_id" \
    " FROM REMOTE.TBL_ARTISTS AS A WHERE A.ID IN" \
    " (SELECT A.ARTIST_ID FROM REMOTE.TBL_TAGS AS A JOIN REMOTE.TBL_FILES AS B ON" \
    " A.FILE_ID = B.ID AND B.IS_ACTIVE = 1 AND B.DEVICE = 'mediashare');"

#define VR_MUSIC_USB_MERGE_GENRE \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT G.ID AS id," VR_MUSIC_DESC_TYPE_GENRE_ID" AS type_id," \
    " G.NAME AS name, NULL AS pronunciation, -1 AS pronunciation_id" \
    " FROM REMOTE.TBL_GENRES AS G WHERE G.ID IN" \
    " (SELECT A.GENRE_ID FROM REMOTE.TBL_TAGS AS A JOIN REMOTE.TBL_FILES AS B ON"\
    " A.FILE_ID = B.ID AND B.IS_ACTIVE = 1 AND B.DEVICE = 'mediashare');"

#define VR_MUSIC_USB_MERGE_ALBUMSONGS \
    "INSERT INTO MAIN." VR_MUIFA_ALBUMSONGS_TBL_NAME\
    " SELECT A.ALBUM_ID AS album_id, A.FILE_ID AS song_id" \
    " FROM REMOTE.TBL_TAGS AS A JOIN REMOTE.TBL_FILES AS B" \
    " ON A.FILE_ID = B.ID AND B.IS_ACTIVE = 1 AND B.DEVICE = 'mediashare';"

#define VR_MUSIC_USB_MERGE_ARTISTSONGS \
    "INSERT INTO MAIN." VR_MUIFA_ARTISTSONGS_TBL_NAME\
    " SELECT A.ARTIST_ID AS artist_id, A.FILE_ID AS song_id" \
    " FROM REMOTE.TBL_TAGS AS A JOIN REMOTE.TBL_FILES AS B" \
    " ON A.FILE_ID = B.ID AND B.IS_ACTIVE = 1 AND B.DEVICE = 'mediashare';"

#define VR_MUSIC_USB_MERGE_GENRESONGS \
    "INSERT INTO MAIN." VR_MUIFA_GENRESONGS_TBL_NAME \
    " SELECT A.GENRE_ID AS genre_id, A.FILE_ID AS song_id" \
    " FROM REMOTE.TBL_TAGS AS A JOIN REMOTE.TBL_FILES AS B" \
    " ON A.FILE_ID = B.ID AND B.IS_ACTIVE = 1 AND B.DEVICE = 'mediashare';"

#define VR_MUSIC_USB_MERGE_TYPE \
    "INSERT INTO " VR_MUIFA_TYPE_TBL_NAME" VALUES" \
    " (" VR_MUSIC_DESC_TYPE_SONG_ID" , " VR_MUSIC_DESC_TYPE_SONG" )," \
    " (" VR_MUSIC_DESC_TYPE_ALBUM_ID" , " VR_MUSIC_DESC_TYPE_ALBUM" )," \
    " (" VR_MUSIC_DESC_TYPE_ARTIST_ID" , " VR_MUSIC_DESC_TYPE_ARTIST" )," \
    " (" VR_MUSIC_DESC_TYPE_GENRE_ID" , " VR_MUSIC_DESC_TYPE_GENRE" )," \
    " (" VR_MUSIC_DESC_TYPE_PLAYLIST_ID" , " VR_MUSIC_DESC_TYPE_PLAYLIST" )"

#define VR_MUSIC_USB_MERGE_NUM 9

/**
 * @brief The VR_MusicDBHelper_USBGeneral class
 *
 * class declaration
 */
class VR_MusicDBHelper_USBGeneral : public VR_MusicDBHelper
{
public:
    VR_MusicDBHelper_USBGeneral(const MusicDBHelperItem& item);
    virtual ~VR_MusicDBHelper_USBGeneral();

public:
    virtual nutshell::NCList<VR_String>* CreateDBTableSQL();
    virtual nutshell::NCList<VR_String>* MergeDBTableSQL();

private:
    VR_MusicDBHelper_USBGeneral(const VR_MusicDBHelper_USBGeneral&);
    VR_MusicDBHelper_USBGeneral& operator =(const VR_MusicDBHelper_USBGeneral&);
    nutshell::NCList<VR_String>* m_MergeSQLList;
    static const char* g_MergeSQLList[];
};

}
}
#endif /* VR_MUSIC_DB_HELPER_USB_GENERAL_H */
/* EOF */
