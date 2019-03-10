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
 * @file VR_CityListFileBuilder_Test.h
 * @brief Declaration file of class VR_CityListFileBuilder_Test.
 *
 * This file includes the declaration of class VR_CityListFileBuilder_Test.
 *
 * @attention used for C++ only.
 */

#include "VR_CityListFileBuilder.h"
#include "gtest/gtest.h"
#include "ncore/NCFile.h"

using namespace nutshell;

namespace navi
{
namespace dataprovider
{
/**
 * @brief The VR_CityListFileBuilder_Test class
 *
 * class declaration.
 */
class VR_CityListFileBuilder_Test : public testing::Test
{
public:
    VR_CityListFileBuilder_Test();
    ~VR_CityListFileBuilder_Test();
    VOID SetUp();
    VOID TearDown();

private:
    VR_CityListFileBuilder* m_test;

private:
    VR_CityListFileBuilder_Test(const VR_CityListFileBuilder_Test&);
    VR_CityListFileBuilder_Test& operator=(const VR_CityListFileBuilder_Test&);
};

VR_CityListFileBuilder_Test::VR_CityListFileBuilder_Test()
{
    m_test = NULL;
}

VR_CityListFileBuilder_Test::~VR_CityListFileBuilder_Test()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

VOID VR_CityListFileBuilder_Test::SetUp()
{
    if (NULL == m_test) {
        m_test = VR_new VR_CityListFileBuilder();
    }
}

VOID VR_CityListFileBuilder_Test::TearDown()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

TEST_F(VR_CityListFileBuilder_Test, AppendCategoryNode)
{
    ASSERT_NE(0, (INT64)m_test);

    UINT number = 25;
    sAvc_CityListDataHeader  cListHeader;
    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
}

TEST_F(VR_CityListFileBuilder_Test, AppendCategoryNode_1)
{
    ASSERT_NE(0, (INT64)m_test);

    UINT number = 1000000;
    sAvc_CityListDataHeader  cListHeader;
    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
}

TEST_F(VR_CityListFileBuilder_Test, AppendCityListNode_false)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_CityListDataUnit appListUnit;
    appListUnit.m_dCityNameLen = 5;
    m_test->m_dpListDataAccess.m_pszFileData = NULL;

    EXPECT_FALSE(m_test->AppendCityListNode(appListUnit));
}

TEST_F(VR_CityListFileBuilder_Test, AppendCityListNode)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_CityListDataUnit appListUnit;
    appListUnit.m_dCityNameLen = 5;
    UINT number = 25;
    sAvc_CityListDataHeader  cListHeader;

    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
    EXPECT_TRUE(m_test->AppendCityListNode(appListUnit));
}

TEST_F(VR_CityListFileBuilder_Test, AppendCityListNode_1)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_CityListDataUnit appListUnit;

    EXPECT_TRUE(m_test->AppendCityListNode(appListUnit));
}

TEST_F(VR_CityListFileBuilder_Test, GetCityListFile)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_CityListDataUnit appListUnit;
    appListUnit.m_dCityNameLen = 5;
    UINT number = 25;
    sAvc_CityListDataHeader  cListHeader;

    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
    EXPECT_TRUE(m_test->AppendCityListNode(appListUnit));

    VR_String pbFile;
    m_test->GetCityListFile(pbFile);
}

TEST_F(VR_CityListFileBuilder_Test, ParserCategoryNode)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_CityListDataUnit appListUnit;
    appListUnit.m_dCityNameLen = 5;
    UINT number = 25;
    sAvc_CityListDataHeader  cListHeader;

    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
    EXPECT_TRUE(m_test->AppendCityListNode(appListUnit));

    INT recordNum = 0;

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access:: eFileType_CityList;
    BL_String destPath = VR_DP_ListData_Access::GetAvcLanRootPath()
            + "/" + VR_DP_ListData_Access::GetFileRelativePath(fileType);
    bool result = m_test->ParserCategoryNode(recordNum, cListHeader);
    if (!result) {
        printf("[ RUN      ] pb file not exists %s\n",
               destPath.GetString());
        SUCCEED();
        return;
    }

    EXPECT_EQ(25, recordNum);
}

TEST_F(VR_CityListFileBuilder_Test, ParserCategoryNode_1)
{
    int listNum = 0;
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_CityList;
    sAvc_CityListDataHeader sListHeader;
    UINT32 writedSize = 0;
    // ReadFileStart open error
    BL_Dir avcLanDir(BL_FILE_PREFIX_RW);
    avcLanDir.RemoveDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME);
    m_test->ParserCategoryNode(listNum, sListHeader);
    VR_String filePath;
    m_test->GetCityListFile(filePath);
    avcLanDir.MakeDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME, CL_FALSE);
    // ReadBuffer error
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sListHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sListHeader, sizeof(sListHeader) - 1, writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(listNum, sListHeader);
    // typecode error
    sListHeader.m_bTypeCode = VR_AVC_LISTDATA_PHONEBOOK_TYPECODE;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sListHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sListHeader, sizeof(sListHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(listNum, sListHeader);
    // MAX_NUM
    sListHeader.m_bTypeCode = VR_AVC_LISTDATA_CITY_LIST_TYPECODE;
    sListHeader.m_wRecordNum = VR_AVC_CITY_LIST_MAX_COUNT + 1;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sListHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sListHeader, sizeof(sListHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(listNum, sListHeader);
    // listNum == 0
    sListHeader.m_wRecordNum = 0;
    m_test->m_dpListDataAccess.WriteFileStart(fileType, sizeof(sListHeader));
    m_test->m_dpListDataAccess.WriteBuffer(&sListHeader, sizeof(sListHeader), writedSize);
    m_test->m_dpListDataAccess.WriteFileEnd();
    m_test->ParserCategoryNode(listNum, sListHeader);
}

TEST_F(VR_CityListFileBuilder_Test, ParserCityListNode)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_CityListDataUnit appListUnit;
    appListUnit.m_dCityNameLen = 5;
    UINT number = 25;
    sAvc_CityListDataHeader  cListHeader;

    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
    EXPECT_TRUE(m_test->AppendCityListNode(appListUnit));

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access:: eFileType_CityList;
    BL_String destPath = VR_DP_ListData_Access::GetAvcLanRootPath()
            + "/" + VR_DP_ListData_Access::GetFileRelativePath(fileType);
    bool result = m_test->ParserCityListNode(appListUnit);
    if (!result) {
        printf("[ RUN      ] pb file not exists %s\n",
               destPath.GetString());
        SUCCEED();
        return;
    }
    EXPECT_TRUE(result);
}

}
}
/* EOF */
