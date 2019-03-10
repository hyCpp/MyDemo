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

#include "VR_DataAccessorMedia.h"

#include <sqlite3.h>
#include "gtest/gtest.h"

/**
* VR_DataAccessorMedia_Test
*
* The class is just for VR_DataAccessorMedia test.
*/
class VR_DataAccessorMedia_Test : public testing::Test
{
public:
    VR_DataAccessorMedia_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

protected:
    static VR_DataAccessorMedia *m_mediaAccessor;

private:
    VR_DataAccessorMedia_Test(const VR_DataAccessorMedia_Test& other);
    void operator= (const VR_DataAccessorMedia_Test& other);
};

VR_DataAccessorMedia *VR_DataAccessorMedia_Test::m_mediaAccessor = nullptr;

void VR_DataAccessorMedia_Test::SetUpTestCase()
{
    sqlite3 *m_dbHandler;
    int dbResult = sqlite3_open("/tmp/VR_DataAccessorMediaTest.db", &m_dbHandler);
    ASSERT_EQ(SQLITE_OK, dbResult);

    // init db data
    char * errmsg = nullptr;

    VoiceMap<std::string, std::string>::type tableFormatMap;
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentAlbums",
            "nAlbumId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentArtists",
            "nArtistId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentGenres",
            "nGenreId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentPlaylists",
            "nPlaylistId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentSongs",
            "nSongId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentPlaylistsSongs",
            "nPlaylistId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));

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

    // start: bad records for MusicAgentPlaylists
    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
        "\"3\", "
        "\"third\", "
        "\"1\", "
        "\"\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
        "\"4\", "
        "\"forth\", "
        "\"\", "
        "\"\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
        "\"5\", "
        "\"fifth\", "
        "\"\", "
        "\"5\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
        "\"6\", "
        "\"\", "
        "\"1\", "
        "\"6\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
        "\"7\", "
        "\"\", "
        "\"\", "
        "\"7\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
        "\"8\", "
        "\"\", "
        "\"\", "
        "\"\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);
    // end: bad records for MusicAgentPlaylists

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

    sqlRequest.assign("INSERT INTO MusicAgentPlaylistsSongs VALUES ("
        "\"1\", "
        "\"one\", "
        "\"1\", "
        "\"1\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylistsSongs VALUES ("
        "\"2\", "
        "\"two\", "
        "\"1\", "
        "\"2\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlite3_exec(m_dbHandler, "COMMIT", NULL, NULL, NULL);
    sqlite3_close(m_dbHandler);

    m_mediaAccessor = new VR_DataAccessorMedia();
    m_mediaAccessor->openDB("/tmp/VR_DataAccessorMediaTest.db");
}

void VR_DataAccessorMedia_Test::TearDownTestCase()
{
    m_mediaAccessor->closeDB();
    delete m_mediaAccessor;
    m_mediaAccessor = nullptr;
    remove("/tmp/VR_DataAccessorMediaTest.db");
}

TEST_F(VR_DataAccessorMedia_Test, getInfo)
{
    std::cout << "pengjialing1" << std::endl;
    std::string response;
    EXPECT_FALSE(m_mediaAccessor->getInfo("", "", response));
    std::cout << "pengjialing2" << std::endl;
    EXPECT_TRUE(m_mediaAccessor->getInfo("_getPlayLists", "", response));
    std::cout << "pengjialing3" << std::endl;
    EXPECT_TRUE(m_mediaAccessor->getInfo("_getPlaylistSongsNumber", "", response));
    std::cout << "pengjialing4" << std::endl;
    EXPECT_TRUE(m_mediaAccessor->getInfo("_queryGrammarUpdateStatus", "", response));
    // EXPECT_FALSE(m_mediaAccessor->getInfo("_getPlaylistSongsNumber", "", response));
    // EXPECT_TRUE(m_mediaAccessor->getInfo("_getRadioStations", "", response));
    // EXPECT_FALSE(m_mediaAccessor->getInfo("_getMusicByType", "", response));
    // EXPECT_FALSE(m_mediaAccessor->getInfo("_selectSongs", "", response));
    // EXPECT_TRUE(m_mediaAccessor->getInfo("_queryOneshotName", "", response));
    // EXPECT_FALSE(m_mediaAccessor->getInfo("_queryOneshotDetail", "", response));
    EXPECT_FALSE(m_mediaAccessor->getInfo("otheroperation", "", response));
}

TEST_F(VR_DataAccessorMedia_Test, isOperationHandled)
{
    EXPECT_FALSE(m_mediaAccessor->isOperationHandled("otheroperation"));
    EXPECT_TRUE(m_mediaAccessor->isOperationHandled("_getPlayLists"));
    EXPECT_FALSE(m_mediaAccessor->isOperationHandled("_getPlaylistSongsNumber"));
    // EXPECT_TRUE(m_mediaAccessor->isOperationHandled("_getPlaylistSongsNumber"));
    // EXPECT_TRUE(m_mediaAccessor->isOperationHandled("_getRadioStations"));
    // EXPECT_TRUE(m_mediaAccessor->isOperationHandled("_getMusicByType"));
    // EXPECT_TRUE(m_mediaAccessor->isOperationHandled("_selectSongs"));
    // EXPECT_TRUE(m_mediaAccessor->isOperationHandled("_queryOneshotName"));
    // EXPECT_TRUE(m_mediaAccessor->isOperationHandled("_queryOneshotDetail"));
}

TEST_F(VR_DataAccessorMedia_Test, updateMusicItemName)
{
    pugi::xml_document testDoc;
    testDoc.load_string("<item>"
                            "<genre_name confidence=\"0\"></genre_name>"
                            "<genre_id confidence=\"0\"></genre_id>"
                            "<artist_name confidence=\"4797\">jack1</artist_name>"
                            "<artist_id confidence=\"5123\">5</artist_id>"
                            "<album_name confidence=\"0\"></album_name>"
                            "<album_id confidence=\"0\"></album_id>"
                            "<playlist_name confidence=\"0\"></playlist_name>"
                            "<playlist_id confidence=\"0\"></playlist_id>"
                            "<song_name confidence=\"0\"></song_name>"
                            "<song_id confidence=\"0\">1</song_id>"
                            "<genre_name confidence=\"0\"></genre_name>"
                            "<genre_id confidence=\"0\"></genre_id>"
                            "<artist_name confidence=\"0\"></artist_name>"
                            "<artist_id confidence=\"0\"></artist_id>"
                            "<album_name confidence=\"0\"></album_name>"
                            "<album_id confidence=\"0\"></album_id>"
                            "<playlist_name confidence=\"0\"></playlist_name>"
                            "<playlist_id confidence=\"0\"></playlist_id>"
                            "<song_name confidence=\"0\"></song_name>"
                            "<song_id confidence=\"0\"></song_id>"
                        "</item>");
    pugi::xml_node itemsNode = testDoc.child("item");
    m_mediaAccessor->updateMusicItemName(itemsNode, "song");
    m_mediaAccessor->updateMusicItemName(itemsNode, "balabala");
    testDoc.load_string("<item>"
                            "<genre_name confidence=\"0\"></genre_name>"
                            "<genre_id confidence=\"0\"></genre_id>"
                            "<artist_name confidence=\"4797\">jack1</artist_name>"
                            "<artist_id confidence=\"5123\">5</artist_id>"
                            "<album_name confidence=\"0\"></album_name>"
                            "<album_id confidence=\"0\"></album_id>"
                            "<playlist_name confidence=\"0\"></playlist_name>"
                            "<playlist_id confidence=\"0\"></playlist_id>"
                            "<song_name confidence=\"0\"></song_name>"
                            "<genre_name confidence=\"0\"></genre_name>"
                            "<genre_id confidence=\"0\"></genre_id>"
                            "<artist_name confidence=\"0\"></artist_name>"
                            "<artist_id confidence=\"0\"></artist_id>"
                            "<album_name confidence=\"0\"></album_name>"
                            "<album_id confidence=\"0\"></album_id>"
                            "<playlist_name confidence=\"0\"></playlist_name>"
                            "<playlist_id confidence=\"0\"></playlist_id>"
                            "<song_name confidence=\"0\"></song_name>"
                        "</item>");
    itemsNode = testDoc.child("item");
    m_mediaAccessor->updateMusicItemName(itemsNode, "song");
    SUCCEED();
    
}

TEST_F(VR_DataAccessorMedia_Test, getMusicDBTableName)
{
    EXPECT_STREQ("MusicAgentAaaas", m_mediaAccessor->getMusicDBTableName("aaaa").c_str());
}

TEST_F(VR_DataAccessorMedia_Test, getSongNumber)
{
    EXPECT_STREQ("3", m_mediaAccessor->getSongNumber().c_str());
}

TEST_F(VR_DataAccessorMedia_Test, getArtistNumber)
{
    EXPECT_STREQ("2", m_mediaAccessor->getArtistNumber().c_str());
}

TEST_F(VR_DataAccessorMedia_Test, getAlbumNumber)
{
    EXPECT_STREQ("2", m_mediaAccessor->getAlbumNumber().c_str());
}

TEST_F(VR_DataAccessorMedia_Test, getPlaylistNumber)
{
    EXPECT_STREQ("8", m_mediaAccessor->getPlaylistNumber().c_str());
}

TEST_F(VR_DataAccessorMedia_Test, getPlaylists)
{
    std::string response;
    m_mediaAccessor->getPlaylists(response);
    SUCCEED();
}

TEST_F(VR_DataAccessorMedia_Test, getPlaylists_dberror)
{
    std::string response;
    m_mediaAccessor->closeDB();
    m_mediaAccessor->openDB("/tmp/vr_empty_db.db");
    EXPECT_FALSE(m_mediaAccessor->getPlaylists(response));
    m_mediaAccessor->openDB("/tmp/VR_DataAccessorMediaTest.db");
}

TEST_F(VR_DataAccessorMedia_Test, getPlaylistSongsNumber_dberror)
{
    std::string response;
    m_mediaAccessor->closeDB();
    m_mediaAccessor->openDB("/tmp/vr_empty_db.db");
    EXPECT_FALSE(m_mediaAccessor->getPlaylistSongsNumber("", response));
    m_mediaAccessor->openDB("/tmp/VR_DataAccessorMediaTest.db");
}

TEST_F(VR_DataAccessorMedia_Test, getPlaylistSongsNumber)
{
    std::string response;
    std::string strReqMeg = "<reqMsg>"
                                "<id>1</id>"
                            "</reqMsg>";
    EXPECT_TRUE(m_mediaAccessor->getPlaylistSongsNumber(strReqMeg, response));
}

// TEST_F(VR_DataAccessorMedia_Test, getPlaylistSongsNumber)
// {
//     std::string response;
//     m_mediaAccessor->getPlaylistSongsNumber("", response);
//     SUCCEED();
// }

// TEST_F(VR_DataAccessorMedia_Test, getRadioStations)
// {
//     std::string response;
//     m_mediaAccessor->getRadioStations(response);
//     SUCCEED();
// }

// TEST_F(VR_DataAccessorMedia_Test, getMusicByType)
// {
//     std::string response;
//     m_mediaAccessor->getMusicByType("", response);
//     m_mediaAccessor->getMusicByType("<type>ALL</type>", response);
//     m_mediaAccessor->getMusicByType("<type>GENRE</type>", response);
//     m_mediaAccessor->getMusicByType("<type>ARTIST</type>", response);
//     m_mediaAccessor->getMusicByType("<type>ALBUM</type>", response);
//     m_mediaAccessor->getMusicByType("<type>PLAYLIST</type>", response);
//     m_mediaAccessor->getMusicByType("<type>SONG</type>", response);
//     SUCCEED();
// }

// TEST_F(VR_DataAccessorMedia_Test, selectSongs)
// {
//     m_mediaAccessor->selectSongs("");
//     m_mediaAccessor->selectSongs("<type>ALL</type>");
//     m_mediaAccessor->selectSongs("<type>GENRE</type>");
//     m_mediaAccessor->selectSongs("<type>ARTIST</type>");
//     m_mediaAccessor->selectSongs("<type>ALBUM</type>");
//     m_mediaAccessor->selectSongs("<type>PLAYLIST</type>");
//     m_mediaAccessor->selectSongs("<type>SONG</type>");
//     SUCCEED();
// }

// TEST_F(VR_DataAccessorMedia_Test, queryOneshotName)
// {
//     std::string response;
//     m_mediaAccessor->queryOneshotName("", response);
//     SUCCEED();
// }

// TEST_F(VR_DataAccessorMedia_Test, queryOneshotDetail)
// {
//     std::string response;
//     m_mediaAccessor->queryOneshotDetail("", response);
//     m_mediaAccessor->queryOneshotDetail("<artist><id>2</id></artist>", response);
//     SUCCEED();
// }

TEST_F(VR_DataAccessorMedia_Test, queryGrammarUpdateStatus)
{
    std::string response = "";
    m_mediaAccessor->queryGrammarUpdateStatus("", response);
    EXPECT_STREQ("<status></status>", response.c_str());
}

TEST_F(VR_DataAccessorMedia_Test, getRecordCount_dberror)
{
    m_mediaAccessor->closeDB();
    m_mediaAccessor->openDB("/tmp/vr_empty_db.db");
    EXPECT_STREQ("0", m_mediaAccessor->getRecordCount("").c_str());
    m_mediaAccessor->openDB("/tmp/VR_DataAccessorMediaTest.db");
}

TEST_F(VR_DataAccessorMedia_Test, getRecordCount_tableNameError)
{
    EXPECT_STREQ("0", m_mediaAccessor->getRecordCount("XXX").c_str());
}

TEST_F(VR_DataAccessorMedia_Test, openDB_Error)
{
    m_mediaAccessor->closeDB();
    m_mediaAccessor->openDB("/tmp/vr_empty_directory.db");
    m_mediaAccessor->openDB("/tmp/VR_DataAccessorMediaTest.db");
    SUCCEED();
}

TEST_F(VR_DataAccessorMedia_Test, updateMusicItemNameCallback)
{
    VoiceMap<std::string, std::string>::type temp;
    VoiceMap<std::string, std::string>::type* idNameMap = &temp;
    char* columnValue[] = { "abc", "def" };
    char* columnName[] = { "", "" };
    m_mediaAccessor->updateMusicItemNameCallback(idNameMap, 1, columnValue, columnName);
    columnValue[0] = nullptr;
    columnValue[1] = nullptr;
    m_mediaAccessor->updateMusicItemNameCallback(idNameMap, 1, columnValue, columnName);
    SUCCEED();
}

/* EOF */
