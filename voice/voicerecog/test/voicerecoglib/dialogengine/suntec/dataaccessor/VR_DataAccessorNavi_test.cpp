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

#include "VR_DataAccessorNavi.h"

#include "VR_Configure_mock.h"
#include <sqlite3.h>
#include "gtest/gtest.h"

using ::testing::Return;
using ::testing::_;
/**
* VR_DataAccessorNavi_Test
*
* The class is just for VR_DataAccessorNavi test.
*/
class VR_DataAccessorNavi_Test : public testing::Test
{
public:
    VR_DataAccessorNavi_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

    static VR_DataAccessorNavi *m_addressNavi;
    static VR_Configure_mock *m_configure;

private:
    VR_DataAccessorNavi_Test(const VR_DataAccessorNavi_Test& other);
    void operator= (const VR_DataAccessorNavi_Test& other);
};

VR_DataAccessorNavi *VR_DataAccessorNavi_Test::m_addressNavi = nullptr;
VR_Configure_mock *VR_DataAccessorNavi_Test::m_configure = nullptr;

void VR_DataAccessorNavi_Test::SetUpTestCase()
{
    m_configure = new VR_Configure_mock();
    ON_CALL(*m_configure, getMapDataPath()).WillByDefault(Return("/data/PP_Data"));
    ON_CALL(*m_configure, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*m_configure, getTTSLanguage(_)).WillByDefault(Return("ENG"));
    ON_CALL(*m_configure, getEngineType()).WillByDefault(Return(VR_ConfigureIF::VR_EngineType::SUNTEC));

    m_addressNavi = new VR_DataAccessorNavi(m_configure);
}

void VR_DataAccessorNavi_Test::TearDownTestCase()
{
    delete m_configure;
    m_configure = nullptr;
    delete m_addressNavi;
    m_addressNavi = nullptr;
}

TEST_F(VR_DataAccessorNavi_Test, getInfo)
{
    std::string reqMsg;
    std::string response;
    m_addressNavi->getInfo("_addressMapping", reqMsg, response);
    m_addressNavi->getInfo("_buildAddress", reqMsg, response);
    m_addressNavi->getInfo("_sendVoiceID", reqMsg, response);
    m_addressNavi->getInfo("_buildAddressName", reqMsg, response);
    m_addressNavi->getInfo("_getPOIPrompt", reqMsg, response);
    m_addressNavi->getInfo("_buildPointType", reqMsg, response);
    m_addressNavi->getInfo("otheroperation", reqMsg, response);
    SUCCEED();
}

TEST_F(VR_DataAccessorNavi_Test, isOperationHandled)
{
    EXPECT_FALSE(m_addressNavi->isOperationHandled(""));
    EXPECT_FALSE(m_addressNavi->isOperationHandled("otheroperation"));
    EXPECT_TRUE(m_addressNavi->isOperationHandled("_addressMapping"));
    EXPECT_TRUE(m_addressNavi->isOperationHandled("_buildAddress"));
    EXPECT_TRUE(m_addressNavi->isOperationHandled("_sendVoiceID"));
    EXPECT_TRUE(m_addressNavi->isOperationHandled("_buildAddressName"));
    EXPECT_TRUE(m_addressNavi->isOperationHandled("_getPOIPrompt"));
    EXPECT_TRUE(m_addressNavi->isOperationHandled("_buildPointType"));
}

TEST_F(VR_DataAccessorNavi_Test, setCountryID)
{
    m_addressNavi->setCountryID("1");
    SUCCEED();
}

TEST_F(VR_DataAccessorNavi_Test, sendVoiceID)
{
    // char teseOne[10] = "12345678";
    // char teseTwo[10] = "87654321";
    // char testThree[2] = "1";
    // char testFour[2] = "2";
    // VoiceList<std::string>::type stringList;
    // char *result[2];
    // result[0] = teseOne;
    // result[1] = testThree;
    // m_addressNavi->getSearchIDCallback(&stringList, 0, result, nullptr);

    // result[0] = teseTwo;
    // result[1] = testFour;
    // m_addressNavi->getSearchIDCallback(&stringList, 0, result, nullptr);

    std::string reqMsg;
    std::string response;

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
                                    "<zoneId confidence=\"4720\">1</zoneId>"
                                    "<cityId confidence=\"5000\"></cityId>"
                                    "<streetId confidence=\"5000\"></streetId>"
                                    "<houseNumber>13</houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    m_addressNavi->addressMapping(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STATE</buildType>"
                    "<addressInfo>"
                        "<zoneId>''</zoneId>"
                        "<zoneNameIndex>''</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>''</cityId>"
                        "<cityNameIndex>''</cityNameIndex>"
                        "<city>''</city>"
                        "<streetExist>''</streetExist>"
                        "<streetId>''</streetId>"
                        "<streetNameIndex>''</streetNameIndex>"
                        "<street>''</street>"
                        "<houseNumberExist>''</houseNumberExist>"
                        "<houseNumber>''</houseNumber>"
                    "</addressInfo>"
                    "<asrResult>"
                        "<list id=\"asr12\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>1</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">1</zoneId>"
                                    "<cityId confidence=\"4325\"></cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_addressNavi->buildAddress(reqMsg, response);

    reqMsg = "<data>"
                "<type>CITY</type>"
                "<info>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<country>''</country>"
                        "<zone>Eastern Cape</zone>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<cityId>18155146735255552</cityId>"
                        "<city>''</city>"
                        "<cityNameIndex>0</cityNameIndex>"
                        "<streetExist>''</streetExist>"
                        "<streetId>2924</streetId>"
                        "<street>''</street>"
                        "<streetNameIndex>0</streetNameIndex>"
                        "<houseNumberExist>''</houseNumberExist>"
                        "<houseNumber>''</houseNumber>"
                    "</addressInfo>"
                "</info>"
            "</data>";
    response.clear();
    m_addressNavi->sendVoiceID(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>CITY</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>''</cityId>"
                        "<cityNameIndex>''</cityNameIndex>"
                        "<city>''</city>"
                        "<streetExist>''</streetExist>"
                        "<streetId>''</streetId>"
                        "<streetNameIndex>''</streetNameIndex>"
                        "<street>''</street>"
                        "<houseNumberExist>''</houseNumberExist>"
                        "<houseNumber>''</houseNumber>"
                    "</addressInfo>"
                    "<asrResult>"
                        "<list id=\"asr12\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>1</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"0\"/>"
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_addressNavi->buildAddress(reqMsg, response);

    reqMsg = "<data>"
                "<type>STREET</type>"
                "<info>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<country>''</country>"
                        "<zone>Eastern Cape</zone>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<cityId>18155146735255552</cityId>"
                        "<city>City of Cape Town</city>"
                        "<cityNameIndex>0</cityNameIndex>"
                        "<streetExist>''</streetExist>"
                        "<streetId>2924</streetId>"
                        "<street>''</street>"
                        "<streetNameIndex>0</streetNameIndex>"
                        "<houseNumberExist>''</houseNumberExist>"
                        "<houseNumber>''</houseNumber>"
                    "</addressInfo>"
                "</info>"
            "</data>";
    response.clear();
    m_addressNavi->sendVoiceID(reqMsg, response);

    reqMsg = "<data>"
                "<type>STATE</type>"
                "<info>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<country>''</country>"
                        "<zone>Eastern Cape</zone>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<cityId>18155146735255552</cityId>"
                        "<city>''</city>"
                        "<cityNameIndex>0</cityNameIndex>"
                        "<streetExist>''</streetExist>"
                        "<streetId>2924</streetId>"
                        "<street>''</street>"
                        "<streetNameIndex>0</streetNameIndex>"
                        "<houseNumberExist>''</houseNumberExist>"
                        "<houseNumber>''</houseNumber>"
                    "</addressInfo>"
                "</info>"
            "</data>";
    response.clear();
    m_addressNavi->sendVoiceID(reqMsg, response);

    reqMsg = "<data>"
                "<type>STATE</type>"
                "<info>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<country>''</country>"
                        "<zone>Eastern Cape</zone>"
                        "<zoneNameIndex>''</zoneNameIndex>"
                        "<cityId>18155146735255552</cityId>"
                        "<city>''</city>"
                        "<cityNameIndex>''</cityNameIndex>"
                        "<streetExist>''</streetExist>"
                        "<streetId>2924</streetId>"
                        "<street>''</street>"
                        "<streetNameIndex>0</streetNameIndex>"
                        "<houseNumberExist>''</houseNumberExist>"
                        "<houseNumber>''</houseNumber>"
                    "</addressInfo>"
                "</info>"
            "</data>";
    response.clear();
    m_addressNavi->sendVoiceID(reqMsg, response);
    SUCCEED();
}

TEST_F(VR_DataAccessorNavi_Test, processRequestMsg)
{
    std::string inputMsg;
    std::string outputMsg;
    inputMsg = "<items>"
                "<item>"
                    "<CMN2031_navi_addr_state_x confidence=\"4720\">3000002</CMN2031_navi_addr_state_x>"
                    "<CMN2032_navi_addr_city_x confidence=\"0\">4000001</CMN2032_navi_addr_city_x>"
                    "<CMN2033_navi_addr_street_x confidence=\"0\">8000008</CMN2033_navi_addr_street_x>"
                    "<number confidence=\"0\">11</number>"
                "</item>"
                "<item>"
                    "<CMN2031_navi_addr_state_x confidence=\"4744\">3000008</CMN2031_navi_addr_state_x>"
                    "<CMN2032_navi_addr_city_x confidence=\"0\">4000005</CMN2032_navi_addr_city_x>"
                    "<CMN2033_navi_addr_street_x confidence=\"0\">8000012</CMN2033_navi_addr_street_x>"
                    "<number confidence=\"0\">8</number>"
                "</item>"
                "<item>"
                    "<CMN2031_navi_addr_state_x confidence=\"4474\">3000008</CMN2031_navi_addr_state_x>"
                    "<CMN2032_navi_addr_city_x confidence=\"0\">4000008</CMN2032_navi_addr_city_x>"
                    "<CMN2033_navi_addr_street_x confidence=\"0\">8000001</CMN2033_navi_addr_street_x>"
                    "<number confidence=\"0\">911</number>"
                "</item>"
            "</items>";
    m_addressNavi->processRequestMsg(inputMsg, outputMsg);
    SUCCEED();
}

TEST_F(VR_DataAccessorNavi_Test, buildPointType)
{
    pugi::xml_document expectDoc;
    std::ostringstream oss;
    std::string reqMsg;
    std::string response;

    expectDoc.load_string("<data><needSelect>false</needSelect><defaultGuidePoint /><selectGuidePoint /></data>");
    expectDoc.print(oss);
    std::string result = oss.str();
    oss.str("");

    // empty input
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // guidePoint node empty
    reqMsg = "<result />";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // list node empty
    reqMsg = "<guidePoint /><result />";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // header node empty
    reqMsg = "<guidePoint />"
             "<result>"
                 "<list />"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // count node empty
    reqMsg = "<guidePoint />"
             "<result>"
                 "<list>"
                     "<header />"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // items node empty
    reqMsg = "<guidePoint>"
                 "samplePoint"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count></count>"
                     "</header>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // count == 1
    expectDoc.load_string("<data>"
                            "<needSelect>false</needSelect>"
                            "<defaultGuidePoint>samplePoint</defaultGuidePoint>"
                            "<selectGuidePoint />"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "samplePoint"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>1</count>"
                     "</header>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // count == 2
    expectDoc.load_string("<data>"
                            "<needSelect>true</needSelect>"
                            "<defaultGuidePoint>samplePoint</defaultGuidePoint>"
                            "<selectGuidePoint>samplePoint</selectGuidePoint>"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "samplePoint"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>2</count>"
                     "</header>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // item node empty
    expectDoc.load_string("<data>"
                            "<needSelect>false</needSelect>"
                            "<defaultGuidePoint>samplePoint</defaultGuidePoint>"
                            "<selectGuidePoint />"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "samplePoint"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>1</count>"
                     "</header>"
                     "<items />"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // item guidePoint node empty
    expectDoc.load_string("<data>"
                            "<needSelect>false</needSelect>"
                            "<defaultGuidePoint>samplePoint</defaultGuidePoint>"
                            "<selectGuidePoint />"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "samplePoint"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>1</count>"
                     "</header>"
                     "<items>"
                         "<item />"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // item guidePoint node invalid
    expectDoc.load_string("<data>"
                            "<needSelect>false</needSelect>"
                            "<defaultGuidePoint>samplePoint</defaultGuidePoint>"
                            "<selectGuidePoint />"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "samplePoint"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>1</count>"
                     "</header>"
                     "<items>"
                         "<item>"
                             "<guidePoint />"
                         "</item>"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // guidePoint one
    expectDoc.load_string("<data>"
                            "<needSelect>false</needSelect>"
                            "<defaultGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>120E</longitude>"
                                "<latitude>36N</latitude>"
                                "<linkId>2</linkId>"
                            "</defaultGuidePoint>"
                            "<selectGuidePoint />"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "<displayGuideType></displayGuideType>"
                 "<longitude>110E</longitude>"
                 "<latitude>26N</latitude>"
                 "<linkId>1</linkId>"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>1</count>"
                     "</header>"
                     "<items>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude>120E</longitude>"
                                 "<latitude>36N</latitude>"
                                 "<linkId>2</linkId>"
                             "</guidePoint>"
                         "</item>"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_TRUE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // guidePoint two
    expectDoc.load_string("<data>"
                            "<needSelect>true</needSelect>"
                            "<defaultGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>120E</longitude>"
                                "<latitude>36N</latitude>"
                                "<linkId>2</linkId>"
                            "</defaultGuidePoint>"
                            "<selectGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>130E</longitude>"
                                "<latitude>46N</latitude>"
                                "<linkId>3</linkId>"
                            "</selectGuidePoint>"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "<displayGuideType></displayGuideType>"
                 "<longitude>110E</longitude>"
                 "<latitude>26N</latitude>"
                 "<linkId>1</linkId>"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>2</count>"
                     "</header>"
                     "<items>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude>120E</longitude>"
                                 "<latitude>36N</latitude>"
                                 "<linkId>2</linkId>"
                             "</guidePoint>"
                         "</item>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude>130E</longitude>"
                                 "<latitude>46N</latitude>"
                                 "<linkId>3</linkId>"
                             "</guidePoint>"
                         "</item>"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_TRUE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    expectDoc.load_string("<data>"
                            "<needSelect>true</needSelect>"
                            "<defaultGuidePoint>"
                                "<displayGuideType>DEFAULT</displayGuideType>"
                                "<longitude>130E</longitude>"
                                "<latitude>46N</latitude>"
                                "<linkId>3</linkId>"
                            "</defaultGuidePoint>"
                            "<selectGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>120E</longitude>"
                                "<latitude>36N</latitude>"
                                "<linkId>2</linkId>"
                            "</selectGuidePoint>"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "<displayGuideType></displayGuideType>"
                 "<longitude>110E</longitude>"
                 "<latitude>26N</latitude>"
                 "<linkId>1</linkId>"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>2</count>"
                     "</header>"
                     "<items>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude>120E</longitude>"
                                 "<latitude>36N</latitude>"
                                 "<linkId>2</linkId>"
                             "</guidePoint>"
                         "</item>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType>DEFAULT</displayGuideType>"
                                 "<longitude>130E</longitude>"
                                 "<latitude>46N</latitude>"
                                 "<linkId>3</linkId>"
                             "</guidePoint>"
                         "</item>"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_TRUE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    expectDoc.load_string("<data>"
                            "<needSelect>true</needSelect>"
                            "<defaultGuidePoint>"
                                "<displayGuideType>DEFAULT</displayGuideType>"
                                "<longitude>130E</longitude>"
                                "<latitude>46N</latitude>"
                                "<linkId>3</linkId>"
                            "</defaultGuidePoint>"
                            "<selectGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>110E</longitude>"
                                "<latitude>26N</latitude>"
                                "<linkId>1</linkId>"
                            "</selectGuidePoint>"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "<displayGuideType></displayGuideType>"
                 "<longitude>110E</longitude>"
                 "<latitude>26N</latitude>"
                 "<linkId>1</linkId>"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>2</count>"
                     "</header>"
                     "<items>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude></longitude>"
                                 "<latitude></latitude>"
                                 "<linkId>2</linkId>"
                             "</guidePoint>"
                         "</item>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType>DEFAULT</displayGuideType>"
                                 "<longitude>130E</longitude>"
                                 "<latitude>46N</latitude>"
                                 "<linkId>3</linkId>"
                             "</guidePoint>"
                         "</item>"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_TRUE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    expectDoc.load_string("<data>"
                            "<needSelect>true</needSelect>"
                            "<defaultGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>130E</longitude>"
                                "<latitude>46N</latitude>"
                                "<linkId>3</linkId>"
                            "</defaultGuidePoint>"
                            "<selectGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>110E</longitude>"
                                "<latitude>26N</latitude>"
                                "<linkId>1</linkId>"
                            "</selectGuidePoint>"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "<displayGuideType></displayGuideType>"
                 "<longitude>110E</longitude>"
                 "<latitude>26N</latitude>"
                 "<linkId>1</linkId>"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>2</count>"
                     "</header>"
                     "<items>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude></longitude>"
                                 "<latitude></latitude>"
                                 "<linkId>2</linkId>"
                             "</guidePoint>"
                         "</item>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude>130E</longitude>"
                                 "<latitude>46N</latitude>"
                                 "<linkId>3</linkId>"
                             "</guidePoint>"
                         "</item>"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_TRUE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    expectDoc.load_string("<data>"
                            "<needSelect>true</needSelect>"
                            "<defaultGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>120E</longitude>"
                                "<latitude>36N</latitude>"
                                "<linkId>2</linkId>"
                            "</defaultGuidePoint>"
                            "<selectGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>110E</longitude>"
                                "<latitude>26N</latitude>"
                                "<linkId>1</linkId>"
                            "</selectGuidePoint>"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "<displayGuideType></displayGuideType>"
                 "<longitude>110E</longitude>"
                 "<latitude>26N</latitude>"
                 "<linkId>1</linkId>"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>2</count>"
                     "</header>"
                     "<items>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude>120E</longitude>"
                                 "<latitude>36N</latitude>"
                                 "<linkId>2</linkId>"
                             "</guidePoint>"
                         "</item>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude></longitude>"
                                 "<latitude></latitude>"
                                 "<linkId>3</linkId>"
                             "</guidePoint>"
                         "</item>"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_TRUE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    expectDoc.load_string("<data>"
                            "<needSelect>true</needSelect>"
                            "<defaultGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>110E</longitude>"
                                "<latitude>26N</latitude>"
                                "<linkId>1</linkId>"
                            "</defaultGuidePoint>"
                            "<selectGuidePoint>"
                                "<displayGuideType></displayGuideType>"
                                "<longitude>110E</longitude>"
                                "<latitude>26N</latitude>"
                                "<linkId>1</linkId>"
                            "</selectGuidePoint>"
                        "</data>");
    expectDoc.print(oss);
    result = oss.str();
    oss.str("");

    reqMsg = "<guidePoint>"
                 "<displayGuideType></displayGuideType>"
                 "<longitude>110E</longitude>"
                 "<latitude>26N</latitude>"
                 "<linkId>1</linkId>"
             "</guidePoint>"
             "<result>"
                 "<list>"
                     "<header>"
                         "<count>2</count>"
                     "</header>"
                     "<items>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude></longitude>"
                                 "<latitude></latitude>"
                                 "<linkId>2</linkId>"
                             "</guidePoint>"
                         "</item>"
                         "<item>"
                             "<guidePoint>"
                                 "<displayGuideType></displayGuideType>"
                                 "<longitude></longitude>"
                                 "<latitude></latitude>"
                                 "<linkId>3</linkId>"
                             "</guidePoint>"
                         "</item>"
                     "</items>"
                 "</list>"
             "</result>";
    response.clear();
    EXPECT_FALSE(m_addressNavi->buildPointType(reqMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());
}
/* EOF */