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

#include "VR_DataAccessorManager.h"
#include "Vr_Asr_Engine.h"

#include "VR_AsrRequestor_mock.h"
#include "VR_DECommon_mock.h"
#include "VR_Configure_mock.h"
#include "Vr_Asr_Event_Phase_mock.h"
#include "VR_DataAccessorVoiceTag.h"
#include "VR_VoiceTagQuerier_mock.h"

#include <sqlite3.h>
#include "gtest/gtest.h"

using ::testing::Return;

/**
* VR_DataAccessorManager_Test
*
* The class is just for VR_DataAccessorManager test.
*/
class VR_DataAccessorManager_Test : public testing::Test
{
public:
    VR_DataAccessorManager_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

    static VR_DataAccessorManager *m_manager;
    static VR_DataAccessorManager *m_managerForVoiceTag;
    static VR_AsrRequestor_mock *m_asrRequestorMock;
    static VR_DECommon_mock *m_DECommon;
    static VR_Configure_mock *m_configure;
private:
    VR_DataAccessorManager_Test(const VR_DataAccessorManager_Test& other);
    void operator= (const VR_DataAccessorManager_Test& other);
};

std::string g_msgToDM;
void updateStateCallback(const std::string &msgToDM)
{
    g_msgToDM = msgToDM;
}

VR_DataAccessorManager *VR_DataAccessorManager_Test::m_manager;
VR_DataAccessorManager *VR_DataAccessorManager_Test::m_managerForVoiceTag;
VR_AsrRequestor_mock *VR_DataAccessorManager_Test::m_asrRequestorMock;
VR_DECommon_mock *VR_DataAccessorManager_Test::m_DECommon;
VR_Configure_mock *VR_DataAccessorManager_Test::m_configure;

// common test
// Test for EU
// Test for CN
// Test for general
// Test for VoiceTag

void VR_DataAccessorManager_Test::SetUpTestCase()
{
    sqlite3 *m_dbHandler;
    int dbResult = sqlite3_open("/tmp/MusicCatalog001.db", &m_dbHandler);
    ASSERT_EQ(SQLITE_OK, dbResult);

    // init db data
    char * errmsg = nullptr;

    VoiceMap<std::string, std::string>::type tableFormatMap;
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentAlbums",
            "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentArtists",
            "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentGenres",
            "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentPlaylists",
            "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentSongs",
            "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));

    std::string sqlRequest;
    for (VoiceMap<std::string, std::string>::iterator it = tableFormatMap.begin();
        it != tableFormatMap.end();
        ++it) {
        sqlRequest.assign("CREATE TABLE " + it->first + "(" + it->second + ")");
        int result = sqlite3_open("/tmp/MusicCatalog001.db", &m_dbHandler);
        result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);
        if (SQLITE_OK != result) {
            return;
        }
    }

    sqlite3_exec(m_dbHandler, "BEGIN", NULL, NULL, NULL);

    // save contact to DB
    sqlRequest.assign("INSERT INTO MusicAgentAlbums VALUES ("
        "\"1\", "
        "\"space\", "
        "\"1\", "
        "\"1\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentAlbums VALUES ("
        "\"2\", "
        "\"empty\", "
        "\"1\", "
        "\"2\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentArtists VALUES ("
        "\"1\", "
        "\"David\", "
        "\"1\", "
        "\"1\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentArtists VALUES ("
        "\"2\", "
        "\"Alice\", "
        "\"1\", "
        "\"2\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenres VALUES ("
        "\"1\", "
        "\"classic\", "
        "\"1\", "
        "\"1\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenres VALUES ("
        "\"2\", "
        "\"blue\", "
        "\"1\", "
        "\"2\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);
    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
        "\"1\", "
        "\"first\", "
        "\"1\", "
        "\"1\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
        "\"2\", "
        "\"second\", "
        "\"1\", "
        "\"2\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
        "\"1\", "
        "\"yesterday once more Ft. test\", "
        "\"1\", "
        "\"1\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
        "\"2\", "
        "\"lemon tree\", "
        "\"1\", "
        "\"2\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
        "\"3\", "
        "\"tomorrow\", "
        "\"1\", "
        "\"3\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlite3_exec(m_dbHandler, "COMMIT", NULL, NULL, NULL);
    sqlite3_close(m_dbHandler);

    m_asrRequestorMock = new VR_AsrRequestor_mock();
    m_DECommon = new VR_DECommon_mock();
    m_configure = new VR_Configure_mock();

    ON_CALL(*m_configure, getVRContry()).WillByDefault(Return(VR_REGION_HK));
    ON_CALL(*m_configure, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*m_configure, getVRProduct()).WillByDefault(Return(VR_PRODUCT_TYPE_L2));
    ON_CALL(*m_configure, getDataPath()).WillByDefault(Return("/tmp/"));
    ON_CALL(*m_configure, getMapDataPath()).WillByDefault(Return("/data/PP_Data"));
    system("mkdir /tmp/VR_DataAccessorTest");
    ON_CALL(*m_configure, getUsrPath()).WillByDefault(Return("/tmp/VR_DataAccessorTest/"));
    ON_CALL(*m_configure, getEngineType()).WillByDefault(Return(VR_ConfigureIF::VR_EngineType::SUNTEC));

    ON_CALL(*m_DECommon, isGetFromData()).WillByDefault(Return(false));
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::gen));

    m_manager = new VR_DataAccessorManager(m_asrRequestorMock, m_DECommon, m_configure);
    boost::function<void(const std::string &)> callback = &updateStateCallback;
    m_manager->setUpdateStateCallback(callback);
    m_manager->setNotifyCallback(callback);
    m_manager->setCountryID("1");

    ON_CALL(*m_DECommon, getProductCountry()).WillByDefault(Return("SouthKorea"));
    m_managerForVoiceTag = new VR_DataAccessorManager(m_asrRequestorMock, m_DECommon, m_configure);
    ON_CALL(*m_DECommon, getProductCountry()).WillByDefault(Return("Southeast-Asia"));
    m_managerForVoiceTag = new VR_DataAccessorManager(m_asrRequestorMock, m_DECommon, m_configure);
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::vt));
    ON_CALL(*m_configure, getUsrPath()).WillByDefault(Return("/tmp/VR_DataAccessorTest/NoDB/"));
    m_managerForVoiceTag = new VR_DataAccessorManager(m_asrRequestorMock, m_DECommon, m_configure);



    ON_CALL(*m_configure, getUsrPath()).WillByDefault(Return("/tmp/VR_DataAccessorTest"));
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::eu));
    VR_DataAccessorManager managerEU(m_asrRequestorMock, m_DECommon, m_configure);
    managerEU.setCountryID("1");

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::cn));
    VR_DataAccessorManager managerCN(m_asrRequestorMock, m_DECommon, m_configure);

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::gen));
    ON_CALL(*m_DECommon, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    m_manager = new VR_DataAccessorManager(m_asrRequestorMock, m_DECommon, m_configure);
}

void function(const std::string &req)
{
    return;
}

void VR_DataAccessorManager_Test::TearDownTestCase()
{
    delete m_manager;
    delete m_managerForVoiceTag;
    delete m_asrRequestorMock;
    delete m_DECommon;
    delete m_configure;
    m_manager = nullptr;
    m_managerForVoiceTag = nullptr;
    m_asrRequestorMock = nullptr;
    m_DECommon = nullptr;
    m_configure = nullptr;
    remove("/tmp/MusicCatalog001.db");
}

TEST_F(VR_DataAccessorManager_Test, getUpdateState)
{
    std::string stateMsg;
    m_manager->getUpdateState(stateMsg);
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, updateState)
{
    VoiceList<std::string>::type ruleList;
    // case 1
    // ruleList.push_back("grm_cmd_phone_contact#rul_slt_phone_contact_name_list_1");
    // EXPECT_CALL(*m_asrRequestorMock,
    //     setGrammarActive("ctx_phone_contact",
    //         true,
    //         ruleList)
    //     ).Times(1);
    // ruleList.clear();
    // ruleList.push_back("grm_cmd_phone_main#rul_slt_phone_contact_name_list_1");
    // EXPECT_CALL(*m_asrRequestorMock,
    //     setGrammarActive("ctx_phone_main",
    //         true,
    //         ruleList)
    //     ).Times(1);
    // ruleList.clear();
    // ruleList.push_back("grm_cmd_phone_main#rul_slt_phone_contact_name_list_1");
    // EXPECT_CALL(*m_asrRequestorMock,
    //     setGrammarActive("ctx_topmenu_main",
    //         true,
    //         ruleList)
    //     ).Times(1);

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
                "<person id=\"9\" first=\"Test\" last=\"TestLast\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</person>"
            "</category>"
        "</grammar_new>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"1\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"connected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"1\"/>"
        "</event>");
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"1\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"connected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"1\"/>"
        "</event>");
    // EXPECT_STREQ("", g_stateMsg.c_str());
    // EXPECT_STREQ("", g_msgToDM.c_str());

    // case 2
    ruleList.clear();
    // ruleList.push_back("grm_cmd_phone_contact#rul_slt_phone_contact_name_list_1");
    // EXPECT_CALL(*m_asrRequestorMock,
    //     setGrammarActive("ctx_phone_contact",
    //         false,
    //         ruleList)
    //     ).Times(1);
    // ruleList.clear();
    // ruleList.push_back("grm_cmd_phone_main#rul_slt_phone_contact_name_list_1");
    // EXPECT_CALL(*m_asrRequestorMock,
    //     setGrammarActive("ctx_phone_main",
    //         false,
    //         ruleList)
    //     ).Times(1);
    // ruleList.clear();
    // ruleList.push_back("grm_cmd_phone_main#rul_slt_phone_contact_name_list_1");
    // EXPECT_CALL(*m_asrRequestorMock,
    //     setGrammarActive("ctx_topmenu_main",
    //         false,
    //         ruleList)
    //     ).Times(1);
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"disconnected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"0\"/>"
        "</event>");
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"disconnected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"0\"/>"
        "</event>");
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"0\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"disconnected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"0\"/>"
        "</event>");
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"0\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"disconnected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"0\"/>"
        "</event>");
    // EXPECT_STREQ("", g_stateMsg.c_str());
    // EXPECT_STREQ("", g_msgToDM.c_str());

    // case 3
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"USB_1_CONNECTED\" value=\"true\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"USB_1_CONNECTED\" value=\"false\"/>"
            "<item key=\"USB_2_CONNECTED\" value=\"true\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"USB_2_CONNECTED\" value=\"false\"/>"
            "<item key=\"IPOD_1_CONNECTED\" value=\"true\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"IPOD_1_CONNECTED\" value=\"false\"/>"
            "<item key=\"IPOD_2_CONNECTED\" value=\"true\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"IPOD_2_CONNECTED\" value=\"false\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"IPOD_2_CONNECTED\" value=\"false\"/>"
        "</event>");

    m_manager->m_voiceTagContextID = "pengjialing";
//    m_manager->setState("PHONE_STATE_CONNECTED", "connected");
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"1\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"connected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"1\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"1\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"connected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"1\"/>"
        "</event>");

    m_manager->updateState(
        "<event key=\"PHONE_STATE_CONNECTED\" value=\"connected\">");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"disconnected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"0\"/>"
        "</event>");
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"disconnected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"0\"/>"
        "</event>");
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"0\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"disconnected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"0\"/>"
        "</event>");
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"0\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"disconnected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"0\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"USB_1_CONNECTED\" value=\"true\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"USB_1_CONNECTED\" value=\"false\"/>"
            "<item key=\"USB_2_CONNECTED\" value=\"true\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"USB_2_CONNECTED\" value=\"false\"/>"
            "<item key=\"IPOD_1_CONNECTED\" value=\"true\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"IPOD_1_CONNECTED\" value=\"false\"/>"
            "<item key=\"IPOD_2_CONNECTED\" value=\"true\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"IPOD_2_CONNECTED\" value=\"false\"/>"
        "</event>");

    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"IPOD_2_CONNECTED\" value=\"false\"/>"
        "</event>");

    m_manager->m_voiceTagContextID = "";

    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, updateGrammar)
{
    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"1000\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/NoDB/MusicCatalog001.db\" songitemcount=\"1000\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" otheritemcount=\"1000\" generalitemcount=\"14990\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"15990\" otheritemcount=\"1000\" generalitemcount=\"14990\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" otheritemcount=\"1000\" generalitemcount=\"15990\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"15990\" otheritemcount=\"1000\" generalitemcount=\"15990\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    m_manager->updateGrammar(
        "<grammar_init agent=\"phone\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"15990\" otheritemcount=\"1000\" generalitemcount=\"15990\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    m_manager->updateGrammar(
        "<grammar_init agent=\"other\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"15990\" otheritemcount=\"1000\" generalitemcount=\"15990\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::eu));
    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::cn));
    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\">"
        "</grammar_diff>");

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::vt));
    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::gen));
    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\">"
            "<category name=\"song\">"
                "<delete>"
                    "<item id=\"2\" name=\"lemon tree\"/>"
                    "<item id=\"3\" name=\"tomorrow\"/>"
                "</delete>"
                "<add>"
                    "<item id=\"5\" name=\"yesterday once more again(test)\" pronid=\"\" pron=\"\"/>"
                    "<item id=\"4\" name=\"show[something]\" pronid=\"\" pron=\"\"/>"
                "</add>"
            "</category>"
            "<category name=\"album\">"
                "<add>"
                    "<item id=\"3\" name=\"new classic\" pronid=\"\" pron=\"\"/>"
                "</add>"
            "</category>"
            "<category name=\"artist\">"
                "<delete>"
                    "<item id=\"1\" name=\"David\"/>"
                "</delete>"
            "</category>"
            "<category name=\"genre\">"
                "<add/>"
            "</category>"
        "</grammar_diff>");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"14990\" generalitemcount=\"14990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"15990\" generalitemcount=\"14990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"14990\" generalitemcount=\"15990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"15990\" generalitemcount=\"15990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"15990\" generalitemcount=\"15990\" reply=\"true\"/>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
            "</category>"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
                "<person id=\"5\" first=\"David\" last=\"Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</person>"
            "</category>"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"audiosource\">"
            "</category>"
        "</grammar_new>");

     m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"audiosource\">"
                "<formalnode name=\"CD\">"
                "</formalnode>"
            "</category>"
        "</grammar_new>");

     m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"audiosource\">"
                "<formalnode name=\"CD\"/>"
                "<formalnode name=\"xx\"/>"
            "</category>"
        "</grammar_new>");

    ON_CALL(*m_configure, getVRContry()).WillByDefault(Return(VR_REGION_CHINA));

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
                "<person id=\"5\" first=\"David\" last=\"Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</person>"
            "</category>"
        "</grammar_new>");

    ON_CALL(*m_configure, getVRContry()).WillByDefault(Return(VR_REGION_HK));

    m_managerForVoiceTag->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
                "<person id=\"5\" first=\"David\" last=\"Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</person>"
            "</category>"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"other\">"
            "<category name=\"other\">"
            "</category>"
            "<category name=\"song\">"
            "</category>"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" generalitemcount=\"14990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"15990\" generalitemcount=\"14990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" generalitemcount=\"15990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"15990\" generalitemcount=\"15990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"2\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"other\" grammarid=\"3\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"media\" grammarid=\"2\" />");

    m_manager->setState("UPDATING_SOURCE_ID", "0");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"media\" grammarid=\"1\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"phone\" path=\"abc\" />");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"phone\"/>");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"other\" reply=\"false\" grammarid=\"2\"/>");

    m_manager->onRecognizeBegin();

    m_manager->setState("ACTIVED_SOURCE_ID", "1");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\">"
            "<category name=\"song\">"
                "<delete>"
                    "<item id=\"2\" name=\"lemon tree\"/>"
                    "<item id=\"3\" name=\"tomorrow\"/>"
                "</delete>"
                "<add>"
                    "<item id=\"5\" name=\"yesterday once more again(test)\" pronid=\"\" pron=\"\"/>"
                    "<item id=\"4\" name=\"show[something]\" pronid=\"\" pron=\"\"/>"
                "</add>"
            "</category>"
            "<category name=\"album\">"
                "<add>"
                    "<item id=\"3\" name=\"new classic\" pronid=\"\" pron=\"\"/>"
                "</add>"
            "</category>"
            "<category name=\"artist\">"
                "<delete>"
                    "<item id=\"1\" name=\"David\"/>"
                "</delete>"
            "</category>"
            "<category name=\"genre\">"
                "<add/>"
            "</category>"
        "</grammar_diff>");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"14990\" generalitemcount=\"14990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"15990\" generalitemcount=\"14990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"14990\" generalitemcount=\"15990\" />");

    m_manager->updateGrammar(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"15990\" generalitemcount=\"15990\" />");

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
            "</category>"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
                "<person id=\"5\" first=\"David\" last=\"Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</person>"
            "</category>"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"audiosource\">"
            "</category>"
        "</grammar_new>");

     m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"audiosource\">"
                "<formalnode name=\"CD\">"
                "</formalnode>"
            "</category>"
        "</grammar_new>");

     m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"audiosource\">"
                "<formalnode name=\"CD\"/>"
                "<formalnode name=\"xx\"/>"
            "</category>"
        "</grammar_new>");

    ON_CALL(*m_configure, getVRContry()).WillByDefault(Return(VR_REGION_CHINA));

    m_manager->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
                "<person id=\"5\" first=\"David\" last=\"Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</person>"
            "</category>"
        "</grammar_new>");

    ON_CALL(*m_configure, getVRContry()).WillByDefault(Return(VR_REGION_HK));

    m_managerForVoiceTag->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
                "<person id=\"5\" first=\"David\" last=\"Smith\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</person>"
            "</category>"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_new agent=\"other\">"
            "<category name=\"other\">"
            "</category>"
            "<category name=\"song\">"
            "</category>"
        "</grammar_new>");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" generalitemcount=\"14990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"15990\" generalitemcount=\"14990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" generalitemcount=\"15990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"15990\" generalitemcount=\"15990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"2\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"other\" grammarid=\"3\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"media\" grammarid=\"2\" />");

    m_manager->setState("UPDATING_SOURCE_ID", "0");

    m_manager->updateGrammar(
        "<grammar_active agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"990\" generalitemcount=\"990\" />");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"media\" grammarid=\"1\" />");

    m_manager->updateGrammar(
        "<grammar_active agent=\"phone\" path=\"abc\" />");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"phone\"/>");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"other\" reply=\"false\" grammarid=\"2\"/>");

    m_manager->updateGrammar(
        "<grammar_disactive agent=\"other\" reply=\"true\" grammarid=\"2\"/>");

    m_manager->m_stateMap.insert(std::pair<std::string, std::string>("ACTIVED_SOURCE_ID", "2"));
    m_manager->m_bIsRecognizing = true;
    m_manager->updateGrammar(
        "<grammar_disactive agent=\"media\" reply=\"true\" grammarid=\"1\"/>");

    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, getInfo)
{
    std::string reqMsg;
    std::string response;

    m_manager->getInfo("_checkContactInPhonebook", reqMsg, response);
    reqMsg = "<data id=\"_checkContactInPhonebook\">"
                "<data>"
                  "<phoneInfo><contactId>25</contactId><phoneKind>2</phoneKind><contactName>marry eric</contactName><phoneType>0</phoneType><number>15151965820</number><timeStamp>20151126T142438</timeStamp><listType>0</listType><phoneTypeName>PHONE_MOBILE</phoneTypeName></phoneInfo>"
                "</data>"
              "</data>";
    m_manager->getInfo("_checkContactInPhonebook", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_getPhoneInfo", reqMsg, response);
    reqMsg = "<data id=\"_getPhoneInfo\">"
                "<data>"
                  "<queryType>NAME</queryType>"
                  "<phoneInfo>"
                    "<contactId>89</contactId>"
                    "<contactName>marry</contactName>"
                    "<phoneKind>''</phoneKind>"
                    "<phoneType>0</phoneType>"
                    "<phoneTypeName>''</phoneTypeName>"
                    "<number>''</number>"
                    "<listType>0</listType>"
                    "<timestamp>''</timestamp>"
                  "</phoneInfo>"
                "</data>"
              "</data>";
    m_manager->getInfo("_getPhoneInfo", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_getPlayLists", reqMsg, response);
    m_manager->getInfo("_getPlayLists", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_getPlaylistSongsNumber", reqMsg, response);
    m_manager->getInfo("_getPlaylistSongsNumber", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_getRadioStations", reqMsg, response);
    m_manager->getInfo("_getRadioStations", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_getMusicByType", reqMsg, response);
    m_manager->getInfo("_getMusicByType", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_selectSongs", reqMsg, response);
    m_manager->getInfo("_selectSongs", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_queryOneshotName", reqMsg, response);
    m_manager->getInfo("_queryOneshotName", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_queryOneshotDetail", reqMsg, response);
    m_manager->getInfo("_queryOneshotDetail", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_reqVoiceTag", reqMsg, response);
    m_manager->getInfo("_reqVoiceTag", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_reqVoiceTagDetail", reqMsg, response);
    m_manager->getInfo("_reqVoiceTagDetail", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_getVoiceTagNumber", reqMsg, response);
    m_manager->getInfo("_getVoiceTagNumber", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_addressMapping", reqMsg, response);
    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4720\">3000002</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"0\"/>"
                                    "<CMN2033_navi_addr_street_x confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4744\">3000009</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"0\"/>"
                                    "<CMN2033_navi_addr_street_x confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4474\">3000019</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"0\"/>"
                                    "<CMN2033_navi_addr_street_x confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    m_manager->getInfo("_addressMapping", reqMsg, response);
    reqMsg.clear();

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4720\">3000003</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"0\"/>"
                                    "<CMN2033_navi_addr_street_x confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4744\">3000002</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"0\"/>"
                                    "<CMN2033_navi_addr_street_x confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4474\">3000001</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"0\"/>"
                                    "<CMN2033_navi_addr_street_x confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_manager->getInfo("_addressMapping", reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4720\">3000003</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001013</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\"></CMN2033_navi_addr_street_x>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4744\">3000001</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001002</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\"></CMN2033_navi_addr_street_x>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4474\">3000003</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001017</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\"></CMN2033_navi_addr_street_x>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_manager->getInfo("_addressMapping", reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4720\">3000003</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001013</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\">8000335</CMN2033_navi_addr_street_x>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4744\">3000001</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001002</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\">8004474</CMN2033_navi_addr_street_x>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4474\">3000003</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001017</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\">8004471</CMN2033_navi_addr_street_x>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_manager->getInfo("_addressMapping", reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4720\">3000003</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001013</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\">8000335</CMN2033_navi_addr_street_x>"
                                    "<number>13</number>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4744\">3000001</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001002</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\">8004474</CMN2033_navi_addr_street_x>"
                                    "<number>7</number>"
                                "</item>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4474\">3000003</CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\">4001017</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\">8004471</CMN2033_navi_addr_street_x>"
                                    "<number>5</number>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_manager->getInfo("_addressMapping", reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>3</count>"
                            "</header>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_manager->getInfo("_addressMapping", reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_manager->getInfo("_addressMapping", reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"4720\"></CMN2031_navi_addr_state_x>"
                                    "<CMN2032_navi_addr_city_x confidence=\"5000\"></CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"5000\"></CMN2033_navi_addr_street_x>"
                                    "<number></number>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_manager->getInfo("_addressMapping", reqMsg, response);

    m_manager->getInfo("_buildAddress", reqMsg, response);
    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>CITY</buildType>"
                    "<addressInfo><zoneId>281474976710656</zoneId><country>''</country><zone>Western Cape</zone><cityId>''</cityId><city>''</city><streetExist>''</streetExist><streetId>''</streetId><street>''</street><houseNumberExist>''</houseNumberExist><houseNumber>''</houseNumber><zoneId>90283098779942912</zoneId><country>''</country><zone>Gauteng</zone><cityId>''</cityId><city>''</city><streetExist>''</streetExist><streetId>''</streetId><street>''</street><houseNumberExist>''</houseNumberExist><houseNumber>''</houseNumber><zoneId>90142361291587584</zoneId><country>''</country><zone>Eastern Cape</zone><cityId>''</cityId><city>''</city><streetExist>''</streetExist><streetId>''</streetId><street>''</street><houseNumberExist>''</houseNumberExist><houseNumber>''</houseNumber></addressInfo>"
                    "<asrResult>"
                        "<list id=\"asr12\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>1</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<CMN2031_navi_addr_state_x confidence=\"0\"/>"
                                    "<CMN2032_navi_addr_city_x confidence=\"4325\">4001002</CMN2032_navi_addr_city_x>"
                                    "<CMN2033_navi_addr_street_x confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    m_manager->getInfo("_buildAddress", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_sendVoiceID", reqMsg, response);
    reqMsg = "<data id=\"_sendVoiceID\">"
                "<data>"
                  "<type>STREET</type>"
                  "<info>"
                    "<addressInfo><zoneId>281474976710656</zoneId><country>''</country><zone>Western Cape</zone><cityId>285778533941248</cityId><city>City of Cape Town</city><streetExist>true</streetExist><streetId>''</streetId><street>''</street><houseNumberExist>''</houseNumberExist><houseNumber>''</houseNumber></addressInfo> "
                  "</info>"
                "</data>"
              "</data>";
    m_manager->getInfo("_sendVoiceID", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("_buildAddressName", reqMsg, response);
    reqMsg = "<data id=\"_buildAddressName\">"
            "<data>"
              "<address><addressInfo>"
            "<zoneId>90705311245008896</zoneId>"
            "<country>''</country>"
            "<zone>Western Cape</zone>"
            "<cityId>90705315539976192</cityId>"
            "<city>City of Cape Town</city>"
            "<streetExist>true</streetExist>"
            "<streetId>638016</streetId>"
            "<street>Alice Street</street>"
            "<houseNumberExist>true</houseNumberExist>"
            "<houseNumber/>"
        "</addressInfo><guidePoint>"
            "<name>Western Cape City of Cape Town Alice Street </name>"
            "<displayGuideType>''</displayGuideType>"
            "<linkId>0</linkId>"
            "<longitude>17094090</longitude>"
            "<latitude>-31248610</latitude>"
            "<list id=\"list_queryAddressInfo\">"
                "<header>"
                    "<pageSize>''</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>1</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<guideType>''</guideType>"
                        "<guideLongitude>17094090</guideLongitude>"
                        "<guideLatitude>-31248610</guideLatitude>"
                    "</item>"
                "</items>"
            "</list>"
        "</guidePoint></address>"
            "</data>"
          "</data>";
    m_manager->getInfo("_buildAddressName", reqMsg, response);
    reqMsg.clear();

    m_manager->getInfo("otheroperation", reqMsg, response);
    reqMsg.clear();

    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, handleMusicResult)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"One\" confidence=\"7772\">"
                "abc"
            "</node>"
        "</result>");
    pugi::xml_node result = testDoc.child("result");
    m_manager->handleMusicResult(result, "");
    testDoc.load_string(
        "<result>"
            "<node name=\"intent\" value=\"phone_dial_name_shortcut_PHRASE\" sentenceValue=\"call Alice\" confidence=\"6916\">"
                "<list id=\"asr7\">"
                    "<header>"
                        "<pageSize>0</pageSize>"
                        "<startIndex>0</startIndex>"
                        "<count>3</count>"
                    "</header>"
                    "<items>"
                        "<item>"
                            "<contact_name confidence=\"7094\">Alice</contact_name>"
                            "<contact_id confidence=\"9999\">136</contact_id>"
                            "<contact_phone_type confidence=\"0\"></contact_phone_type>"
                        "</item>"
                        "<item>"
                            "<contact_name confidence=\"7094\">Alice</contact_name>"
                            "<contact_id confidence=\"9999\">136</contact_id>"
                            "<contact_phone_type confidence=\"0\"></contact_phone_type>"
                        "</item>"
                        "<item>"
                            "<contact_name confidence=\"7094\">Alice</contact_name>"
                            "<contact_id confidence=\"9999\">137</contact_id>"
                            "<contact_phone_type confidence=\"0\"></contact_phone_type>"
                        "</item>"
                        "<item>"
                            "<contact_name confidence=\"5580\">Alice Test</contact_name>"
                            "<contact_id confidence=\"9999\">141</contact_id>"
                            "<contact_phone_type confidence=\"0\"></contact_phone_type>"
                        "</item>"
                    "</items>"
                "</list>"
            "</node>"
        "</result>");
    result = testDoc.child("result");
    m_manager->handleMusicResult(result, "contact");
    testDoc.load_string(
        "<result>"
            "<node name=\"intent\" value=\"phone_dial_name_shortcut_PHRASE\" sentenceValue=\"call Alice\" confidence=\"6916\">"
                "<items>"
                    "<item>"
                        "<contact_name confidence=\"7094\">Alice</contact_name>"
                        "<contact_id confidence=\"9999\">136</contact_id>"
                        "<contact_phone_type confidence=\"0\"></contact_phone_type>"
                    "</item>"
                "</items>"
            "</node>"
        "</result>");
    result = testDoc.child("result");
    m_manager->m_accessorMedia = NULL;
    m_manager->handleMusicResult(result, "contact");
    m_manager = new VR_DataAccessorManager(m_asrRequestorMock, m_DECommon, m_configure);
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, isRecognizing)
{
    m_manager->onRecognizeBegin();
    EXPECT_TRUE(m_manager->isRecognizing());
    m_manager->onRecognizeEnd();
    EXPECT_FALSE(m_manager->isRecognizing());
}

TEST_F(VR_DataAccessorManager_Test, onRecognizeEnd)
{
    m_manager->onRecognizeBegin();
    m_manager->updateGrammar(
        "<grammar_disactive agent=\"media\" reply=\"true\" grammarid=\"1\"/>");
    m_manager->onRecognizeEnd();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, onUpdateGrammarCategoryFinish)
{
    m_manager->onUpdateGrammarCategoryFinish("music");
    m_manager->onUpdateGrammarCategoryFinish("contact");
    m_manager->onUpdateGrammarCategoryFinish("genre");
    m_manager->onUpdateGrammarCategoryFinish("album");
    m_manager->onUpdateGrammarCategoryFinish("artist");
    m_manager->onUpdateGrammarCategoryFinish("playlist");
    m_manager->onUpdateGrammarCategoryFinish("song");
    m_manager->onUpdateGrammarCategoryFinish("other");
    m_manager->m_stateMap["UPDATING_SOURCE_ID"] = "usb1";
    m_manager->m_stateMap["ACTIVED_SOURCE_ID"] = "usb1";
    m_manager->m_updatingDictionaryStateMap["music"] = "DICTIONARY_OK";
    m_manager->onUpdateGrammarCategoryFinish("music");
    m_manager->onUpdateGrammarCategoryFinish("contact");
    m_manager->onUpdateGrammarCategoryFinish("genre");
    m_manager->onUpdateGrammarCategoryFinish("album");
    m_manager->onUpdateGrammarCategoryFinish("artist");
    m_manager->onUpdateGrammarCategoryFinish("playlist");
    m_manager->onUpdateGrammarCategoryFinish("song");
    m_manager->onUpdateGrammarCategoryFinish("other");
    m_manager->m_notifyCallback = &function;
    m_manager->onUpdateGrammarCategoryFinish("contact");
    m_manager->m_stateMap["ACTIVED_SOURCE_ID"] = "usb2";
    m_manager->onUpdateGrammarCategoryFinish("song");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, onUpdateGrammarFinish)
{
    m_manager->onUpdateGrammarFinish();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, triggerGenerateGrammar)
{
    m_manager->m_bIsRecognizing = false;
    pugi::sp_xml_document testDoc(new pugi::xml_document());
    testDoc->load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"One\" confidence=\"7772\">"
                "abc"
            "</node>"
        "</result>");
    m_manager->m_generatingGrammarMsg = testDoc;
    m_manager->restartGenerateGrammar();
    m_manager->m_generateGrammarEnable = true;
    m_manager->triggerGenerateGrammar();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, handleGrammarGenerateMsg)
{
    m_manager->m_bIsRecognizing = false;
    pugi::sp_xml_document testDoc(new pugi::xml_document());
    testDoc->load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"One\" confidence=\"7772\">"
                "abc"
            "</node>"
        "</result>");
    m_manager->m_generateGrammarEnable = true;
    m_manager->handleGrammarGenerateMsg(testDoc);
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, generateGrammar)
{
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::eu));
    pugi::sp_xml_document testDoc(new pugi::xml_document());
    testDoc->load_string(
        "<grammar_init agent=\"phone\" path=\"VR_PhoneBookContacts_DE.db\"></grammar_init>");
    m_manager->generateGrammar(testDoc);
    testDoc->load_string(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");
    m_manager->generateGrammar(testDoc);
    testDoc->load_string(
        "<grammar_init agent=\"navi\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"14990\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");
    m_manager->generateGrammar(testDoc);
    testDoc->load_string(
        "<grammar_init agent=\"navi\" grammarid=\"1\" path=\"/tmMusicCatalog001\" songitemcount=\"14990\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");
    m_manager->generateGrammar(testDoc);
    testDoc->load_string(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"15990\" generalitemcount=\"15990\" />");
    m_manager->generateGrammar(testDoc);
    m_manager->m_musicCategoryList.push_back("album");
    m_manager->m_musicCategoryList.push_back("artist");
    testDoc->load_string(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"15990\" generalitemcount=\"15990\" />");
    m_manager->generateGrammar(testDoc);
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::cn));
    m_manager->generateGrammar(testDoc);
    testDoc->load_string(
        "<grammar_new agent=\"other\">"
            "<category name=\"other\">"
            "</category>"
            "<category name=\"song\">"
            "</category>"
        "</grammar_new>");
    m_manager->generateGrammar(testDoc);
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, handleMediaGrammarDiff)
{
    m_manager->m_musicCategoryList.push_back("song");
    m_manager->m_musicCategoryList.push_back("genre");
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<grammar_diff agent=\"media\" grammarid=\"1\" songitemcount=\"15990\" generalitemcount=\"15990\" />");
    pugi::xml_node category = testDoc.child("grammar_diff");
    m_manager->handleMediaGrammarDiff(category, "1", 1000, 1000);
    testDoc.load_string(
        "<category name=\"song\">"
            "<delete>"
                "<item id=\"2\" name=\"lemon tree\"/>"
                "<item id=\"3\" name=\"tomorrow\"/>"
            "</delete>"
            "<add>"
                "<item id=\"5\" name=\"yesterday once more again(test)\" pronid=\"\" pron=\"\"/>"
                "<item id=\"4\" name=\"show[something]\" pronid=\"\" pron=\"\"/>"
            "</add>"
        "</category>");
    category = testDoc.child("category");
    m_manager->handleMediaGrammarDiff(category, "1", 1000, 1000);
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, handleGrammarNew)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<category name=\"audiosource\">"
            "<formal id=\"12\" name=\"AM\">"
                "<alias name=\"AM\" tts=\"AM\" />"
                "<alias name=\"AM Radio\" tts=\"AM\" />"
            "</formal>"
            "<formal id=\"11\" name=\"FM\">"
                "<alias name=\"FM\" tts=\"FM\" />"
                "<alias name=\"FM Radio\" tts=\"FM\" />"
                "<alias name=\"Radio\" tts=\"FM\" />"
            "</formal>"
        "</category>");
    pugi::xml_node category = testDoc.child("category");
    m_manager->handleGrammarNew(category);
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, handleMusicActive)
{
    m_manager->handleMusicActive(true, "1", 1000, 1000, "/tmp/MusicCatalog001.db", true);
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, selectGrammarType)
{
    EXPECT_STREQ("title", m_manager->selectGrammarType("song").c_str());
    EXPECT_STREQ("normal", m_manager->selectGrammarType("playlist").c_str());
    EXPECT_STREQ("artist", m_manager->selectGrammarType("artist").c_str());
    EXPECT_STREQ("album", m_manager->selectGrammarType("album").c_str());
    EXPECT_STREQ("normal", m_manager->selectGrammarType("genre").c_str());
    EXPECT_STREQ("normal", m_manager->selectGrammarType("other").c_str());
}

TEST_F(VR_DataAccessorManager_Test, saveVoiceTagGrammar)
{
    m_manager->saveVoiceTagGrammar("0", "david", "dsfsekfowe", "1");
    m_managerForVoiceTag->saveVoiceTagGrammar("0", "david", "dsfsekfowe", "1");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, deleteVoiceTagGrammar)
{
    VoiceList<std::pair<std::string, std::string>>::type deleteList;
    deleteList.push_back(std::make_pair("0", "david"));
    deleteList.push_back(std::make_pair("1", "tom"));

    m_manager->deleteVoiceTagGrammar(deleteList, "1");
    m_managerForVoiceTag->deleteVoiceTagGrammar(deleteList, "1");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, deleteAllVoiceTagGrammar)
{
    m_manager->deleteAllVoiceTagGrammar("1");
    m_managerForVoiceTag->deleteAllVoiceTagGrammar("1");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, clearVoiceTagGrammar)
{
    m_manager->clearVoiceTagGrammar();
    m_managerForVoiceTag->clearVoiceTagGrammar();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, genCTermListCallback)
{
    VoiceList<spC_Term>::type temp;
    VoiceList<spC_Term>::type* addList = &temp;
    char* columnValue[] = { "1", "abc" };
    char* columnName[] = { "", "" };
    m_manager->genCTermListCallback(addList, 1, columnValue, columnName);
    columnValue[0] = nullptr;
    m_manager->genCTermListCallback(addList, 1, columnName, columnName);
    columnValue[0] = "1";
    columnValue[1] = nullptr;
    m_manager->genCTermListCallback(addList, 1, columnName, columnName);
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, voiceTagUpdateGrammarCallback)
{
    C_Event_Phase_mock phaseEvent;
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_None));
    m_manager->voiceTagUpdateGrammarCallback(phaseEvent, "1");
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End));
    m_manager->voiceTagUpdateGrammarCallback(phaseEvent, "1");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, voiceTagDeleteGrammarCallback)
{
    C_Event_Phase_mock phaseEvent;
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_Begin));
    m_manager->voiceTagDeleteGrammarCallback(phaseEvent, "1");
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End));
    m_manager->voiceTagDeleteGrammarCallback(phaseEvent, "1");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, updateGrammarCallback)
{
    C_Event_Phase_mock phaseEvent;
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_Begin));
    m_manager->updateGrammarCallback(phaseEvent, "song");
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End));
    m_manager->updateGrammarCallback(phaseEvent, "song");
    m_manager->updateGrammarCallback(phaseEvent, "contact");

    boost::function<void(const std::string &)> tmpCallback = NULL;
    m_manager->setNotifyCallback(tmpCallback);
    m_manager->updateGrammarCallback(phaseEvent, "song");
    tmpCallback = &updateStateCallback;
    m_manager->setNotifyCallback(tmpCallback);

    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_None));
    m_manager->updateGrammarCallback(phaseEvent, "song");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, getSingleColumnList)
{
    VoiceList<std::string>::type list;
    char * columnValue[1] = { nullptr };
    char temp[2] = { 'a', '\0' };
    m_manager->getSingleColumnList(&list, 0, columnValue, nullptr);
    EXPECT_TRUE(list.empty());
    columnValue[0] = &temp[0];
    m_manager->getSingleColumnList(&list, 0, columnValue, nullptr);
    EXPECT_FALSE(list.empty());
}

TEST_F(VR_DataAccessorManager_Test, getSingleColumnValue)
{
    std::string value;
    char * columnValue[1] = { nullptr };
    char temp[2] = { 'a', '\0' };
    m_manager->getSingleColumnValue(&value, 0, columnValue, nullptr);
    EXPECT_TRUE(value.empty());
    columnValue[0] = &temp[0];
    m_manager->getSingleColumnValue(&value, 0, columnValue, nullptr);
    EXPECT_FALSE(value.empty());
}

// getCTermList

// TEST_F(VR_DataAccessorManager_Test, insertRecord)
// {
//     m_manager->insertRecord("contacts", "27", "abc", "xyz");
//     m_manager->insertRecord("contacts", "28", "abc", "");
//     SUCCEED();
// }

TEST_F(VR_DataAccessorManager_Test, notifyStateUpdated)
{
    m_manager->notifyStateUpdated();
    m_manager->setState("MUSIC_DICTIONARY_STATE", "DICTIONARY_SYNCING");
    m_manager->setState("GENRE_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->setState("ARTIST_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->setState("ALBUM_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->setState("PLAYLIST_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->setState("SONG_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->m_isMusicGrammarDroped = true;
    // m_manager->notifyStateUpdated(true);
    m_manager->notifyStateUpdated();

    m_manager->setState("MUSIC_DICTIONARY_STATE", "DICTIONARY_SYNCING");
    m_manager->m_isMusicGrammarDroped = false;
    // m_manager->notifyStateUpdated(true);
    m_manager->notifyStateUpdated();

    m_manager->setState("GENRE_DICTIONARY_STATE", "DICTIONARY_SYNCING");
    // m_manager->notifyStateUpdated(true);
    m_manager->notifyStateUpdated();

    m_manager->setState("GENRE_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->setState("ARTIST_DICTIONARY_STATE", "DICTIONARY_SYNCING");
    // m_manager->notifyStateUpdated(true);
    m_manager->notifyStateUpdated();

    m_manager->setState("ARTIST_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->setState("ALBUM_DICTIONARY_STATE", "DICTIONARY_SYNCING");
    // m_manager->notifyStateUpdated(true);
    m_manager->notifyStateUpdated();

    m_manager->setState("ALBUM_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->setState("PLAYLIST_DICTIONARY_STATE", "DICTIONARY_SYNCING");
    // m_manager->notifyStateUpdated(true);
    m_manager->notifyStateUpdated();

    m_manager->setState("PLAYLIST_DICTIONARY_STATE", "DICTIONARY_OK");
    m_manager->setState("SONG_DICTIONARY_STATE", "DICTIONARY_SYNCING");
    // m_manager->notifyStateUpdated(true);
    m_manager->notifyStateUpdated();


    m_managerForVoiceTag->notifyStateUpdated();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, checkMusicGrammarState)
{
    m_manager->m_requestUpdateMediaGrammarFinish = true;
    m_manager->checkMusicGrammarState();
    m_manager->m_updatingDictionaryStateMap["music"] = "DICTIONARY_SYNCING";
    m_manager->m_updatingDictionaryStateMap["genre"] = "genre";
    m_manager->m_updatingDictionaryStateMap["artist"] = "artist";
    m_manager->m_updatingDictionaryStateMap["album"] = "album";
    m_manager->m_updatingDictionaryStateMap["playlist"] = "playlist";
    m_manager->m_updatingDictionaryStateMap["song"] = "song";
    m_manager->m_isMusicGrammarDroped = true;
    m_manager->checkMusicGrammarState();
    m_manager->m_isMusicGrammarDroped = false;
    m_manager->checkMusicGrammarState();
    m_manager->m_updatingDictionaryStateMap["song"] = "DICTIONARY_SYNCING";
    m_manager->checkMusicGrammarState();
    m_manager->m_updatingDictionaryStateMap["playlist"] = "DICTIONARY_SYNCING";
    m_manager->checkMusicGrammarState();
    m_manager->m_updatingDictionaryStateMap["album"] = "DICTIONARY_SYNCING";
    m_manager->checkMusicGrammarState();
    m_manager->m_updatingDictionaryStateMap["artist"] = "DICTIONARY_SYNCING";
    m_manager->checkMusicGrammarState();
    m_manager->m_updatingDictionaryStateMap["genre"] = "DICTIONARY_SYNCING";
    m_manager->checkMusicGrammarState();
    m_manager->m_updatingDictionaryStateMap["music"] = "music";
    m_manager->checkMusicGrammarState();
    m_manager->m_updatingDictionaryStateMap["music"] = "DICTIONARY_SYNCING";
    m_manager->m_updatingDictionaryStateMap["genre"] = "genre";
    m_manager->m_updatingDictionaryStateMap["artist"] = "artist";
    m_manager->m_updatingDictionaryStateMap["album"] = "album";
    m_manager->m_updatingDictionaryStateMap["playlist"] = "playlist";
    m_manager->m_updatingDictionaryStateMap["song"] = "song";
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, initializeState)
{
    ON_CALL(*m_DECommon, isGetFromData()).WillByDefault(Return(true));
    ON_CALL(*m_configure, getDataPath()).WillByDefault(Return("/vdata/.VDATA/VDATA/VR/SUNTEC/settings/"));

    m_manager->initializeState();

    ON_CALL(*m_DECommon, isGetFromData()).WillByDefault(Return(false));
    ON_CALL(*m_configure, getDataPath()).WillByDefault(Return("/tmp/"));

    m_manager->initializeState();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, updateMusicGrammarState)
{
    m_manager->m_updatingDictionaryStateMap["music"] = "DICTIONARY_OK";
    m_manager->updateMusicGrammarState("music", "0");
    m_manager->m_updatingDictionaryStateMap["music"] = "DICTIONARY_SYNCING";
    m_manager->updateMusicGrammarState("music", "0");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, checkGrammarNeedUpdate)
{
    pugi::sp_xml_document testDoc(new pugi::xml_document());
    testDoc->load_string(
            "<grammar_init agent=\"media\" grammarid=\"0\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"1000\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");
    m_manager->m_generateGrammarList.push_back(testDoc);
    m_manager->checkGrammarNeedUpdate();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, resetGrammarGeneration)
{
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::vt));
    m_manager->resetGrammarGeneration();
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::cn));
    m_manager->resetGrammarGeneration();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, restartGenerateGrammar)
{
    m_manager->m_generatingGrammarMsg = NULL;
    m_manager->restartGenerateGrammar();
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, checkContextIDAvailable)
{
    EXPECT_TRUE(m_manager->checkContextIDAvailable("ctx_tuner_station_name_list"));
    EXPECT_FALSE(m_manager->checkContextIDAvailable("list"));

}

TEST_F(VR_DataAccessorManager_Test, getContextIDAgent)
{
    EXPECT_STREQ("abc", m_manager->getContextIDAgent("_abc_").c_str());
    EXPECT_STREQ("voicetag", m_manager->getContextIDAgent("_voice_").c_str());
}

TEST_F(VR_DataAccessorManager_Test, getContextIDIndex)
{
    EXPECT_STREQ("abc", m_manager->getContextIDIndex("genera_abc").c_str());
}

TEST_F(VR_DataAccessorManager_Test, setGenerateGrammarEnable)
{
    m_manager->setGenerateGrammarEnable(true);
}

TEST_F(VR_DataAccessorManager_Test, generateVoiceTagGrammar)
{
    std::string device = "shanghai";
    EXPECT_FALSE(m_manager->generateVoiceTagGrammar(device));
    m_manager->m_voiceTagContextID = "ctx_tuner_station_name_list";
    device = "1";
    m_manager->generateVoiceTagGrammar(device);
    device = "2";
    m_manager->generateVoiceTagGrammar(device);
    device = "3";
    m_manager->generateVoiceTagGrammar(device);
    device = "4";
    m_manager->generateVoiceTagGrammar(device);
    device = "5";
    m_manager->generateVoiceTagGrammar(device);
}

TEST_F(VR_DataAccessorManager_Test, removeGrammarDiffMsg)
{
    m_manager->removeGrammarDiffMsg("media", "0");
    m_manager->removeGrammarDiffMsg("music", "1");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, updateOtherGrammarState)
{
    m_manager->updateOtherGrammarState("contact");
    SUCCEED();
}

TEST_F(VR_DataAccessorManager_Test, getFullName)
{
    std::string fullname;
    m_manager->getFullName(fullname, "", "", true);
    EXPECT_STREQ("", fullname.c_str());

    m_manager->getFullName(fullname, "first", "", true);
    EXPECT_STREQ("first", fullname.c_str());

    m_manager->getFullName(fullname, "", "last", true);
    EXPECT_STREQ("last", fullname.c_str());

    m_manager->getFullName(fullname, "first", "last", true);
    EXPECT_STREQ("first last", fullname.c_str());

    m_manager->getFullName(fullname, "first", "last", false);
    EXPECT_STREQ("lastfirst", fullname.c_str());
}

TEST_F(VR_DataAccessorManager_Test, trim)
{
    std::string test("");
    EXPECT_STREQ("", m_manager->trim(test).c_str());
    test.assign(" abc ");
    EXPECT_STREQ("abc", m_manager->trim(test).c_str());
}

TEST_F(VR_DataAccessorManager_Test, getState)
{
    EXPECT_TRUE(m_manager->getState("other").empty());
    m_manager->setState("UPDATING_SOURCE_ID", "1");
    EXPECT_STREQ("1", m_manager->getState("UPDATING_SOURCE_ID").c_str());
}

TEST_F(VR_DataAccessorManager_Test, setState)
{
    m_manager->setState("", "testValue");
    m_manager->setState("testKey", "testValue");
    EXPECT_STREQ("testValue", m_manager->getState("testKey").c_str());
}

TEST_F(VR_DataAccessorManager_Test, clearAllGrammar)
{
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::vt));
    m_manager->clearAllGrammar();
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::gen));
    m_manager->clearAllGrammar();
}

TEST_F(VR_DataAccessorManager_Test, clearGrammarCallback)
{
    C_Event_Phase_mock phaseEvent;
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_Begin));
    m_manager->clearGrammarCallback(phaseEvent);
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End));
    m_manager->clearGrammarCallback(phaseEvent);
    ON_CALL(phaseEvent, Get_Event_Phase_Type()).WillByDefault(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_None));
    m_manager->clearGrammarCallback(phaseEvent);
}

/* EOF */
