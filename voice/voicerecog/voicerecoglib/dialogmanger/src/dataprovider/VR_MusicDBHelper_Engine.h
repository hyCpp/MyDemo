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
 * @file VR_MusicDBHelper_Engine.h
 * @brief voice engine device dbhelper descriptor
 *
 * @attention used for C++ only.
 */

#ifndef VR_MUSIC_DB_HELPER_FILE_TEMP_H
#define VR_MUSIC_DB_HELPER_FILE_TEMP_H

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

#define VR_MUSIC_ENGINE_CREATE_INFO_TABLE_SQL \
    "CREATE TABLE IF NOT EXISTS " VR_MUIFA_INFO_TBL_NAME" (" \
    " name TEXT NOT NULL," \
    " source_id INTEGER NOT NULL);"

#define VR_MUSIC_ENGINE_CREATE_ENTITY_TABLE_SQL \
    "CREATE TABLE IF NOT EXISTS " VR_MUIFA_ENTITY_TBL_NAME" (" \
    " id INTEGER NOT NULL," \
    " type_id INTEGER NOT NULL," \
    " name TEXT NOT NULL," \
    " pronunciation TEXT," \
    " pronunciation_id INTEGER);"

#define VR_MUSIC_ENGINE_CREATE_TYPE_SQL \
    "CREATE TABLE IF NOT EXISTS " VR_MUIFA_TYPE_TBL_NAME" (" \
    " id INTEGER NOT NULL," \
    " type TEXT NOT NULL);"

#define VR_MUSIC_ENGINE_CREATE_ALBUMSONG_SQL \
    "CREATE TABLE IF NOT EXISTS " VR_MUIFA_ALBUMSONGS_TBL_NAME" (" \
    " album_id INTEGER NOT NULL," \
    " song_id INTEGER NOT NULL)"

#define VR_MUSIC_ENGINE_CREATE_ARTISTSSONG_SQL \
    "CREATE TABLE IF NOT EXISTS " VR_MUIFA_ARTISTSONGS_TBL_NAME " (" \
    " artist_id INTEGER NOT NULL," \
    " song_id  INTEGER NOT NULL);"

#define VR_MUSIC_ENGINE_CREATE_GENRESSONGS_SQL \
    "CREATE TABLE IF NOT EXISTS " VR_MUIFA_GENRESONGS_TBL_NAME" (" \
    " genre_id INTEGER NOT NULL,"\
    " song_id INTEGER NOT NULL);"

#define VR_MUSIC_ENGINE_CREATE_PLAYLISTSSONGS_SQL \
    "CREATE TABLE IF NOT EXISTS " VR_MUIFA_PLAYLISTSONGS_TBL_NAME" (" \
    " playlist_id INTEGER NOT NULL," \
    " song_id INTEGER NOT NULL)"

#define VR_MUSIC_ENGINE_MAX 7

/**
 * @brief The VR_MusicDBHelper_FileTemp class
 *
 * class declaration
 */
class VR_MusicDBHelper_Engine : public VR_MusicDBHelper
{
public:
    VR_MusicDBHelper_Engine(const MusicDBHelperItem& item);
    virtual ~VR_MusicDBHelper_Engine();

public:
    virtual nutshell::NCList<VR_String>* CreateDBTableSQL();
    virtual nutshell::NCList<VR_String>* MergeDBTableSQL();

private:
    VR_MusicDBHelper_Engine(const VR_MusicDBHelper_Engine&);
    VR_MusicDBHelper_Engine& operator =(const VR_MusicDBHelper_Engine&);
    nutshell::NCList<VR_String>* m_CreateSQLList;
};

}
}
#endif /* VR_MUSIC_DB_HELPER_USB_GENERAL_H */
/* EOF */
