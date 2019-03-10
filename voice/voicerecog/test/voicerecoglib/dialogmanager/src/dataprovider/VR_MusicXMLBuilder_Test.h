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
/**
 * @file VR_MusicXMLBuilder_Test.h
 * @brief Declaration file of class VR_MusicXMLBuilder_Test.
 *
 * This file includes the declaration of class VR_MusicXMLBuilder_Test.
 *
 * @attention used for C++ only.
 */
#ifndef VR_MUSIC_XML_BUILDER_TEST_H
#define VR_MUSIC_XML_BUILDER_TEST_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_MusicXMLBuilder.h"
#include "gtest/gtest.h"

namespace navi
{
namespace dataprovider
{
#define VR_MUSICDB_INIT_TEST_XML   "<grammar_init agent=\"media\" grammarid=\"2\" path=\"/var/user/MusicCatalog_2.db\" songitemcount=\"2000\" otheritemcount=\"3000\" generalitemcount=\"1200\" ucitemcount=\"1200\" genreitemcount=\"600\" />"
#define VR_MUSICDB_DIFF_TEST_XML   "<grammar_diff agent=\"media\" grammarid=\"1\" path=\"/var/user/MusicCatalog_1.db\" songitemcount=\"1000\" otheritemcount=\"2000\" generalitemcount=\"800\" ucitemcount=\"800\" genreitemcount=\"400\">" \
                                       "<category name=\"song\">" \
                                           "<delete>" \
                                               "<item id=\"5\" name=\"lemon tree\" />" \
                                               "<item id=\"9\" name=\"tomorrow\" />" \
                                           "</delete>" \
                                           "<add>" \
                                               "<item id=\"7\" name=\"yesterday once more\" pronid=\"-1\" pron=\"\" />" \
                                               "<item id=\"2\" name=\"show\" pronid=\"-1\" pron=\"\" />" \
                                           "</add>" \
                                       "</category>" \
                                   "</grammar_diff>"

#define VR_MUSICDB_ACTIVE_TEST_XML                "<grammar_active agent=\"media\" reply=\"false\" grammarid=\"2\" path=\"/var/user/MusicCatalog_2.db\" songitemcount=\"500\" generalitemcount=\"400\" ucitemcount=\"400\" genreitemcount=\"200\" />"
#define VR_MUSICDB_INACTIVE_TEST_XML              "<grammar_disactive agent=\"media\" reply=\"false\" grammarid=\"2\" />"
#define VR_MUSICDB_PREUPDATE_TEST_XML             "<grammar_preupdate agent=\"media\" grammarid=\"1\" />"
#define VR_MUSICDEV_STARTPLAY_STATUS_TEST_XML     "<event name=\"updateState\">" \
                                                      "<item key=\"MUSIC_SONG_INFORMATION_AVAILABLE\" value=\"true\" />" \
                                                  "</event>"
#define VR_MUSICDEV_STOPPLAY_STATUS_TEST_XML     "<event name=\"updateState\">" \
                                                      "<item key=\"MUSIC_SONG_INFORMATION_AVAILABLE\" value=\"false\" />" \
                                                  "</event>"
#define VR_MUSICDEV_USB_CONNECT_STATUS_TEST_XML   "<event name=\"updateState\">" \
                                                      "<item key=\"USB_1_CONNECTED\" value=\"true\" />" \
                                                  "</event>"
#define VR_MUSICDEV_USB_DISCONN_STATUS_TEST_XML   "<event name=\"updateState\">" \
                                                      "<item key=\"USB_1_CONNECTED\" value=\"false\" />" \
                                                  "</event>"
#define VR_MUSICDEV_IPOD_CONNECT_STATUS_TEST_XML  "<event name=\"updateState\">" \
                                                      "<item key=\"IPOD_2_CONNECTED\" value=\"true\" />" \
                                                  "</event>"
#define VR_MUSICDEV_IPOD_DISCONN_STATUS_TEST_XML  "<event name=\"updateState\">" \
                                                      "<item key=\"IPOD_2_CONNECTED\" value=\"false\" />" \
                                                  "</event>"
#define VR_MUSIC_MAGICSTRING_TEST_XML             "<event name=\"updateState\">" \
                                                      "<item key=\"MEDIAINFOKEY_MAGICSTRING\" value=\"010203040506\" />" \
                                                  "</event>"
#define VR_MUSICVR_CANCEL_TEST_XML                "<event name=\"cancel\"></event>"

/**
 * @brief The VR_MusicXMLBuilder_Test class
 *
 * class declaration.
 */
class VR_MusicXMLBuilder_Test : public testing::Test
{
public:
    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

protected:
    VR_MusicXMLBuilder_Test();
    ~VR_MusicXMLBuilder_Test();
    VOID SetUp();
    VOID TearDown();

private:
    VR_MusicXMLBuilder_Test(const VR_MusicXMLBuilder_Test&);
    VR_MusicXMLBuilder_Test& operator=(const VR_MusicXMLBuilder_Test&);

    // test suite level
    static VR_MusicXMLBuilder* m_testSuite;

    // test level
    VR_MusicXMLBuilder* m_test;
};

}
}
#endif /* VR_MUSIC_XML_BUILDER_TEST_H */
/* EOF */
