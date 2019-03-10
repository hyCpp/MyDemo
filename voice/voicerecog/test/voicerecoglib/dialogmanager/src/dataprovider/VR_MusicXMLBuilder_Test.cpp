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

#include "VR_MusicXMLBuilder_Test.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_MusicXMLBuilder* VR_MusicXMLBuilder_Test::m_testSuite = NULL;

VOID VR_MusicXMLBuilder_Test::SetUpTestCase()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_new VR_MusicXMLBuilder();
    }
}

VOID VR_MusicXMLBuilder_Test::TearDownTestCase()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
        m_testSuite = NULL;
    }
}

VR_MusicXMLBuilder_Test::VR_MusicXMLBuilder_Test()
{
    m_test = NULL;
}

VR_MusicXMLBuilder_Test::~VR_MusicXMLBuilder_Test()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

VOID VR_MusicXMLBuilder_Test::SetUp()
{
    if (NULL == m_test) {
        m_test = VR_new VR_MusicXMLBuilder();
    }
}

VOID VR_MusicXMLBuilder_Test::TearDown()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

TEST_F(VR_MusicXMLBuilder_Test, AppendAgentNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // record num is 0
    NCCHAR* dbPath = "/var/user/MusicCatalog_1.db";
    EXPECT_FALSE(m_test->AppendAgentNode(1, 0, dbPath));

    // dbpath is null
    dbPath = NULL;
    EXPECT_FALSE(m_test->AppendAgentNode(1, 4, dbPath));

    // document load string invalid
    dbPath = "\"/";
    EXPECT_FALSE(m_test->AppendAgentNode(1, 4, dbPath));
}

TEST_F(VR_MusicXMLBuilder_Test, AppendAgentNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCCHAR* dbPath = "/var/user/MusicCatalog_1.db";
    EXPECT_TRUE(m_testSuite->AppendAgentNode(1, 4, dbPath));
}

TEST_F(VR_MusicXMLBuilder_Test, AppendCategoryNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // table name is null
    NCCHAR* tableName = NULL;
    EXPECT_FALSE(m_test->AppendCategoryNode(tableName));

    // table is innertable
    tableName = "MusicAgentAlbumsSongs";
    EXPECT_TRUE(m_test->AppendCategoryNode(tableName));

    // table already append
    tableName = "MusicAgentAlbums";
    m_test->m_xmlName = "album";
    EXPECT_TRUE(m_test->AppendCategoryNode(tableName));

    // append new table
    tableName = "MusicAgentSongs";
    m_test->m_xmlName = "album";
    m_test->m_agentNode = pugi::xml_node();
    EXPECT_FALSE(m_test->AppendCategoryNode(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, AppendCategoryNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCCHAR* tableName = "MusicAgentSongs";
    EXPECT_TRUE(m_testSuite->AppendCategoryNode(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, AppendDelNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // xmlname is null
    NCContentValues values;
    values.put("nId", "5");
    values.put("cName", "lemon tree");
    EXPECT_TRUE(m_test->AppendDelNode(values));

    // category node is null
    m_test->m_xmlName = "song";
    m_test->m_categoryNode = pugi::xml_node();
    EXPECT_FALSE(m_test->AppendDelNode(values));

    // category node is invalid
    m_test->m_xmlName = "song";
    pugi::xml_document doc;
    doc.load_string(VR_MUSICVR_CANCEL_XML);
    m_test->m_delNode = doc.append_child(pugi::node_pcdata);
    EXPECT_FALSE(m_test->AppendDelNode(values));
}

TEST_F(VR_MusicXMLBuilder_Test, AppendDelNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("nId", "5");
    values.put("cName", "lemon tree");
    values.put("nSourceId", "1");
    values.put("nExternalId", "5");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_testSuite->AppendDelNode(values));

    values.clear();
    values.put("nId", "9");
    values.put("cName", "tomorrow");
    values.put("nSourceId", "1");
    values.put("nExternalId", "9");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_testSuite->AppendDelNode(values));
}

TEST_F(VR_MusicXMLBuilder_Test, AppendAddNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // xmlname is null
    NCContentValues values;
    values.put("nId", "7");
    values.put("cName", "yesterday once more");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_test->AppendAddNode(values));

    // category node is null
    m_test->m_xmlName = "song";
    m_test->m_categoryNode = pugi::xml_node();
    EXPECT_FALSE(m_test->AppendAddNode(values));

    // category node is invalid
    m_test->m_xmlName = "song";
    pugi::xml_document doc;
    doc.load_string(VR_MUSICVR_CANCEL_XML);
    m_test->m_addNode = doc.append_child(pugi::node_pcdata);
    EXPECT_FALSE(m_test->AppendAddNode(values));
}

TEST_F(VR_MusicXMLBuilder_Test, AppendAddNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("nId", "7");
    values.put("cName", "yesterday once more");
    values.put("nSourceId", "1");
    values.put("nExternalId", "7");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_testSuite->AppendAddNode(values));

    values.clear();
    values.put("nId", "2");
    values.put("cName", "show");
    values.put("nSourceId", "1");
    values.put("nExternalId", "2");
    values.put("cPronunciation", "");
    values.put("nPronunciationId", "-1");
    EXPECT_TRUE(m_testSuite->AppendAddNode(values));
}

TEST_F(VR_MusicXMLBuilder_Test, UpdateAgentNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    m_test->m_agentNode = pugi::xml_node();
    EXPECT_FALSE(m_test->UpdateAgentNode(1000, 2000, 800, 800, 400));

}

TEST_F(VR_MusicXMLBuilder_Test, UpdateAgentNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    EXPECT_TRUE(m_testSuite->UpdateAgentNode(1000, 2000, 800, 800, 400));
}

TEST_F(VR_MusicXMLBuilder_Test, GetDiffXML)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    VR_String diffStr;
    EXPECT_TRUE(m_testSuite->GetDiffXML(diffStr));

    NCString tempDiffStr = diffStr.c_str();
    tempDiffStr.replace("\r", "");
    tempDiffStr.replace("\n", "");
    tempDiffStr.replace("\t", "");
    EXPECT_STREQ(VR_MUSICDB_DIFF_TEST_XML, tempDiffStr.getString())
            << "diffXml" << tempDiffStr.getString();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildInitXMLException)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String initStr;
    NCCHAR* dbPath = NULL;
    EXPECT_FALSE(m_test->BuildInitXML(2, dbPath, 2000, 3000, 1200, 1200, 600, initStr));
}

TEST_F(VR_MusicXMLBuilder_Test, BuildInitXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String initStr;
    NCCHAR* dbPath = "/var/user/MusicCatalog_2.db";
    EXPECT_TRUE(m_test->BuildInitXML(2, dbPath, 2000, 3000, 1200, 1200, 600, initStr));
    EXPECT_STREQ(VR_MUSICDB_INIT_TEST_XML, initStr.c_str())
            << "initXml" << initStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildActiveXMLException)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String activeStr;
    NCCHAR* dbPath = NULL;
    EXPECT_FALSE(m_test->BuildActiveXML(2, dbPath, 500, 400, 400, 200, false, "ipod", activeStr));
}

TEST_F(VR_MusicXMLBuilder_Test, BuildActiveXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String activeStr;
    NCCHAR* dbPath = "/var/user/MusicCatalog_2.db";
    EXPECT_TRUE(m_test->BuildActiveXML(2, dbPath, 500, 400, 400, 200, false, "ipod", activeStr));
    EXPECT_STREQ(VR_MUSICDB_ACTIVE_TEST_XML, activeStr.c_str())
            << "activeXml" << activeStr.c_str();
    EXPECT_TRUE(m_test->BuildActiveXML(2, dbPath, 500, 400, 400, 200, true, "ipod", activeStr));
}

TEST_F(VR_MusicXMLBuilder_Test, BuildInactiveXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String inactiveStr;
    EXPECT_TRUE(m_test->BuildInactiveXML(2, false, inactiveStr));
    EXPECT_STREQ(VR_MUSICDB_INACTIVE_TEST_XML, inactiveStr.c_str())
            << "inactiveXml" << inactiveStr.c_str();
    EXPECT_TRUE(m_test->BuildInactiveXML(2, true, inactiveStr));
}

TEST_F(VR_MusicXMLBuilder_Test, BuildPreUpdateXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String preUpdateStr;
    EXPECT_TRUE(m_test->BuildPreUpdateXML(1, preUpdateStr));
    EXPECT_STREQ(VR_MUSICDB_PREUPDATE_TEST_XML, preUpdateStr.c_str())
            << "preUpdateXml" << preUpdateStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildDevStartPlayStatusXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String playStatusStr;
    EXPECT_TRUE(m_test->BuildDevPlayStatusXML(true, playStatusStr));
    EXPECT_STREQ(VR_MUSICDEV_STARTPLAY_STATUS_TEST_XML, playStatusStr.c_str())
            << "playStatusXml" << playStatusStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildDevStopPlayStatusXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String playStatusStr;
    EXPECT_TRUE(m_test->BuildDevPlayStatusXML(false, playStatusStr));
    EXPECT_STREQ(VR_MUSICDEV_STOPPLAY_STATUS_TEST_XML, playStatusStr.c_str())
            << "playStatusXml" << playStatusStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildUSBConnStatusXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String usbConnStr;
    EXPECT_TRUE(m_test->BuildUSBConnStatusXML(1, true, usbConnStr));
    EXPECT_STREQ(VR_MUSICDEV_USB_CONNECT_STATUS_TEST_XML, usbConnStr.c_str())
            << "usbConnXml" << usbConnStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildUSBDisconnStatusXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String usbConnStr;
    EXPECT_TRUE(m_test->BuildUSBConnStatusXML(1, false, usbConnStr));
    EXPECT_STREQ(VR_MUSICDEV_USB_DISCONN_STATUS_TEST_XML, usbConnStr.c_str())
            << "usbConnXml" << usbConnStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildIpodConnStatusXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String ipodConnStr;
    EXPECT_TRUE(m_test->BuildIpodConnStatusXML(2, true, ipodConnStr));
    EXPECT_STREQ(VR_MUSICDEV_IPOD_CONNECT_STATUS_TEST_XML, ipodConnStr.c_str())
            << "ipodConnXml" << ipodConnStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildIpodDisconnStatusXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String ipodConnStr;
    EXPECT_TRUE(m_test->BuildIpodConnStatusXML(2, false, ipodConnStr));
    EXPECT_STREQ(VR_MUSICDEV_IPOD_DISCONN_STATUS_TEST_XML, ipodConnStr.c_str())
            << "ipodConnXml" << ipodConnStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildVRCancelXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String cancelXml;
    EXPECT_TRUE(m_test->BuildVRCancelXML(cancelXml));
    EXPECT_STREQ(VR_MUSICVR_CANCEL_TEST_XML, cancelXml.c_str())
            << "cancelXml" << cancelXml.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, BuildMagicStringXMLException)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String magicStr;
    NCCHAR* magic = NULL;
    EXPECT_FALSE(m_test->BuildMagicStringXML(magic, magicStr));
}

TEST_F(VR_MusicXMLBuilder_Test, BuildMagicStringXML)
{
    ASSERT_NE(0, (INT64)m_test);

    VR_String magicStr;
    NCCHAR* magic = "010203040506";
    EXPECT_TRUE(m_test->BuildMagicStringXML(magic, magicStr));
    EXPECT_STREQ(VR_MUSIC_MAGICSTRING_TEST_XML, magicStr.c_str())
            << "magicXml" << magicStr.c_str();
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNameException)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = NULL;
    const NCCHAR* mapName = m_test->FindXMLName(tableName);
    ASSERT_EQ(0, (INT64)mapName);
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNameAlbum)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = "MusicAgentAlbums";
    ASSERT_STREQ("album", m_test->FindXMLName(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNamePodcast)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = "MusicAgentPodcasts";
    ASSERT_STREQ("podcast", m_test->FindXMLName(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNameArtist)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = "MusicAgentArtists";
    ASSERT_STREQ("artist", m_test->FindXMLName(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNameAudiobook)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = "MusicAgentAudiobooks";
    ASSERT_STREQ("audiobook", m_test->FindXMLName(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNameComposer)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = "MusicAgentComposers";
    ASSERT_STREQ("composer", m_test->FindXMLName(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNameGenre)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = "MusicAgentGenres";
    ASSERT_STREQ("genre", m_test->FindXMLName(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNamePlaylist)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = "MusicAgentPlaylists";
    ASSERT_STREQ("playlist", m_test->FindXMLName(tableName));
}

TEST_F(VR_MusicXMLBuilder_Test, FindXMLNameSong)
{
    ASSERT_NE(0, (INT64)m_test);

    NCCHAR* tableName = "MusicAgentSongs";
    ASSERT_STREQ("song", m_test->FindXMLName(tableName));
}

}
}
/* EOF */
