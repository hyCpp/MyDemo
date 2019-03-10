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
#include "VR_MusicDBHelper_Test.h"
#include "VR_DataProviderComm.h"
// #include "VR_AvcLanSenderIF.h"
#include "VR_MusicFileBuilder.h"
#include "VR_MusicDBHelper_FileTemp.h"
#include "VR_MusicDBHelper_FileVBT.h"
#include "VR_MusicDBHelper_IPodIAP1.h"
#include "VR_MusicDBHelper_IPodIAP2.h"
#include "VR_MusicDBHelper_USBGeneral.h"
#include "VR_MusicDBHelperManager.h"
#include "ncore/NCFile.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
// cover define
#define VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_ARTISTS_SQL \
    "CREATE VIEW IF NOT EXISTS "VR_FILE_VBT_TABLE_OF_ALBUMS_ARTISTS \
    " AS" \
    " SELECT DISTINCT A.nAlbumId AS nAlbumId, B.nArtistId AS nArtistId, 1 AS nSourceId" \
    " FROM MusicAgentAlbumsSongs AS A, MusicAgentArtistsSongs AS B" \
    " WHERE A.nSongId = B.nSongId;"

#define VR_FILE_VBT_CREATE_TABLE_OF_GENRES_ALBUMS_SQL \
    "CREATE VIEW IF NOT EXISTS "VR_FILE_VBT_TABLE_OF_GENRES_ALBUMS \
    " AS" \
    " SELECT DISTINCT A.nGenreId AS nGenreId, B.nAlbumId AS nAlbumId, 1 AS nSourceId" \
    " FROM MusicAgentGenresSongs AS A, MusicAgentAlbumsSongs AS B" \
    " WHERE A.nSongId = B.nSongId;"

#define VR_FILE_VBT_CREATE_TABLE_OF_GENRES_ARTISTS_SQL \
    "CREATE VIEW IF NOT EXISTS "VR_FILE_VBT_TABLE_OF_GENRES_ARTISTS \
    " AS" \
    " SELECT DISTINCT A.nGenreId AS nGenreId, B.nArtistId AS nArtistId, 1 AS nSourceId" \
    " FROM MusicAgentGenresSongs AS A, MusicAgentArtistsSongs AS B" \
    " WHERE A.nSongId = B.nSongId;"

const NCCHAR* VR_MusicDBProvider_Test::g_IpodIap2BuilderSQL[] = {
    VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_ALBUMS_SQL,
    VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_ARTISTS_SQL,
    VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_COMPOSERS_SQL,
    VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_GENRES_SQL,
    VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_PLAYLISTS_SQL,
    VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_ITEMS_SQL,
    VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_PLAYLISTSMAP_SQL,
    VR_GTEST_IPOD_IAP2_CREATE_TABLE_OF_TBLDEVICE,
    VR_GTEST_IPOD_IAP2_CREATE_VIEW_OF_TESTVIEW,
    VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_ALBUMS_SQL,
    VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_ARTISTS_SQL,
    VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_COMPOSERS_SQL,
    VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_GENRES_SQL,
    VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_PLAYLISTS_SQL,
    VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_ITEMS_SQL,
    VR_GTEST_IPOD_IAP2_INSERT_TABLE_OF_PLAYLISTMAP_SQL
};

const NCCHAR* VR_MusicDBProvider_Test::g_FileTempBuilderSQL[] = {
    VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PODCASTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ARTISTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_AUDIOBOOKS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_COMPOSERS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PLAYLISTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_EX_EPISODES_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_EX_PODCASTS_EPISODES_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ARTISTS_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_COMPOSERS_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PLAYLISTS_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_ARTISTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_ALBUMS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_ARTISTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_UID_TABLE_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PODCASTS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ARTISTS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_AUDIOBOOKS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_COMPOSERS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PLAYLISTS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_SONGS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_EX_EPISODES_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_SONGS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_EX_PODCASTS_EPISODES_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ARTISTS_SONGS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_COMPOSERS_SONGS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_SONGS_TEMP_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PLAYLISTS_SONGS_TEMP_SQL
};

const NCCHAR* VR_MusicDBProvider_Test::g_FileVBTBuilderSQL[] = {
    VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PODCASTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ARTISTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_AUDIOBOOKS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_COMPOSERS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PLAYLISTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_EX_EPISODES_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_EX_PODCASTS_EPISODES_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ARTISTS_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_COMPOSERS_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_PLAYLISTS_SONGS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_ALBUMS_ARTISTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_ALBUMS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_GENRES_ARTISTS_SQL,
    VR_FILE_VBT_CREATE_TABLE_OF_UID_TABLE_SQL,
};

VR_MusicDBProvider_Test::VR_MusicDBProvider_Test()
{
}

VR_MusicDBProvider_Test::~VR_MusicDBProvider_Test()
{
}

VOID VR_MusicDBProvider_Test::CreateIpodIap2DataBase(const NCCHAR* dbName)
{
    INT result = 0;

    NCSQLiteDatabase db(dbName);
    INT count = sizeof(g_IpodIap2BuilderSQL) / sizeof(NCCHAR*);
    for (INT i = 0; i < count; i++) {
        result = db.execSQL(g_IpodIap2BuilderSQL[i]);
        if (0 != result) {
            printf("[ RUN      ] ErrCode:%d.%s failed", result, g_IpodIap2BuilderSQL[i]);
            return;
        }
    }
}

VOID VR_MusicDBProvider_Test::CreateFileTempDataBase(const NCCHAR* dbName)
{
    INT result = 0;

    NCSQLiteDatabase db(dbName);
    INT count = sizeof(g_FileTempBuilderSQL) / sizeof(NCCHAR*);
    for (INT i = 0; i < count; i++) {
        result = db.execSQL(g_FileTempBuilderSQL[i]);
        if (0 != result) {
            printf("[ RUN      ] ErrCode:%d.%s failed", result, g_FileTempBuilderSQL[i]);
            return;
        }
    }
}

VOID VR_MusicDBProvider_Test::CreateFileVBTDataBase(const NCCHAR* dbName)
{
    INT result = 0;

    NCSQLiteDatabase db(dbName);
    INT count = sizeof(g_FileVBTBuilderSQL) / sizeof(NCCHAR*);
    for (INT i = 0; i < count; i++) {
        result = db.execSQL(g_FileVBTBuilderSQL[i]);
        if (0 != result) {
            printf("[ RUN      ] ErrCode:%d.%s failed", result, g_FileVBTBuilderSQL[i]);
            return;
        }
    }
}

// UINT32 VR_MusicDBHelperNotify_Test::GetSenderType()
// {
//    RETURN(VR_AvcLanSenderTypeAgent);
// }

bool VR_MusicDBHelperNotify_Test::AppendAgentNode(const UINT32& sourceId,
                                                  const UINT32& diffNum)
{
    printf("[ RUN      ] AppendAgentNode, sourceId:%u, diffNum:%u\n",
           sourceId, diffNum);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::AppendCategoryNode(const UINT32& sourceId,
                                                     const NCCHAR* tableName)
{
    if (NULL == tableName) {
        tableName = "null";
    }
    printf("[ RUN      ] AppendCategoryNode, sourceId:%u, tableName:%s\n",
           sourceId, tableName);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::AppendDelNode(const UINT32& sourceId,
                                                const NCContentValues& values)
{
    printf("[ RUN      ] AppendDelNode, sourceId:%u\n", sourceId);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::AppendAddNode(const UINT32& sourceId,
                                                const NCContentValues& values)
{
    printf("[ RUN      ] AppendAddNode, sourceId:%u\n", sourceId);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::SendDBDiffNotify(const UINT32& sourceId,
                                                   const MusicDBtableRecords& recordNum)
{
    printf("[ RUN      ] SendDBDiffNotify, sourceId:%u, recordNum.total:%u\n",
           sourceId, recordNum.totalNum);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::SendDBInitNotify(const UINT32& sourceId,
                                                   const MusicDBtableRecords& recordNum)
{
    printf("[ RUN      ] SendDBInitNotify, sourceId:%u, recordNum.total:%u\n",
           sourceId, recordNum.totalNum);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::SendActiveNotify(const UINT32& sourceId,
                                                   const bool& IsReply)
{
    printf("[ RUN      ] SendActiveNotify, sourceId:%u, IsReply:%u\n",
           sourceId, (UINT32)IsReply);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::SendInactiveNotify(const UINT32& sourceId,
                                                     const bool& IsReply)
{
    printf("[ RUN      ] SendInactiveNotify, sourceId:%u, IsReply:%u\n",
           sourceId, (UINT32)IsReply);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::SendPreUpdateNotify(const UINT32& sourceId)
{
    printf("[ RUN      ] SendPreUpdateNotify, sourceId:%u\n", sourceId);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::SendConnStatusNotify(const UINT32& sourceId)
{
    printf("[ RUN      ] SendConnStatusNotify, sourceId:%u\n", sourceId);
    RETURN(true);
}

bool VR_MusicDBHelperNotify_Test::SendCancelNotify(const UINT32& sourceId)
{
    printf("[ RUN      ] SendCancelNotify, sourceId:%u\n", sourceId);
    RETURN(true);
}

VR_MusicDBHelper* VR_MusicDBHelper_Test::m_testSuite = NULL;
VR_MusicDBHelper* VR_MusicDBHelper_Test::m_testSubSuite = NULL;

VOID VR_MusicDBHelper_Test::SetUpTestCase()
{
    if (NULL == m_testSuite) {
        MusicDBHelperItem item;
        memset(&item, 0, sizeof(item));

        // init device item
        item.Index = 1;
        item.DeviceItem.linkSourceId = 1;
        strcpy(item.DeviceItem.Uuid, "0102030405060708");
        item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_OFF_LINE;
        strcpy(item.DeviceItem.DeviceName, "/tmp/music_ipodiap2.db");
        item.DeviceItem.DeviceType = MUSIC_DEVICE_TYPE_FILE;
        item.DeviceItem.DevicePortType = 0;
        item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_TEMP;

        // init db item
        item.DBItem.SourceId = 1;
        item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
        strcpy(item.DBItem.DBName, "/tmp/MusicCatalog_1.db.temp");
        item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
        m_testSuite = VR_new VR_MusicDBHelper_FileTemp(item);

        VR_MusicDBHelperManager::Assign();
        VR_MusicDBHelperNotify* notify = VR_MusicDBHelperManager::GetInstance();
        m_testSuite->SetNotify(notify);
    }
}

VOID VR_MusicDBHelper_Test::TearDownTestCase()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
        m_testSuite = NULL;
        VR_MusicDBHelperManager::Dispose();
    }
}

VR_MusicDBHelper_Test::VR_MusicDBHelper_Test()
{
}

VR_MusicDBHelper_Test::~VR_MusicDBHelper_Test()
{
}

TEST_F(VR_MusicDBHelper_Test, NULLDestruction)
{
    MusicDBHelperItem item;
    memset(&item, 0, sizeof(item));

    VR_MusicDBHelper dbHelper(item);
    SUCCEED();
}

TEST_F(VR_MusicDBHelper_Test, LoadDBInfo)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    EXPECT_TRUE(m_testSuite->VR_MusicDBHelper::LoadDBInfo());
}

TEST_F(VR_MusicDBHelper_Test, UpdateVersion)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    EXPECT_TRUE(m_testSuite->VR_MusicDBHelper::UpdateVersion());
}

TEST_F(VR_MusicDBHelper_Test, GetDiffFormat)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    const VOID* ptr;
    ptr = m_testSuite->VR_MusicDBHelper::GetDiffFormat(MUSIC_DRIVER_TYPE_UNKOWN);
    EXPECT_EQ(0, (INT64)ptr);
}

TEST_F(VR_MusicDBHelper_Test, GetTrimSQL)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    VOID* ptr = m_testSuite->VR_MusicDBHelper::GetTrimSQL(MUSIC_DRIVER_TYPE_UNKOWN);
    EXPECT_EQ(0, (INT64)ptr);

    // music_ipodiap2.db not exis sqlite_master table. not formal db
    // BL_File dbFile(BL_FILE_PREFIX_TMP);
    // dbFile.Open("music_ipodiap2.db", BL_FILE_OPEN_MODE_WP);
    // dbFile.Close();
    // NCObjList<NCString>* tableTrimList;
    // tableTrimList = m_testSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    // EXPECT_EQ(0, (INT64)tableTrimList);
    // NCFile::remove("/tmp/music_ipodiap2.db");
}

TEST_F(VR_MusicDBHelper_Test, GetFullCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    VOID* ptr = m_testSuite->VR_MusicDBHelper::GetFullCreateSQL(MUSIC_DRIVER_TYPE_UNKOWN);
    EXPECT_EQ(0, (INT64)ptr);
}

TEST_F(VR_MusicDBHelper_Test, GetDiffCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    VOID* ptr = m_testSuite->VR_MusicDBHelper::GetDiffCreateSQL(MUSIC_DRIVER_TYPE_UNKOWN, 0);
    EXPECT_EQ(0, (INT64)ptr);
}

TEST_F(VR_MusicDBHelper_Test, CreateObject)
{
    // build invalid db file
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_unkown.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();

    MusicDBHelperItem item;
    memset(&item, 0, sizeof(item));
    strcpy(item.DBItem.DBName, "/tmp/music_unkown.db");

    // unkown driver
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_UNKOWN;
    VR_MusicDBHelper* dbHelper = VR_MusicDBHelper::CreateObject(item);
    EXPECT_EQ(0, (INT64)dbHelper);

    // temp driver load info success  ==> UidTable table records is null[true]
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_TEMP;
    dbHelper = VR_MusicDBHelper::CreateObject(item);
    EXPECT_NE(0, (INT64)dbHelper);
    if (NULL != dbHelper) {
        delete dbHelper;
        dbHelper = NULL;
    }

    // vbt driver load info success   ==> UidTable table records is null[true]
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_VBT;
    dbHelper = VR_MusicDBHelper::CreateObject(item);
    EXPECT_NE(0, (INT64)dbHelper);
    if (NULL != dbHelper) {
        delete dbHelper;
        dbHelper = NULL;
    }

    // usb general driver load info failed   ==>  tbl_device table is not  exist[false]
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_USB_GENERAL;
    dbHelper = VR_MusicDBHelper::CreateObject(item);
    EXPECT_EQ(0, (INT64)dbHelper);

    // ipod iap1 driver load info failed     ==>  tbl_device table is not  exist[false]
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP1;
    dbHelper = VR_MusicDBHelper::CreateObject(item);
    EXPECT_EQ(0, (INT64)dbHelper);

    // ipod iap2 driver load info failed     ==>  tbl_device table is not  exist[false]
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;
    dbHelper = VR_MusicDBHelper::CreateObject(item);
    EXPECT_EQ(0, (INT64)dbHelper);

    // ipod iap2 drivder load info success but db file not exist
    NCFile::remove(item.DBItem.DBName);
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;
    dbHelper = VR_MusicDBHelper::CreateObject(item);
    EXPECT_NE(0, (INT64)dbHelper);
    if (NULL != dbHelper) {
        delete dbHelper;
        dbHelper = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, SetOutputBuffer)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    BL_File dbFile(BL_FILE_PREFIX_TMP);

    // buffer object not exist, buffer file not exist
    m_testSuite->SetOutputBuffer("/tmp/MusicCatalog_1.db");

    // buffer object exist, buffer file exist
    dbFile.Open("MusicCatalog_1.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    m_testSuite->SetOutputBuffer("/tmp/MusicCatalog_1.db");

    // buffer object exist, not check buffer file
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->SetOutputBuffer("/tmp/MusicCatalog_1.db");
    SUCCEED();
}

TEST_F(VR_MusicDBHelper_Test, SetNotify)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->SetNotify(NULL);
}

TEST_F(VR_MusicDBHelper_Test, Refresh)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    // outputBuffer is null
    VR_MusicDBHelper* outputBuffer = m_testSuite->m_outputBuffer;
    m_testSuite->m_outputBuffer = NULL;
    EXPECT_FALSE(m_testSuite->Refresh());
    m_testSuite->m_outputBuffer = outputBuffer;

    // update outputBuffer version failed. file not exist UidTable
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("MusicCatalog_1.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSuite->Refresh());

    // file not exist.update version success but device offline
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_OFF_LINE;
    NCFile::remove("/tmp/MusicCatalog_1.db");
    EXPECT_TRUE(m_testSuite->Refresh());

    // update version success and device online but onFlush failed
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    m_testSuite->m_outputBuffer->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_outputBuffer->m_item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_UNKOWN;
    EXPECT_FALSE(m_testSuite->Refresh());
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;

    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;
    EXPECT_TRUE(m_testSuite->Refresh());

    // set notify
    VR_MusicDBHelperNotify* notify = VR_new VR_MusicDBHelperNotify_Test;
    m_testSuite->SetNotify(notify);
    EXPECT_TRUE(m_testSuite->Refresh());

    // release notify
    if (NULL != m_testSuite->m_notify) {
        delete m_testSuite->m_notify;
        m_testSuite->m_notify = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, OnNotifyUpdateDataByNotify)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    MusicDeviceItem deviceItem;
    MusicDBtableRecords recordNum;
    memset(&deviceItem, 0, sizeof(deviceItem));
    memset(&recordNum, 0, sizeof(recordNum));

    // set notify
    VR_MusicDBHelperNotify* notify = VR_new VR_MusicDBHelperNotify_Test;
    m_testSuite->SetNotify(notify);

    // init device item
    deviceItem.linkSourceId = 1;
    strcpy(deviceItem.Uuid, "0102030405060708");
    deviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    strcpy(deviceItem.DeviceName, "music_ipodiap2.db");
    deviceItem.DeviceType = MUSIC_DEVICE_TYPE_IPOD;
    deviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    deviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;

    // OnNotifyUpdate success by major version change
    VR_MusicDBProvider_Test builderDB;
    builderDB.CreateIpodIap2DataBase("/tmp/music_ipodiap2.db");
    builderDB.CreateFileTempDataBase("/tmp/MusicCatalog_1.db.temp");
    builderDB.CreateFileVBTDataBase("/tmp/MusicCatalog_1.db");
    {
        NCSQLiteDatabase remoteAccess("/tmp/music_ipodiap2.db", 0x00, NULL, 0);
        NCCHAR* recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO TBL_DEVICE VALUES('0102030405060708', 3, 5, 'IAP2');" \
            " COMMIT;";
        remoteAccess.execSQL(recordSQL);

        NCSQLiteDatabase localAccess("/tmp/MusicCatalog_1.db.temp", 0x00, NULL, 0);
        recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 2, 1);" \
            " COMMIT;";
        localAccess.execSQL(recordSQL);

        NCSQLiteDatabase dbAccess("/tmp/MusicCatalog_1.db", 0x00, NULL, 0);
        recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 1, 1);" \
            " COMMIT;";
        dbAccess.execSQL(recordSQL);
    }
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 2;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 1;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdate(deviceItem));
    EXPECT_TRUE(m_testSuite->GetFullRecords(recordNum));
    VR_MusicDBHelper* outputBuffer = m_testSuite->m_outputBuffer;
    m_testSuite->m_outputBuffer = NULL;
    EXPECT_FALSE(m_testSuite->GetFullRecords(recordNum));
    m_testSuite->m_outputBuffer = outputBuffer;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 3;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 5;
    m_testSuite->m_outputBuffer->UpdateVersion();

    // OnNotifyUpdate success by minor version change
    {
        NCSQLiteDatabase remoteAccess("/tmp/music_ipodiap2.db", 0x00, NULL, 0);
        NCCHAR* recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO TBL_DEVICE VALUES('0102030405060708', 3, 8, 'IAP2');" \
            " COMMIT;";
        remoteAccess.execSQL(recordSQL);

        recordSQL = "BEGIN TRANSACTION;" \
            "REPLACE INTO items VALUES(5, 3, 6, 1, 1, 1, 1, 1, 0, 0, 0, 'song5');" \
            "REPLACE INTO items VALUES(6, 3, 7, 1, 1, 1, 1, 1, 0, 0, 0, 'song6');" \
            "REPLACE INTO items VALUES(7, 1, 8, 1, 1, 1, 1, 1, 0, 0, 0, 'song7');" \
            "REPLACE INTO items VALUES(8, 1, 9, 1, 1, 1, 1, 1, 0, 0, 0, 'song8');" \
            " COMMIT;";
        remoteAccess.execSQL(recordSQL);
    }
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 3;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 5;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 3;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 5;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdate(deviceItem));
    NCFile::remove("/tmp/music_ipodiap2.db");
    NCFile::remove("/tmp/MusicCatalog_1.db.temp");
    NCFile::remove("/tmp/MusicCatalog_1.db");
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 0;

    // release notify
    if (NULL != m_testSuite->m_notify) {
        delete m_testSuite->m_notify;
        m_testSuite->m_notify = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, OnNotifyUpdateDataByNotNotify)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    MusicDeviceItem deviceItem;
    memset(&deviceItem, 0, sizeof(deviceItem));

    // init device item
    deviceItem.linkSourceId = 1;
    strcpy(deviceItem.Uuid, "0102030405060708");
    deviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    strcpy(deviceItem.DeviceName, "music_ipodiap2.db");
    deviceItem.DeviceType = MUSIC_DEVICE_TYPE_IPOD;
    deviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    deviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;

    // OnNotifyUpdate success by major version change
    VR_MusicDBProvider_Test builderDB;
    builderDB.CreateIpodIap2DataBase("/tmp/music_ipodiap2.db");
    builderDB.CreateFileTempDataBase("/tmp/MusicCatalog_1.db.temp");
    builderDB.CreateFileVBTDataBase("/tmp/MusicCatalog_1.db");
    {
        NCSQLiteDatabase remoteAccess("/tmp/music_ipodiap2.db", 0x00, NULL, 0);
        NCCHAR* recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO TBL_DEVICE VALUES('0102030405060708', 3, 5, 'IAP2');" \
            " COMMIT;";
        remoteAccess.execSQL(recordSQL);

        NCSQLiteDatabase localAccess("/tmp/MusicCatalog_1.db.temp", 0x00, NULL, 0);
        recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 2, 1);" \
            " COMMIT;";
        localAccess.execSQL(recordSQL);

        NCSQLiteDatabase dbAccess("/tmp/MusicCatalog_1.db", 0x00, NULL, 0);
        recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 1, 1);" \
            " COMMIT;";
        dbAccess.execSQL(recordSQL);
    }
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 2;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 1;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdate(deviceItem));
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 3;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 5;
    m_testSuite->m_outputBuffer->UpdateVersion();

    // OnNotifyUpdate success by minor version change
    {
        NCSQLiteDatabase remoteAccess("/tmp/music_ipodiap2.db", 0x00, NULL, 0);
        NCCHAR* recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO TBL_DEVICE VALUES('0102030405060708', 3, 8, 'IAP2');" \
            " COMMIT;";
        remoteAccess.execSQL(recordSQL);

        recordSQL = "BEGIN TRANSACTION;" \
            "REPLACE INTO items VALUES(5, 3, 6, 1, 1, 1, 1, 1, 0, 0, 0, 'song5');" \
            "REPLACE INTO items VALUES(6, 3, 7, 1, 1, 1, 1, 1, 0, 0, 0, 'song6');" \
            "REPLACE INTO items VALUES(7, 1, 8, 1, 1, 1, 1, 1, 0, 0, 0, 'song7');" \
            "REPLACE INTO items VALUES(8, 1, 9, 1, 1, 1, 1, 1, 0, 0, 0, 'song8');" \
            " COMMIT;";
        remoteAccess.execSQL(recordSQL);
    }
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 3;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 5;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 3;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 5;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdate(deviceItem));
    NCFile::remove("/tmp/music_ipodiap2.db");
    NCFile::remove("/tmp/MusicCatalog_1.db.temp");
    NCFile::remove("/tmp/MusicCatalog_1.db");
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 0;
}

TEST_F(VR_MusicDBHelper_Test, OnNotifyUpdate)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    MusicDeviceItem deviceItem;
    memset(&deviceItem, 0, sizeof(deviceItem));

    // init device item
    deviceItem.linkSourceId = 1;
    strcpy(deviceItem.Uuid, "0102030405060708");
    deviceItem.Enable = MUSIC_DEVICE_STATUS_OFF_LINE;
    strcpy(deviceItem.DeviceName, "music_ipodiap2.db");
    deviceItem.DeviceType = MUSIC_DEVICE_TYPE_IPOD;
    deviceItem.DevicePortType = 0;
    deviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;

    // notify device disconnect
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    m_testSuite->m_outputBuffer->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_outputBuffer->m_item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdate(deviceItem));

    // notfiy device connect but create object failed
    deviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    deviceItem.DriverType = MUSIC_DRIVER_TYPE_UNKOWN;
    deviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    EXPECT_FALSE(m_testSuite->OnNotifyUpdate(deviceItem));
    deviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;

    // sync update data failed. full update remote db not exist
    deviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    NCFile::remove("/tmp/music_ipodiap2.db");
    EXPECT_FALSE(m_testSuite->OnNotifyUpdate(deviceItem));
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;

    // OnNotifyUpdate failed and outputBuffer not exist
    VR_MusicDBHelper* outputBuffer = m_testSuite->m_outputBuffer;
    m_testSuite->m_outputBuffer = NULL;
    EXPECT_FALSE(m_testSuite->OnNotifyUpdate(deviceItem));
    m_testSuite->m_outputBuffer = outputBuffer;

    // OnNotifyUpdate success by not version change
    EXPECT_TRUE(m_testSuite->OnNotifyUpdate(deviceItem));
}

TEST_F(VR_MusicDBHelper_Test, OnNotifyUpdateStatus)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    MusicDeviceItem deviceItem;
    memset(&deviceItem, 0, sizeof(deviceItem));

    // init device item
    deviceItem.linkSourceId = 1;
    strcpy(deviceItem.Uuid, "0102030405060708");
    deviceItem.Enable = MUSIC_DEVICE_STATUS_OFF_LINE;
    strcpy(deviceItem.DeviceName, "music_ipodiap2.db");
    deviceItem.DeviceType = MUSIC_DEVICE_TYPE_IPOD;
    deviceItem.DevicePortType = 0;
    deviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;

    // set notify
    VR_MusicDBHelperNotify* notify = VR_new VR_MusicDBHelperNotify_Test;
    m_testSuite->SetNotify(notify);

    // device already offline. offline to offline
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_OFF_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = 0;
    m_testSuite->m_outputBuffer->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_OFF_LINE;
    m_testSuite->m_outputBuffer->m_item.DeviceItem.DevicePortType = 0;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdateStatus(deviceItem));

    // device online to offline. [notify != null , db == inactive]
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdateStatus(deviceItem));

    // device online to offline. [notify != null, db == inactive_mid]
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE_MID;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdateStatus(deviceItem));

    // device online to offline. [notify != null, db == active]
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_ACTIVE;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdateStatus(deviceItem));

    // [notify == null] by m_outputBuffer->OnNotifyUpdateStatus coverage

    // device already online. online to online
    deviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    deviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdateStatus(deviceItem));

    // device offline to online. [notify != null, outputBuffer != null]
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_OFF_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = 0;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdateStatus(deviceItem));

    // release notify
    if (NULL != m_testSuite->m_notify) {
        delete m_testSuite->m_notify;
        m_testSuite->m_notify = NULL;
    }

    // device offline to online. [notify == null, outputBuffer == null]
    VR_MusicDBHelper* outputBuffer = m_testSuite->m_outputBuffer;
    m_testSuite->m_outputBuffer = NULL;
    m_testSuite->m_item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_OFF_LINE;
    m_testSuite->m_item.DeviceItem.DevicePortType = 0;
    EXPECT_TRUE(m_testSuite->OnNotifyUpdateStatus(deviceItem));
    m_testSuite->m_outputBuffer = outputBuffer;
}

TEST_F(VR_MusicDBHelper_Test, SyncUpdatedata)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    MusicDBHelperItem item;
    memset(&item, 0, sizeof(item));

    // init device item
    item.Index = 1;
    item.DeviceItem.linkSourceId = 1;
    strcpy(item.DeviceItem.Uuid, "0102030405060708");
    item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    strcpy(item.DeviceItem.DeviceName, "music_ipodiap2.db");
    item.DeviceItem.DeviceType = MUSIC_DEVICE_TYPE_IPOD;
    item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;

    // init db item
    item.DBItem.SourceId = 1;
    item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    strcpy(item.DBItem.DBName, "/tmp/music_ipodiap2.db");
    item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;

    // dbHelper is null
    EXPECT_FALSE(m_testSuite->SyncUpdateData(NULL));

    // not update [remoteDB <=> localDB]
    VR_MusicDBHelper* dbHelper = VR_new VR_MusicDBHelper_IPodIAP2(item);
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;
    EXPECT_TRUE(m_testSuite->SyncUpdateData(dbHelper));

    // full update failed. remote path not exist. version change small
    dbHelper->m_item.DBItem.DBVersion.CurMajorVer = 1;
    dbHelper->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 2;
    EXPECT_FALSE(m_testSuite->SyncUpdateData(dbHelper));

    // full update failed. remote path not exist. major version change big
    dbHelper->m_item.DBItem.DBVersion.CurMajorVer = 1;
    dbHelper->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;
    EXPECT_FALSE(m_testSuite->SyncUpdateData(dbHelper));

    // diff update failed. getDiff record failed, remote db not exist
    dbHelper->m_item.DBItem.DBVersion.CurMajorVer = 1;
    dbHelper->m_item.DBItem.DBVersion.CurMinorVer = 2;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    EXPECT_FALSE(m_testSuite->SyncUpdateData(dbHelper));

    // diff update success. getDiff\diffUpdate record success. driver not support
    dbHelper->m_item.DBItem.DBVersion.CurMajorVer = 1;
    dbHelper->m_item.DBItem.DBVersion.CurMinorVer = 2;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_UNKOWN;
    EXPECT_TRUE(m_testSuite->SyncUpdateData(dbHelper));
    m_testSuite->m_item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_TEMP;

    // diff update failed. update version failed. not exist UidTable
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("MusicCatalog_1.db.temp", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    dbHelper->m_item.DBItem.DBVersion.CurMajorVer = 1;
    dbHelper->m_item.DBItem.DBVersion.CurMinorVer = 2;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_UNKOWN;
    EXPECT_FALSE(m_testSuite->SyncUpdateData(dbHelper));
    m_testSuite->m_item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_TEMP;
    NCFile::remove("/tmp/MusicCatalog_1.db.temp");

    // build remote database and full update
    VR_MusicDBProvider_Test builderDB;
    builderDB.CreateIpodIap2DataBase("/tmp/music_ipodiap2.db");
    builderDB.CreateFileTempDataBase("/tmp/MusicCatalog_1.db.temp");
    {
        NCSQLiteDatabase remoteAccess("/tmp/music_ipodiap2.db", 0x00, NULL, 0);
        NCCHAR* recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO TBL_DEVICE VALUES('0102030405060708', 2, 1, 'IAP2');" \
            " COMMIT;";
        remoteAccess.execSQL(recordSQL);

        NCSQLiteDatabase localAccess("/tmp/MusicCatalog_1.db.temp", 0x00, NULL, 0);
        recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 1, 1);" \
            " COMMIT;";
        localAccess.execSQL(recordSQL);
    }

    // full update success
    dbHelper->m_item.DBItem.DBVersion.CurMajorVer = 2;
    dbHelper->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    EXPECT_TRUE(m_testSuite->SyncUpdateData(dbHelper));
    NCFile::remove("/tmp/music_ipodiap2.db");
    NCFile::remove("/tmp/MusicCatalog_1.db.temp");

    // release remote dbHelper
    if (NULL != dbHelper) {
        delete dbHelper;
        dbHelper = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, OnFlush)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    // outputBuffer is null
    VR_MusicDBHelper* outputBuffer = m_testSuite->m_outputBuffer;
    m_testSuite->m_outputBuffer = NULL;
    EXPECT_FALSE(m_testSuite->OnFlush());
    m_testSuite->m_outputBuffer = outputBuffer;

    // not data update and notify == null [local database <==> output database]
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 0;
    EXPECT_TRUE(m_testSuite->OnFlush());

    // not data update and notify != null [local database <==> output database]
    m_testSuite->m_notify = VR_new VR_MusicDBHelperNotify_Test;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 0;
    EXPECT_TRUE(m_testSuite->OnFlush());

    // music dictionary has not result
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_UNKOWN;
    EXPECT_FALSE(m_testSuite->OnFlush());

    // music disactive has not result
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE_MID;
    EXPECT_FALSE(m_testSuite->OnFlush());

    // music is active. need first disactive
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_ACTIVE;
    EXPECT_FALSE(m_testSuite->OnFlush());

    // diff update failed.getDiffRecords failed, local db not exist
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 2;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    EXPECT_FALSE(m_testSuite->OnFlush());

    // diff update success and notify != null
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 2;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_UNKOWN;
    VR_MusicDBProvider_Test builderDB;
    builderDB.CreateFileTempDataBase("/tmp/MusicCatalog_1.db.temp");
    EXPECT_TRUE(m_testSuite->OnFlush());
    m_testSuite->m_outputBuffer->m_item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_VBT;

    // diff update success and notify == null
    VR_MusicDBHelperNotify* notify = m_testSuite->m_notify;
    m_testSuite->m_notify = NULL;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_UNKOWN;
    EXPECT_TRUE(m_testSuite->OnFlush());
    m_testSuite->m_outputBuffer->m_item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_VBT;
    m_testSuite->m_notify = notify;

    // MergeDiffToFull failed[music temp db not exist]. version change small
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 0;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 1;
    EXPECT_FALSE(m_testSuite->OnFlush());

    // MergeDiffToFull failed. major version change big
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 2;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 1;
    EXPECT_FALSE(m_testSuite->OnFlush());

    // build local database and full update
    builderDB.CreateFileTempDataBase("/tmp/MusicCatalog_1.db.temp");
    builderDB.CreateFileVBTDataBase("/tmp/MusicCatalog_1.db");
    {
        NCSQLiteDatabase localAccess("/tmp/MusicCatalog_1.db.temp", 0x00, NULL, 0);
        NCCHAR* recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 2, 1);" \
            " COMMIT;";
        localAccess.execSQL(recordSQL);

        NCSQLiteDatabase dbAccess("/tmp/MusicCatalog_1.db", 0x00, NULL, 0);
        recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 1, 1);" \
            " COMMIT;";
        dbAccess.execSQL(recordSQL);
    }

    // full update success and notify != null
    EXPECT_TRUE(m_testSuite->OnFlush());

    // full update success and notify == null
    notify = m_testSuite->m_notify;
    m_testSuite->m_notify = NULL;
    m_testSuite->m_item.DBItem.DBVersion.CurMajorVer = 2;
    m_testSuite->m_item.DBItem.DBVersion.CurMinorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMajorVer = 1;
    m_testSuite->m_outputBuffer->m_item.DBItem.DBVersion.CurMinorVer = 1;
    EXPECT_TRUE(m_testSuite->OnFlush());
    m_testSuite->m_notify = notify;
    NCFile::remove("/tmp/MusicCatalog_1.db.temp");
    NCFile::remove("/tmp/MusicCatalog_1.db");

    // relese notify
    if (NULL != m_testSuite->m_notify) {
        delete m_testSuite->m_notify;
        m_testSuite->m_notify = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, FileTemp_SetUpTestCase)
{
    if (NULL == m_testSubSuite) {
        MusicDBHelperItem item;
        memset(&item, 0, sizeof(item));

        // set device item
        item.Index = 1;
        item.DeviceItem.linkSourceId = 1;
        strcpy(item.DeviceItem.Uuid, "0102030405060708");
        item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
        strcpy(item.DeviceItem.DeviceName, "/tmp/music_filevbt.db");
        item.DeviceItem.DeviceType = MUSIC_DEVICE_TYPE_FILE;
        item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
        item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_TEMP;

        // set db item
        item.DBItem.SourceId = 1;
        item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
        strcpy(item.DBItem.DBName, "/tmp/MusicCatalog_1.db.temp");
        item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
        m_testSubSuite = VR_new VR_MusicDBHelper_FileTemp(item);
    }
}

TEST_F(VR_MusicDBHelper_Test, FileTemp_LoadDBInfo)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);
    EXPECT_TRUE(m_testSubSuite->LoadDBInfo());
}

TEST_F(VR_MusicDBHelper_Test, FileTemp_UpdateVersion)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // update version failed. not exist UidTable
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("MusicCatalog_1.db.temp", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSubSuite->UpdateVersion());
    NCFile::remove("/tmp/MusicCatalog_1.db.temp");

    // update version success
    EXPECT_TRUE(m_testSubSuite->UpdateVersion());
}

TEST_F(VR_MusicDBHelper_Test, FileTemp_GetDiffFormat)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCList<MusicDBTableFormat>* tableFormatList;
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_UNKOWN);
    EXPECT_EQ(0, (INT64)tableFormatList);

    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_VBT);
    EXPECT_NE(0, (INT64)tableFormatList);
    EXPECT_EQ(15, tableFormatList->length());

    // repeat
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_VBT);
    EXPECT_NE(0, (INT64)tableFormatList);
    EXPECT_EQ(15, tableFormatList->length());
}

TEST_F(VR_MusicDBHelper_Test, FileTemp_GetTrimSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* tableTrimList;
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, (INT64)tableTrimList);

    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_VBT);
    EXPECT_NE(0, (INT64)tableTrimList);
    EXPECT_EQ(19, tableTrimList->length());

    // repeat
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_VBT);
    EXPECT_NE(0, (INT64)tableTrimList);
    EXPECT_EQ(19, tableTrimList->length());
}

TEST_F(VR_MusicDBHelper_Test, FileTemp_GetFullCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* fullSqlList;
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, (INT64)fullSqlList);

    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_VBT);
    EXPECT_EQ(0, (INT64)fullSqlList);
}

TEST_F(VR_MusicDBHelper_Test, FileTemp_GetDiffCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* diffSqlList;
    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP, 0);
    EXPECT_EQ(0, (INT64)diffSqlList);

    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_VBT, 0);
    EXPECT_EQ(0, (INT64)diffSqlList);
}

TEST_F(VR_MusicDBHelper_Test, OnDiffUpdate)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // dbHelper is null
    EXPECT_FALSE(m_testSubSuite->OnDiffUpdate(NULL));

    // build vbt dbHelper
    MusicDBHelperItem item;
    memset(&item, 0, sizeof(item));

    // init device item
    item.Index = 1;
    item.DeviceItem.linkSourceId = 1;
    strcpy(item.DeviceItem.Uuid, "0102030405060708");
    item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
    strcpy(item.DeviceItem.DeviceName, "music_filevbt.db");
    item.DeviceItem.DeviceType = MUSIC_DEVICE_TYPE_FILE;
    item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
    item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_VBT;

    // set db item
    item.DBItem.SourceId = 1;
    item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
    strcpy(item.DBItem.DBName, "/tmp/music_filevbt.db");
    item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
    VR_MusicDBHelper* dbHelper = VR_new VR_MusicDBHelper_FileVBT(item);

    // build inc binary file
    VR_MusicFileBuilder fileBuilder;
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicIncList;

    VR_String incFile;
    sAvc_MusicIncUnit musicIncUnit;
    musicIncUnit.m_dwActiveDevice = 1;
    musicIncUnit.m_dwSourceNum = 1;
    musicIncUnit.m_stSourceData[0].m_dwSourceId = 1;
    musicIncUnit.m_stSourceData[0].m_dwSourceType = VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_IPOD;
    musicIncUnit.m_stIPodInfo[0].m_dwSourceId = 1;
    musicIncUnit.m_stIPodInfo[0].m_bIPodNameLen = 16;
    strcpy(musicIncUnit.m_stIPodInfo[0].m_bzIPodName, "0102030405060708");
    strcpy(musicIncUnit.m_stVersion.m_bzCurDataVersion, "0000000100000002");
    strcpy(musicIncUnit.m_stVersion.m_bzOldDataVersion, "0000000100000001");
    musicIncUnit.m_stActiveUpdateUnit.m_bIsActive = 0x00;
    musicIncUnit.m_stActiveUpdateUnit.m_bGrammarDiffUpdate = 0x00;
    fileBuilder.BuildInitFile(fileType, musicIncUnit, incFile);

    // current diff update disable
    EXPECT_TRUE(m_testSubSuite->OnDiffUpdate(dbHelper));

    // curent diff update but MusicCatalog_1.db.temp not exist
    musicIncUnit.m_stActiveUpdateUnit.m_bIsActive = 0x01;
    musicIncUnit.m_stActiveUpdateUnit.m_bGrammarDiffUpdate = 0x01;
    fileBuilder.BuildInitFile(fileType, musicIncUnit, incFile);

    // build local database and diff update
    VR_MusicDBProvider_Test builderDB;
    builderDB.CreateFileVBTDataBase("/tmp/music_filevbt.db");
    builderDB.CreateFileTempDataBase("/tmp/MusicCatalog_1.db.temp");
    {
        NCSQLiteDatabase remoteAccess("/tmp/music_filevbt.db", 0x00, NULL, 0);
        NCCHAR* recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 1, 2);" \
            " COMMIT;";
        remoteAccess.execSQL(recordSQL);

        NCSQLiteDatabase localAccess("/tmp/MusicCatalog_1.db.temp", 0x00, NULL, 0);
        recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708', 1, 1);" \
            " COMMIT;";
        localAccess.execSQL(recordSQL);
    }

    EXPECT_TRUE(m_testSubSuite->OnDiffUpdate(dbHelper));

    // build only header exc binary file
    fileType = VR_DP_ListData_Access::eFileType_MusicExcList;
    UINT32 writedSize = 0;
    sAvc_MusicExcHeader musicExcHeader;
    musicExcHeader.m_bTypeCode = VR_AVC_LISTDATA_MUSIC_EXC_TYPECODE;
    musicExcHeader.m_wRecordNum = 0;
    fileBuilder.m_dpListDataAccess.WriteFileStart(fileType, sizeof(musicExcHeader));
    fileBuilder.m_dpListDataAccess.WriteBuffer(&musicExcHeader, sizeof(musicExcHeader), writedSize);
    fileBuilder.m_dpListDataAccess.WriteFileEnd();

    // build exc binary file
    fileBuilder.AppendAgentNode(1, 4, fileType);
    fileBuilder.AppendCategoryNode("MusicAgentSongs");
    NCContentValues values;
    values.put("nId", "7");
    values.put("cName", "yesterday once more");
    values.put("nSourceId", "1");
    values.put("nExternalId", "7");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    fileBuilder.AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, values);
    values.clear();
    values.put("nId", "2");
    values.put("cName", "show");
    values.put("nSourceId", "1");
    values.put("nExternalId", "2");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    fileBuilder.AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, values);

    fileBuilder.AppendCategoryNode("MusicAgentAlbumsSongs");
    values.clear();
    values.put("nAlbumId", "7");
    values.put("nSongId", "7");
    values.put("nSourceId", "1");
    fileBuilder.AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, values);
    values.clear();
    values.put("nAlbumId", "2");
    values.put("nSongId", "2");
    values.put("nSourceId", "1");
    fileBuilder.AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, values);

    VR_String excFile;
    fileBuilder.GetDiffFile(excFile);

    // diff binary file
    EXPECT_TRUE(m_testSubSuite->OnDiffUpdate(dbHelper));

    // remove file
    NCFile::remove(incFile.c_str());
    NCFile::remove(excFile.c_str());
    NCFile::remove("/tmp/music_filevbt.db");
    NCFile::remove("/tmp/MusicCatalog_1.db.temp");

    // release dbHelper
    if (NULL != dbHelper) {
        delete dbHelper;
        dbHelper = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, FileTemp_TearDownTestCase)
{
    if (NULL != m_testSubSuite) {
        delete m_testSubSuite;
        m_testSubSuite = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, FileVBT_SetUpTestCase)
{
    if (NULL == m_testSubSuite) {
        MusicDBHelperItem item;
        memset(&item, 0, sizeof(item));

        // set device item
        item.Index = 1;
        item.DeviceItem.linkSourceId = 1;
        strcpy(item.DeviceItem.Uuid, "0102030405060708");
        item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
        strcpy(item.DeviceItem.DeviceName, "music_filevbt.db");
        item.DeviceItem.DeviceType = MUSIC_DEVICE_TYPE_FILE;
        item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
        item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_FILE_VBT;

        // set db item
        item.DBItem.SourceId = 1;
        item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
        strcpy(item.DBItem.DBName, "/tmp/music_filevbt.db");
        item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
        m_testSubSuite = VR_new VR_MusicDBHelper_FileVBT(item);
    }
}

TEST_F(VR_MusicDBHelper_Test, FileVBT_LoadDBInfo)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // music_filevbt.db not exist. load db init version:0.0
    EXPECT_TRUE(m_testSubSuite->LoadDBInfo());

    // music_filevbt.db exist. sourceId == 1
    VR_MusicDBProvider_Test builderDB;
    builderDB.CreateFileVBTDataBase("/tmp/music_filevbt.db");
    {
        NCSQLiteDatabase dbAccess("/tmp/music_filevbt.db", 0x00, NULL, 0);
        NCCHAR* recordSQL = "BEGIN TRANSACTION;" \
            " REPLACE INTO UidTable VALUES(1, '0102030405060708');" \
            " COMMIT;";
        dbAccess.execSQL(recordSQL);
    }
    EXPECT_TRUE(m_testSubSuite->LoadDBInfo());

    // music_filevbt.db exist. source != 1
    m_testSubSuite->m_item.DBItem.SourceId = 2;
    EXPECT_TRUE(m_testSubSuite->LoadDBInfo());
    m_testSubSuite->m_item.DBItem.SourceId = 1;
    NCFile::remove("/tmp/music_filevbt.db");
}

TEST_F(VR_MusicDBHelper_Test, FileVBT_UpdateVersion)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // music_filevbt.db not exist UidTable
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_filevbt.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSubSuite->UpdateVersion());
    NCFile::remove("/tmp/music_filevbt.db");

    // music_filevbt.db not exist. update db version success
    EXPECT_TRUE(m_testSubSuite->UpdateVersion());
}

TEST_F(VR_MusicDBHelper_Test, FileVBT_GetDiffFormat)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCList<MusicDBTableFormat>* tableFormatList;
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_VBT);
    EXPECT_EQ(0, (INT64)tableFormatList);

    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, (INT64)tableFormatList);
}

TEST_F(VR_MusicDBHelper_Test, FileVBT_GetTrimSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* tableTrimList;
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_VBT);
    EXPECT_EQ(0, (INT64)tableTrimList);

    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, (INT64)tableTrimList);
}

TEST_F(VR_MusicDBHelper_Test, FileVBT_GetFullCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* fullSqlList;
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_VBT);
    EXPECT_EQ(0, (INT64)fullSqlList);

    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)fullSqlList);
    EXPECT_EQ(15, fullSqlList->length());

    // repeat
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)fullSqlList);
    EXPECT_EQ(15, fullSqlList->length());
}

TEST_F(VR_MusicDBHelper_Test, FileVBT_GetDiffCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* diffSqlList;
    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_VBT, 0);
    EXPECT_EQ(0, (INT64)diffSqlList);

    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP, 0);
    EXPECT_EQ(0, (INT64)diffSqlList);
}

TEST_F(VR_MusicDBHelper_Test, FileVBT_TearDownTestCase)
{
    if (NULL != m_testSubSuite) {
        delete m_testSubSuite;
        m_testSubSuite = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_SetUpTestCase)
{
    if (NULL == m_testSubSuite) {
        MusicDBHelperItem item;
        memset(&item, 0, sizeof(item));

        // set device item
        item.Index = 1;
        item.DeviceItem.linkSourceId = 1;
        strcpy(item.DeviceItem.Uuid, "0102030405060708");
        item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
        strcpy(item.DeviceItem.DeviceName, "music_ipodiap1.db");
        item.DeviceItem.DeviceType = MUSIC_DEVICE_TYPE_IPOD;
        item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod1;
        item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP1;

        // set db item
        item.DBItem.SourceId = 1;
        item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
        strcpy(item.DBItem.DBName, "/tmp/music_ipodiap1.db");
        item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
        m_testSubSuite = VR_new VR_MusicDBHelper_IPodIAP1(item);
    }
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_NULLDestruction)
{
    MusicDBHelperItem item;
    memset(&item, 0, sizeof(item));

    VR_MusicDBHelper_IPodIAP1 dbHelper(item);
    SUCCEED();
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_LoadDBInfo)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // music_ipodiap1.db not exist. load db init version:0.0
    EXPECT_TRUE(m_testSubSuite->LoadDBInfo());

    // music_ipodiap1.db not exist UidTable.load db failed
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_ipodiap1.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSubSuite->LoadDBInfo());
    NCFile::remove("/tmp/music_ipodiap1.db");
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_UpdateVersion)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // music_ipodiap1.db not exist. update db success
    EXPECT_TRUE(m_testSubSuite->UpdateVersion());

    // music_ipodiap1.db not exist UitTable.update db failed
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_ipodiap1.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSubSuite->UpdateVersion());
    NCFile::remove("/tmp/music_ipodiap1.db");
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_GetDiffFormat)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCList<MusicDBTableFormat>* tableFormatList;
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_IPOD_IAP1);
    EXPECT_EQ(0, (INT64)tableFormatList);

    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)tableFormatList);
    EXPECT_EQ(14, tableFormatList->length());

    // repeat
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)tableFormatList);
    EXPECT_EQ(14, tableFormatList->length());
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_GetTrimSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* tableTrimList;
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_IPOD_IAP1);
    EXPECT_EQ(0, (INT64)tableTrimList);

    // music_ipodiap1.db not exis sqlite_master table. not formal db
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_ipodiap1.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, (INT64)tableTrimList);
    NCFile::remove("/tmp/music_ipodiap1.db");

    // music_ipodiap1.db not exist.trim success but sql null
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, tableTrimList->length());

    // repeat
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, tableTrimList->length());
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_GetFullCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // drover not support
    NCObjList<NCString>* fullSqlList;
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_IPOD_IAP1);
    EXPECT_EQ(0, (INT64)fullSqlList);

    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)fullSqlList);
    EXPECT_EQ(50, fullSqlList->length());

    // repeat
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)fullSqlList);
    EXPECT_EQ(50, fullSqlList->length());
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_GetDiffCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* diffSqlList;
    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_IPOD_IAP1, 0);
    EXPECT_EQ(0, (INT64)diffSqlList);

    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP, 0);
    EXPECT_EQ(0, (INT64)diffSqlList);
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP1_TearDownTestCase)
{
    if (NULL != m_testSubSuite) {
        delete m_testSubSuite;
        m_testSubSuite = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_SetUpTestCase)
{
    if (NULL == m_testSubSuite) {
        MusicDBHelperItem item;
        memset(&item, 0, sizeof(item));

        // set device item
        item.Index = 1;
        item.DeviceItem.linkSourceId = 1;
        strcpy(item.DeviceItem.Uuid, "0102030405060708");
        item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
        strcpy(item.DeviceItem.DeviceName, "music_ipodiap2.db");
        item.DeviceItem.DeviceType = MUSIC_DEVICE_TYPE_IPOD;
        item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_Ipod2;
        item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_IPOD_IAP2;

        // set db item
        item.DBItem.SourceId = 1;
        item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
        strcpy(item.DBItem.DBName, "/tmp/music_ipodiap2.db");
        item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
        m_testSubSuite = VR_new VR_MusicDBHelper_IPodIAP2(item);
    }
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_NULLDestruction)
{
    MusicDBHelperItem item;
    memset(&item, 0, sizeof(item));

    VR_MusicDBHelper_IPodIAP2 dbHelper(item);
    SUCCEED();
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_LoadDBInfo)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // music_ipodiap2.db not exist. load db init version:0.0
    EXPECT_TRUE(m_testSubSuite->LoadDBInfo());

    // music_ipodiap2.db not exist UidTable.load db failed
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_ipodiap2.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSubSuite->LoadDBInfo());
    NCFile::remove("/tmp/music_ipodiap2.db");
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_UpdateVersion)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // music_ipodiap2.db not exist. update db success
    EXPECT_TRUE(m_testSubSuite->UpdateVersion());

    // music_ipodiap2.db not exist UitTable.update db failed
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_ipodiap2.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSubSuite->UpdateVersion());
    NCFile::remove("/tmp/music_ipodiap2.db");
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_GetDiffFormat)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCList<MusicDBTableFormat>* tableFormatList;
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_IPOD_IAP2);
    EXPECT_EQ(0, (INT64)tableFormatList);

    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)tableFormatList);
    EXPECT_EQ(7, tableFormatList->length());

    // repeat
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)tableFormatList);
    EXPECT_EQ(7, tableFormatList->length());
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_GetTrimSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* tableTrimList;
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_IPOD_IAP2);
    EXPECT_EQ(0, (INT64)tableTrimList);

    // repeat
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)tableTrimList);
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_GetFullCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // drover not support
    NCObjList<NCString>* fullSqlList;
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_IPOD_IAP2);
    EXPECT_EQ(0, (INT64)fullSqlList);

    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    ASSERT_NE(0, (INT64)fullSqlList);
    EXPECT_EQ(50, fullSqlList->length());

    // repeat
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    ASSERT_NE(0, (INT64)fullSqlList);
    EXPECT_EQ(50, fullSqlList->length());
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_GetDiffCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* diffSqlList;
    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_IPOD_IAP2, 0);
    EXPECT_EQ(0, (INT64)diffSqlList);

    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP, 0);
    EXPECT_NE(0, (INT64)diffSqlList);
    EXPECT_EQ(15, diffSqlList->length());

    // repeat
    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP, 0);
    EXPECT_NE(0, (INT64)diffSqlList);
    EXPECT_EQ(15, diffSqlList->length());
}

TEST_F(VR_MusicDBHelper_Test, IPodIAP2_TearDownTestCase)
{
    if (NULL != m_testSubSuite) {
        delete m_testSubSuite;
        m_testSubSuite = NULL;
    }
}

TEST_F(VR_MusicDBHelper_Test, USBGeneral_SetUpTestCase)
{
    if (NULL == m_testSubSuite) {
        MusicDBHelperItem item;
        memset(&item, 0, sizeof(item));

        // set device item
        item.Index = 1;
        item.DeviceItem.linkSourceId = 1;
        strcpy(item.DeviceItem.Uuid, "0102030405060708");
        item.DeviceItem.Enable = MUSIC_DEVICE_STATUS_ON_LINE;
        strcpy(item.DeviceItem.DeviceName, "music_usbgeneral.db");
        item.DeviceItem.DeviceType = MUSIC_DEVICE_TYPE_USB;
        item.DeviceItem.DevicePortType = DM_MediaInfo_SrcID_USB1;
        item.DeviceItem.DriverType = MUSIC_DRIVER_TYPE_USB_GENERAL;

        // set db item
        item.DBItem.SourceId = 1;
        item.DBItem.Enable = MUSIC_DB_STATUS_INACTIVE;
        strcpy(item.DBItem.DBName, "/tmp/music_usbgeneral.db");
        item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_SUCCESS;
        m_testSubSuite = VR_new VR_MusicDBHelper_USBGeneral(item);
    }
}

TEST_F(VR_MusicDBHelper_Test, USBGeneral_NULLDestruction)
{
    MusicDBHelperItem item;
    memset(&item, 0, sizeof(item));

    VR_MusicDBHelper_USBGeneral dbHelper(item);
    SUCCEED();
}

TEST_F(VR_MusicDBHelper_Test, USBGeneral_LoadDBInfo)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // music_usbgeneral.db not exist. load db init version:0.0
    EXPECT_TRUE(m_testSubSuite->LoadDBInfo());

    // music_usbgeneral.db not exist UidTable.load db failed
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_usbgeneral.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSubSuite->LoadDBInfo());
    NCFile::remove("/tmp/music_usbgeneral.db");
}

TEST_F(VR_MusicDBHelper_Test, USBGeneral_UpdateVersion)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // music_usbgeneral.db not exist. update db success
    EXPECT_TRUE(m_testSubSuite->UpdateVersion());

    // music_usbgeneral.db not exist UitTable.update db failed
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_usbgeneral.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_FALSE(m_testSubSuite->UpdateVersion());
    NCFile::remove("/tmp/music_usbgeneral.db");
}

TEST_F(VR_MusicDBHelper_Test, USBGeneral_GetDiffFormat)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCList<MusicDBTableFormat>* tableFormatList;
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_USB_GENERAL);
    EXPECT_EQ(0, (INT64)tableFormatList);

    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)tableFormatList);
    EXPECT_EQ(9, tableFormatList->length());

    // repeat
    tableFormatList = m_testSubSuite->GetDiffFormat(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_NE(0, (INT64)tableFormatList);
    EXPECT_EQ(9, tableFormatList->length());
}

TEST_F(VR_MusicDBHelper_Test, USBGeneral_GetTrimSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* tableTrimList;
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_USB_GENERAL);
    EXPECT_EQ(0, (INT64)tableTrimList);

    // music_usbgeneral.db not exis sqlite_master table. not formal db
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("music_usbgeneral.db", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, (INT64)tableTrimList);
    NCFile::remove("/tmp/music_usbgeneral.db");

    // music_usbgeneral.db not exist.trim success but sql null
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, tableTrimList->length());

    // repeat
    tableTrimList = m_testSubSuite->GetTrimSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    EXPECT_EQ(0, tableTrimList->length());
}

TEST_F(VR_MusicDBHelper_Test, USBgeneral_GetFullCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // drover not support
    NCObjList<NCString>* fullSqlList;
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_USB_GENERAL);
    EXPECT_EQ(0, (INT64)fullSqlList);

    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    ASSERT_NE(0, (INT64)fullSqlList);
    EXPECT_EQ(48, fullSqlList->length());

    // repeat
    fullSqlList = m_testSubSuite->GetFullCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP);
    ASSERT_NE(0, (INT64)fullSqlList);
    EXPECT_EQ(48, fullSqlList->length());
}

TEST_F(VR_MusicDBHelper_Test, USBGeneral_GetDiffCreateSQL)
{
    ASSERT_NE(0, (INT64)m_testSubSuite);

    // driver not support
    NCObjList<NCString>* diffSqlList;
    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_USB_GENERAL, 0);
    EXPECT_EQ(0, (INT64)diffSqlList);

    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP, 0);
    EXPECT_NE(0, (INT64)diffSqlList);
    EXPECT_EQ(9, diffSqlList->length());

    // repeat
    diffSqlList = m_testSubSuite->GetDiffCreateSQL(MUSIC_DRIVER_TYPE_FILE_TEMP, 0);
    EXPECT_NE(0, (INT64)diffSqlList);
    EXPECT_EQ(9, diffSqlList->length());
}

TEST_F(VR_MusicDBHelper_Test, USBGeneral_TearDownTestCase)
{
    if (NULL != m_testSubSuite) {
        delete m_testSubSuite;
        m_testSubSuite = NULL;
    }
}

}
}
/* EOF */
