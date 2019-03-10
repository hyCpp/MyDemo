/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

#define private public
#define protected public

#include "VR_DEDataManager.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdlib>
using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;

/**
* VR_DEDataManager_test.cpp
*
* The class for VR_DEDataManager_test.
*/
class VR_DEDataManager_test : public testing::Test
{
public:
    VR_DEDataManager_test()
    {
        m_dedatamanager = NULL;
    }

    virtual ~VR_DEDataManager_test()
    {
    }

    virtual void SetUp()
    {
        m_dedatamanager = new VR_DEDataManager("");
    }

    virtual void TearDown()
    {
        delete m_dedatamanager;
    }

private:
    VR_DEDataManager_test(const VR_DEDataManager_test&);
    VR_DEDataManager_test& operator= (const VR_DEDataManager_test&);

    VR_DEDataManager* m_dedatamanager;
};

// initData BEGIN
TEST_F(VR_DEDataManager_test, initData)
{
    std::string language = "ms";
    EXPECT_TRUE(m_dedatamanager->initData(language));
    SUCCEED();
}
// initData END

// releaseData BEGIN
TEST_F(VR_DEDataManager_test, releaseData)
{
    EXPECT_TRUE(m_dedatamanager->releaseData());
    SUCCEED();
}
// releaseData END

// //changeLanguage BEGIN
// TEST_F(VR_DEDataManager_test, changeLanguage)
// {
//     std::string language;
//     EXPECT_TRUE(m_dedatamanager->changeLanguage(language));
//     SUCCEED();
// }
// //changeLanguage END

// getScxmlPath BEGIN
TEST_F(VR_DEDataManager_test, getScxmlPath_00)
{
    std::string agentName = "/tmp";
    m_dedatamanager->getScxmlPath(agentName);
    SUCCEED();
}

TEST_F(VR_DEDataManager_test, getScxmlPath_01)
{
    system("mkdir -p /data/engine");
    std::ofstream create_file("/data/engine/mscxmltest.zip", std::ios::binary);
    std::string agentName = "mscxmltest";
    m_dedatamanager->m_asrPath = "/data/";
    m_dedatamanager->getScxmlPath(agentName);
    SUCCEED();
    system("/data/engine/mscxmltest.zip");
}
// getScxmlPath END

// getPrompts BEGIN
TEST_F(VR_DEDataManager_test, getPrompts_0)
{
    std::string sKey = "name";
    m_dedatamanager->getPrompts(sKey);
    SUCCEED();
}

TEST_F(VR_DEDataManager_test, getPrompts_1)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_prompts);
    std::string sKey = "name";
    m_dedatamanager->getPrompts(sKey);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getPrompts_1_01)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_prompts);
    std::string sKey = "name";
    m_dedatamanager->getPrompts(sKey);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getPrompts_2)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_prompts);
    std::string sKey = "";
    m_dedatamanager->getPrompts(sKey);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getPrompts_3)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"xxxx\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_prompts);
    std::string sKey = "";
    m_dedatamanager->getPrompts(sKey);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getPrompts_4)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_prompts);
    std::string sKey = "skeyishere";
    m_dedatamanager->getPrompts(sKey);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getPrompts_5)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"xxxx\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_prompts);
    std::string sKey = "skeyishere";
    m_dedatamanager->getPrompts(sKey);
    SUCCEED();
    system("rm /data/test.txt");
}
// getPrompts END

// getState2Text BEGIN
TEST_F(VR_DEDataManager_test, getState2Text_0)
{
    std::string id = "";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
}

TEST_F(VR_DEDataManager_test, getState2Text_1)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"xxxx\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_id2text);
    std::string id = "";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_1_01)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_id2text);
    std::string id = "name";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_2)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_id2text);
    std::string id = "name";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_3)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"xxxx\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_id2text);
    std::string id = "1234";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_4)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"xxxx\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_id2text);
    std::string id = "1234";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_5)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_id2text);
    std::string id = "name";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_6_01)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);

    system("mkdir -p /data/");
    std::ofstream create_file2("/data/test2.txt", std::ios::binary);
    char buffer2[] = { 239, 187, 191 };
    create_file2.write(buffer2, 3);
    create_file2 << "{ \"name\" : \"name\"}" << std::endl;

    std::string filePath2 = "/data/test2.txt";
    m_dedatamanager->loadJsonData(filePath2, m_dedatamanager->m_id2text);

    std::string id = "name";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
    system("rm /data/test2.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_6_02)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);

    std::ofstream create_file2("/data/test2.txt", std::ios::binary);
    std::string filePath2 = "/data/test2.txt";
    m_dedatamanager->loadJsonData(filePath2, m_dedatamanager->m_id2text);

    std::string id = "name";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
    system("rm /data/test2.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_6_03)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);

    system("mkdir -p /data/");
    std::ofstream create_file2("/data/test2.txt", std::ios::binary);
    char buffer2[] = { 239, 187, 191 };
    create_file2.write(buffer2, 3);
    create_file2 << "{ \"xxxx\" : \"xxxx\"}" << std::endl;
    std::string filePath2 = "/data/test2.txt";
    m_dedatamanager->loadJsonData(filePath2, m_dedatamanager->m_id2text);

    std::string id = "name";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
    system("rm /data/test2.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_6_04)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);

    system("mkdir -p /data/");
    std::ofstream create_file2("/data/test2.txt", std::ios::binary);
    char buffer2[] = { 239, 187, 191 };
    create_file2.write(buffer2, 3);
    create_file2 << "{ \"name\" : \"xxxx\"}" << std::endl;
    std::string filePath2 = "/data/test2.txt";
    m_dedatamanager->loadJsonData(filePath2, m_dedatamanager->m_id2text);

    std::string id = "name";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
    system("rm /data/test2.txt");
}

TEST_F(VR_DEDataManager_test, getState2Text_6_05)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"name\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    m_dedatamanager->loadJsonData(filePath, m_dedatamanager->m_state2id);

    system("mkdir -p /data/");
    std::ofstream create_file2("/data/test2.txt", std::ios::binary);
    char buffer2[] = { 239, 187, 191 };
    create_file2.write(buffer2, 3);
    create_file2 << "{ \"name\" : \"\"}" << std::endl;
    std::string filePath2 = "/data/test2.txt";
    m_dedatamanager->loadJsonData(filePath2, m_dedatamanager->m_id2text);

    std::string id = "name";
    m_dedatamanager->getState2Text(id);
    SUCCEED();
    system("rm /data/test.txt");
    system("rm /data/test2.txt");
}
// getState2Text END


// loadData BEGIN
TEST_F(VR_DEDataManager_test, loadData)
{
    m_dedatamanager->m_language = "en-gb";
    m_dedatamanager->m_mapLangToEng[m_dedatamanager->m_language] = m_dedatamanager->m_language;
    EXPECT_TRUE(m_dedatamanager->loadData());
    SUCCEED();
}
// loadData END

// loadJsonData BEGIN
TEST_F(VR_DEDataManager_test, loadJsonData_0)
{
    std::string filePath = "/data/test.txt";
    rapidjson::Document doc;
    m_dedatamanager->loadJsonData(filePath, doc);
    SUCCEED();
}

TEST_F(VR_DEDataManager_test, loadJsonData_1)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt");
    create_file << "abcdefg" << std::endl;
    std::string filePath = "/data/test.txt";
    rapidjson::Document doc;
    m_dedatamanager->loadJsonData(filePath, doc);
    system("rm /data/test.txt");
    SUCCEED();
}

TEST_F(VR_DEDataManager_test, loadJsonData_2)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    create_file << "{ \"name\" : \"xxxx\"}" << std::endl;
    std::string filePath = "/data/test.txt";
    rapidjson::Document doc;
    m_dedatamanager->loadJsonData(filePath, doc);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, loadJsonData_3)
{
    system("mkdir -p /data/");
    std::ofstream create_file("/data/test.txt", std::ios::binary);
    char buffer[] = { 239, 187, 191 };
    create_file.write(buffer, 3);
    std::string filePath = "/data/test.txt";
    rapidjson::Document doc;
    m_dedatamanager->loadJsonData(filePath, doc);
    SUCCEED();
    system("rm /data/test.txt");
}

TEST_F(VR_DEDataManager_test, loadJsonData_4)
{
    std::string filePath = "";
    rapidjson::Document doc;
    m_dedatamanager->loadJsonData(filePath, doc);
    SUCCEED();
}
// loadJsonData END
/* EOF */
