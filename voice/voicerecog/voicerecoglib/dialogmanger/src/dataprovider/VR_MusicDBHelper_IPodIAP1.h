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
 * @file VR_MusicDBHelper_IPodIAP1.h
 * @brief iap1 device dbhelper descriptor
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_MUSIC_DB_HELPER_IPOD_IAP1_H
#define VR_MUSIC_DB_HELPER_IPOD_IAP1_H

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

#define VR_MUSIC_IAP1_MERGE_DEV_INFO \
    "INSERT INTO " VR_MUIFA_INFO_TBL_NAME" VALUES('ipod', 2);"

#define VR_MUSIC_IAP1_MERGE_SONGS \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT song_id AS id," VR_MUSIC_DESC_TYPE_SONG_ID" AS type_id," \
    " title as name," \
    " NULL AS pronunciation, -1 AS pronunciation_id FROM REMOTE.TBL_SONG;"

#define VR_MUSIC_IAP1_MERGE_ALBUM \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT album_id AS id," VR_MUSIC_DESC_TYPE_ALBUM_ID" AS type_id," \
    " title AS name,"\
    " NULL AS pronunciation, global_id AS pronunciation_id FROM REMOTE.TBL_ALBUM;"

#define VR_MUSIC_IAP1_MERGE_ARTIST \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT artist_id AS id," VR_MUSIC_DESC_TYPE_ARTIST_ID" AS type_id," \
    " title AS name," \
    " NULL AS pronunciation, global_id AS pronunciation_id FROM REMOTE.TBL_ARTIST;"

#define VR_MUSIC_IAP1_MERGE_GENRE \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT genre_id AS id," VR_MUSIC_DESC_TYPE_GENRE_ID" AS type_id," \
    " title AS name," \
    " genre_gn AS pronunciation, -1 AS pronunciation_id FROM REMOTE.TBL_GENRE;"

#define VR_MUSIC_IAP1_MERGE_PLAYLIST \
    "INSERT INTO MAIN." VR_MUIFA_ENTITY_TBL_NAME \
    " SELECT playlist_id AS id," VR_MUSIC_DESC_TYPE_PLAYLIST_ID" AS type_id," \
    " title AS name," \
    " NULL AS pronunciation, -1 AS pronunciation_id FROM REMOTE.TBL_PLAYLIST;"

#define VR_MUSIC_IAP1_MERGE_ALBUMSONGS \
    "INSERT INTO MAIN." VR_MUIFA_ALBUMSONGS_TBL_NAME\
    " SELECT album_id, song_id FROM REMOTE.TBL_ALBUMSONG;"

#define VR_MUSIC_IAP1_MERGE_ARTISTSONGS \
    "INSERT INTO MAIN." VR_MUIFA_ARTISTSONGS_TBL_NAME \
    " SELECT artist_id,song_id FROM REMOTE.TBL_ARTISTSONG;"

#define VR_MUSIC_IAP1_MERGE_GENRESONGS \
    "INSERT INTO MAIN." VR_MUIFA_GENRESONGS_TBL_NAME \
    " SELECT genre_id, song_id FROM REMOTE.TBL_GENRESONG;"

#define VR_MUSIC_IAP1_MERGE_PLAYLISTSONGS \
    "INSERT INTO MAIN." VR_MUIFA_PLAYLISTSONGS_TBL_NAME \
    " SELECT playlist_id, song_id FROM REMOTE.TBL_PLAYLISTSONG;"

#define VR_MUSIC_IAP1_MERGE_TYPE \
    "INSERT INTO " VR_MUIFA_TYPE_TBL_NAME" VALUES" \
    "(" VR_MUSIC_DESC_TYPE_SONG_ID" , " VR_MUSIC_DESC_TYPE_SONG" )," \
    "(" VR_MUSIC_DESC_TYPE_ALBUM_ID" , " VR_MUSIC_DESC_TYPE_ALBUM" )," \
    "(" VR_MUSIC_DESC_TYPE_ARTIST_ID" , " VR_MUSIC_DESC_TYPE_ARTIST" )," \
    "(" VR_MUSIC_DESC_TYPE_GENRE_ID" , " VR_MUSIC_DESC_TYPE_GENRE" )," \
    "(" VR_MUSIC_DESC_TYPE_PLAYLIST_ID" , " VR_MUSIC_DESC_TYPE_PLAYLIST" )"

#define VR_MUSIC_IAP1_MERGE_NUM 11

/**
 * @brief The VR_MusicDBHelper_IPodIAP1 class
 *
 * class declaration
 */
class VR_MusicDBHelper_IPodIAP1 : public VR_MusicDBHelper
{
public:
    VR_MusicDBHelper_IPodIAP1(const MusicDBHelperItem& item);
    virtual ~VR_MusicDBHelper_IPodIAP1();

public:
    virtual nutshell::NCList<VR_String>* CreateDBTableSQL();
    virtual nutshell::NCList<VR_String>* MergeDBTableSQL();

private:
    VR_MusicDBHelper_IPodIAP1(const VR_MusicDBHelper_IPodIAP1&);
    VR_MusicDBHelper_IPodIAP1 operator =(const VR_MusicDBHelper_IPodIAP1&);
    nutshell::NCList<VR_String>* m_MergeSQLList;
    static const char* g_MergeSQLList[];
};

}
}
#endif /* VR_MUSIC_DB_HELPER_IPOD_IAP1_H */
/* EOF */
