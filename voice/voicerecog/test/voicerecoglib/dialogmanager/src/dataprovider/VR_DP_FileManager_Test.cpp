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
#include "VR_DP_ListData_Access.h"
#include "VR_DP_FileManager_Test.h"
#include "ncore/NCFile.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_DP_FileManager* VR_DP_FileManager_Test::m_testSuite = NULL;

VOID VR_DP_FileManager_Test::SetUpTestCase()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_DP_FileManager::GetInstance();
    }
}

VOID VR_DP_FileManager_Test::TearDownTestCase()
{
    if (NULL != m_testSuite) {
        VR_DP_FileManager::Dispose();
    }
}

VR_DP_FileManager_Test::VR_DP_FileManager_Test()
{
}

VR_DP_FileManager_Test::~VR_DP_FileManager_Test()
{
}

TEST_F(VR_DP_FileManager_Test, SetFileCache)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    INT fileKey;
    NCCHAR* fileName;
    NCCHAR* cachePath;
    INT cacheSize;

    // file key invalid
    fileKey = -1;
    fileName = "VRListDataCity.dat";
    cachePath = "/tmp";
    cacheSize = 3;
    EXPECT_FALSE(m_testSuite->SetFileCache(fileKey, fileName, cachePath, cacheSize));

    // cache size invalid
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    fileName = "VRListDataCity.dat";
    cachePath = "/tmp";
    cacheSize = -1;
    EXPECT_FALSE(m_testSuite->SetFileCache(fileKey, fileName, cachePath, cacheSize));

    // file name is null
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    fileName = NULL;
    cachePath = "/tmp";
    cacheSize = 3;
    EXPECT_FALSE(m_testSuite->SetFileCache(fileKey, fileName, cachePath, cacheSize));

    // cache path is null
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    fileName = "VRListDataCity.dat";
    cachePath = NULL;
    cacheSize = 3;
    EXPECT_FALSE(m_testSuite->SetFileCache(fileKey, fileName, cachePath, cacheSize));

    // assign VRListDataCity file cache space
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    fileName = "VRListDataCity.dat";
    cachePath = "/tmp";
    cacheSize = 3;
    EXPECT_TRUE(m_testSuite->SetFileCache(fileKey, fileName, cachePath, cacheSize));

    // repeat
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    fileName = "VRListDataCity.dat";
    cachePath = "/tmp";
    cacheSize = 3;
    EXPECT_TRUE(m_testSuite->SetFileCache(fileKey, fileName, cachePath, cacheSize));
}

TEST_F(VR_DP_FileManager_Test, PushBack)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    INT fileKey;
    NCCHAR* filePath;
    DPFileHead* headNode;
    DPFileNode* nodeList;

    // file key invalid
    fileKey = -1;
    filePath = "/tmp/VRListDataCity.dat";
    EXPECT_FALSE(m_testSuite->PushBack(fileKey, filePath));

    // file path invalid
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    filePath = NULL;
    EXPECT_FALSE(m_testSuite->PushBack(fileKey, filePath));

    // VRListDataCity file cache space not assign => key not exist
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    filePath = "/tmp/VRListDataCity.dat";
    headNode = reinterpret_cast<DPFileHead*>(m_testSuite->m_fileMap[fileKey]);
    VR_DP_FileManager::DPFileMap::iterator iterator;
    iterator = m_testSuite->m_fileMap.find(fileKey);
    m_testSuite->m_fileMap.erase(iterator);
    EXPECT_FALSE(m_testSuite->PushBack(fileKey, filePath));

    // VRListDataCity file cache space not assign => value not exist
    m_testSuite->m_fileMap[fileKey] = (INT64)0;
    EXPECT_FALSE(m_testSuite->PushBack(fileKey, filePath));
    m_testSuite->m_fileMap[fileKey] = reinterpret_cast<INT64>(headNode);

    // VRListDataCity file cache space not assign => node list not exist
    nodeList = headNode->nodeList;
    headNode->nodeList = NULL;
    EXPECT_FALSE(m_testSuite->PushBack(fileKey, filePath));
    headNode->nodeList = nodeList;

    // cache not overflow and cache file not exist
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    filePath = "/tmp/VRListDataCity.dat";
    BL_File dbFile(BL_FILE_PREFIX_TMP);
    dbFile.Open("VRListDataCity.dat", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_TRUE(m_testSuite->PushBack(fileKey, filePath));

    // cache not overflow and cache file exist
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    filePath = "/tmp/VRListDataCity.dat";
    dbFile.Open("VRListDataCity.dat", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    dbFile.Open("VRListDataCity.dat.temp2", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_TRUE(m_testSuite->PushBack(fileKey, filePath));

    // cache full
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    filePath = "/tmp/VRListDataCity.dat";
    dbFile.Open("VRListDataCity.dat", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_TRUE(m_testSuite->PushBack(fileKey, filePath));

    // cache overflow and first cache not occupy. virtual = real + 1
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    filePath = "/tmp/VRListDataCity.dat";
    dbFile.Open("VRListDataCity.dat", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_TRUE(m_testSuite->PushBack(fileKey, filePath));

    // cache overflow and first cache occupy. virtaul = real + 2
    nodeList[0].accessCount = 1;
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    filePath = "/tmp/VRListDataCity.dat";
    dbFile.Open("VRListDataCity.dat", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_TRUE(m_testSuite->PushBack(fileKey, filePath));

    // cache overflow and last cache occupy. virtual = real + 3
    nodeList[0].accessCount = 1;
    headNode->nodeMaxLen = 1;
    headNode->nodeLen = 1;
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    filePath = "/tmp/VRListDataCity.dat";
    dbFile.Open("VRListDataCity.dat", BL_FILE_OPEN_MODE_WP);
    dbFile.Close();
    EXPECT_TRUE(m_testSuite->PushBack(fileKey, filePath));
    nodeList[0].accessCount = 0;
    headNode->nodeMaxLen = 3;
    headNode->nodeLen = 3;
    NCFile::remove("/tmp/VRListDataCity.dat");
}

TEST_F(VR_DP_FileManager_Test, Front)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    INT fileKey;
    DPFileHead* headNode;
    DPFileNode* nodeList;
    NCCHAR filePath[VR_DPCACHE_PATH_MAXLEN + 4] = { 0 };
    NCCHAR fileName[VR_DPCACHE_FILENAME_MAXLEN + 4] = { 0 };

    // file key invalid
    fileKey = -1;
    EXPECT_FALSE(m_testSuite->Front(fileKey, filePath, fileName));

    // file path is null or file name is null
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    EXPECT_FALSE(m_testSuite->Front(fileKey, NULL, fileName));
    EXPECT_FALSE(m_testSuite->Front(fileKey, filePath, NULL));

    // VRListDataCity file cache space not assign => key not exist
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    headNode = reinterpret_cast<DPFileHead*>(m_testSuite->m_fileMap[fileKey]);
    VR_DP_FileManager::DPFileMap::iterator iterator;
    iterator = m_testSuite->m_fileMap.find(fileKey);
    m_testSuite->m_fileMap.erase(iterator);
    EXPECT_FALSE(m_testSuite->Front(fileKey, filePath, fileName));

    // VRListDataCity file cache space not assign => value not exist
    m_testSuite->m_fileMap[fileKey] = (INT64)0;
    EXPECT_FALSE(m_testSuite->Front(fileKey, filePath, fileName));
    m_testSuite->m_fileMap[fileKey] = reinterpret_cast<INT64>(headNode);

    // cache overflow. [virtual - real = 3]
    EXPECT_TRUE(m_testSuite->Front(fileKey, filePath, fileName));

    // not exist valid node
    headNode->virtualNodeLen = 0;
    headNode->nodeLen = 0;
    EXPECT_FALSE(m_testSuite->Front(fileKey, filePath, fileName));
    headNode->virtualNodeLen = 3;
    headNode->nodeLen = 3;

    // VRListDataCity file cache space not assign => node list not exist
    nodeList = headNode->nodeList;
    headNode->nodeList = NULL;
    EXPECT_FALSE(m_testSuite->Front(fileKey, filePath, fileName));
    headNode->nodeList = nodeList;

    // VRListDataCity file cache front first node
    EXPECT_TRUE(m_testSuite->Front(fileKey, filePath, fileName));
    headNode->virtualNodeLen = 6;
}

TEST_F(VR_DP_FileManager_Test, PopFront)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    INT fileKey;
    DPFileHead* headNode;
    DPFileNode* nodeList;

    // file key invalid
    fileKey = -1;
    EXPECT_FALSE(m_testSuite->PopFront(fileKey));

    // VRListDataCity file cache space not assign => key not exist
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    headNode = reinterpret_cast<DPFileHead*>(m_testSuite->m_fileMap[fileKey]);
    VR_DP_FileManager::DPFileMap::iterator iterator;
    iterator = m_testSuite->m_fileMap.find(fileKey);
    m_testSuite->m_fileMap.erase(iterator);
    EXPECT_FALSE(m_testSuite->PopFront(fileKey));

    // VRListDataCity file cache space not assign => value not exist
    m_testSuite->m_fileMap[fileKey] = (INT64)0;
    EXPECT_FALSE(m_testSuite->PopFront(fileKey));
    m_testSuite->m_fileMap[fileKey] = reinterpret_cast<INT64>(headNode);

    // cache overflow. [virtual - real = 3]
    EXPECT_TRUE(m_testSuite->PopFront(fileKey));
    headNode->virtualNodeLen = 3;

    // VRListDataCity file cache space not assign => node list not exist
    nodeList = headNode->nodeList;
    headNode->nodeList = NULL;
    EXPECT_FALSE(m_testSuite->PopFront(fileKey));
    headNode->nodeList = nodeList;

    // first node occupy and file exist. nodeLen = 2
    nodeList[0].accessCount = 1;
    EXPECT_TRUE(m_testSuite->PopFront(fileKey));

    // first node not occupy and file not exist. nodeLen = 1
    nodeList[0].accessCount = 0;
    NCFile::remove(nodeList[0].path);
    EXPECT_TRUE(m_testSuite->PopFront(fileKey));

    // node list empty
    EXPECT_TRUE(m_testSuite->PopFront(fileKey));
}

TEST_F(VR_DP_FileManager_Test, Clear)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    INT fileKey;
    DPFileHead* headNode;
    DPFileNode* nodeList;

    // file key invalid
    fileKey = -1;
    EXPECT_FALSE(m_testSuite->Clear(fileKey));

    // VRListDataCity file cache space not assign => key not exist
    fileKey = VR_DP_ListData_Access::eFileType_CityList + 1;
    headNode = reinterpret_cast<DPFileHead*>(m_testSuite->m_fileMap[fileKey]);
    VR_DP_FileManager::DPFileMap::iterator iterator;
    iterator = m_testSuite->m_fileMap.find(fileKey);
    m_testSuite->m_fileMap.erase(iterator);
    EXPECT_FALSE(m_testSuite->Clear(fileKey));

    // VRListDataCity file cache space not assign => value not exist
    m_testSuite->m_fileMap[fileKey] = (INT64)0;
    EXPECT_FALSE(m_testSuite->Clear(fileKey));
    m_testSuite->m_fileMap[fileKey] = reinterpret_cast<INT64>(headNode);

    // VRListDataCity file cache space not assign => node list not exist
    nodeList = headNode->nodeList;
    headNode->nodeList = NULL;
    EXPECT_FALSE(m_testSuite->Clear(fileKey));
    headNode->nodeList = nodeList;

    // clear node list content
    EXPECT_TRUE(m_testSuite->Clear(fileKey));
}

}
}
/* EOF */
