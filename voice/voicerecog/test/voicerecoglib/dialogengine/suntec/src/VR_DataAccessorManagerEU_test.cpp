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

#include "VR_DataAccessorManagerEU.h"
#include "VR_DataAccessorMediaEU.h"
#include "Vr_Asr_Engine.h"

#include "VR_AsrRequestor_mock.h"
#include "VR_DECommon_mock.h"
#include "VR_Configure_mock.h"
#include "Vr_Asr_Event_Phase_mock.h"

#include <sqlite3.h>
#include "gtest/gtest.h"

using ::testing::Return;

/**
* VR_DataAccessorManagerEU_Test
*
* The class is just for VR_DataAccessorManagerEU test.
*/
class VR_DataAccessorManagerEU_Test : public testing::Test
{
public:
    VR_DataAccessorManagerEU_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

    static VR_DataAccessorManagerEU *m_managerEU;
    static VR_AsrRequestor_mock *m_asrRequestorMock;
    static VR_DECommon_mock *m_DECommon;
    static VR_Configure_mock *m_configure;
private:
    VR_DataAccessorManagerEU_Test(const VR_DataAccessorManagerEU_Test& other);
    void operator= (const VR_DataAccessorManagerEU_Test& other);
};

std::string g_msgToDMEU;
void updateStateEUCallback(const std::string &msgToDM)
{
    g_msgToDMEU = msgToDM;
}

VR_DataAccessorManagerEU *VR_DataAccessorManagerEU_Test::m_managerEU;
VR_AsrRequestor_mock *VR_DataAccessorManagerEU_Test::m_asrRequestorMock;
VR_DECommon_mock *VR_DataAccessorManagerEU_Test::m_DECommon;
VR_Configure_mock *VR_DataAccessorManagerEU_Test::m_configure;

// common test
// Test for EU
// Test for CN
// Test for general
// Test for VoiceTag

void VR_DataAccessorManagerEU_Test::SetUpTestCase()
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
        int result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);
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

    m_managerEU = new VR_DataAccessorManagerEU(m_asrRequestorMock, m_DECommon, m_configure);
    boost::function<void(const std::string &)> callback = &updateStateEUCallback;
    m_managerEU->setUpdateStateCallback(callback);
    m_managerEU->setNotifyCallback(callback);
    m_managerEU->setCountryID("1");

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::vt));
    ON_CALL(*m_configure, getUsrPath()).WillByDefault(Return("/tmp/VR_DataAccessorTest/NoDB/"));

    ON_CALL(*m_configure, getUsrPath()).WillByDefault(Return("/tmp/VR_DataAccessorTest"));
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::eu));
    VR_DataAccessorManagerEU managerEU(m_asrRequestorMock, m_DECommon, m_configure);

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::cn));
    VR_DataAccessorManagerEU managerCN(m_asrRequestorMock, m_DECommon, m_configure);

    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::gen));
}

void VR_DataAccessorManagerEU_Test::TearDownTestCase()
{
    delete m_managerEU;
    delete m_asrRequestorMock;
    delete m_DECommon;
    delete m_configure;
    m_managerEU = nullptr;
    m_asrRequestorMock = nullptr;
    m_DECommon = nullptr;
    m_configure = nullptr;
    remove("/tmp/MusicCatalog001.db");
}

TEST_F(VR_DataAccessorManagerEU_Test, getStationBand)
{
    EXPECT_STREQ("DAB", m_managerEU->getStationBand(500).c_str());
    EXPECT_STREQ("FM", m_managerEU->getStationBand(1500).c_str());
}

TEST_F(VR_DataAccessorManagerEU_Test, getStationID)
{
    EXPECT_EQ(500, m_managerEU->getStationID(1500));
    EXPECT_EQ(500, m_managerEU->getStationID(500));
}

TEST_F(VR_DataAccessorManagerEU_Test, getStationGrammarID)
{
    EXPECT_EQ(1500, m_managerEU->getStationGrammarID(500, VR_StationType::FM));
    EXPECT_EQ(500, m_managerEU->getStationGrammarID(500, VR_StationType::DAB));
}

TEST_F(VR_DataAccessorManagerEU_Test, updateOtherGrammarState)
{
    m_managerEU->updateOtherGrammarState("contact");
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerEU_Test, setMusicGrammarActive)
{
    m_managerEU->setMusicGrammarActive("1", true, 10000, 10000);
    m_managerEU->setMusicGrammarActive("1", false, 10000, 10000);
    m_managerEU->setMusicGrammarActive("1", true, 10000, 100);
    m_managerEU->setMusicGrammarActive("1", true, 20000, 100);
    m_managerEU->setMusicGrammarActive("1", false, 10000, 100);
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerEU_Test, setPhoneContactGrammarActive)
{
    m_managerEU->setPhoneContactGrammarActive(true);
    m_managerEU->setPhoneContactGrammarActive(false);
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerEU_Test, checkPhoneTypeAvailable)
{
    m_managerEU->checkPhoneTypeAvailable(1);
    m_managerEU->m_accessorContact = NULL;
    m_managerEU->checkPhoneTypeAvailable(1);
    m_managerEU = new VR_DataAccessorManagerEU(m_asrRequestorMock, m_DECommon, m_configure);
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerEU_Test, handleMusicOneshotResult)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<data>"
            "<result>One</result>"
        "</data>");
    pugi::xml_node result = testDoc.child("result");
    m_managerEU->handleMusicOneshotResult(result);
    m_managerEU->m_accessorMedia = NULL;
    m_managerEU->handleMusicOneshotResult(result);
    m_managerEU = new VR_DataAccessorManagerEU(m_asrRequestorMock, m_DECommon, m_configure);
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerEU_Test, processRadioGrammarNew)
{
    VoiceList<spC_Term>::type addList;
    m_managerEU->processRadioGrammarNew(addList);
    m_managerEU->m_accessorMediaEU->m_radioFMStationMap.insert(std::pair<int, std::string>(1, "aaa"));
    m_managerEU->m_accessorMediaEU->m_radioDABStationMap.insert(std::pair<int, std::string>(2, "bbb"));
    m_managerEU->processRadioGrammarNew(addList);
    m_managerEU->m_accessorMedia = NULL;
    m_managerEU->processRadioGrammarNew(addList);
    m_managerEU = new VR_DataAccessorManagerEU(m_asrRequestorMock, m_DECommon, m_configure);
    SUCCEED();    
}

TEST_F(VR_DataAccessorManagerEU_Test, handleGrammarNew)
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
                "<formal id=\"21\" name=\"CD\">"
                    "<alias name=\"CD\" tts=\"CD\" />"
                    "<alias name=\"CD Player\" tts=\"CD\" />"
                    "<alias name=\"CD Changer\" tts=\"CD\" />"
                    "<alias name=\"Disc\" tts=\"CD\" />"
                    "<alias name=\"MP3 CD\" tts=\"CD\" />"
                "</formal>"
                "<formal id=\"3\" name=\"USB\">"
                    "<alias name=\"USB\" tts=\"USB\" />"
                    "<alias name=\"USB Audio\" tts=\"USB\" />"
                    "<alias name=\"USB 1\" tts=\"USB\" />"
                    "<alias name=\"MP3 Player\" tts=\"USB\" />"
                    "<alias name=\"MP3 Player 1\" tts=\"USB\" />"
                    "<alias name=\"MP3 1\" tts=\"USB\" />"
                    "<alias name=\"MP3\" tts=\"USB\" />"
                    "<alias name=\"USB1\" tts=\"USB\" />"
                "</formal>"
                "<formal id=\"5\" name=\"Bluetooth Audio\">"
                    "<alias name=\"Bluetooth\" tts=\"Bluetooth Audio\" />"
                    "<alias name=\"Bluetooth Audio\" tts=\"Bluetooth Audio\" />"
                    "<alias name=\"BT Audio\" tts=\"Bluetooth Audio\" />"
                    "<alias name=\"BTA\" tts=\"Bluetooth Audio\" />"
                "</formal>"
        "</category>");
    pugi::xml_node category = testDoc.child("category");
    m_managerEU->handleGrammarNew(category);

    testDoc.load_string(
        "<category name=\"fmstation\">"
                "<formal id=\"1\" name=\"one\" />"
        "</category>");
    category = testDoc.child("category");
    m_managerEU->handleGrammarNew(category);

    testDoc.load_string(
        "<category name=\"dabstation\">"
                "<formal id=\"1\" name=\"one\" />"
        "</category>");
    category = testDoc.child("category");
    m_managerEU->handleGrammarNew(category);

    testDoc.load_string(
        "<category name=\"song\">"
                "<formal id=\"1\" name=\"one\" />"
        "</category>");
    category = testDoc.child("category");
    m_managerEU->handleGrammarNew(category);
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerEU_Test, genMusicCTermListCallbackEU)
{
    VoiceList<spC_Term>::type temp;
    VoiceList<spC_Term>::type* addList = &temp;
    char* columnValue[] = { "1", "a", "2", "b" };
    char* columnName[] = { "", "" };
    m_managerEU->genMusicCTermListCallbackEU(addList, 1, columnValue, columnName);
    columnValue[0] = nullptr;
    columnValue[1] = nullptr;
    m_managerEU->genMusicCTermListCallbackEU(addList, 1, columnValue, columnName);
    columnValue[2] = nullptr;
    columnValue[3] = nullptr;
    m_managerEU->genMusicCTermListCallbackEU(addList, 1, columnValue, columnName);
    SUCCEED();

}

TEST_F(VR_DataAccessorManagerEU_Test, updateGrammar)
{
    VoiceList<std::string>::type ruleList;
    m_managerEU->updateGrammar(
        "<grammar_new agent=\"phone\">"
            "<category name=\"contact\">"
                "<person id=\"9\" first=\"Test\" last=\"TestLast\">"
                    "<phone_item type=\"2\" number=\"18845678912\"/>"
                    "<phone_item type=\"3\" number=\"13645678912\"/>"
                    "<phone_item type=\"1\" number=\"021-45678901\"/>"
                "</person>"
            "</category>"
        "</grammar_new>");
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerEU_Test, saveRadioData)
{
    VoiceMap<int, std::string>::type radioMap;
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<item key=\"FMSTATION_DICTIONARY_STATE\" value=\"DICTIONARY_NONE\" />");
    pugi::xml_node item = testDoc.child("item");    
    m_managerEU->saveRadioData(item, radioMap);

}

TEST_F(VR_DataAccessorManagerEU_Test, handleAsrMusicStepByStepResult)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<data>"
            "<item>"
                "<id>\"1\"</id>"
                "<name>\"one\"</name>"
            "</item>"
        "</data>");
    pugi_vr::xml_node result = testDoc.child("item");
    m_managerEU->handleAsrMusicStepByStepResult(result);
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerEU_Test, preprocessRadioMsg)
{
    m_managerEU->preprocessRadioMsg(
        "<grammar_new agent=\"media\">"
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
                "<formal id=\"21\" name=\"CD\">"
                    "<alias name=\"CD\" tts=\"CD\" />"
                    "<alias name=\"CD Player\" tts=\"CD\" />"
                    "<alias name=\"CD Changer\" tts=\"CD\" />"
                    "<alias name=\"Disc\" tts=\"CD\" />"
                    "<alias name=\"MP3 CD\" tts=\"CD\" />"
                "</formal>"
                    "<formal id=\"5\" name=\"Bluetooth Audio\">"
                    "<alias name=\"Bluetooth\" tts=\"Bluetooth Audio\" />"
                    "<alias name=\"Bluetooth Audio\" tts=\"Bluetooth Audio\" />"
                    "<alias name=\"BT Audio\" tts=\"Bluetooth Audio\" />"
                    "<alias name=\"BTA\" tts=\"Bluetooth Audio\" />"
                "</formal>"
            "</category>"
        "</grammar_new>");
    m_managerEU->preprocessRadioMsg(
        "<grammar_new agent=\"media\">"
            "<category name=\"fmstation\">"
                "<formal id=\"1\" name=\"one\" />"
            "</category>"
        "</grammar_new>");
    m_managerEU->preprocessRadioMsg(
        "<grammar_new agent=\"media\">"
            "<category name=\"dabstation\">"
                "<formal id=\"1\" name=\"one\" />"
            "</category>"
        "</grammar_new>");
    m_managerEU->preprocessRadioMsg(
        "");    

    SUCCEED();

}

TEST_F(VR_DataAccessorManagerEU_Test, getMusicCTermList)
{
    VoiceList<spC_Term>::type cTermList;
    m_managerEU->getMusicCTermList(cTermList, 1, "aaa", "", "");
    SUCCEED();
}



/* EOF */
