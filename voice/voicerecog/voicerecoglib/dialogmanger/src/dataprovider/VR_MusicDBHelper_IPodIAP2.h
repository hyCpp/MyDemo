/**
 * Copyright @ 2013 - 2018 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_MusicDBHelper_IPodIAP2.h
 * @brief iap2 device dbhelper descriptor
 *
 * @attention used for C++ only.
 */

#ifndef VR_MUSIC_DB_HELPER_IPOD_IAP2_H
#define VR_MUSIC_DB_HELPER_IPOD_IAP2_H

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

#define VR_MUSIC_IAP2_MERGE_DEV_INFO \
    "INSERT INTO " VR_MUIFA_INFO_TBL_NAME" VALUES('ipod', 2);"

#define VR_MUSIC_IAP2_MERGE_SONGS \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT ID_VR as id," VR_MUSIC_DESC_TYPE_SONG_ID" AS type_id," \
    " title as name," \
    " NULL AS pronunciation, -1 AS pronunciation_id FROM REMOTE.items" \
    " WHERE ITEM_STATUS = 3 AND IS_MUSIC = 1;"

#define VR_MUSIC_IAP2_MERGE_ALBUM \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT ID_VR as id," VR_MUSIC_DESC_TYPE_ALBUM_ID" AS type_id," \
    " album as name," \
    " NULL as pronunciation, global_id as pronunciation_id FROM REMOTE.albums" \
    " WHERE ITEM_STATUS = 3;"

#define VR_MUSIC_IAP2_MERGE_ARTIST \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT ID_VR as id," VR_MUSIC_DESC_TYPE_ARTIST_ID" AS type_id," \
    " artist as name," \
    " NULL as pronunciation, global_id as pronunciation_id FROM REMOTE.artists" \
    " WHERE ITEM_STATUS = 3;"

#define VR_MUSIC_IAP2_MERGE_GENRE \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT ID_VR as id," VR_MUSIC_DESC_TYPE_GENRE_ID" AS type_id," \
    " genre as name," \
    " GENRE_GN as pronunciation, -1 as pronunciation_id FROM REMOTE.genres" \
    " WHERE ITEM_STATUS = 3;"

#define VR_MUSIC_IAP2_MERGE_PLAYLIST \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT ID_VR as id," VR_MUSIC_DESC_TYPE_PLAYLIST_ID" AS type_id," \
    " name as name," \
    " NULL as pronunciation, -1 as pronunciation_id FROM REMOTE.playlists" \
    " WHERE ITEM_STATUS = 3 AND is_station = 0 AND is_folder = 0;"

#define VR_MUSIC_IAP2_MERGE_ALBUMSONGS \
    "INSERT INTO MAIN." VR_MUIFA_ALBUMSONGS_TBL_NAME \
    " SELECT A.ID_VR AS album_id, S.ID_VR AS song_id FROM REMOTE.ITEMS AS S," \
    " REMOTE.ALBUMS AS A WHERE S.album_id = A.album_id AND S.ITEM_STATUS = 3 AND" \
    " A.ITEM_STATUS = 3 AND S.IS_MUSIC = 1;"

#define VR_MUSIC_IAP2_MERGE_ARTISTSONGS \
    "INSERT INTO MAIN." VR_MUIFA_ARTISTSONGS_TBL_NAME \
    " SELECT A.ID_VR AS artist_id, S.ID_VR AS song_id FROM REMOTE.ITEMS AS S," \
    " REMOTE.ARTISTS AS A WHERE S.artist_id = A.artist_id AND S.ITEM_STATUS = 3 AND" \
    " A.ITEM_STATUS = 3 AND S.IS_MUSIC = 1;"

#define VR_MUSIC_IAP2_MERGE_GENRESONGS \
    "INSERT INTO MAIN." VR_MUIFA_GENRESONGS_TBL_NAME \
    " SELECT A.ID_VR AS genre_id, S.ID_VR AS song_id FROM REMOTE.ITEMS AS S," \
    " REMOTE.GENRES AS A WHERE S.genre_id = A.genre_id AND S.ITEM_STATUS = 3 AND" \
    " A.ITEM_STATUS = 3 AND S.IS_MUSIC = 1;"

#define VR_MUSIC_IAP2_MERGE_PLAYLISTSONGS \
    "INSERT INTO MAIN." VR_MUIFA_PLAYLISTSONGS_TBL_NAME \
    " SELECT A.ID_VR AS playlist_id, S.ID_VR AS song_id FROM REMOTE.ITEMS AS S," \
    " REMOTE.PLAYLISTS AS A, REMOTE.PLAYLISTS_MAP AS B WHERE B.playlist_id = A.playlist_id" \
    " AND S.ITEM_STATUS = 3 AND B.ITEM_STATUS = 3 AND A.ITEM_STATUS = 3" \
    " AND S.IS_MUSIC = 1 AND A.is_station = 0 AND A.is_folder = 0;"

#define VR_MUSIC_IAP2_MERGE_TYPE \
    "INSERT INTO " VR_MUIFA_TYPE_TBL_NAME" VALUES" \
    "(" VR_MUSIC_DESC_TYPE_SONG_ID" , " VR_MUSIC_DESC_TYPE_SONG" )," \
    "(" VR_MUSIC_DESC_TYPE_ALBUM_ID" , " VR_MUSIC_DESC_TYPE_ALBUM" )," \
    "(" VR_MUSIC_DESC_TYPE_ARTIST_ID" , " VR_MUSIC_DESC_TYPE_ARTIST" )," \
    "(" VR_MUSIC_DESC_TYPE_GENRE_ID" , " VR_MUSIC_DESC_TYPE_GENRE" )," \
    "(" VR_MUSIC_DESC_TYPE_PLAYLIST_ID" , " VR_MUSIC_DESC_TYPE_PLAYLIST" )"

#define VR_MUSIC_IAP2_MERGE_NUM 11

/**
 * @brief The VR_MusicDBHelper_IPodIAP1 class
 *
 * class declaration
 */
class VR_MusicDBHelper_IPodIAP2 : public VR_MusicDBHelper
{
public:
    VR_MusicDBHelper_IPodIAP2(const MusicDBHelperItem& item);
    virtual ~VR_MusicDBHelper_IPodIAP2();

public:
    virtual nutshell::NCList<VR_String>* CreateDBTableSQL();
    virtual nutshell::NCList<VR_String>* MergeDBTableSQL();

private:
    VR_MusicDBHelper_IPodIAP2(const VR_MusicDBHelper_IPodIAP2&);
    VR_MusicDBHelper_IPodIAP2& operator =(const VR_MusicDBHelper_IPodIAP2&);
    nutshell::NCList<VR_String>* m_MergeSQLList;
    static const char* g_MergeSQLList[];
};

}
}
#endif /* VR_MUSIC_DB_HELPER_IPOD_IAP2_H */
/* EOF */
