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
#include "VR_DP_ListData_Access_Test.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_DP_ListData_Access* VR_DP_ListData_Access_Test::m_testSuite = NULL;

VOID VR_DP_ListData_Access_Test::SetUpTestCase()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_new VR_DP_ListData_Access;
    }
}

VOID VR_DP_ListData_Access_Test::TearDownTestCase()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
        m_testSuite = NULL;
    }
}

VR_DP_ListData_Access_Test::VR_DP_ListData_Access_Test()
{
}

VR_DP_ListData_Access_Test::~VR_DP_ListData_Access_Test()
{
}

TEST_F(VR_DP_ListData_Access_Test, NULLDestruction)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    VR_DP_ListData_Access dpAccess;
    SUCCEED();
}

TEST_F(VR_DP_ListData_Access_Test, GetFileType)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    // file path unkown
    BL_String filePath = "VRListDataUnkown.dat";
    EXPECT_EQ(VR_DP_ListData_Access::eFileType_Invalid, m_testSuite->GetFileType(filePath));

    // exc file
    filePath = "/tmp/VRListDataMusicListWithoutCatalogDB.dat";
    EXPECT_EQ(VR_DP_ListData_Access::eFileType_MusicExcList, m_testSuite->GetFileType(filePath));
}

TEST_F(VR_DP_ListData_Access_Test, GetFileRelativePath)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    VR_DP_ListData_Access::eFileType fileType;
    BL_String fileListName;

    // file type invalid
    fileType = VR_DP_ListData_Access::eFileType_Invalid;
    fileListName = m_testSuite->GetFileRelativePath(fileType);
    EXPECT_STREQ("InvalidFileType", fileListName.GetString());

    fileType = VR_DP_ListData_Access::eFileType_MusicExcList;
    fileListName = m_testSuite->GetFileRelativePath(fileType);
    EXPECT_STREQ("VRListDataMusicListWithoutCatalogDB.dat", fileListName.GetString());
}

TEST_F(VR_DP_ListData_Access_Test, GetAvcLanRootPath)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    BL_String rootPath = m_testSuite->GetAvcLanRootPath();
    INT nPos = rootPath.Find("avclan");
    EXPECT_NE(-1, nPos);
}

TEST_F(VR_DP_ListData_Access_Test, WriteFileStartException)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    // write file size is 0
    INT fileSize = 0;
    VR_DP_ListData_Access::eFileType fileType;
    fileType = VR_DP_ListData_Access::eFileType_MusicExcList;
    EXPECT_TRUE(m_testSuite->WriteFileStart(fileType, fileSize));
}

TEST_F(VR_DP_ListData_Access_Test, WriteFileEndException)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    // file data is null
    BYTE* data = m_testSuite->m_pszFileData;
    m_testSuite->m_pszFileData = NULL;
    EXPECT_TRUE(m_testSuite->WriteFileEnd());
    m_testSuite->m_pszFileData = data;
}

TEST_F(VR_DP_ListData_Access_Test, Seek)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    INT dwStep;
    VR_DP_ListData_Access::eSeekType seekType;
    m_testSuite->m_dwCursor = 0;
    m_testSuite->m_dwFileSize = 50;

    // seek type is start. dwStep <= 0
    dwStep = -20;
    seekType = VR_DP_ListData_Access::eSeekType_Start;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));

    // seek type is start. dwStep >= m_dwFileSize
    dwStep = 70;
    seekType = VR_DP_ListData_Access::eSeekType_Start;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));

    // seek type is start.
    dwStep = 25;
    seekType = VR_DP_ListData_Access::eSeekType_Start;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));

    // seek type cur. cur + dwStep < 0
    dwStep = -30;
    seekType = VR_DP_ListData_Access::eSeekType_Cur;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));

    // seek type cur. cur + dwStep > 0
    dwStep = 70;
    seekType = VR_DP_ListData_Access::eSeekType_Cur;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));

    // seek type cur.
    dwStep = -25;
    seekType = VR_DP_ListData_Access::eSeekType_Cur;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));

    // seek type end. dwStep > 0
    dwStep = 30;
    seekType = VR_DP_ListData_Access::eSeekType_End;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));

    // seek type end. -dwStep >= m_dwFileSize
    dwStep = -70;
    seekType = VR_DP_ListData_Access::eSeekType_End;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));

    // seek type end.
    dwStep = -25;
    seekType = VR_DP_ListData_Access::eSeekType_End;
    EXPECT_TRUE(m_testSuite->Seek(seekType, dwStep));
}

TEST_F(VR_DP_ListData_Access_Test, WriteByte)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    BYTE data;

    // file data is null
    if (NULL != m_testSuite->m_pszFileData) {
        delete m_testSuite->m_pszFileData;
        m_testSuite->m_pszFileData = NULL;
    }
    EXPECT_FALSE(m_testSuite->WriteByte(data));

    // file data space not enough
    m_testSuite->m_pszFileData = VR_new BYTE[50];
    m_testSuite->m_dwFileSize = 50;
    m_testSuite->m_dwCursor = 50;
    EXPECT_FALSE(m_testSuite->WriteByte(data));
}

TEST_F(VR_DP_ListData_Access_Test, WriteWord)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    WORD data;

    // file data is null
    if (NULL != m_testSuite->m_pszFileData) {
        delete m_testSuite->m_pszFileData;
        m_testSuite->m_pszFileData = NULL;
    }
    EXPECT_FALSE(m_testSuite->WriteWord(data));

    // file data space not enough
    m_testSuite->m_pszFileData = VR_new BYTE[50];
    m_testSuite->m_dwFileSize = 50;
    m_testSuite->m_dwCursor = 50;
    EXPECT_FALSE(m_testSuite->WriteWord(data));
}

TEST_F(VR_DP_ListData_Access_Test, WriteDWord)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    UINT32 data;

    // file data is null
    if (NULL != m_testSuite->m_pszFileData) {
        delete m_testSuite->m_pszFileData;
        m_testSuite->m_pszFileData = NULL;
    }
    EXPECT_FALSE(m_testSuite->WriteDWord(data));

    // file data space not enough
    m_testSuite->m_pszFileData = VR_new BYTE[50];
    m_testSuite->m_dwFileSize = 50;
    m_testSuite->m_dwCursor = 50;
    EXPECT_FALSE(m_testSuite->WriteDWord(data));
}

TEST_F(VR_DP_ListData_Access_Test, WriteBuffer)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    UINT32 writedSize = 0;
    NCCHAR data[30] = "write buffer test";

    // file data is null
    if (NULL != m_testSuite->m_pszFileData) {
        delete m_testSuite->m_pszFileData;
        m_testSuite->m_pszFileData = NULL;
    }
    EXPECT_FALSE(m_testSuite->WriteBuffer(NULL, 0, writedSize));
    EXPECT_FALSE(m_testSuite->WriteBuffer(data, strlen(data), writedSize));

    // file data space not enough
    m_testSuite->m_pszFileData = VR_new BYTE[50];
    m_testSuite->m_dwFileSize = 50;
    m_testSuite->m_dwCursor = 50;
    EXPECT_FALSE(m_testSuite->WriteBuffer(data, strlen(data), writedSize));
}

TEST_F(VR_DP_ListData_Access_Test, ReadByte)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    BYTE data;

    // file data is null
    if (NULL != m_testSuite->m_pszFileData) {
        delete m_testSuite->m_pszFileData;
        m_testSuite->m_pszFileData = NULL;
    }
    EXPECT_FALSE(m_testSuite->ReadByte(data));

    // file data space not enough
    m_testSuite->m_pszFileData = VR_new BYTE[50];
    m_testSuite->m_dwFileSize = 50;
    m_testSuite->m_dwCursor = 50;
    EXPECT_FALSE(m_testSuite->ReadByte(data));
}

TEST_F(VR_DP_ListData_Access_Test, ReadWord)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    WORD data;

    // file data is null
    if (NULL != m_testSuite->m_pszFileData) {
        delete m_testSuite->m_pszFileData;
        m_testSuite->m_pszFileData = NULL;
    }
    EXPECT_FALSE(m_testSuite->ReadWord(data));

    // file data space not enough
    m_testSuite->m_pszFileData = VR_new BYTE[50];
    m_testSuite->m_dwFileSize = 50;
    m_testSuite->m_dwCursor = 50;
    EXPECT_FALSE(m_testSuite->ReadWord(data));
}

TEST_F(VR_DP_ListData_Access_Test, ReadDWord)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    UINT32 data;

    // file data is null
    if (NULL != m_testSuite->m_pszFileData) {
        delete m_testSuite->m_pszFileData;
        m_testSuite->m_pszFileData = NULL;
    }
    EXPECT_FALSE(m_testSuite->ReadDWord(data));

    // file data space not enough
    m_testSuite->m_pszFileData = VR_new BYTE[50];
    m_testSuite->m_dwFileSize = 50;
    m_testSuite->m_dwCursor = 50;
    EXPECT_FALSE(m_testSuite->ReadDWord(data));
}

TEST_F(VR_DP_ListData_Access_Test, ReadBuffer)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    UINT32 readedSize = 0;
    NCCHAR data[30] = { 0 };

    // file data is null
    if (NULL != m_testSuite->m_pszFileData) {
        delete m_testSuite->m_pszFileData;
        m_testSuite->m_pszFileData = NULL;
    }
    EXPECT_FALSE(m_testSuite->ReadBuffer(NULL, 0, readedSize));
    EXPECT_FALSE(m_testSuite->ReadBuffer(data, strlen(data), readedSize));

    // file data space not enough
    m_testSuite->m_pszFileData = VR_new BYTE[50];
    m_testSuite->m_dwFileSize = 50;
    m_testSuite->m_dwCursor = 50;
    EXPECT_FALSE(m_testSuite->ReadBuffer(data, sizeof(data), readedSize));
}

}
}
/* EOF */
