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

#include "VR_DataAccessorManagerCN.h"

#include "Vr_Asr_Engine.h"

#include "VR_AsrRequestor_mock.h"
#include "VR_DECommon_mock.h"
#include "VR_Configure_mock.h"

#include <sqlite3.h>
#include "gtest/gtest.h"

using ::testing::Return;

/**
* VR_DataAccessorManagerCN_Test
*
* The class is just for VR_DataAccessorManagerCN test.
*/
class VR_DataAccessorManagerCN_Test : public testing::Test
{
public:
    VR_DataAccessorManagerCN_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

    static VR_DataAccessorManagerCN *m_manager;
    static VR_AsrRequestor_mock *m_asrRequestorMock;
    static VR_DECommon_mock *m_DECommon;
    static VR_Configure_mock *m_configure;

private:
    VR_DataAccessorManagerCN_Test(const VR_DataAccessorManagerCN_Test& other);
    void operator= (const VR_DataAccessorManagerCN_Test& other);
};

extern std::string g_msgToDM;
extern void updateStateCallback(const std::string &msgToDM);

VR_DataAccessorManagerCN *VR_DataAccessorManagerCN_Test::m_manager = nullptr;
VR_AsrRequestor_mock *VR_DataAccessorManagerCN_Test::m_asrRequestorMock = nullptr;
VR_DECommon_mock *VR_DataAccessorManagerCN_Test::m_DECommon = nullptr;
VR_Configure_mock *VR_DataAccessorManagerCN_Test::m_configure = nullptr;

void VR_DataAccessorManagerCN_Test::SetUpTestCase()
{
    sqlite3 *m_dbHandler;
    int dbResult = sqlite3_open("/tmp/MusicCatalog001.db", &m_dbHandler);
    ASSERT_EQ(SQLITE_OK, dbResult);

    // init db data
    char * errmsg = nullptr;
    int result;

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
        "\"yesterday once more\", "
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

    ON_CALL(*m_configure, getVRContry()).WillByDefault(Return(VR_REGION_SA));
    ON_CALL(*m_configure, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*m_configure, getVRProduct()).WillByDefault(Return(VR_PRODUCT_TYPE_T2));
    ON_CALL(*m_configure, getDataPath()).WillByDefault(Return("/tmp/"));
    ON_CALL(*m_configure, getMapDataPath()).WillByDefault(Return("/tmp/"));
    system("mkdir /tmp/VR_DataAccessorTest");
    ON_CALL(*m_configure, getUsrPath()).WillByDefault(Return("/tmp/VR_DataAccessorTest"));

    ON_CALL(*m_DECommon, isGetFromData()).WillByDefault(Return(false));
    ON_CALL(*m_DECommon, getCountryType()).WillByDefault(Return(VR_DECommonIF::DE_Country::gen));

    m_manager = new VR_DataAccessorManagerCN(m_asrRequestorMock, m_DECommon, m_configure);
    boost::function<void(const std::string &)> callback = &updateStateCallback;
    m_manager->setUpdateStateCallback(callback);
    m_manager->setCountryID("1");
}

void VR_DataAccessorManagerCN_Test::TearDownTestCase()
{
    delete m_manager;
    delete m_asrRequestorMock;
    delete m_DECommon;
    delete m_configure;
    m_manager = nullptr;
    m_asrRequestorMock = nullptr;
    m_DECommon = nullptr;
    m_configure = nullptr;
    remove("/tmp/MusicCatalog001.db");
}

TEST_F(VR_DataAccessorManagerCN_Test, setMusicGrammarActive)
{
    m_manager->setMusicGrammarActive("1", true, 1, 1);
    m_manager->setMusicGrammarActive("1", false, 0, 0);
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"USB_1_CONNECTED\" value=\"true\"/>"
        "</event>");
    m_manager->updateGrammar(
        "<grammar_init agent=\"media\" grammarid=\"1\" path=\"/tmp/MusicCatalog001.db\" songitemcount=\"1000\" otheritemcount=\"1000\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\"/>");
    m_manager->setMusicGrammarActive("1", true, 1, 1);
    m_manager->setMusicGrammarActive("1", false, 0, 0);
    m_manager->setMusicGrammarActive("1", true, 5000, 9001);
    m_manager->setMusicGrammarActive("1", true, 5000, 13001);
    m_manager->setMusicGrammarActive("1", true, 18000, 1);
    SUCCEED();
}

TEST_F(VR_DataAccessorManagerCN_Test, setPhoneContactGrammarActive)
{
    m_manager->setPhoneContactGrammarActive(true);
    m_manager->setPhoneContactGrammarActive(false);
    m_manager->updateState(
        "<event name=\"updateState\">"
            "<item key=\"PHONE_DEVICE_ID\" value=\"1\"/>"
            "<item key=\"PHONE_STATE_CONNECTED\" value=\"connected\"/>"
            "<item key=\"PHONE_STATE_REGISTERED\" value=\"1\"/>"
        "</event>");
    m_manager->setPhoneContactGrammarActive(true);
    m_manager->setPhoneContactGrammarActive(false);
    SUCCEED();
}

/* EOF */