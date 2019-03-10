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

#include "VR_VoiceTagIDManager.h"

#include "gtest/gtest.h"

/**
* VR_VoiceTagIDManager_Test
*
* The class is just for VR_VoiceTagIDManager test.
*/
class VR_VoiceTagIDManager_Test : public testing::Test
{
public:
    VR_VoiceTagIDManager_Test()
    {
    }

    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}

private:
    VR_VoiceTagIDManager_Test(const VR_VoiceTagIDManager_Test& other);
    void operator= (const VR_VoiceTagIDManager_Test& other);
};

TEST_F(VR_VoiceTagIDManager_Test, saveVoiceTagID)
{
    VR_VoiceTagIDManager::getInstance()->saveVoiceTagID("0");
    VR_VoiceTagIDManager::getInstance()->saveVoiceTagID("1");
    VR_VoiceTagIDManager::getInstance()->saveVoiceTagID("999");
    SUCCEED();
}

TEST_F(VR_VoiceTagIDManager_Test, deleteVoiceTagID)
{
    VoiceList<std::string>::type deleteList;
    deleteList.push_back("0");
    VR_VoiceTagIDManager::getInstance()->deleteVoiceTagID(deleteList);
    deleteList.clear();
    deleteList.push_back("2");
    VR_VoiceTagIDManager::getInstance()->deleteVoiceTagID(deleteList);
    SUCCEED();
}

TEST_F(VR_VoiceTagIDManager_Test, clearVoiceTagID)
{
    VR_VoiceTagIDManager::getInstance()->clearVoiceTagID();
    SUCCEED();
}

TEST_F(VR_VoiceTagIDManager_Test, getVoiceTagID)
{
    VR_VoiceTagIDManager::getInstance()->getVoiceTagID("-1");
    VR_VoiceTagIDManager::getInstance()->getVoiceTagID("0");
    VR_VoiceTagIDManager::getInstance()->saveVoiceTagID("0");
    VR_VoiceTagIDManager::getInstance()->getVoiceTagID("0");
    SUCCEED();
}

TEST_F(VR_VoiceTagIDManager_Test, getVoiceTagStorageKey)
{
    VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey("0");
    VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey("1");
    VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey("2");
    VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey("3");
    VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey("4");
    VR_VoiceTagIDManager::getInstance()->getVoiceTagStorageKey("5");
    SUCCEED();
}

TEST_F(VR_VoiceTagIDManager_Test, constructor)
{
    VR_VoiceTagIDManager::getInstance()->saveVoiceTagID("1");
    VR_VoiceTagIDManager::getInstance()->saveVoiceTagID("2");
    VR_VoiceTagIDManager::getInstance()->saveVoiceTagID("999");
    VR_VoiceTagIDManager managerOne;
    VR_VoiceTagIDManager::getInstance()->clearVoiceTagID();
    VR_VoiceTagIDManager managerTwo;
}
/* EOF */