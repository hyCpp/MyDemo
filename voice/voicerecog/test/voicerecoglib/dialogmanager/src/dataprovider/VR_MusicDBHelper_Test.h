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
 * @file VR_MusicDBHelper_Test.h
 * @brief Declaration file of class VR_MusicDBHelper_Test.
 *
 * This file includes the declaration of class VR_MusicDBHelper_Test.
 *
 * @attention used for C++ only.
 */
#ifndef VR_MUSIC_DB_HELPER_TEST_H
#define VR_MUSIC_DB_HELPER_TEST_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "dbhelper/NCSQLiteOpenHelper.h"
#include "VR_MusicDBHelper.h"
#include "VR_MusicResource_FileVBT.h"
#include "gtest/gtest.h"

namespace navi
{
namespace dataprovider
{
#define VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_ALBUMS_SQL \
    "CREATE TABLE IF NOT EXISTS albums (" \
    "ID_VR            INTEGER         PRIMARY KEY AUTOINCREMENT," \
    "GLOBAL_ID        INTEGER         DEFAULT ( -1 )," \
    "ITEM_STATUS      INTEGER         DEFAULT ( 0 )," \
    "UPDATE_STAMP     INTEGER         DEFAULT ( 0 )," \
    "album_id         INTEGER," \
    "album            TEXT" \
    ");"

#define VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_ALBUMS_SQL \
    "BEGIN TRANSACTION;" \
    "REPLACE INTO albums VALUES(1, -1, 3, 2, 1, 'album1');" \
    "REPLACE INTO albums VALUES(2, -1, 3, 3, 2, 'album2');" \
    "REPLACE INTO albums VALUES(3, -1, 1, 4, 3, 'album3');" \
    "REPLACE INTO albums VALUES(4, -1, 1, 5, 4, 'album4');" \
    " COMMIT;"

#define VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_ARTISTS_SQL \
    "CREATE TABLE IF NOT EXISTS artists (" \
    "ID_VR            INTEGER         PRIMARY KEY AUTOINCREMENT," \
    "GLOBAL_ID        INTEGER         DEFAULT ( -1 )," \
    "ITEM_STATUS      INTEGER         DEFAULT ( 0 )," \
    "UPDATE_STAMP     INTEGER         DEFAULT ( 0 )," \
    "artist_id        INTEGER," \
    "artist           TEXT" \
    ");"

#define VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_ARTISTS_SQL \
    "BEGIN TRANSACTION;" \
    "REPLACE INTO artists VALUES(1, -1, 3, 2, 1, 'artist1');" \
    "REPLACE INTO artists VALUES(2, -1, 3, 3, 2, 'artist2');" \
    "REPLACE INTO artists VALUES(3, -1, 1, 4, 3, 'artist3');" \
    "REPLACE INTO artists VALUES(4, -1, 1, 5, 4, 'artist4');" \
    " COMMIT;"

#define VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_COMPOSERS_SQL \
    "CREATE TABLE IF NOT EXISTS composers (" \
    "ID_VR            INTEGER         PRIMARY KEY AUTOINCREMENT," \
    "ITEM_STATUS      INTEGER         DEFAULT ( 0 )," \
    "UPDATE_STAMP     INTEGER         DEFAULT ( 0 )," \
    "composer_id      INTEGER," \
    "composer         TEXT" \
    ");"

#define VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_COMPOSERS_SQL \
    "BEGIN TRANSACTION;" \
    "REPLACE INTO composers VALUES(1, 3, 2, 1, 'composer1');" \
    "REPLACE INTO composers VALUES(2, 3, 3, 2, 'composer2');" \
    "REPLACE INTO composers VALUES(3, 1, 4, 3, 'composer3');" \
    "REPLACE INTO composers VALUES(4, 1, 5, 4, 'composer4');" \
    " COMMIT;"

#define VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_GENRES_SQL \
    "CREATE TABLE IF NOT EXISTS genres (" \
    "ID_VR            INTEGER         PRIMARY KEY AUTOINCREMENT," \
    "GENRE_GN         INTEGER," \
    "ITEM_STATUS      INTEGER         DEFAULT ( 0 )," \
    "UPDATE_STAMP     INTEGER         DEFAULT ( 0 )," \
    "genre_id         INTEGER," \
    "genre            TEXT" \
    ");"

#define VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_GENRES_SQL \
    "BEGIN TRANSACTION;" \
    "REPLACE INTO genres VALUES(1, 0, 3, 2, 1, 'genre1');" \
    "REPLACE INTO genres VALUES(2, 0, 3, 3, 2, 'genre2');" \
    "REPLACE INTO genres VALUES(3, 0, 1, 4, 3, 'genre3');" \
    "REPLACE INTO genres VALUES(4, 0, 1, 5, 4, 'genre4');" \
    " COMMIT;"

#define VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_PLAYLISTS_SQL \
    "CREATE TABLE IF NOT EXISTS playlists (" \
    "ID_VR            INTEGER         PRIMARY KEY AUTOINCREMENT," \
    "ITEM_STATUS      INTEGER         DEFAULT ( 0 )," \
    "UPDATE_STAMP     INTEGER         DEFAULT ( 0 )," \
    "playlist_id      INTEGER," \
    "parent_id        INTEGER," \
    "name             TEXT" \
    ");"

#define VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_PLAYLISTS_SQL \
    "BEGIN TRANSACTION;" \
    "REPLACE INTO playlists VALUES(1, 3, 2, 1, 1, 'playlist1');" \
    "REPLACE INTO playlists VALUES(2, 3, 3, 2, 1, 'playlist2');" \
    "REPLACE INTO playlists VALUES(3, 1, 4, 3, 1, 'playlist3');" \
    "REPLACE INTO playlists VALUES(4, 1, 5, 4, 1, 'playlist4');" \
    " COMMIT;"

#define VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_ITEMS_SQL \
    "CREATE TABLE IF NOT EXISTS items (" \
    "ID_VR            INTEGER         PRIMARY KEY AUTOINCREMENT," \
    "ITEM_STATUS      INTEGER         DEFAULT ( 0 )," \
    "UPDATE_STAMP     INTEGER         DEFAULT ( 0 )," \
    "item_id          INTEGER," \
    "album_id         INTEGER," \
    "artist_id        INTEGER," \
    "genre_id         INTEGER," \
    "composer_id      INTEGER," \
    "is_music         INTEGER         DEFAULT ( 0 )," \
    "is_podcast       INTEGER         DEFAULT ( 0 )," \
    "is_audiobook     INTEGER         DEFAULT ( 0 )," \
    "title            TEXT" \
    ");"

#define VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_ITEMS_SQL \
    "BEGIN TRANSACTION;" \
    "REPLACE INTO items VALUES(1, 3, 2, 1, 1, 1, 1, 1, 0, 0, 0, 'song1');" \
    "REPLACE INTO items VALUES(2, 3, 3, 2, 2, 2, 2, 2, 0, 0, 0, 'song2');" \
    "REPLACE INTO items VALUES(3, 1, 4, 3, 3, 3, 3, 3, 0, 0, 0, 'song3');" \
    "REPLACE INTO items VALUES(4, 1, 5, 4, 4, 4, 4, 4, 0, 0, 0, 'song4');" \
    " COMMIT;"

#define VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_PLAYLISTSMAP_SQL \
    "CREATE TABLE IF NOT EXISTS playlists_map (" \
    "_id              INTEGER         PRIMARY KEY AUTOINCREMENT," \
    "ITEM_STATUS      INTEGER         DEFAULT ( 0 )," \
    "UPDATE_STAMP     INTEGER         DEFAULT ( 0 )," \
    "playlist_id      INTEGER," \
    "item_id          INTEGER" \
    ");"

#define VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_PLAYLISTMAP_SQL \
    "BEGIN TRANSACTION;" \
    "REPLACE INTO playlists_map VALUES(1, 3, 2, 1, 1);" \
    "REPLACE INTO playlists_map VALUES(2, 3, 3, 2, 2);" \
    "REPLACE INTO playlists_map VALUES(3, 1, 4, 3, 3);" \
    "REPLACE INTO playlists_map VALUES(4, 1, 5, 4, 4);" \
    " COMMIT;"

#define VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_TBLDEVICE \
    "CREATE TABLE IF NOT EXISTS TBL_DEVICE (" \
    "DEVICE_UUID      TEXT      PRIMARY KEY," \
    "DB_VERSION       INTEGER   DEFAULT ( 0 )," \
    "UPDATE_STAMP     INTEGER   DEFAULT ( 0 )," \
    "PROTOCOL_TYPE    TEXT" \
    ");"

#define VR_GTEST_IPOD_IAP2_CREATE_VIEW_OF_TESTVIEW \
    "CREATE VIEW IF NOT EXISTS testview" \
    " AS" \
    " SELECT DISTINCT 1 AS nExternalId, A.DEVICE_UUID AS UID," \
    " A.DB_VERSION AS DB_VERSION, A.UPDATE_STAMP AS UPDATE_STAMP" \
    " FROM TBL_DEVICE AS A;"

/**
 * @brief The VR_MusicDBProvider_Test class
 *
 * class declaration
 */
class VR_MusicDBProvider_Test
{
public:
    VR_MusicDBProvider_Test();
    ~VR_MusicDBProvider_Test();

public:
    VOID CreateIpodIap2DataBase(const nutshell::NCCHAR* dbName);
    VOID CreateFileTempDataBase(const nutshell::NCCHAR* dbName);
    VOID CreateFileVBTDataBase(const nutshell::NCCHAR* dbName);

private:
    VR_MusicDBProvider_Test(const VR_MusicDBProvider_Test&);
    VR_MusicDBProvider_Test& operator=(const VR_MusicDBProvider_Test&);
    static const nutshell::NCCHAR* g_IpodIap2BuilderSQL[];
    static const nutshell::NCCHAR* g_FileTempBuilderSQL[];
    static const nutshell::NCCHAR* g_FileVBTBuilderSQL[];
};

/**
 * @brief The VR_MusicDBHelperNotify_Test class
 *
 * class declaration
 */
class VR_MusicDBHelperNotify_Test : public VR_MusicDBHelperNotify
{
public:
    nutshell::UINT32 GetSenderType();
    bool AppendAgentNode(const nutshell::UINT32& sourceId,
                         const nutshell::UINT32& diffNum);
    bool AppendCategoryNode(const nutshell::UINT32& sourceId,
                            const nutshell::NCCHAR* tableName);
    bool AppendDelNode(const nutshell::UINT32& sourceId,
                       const nutshell::NCContentValues& values);
    bool AppendAddNode(const nutshell::UINT32& sourceId,
                       const nutshell::NCContentValues& values);
    bool SendDBDiffNotify(const nutshell::UINT32& sourceId,
                          const MusicDBtableRecords& recordNum);
    bool SendDBInitNotify(const nutshell::UINT32& sourceId,
                          const MusicDBtableRecords& recordNum);
    bool SendActiveNotify(const nutshell::UINT32& sourceId,
                          const bool& IsReply = false);
    bool SendInactiveNotify(const nutshell::UINT32& sourceId,
                            const bool& IsReply = false);
    bool SendPreUpdateNotify(const nutshell::UINT32& sourceId);
    bool SendConnStatusNotify(const nutshell::UINT32& sourceId);
    bool SendCancelNotify(const nutshell::UINT32& sourceId);
};

/**
 * @brief The VR_MusicDBHelper_Test class
 *
 * class declaration
 */
class VR_MusicDBHelper_Test : public testing::Test
{
public:
    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

protected:
    VR_MusicDBHelper_Test();
    ~VR_MusicDBHelper_Test();

private:
    VR_MusicDBHelper_Test(const VR_MusicDBHelper_Test&);
    VR_MusicDBHelper_Test& operator=(const VR_MusicDBHelper_Test&);

    // test suite level
    static VR_MusicDBHelper* m_testSuite;

    // middle test suite level and test level
    static VR_MusicDBHelper* m_testSubSuite;
};

}
}
#endif /* VR_MUSIC_DB_HELPER_TEST_H */
/* EOF */
