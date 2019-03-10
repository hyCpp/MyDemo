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


#include "gtest/gtest.h"
#include "VR_VoiceIDConverter.h"

/**
* VR_VoiceIDConverter_Test
*
* The class is just for VR_VoiceIDConverter test.
*/
class VR_VoiceIDConverter_Test : public testing::Test
{
public:
    VR_VoiceIDConverter_Test()
    {
    }

    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}

protected:
    VR_VoiceIDConverter m_converter;

private:
    VR_VoiceIDConverter_Test(const VR_VoiceIDConverter_Test& other);
    void operator= (const VR_VoiceIDConverter_Test& other);
};

TEST_F(VR_VoiceIDConverter_Test, getGlobalVoiceID)
{
    VR_GlobalVoiceIDResult result = m_converter.getGlobalVoiceID("", "", "", "");
    EXPECT_EQ(VR_MappingType::EMPTY, result.type);
    EXPECT_TRUE(result.zoneid.empty());
    EXPECT_TRUE(result.cityid.empty());
    EXPECT_TRUE(result.streetid.empty());
    EXPECT_TRUE(result.houseNumber.empty());

    result = m_converter.getGlobalVoiceID("105", "", "", "");
    EXPECT_EQ(VR_MappingType::TILL_STATE, result.type);
    EXPECT_STREQ("29554872554618880", result.zoneid.c_str());
    EXPECT_TRUE(result.cityid.empty());
    EXPECT_TRUE(result.streetid.empty());
    EXPECT_TRUE(result.houseNumber.empty());

    result = m_converter.getGlobalVoiceID("105", "46198", "", "");
    EXPECT_EQ(VR_MappingType::TILL_CITY, result.type);
    EXPECT_STREQ("29554872554618880", result.zoneid.c_str());
    EXPECT_STREQ("29753291453759488", result.cityid.c_str());
    EXPECT_TRUE(result.streetid.empty());
    EXPECT_TRUE(result.houseNumber.empty());

    result = m_converter.getGlobalVoiceID("105", "46198", "3751907395", "");
    EXPECT_EQ(VR_MappingType::TILL_STREET, result.type);
    EXPECT_STREQ("29554872554618880", result.zoneid.c_str());
    EXPECT_STREQ("29753291453759488", result.cityid.c_str());
    EXPECT_STREQ("29753295205666883", result.streetid.c_str());
    EXPECT_TRUE(result.houseNumber.empty());

    result = m_converter.getGlobalVoiceID("105", "46198", "3751907395", "12345");
    EXPECT_EQ(VR_MappingType::FULL, result.type);
    EXPECT_STREQ("29554872554618880", result.zoneid.c_str());
    EXPECT_STREQ("29753291453759488", result.cityid.c_str());
    EXPECT_STREQ("29753295205666883", result.streetid.c_str());
    EXPECT_STREQ("12345", result.houseNumber.c_str());

    result = m_converter.getGlobalVoiceID("", "46198", "3751907395", "12345");
    EXPECT_EQ(VR_MappingType::EMPTY, result.type);
    EXPECT_TRUE(result.zoneid.empty());
    EXPECT_TRUE(result.cityid.empty());
    EXPECT_TRUE(result.streetid.empty());
    EXPECT_TRUE(result.houseNumber.empty());

    result = m_converter.getGlobalVoiceID("105", "", "3751907395", "12345");
    EXPECT_EQ(VR_MappingType::TILL_STATE, result.type);
    EXPECT_STREQ("29554872554618880", result.zoneid.c_str());
    EXPECT_TRUE(result.cityid.empty());
    EXPECT_TRUE(result.streetid.empty());
    EXPECT_TRUE(result.houseNumber.empty());

    result = m_converter.getGlobalVoiceID("105", "46198", "", "12345");
    EXPECT_EQ(VR_MappingType::TILL_CITY, result.type);
    EXPECT_STREQ("29554872554618880", result.zoneid.c_str());
    EXPECT_STREQ("29753291453759488", result.cityid.c_str());
    EXPECT_TRUE(result.streetid.empty());
    EXPECT_TRUE(result.houseNumber.empty());
}

TEST_F(VR_VoiceIDConverter_Test, getGlobalZoneID)
{
    // zoneid(0~255)
    std::string globalZoneID(m_converter.getGlobalZoneID("105"));
    EXPECT_STREQ("29554872554618880", globalZoneID.c_str());
}

TEST_F(VR_VoiceIDConverter_Test, getGlobalCityID)
{
    // cityid(0~65535)
    std::string globalCityID(m_converter.getGlobalCityID("29554872554618880", "46198"));
    EXPECT_STREQ("29753291453759488", globalCityID.c_str());
}

TEST_F(VR_VoiceIDConverter_Test, getGlobalStreetID)
{
    // streetid(0~4294967295)
    std::string globalStreetID(m_converter.getGlobalStreetID("29753291453759488", "3751907395"));
    EXPECT_STREQ("29753295205666883", globalStreetID.c_str());
}

TEST_F(VR_VoiceIDConverter_Test, getZoneID)
{
    EXPECT_STREQ("105", m_converter.getZoneID("29753295205666883").c_str());
}

TEST_F(VR_VoiceIDConverter_Test, getCityID)
{
    EXPECT_STREQ("46198", m_converter.getCityID("29753295205666883").c_str());
}

TEST_F(VR_VoiceIDConverter_Test, getStreetID)
{
    EXPECT_STREQ("3751907395", m_converter.getStreetID("29753295205666883").c_str());
}

/* EOF */