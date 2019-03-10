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
 * @file VR_RadioFileBuilder_Test.h
 * @brief Declaration file of class VR_RadioFileBuilder_Test.
 *
 * This file includes the declaration of class VR_RadioFileBuilder_Test.
 *
 * @attention used for C++ only.
 */

#include "VR_RadioFileBuilder.h"
#include "gtest/gtest.h"
#include "ncore/NCFile.h"

using namespace nutshell;

namespace navi
{
namespace dataprovider
{
/**
 * @brief The VR_RadioFileBuilder test class
 *
 * class declaration.
 */
class VR_RadioFileBuilder_Test : public testing::Test
{
public:
    VR_RadioFileBuilder_Test();
    ~VR_RadioFileBuilder_Test();
    static VOID SetUpTestCase();
    static VOID TearDownTestCase();
    VOID SetUp();
    VOID TearDown();

private:
    VR_RadioFileBuilder* m_test;
    static VR_RadioFileBuilder* m_testSuite;

private:
    VR_RadioFileBuilder_Test(const VR_RadioFileBuilder_Test&);
    VR_RadioFileBuilder_Test& operator=(const VR_RadioFileBuilder_Test&);
};

VR_RadioFileBuilder* VR_RadioFileBuilder_Test::m_testSuite = NULL;

VOID VR_RadioFileBuilder_Test::SetUpTestCase()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_new VR_RadioFileBuilder();
    }
}

VOID VR_RadioFileBuilder_Test::TearDownTestCase()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
        m_testSuite = NULL;
    }
}

VR_RadioFileBuilder_Test::VR_RadioFileBuilder_Test()
{
    m_test = NULL;
}

VR_RadioFileBuilder_Test::~VR_RadioFileBuilder_Test()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

VOID VR_RadioFileBuilder_Test::SetUp()
{
    if (NULL == m_test) {
        m_test = VR_new VR_RadioFileBuilder();
    }
}

VOID VR_RadioFileBuilder_Test::TearDown()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

TEST_F(VR_RadioFileBuilder_Test, AppendCategoryNode_RECORD_MAX)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 451;
    nutshell::INT stationNum = 5;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_FALSE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
}

TEST_F(VR_RadioFileBuilder_Test, AppendCategoryNode_STATION_MAX)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 386;
    nutshell::INT stationNum = 386;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_FALSE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
}

TEST_F(VR_RadioFileBuilder_Test, AppendCategoryNode_GENRE_MAX)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 66;
    nutshell::INT stationNum = 0;
    nutshell::INT genreNum = 66;
    nutshell::INT radioType = 0;
    EXPECT_FALSE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
}

TEST_F(VR_RadioFileBuilder_Test, AppendCategoryNode_true)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 5;
    nutshell::INT stationNum = 5;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
}

TEST_F(VR_RadioFileBuilder_Test, AppendStationFormalNode_NAME_MAX_LEN)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 5;
    nutshell::INT stationNum = 5;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 69;
    NCString name("012345678901234567890123456789012345678901234567890123456789012345678");
    BYTE type = 0;

    EXPECT_FALSE(m_test->AppendStationFormalNode(Id, name, type));
}

TEST_F(VR_RadioFileBuilder_Test, AppendStationFormalNode_WriteDataFailed)
{
    ASSERT_NE(0, (INT64)m_test);
    INT Id = 7;
    NCString name("station");
    BYTE type = 0;

    EXPECT_FALSE(m_test->AppendStationFormalNode(Id, name, type));
}

TEST_F(VR_RadioFileBuilder_Test, AppendStationFormalNode_true)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 5;
    nutshell::INT stationNum = 5;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 0;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
}

TEST_F(VR_RadioFileBuilder_Test, GetDataFile)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 5;
    nutshell::INT stationNum = 5;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 0;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));

    VR_String pbFile;
    m_test->GetDataFile(pbFile);
}

TEST_F(VR_RadioFileBuilder_Test, ParserCategoryNode)
{
    ASSERT_NE(0, (INT64)m_test);
    bool IsCanContinue;

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 5;
    nutshell::INT stationNum = 5;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 0;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));

    BL_String destPath = VR_DP_ListData_Access::GetAvcLanRootPath()
            + "/" + VR_DP_ListData_Access::GetFileRelativePath(fileType);
    bool result = m_test->ParserCategoryNode(fileType, recordNum, stationNum, genreNum, radioType, IsCanContinue);
    if (!result) {
        printf("[ RUN      ] pb file not exists %s\n",
               destPath.GetString());
        SUCCEED();
        return;
    }

    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, ParserCategoryNode_1)
{
    int listNum = 0;
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_RadioStationList;
    sAvc_RadioHeader sRadioHeader;
    UINT32 writedSize = 0;
    nutshell::INT recordNum = 5;
    nutshell::INT stationNum = 5;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    bool IsCanContinue;
    // ReadFileStart open error
    BL_Dir avcLanDir(BL_FILE_PREFIX_RW);
    avcLanDir.RemoveDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME);
    m_test->ParserCategoryNode(fileType, recordNum, stationNum, genreNum, radioType, IsCanContinue);
    VR_String filePath;
    m_test->GetDataFile(filePath);
    avcLanDir.MakeDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME, CL_FALSE);
    // ReadBuffer error
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sRadioHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sRadioHeader, sizeof(sRadioHeader) - 1, writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(fileType, recordNum, stationNum, genreNum, radioType, IsCanContinue);
    // typecode error
    sRadioHeader.m_bTypeCode = VR_AVC_LISTDATA_PHONEBOOK_TYPECODE;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sRadioHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sRadioHeader, sizeof(sRadioHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(fileType, recordNum, stationNum, genreNum, radioType, IsCanContinue);
    // MAX_RECORD
    sRadioHeader.m_wRecordNum = VR_AVC_RADIO_RECORD_MAX_COUNT + 1;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sRadioHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sRadioHeader, sizeof(sRadioHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(fileType, recordNum, stationNum, genreNum, radioType, IsCanContinue);
    // MAX_STATION
    sRadioHeader.m_wStationNum = VR_AVC_RADIO_STATION_MAX_COUNT + 1;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sRadioHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sRadioHeader, sizeof(sRadioHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(fileType, recordNum, stationNum, genreNum, radioType, IsCanContinue);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    // MAX_GENRE
    sRadioHeader.m_wGenreNum = VR_AVC_RADIO_GENRE_MAX_COUNT + 1;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sRadioHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sRadioHeader, sizeof(sRadioHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(fileType, recordNum, stationNum, genreNum, radioType, IsCanContinue);
    // recordNum == 0
    sRadioHeader.m_bTypeCode = VR_AVC_LISTDATA_SERVICE_TYPECODE;
    sRadioHeader.m_wRecordNum = 0;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sRadioHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sRadioHeader, sizeof(sRadioHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(fileType, recordNum, stationNum, genreNum, radioType, IsCanContinue);
}

TEST_F(VR_RadioFileBuilder_Test, ParserStationFormalNode)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 5;
    nutshell::INT stationNum = 5;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 0;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));

    bool IsContinue;
    BL_String destPath = VR_DP_ListData_Access::GetAvcLanRootPath()
            + "/" + VR_DP_ListData_Access::GetFileRelativePath(fileType);
    bool result = m_test->ParserStationFormalNode(Id, name, type, IsContinue);
    if (!result) {
        printf("[ RUN      ] pb file not exists %s\n",
               destPath.GetString());
        SUCCEED();
        return;
    }
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_FM_Station)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 1;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 0;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_FM_Genre)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 0;
    nutshell::INT genreNum = 1;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 1;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_FM_other)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 0;
    nutshell::INT genreNum = 1;
    nutshell::INT radioType = 0;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 2;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_XM_Station)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 1;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 1;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 0;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_XM_Genre)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 0;
    nutshell::INT genreNum = 1;
    nutshell::INT radioType = 1;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 1;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_XM_other)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 0;
    nutshell::INT genreNum = 1;
    nutshell::INT radioType = 1;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 2;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_DAB_Station)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 1;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 2;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 0;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_DAB_genre)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 1;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 2;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 1;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

TEST_F(VR_RadioFileBuilder_Test, GetRadioType_Other)
{
    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_RadioStationList;
    nutshell::INT recordNum = 1;
    nutshell::INT stationNum = 1;
    nutshell::INT genreNum = 0;
    nutshell::INT radioType = 10;
    EXPECT_TRUE(m_test->AppendCategoryNode(fileType, recordNum, stationNum, genreNum, radioType));
    INT Id = 7;
    NCString name("station");
    BYTE type = 1;

    EXPECT_TRUE(m_test->AppendStationFormalNode(Id, name, type));
    VR_String pbFile;
    m_test->GetDataFile(pbFile);
    VR_INT32 fileType1 = 0;
    m_test->GetRadioType(fileType1);
    SUCCEED();
}

}
}
/* EOF */
