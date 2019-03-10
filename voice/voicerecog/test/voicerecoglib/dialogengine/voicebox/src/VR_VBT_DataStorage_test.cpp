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
#include <string>
#include "gtest/gtest.h"
#include "VR_VoiceBoxDataStorage.h"
#include "BL_DataStorage.h"

/**
* VR_VBT_DataStorage_Test
*
* The class is just for VR_VoiceBoxDataStorage test.
*/

class VR_VBT_DataStorage_Test : public testing::Test
{
public:
    VR_VBT_DataStorage_Test() : m_dataStorage(NULL)
    {
    }

    virtual void SetUp();
    virtual void TearDown();
private:
    VR_VoiceBoxDataStorage* m_dataStorage;
};

void VR_VBT_DataStorage_Test::SetUp()
{
    m_dataStorage = new VR_VoiceBoxDataStorage();
}

void VR_VBT_DataStorage_Test::TearDown()
{
    delete m_dataStorage;
    m_dataStorage = NULL;
}

TEST_F(VR_VBT_DataStorage_Test, GetValue_String)
{
    /* BL_DATA_KEY_VR_VOICETAG_1, value is null, GetValue Failed */
    {
        std::string deviceVoiceTagValueStr;
        m_dataStorage->GetValue(deviceVoiceTagValueStr);
        SUCCEED();
    }
    /* BL_DATA_KEY_VR_VOICETAG_2, value is not null, GetValue OK */
    {
        std::string valueStr = "helloworld";
        std::string deviceVoiceTagValueStr;
        m_dataStorage->PutValue(valueStr);
        m_dataStorage->GetValue(deviceVoiceTagValueStr);
        SUCCEED();
    }
}

TEST_F(VR_VBT_DataStorage_Test, GetValue_With_ID)
{
    {
        std::string deviceVoiceTagValueStr;
        m_dataStorage->GetValue(5, deviceVoiceTagValueStr);
        SUCCEED();
    }

    {
        std::string valueStr = "helloworld";
        std::string deviceVoiceTagValueStr;
        m_dataStorage->PutValue(5, valueStr);
        m_dataStorage->GetValue(5, deviceVoiceTagValueStr);
        SUCCEED();
        m_dataStorage->DeleteValue(5);
    }
}

TEST_F(VR_VBT_DataStorage_Test, GetValue_Data)
{
    int i = 5;
    m_dataStorage->PutValue(5, &i, sizeof(i));
    int j = 0;
    int size = 0;
    m_dataStorage->GetValue(5, &j, size);
    // EXPECT_EQ(i, j);
    // EXPECT_EQ(sizeof(i), size);
    m_dataStorage->DeleteValue(5);
}

TEST_F(VR_VBT_DataStorage_Test, DeleteValue_SmartAgent)
{
    m_dataStorage->DeleteValue();
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, RemoveDir)
{
    m_dataStorage->RemoveDir("");
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, RemoveFile)
{
    m_dataStorage->RemoveFile("");
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, GetGrammarStatusValue)
{
    unsigned int statusvalue = 1;
    m_dataStorage->GetGrammarStatusValue(statusvalue);
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, PutGrammarStatusValue)
{
    unsigned int statusvalue = 1;
    m_dataStorage->PutGrammarStatusValue(statusvalue);
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, PutInstallTslAgent_1)
{
    std::string strInstall = "Install";
    m_dataStorage->PutInstallTslAgent(strInstall);
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, PutInstallTslAgent_2)
{
    std::string strInstall = "UnInstall";
    m_dataStorage->PutInstallTslAgent(strInstall);
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, GetInstallTslAgent_1)
{
    std::string strInstall = "Install";
    m_dataStorage->GetInstallTslAgent(strInstall);
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, GetInstallTslAgent_2)
{
    std::string strInstall = "UnInstall";
    m_dataStorage->PutInstallTslAgent(strInstall);
    m_dataStorage->GetInstallTslAgent(strInstall);
    SUCCEED();
}

TEST_F(VR_VBT_DataStorage_Test, DeleteAllVoiceTag)
{
    m_dataStorage->DeleteAllVoiceTag();
    SUCCEED();
}

/* EOF */
