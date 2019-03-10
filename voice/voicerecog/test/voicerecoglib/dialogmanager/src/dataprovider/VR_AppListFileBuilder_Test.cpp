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
 * @file VR_AppListFileBuilder_Test.h
 * @brief Declaration file of class VR_AppListFileBuilder_Test.
 *
 * This file includes the declaration of class VR_AppListFileBuilder_Test.
 *
 * @attention used for C++ only.
 */

#include "VR_AppListFileBuilder.h"
#include "gtest/gtest.h"
#include "ncore/NCFile.h"

using namespace nutshell;

namespace navi
{
namespace dataprovider
{
/**
 * @brief The VR_AppListFileBuilder_Test class
 *
 * class declaration.
 */
class VR_AppListFileBuilder_Test : public testing::Test
{
public:
    VR_AppListFileBuilder_Test();
    ~VR_AppListFileBuilder_Test();
    VOID SetUp();
    VOID TearDown();

private:
    VR_AppListFileBuilder* m_test;

private:
    VR_AppListFileBuilder_Test(const VR_AppListFileBuilder_Test&);
    VR_AppListFileBuilder_Test& operator=(const VR_AppListFileBuilder_Test&);
};

VR_AppListFileBuilder_Test::VR_AppListFileBuilder_Test()
{
    m_test = NULL;
}

VR_AppListFileBuilder_Test::~VR_AppListFileBuilder_Test()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

VOID VR_AppListFileBuilder_Test::SetUp()
{
    if (NULL == m_test) {
        m_test = VR_new VR_AppListFileBuilder();
    }
}

VOID VR_AppListFileBuilder_Test::TearDown()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

TEST_F(VR_AppListFileBuilder_Test, AppendCategoryNode)
{
    ASSERT_NE(0, (INT64)m_test);

    UINT number = 25;
    sAvc_AppListDataHeader cListHeader;
    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
}

TEST_F(VR_AppListFileBuilder_Test, AppendCategoryNode_1)
{
    ASSERT_NE(0, (INT64)m_test);

    UINT number = 1000000;
    sAvc_AppListDataHeader cListHeader;
    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
}

TEST_F(VR_AppListFileBuilder_Test, AppendAppListNode_false)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_AppListDataUnit appListUnit;
    memset(&appListUnit, sizeof(appListUnit), 0);
    memcpy(&appListUnit.m_cManiFest, "123456789", 9);
    appListUnit.m_dManiFestLen = 9;

    EXPECT_FALSE(m_test->AppendAppListNode(appListUnit));
}

TEST_F(VR_AppListFileBuilder_Test, AppendAppListNode_1)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_AppListDataUnit appListUnit;
    appListUnit.m_dManiFestLen = 5;
    m_test->m_dpListDataAccess.m_pszFileData = NULL;
    EXPECT_FALSE(m_test->AppendAppListNode(appListUnit));
}

TEST_F(VR_AppListFileBuilder_Test, AppendAppListNode)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_AppListDataUnit appListUnit;
    UINT number = 25;
    sAvc_AppListDataHeader cListHeader;
    appListUnit.m_dManiFestLen = 5;

    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
    EXPECT_TRUE(m_test->AppendAppListNode(appListUnit));
}

TEST_F(VR_AppListFileBuilder_Test, GetAppListFile)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_AppListDataUnit appListUnit;
    UINT number = 25;
    sAvc_AppListDataHeader cListHeader;
    appListUnit.m_dManiFestLen = 5;

    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
    EXPECT_TRUE(m_test->AppendAppListNode(appListUnit));

    VR_String pbFile;
    m_test->GetAppListFile(pbFile);
}

TEST_F(VR_AppListFileBuilder_Test, ParserCategoryNode)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_AppListDataUnit appListUnit;
    UINT number = 25;
    sAvc_AppListDataHeader cListHeader;
    appListUnit.m_dManiFestLen = 5;

    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
    EXPECT_TRUE(m_test->AppendAppListNode(appListUnit));

    INT recordNum = 0;

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_AppList;
    BL_String destPath = VR_DP_ListData_Access::GetAvcLanRootPath()
            + "/" + VR_DP_ListData_Access::GetFileRelativePath(fileType);
    bool result = m_test->ParserCategoryNode(recordNum, cListHeader);
    if (!result) {
        printf("[ RUN      ] pb file not exists %s\n",
               destPath.GetString());
        SUCCEED();
        return;
    }
}

TEST_F(VR_AppListFileBuilder_Test, ParserCategoryNode_1)
{
    int listNum = 0;
    VR_DP_ListData_Access::eFileType fileType =
            VR_DP_ListData_Access::eFileType_AppList;
    sAvc_AppListDataHeader sListHeader;
    UINT32 writedSize = 0;
    // ReadFileStart open error
    BL_Dir avcLanDir(BL_FILE_PREFIX_RW);
    avcLanDir.RemoveDir(VR_AVC_LISTDATA_FILE_RWDISK_FOLDERNAME);
    m_test->ParserCategoryNode(listNum, sListHeader);
    VR_String filePath;
    m_test->GetAppListFile(filePath);
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
    sListHeader.m_bTypeCode = VR_AVC_LISTDATA_APPS_TYPECODE;
    sListHeader.m_wRecordNum = VR_AVC_APP_LIST_MAX_COUNT + 1;
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

TEST_F(VR_AppListFileBuilder_Test, ParserAppListNode)
{
    ASSERT_NE(0, (INT64)m_test);
    sAvc_AppListDataUnit appListUnit;
    UINT number = 25;
    sAvc_AppListDataHeader cListHeader;
    appListUnit.m_dManiFestLen = 5;

    EXPECT_TRUE(m_test->AppendCategoryNode(number, cListHeader));
    EXPECT_TRUE(m_test->AppendAppListNode(appListUnit));

    VR_DP_ListData_Access::eFileType fileType = VR_DP_ListData_Access::eFileType_AppList;
    BL_String destPath = VR_DP_ListData_Access::GetAvcLanRootPath()
            + "/" + VR_DP_ListData_Access::GetFileRelativePath(fileType);
    bool result = m_test->ParserAppListNode(appListUnit);
    if (!result) {
        printf("[ RUN      ] pb file not exists %s\n",
               destPath.GetString());
        SUCCEED();
        return;
    }
}

}
}
/* EOF */
