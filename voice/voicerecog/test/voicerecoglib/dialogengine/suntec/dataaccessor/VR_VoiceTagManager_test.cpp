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

#include "VR_VoiceTagManager.h"
// #include "BL_AplSystem.h"
// #include "BL_DataMgt.h"
// #include "BL_AbstractSecureFile.h"

#include "VR_Configure_mock.h"

#include <pugixml.hpp>
#include "gtest/gtest.h"

using ::testing::Return;
using ::testing::_;

/**
* VR_VoiceTagManager_Impl
*
* The class is Impl test for VR_VoiceTagManager.
*/
class VR_VoiceTagManager_Impl : public VR_VoiceTagManager
{
public:
    VR_VoiceTagManager_Impl(VR_ConfigureIF* config)
        : VR_VoiceTagManager(config)
    {
    }

    virtual bool saveVoiceTagGrammar(const std::string &voiceTagID, const std::string &name, const std::string &phoneme, const std::string &deviceAddress)
    {
        return true;
    }

    virtual bool deleteVoiceTagGrammar(const VoiceList<std::pair<std::string, std::string>>::type &deleteList, const std::string &deviceAddress)
    {
        return true;
    }

    virtual bool deleteAllVoiceTagGrammar(const std::string &deviceAddress)
    {
        return true;
    }

    virtual bool clearVoiceTagGrammar()
    {
        return true;
    }

};

/**
* VR_VoiceTagManager_Test
*
* The class is just for VR_VoiceTagManager test.
*/
class VR_VoiceTagManager_Test : public testing::Test
{
public:
    VR_VoiceTagManager_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

    static VR_VoiceTagManager *m_VTManager;
    static VR_Configure_mock *m_configure;

private:
    VR_VoiceTagManager_Test(const VR_VoiceTagManager_Test& other);
    void operator= (const VR_VoiceTagManager_Test& other);
};

VR_VoiceTagManager *VR_VoiceTagManager_Test::m_VTManager = nullptr;
VR_Configure_mock *VR_VoiceTagManager_Test::m_configure = nullptr;

void VR_VoiceTagManager_Test::SetUpTestCase()
{
    // BL_AbstractSecureFile::Initialize();
    // BL_DataMgt::GetInstance()->Start("VR");
    m_configure = new VR_Configure_mock();
    ON_CALL(*m_configure, getUsrPath()).WillByDefault(Return("/tmp/"));
    ON_CALL(*m_configure, isFileExist(_)).WillByDefault(Return(true));
    ON_CALL(*m_configure, removeFile(_)).WillByDefault(Return(true));

    ON_CALL(*m_configure, makeDir("/tmp/rec/")).WillByDefault(Return(system("mkdir /tmp/rec")));
    m_VTManager = new VR_VoiceTagManager_Impl(m_configure);
}

void VR_VoiceTagManager_Test::TearDownTestCase()
{
    delete m_configure;
    m_configure = nullptr;
    delete m_VTManager;
    m_VTManager = nullptr;
    // BL_DataMgt::GetInstance()->Stop();
    // BL_AbstractSecureFile::UnInitialize();
}

TEST_F(VR_VoiceTagManager_Test, saveVoiceTag)
{
    std::string contactMsg;
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, false);

    contactMsg = "<contact name=\"David Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</contact>";
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, false);

    contactMsg = "<contact name=\"DavidName SmithNew\" asrText=\"David Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"0\" number=\"021-10010\"/>"
                "</contact>";
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, true);

    m_VTManager->setCurrentDevice("1");
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, true);

    ON_CALL(*m_configure, isFileExist(_)).WillByDefault(Return(true));
    ON_CALL(*m_configure, removeFile(_)).WillByDefault(Return(false));
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, true);


    ON_CALL(*m_configure, isFileExist(_)).WillByDefault(Return(false));
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, true);

    ON_CALL(*m_configure, isFileExist(_)).WillByDefault(Return(true));
    ON_CALL(*m_configure, removeFile(_)).WillByDefault(Return(true));

    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, true);
    m_VTManager->saveVoiceTag("6", "0", "/tmp/tmp.wav", "abc", contactMsg, true);

    m_VTManager->setCurrentDevice("1");

    m_VTManager->saveVoiceTag("", "0", "/tmp/tmp.wav", "abc", contactMsg, true);
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "", contactMsg, true);

    contactMsg = "<contact name=\"DavidName SmithNew\" asrText=\"David Smith\" contactID=\"3\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"0\" number=\"021-10010\"/>"
                "</contact>";
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, true);

    ON_CALL(*m_configure, isFileExist(_)).WillByDefault(Return(false));
    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, true);
    ON_CALL(*m_configure, isFileExist(_)).WillByDefault(Return(true));
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, getVoiceTagPCMPath)
{
    m_VTManager->setCurrentDevice("1");
    EXPECT_STREQ("/tmp/rec/1/0.wav", m_VTManager->getVoiceTagPCMPath("0").c_str());

    m_VTManager->clearCurrentDevice();
    EXPECT_TRUE(m_VTManager->getVoiceTagPCMPath("0").empty());
    m_VTManager->setCurrentDevice("6");
    EXPECT_TRUE(m_VTManager->getVoiceTagPCMPath("0").empty());
    m_VTManager->setCurrentDevice("2");
    EXPECT_TRUE(m_VTManager->getVoiceTagPCMPath("0").empty());
    m_VTManager->setCurrentDevice("5");
    EXPECT_TRUE(m_VTManager->getVoiceTagPCMPath("0").empty());
    m_VTManager->setCurrentDevice("1");
}

TEST_F(VR_VoiceTagManager_Test, deleteVoiceTag)
{
    EXPECT_FALSE(m_VTManager->deleteVoiceTag("", "0"));
    m_VTManager->deleteVoiceTag("6", "0");
    m_VTManager->deleteVoiceTag("2", "0");
    m_VTManager->deleteVoiceTag("1", "0");
    m_VTManager->deleteVoiceTag("3", "0");
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, deleteAllVoiceTag)
{
    EXPECT_FALSE(m_VTManager->deleteAllVoiceTag(""));
    m_VTManager->deleteAllVoiceTag("6");
    m_VTManager->deleteAllVoiceTag("2");
    m_VTManager->deleteAllVoiceTag("1");
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, clearVoiceTag)
{
    m_VTManager->clearVoiceTag();
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, syncVoiceTag)
{
    m_VTManager->setCurrentDevice("1");
    std::string contactMsg;
    std::string btVoiceTagIDStr = "1,2,3";
    std::string btContactIDStr = "1,2,3";
    VoiceList<std::string>::type btIDList;

    m_VTManager->syncVoiceTag("1", btContactIDStr, btVoiceTagIDStr);

    contactMsg = "<contact name=\"David Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"0\" number=\"021-10010\"/>"
                "</contact>";
    m_VTManager->saveVoiceTag("1", "1", "/tmp/tmp.wav", "abc", contactMsg, false);
    m_VTManager->syncVoiceTag("1", btContactIDStr, btVoiceTagIDStr);

    contactMsg = "<contact name=\"David Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"0\" number=\"021-10010\"/>"
                "</contact>";
    m_VTManager->saveVoiceTag("1", "1", "/tmp/tmp.wav", "abc", contactMsg, false);
    m_VTManager->saveVoiceTag("1", "2", "/tmp/tmp.wav", "abc", contactMsg, false);
    // btIDList.push_back("1");
    // btIDList.push_back("2");
    // btIDList.push_back("3");
    m_VTManager->syncVoiceTag("1", btContactIDStr, btVoiceTagIDStr);
    m_VTManager->syncVoiceTag("6", btContactIDStr, btVoiceTagIDStr);
    m_VTManager->syncVoiceTag("2", btContactIDStr, btVoiceTagIDStr);
    m_VTManager->syncVoiceTag("5", btContactIDStr, btVoiceTagIDStr);

    // btIDList.clear();
    // btIDList.push_back("3");
    // btIDList.push_back("4");
    m_VTManager->syncVoiceTag("1", btContactIDStr, btVoiceTagIDStr);

    // btIDList.clear();
    // btIDList.push_back("0");
    // btIDList.push_back("1");
    // btIDList.push_back("2");
    // btIDList.push_back("3");
    m_VTManager->syncVoiceTag("1", btContactIDStr, btVoiceTagIDStr);

    btIDList.clear();
    btIDList.push_back("1");
    m_VTManager->syncVoiceTag("1", btContactIDStr, btVoiceTagIDStr);

    m_VTManager->saveVoiceTag("1", "0", "/tmp/tmp.wav", "abc", contactMsg, false);
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, updateVoiceTag)
{
    std::string contactMsg;
    contactMsg = "<contact name=\"Tom\">"
                    "<phone_item type=\"1\" number=\"10086\"/>"
                "</contact>";
    m_VTManager->saveVoiceTag("1", "1", "/tmp/tmp.wav", "abc", contactMsg, false);
    contactMsg = "<contact name=\"Jimmy\" asrText=\"JimmyOldName\">"
                    "<phone_item type=\"2\" number=\"95588\"/>"
                "</contact>";
    pugi::xml_document doc;
    doc.load_string(contactMsg.c_str());
    pugi::xml_node contactNode = doc.first_child();
    EXPECT_TRUE(m_VTManager->updateVoiceTag("1", contactNode));

    EXPECT_FALSE(m_VTManager->updateVoiceTag("6", contactNode));

    m_VTManager->setCurrentDevice("6");
    EXPECT_FALSE(m_VTManager->updateVoiceTag("1", contactNode));
    m_VTManager->setCurrentDevice("5");
    EXPECT_FALSE(m_VTManager->updateVoiceTag("1", contactNode));
    m_VTManager->clearCurrentDevice();
    EXPECT_FALSE(m_VTManager->updateVoiceTag("1", contactNode));
    m_VTManager->setCurrentDevice("5");
    contactMsg = "<contact name=\"Jimmy\" asrText=\"\">"
                    "<phone_item type=\"2\" number=\"95588\"/>"
                "</contact>";
    doc.load_string(contactMsg.c_str());
    contactNode = doc.first_child();
    EXPECT_FALSE(m_VTManager->updateVoiceTag("1", contactNode));
}

TEST_F(VR_VoiceTagManager_Test, setCurrentDevice)
{
    m_VTManager->setCurrentDevice("abc");
    EXPECT_STREQ("abc", m_VTManager->m_currentDeviceAddress.c_str());
    m_VTManager->setCurrentDevice("");
    EXPECT_STREQ("abc", m_VTManager->m_currentDeviceAddress.c_str());
}

TEST_F(VR_VoiceTagManager_Test, clearCurrentDevice)
{
    m_VTManager->clearCurrentDevice();
    EXPECT_TRUE(m_VTManager->m_currentDeviceAddress.empty());
}

TEST_F(VR_VoiceTagManager_Test, getContactInfo)
{
    m_VTManager->setCurrentDevice("6");
    m_VTManager->getContactInfo("0");
    m_VTManager->setCurrentDevice("1");
    m_VTManager->getContactInfo("0");
    m_VTManager->getContactInfo("1");
    m_VTManager->getContactInfo("2");
    m_VTManager->setCurrentDevice("5");
    m_VTManager->getContactInfo("0");
    m_VTManager->m_currentDeviceAddress.clear();
    m_VTManager->getContactInfo("0");
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, getVoiceTagCount)
{
    std::string contactMsg;
    m_VTManager->setCurrentDevice("2");

    contactMsg = "<contact name=\"David Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"0\" number=\"021-10010\"/>"
                "</contact>";
    m_VTManager->saveVoiceTag("2", "15", "/tmp/tmp.wav", "abc", contactMsg, false);

    contactMsg = "<contact name=\"Lily\">"
                    "<phone_item type=\"0\" number=\"010-21288\"/>"
                "</contact>";
    m_VTManager->saveVoiceTag("2", "16", "/tmp/tmp.wav", "cfed", contactMsg, false);

    EXPECT_EQ(2, m_VTManager->getVoiceTagCount());
    m_VTManager->clearCurrentDevice();
    EXPECT_EQ(0, m_VTManager->getVoiceTagCount());
    m_VTManager->setCurrentDevice("6");
    EXPECT_EQ(0, m_VTManager->getVoiceTagCount());
    m_VTManager->setCurrentDevice("5");
    EXPECT_EQ(0, m_VTManager->getVoiceTagCount());
}

TEST_F(VR_VoiceTagManager_Test, getContactIDByVoiceTagID)
{
    m_VTManager->m_voiceTagContactIDMap.insert(std::pair<std::string, std::string>("1", "pengjialing"));
    m_VTManager->getContactIDByVoiceTagID("1");
    m_VTManager->getContactIDByVoiceTagID("100");
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, getVoiceTagIDByContactID)
{
    m_VTManager->m_voiceTagContactIDMap.insert(std::pair<std::string, std::string>("1", "pengjialing"));
    m_VTManager->getVoiceTagIDByContactID("1");
    m_VTManager->getVoiceTagIDByContactID("pengjialing");
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, getVoiceTagInfo)
{
    VoiceList<VR_VoiceTagInfo>::type list;
    m_VTManager->getVoiceTagInfo("8", list);
    m_VTManager->getVoiceTagInfo("", list);
    SUCCEED();
}

TEST_F(VR_VoiceTagManager_Test, getVoiceTagGrammarParam)
{
    VoiceList<VR_VoiceTagManager::VoiceTagGrammarParam>::type paramList;
    m_VTManager->setCurrentDevice("6");
    m_VTManager->getVoiceTagGrammarParam(paramList);
    EXPECT_TRUE(paramList.empty());
    m_VTManager->clearCurrentDevice();
    m_VTManager->getVoiceTagGrammarParam(paramList);
    EXPECT_TRUE(paramList.empty());
    m_VTManager->setCurrentDevice("4");
    m_VTManager->getVoiceTagGrammarParam(paramList);
    EXPECT_TRUE(paramList.empty());
    m_VTManager->setCurrentDevice("1");
    m_VTManager->getVoiceTagGrammarParam(paramList);
    EXPECT_FALSE(paramList.empty());
}

/* EOF */
