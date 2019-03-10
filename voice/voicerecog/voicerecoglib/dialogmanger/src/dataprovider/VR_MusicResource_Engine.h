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
/**
 * @file VR_MusicResource_FileVBT.h
 * @brief Declaration file of vbt format sql resource.
 *
 * This file includes the vbt format sql resource.
 *
 * @attention used for C++ only.
 */
#ifndef VR_MUSIC_RESOURCE_FILE_VBT_H
#define VR_MUSIC_RESOURCE_FILE_VBT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "ncore/NCTypesDefine.h"

namespace navi
{
namespace dataprovider
{

#define VR_MUIFA_INFO_TBL_NAME              "DeviceInformation"
#define VR_MUIFA_ENTITY_TBL_NAME            "Entity"
#define VR_MUIFA_TYPE_TBL_NAME              "Type"
#define VR_MUIFA_ALBUMSONGS_TBL_NAME        "AlbumSongs"
#define VR_MUIFA_ARTISTSONGS_TBL_NAME       "ArtistsSongs"
#define VR_MUIFA_GENRESONGS_TBL_NAME        "GenresSongs"
#define VR_MUIFA_PLAYLISTSONGS_TBL_NAME     "PlaylistsSongs"

#define VR_MUSIC_DESC_TYPE_SONG_ID         "1"
#define VR_MUSIC_DESC_TYPE_ALBUM_ID        "2"
#define VR_MUSIC_DESC_TYPE_ARTIST_ID       "3"
#define VR_MUSIC_DESC_TYPE_GENRE_ID        "4"
#define VR_MUSIC_DESC_TYPE_PLAYLIST_ID     "5"

#define VR_MUSIC_DESC_TYPE_SONG         "'song'"
#define VR_MUSIC_DESC_TYPE_ALBUM        "'album'"
#define VR_MUSIC_DESC_TYPE_ARTIST       "'artist'"
#define VR_MUSIC_DESC_TYPE_GENRE        "'genre'"
#define VR_MUSIC_DESC_TYPE_PLAYLIST     "'playlist'"

#define VR_ATTACH_DB_SQL                "ATTACH DATABASE '%s' AS 'REMOTE';"
#define VR_DEATCH_DB_SQL                "DETACH DATABASE 'REMOTE';"
#define VR_TRANACTION_BEGIN_SQL         "BEGIN TRANSACTION;"
#define VR_TRANACTION_END_SQL           "COMMIT;"


#define VR_FORMAT_SPECIAL_SQL           "UPDATE " VR_MUIFA_ENTITY_TBL_NAME" SET name = 'Unknown' WHERE name IN (\"\", \"\x08\")"


}
}
#endif /* VR_MUSIC_RESOURCE_FILE_VBT_H */
/* EOF */
