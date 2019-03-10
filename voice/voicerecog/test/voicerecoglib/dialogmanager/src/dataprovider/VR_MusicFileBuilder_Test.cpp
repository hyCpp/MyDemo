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
#include "VR_MusicFileBuilder_Test.h"
#include "ncore/NCFile.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_MusicFileBuilder* VR_MusicFileBuilder_Test::m_testSuite = NULL;

VOID VR_MusicFileBuilder_Test::SetUpTestCase()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_new VR_MusicFileBuilder();
    }
}

VOID VR_MusicFileBuilder_Test::TearDownTestCase()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
        m_testSuite = NULL;
    }
}

VR_MusicFileBuilder_Test::VR_MusicFileBuilder_Test()
{
    m_test = NULL;
}

VR_MusicFileBuilder_Test::~VR_MusicFileBuilder_Test()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

VOID VR_MusicFileBuilder_Test::SetUp()
{
    if (NULL == m_test) {
        m_test = VR_new VR_MusicFileBuilder();
    }
}

VOID VR_MusicFileBuilder_Test::TearDown()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

TEST_F(VR_MusicFileBuilder_Test, AppendAgentNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // record num more than maxCount[500]
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicExcList;
    EXPECT_FALSE(m_test->AppendAgentNode(1, 501, fileType));

    // write file start failed

    // write music exc header info failed
}

TEST_F(VR_MusicFileBuilder_Test, AppendCategoryNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // table name is null
    NCCHAR* tableName = NULL;
    EXPECT_FALSE(m_test->AppendCategoryNode(tableName));

    // table name more than maxCount[32]
    tableName = "Test TableName MoreThan 32Byte Length";
    EXPECT_FALSE(m_test->AppendCategoryNode(tableName));
}

TEST_F(VR_MusicFileBuilder_Test, AppendBasicNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // cName more than maxCount[256]
    NCContentValues values;
    values.put("nId", "5");
    values.put("cName", "Basic Node cName Field Length More Than 256 Bytes." \
               "Fill Invalid Char:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
    values.put("nSourceId", "1");
    values.put("nExternalId", "5");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_FALSE(m_test->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));

    // cPronunciation more than maxCount[256]
    values.clear();
    values.put("nId", "5");
    values.put("cName", "lemon tree");
    values.put("nSourceId", "1");
    values.put("nExternalId", "5");
    values.put("cPronunciation", "Basic Node cPronunciation Field Length More Than" \
               "256 Byte. Fill Invalid Char:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
    values.put("nPronunciationId", "-1");
    EXPECT_FALSE(m_test->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));

    // write failed
    values.clear();
    values.put("nId", "5");
    values.put("cName", "lemon tree");
    values.put("nSourceId", "1");
    values.put("nExternalId", "5");
    values.put("cPronunciation", "unkown");
    values.put("nPronunciationId", "-1");

    // write table name error. not enough storage space
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicContentsDB;
    m_test->m_tableName = "MusicAgentSongs";
    UINT32 recordLen = sizeof(WORD) + sizeof(BYTE) + sizeof(BYTE) + 15; // 19
    recordLen += sizeof(UINT32) + sizeof(WORD) + 10 + sizeof(UINT32);   // 20
    recordLen += sizeof(UINT32) + sizeof(WORD) + 6 + sizeof(UINT32);    // 16
    EXPECT_TRUE(m_test->m_dpListDataAccess.WriteFileStart(fileType, recordLen));

    // write table name error. not enough storage space
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -19 + 1);
    EXPECT_FALSE(m_test->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));

    // write cName error. not enough storage space
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -35 + 1);
    EXPECT_FALSE(m_test->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));

    // write cPronunciationLen error. not enough storage space
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -51 + 1);
    EXPECT_FALSE(m_test->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));
}

TEST_F(VR_MusicFileBuilder_Test, AppendLinkNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // table name is unkown linkTable
    NCContentValues values;
    values.put("nAlbumId", "5");
    values.put("nSongId", "5");
    values.put("nSourceId", "1");
    m_test->m_tableName = "MusicAgentUnkownTable";
    EXPECT_FALSE(m_test->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));

    // write table name error. not enough storage space
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicContentsDB;
    m_test->m_tableName = "MusicAgentAlbumsSongs";
    UINT32 recordLen = sizeof(WORD) + sizeof(BYTE) + sizeof(BYTE) + 21; // 25
    recordLen += sizeof(UINT32) + sizeof(UINT32) + sizeof(UINT32);      // 12
    EXPECT_TRUE(m_test->m_dpListDataAccess.WriteFileStart(fileType, recordLen));

    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -25 + 1);
    EXPECT_FALSE(m_test->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));

    // field count invalid
    values.put("unkown", "unkown");
    EXPECT_FALSE(m_test->AppendLinkNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));
}

TEST_F(VR_MusicFileBuilder_Test, GetDiffFileException)
{
    ASSERT_NE(0, (INT64)m_test);
    BL_Dir avcLanDir(BL_FILE_PREFIX_RW);
    avcLanDir.RemoveDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME);

    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicContentsDB;
    EXPECT_TRUE(m_test->m_dpListDataAccess.WriteFileStart(fileType, 37));

    // write file end failed. path is not exist
    VR_String diffFile;
    EXPECT_FALSE(m_test->GetDiffFile(diffFile));
}

TEST_F(VR_MusicFileBuilder_Test, ParserAgentNodeException)
{
    ASSERT_NE(0, (INT64)m_test);
    BL_Dir avcLanDir(BL_FILE_PREFIX_RW);
    avcLanDir.RemoveDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME);

    // data file path not exist => ReadFileStart open error
    INT32 recordNum = 0;
    bool IsContinue = false;
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicContentsDB;
    EXPECT_FALSE(m_test->ParserAgentNode(fileType, recordNum, IsContinue));
    avcLanDir.MakeDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME, CL_FALSE);

    // build null data file => ReadFileStart file size error
    UINT32 writedSize = 0;
    sAvc_MusicExcHeader musicExcHeader;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(musicExcHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&musicExcHeader, sizeof(musicExcHeader) + 1, writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_FALSE(m_test->ParserAgentNode(fileType, recordNum, IsContinue));

    // build header length error data file => ReadBuffer error
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(musicExcHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&musicExcHeader, sizeof(musicExcHeader) - 1, writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_FALSE(m_test->ParserAgentNode(fileType, recordNum, IsContinue));

    // build header content error data file => typecode error
    musicExcHeader.m_bTypeCode = VR_AVC_LISTDATA_PHONEBOOK_TYPECODE;
    musicExcHeader.m_wRecordNum = 4;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(musicExcHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&musicExcHeader, sizeof(musicExcHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_FALSE(m_test->ParserAgentNode(fileType, recordNum, IsContinue));

    // build header content error data file => record num more than 500
    musicExcHeader.m_bTypeCode = VR_AVC_LISTDATA_MUSIC_EXC_TYPECODE;
    musicExcHeader.m_wRecordNum = 501;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(musicExcHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&musicExcHeader, sizeof(musicExcHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_FALSE(m_test->ParserAgentNode(fileType, recordNum, IsContinue));

    // build header content error data file => record num = 0
    musicExcHeader.m_bTypeCode = VR_AVC_LISTDATA_MUSIC_EXC_TYPECODE;
    musicExcHeader.m_wRecordNum = 0;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(musicExcHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&musicExcHeader, sizeof(musicExcHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_TRUE(m_test->ParserAgentNode(fileType, recordNum, IsContinue));
}

TEST_F(VR_MusicFileBuilder_Test, ParserRecordNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    UINT32 writedSize = 0;
    INT32 type = 0;
    bool IsContinue = false;
    NCContentValues values;
    NCCHAR tableName[VR_AVC_MUSIC_EXC_TABLE_NAME_MAX_LEN + 4] = { 0 };

    // param tableName is null
    EXPECT_FALSE(m_test->ParserRecordNode(type, NULL, values, IsContinue));

    // ready basic record memory data
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicContentsDB;
    UINT32 recordLen = sizeof(WORD) + sizeof(BYTE) + sizeof(BYTE) + 15; // 19
    recordLen += sizeof(UINT32) + sizeof(WORD) + 10 + sizeof(UINT32);   // 20
    recordLen += sizeof(UINT32) + sizeof(WORD) + 6 + sizeof(UINT32);    // 16
    EXPECT_TRUE(m_test->m_dpListDataAccess.WriteFileStart(fileType, recordLen + 2));

    // basic record length more than 568
    m_test->m_dpListDataAccess.WriteWord((WORD)600);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);
    EXPECT_FALSE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // basic record table name length more than 32
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)36);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);
    EXPECT_FALSE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // basic record table name read buffer failed
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -19 + 1);
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)15);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -19 + 1);
    EXPECT_FALSE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // basic record cName length more than 256
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)15);
    m_test->m_dpListDataAccess.WriteBuffer("MusicAgentSongs", 15, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteWord((WORD)300);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);
    EXPECT_FALSE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // basic record cName read buffer failed
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -35 + 1);
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)15);
    m_test->m_dpListDataAccess.WriteBuffer("MusicAgentSongs", 15, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteWord((WORD)10);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -35 + 1);
    EXPECT_FALSE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // basic record cPronunciation length more than 256
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)15);
    m_test->m_dpListDataAccess.WriteBuffer("MusicAgentSongs", 15, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteWord((WORD)10);
    m_test->m_dpListDataAccess.WriteBuffer("lemon tree", 10, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(1);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteWord((WORD)300);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);
    EXPECT_FALSE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // basic record cPronunciation read buffer failed
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -51 + 1);
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)15);
    m_test->m_dpListDataAccess.WriteBuffer("MusicAgentSongs", 15, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteWord((WORD)10);
    m_test->m_dpListDataAccess.WriteBuffer("lemon tree", 10, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(1);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteWord((WORD)6);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_End, -51 + 1);
    EXPECT_FALSE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // basic record next record len field is 0
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)15);
    m_test->m_dpListDataAccess.WriteBuffer("MusicAgentSongs", 15, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteWord((WORD)10);
    m_test->m_dpListDataAccess.WriteBuffer("lemon tree", 10, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(1);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteWord((WORD)6);
    m_test->m_dpListDataAccess.WriteBuffer("unkown", 6, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(-1);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);
    EXPECT_TRUE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // ready link record memory data
    recordLen = sizeof(WORD) + sizeof(BYTE) + sizeof(BYTE) + 21;        // 25
    recordLen += sizeof(UINT32) + sizeof(UINT32) + sizeof(UINT32);      // 12
    EXPECT_TRUE(m_test->m_dpListDataAccess.WriteFileStart(fileType, recordLen + 2));

    // link record table name unkown
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)21);
    m_test->m_dpListDataAccess.WriteBuffer("MusicAgentUnkownTable", 21, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteDWord(1);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);
    EXPECT_FALSE(m_test->ParserRecordNode(type, tableName, values, IsContinue));
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);

    // link record table name kown
    m_test->m_dpListDataAccess.WriteWord((WORD)recordLen);
    m_test->m_dpListDataAccess.WriteByte((BYTE)VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL);
    m_test->m_dpListDataAccess.WriteByte((BYTE)21);
    m_test->m_dpListDataAccess.WriteBuffer("MusicAgentAlbumsSongs", 21, writedSize);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteDWord(5);
    m_test->m_dpListDataAccess.WriteDWord(1);
    m_test->m_dpListDataAccess.Seek(VR_DP_ListData_Access::eSeekType_Start, 0);
    m_test->ParserRecordNode(type, tableName, values, IsContinue);
}

TEST_F(VR_MusicFileBuilder_Test, BuildInitFileException)
{
    ASSERT_NE(0, (INT64)m_test);
    BL_Dir avcLanDir(BL_FILE_PREFIX_RW);
    avcLanDir.RemoveDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME);

    MusicAllDeviceStatus allDeviceStatus;
    memset(&allDeviceStatus, 0, sizeof(allDeviceStatus));
    allDeviceStatus.sourceNum = 4;
    allDeviceStatus.sourceData[0].m_dwSourceId = 1;
    allDeviceStatus.sourceData[0].m_dwSourceType =
            VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_USBA;
    allDeviceStatus.sourceData[1].m_dwSourceId = 2;
    allDeviceStatus.sourceData[1].m_dwSourceType =
            VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_USBA;
    allDeviceStatus.sourceData[2].m_dwSourceId = 3;
    allDeviceStatus.sourceData[2].m_dwSourceType =
            VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_IPOD;
    allDeviceStatus.sourceData[3].m_dwSourceId = 4;
    allDeviceStatus.sourceData[3].m_dwSourceType =
            VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_IPOD;

    allDeviceStatus.ipodInfo[0].m_dwSourceId = 3;
    allDeviceStatus.ipodInfo[0].m_bIPodNameLen = 10;
    strcpy(allDeviceStatus.ipodInfo[0].m_bzIPodName, "Iap1VRLib1");
    allDeviceStatus.ipodInfo[1].m_dwSourceId = 4;
    allDeviceStatus.ipodInfo[1].m_bIPodNameLen = 45;
    strcpy(allDeviceStatus.ipodInfo[1].m_bzIPodName,
            "9C21EE57-0FAC-45D5-AB38-4003403C12D6-7.1.2");

    sAvc_MusicIncUnit musicIncUnit;
    musicIncUnit.m_dwActiveDevice = 1;
    musicIncUnit.m_dwSourceNum = allDeviceStatus.sourceNum;
    memcpy(musicIncUnit.m_stSourceData, allDeviceStatus.sourceData,
           sizeof(allDeviceStatus.sourceData));
    memcpy(musicIncUnit.m_stIPodInfo, allDeviceStatus.ipodInfo,
           sizeof(allDeviceStatus.ipodInfo));

    CHAR tempDataVersion[VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN + 1];
    memset(tempDataVersion, 0x00, sizeof(tempDataVersion));

    snprintf(tempDataVersion, sizeof(tempDataVersion), "%08d%08d",
             4, 357);
    memcpy(musicIncUnit.m_stVersion.m_bzCurDataVersion, tempDataVersion,
           VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN);
    musicIncUnit.m_stVersion.m_bOldDataVersionLen = strlen(tempDataVersion);

    snprintf(tempDataVersion, sizeof(tempDataVersion), "%08d%08d",
             1, 9693);
    memcpy(musicIncUnit.m_stVersion.m_bzOldDataVersion, tempDataVersion,
           VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN);
    musicIncUnit.m_stVersion.m_bOldDataVersionLen = strlen(tempDataVersion);

    musicIncUnit.m_stActiveUpdateUnit.m_bIsActive = 0x01;
    musicIncUnit.m_stActiveUpdateUnit.m_bGrammarDiffUpdate = 0x00;

    // write file end failed. path is not exist
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicCatalogDB;
    VR_String fullFile;
    EXPECT_FALSE(m_test->BuildInitFile(fileType, musicIncUnit, fullFile));
}

TEST_F(VR_MusicFileBuilder_Test, ParserInitFileException)
{
    ASSERT_NE(0, (INT64)m_test);
    BL_Dir avcLanDir(BL_FILE_PREFIX_RW);
    avcLanDir.RemoveDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME);

    // data file path not exist => ReadFileStart open error
    UINT32 writedSize = 0;
    sAvc_MusicIncHeader incHeader;
    sAvc_MusicIncUnit   incUnit;
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicCatalogDB;
    EXPECT_FALSE(m_test->ParserInitFile(fileType, incUnit));
    avcLanDir.MakeDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME, CL_FALSE);

    // build null data file => ReadFileStart file size error
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(incHeader));
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_FALSE(m_test->ParserInitFile(fileType, incUnit));

    // build header length error data file => ReadBuffer error
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(incHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&incHeader, sizeof(incHeader) - 1, writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_FALSE(m_test->ParserInitFile(fileType, incUnit));

    // build header content error data file ==> typecode error
    incHeader.m_bTypeCode = VR_AVC_LISTDATA_PHONEBOOK_TYPECODE;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(incHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&incHeader, sizeof(incHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_FALSE(m_test->ParserInitFile(fileType, incUnit));

    // build unit length error data file => ReadBuffer error
    incHeader.m_bTypeCode = VR_AVC_LISTDATA_MUSIC_INC_TYPECODE;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(incHeader) + sizeof(incUnit));
    m_test->m_dpListDataAccess.WriteBuffer(&incHeader, sizeof(incHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    EXPECT_FALSE(m_test->ParserInitFile(fileType, incUnit));
}

TEST_F(VR_MusicFileBuilder_Test, AppendAgentNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicExcList;
    EXPECT_TRUE(m_testSuite->AppendAgentNode(1, 8, fileType));
}

TEST_F(VR_MusicFileBuilder_Test, AppendCategoryBasicNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCCHAR* tableName = "MusicAgentSongs";
    EXPECT_TRUE(m_testSuite->AppendCategoryNode(tableName));
}

TEST_F(VR_MusicFileBuilder_Test, AppendDelBasicNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("nId", "5");
    values.put("cName", "lemon tree");
    values.put("nSourceId", "1");
    values.put("nExternalId", "5");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_testSuite->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));

    values.clear();
    values.put("nId", "9");
    values.put("cName", "tomorrow");
    values.put("nSourceId", "1");
    values.put("nExternalId", "9");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_testSuite->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, values));
}

TEST_F(VR_MusicFileBuilder_Test, AppendAddBasicNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("nId", "7");
    values.put("cName", "yesterday once more");
    values.put("nSourceId", "1");
    values.put("nExternalId", "7");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_testSuite->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, values));

    values.clear();
    values.put("nId", "2");
    values.put("cName", "show");
    values.put("nSourceId", "1");
    values.put("nExternalId", "2");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_testSuite->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, values));
}

TEST_F(VR_MusicFileBuilder_Test, AppendCategoryLinkNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCCHAR* tableName = "MusicAgentAlbumsSongs";
    EXPECT_TRUE(m_testSuite->AppendCategoryNode(tableName));
}

TEST_F(VR_MusicFileBuilder_Test, AppendAddLinkNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("nAlbumId", "7");
    values.put("nSongId", "7");
    values.put("nSourceId", "1");
    EXPECT_TRUE(m_testSuite->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, values));

    values.clear();
    values.put("nAlbumId", "2");
    values.put("nSongId", "2");
    values.put("nSourceId", "1");
    EXPECT_TRUE(m_testSuite->AppendNode(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, values));
}

TEST_F(VR_MusicFileBuilder_Test, GetDiffFile)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    VR_String diffFile;
    EXPECT_TRUE(m_testSuite->GetDiffFile(diffFile));
}

TEST_F(VR_MusicFileBuilder_Test, ParserAgentNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    INT32 recordNum = 0;
    bool IsContinue = false;
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicExcList;
    EXPECT_TRUE(m_testSuite->ParserAgentNode(fileType, recordNum, IsContinue));
    EXPECT_EQ(8, recordNum);
    EXPECT_TRUE(IsContinue);
}

TEST_F(VR_MusicFileBuilder_Test, ParserRecordNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    INT32 type;
    bool IsContinue;
    NCContentValues values;
    NCCHAR tableName[VR_AVC_MUSIC_EXC_TABLE_NAME_MAX_LEN + 4];

    type = 0;
    IsContinue = false;
    values.clear();
    memset(tableName, 0, sizeof(tableName));
    EXPECT_TRUE(m_testSuite->ParserRecordNode(type, tableName, values, IsContinue));
    EXPECT_EQ(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, type);
    EXPECT_STREQ("MusicAgentSongs", tableName);
    EXPECT_STREQ("5", values.getStringValue("nId"));
    EXPECT_STREQ("lemon tree", values.getStringValue("cName"));
    EXPECT_STREQ("1", values.getStringValue("nSourceId"));
    EXPECT_STREQ("5", values.getStringValue("nExternalId"));
    EXPECT_STREQ("", values.getStringValue("cPronunciation"));
    EXPECT_STREQ("-1", values.getStringValue("nPronunciationId"));
    EXPECT_TRUE(IsContinue);

    type = 0;
    IsContinue = false;
    values.clear();
    memset(tableName, 0, sizeof(tableName));
    EXPECT_TRUE(m_testSuite->ParserRecordNode(type, tableName, values, IsContinue));
    EXPECT_EQ(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_DEL, type);
    EXPECT_STREQ("MusicAgentSongs", tableName);
    EXPECT_STREQ("9", values.getStringValue("nId"));
    EXPECT_STREQ("tomorrow", values.getStringValue("cName"));
    EXPECT_STREQ("1", values.getStringValue("nSourceId"));
    EXPECT_STREQ("9", values.getStringValue("nExternalId"));
    EXPECT_STREQ("", values.getStringValue("cPronunciation"));
    EXPECT_STREQ("-1", values.getStringValue("nPronunciationId"));
    EXPECT_TRUE(IsContinue);

    type = 0;
    IsContinue = false;
    values.clear();
    memset(tableName, 0, sizeof(tableName));
    EXPECT_TRUE(m_testSuite->ParserRecordNode(type, tableName, values, IsContinue));
    EXPECT_EQ(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, type);
    EXPECT_STREQ("MusicAgentSongs", tableName);
    EXPECT_STREQ("7", values.getStringValue("nId"));
    EXPECT_STREQ("yesterday once more", values.getStringValue("cName"));
    EXPECT_STREQ("1", values.getStringValue("nSourceId"));
    EXPECT_STREQ("7", values.getStringValue("nExternalId"));
    EXPECT_STREQ("", values.getStringValue("cPronunciation"));
    EXPECT_STREQ("-1", values.getStringValue("nPronunciationId"));
    EXPECT_TRUE(IsContinue);

    type = 0;
    IsContinue = false;
    values.clear();
    memset(tableName, 0, sizeof(tableName));
    EXPECT_TRUE(m_testSuite->ParserRecordNode(type, tableName, values, IsContinue));
    EXPECT_EQ(VR_AVC_MUSIC_EXC_DIFF_TYPE_OF_ADD, type);
    EXPECT_STREQ("MusicAgentSongs", tableName);
    EXPECT_STREQ("2", values.getStringValue("nId"));
    EXPECT_STREQ("show", values.getStringValue("cName"));
    EXPECT_STREQ("1", values.getStringValue("nSourceId"));
    EXPECT_STREQ("2", values.getStringValue("nExternalId"));
    EXPECT_STREQ("", values.getStringValue("cPronunciation"));
    EXPECT_STREQ("-1", values.getStringValue("nPronunciationId"));
    EXPECT_TRUE(IsContinue);
}

TEST_F(VR_MusicFileBuilder_Test, BuildInitFile)
{
    ASSERT_NE(0, (INT64)m_test);

    MusicAllDeviceStatus allDeviceStatus;
    memset(&allDeviceStatus, 0, sizeof(allDeviceStatus));
    allDeviceStatus.sourceNum = 4;
    allDeviceStatus.sourceData[0].m_dwSourceId = 1;
    allDeviceStatus.sourceData[0].m_dwSourceType =
            VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_USBA;
    allDeviceStatus.sourceData[1].m_dwSourceId = 2;
    allDeviceStatus.sourceData[1].m_dwSourceType =
            VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_USBA;
    allDeviceStatus.sourceData[2].m_dwSourceId = 3;
    allDeviceStatus.sourceData[2].m_dwSourceType =
            VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_IPOD;
    allDeviceStatus.sourceData[3].m_dwSourceId = 4;
    allDeviceStatus.sourceData[3].m_dwSourceType =
            VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_IPOD;

    allDeviceStatus.ipodInfo[0].m_dwSourceId = 3;
    allDeviceStatus.ipodInfo[0].m_bIPodNameLen = 10;
    strcpy(allDeviceStatus.ipodInfo[0].m_bzIPodName, "Iap1VRLib1");
    allDeviceStatus.ipodInfo[1].m_dwSourceId = 4;
    allDeviceStatus.ipodInfo[1].m_bIPodNameLen = 45;
    strcpy(allDeviceStatus.ipodInfo[1].m_bzIPodName,
            "9C21EE57-0FAC-45D5-AB38-4003403C12D6-7.1.2");

    sAvc_MusicIncUnit musicIncUnit;
    musicIncUnit.m_dwActiveDevice = 1;
    musicIncUnit.m_dwSourceNum = allDeviceStatus.sourceNum;
    memcpy(musicIncUnit.m_stSourceData, allDeviceStatus.sourceData,
           sizeof(allDeviceStatus.sourceData));
    memcpy(musicIncUnit.m_stIPodInfo, allDeviceStatus.ipodInfo,
           sizeof(allDeviceStatus.ipodInfo));

    CHAR tempDataVersion[VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN + 1];
    memset(tempDataVersion, 0x00, sizeof(tempDataVersion));

    snprintf(tempDataVersion, sizeof(tempDataVersion), "%08d%08d",
             4, 357);
    memcpy(musicIncUnit.m_stVersion.m_bzCurDataVersion, tempDataVersion,
           VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN);
    musicIncUnit.m_stVersion.m_bOldDataVersionLen = strlen(tempDataVersion);

    snprintf(tempDataVersion, sizeof(tempDataVersion), "%08d%08d",
             1, 9693);
    memcpy(musicIncUnit.m_stVersion.m_bzOldDataVersion, tempDataVersion,
           VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN);
    musicIncUnit.m_stVersion.m_bOldDataVersionLen = strlen(tempDataVersion);

    musicIncUnit.m_stActiveUpdateUnit.m_bIsActive = 0x01;
    musicIncUnit.m_stActiveUpdateUnit.m_bGrammarDiffUpdate = 0x00;

    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicIncList;
    VR_String fullFile;
    m_test->BuildInitFile(fileType, musicIncUnit, fullFile);
}

TEST_F(VR_MusicFileBuilder_Test, ParserInitFile)
{
    ASSERT_NE(0, (INT64)m_test);

    sAvc_MusicIncUnit musicIncUnit;
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_MusicIncList;
    EXPECT_TRUE(m_test->ParserInitFile(fileType, musicIncUnit));
    EXPECT_EQ(1, musicIncUnit.m_dwActiveDevice);
    EXPECT_EQ(4, musicIncUnit.m_dwSourceNum);

    EXPECT_EQ(1, musicIncUnit.m_stSourceData[0].m_dwSourceId);
    EXPECT_EQ(VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_USBA,
              musicIncUnit.m_stSourceData[0].m_dwSourceType);
    EXPECT_EQ(2, musicIncUnit.m_stSourceData[1].m_dwSourceId);
    EXPECT_EQ(VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_USBA,
              musicIncUnit.m_stSourceData[1].m_dwSourceType);
    EXPECT_EQ(3, musicIncUnit.m_stSourceData[2].m_dwSourceId);
    EXPECT_EQ(VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_IPOD,
              musicIncUnit.m_stSourceData[2].m_dwSourceType);
    EXPECT_EQ(4, musicIncUnit.m_stSourceData[3].m_dwSourceId);
    EXPECT_EQ(VR_SOURCE_DATA_UNIT_CONNECT_DEVICE_TYPE_OF_IPOD,
              musicIncUnit.m_stSourceData[3].m_dwSourceType);

    EXPECT_EQ(3, musicIncUnit.m_stIPodInfo[0].m_dwSourceId);
    EXPECT_EQ(10, musicIncUnit.m_stIPodInfo[0].m_bIPodNameLen);
    EXPECT_STREQ("Iap1VRLib1", musicIncUnit.m_stIPodInfo[0].m_bzIPodName);
    EXPECT_EQ(4, musicIncUnit.m_stIPodInfo[1].m_dwSourceId);
    EXPECT_EQ(45, musicIncUnit.m_stIPodInfo[1].m_bIPodNameLen);
    EXPECT_STREQ("9C21EE57-0FAC-45D5-AB38-4003403C12D6-7.1.2",
                 musicIncUnit.m_stIPodInfo[1].m_bzIPodName);

    CHAR tempDataVersion[VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN + 1];
    memset(tempDataVersion, 0x00, sizeof(tempDataVersion));

    memcpy(tempDataVersion, musicIncUnit.m_stVersion.m_bzCurDataVersion,
           VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN);
    EXPECT_STREQ("0000000400000357", tempDataVersion);
    memcpy(tempDataVersion, musicIncUnit.m_stVersion.m_bzOldDataVersion,
           VR_AVC_MUSIC_INC_DATA_VERSION_MAX_LEN);
    EXPECT_STREQ("0000000100009693", tempDataVersion);

    EXPECT_EQ(1, musicIncUnit.m_stActiveUpdateUnit.m_bIsActive);
    EXPECT_EQ(0, musicIncUnit.m_stActiveUpdateUnit.m_bGrammarDiffUpdate);
}

}
}
/* EOF */
