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

#include "VR_NaviAddressCommon.h"

#include "VR_Configure_mock.h"
#include <sqlite3.h>
#include "gtest/gtest.h"

using ::testing::Return;
using ::testing::_;

/**
* VR_NaviAddressCommon_Test
*
* The class is just for VR_NaviAddressCommon test.
*/
class VR_NaviAddressCommon_Test : public testing::Test
{
public:
    VR_NaviAddressCommon_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

    static VR_NaviAddressCommon *m_naviAddress;
    static VR_NaviAddressCommon *m_naviAddressOld;
    static VR_Configure_mock *m_configure;

private:
    VR_NaviAddressCommon_Test(const VR_NaviAddressCommon_Test& other);
    void operator= (const VR_NaviAddressCommon_Test& other);
};

VR_NaviAddressCommon *VR_NaviAddressCommon_Test::m_naviAddress = nullptr;
VR_NaviAddressCommon *VR_NaviAddressCommon_Test::m_naviAddressOld = nullptr;
VR_Configure_mock *VR_NaviAddressCommon_Test::m_configure = nullptr;

void VR_NaviAddressCommon_Test::SetUpTestCase()
{
    m_configure = new VR_Configure_mock();
    ON_CALL(*m_configure, getMapDataPath()).WillByDefault(Return("/data/PP_Data"));
    ON_CALL(*m_configure, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*m_configure, getTTSLanguage(_)).WillByDefault(Return("ENG"));
    ON_CALL(*m_configure, getEngineType()).WillByDefault(Return(VR_ConfigureIF::VR_EngineType::SUNTEC));
    // ON_CALL(*m_configure, setHybridVRUrl("abc")).WillByDefault(Return());
    // ON_CALL(*m_configure, getHybridVRUrl()).WillByDefault(Return("abc"));
    // eVR_AppStatus eType;
    // ON_CALL(*m_configure, setAppStatus(eType, "OK")).WillByDefault(Return());
    // ON_CALL(*m_configure, getAppStatus(eType, "OK")).WillByDefault(Return());

    m_naviAddress = new VR_NaviAddressCommon(m_configure);

    ON_CALL(*m_configure, getMapDataPath()).WillByDefault(Return("/data/TP_Data"));
    m_naviAddressOld = new VR_NaviAddressCommon(m_configure);

    ON_CALL(*m_configure, getMapDataPath()).WillByDefault(Return("/data/PP_Data"));
}

void VR_NaviAddressCommon_Test::TearDownTestCase()
{
    delete m_configure;
    m_configure = nullptr;
    delete m_naviAddress;
    m_naviAddress = nullptr;
    delete m_naviAddressOld;
    m_naviAddressOld = nullptr;
}

TEST_F(VR_NaviAddressCommon_Test, setCountryID)
{
    m_naviAddress->setCountryID("1");
    m_naviAddressOld->setCountryID("1");
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, addressMapping)
{
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
                                    "<zoneId confidence=\"4720\">3</zoneId>"
                                    "<cityId confidence=\"0\"/>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"4744\">2</zoneId>"
                                    "<cityId confidence=\"0\"/>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"4474\">3</zoneId>"
                                    "<cityId confidence=\"0\"/>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

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
                                    "<zoneId confidence=\"4720\">3</zoneId>"
                                    "<cityId confidence=\"5000\">1013</cityId>"
                                    "<streetId confidence=\"5000\"></streetId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"4744\">1</zoneId>"
                                    "<cityId confidence=\"5000\">1002</cityId>"
                                    "<streetId confidence=\"5000\"></streetId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"4474\">3</zoneId>"
                                    "<cityId confidence=\"5000\">1017</cityId>"
                                    "<streetId confidence=\"5000\"></streetId>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

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
                                    "<zoneId confidence=\"4720\">3</zoneId>"
                                    "<cityId confidence=\"5000\">1013</cityId>"
                                    "<streetId confidence=\"5000\">335</streetId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"4744\">1</zoneId>"
                                    "<cityId confidence=\"5000\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"4474\">3</zoneId>"
                                    "<cityId confidence=\"5000\">1017</cityId>"
                                    "<streetId confidence=\"5000\">4471</streetId>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

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
                                    "<zoneId confidence=\"4720\">3</zoneId>"
                                    "<cityId confidence=\"5000\">1013</cityId>"
                                    "<streetId confidence=\"5000\">335</streetId>"
                                    "<houseNumber>13</houseNumber>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"4744\">1</zoneId>"
                                    "<cityId confidence=\"5000\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber>7</houseNumber>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"4474\">3</zoneId>"
                                    "<cityId confidence=\"5000\">1017</cityId>"
                                    "<streetId confidence=\"5000\">4471</streetId>"
                                    "<houseNumber>5</houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

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
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

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
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

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
                                    "<zoneId confidence=\"4720\"></zoneId>"
                                    "<cityId confidence=\"5000\"></cityId>"
                                    "<streetId confidence=\"5000\"></streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>5</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>2</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">3</zoneId>"
                                    "<cityId confidence=\"5000\">1013</cityId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\"></zoneId>"
                                    "<cityId confidence=\"5000\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4478</streetId>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>5</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>2</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\"></zoneId>"
                                    "<cityId confidence=\"5000\"></cityId>"
                                    "<streetId confidence=\"5000\"></streetId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\"></zoneId>"
                                    "<cityId confidence=\"5000\"></cityId>"
                                    "<streetId confidence=\"5000\"></streetId>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_naviAddress->addressMapping(reqMsg, response);
    m_naviAddressOld->addressMapping(reqMsg, response);

    ON_CALL(*m_configure, getEngineType()).WillByDefault(Return(VR_ConfigureIF::VR_EngineType::VBT));
    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list id=\"asr2\">"
                            "<header>"
                                "<pageSize>5</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>2</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">3</zoneId>"
                                    "<cityId confidence=\"5000\">1013</cityId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\"></zoneId>"
                                    "<cityId confidence=\"5000\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4478</streetId>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_naviAddress->addressMapping(reqMsg, response);

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
                                    "<streetId confidence=\"5000\">335</streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
              "</data>";
    response.clear();
    m_naviAddress->addressMapping(reqMsg, response);
    ON_CALL(*m_configure, getEngineType()).WillByDefault(Return(VR_ConfigureIF::VR_EngineType::SUNTEC));
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, buildAddress)
{
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
    m_naviAddress->addressMapping(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STATE</buildType>"
                    "<addressInfo>"
                        "<zoneId>''</zoneId>"
                        "<zoneNameIndex>''</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>''</zone>"
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
    m_naviAddress->buildAddress(reqMsg, response);
    // m_naviAddressOld->buildAddress(reqMsg, response);

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
    m_naviAddress->buildAddress(reqMsg, response);
    // m_naviAddressOld->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
                        "<cityNameIndex>0</cityNameIndex>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex>''</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
                        "<cityNameIndex>0</cityNameIndex>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex></zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
                        "<cityNameIndex>0</cityNameIndex>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex></zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
                        "<cityNameIndex></cityNameIndex>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->buildAddress(reqMsg, response);
    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex></zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->buildAddress(reqMsg, response);
    // m_naviAddressOld->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>NUMBER</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
                        "<cityNameIndex>0</cityNameIndex>"
                        "<city>''</city>"
                        "<streetExist>''</streetExist>"
                        "<streetId>2924</streetId>"
                        "<streetNameIndex>0</streetNameIndex>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber>7</houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->buildAddress(reqMsg, response);
    // m_naviAddressOld->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>NUMBER</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
                        "<cityNameIndex>0</cityNameIndex>"
                        "<city>''</city>"
                        "<streetExist>''</streetExist>"
                        "<streetId>2924</streetId>"
                        "<streetNameIndex>0</streetNameIndex>"
                        "<street>''</street>"
                        "<houseNumberExist>''</houseNumberExist>"
                        "<houseNumber>''</houseNumber>"
                    "</addressInfo>"
                    "<asrResult>"
                        "<list id=\"asr12\">"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>2</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">1</zoneId>"
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber>7</houseNumber>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">1</zoneId>"
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                    "<data>"
                        "<buildType>STATE</buildType>"
                        "<addressInfo>"
                            "<zoneId>''</zoneId>"
                            "<zoneNameIndex>''</zoneNameIndex>"
                            "<country>''</country>"
                            "<zone>''</zone>"
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
                                        "<zoneId confidence=\"0\"></zoneId>"
                                        "<cityId confidence=\"4325\"></cityId>"
                                        "<streetId confidence=\"0\"/>"
                                    "</item>"
                                "</items>"
                            "</list>"
                        "</asrResult>"
                    "</data>"
                "</data>";
    response.clear();
    m_naviAddress->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>CITY</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->setCountryID("5");
    m_naviAddress->buildAddress(reqMsg, response);

    response.clear();
    m_naviAddress->setCountryID("1");
    m_naviAddress->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex>''</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->setCountryID("5");
    m_naviAddress->buildAddress(reqMsg, response);

    response.clear();
    m_naviAddress->setCountryID("1");
    m_naviAddress->buildAddress(reqMsg, response);

    reqMsg = "<data id=\"_addressMapping\">"
                "<data>"
                    "<index>0</index>"
                    "<asrResult>"
                        "<list/>"
                    "</asrResult>"
                "</data>"
              "</data>";
    m_naviAddress->addressMapping(reqMsg, response);

    reqMsg = "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex></zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>18155146735255552</cityId>"
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
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"5000\">4474</streetId>"
                                    "<houseNumber></houseNumber>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    response.clear();
    m_naviAddress->m_searchIDVoiceIDCacheMap.clear();
    m_naviAddress->buildAddress(reqMsg, response);

    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, buildAddressName)
{
    std::string reqMsg;
    std::string response;
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
            "<houseNumber>102</houseNumber>"
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
    m_naviAddress->buildAddressName(reqMsg, response);

    ON_CALL(*m_configure, getVRLanguage()).WillByDefault(Return("zh-hk"));
    m_naviAddress->buildAddressName(reqMsg, response);

    ON_CALL(*m_configure, getVRLanguage()).WillByDefault(Return("en-gb"));
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, getGlobalVoiceID)
{
    std::string globalVoiceID;
    m_naviAddress->getGlobalVoiceID("123", globalVoiceID);
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, getTableName)
{
    m_naviAddress->setCountryID("");
    m_naviAddress->getTableName(true);

    m_naviAddress->setCountryID("1");
    m_naviAddress->getTableName(true);
    m_naviAddress->getTableName(false);
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, getSingleListCallback)
{
    char testOne[2] = "";
    char testTwo[8] = "123465";
    VoiceList<std::string>::type stringList;
    char *result[2];
    result[0] = nullptr;
    m_naviAddress->getSingleListCallback(&stringList, 0, result, nullptr);

    result[0] = testOne;
    m_naviAddress->getSingleListCallback(&stringList, 0, result, nullptr);

    result[0] = testTwo;
    m_naviAddress->getSingleListCallback(&stringList, 0, result, nullptr);
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, getSearchIDCallback)
{
    char testOne[4] = "123";
    char testTwo[2] = "1";
    char testThree[2] = "";
    char testFour[9] = "123,456,";
    char testFive[12] = "123,456,789";
    VoiceList<std::string>::type stringList;
    char *result[2];
    result[0] = nullptr;
    result[1] = nullptr;
    m_naviAddress->getSearchIDCallback(&stringList, 0, result, nullptr);

    result[0] = testOne;
    result[1] = nullptr;
    m_naviAddress->getSearchIDCallback(&stringList, 0, result, nullptr);

    result[0] = nullptr;
    result[1] = testTwo;
    m_naviAddress->getSearchIDCallback(&stringList, 0, result, nullptr);

    result[0] = testOne;
    result[1] = testTwo;
    m_naviAddress->getSearchIDCallback(&stringList, 0, result, nullptr);

    result[0] = testThree;
    result[1] = testTwo;
    m_naviAddress->getSearchIDCallback(&stringList, 0, result, nullptr);

    result[0] = testFive;
    result[1] = testTwo;
    m_naviAddress->getSearchIDCallback(&stringList, 0, result, nullptr);

    result[0] = testFour;
    result[1] = testTwo;
    m_naviAddress->getSearchIDCallback(&stringList, 0, result, nullptr);
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, getOldDataSearchIDList)
{
    VoiceMultiMap<std::string, std::string>::type tmpMap = m_naviAddress->m_searchIDVoiceIDCacheMap;
    VoiceList<std::string>::type globalVoiceIDList;
    VoiceList<std::string>::type searchIDList;
    globalVoiceIDList.push_back("562949953421312");
    globalVoiceIDList.push_back("1125899906842624");
    m_naviAddress->getOldDataSearchIDList(m_naviAddress->getTableName(), globalVoiceIDList, searchIDList, true);
    m_naviAddress->getOldDataSearchIDList("abc", globalVoiceIDList, searchIDList, true);

    searchIDList.clear();
    m_naviAddress->getOldDataSearchIDList(m_naviAddress->getTableName(), globalVoiceIDList, searchIDList, false);
    ON_CALL(*m_configure, getEngineType()).WillByDefault(Return(VR_ConfigureIF::VR_EngineType::VBT));
    m_naviAddress->getOldDataSearchIDList(m_naviAddress->getTableName(), globalVoiceIDList, searchIDList, true);
    m_naviAddress->m_searchIDVoiceIDCacheMap = tmpMap;
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, getNewDataSearchIDList)
{
    VoiceMultiMap<std::string, std::string>::type tmpMap = m_naviAddress->m_searchIDVoiceIDCacheMap;
    VoiceList<std::string>::type globalVoiceIDList;
    VoiceList<std::string>::type searchIDList;
    globalVoiceIDList.push_back("562949953421312");
    globalVoiceIDList.push_back("1125899906842624");
    m_naviAddress->getNewDataSearchIDList(m_naviAddress->getTableName(), globalVoiceIDList, searchIDList, true);
    ON_CALL(*m_configure, getEngineType()).WillByDefault(Return(VR_ConfigureIF::VR_EngineType::SUNTEC));
    m_naviAddress->getNewDataSearchIDList("abc", globalVoiceIDList, searchIDList, true);
    m_naviAddress->getNewDataSearchIDList(m_naviAddress->getTableName(), globalVoiceIDList, searchIDList, true);

    searchIDList.clear();
    m_naviAddress->getNewDataSearchIDList(m_naviAddress->getTableName(), globalVoiceIDList, searchIDList, false);

    m_naviAddress->m_searchIDVoiceIDCacheMap = tmpMap;
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, processReceivedSearchID)
{
    std::string searchID = "1";
    std::string tmp = m_naviAddress->m_dataVersion;
    m_naviAddress->m_dataVersion.clear();
    m_naviAddress->processReceivedSearchID(searchID, "0");

    m_naviAddress->m_dataVersion = "test";
    m_naviAddress->processReceivedSearchID(searchID, "0");

    m_naviAddress->m_dataVersion = tmp;
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, processSearchID)
{
    VoiceList<std::string>::type searchIDList;
    searchIDList.push_back("1_0");
    searchIDList.push_back("2");
    VoiceList<std::string>::const_iterator it = searchIDList.begin();

    std::string searchID;
    std::string nameIndex;

    std::string tmp = m_naviAddress->m_dataVersion;
    m_naviAddress->m_dataVersion.clear();

    m_naviAddress->processSearchID(searchID, nameIndex, it);

    m_naviAddress->m_dataVersion = "test";
    m_naviAddress->processSearchID(searchID, nameIndex, it);

    ++it;
    m_naviAddress->processSearchID(searchID, nameIndex, it);

    m_naviAddress->m_dataVersion = tmp;
    SUCCEED();
}

TEST_F(VR_NaviAddressCommon_Test, querySearchID_1)
{
    VoiceList<std::string>::type zoneGlobalVoiceIDList;
    VoiceList<std::string>::type cityGlobalVoiceIDList;
    VoiceList<std::string>::type streetGlobalVoiceIDList;

    VR_Configure_mock configObj;
    // DB is not OK
    ON_CALL(configObj, getMapDataPath()).WillByDefault(Return("/data/PP"));
    ON_CALL(configObj, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(configObj, getTTSLanguage(_)).WillByDefault(Return("ENG"));

    VR_NaviAddressCommon naviAddressObj(&configObj);
    EXPECT_FALSE(naviAddressObj.querySearchID(zoneGlobalVoiceIDList, cityGlobalVoiceIDList, streetGlobalVoiceIDList));

}

// TEST_F(VR_NaviAddressCommon_Test, querySearchID_2)
// {
//    VoiceList<std::string>::type zoneGlobalVoiceIDList;
//    VoiceList<std::string>::type cityGlobalVoiceIDList;
//    VoiceList<std::string>::type streetGlobalVoiceIDList;
//
//    EXPECT_TRUE(m_naviAddress->querySearchID(zoneGlobalVoiceIDList, cityGlobalVoiceIDList, streetGlobalVoiceIDList));
//
// }

TEST_F(VR_NaviAddressCommon_Test, queryCitySearchID_1)
{
    pugi::xpath_node_set cityNodes;
    std::string zoneGlobalVoiceID;
    VoiceList<std::string>::type citySearchIDList;

    VR_Configure_mock configObj;
    // DB is not OK
    ON_CALL(configObj, getMapDataPath()).WillByDefault(Return("/data/PP"));
    ON_CALL(configObj, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(configObj, getTTSLanguage(_)).WillByDefault(Return("ENG"));

    VR_NaviAddressCommon naviAddressObj(&configObj);
    EXPECT_FALSE(naviAddressObj.queryCitySearchID(cityNodes, zoneGlobalVoiceID, citySearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, queryCitySearchID_2)
{
    pugi::xpath_node_set cityNodes;
    std::string zoneGlobalVoiceID;
    VoiceList<std::string>::type citySearchIDList;

    m_naviAddress->setCountryID("");
    EXPECT_FALSE(m_naviAddress->queryCitySearchID(cityNodes, zoneGlobalVoiceID, citySearchIDList));
    m_naviAddress->setCountryID("1");
}

TEST_F(VR_NaviAddressCommon_Test, queryCitySearchID_3)
{
    std::string reqMsg =
            "<data id=\"_buildAddress\">"
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
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">1</zoneId>"
                                    "<cityId confidence=\"0\"></cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">2</zoneId>"
                                    "<cityId confidence=\"0\"></cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\"/>3</zoneId>"
                                    "<cityId confidence=\"0\"></cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    pugi::xml_node asrResuldNode = msgDoc.select_node((std::string("//") + "asrResult").c_str()).node();
    std::ostringstream oss;
    asrResuldNode.print(oss);
    pugi::xml_document asrResuldNodeDoc;
    asrResuldNodeDoc.load_string(oss.str().c_str());
    pugi::xpath_node_set cityNodes = asrResuldNodeDoc.select_nodes((std::string("//") + "cityId").c_str());

    pugi::xml_node addressInfoNode = msgDoc.select_node((std::string("//") + "addressInfo").c_str()).node();
    std::string zoneSearchID(addressInfoNode.child("zoneId").text().as_string());
    std::string zoneNameIndex(addressInfoNode.child("zoneNameIndex").text().as_string());

    m_naviAddress->processReceivedSearchID(zoneSearchID, zoneNameIndex);
    std::string zoneGlobalVoiceID;
    m_naviAddress->getGlobalVoiceID(zoneSearchID, zoneGlobalVoiceID);

    VoiceList<std::string>::type citySearchIDList;
    EXPECT_FALSE(m_naviAddress->queryCitySearchID(cityNodes, zoneGlobalVoiceID, citySearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, queryCitySearchID_4)
{
    std::string reqMsg =
            "<data id=\"_buildAddress\">"
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
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">1</zoneId>"
                                    "<cityId confidence=\"5000\">0</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">2</zoneId>"
                                    "<cityId confidence=\"5000\">0</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\"/>3</zoneId>"
                                    "<cityId confidence=\"5000\">0</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    pugi::xml_node asrResuldNode = msgDoc.select_node((std::string("//") + "asrResult").c_str()).node();
    std::ostringstream oss;
    asrResuldNode.print(oss);
    pugi::xml_document asrResuldNodeDoc;
    asrResuldNodeDoc.load_string(oss.str().c_str());
    pugi::xpath_node_set cityNodes = asrResuldNodeDoc.select_nodes((std::string("//") + "cityId").c_str());

    pugi::xml_node addressInfoNode = msgDoc.select_node((std::string("//") + "addressInfo").c_str()).node();
    std::string zoneSearchID(addressInfoNode.child("zoneId").text().as_string());
    std::string zoneNameIndex(addressInfoNode.child("zoneNameIndex").text().as_string());

    m_naviAddress->processReceivedSearchID(zoneSearchID, zoneNameIndex);
    std::string zoneGlobalVoiceID;
    m_naviAddress->getGlobalVoiceID(zoneSearchID, zoneGlobalVoiceID);

    VoiceList<std::string>::type citySearchIDList;
    EXPECT_TRUE(m_naviAddress->queryCitySearchID(cityNodes, zoneGlobalVoiceID, citySearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, queryStreetSearchID_1)
{
    pugi::xpath_node_set streetNodes;
    std::string cityGlobalVoiceID;
    VoiceList<std::string>::type streetSearchIDList;

    VR_Configure_mock configObj;
    // DB is not OK
    ON_CALL(configObj, getMapDataPath()).WillByDefault(Return("/data/PP"));
    ON_CALL(configObj, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(configObj, getTTSLanguage(_)).WillByDefault(Return("ENG"));

    VR_NaviAddressCommon naviAddressObj(&configObj);
    EXPECT_FALSE(naviAddressObj.queryStreetSearchID(streetNodes, cityGlobalVoiceID, streetSearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, queryStreetSearchID_2)
{
    pugi::xpath_node_set streetNodes;
    std::string cityGlobalVoiceID;
    VoiceList<std::string>::type streetSearchIDList;

    m_naviAddress->setCountryID("");
    EXPECT_FALSE(m_naviAddress->queryStreetSearchID(streetNodes, cityGlobalVoiceID, streetSearchIDList));
    m_naviAddress->setCountryID("1");
}

TEST_F(VR_NaviAddressCommon_Test, queryStreetSearchID_3)
{
    std::string reqMsg =
            "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>1003</cityId>"
                        "<cityNameIndex>0</cityNameIndex>"
                        "<city>abc</city>"
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
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">1</zoneId>"
                                    "<cityId confidence=\"5000\"1003</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">2</zoneId>"
                                    "<cityId confidence=\"5000\">1003</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\"/>3</zoneId>"
                                    "<cityId confidence=\"5000\">1003</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    pugi::xml_node asrResuldNode = msgDoc.select_node((std::string("//") + "asrResult").c_str()).node();
    std::ostringstream oss;
    asrResuldNode.print(oss);
    pugi::xml_document asrResuldNodeDoc;
    asrResuldNodeDoc.load_string(oss.str().c_str());
    pugi::xpath_node_set streetNodes = asrResuldNodeDoc.select_nodes((std::string("//") + "streetId").c_str());

    pugi::xml_node addressInfoNode = msgDoc.select_node((std::string("//") + "addressInfo").c_str()).node();
    std::string citySearchID(addressInfoNode.child("cityId").text().as_string());
    std::string cityNameIndex(addressInfoNode.child("cityNameIndex").text().as_string());

    m_naviAddress->processReceivedSearchID(citySearchID, cityNameIndex);
    std::string cityGlobalVoiceID;
    m_naviAddress->getGlobalVoiceID(citySearchID, cityGlobalVoiceID);

    VoiceList<std::string>::type streetSearchIDList;
    EXPECT_FALSE(m_naviAddress->queryStreetSearchID(streetNodes, cityGlobalVoiceID, streetSearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, queryStreetSearchID_4)
{
    std::string reqMsg =
            "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STREET</buildType>"
                    "<addressInfo>"
                        "<zoneId>18155135997837312</zoneId>"
                        "<zoneNameIndex>0</zoneNameIndex>"
                        "<country>''</country>"
                        "<zone>Western Cape</zone>"
                        "<cityId>1003</cityId>"
                        "<cityNameIndex>0</cityNameIndex>"
                        "<city>abc</city>"
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
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">1</zoneId>"
                                    "<cityId confidence=\"5000\"1003</cityId>"
                                    "<streetId confidence=\"5000\">1010101</streetId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">2</zoneId>"
                                    "<cityId confidence=\"5000\">1003</cityId>"
                                    "<streetId confidence=\"5000\">1010101</streetId>"
                                "</item>"
                                "<item>"
                                    "<zoneId confidence=\"5000\"/>3</zoneId>"
                                    "<cityId confidence=\"5000\">1003</cityId>"
                                    "<streetId confidence=\"5000\">1010101</streetId>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    pugi::xml_node asrResuldNode = msgDoc.select_node((std::string("//") + "asrResult").c_str()).node();
    std::ostringstream oss;
    asrResuldNode.print(oss);
    pugi::xml_document asrResuldNodeDoc;
    asrResuldNodeDoc.load_string(oss.str().c_str());
    pugi::xpath_node_set streetNodes = asrResuldNodeDoc.select_nodes((std::string("//") + "streetId").c_str());

    pugi::xml_node addressInfoNode = msgDoc.select_node((std::string("//") + "addressInfo").c_str()).node();
    std::string citySearchID(addressInfoNode.child("cityId").text().as_string());
    std::string cityNameIndex(addressInfoNode.child("cityNameIndex").text().as_string());

    m_naviAddress->processReceivedSearchID(citySearchID, cityNameIndex);
    std::string cityGlobalVoiceID;
    m_naviAddress->getGlobalVoiceID(citySearchID, cityGlobalVoiceID);

    VoiceList<std::string>::type streetSearchIDList;
    EXPECT_FALSE(m_naviAddress->queryStreetSearchID(streetNodes, cityGlobalVoiceID, streetSearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, queryZoneSearchID_1)
{
    pugi::xpath_node_set zoneNodes;
    VoiceList<std::string>::type zoneSearchIDList;

    VR_Configure_mock configObj;
    // DB is not OK
    ON_CALL(configObj, getMapDataPath()).WillByDefault(Return("/data/PP"));
    ON_CALL(configObj, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(configObj, getTTSLanguage(_)).WillByDefault(Return("ENG"));

    VR_NaviAddressCommon naviAddressObj(&configObj);
    EXPECT_FALSE(naviAddressObj.queryZoneSearchID(zoneNodes, zoneSearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, queryZoneSearchID_2)
{
    pugi::xpath_node_set zoneNodes;
    VoiceList<std::string>::type zoneSearchIDList;

    m_naviAddress->setCountryID("");
    EXPECT_FALSE(m_naviAddress->queryZoneSearchID(zoneNodes, zoneSearchIDList));
    m_naviAddress->setCountryID("1");
}

TEST_F(VR_NaviAddressCommon_Test, queryZoneSearchID_3)
{
    std::string reqMsg =
            "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STATE</buildType>"
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
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"0\"/>"
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                 "<item>"
                                    "<zoneId confidence=\"0\"/>"
                                    "<cityId confidence=\"5000\">1003</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                 "<item>"
                                    "<zoneId confidence=\"0\"/>"
                                    "<cityId confidence=\"5000\">1004</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    pugi::xml_node asrResuldNode = msgDoc.select_node((std::string("//") + "asrResult").c_str()).node();
    std::ostringstream oss;
    asrResuldNode.print(oss);
    pugi::xml_document asrResuldNodeDoc;
    asrResuldNodeDoc.load_string(oss.str().c_str());
    pugi::xpath_node_set zoneNodes = asrResuldNodeDoc.select_nodes((std::string("//") + "zoneId").c_str());
    VoiceList<std::string>::type zoneSearchIDList;
    EXPECT_FALSE(m_naviAddress->queryZoneSearchID(zoneNodes, zoneSearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, queryZoneSearchID_4)
{
    std::string reqMsg =
            "<data id=\"_buildAddress\">"
                "<data>"
                    "<buildType>STATE</buildType>"
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
                                "<count>3</count>"
                            "</header>"
                            "<items>"
                                "<item>"
                                    "<zoneId confidence=\"5000\">100000</zoneId>"
                                    "<cityId confidence=\"4325\">1002</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                 "<item>"
                                    "<zoneId confidence=\"5000\">100000<zoneId/>"
                                    "<cityId confidence=\"5000\">1003</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                                 "<item>"
                                    "<zoneId confidence=\"5000\">100000</zoneId>"
                                    "<cityId confidence=\"5000\">1004</cityId>"
                                    "<streetId confidence=\"0\"/>"
                                "</item>"
                            "</items>"
                        "</list>"
                    "</asrResult>"
                "</data>"
            "</data>";
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    pugi::xml_node asrResuldNode = msgDoc.select_node((std::string("//") + "asrResult").c_str()).node();
    std::ostringstream oss;
    asrResuldNode.print(oss);
    pugi::xml_document asrResuldNodeDoc;
    asrResuldNodeDoc.load_string(oss.str().c_str());
    pugi::xpath_node_set zoneNodes = asrResuldNodeDoc.select_nodes((std::string("//") + "zoneId").c_str());
    VoiceList<std::string>::type zoneSearchIDList;
    EXPECT_TRUE(m_naviAddress->queryZoneSearchID(zoneNodes, zoneSearchIDList));
}

TEST_F(VR_NaviAddressCommon_Test, getPOIPrompt)
{
    std::string promptText;
    EXPECT_FALSE(m_naviAddress->getPOIPrompt("123", promptText));
    promptText.clear();

    // create new table and insert data
    sqlite3 * dbHandler = nullptr;
    sqlite3_open_v2("/data/PP_Data/voice_search_id.db", &dbHandler, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, NULL);
    std::string sqlRequest;
    char * errmsg = nullptr;
    sqlRequest.assign("CREATE TABLE pcp_tbl_ENG(search_id integer,prompt_name varchar(255))");
    sqlite3_exec(dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);
    sqlRequest.assign("INSERT INTO pcp_tbl_ENG VALUES(\"123\", \"abc\")");
    sqlite3_exec(dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);
    sqlRequest.assign("INSERT INTO pcp_tbl_ENG VALUES(\"999\", \"\")");
    sqlite3_exec(dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);

    // list empty
    EXPECT_FALSE(m_naviAddress->getPOIPrompt("456", promptText));
    promptText.clear();

    // list front empty
    EXPECT_FALSE(m_naviAddress->getPOIPrompt("999", promptText));
    promptText.clear();

    // both not empty
    EXPECT_TRUE(m_naviAddress->getPOIPrompt("123", promptText));
    EXPECT_STREQ("abc", promptText.c_str());
    promptText.clear();

    // drop table
    sqlRequest.assign("DROP TABLE pcp_tbl_ENG");
    sqlite3_exec(dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);
    sqlite3_close(dbHandler);
    dbHandler = nullptr;

    m_naviAddress->m_dbResult = SQLITE_ERROR;
    EXPECT_FALSE(m_naviAddress->getPOIPrompt("123", promptText));
    promptText.clear();

    m_naviAddress->m_dbResult = SQLITE_OK;
}

/* EOF */
