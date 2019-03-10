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

#include "VR_DataAccessorMediaEU.h"

#include <sqlite3.h>
#include "gtest/gtest.h"

/**
* VR_DataAccessorMediaEU_Test
*
* The class is just for VR_DataAccessorMediaEU test.
*/
class VR_DataAccessorMediaEU_Test : public testing::Test
{
public:
    VR_DataAccessorMediaEU_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

protected:
    static VR_DataAccessorMediaEU *m_mediaAccessorEU;

private:
    VR_DataAccessorMediaEU_Test(const VR_DataAccessorMediaEU_Test& other);
    void operator= (const VR_DataAccessorMediaEU_Test& other);
};

VR_DataAccessorMediaEU *VR_DataAccessorMediaEU_Test::m_mediaAccessorEU = nullptr;

void VR_DataAccessorMediaEU_Test::SetUpTestCase()
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
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentAlbumsSongs",
            "nAlbumId integer,nSongId integer,nSourceId integer"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentArtistsSongs",
            "nArtistId integer,nSongId integer,nSourceId integer"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentGenresSongs",
            "nGenreId integer,nSongId integer,nSourceId integer"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentPlaylistsSongs",
            "nPlaylistId integer,nSongId integer,nSourceId integer"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentAlbumsArtists",
            "nAlbumId integer,nArtistId integer,nSourceId integer"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentGenresAlbums",
            "nGenreId integer,nAlbumId integer,nSourceId integer"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "MusicAgentGenresArtists",
            "nGenreId integer,nArtistId integer,nSourceId integer"));


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

    sqlRequest.assign("INSERT INTO MusicAgentAlbumsSongs VALUES ("
        "\"1\", "
        "\"1\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentAlbumsSongs VALUES ("
        "\"1\", "
        "\"2\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentAlbumsSongs VALUES ("
        "\"2\", "
        "\"3\", "
        "\"1\")");
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

    sqlRequest.assign("INSERT INTO MusicAgentArtistsSongs VALUES ("
        "\"1\", "
        "\"1\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentArtistsSongs VALUES ("
        "\"1\", "
        "\"2\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentArtistsSongs VALUES ("
        "\"2\", "
        "\"3\", "
        "\"1\")");
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

    sqlRequest.assign("INSERT INTO MusicAgentGeneresSongs VALUES ("
        "\"1\", "
        "\"1\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGeneresSongs VALUES ("
        "\"1\", "
        "\"2\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGeneresSongs VALUES ("
        "\"2\", "
        "\"3\", "
        "\"1\")");
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

    sqlRequest.assign("INSERT INTO MusicAgentPlaylistsSongs VALUES ("
        "\"1\", "
        "\"1\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylistsSongs VALUES ("
        "\"1\", "
        "\"2\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylistsSongs VALUES ("
        "\"2\", "
        "\"3\", "
        "\"1\")");
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

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
        "\"4\", "
        "\"hello\", "
        "\"1\", "
        "\"4\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
        "\"5\", "
        "\"world\", "
        "\"1\", "
        "\"5\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
        "\"6\", "
        "\"monday\", "
        "\"1\", "
        "\"6\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
        "\"7\", "
        "\"tuesday\", "
        "\"1\", "
        "\"7\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
        "\"8\", "
        "\"wednesday\", "
        "\"1\", "
        "\"8\", "
        "\"\", "
        "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentAlbumsArtists VALUES ("
        "\"1\", "
        "\"1\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentAlbumsArtists VALUES ("
        "\"1\", "
        "\"2\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentAlbumsArtists VALUES ("
        "\"2\", "
        "\"3\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenresAlbums VALUES ("
        "\"1\", "
        "\"1\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenresAlbums VALUES ("
        "\"1\", "
        "\"2\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenresAlbums VALUES ("
        "\"2\", "
        "\"3\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenresArtists VALUES ("
        "\"1\", "
        "\"1\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenresArtists VALUES ("
        "\"1\", "
        "\"2\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenresArtists VALUES ("
        "\"2\", "
        "\"3\", "
        "\"1\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);


    // sqlRequest.assign("INSERT INTO MusicAgentPlaylistsSongs VALUES ("
    //     "\"1\", "
    //     "\"one\", "
    //     "\"1\", "
    //     "\"1\", "
    //     "\"\", "
    //     "\"\")");
    // sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    // sqlRequest.assign("INSERT INTO MusicAgentPlaylistsSongs VALUES ("
    //     "\"2\", "
    //     "\"two\", "
    //     "\"1\", "
    //     "\"2\", "
    //     "\"\", "
    //     "\"\")");
    // sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlite3_exec(m_dbHandler, "COMMIT", NULL, NULL, NULL);
    sqlite3_close(m_dbHandler);

    m_mediaAccessorEU = new VR_DataAccessorMediaEU();
    m_mediaAccessorEU->openDB("/tmp/VR_DataAccessorMediaTest.db");
    VoiceMap<int, std::string>::type radioFM;
    // VoiceMap<int, std::string>::type radioFM = m_mediaAccessorEU->getRadioStationMap(VR_StationType::FM);
    radioFM.insert(std::pair<int, std::string>(1, "99.7"));
    radioFM.insert(std::pair<int, std::string>(2, "109.7"));
    m_mediaAccessorEU->m_radioFMStationMap = radioFM;
    VoiceMap<int, std::string>::type radioDAB;
    // VoiceMap<int, std::string>::type radioDAB = m_mediaAccessorEU->getRadioStationMap(VR_StationType::DAB);
    radioDAB.insert(std::pair<int, std::string>(1, "1111"));
    radioDAB.insert(std::pair<int, std::string>(2, "2222"));
    radioDAB.insert(std::pair<int, std::string>(3, "109.7"));
    m_mediaAccessorEU->m_radioDABStationMap = radioDAB;
}

void VR_DataAccessorMediaEU_Test::TearDownTestCase()
{
    m_mediaAccessorEU->closeDB();
    delete m_mediaAccessorEU;
    m_mediaAccessorEU = nullptr;
    remove("/tmp/VR_DataAccessorMediaTest.db");
}

TEST_F(VR_DataAccessorMediaEU_Test, getRadioStationMap)
{
    m_mediaAccessorEU->getRadioStationMap(VR_StationType::FM);
    m_mediaAccessorEU->getRadioStationMap(VR_StationType::DAB);
    SUCCEED();
}

TEST_F(VR_DataAccessorMediaEU_Test, getInfo)
{
    std::string response;
    EXPECT_FALSE(m_mediaAccessorEU->getInfo("", "", response));
    EXPECT_TRUE(m_mediaAccessorEU->getInfo("_getPlayLists", "", response));
    EXPECT_FALSE(m_mediaAccessorEU->getInfo("_getPlaylistSongsNumber", "", response));
    EXPECT_TRUE(m_mediaAccessorEU->getInfo("_getRadioStations", "", response));
    EXPECT_FALSE(m_mediaAccessorEU->getInfo("_getMusicByType", "", response));
    EXPECT_FALSE(m_mediaAccessorEU->getInfo("_selectSongs", "", response));
    EXPECT_FALSE(m_mediaAccessorEU->getInfo("_queryOneshotName", "", response));
    EXPECT_FALSE(m_mediaAccessorEU->getInfo("_queryOneshotDetail", "", response));
    EXPECT_TRUE(m_mediaAccessorEU->getInfo("_queryGrammarUpdateStatus", "", response));
    EXPECT_FALSE(m_mediaAccessorEU->getInfo("_queryTunerResult", "", response));
    EXPECT_FALSE(m_mediaAccessorEU->getInfo("otheroperation", "", response));
}

TEST_F(VR_DataAccessorMediaEU_Test, handleMusicOneshotResult)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<node name=\"intent\" value=\"media_oneshot_with_slot_PHRASE\" sentenceValue=\"Play jack1\" confidence=\"5402\">"
            "<list id=\"asr0\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>2</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">hello</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack2</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack3</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack2</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\">David</artist_name>"
                        "<artist_id confidence=\"4205\">1</artist_id>"
                        "<album_name confidence=\"4500\">space</album_name>"
                        "<album_id confidence=\"4500\">1</album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">lemon tree</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">hello</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\"></artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"4500\">3</album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"4500\">hello</song_name>"
                        "<song_id confidence=\"4500\">2</song_id>"
                        "<genre_name confidence=\"856\">red</genre_name>"
                        "<genre_id confidence=\"72\">6</genre_id>"
                        "<artist_name confidence=\"0\"></artist_name>"
                        "<artist_id confidence=\"0\"></artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                    "</item>"
                "</items>"
            "</list>"
        "</node>");
    pugi::xml_node result = testDoc.child("node");
    m_mediaAccessorEU->handleMusicOneshotResult(result);
    testDoc.load_string(
        "<node name=\"intent\" value=\"media_oneshot_with_slot_PHRASE\" sentenceValue=\"Play jack1\" confidence=\"5402\">"
            "<list id=\"asr0\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>2</count>"
                "</header>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\">David</artist_name>"
                        "<artist_id confidence=\"4205\">1</artist_id>"
                        "<album_name confidence=\"4500\">space</album_name>"
                        "<album_id confidence=\"4500\">1</album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">lemon tree</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                    "</item>"
                "</items>"
            "</list>"
        "</node>");
    result = testDoc.child("node");
    m_mediaAccessorEU->handleMusicOneshotResult(result);
    SUCCEED();
}

TEST_F(VR_DataAccessorMediaEU_Test, getRadioStations)
{
    std::string response;
    EXPECT_TRUE(m_mediaAccessorEU->getRadioStations(response));
}

TEST_F(VR_DataAccessorMediaEU_Test, queryOneshotName)
{
    std::string response;
    EXPECT_FALSE(m_mediaAccessorEU->queryOneshotName("", response));
    m_mediaAccessorEU->m_artistIDList.push_back("3");
    EXPECT_TRUE(m_mediaAccessorEU->queryOneshotName("", response));
    m_mediaAccessorEU->closeDB();
    EXPECT_FALSE(m_mediaAccessorEU->queryOneshotName("", response));
    m_mediaAccessorEU->openDB("/tmp/VR_DataAccessorMediaTest.db");
}

TEST_F(VR_DataAccessorMediaEU_Test, queryOneshotDetail)
{
    std::string reqMsg = "<data id=\"_queryOneshotDetail\" xmlns=\"http:\/\/www.w3.org/2005/07/scxml\"><data><artist><artist_name>David</artist_name><artist_id>1</artist_id></artist></data></data>";
    std::string response;
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<node name=\"intent\" value=\"media_oneshot_with_slot_PHRASE\" sentenceValue=\"Play jack1\" confidence=\"5402\">"
            "<list id=\"asr0\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>2</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">hello</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack2</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack3</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack2</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\">David</artist_name>"
                        "<artist_id confidence=\"4205\">1</artist_id>"
                        "<album_name confidence=\"4500\">space</album_name>"
                        "<album_id confidence=\"4500\">1</album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">lemon tree</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">hello</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\"></artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"4500\">3</album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"4500\">hello</song_name>"
                        "<song_id confidence=\"4500\">2</song_id>"
                        "<genre_name confidence=\"856\">red</genre_name>"
                        "<genre_id confidence=\"72\">6</genre_id>"
                        "<artist_name confidence=\"0\"></artist_name>"
                        "<artist_id confidence=\"0\"></artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                    "</item>"
                "</items>"
            "</list>"
        "</node>");
    pugi::xml_node result = testDoc.child("node");
    m_mediaAccessorEU->handleMusicOneshotResult(result);
    EXPECT_FALSE(m_mediaAccessorEU->queryOneshotDetail("", response));
    EXPECT_TRUE(m_mediaAccessorEU->queryOneshotDetail(reqMsg, response));
    m_mediaAccessorEU->closeDB();
    EXPECT_FALSE(m_mediaAccessorEU->queryOneshotDetail("", response));
    m_mediaAccessorEU->openDB("/tmp/VR_DataAccessorMediaTest.db");
}

TEST_F(VR_DataAccessorMediaEU_Test, setVoiceLanguage)
{
    m_mediaAccessorEU->setVoiceLanguage("abc");
    SUCCEED();
}

TEST_F(VR_DataAccessorMediaEU_Test, getMusicByType)
{
    std::string resMsg;
    std::string response;
    EXPECT_FALSE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>GENRE</type><genre><genreName>'classic'</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->m_selectedGenreID = "5";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>ARTIST</type><genre><genreName>''</genreName><genreId>5</genreId></genre><artist><artistName>'David'</artistName><artistId>1</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->m_selectedArtistID = "1";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->m_selectedGenreID.clear();
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->m_selectedArtistID.clear();
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>ALBUM</type><genre><genreName>''</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>'space'</albumName><albumId>1</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->m_selectedArtistID = "1";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->m_selectedGenreID = "5";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->m_selectedArtistID.clear();
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>SONG</type><genre><genreName>''</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album><song><songName>'lemon tree'</songName><songId>2</songId></song></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>PLAYLIST</type><genre><genreName>''</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album><playlist><playlistName>'first'</playlistName><playlistId>1</playlistId></playlist></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>BALABALA</type><genre><genreName>'classic'</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_FALSE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->closeDB();
    EXPECT_FALSE(m_mediaAccessorEU->getMusicByType(resMsg, response));
    m_mediaAccessorEU->openDB("/tmp/VR_DataAccessorMediaTest.db");
}

TEST_F(VR_DataAccessorMediaEU_Test, selectSongs)
{
    std::string resMsg;
    std::string response;
    EXPECT_FALSE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>GENRE</type><genre><genreName>'classic'</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>GENRE</type><genre><genreName>'classic'</genreName><genreId>none</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>ARTIST</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>'David'</artistName><artistId>1</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>ALBUM</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>'space'</albumName><albumId>1</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>ALBUM</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>'space'</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>SONG</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album><song><songName>'lemon tree'</songName><songId>2</songId></song></data></data>";
    EXPECT_FALSE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>PLAYLIST</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album><playlist><playlistName>'first'</playlistName><playlistId>1</playlistId></playlist></data></data>";
    EXPECT_FALSE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>BALABALA</type><genre><genreName>'classic'</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_FALSE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>ALL</type><genre><genreName>'classic'</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    EXPECT_TRUE(m_mediaAccessorEU->selectSongs(resMsg, response));
    resMsg = "<data id=\"_selectSongs\"><data><type>GENRE</type><genr";
    EXPECT_FALSE(m_mediaAccessorEU->selectSongs(resMsg, response));
}

TEST_F(VR_DataAccessorMediaEU_Test, queryTunerResult)
{
    std::string reqMsg = "<data><station><station_band>DAB</station_band><station_id>3</station_id><station_name>109.7</station_name></station></data>";
    std::string response;
    EXPECT_FALSE(m_mediaAccessorEU->queryTunerResult("", response));
    EXPECT_TRUE(m_mediaAccessorEU->queryTunerResult(reqMsg, response));
    reqMsg = "<data><station><station_band>FM</station_band><station_id>2</station_id><station_name>109.7</station_name></station></data>";
    EXPECT_TRUE(m_mediaAccessorEU->queryTunerResult(reqMsg, response));
    reqMsg = "<data><station><station_band>invalid</station_band></station></data>";
    EXPECT_TRUE(m_mediaAccessorEU->queryTunerResult(reqMsg, response));
    reqMsg = "<data><station><station_band>DAB</station_band><station_id>1</station_id><station_name>109.7</station_name></station></data>";
    EXPECT_TRUE(m_mediaAccessorEU->queryTunerResult(reqMsg, response));
    reqMsg = "<data><station><station_band>FM</station_band><station_id>1</station_id><station_name>109.7</station_name></station></data>";
    EXPECT_TRUE(m_mediaAccessorEU->queryTunerResult(reqMsg, response));
    reqMsg = "<data><station><station_band>FM</station_ba";
    EXPECT_FALSE(m_mediaAccessorEU->queryTunerResult(reqMsg, response));
}

TEST_F(VR_DataAccessorMediaEU_Test, queryOneshotNameCallback)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<node name=\"intent\" value=\"media_oneshot_with_slot_PHRASE\" sentenceValue=\"Play jack1\" confidence=\"5402\">"
            "<list id=\"asr0\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>2</count>"
                "</header>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\">David</artist_name>"
                        "<artist_id confidence=\"4205\">1</artist_id>"
                        "<album_name confidence=\"4500\">space</album_name>"
                        "<album_id confidence=\"4500\">1</album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">lemon tree</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                    "</item>"
                "</items>"
            "</list>"
        "</node>");
    pugi::xml_node result = testDoc.child("node");
    pugi::xml_node* itemsNode = &result;
    char* columnValue[] = { "0", "jack" };
    char* columnName[] = { "aaaa" };
    EXPECT_EQ(0, m_mediaAccessorEU->queryOneshotNameCallback(itemsNode, 1, columnValue, columnName));
    SUCCEED();
}

TEST_F(VR_DataAccessorMediaEU_Test, filterAsrMusicStepByStepResult)
{
    pugi::xml_document testDoc;
    std::string resMsg;
    std::string response;
    testDoc.load_string(
        "<node name=\"intent\" value=\"media_oneshot_with_slot_PHRASE\" sentenceValue=\"Play jack1\" confidence=\"5402\">"
            "<list id=\"asr0\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>2</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">hello</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack2</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack3</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4412\">jack2</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"0\"></song_name>"
                        "<song_id confidence=\"0\"></song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\">David</artist_name>"
                        "<artist_id confidence=\"4205\">1</artist_id>"
                        "<album_name confidence=\"4500\">space</album_name>"
                        "<album_id confidence=\"4500\">1</album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">lemon tree</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                    "</item>"
                    "<item>"
                        "<genre_name confidence=\"0\"></genre_name>"
                        "<genre_id confidence=\"0\"></genre_id>"
                        "<artist_name confidence=\"4350\">jack1</artist_name>"
                        "<artist_id confidence=\"4205\">5</artist_id>"
                        "<album_name confidence=\"0\"></album_name>"
                        "<album_id confidence=\"0\"></album_id>"
                        "<playlist_name confidence=\"0\"></playlist_name>"
                        "<playlist_id confidence=\"0\"></playlist_id>"
                        "<song_name confidence=\"5100\">hello</song_name>"
                        "<song_id confidence=\"5100\">2</song_id>"
                    "</item>"
                "</items>"
            "</list>"
        "</node>");
    pugi_vr::xml_node result = testDoc.child("node");
    resMsg = "<data id=\"_selectSongs\"><data><type>GENRE</type><genre><genreName>'classic'</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    m_mediaAccessorEU->getMusicByType(resMsg, response);
    EXPECT_FALSE(m_mediaAccessorEU->filterAsrMusicStepByStepResult(result));
    resMsg = "<data id=\"_selectSongs\"><data><type>ARTIST</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>'David'</artistName><artistId>1</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    m_mediaAccessorEU->getMusicByType(resMsg, response);
    EXPECT_TRUE(m_mediaAccessorEU->filterAsrMusicStepByStepResult(result));
    resMsg = "<data id=\"_selectSongs\"><data><type>ALBUM</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>'space'</albumName><albumId>1</albumId></album></data></data>";
    m_mediaAccessorEU->getMusicByType(resMsg, response);
    EXPECT_FALSE(m_mediaAccessorEU->filterAsrMusicStepByStepResult(result));
    resMsg = "<data id=\"_selectSongs\"><data><type>SONG</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album><song><songName>'lemon tree'</songName><songId>2</songId></song></data></data>";
    m_mediaAccessorEU->getMusicByType(resMsg, response);
    EXPECT_FALSE(m_mediaAccessorEU->filterAsrMusicStepByStepResult(result));
    resMsg = "<data id=\"_selectSongs\"><data><type>PLAYLIST</type><genre><genreName>''</genreName><genreId>none</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album><playlist><playlistName>'first'</playlistName><playlistId>1</playlistId></playlist></data></data>";
    m_mediaAccessorEU->getMusicByType(resMsg, response);
    EXPECT_FALSE(m_mediaAccessorEU->filterAsrMusicStepByStepResult(result));
    resMsg = "<data id=\"_selectSongs\"><data><type>BALABALA</type><genre><genreName>'classic'</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    m_mediaAccessorEU->getMusicByType(resMsg, response);
    EXPECT_FALSE(m_mediaAccessorEU->filterAsrMusicStepByStepResult(result));
    resMsg = "<data id=\"_selectSongs\"><data><type>ALL</type><genre><genreName>'classic'</genreName><genreId>5</genreId></genre><artist><artistName>''</artistName><artistId>none</artistId></artist><album><albumName>''</albumName><albumId>none</albumId></album></data></data>";
    m_mediaAccessorEU->getMusicByType(resMsg, response);
    EXPECT_FALSE(m_mediaAccessorEU->filterAsrMusicStepByStepResult(result));
}

TEST_F(VR_DataAccessorMediaEU_Test, sortRadioStationsResponse)
{
    pugi_vr::xml_node itemNode;
    VoiceMultiMap<std::string, VR_DataAccessorMediaEU::StationInfo>::type stationResult;
    struct VR_DataAccessorMediaEU::StationInfo preset1 = { "one", "FM", 1 };
    struct VR_DataAccessorMediaEU::StationInfo preset2 = { "two", "FM", 2 };
    struct VR_DataAccessorMediaEU::StationInfo preset3 = { "one", "DAB", 3 };
    struct VR_DataAccessorMediaEU::StationInfo preset4 = { "two", "DAB", 4 };
    struct VR_DataAccessorMediaEU::StationInfo preset5 = { "three", "", 5 };
    struct VR_DataAccessorMediaEU::StationInfo preset6 = { "three", "aaa", 6 };

    stationResult.insert(std::pair<std::string, VR_DataAccessorMediaEU::StationInfo>("preset1", preset1));
    stationResult.insert(std::pair<std::string, VR_DataAccessorMediaEU::StationInfo>("preset1", preset3));
    EXPECT_TRUE(m_mediaAccessorEU->sortRadioStationsResponse(itemNode, stationResult));
    stationResult.insert(std::pair<std::string, VR_DataAccessorMediaEU::StationInfo>("preset2", preset4));
    stationResult.insert(std::pair<std::string, VR_DataAccessorMediaEU::StationInfo>("preset2", preset2));
    EXPECT_TRUE(m_mediaAccessorEU->sortRadioStationsResponse(itemNode, stationResult));
    stationResult.insert(std::pair<std::string, VR_DataAccessorMediaEU::StationInfo>("preset3", preset5));
    stationResult.insert(std::pair<std::string, VR_DataAccessorMediaEU::StationInfo>("preset3", preset6));
    EXPECT_TRUE(m_mediaAccessorEU->sortRadioStationsResponse(itemNode, stationResult));

}


/* EOF */
