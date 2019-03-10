/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

#define private public
#define protected public

// #include <boost/filesystem.hpp>
#include "VR_DECommon.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "VR_Configure_mock.h"
#include <cstdlib>

using ::testing::Return;
using ::testing::_;
using namespace pugi;


/**
* VR_DECommon_test.cpp
*
* The class for VR_DECommon_test.
*/
class VR_DECommon_test : public testing::Test
{
public:
    VR_DECommon_test()
    {
        m_decommon = NULL;
    }

    virtual ~VR_DECommon_test()
    {
    }

    virtual void SetUp()
    {
        m_decommon = new VR_DECommon();
        pConfigure_mock = new VR_Configure_mock();
    }

    virtual void TearDown()
    {
        delete m_decommon;
        delete pConfigure_mock;
    }

private:
    VR_DECommon_test(const VR_DECommon_test&);
    VR_DECommon_test& operator= (const VR_DECommon_test&);

    VR_DECommon* m_decommon;
    VR_Configure_mock* pConfigure_mock;
};

// isGetFromData BEGIN
TEST_F(VR_DECommon_test, isGetFromData_0)
{
    EXPECT_TRUE(m_decommon->isGetFromData());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isGetFromData_1_01)
{
    system("mkdir -p /system/etc/voicerecog/settings/");
    std::ofstream mcreate_file("/system/etc/voicerecog/settings/.version");
    m_decommon->m_dataPath = "/system/etc/voicerecog/";
    std::cout << m_decommon->m_dataPath << std::endl;
    EXPECT_TRUE(m_decommon->isGetFromData());
    SUCCEED();
    system("rm /system/etc/voicerecog/settings/.version");
}

TEST_F(VR_DECommon_test, isGetFromData_1_02_001)
{
    system("mkdir -p /system/etc/voicerecog/settings/");
    std::ofstream mcreate_file("/system/etc/voicerecog/settings/.version");
    mcreate_file << "<list id='hints_topmenu' type='hint'><items><item></item><item></item><item></item></items></list>" << std::endl;
    m_decommon->m_dataPath = "/data/";
    system("mkdir -p /data/settings/");
    std::ofstream datacreate_file("/data/settings/.version");
    EXPECT_FALSE(m_decommon->isGetFromData());
    SUCCEED();
    system("rm /data/settings/.version");
    system("rm /system/etc/voicerecog/settings/.version");
}

TEST_F(VR_DECommon_test, isGetFromData_1_02_002)
{
    system("mkdir -p /system/etc/voicerecog/settings/");
    std::ofstream mcreate_file("/system/etc/voicerecog/settings/.version");
    m_decommon->m_dataPath = "/data/";
    system("mkdir -p /data/settings/");
    std::ofstream datacreate_file("/data/settings/.version");  
    EXPECT_TRUE(m_decommon->isGetFromData());
    SUCCEED();
    system("rm /data/settings/.version");
    system("rm /system/etc/voicerecog/settings/.version");
}
// isGetFromData END

// getProductCountry BEGIN
TEST_F(VR_DECommon_test, getProductCountry)
{
    // m_decommon->getProductCountry();
    SUCCEED();
}
// getProductCountry END

// getCountryType BEGIN
TEST_F(VR_DECommon_test, getCountryType_00)
{
    m_decommon->m_country = "Japan";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    SUCCEED();
}

TEST_F(VR_DECommon_test, getCountryType_01)
{
    m_decommon->m_country = "European";      // DE_Country::eu
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    SUCCEED();
}

TEST_F(VR_DECommon_test, getCountryType_02)
{
    m_decommon->m_country = "China";   // DE_Country :: cn 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    SUCCEED();
}

TEST_F(VR_DECommon_test, getCountryType_03)
{
    m_decommon->m_country = "America";   // DE_Country::vt
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    SUCCEED();
}

TEST_F(VR_DECommon_test, getCountryType_04)
{
    m_decommon->m_country = "";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    SUCCEED();
}
// getCountryType END

// getAsrSupportLanguage BEGIN
TEST_F(VR_DECommon_test, getAsrSupportLanguage_0)
{
    std::string lang = "fr-ca";
    std::string outLanguage = "";

    EXPECT_FALSE(m_decommon->getAsrSupportLanguage(lang, outLanguage));
    SUCCEED();
}

TEST_F(VR_DECommon_test, getAsrSupportLanguage_1)
{
    std::string lang = "fr-ca";
    std::string outLanguage = "";

    m_decommon->m_dataPath = "/data/others/";
    system("mkdir -p /data/others");
    system("mkdir -p /data/ASR/Model/");
    std::ofstream mcreate_file("/data/ASR/Model/Language_frc");
    EXPECT_TRUE(m_decommon->getAsrSupportLanguage(lang, outLanguage));
    SUCCEED();
    system("rm /data/ASR/Model/Language_frc");
}

TEST_F(VR_DECommon_test, getAsrSupportLanguage_2)
{
    std::string lang = "fr-ca";
    std::string outLanguage = "";

    m_decommon->m_dataPath = "/data/others/";
    system("mkdir -p /data/others");
    system("mkdir -p /data/ASR/Model/");
    std::ofstream mcreate_file("/data/ASR/Model/Language_eng");
    EXPECT_TRUE(m_decommon->getAsrSupportLanguage(lang, outLanguage));
    SUCCEED();
    system("rm /data/ASR/Model/Language_eng");
}

TEST_F(VR_DECommon_test, getAsrSupportLanguage_3)
{
    std::string lang = "fr-ca";
    std::string outLanguage = "";

    m_decommon->m_dataPath = "/data/others/";
    system("mkdir -p /data/others");
    system("mkdir -p /data/ASR/Model/");
    std::ofstream mcreate_file("/data/ASR/Model/Language_enu");
    EXPECT_TRUE(m_decommon->getAsrSupportLanguage(lang, outLanguage));
    SUCCEED();
    system("rm /data/ASR/Model/Language_enu");
}

TEST_F(VR_DECommon_test, getAsrSupportLanguage_4)
{
    std::string lang = "";
    std::string outLanguage = "";

    EXPECT_FALSE(m_decommon->getAsrSupportLanguage(lang, outLanguage));
    SUCCEED();
}

TEST_F(VR_DECommon_test, getAsrSupportLanguage_5)
{
    std::string lang = "";
    std::string outLanguage = "";

    m_decommon->m_dataPath = "/data/others/";
    system("mkdir -p /data/others");
    system("mkdir -p /data/ASR/Model/");
    std::ofstream mcreate_file("/data/ASR/Model/Language_frc");
    EXPECT_TRUE(m_decommon->getAsrSupportLanguage(lang, outLanguage));
    SUCCEED();
    system("rm /data/ASR/Model/Language_frc");
}

TEST_F(VR_DECommon_test, getAsrSupportLanguage_6)
{
    std::string lang = "";
    std::string outLanguage = "";

    m_decommon->m_dataPath = "/data/others/";
    system("mkdir -p /data/others");
    system("mkdir -p /data/ASR/Model/");
    std::ofstream mcreate_file("/data/ASR/Model/Language_eng");
    EXPECT_TRUE(m_decommon->getAsrSupportLanguage(lang, outLanguage));
    SUCCEED();
    system("rm /data/ASR/Model/Language_eng");
}

TEST_F(VR_DECommon_test, getAsrSupportLanguage_7)
{
    std::string lang = "";
    std::string outLanguage = "";

    m_decommon->m_dataPath = "/data/others/";
    system("mkdir -p /data/others");
    system("mkdir -p /data/ASR/Model/");
    std::ofstream mcreate_file("/data/ASR/Model/Language_enu");
    EXPECT_TRUE(m_decommon->getAsrSupportLanguage(lang, outLanguage));
    SUCCEED();
    system("rm /data/ASR/Model/Language_enu");
}
// getAsrSupportLanguage  END

// readExistDirLanguageName BEGIN
TEST_F(VR_DECommon_test, readExistDirLanguageName)
{
    VoiceList<std::string>::type  languageNameList;
    m_decommon->m_dataPath = "/data/others/";
    system("mkdir -p /data/others");
    system("mkdir -p /data/ASR/Model/");
    m_decommon->readExistDirLanguageName(languageNameList);
    SUCCEED();
}
// readExistDirLanguageName  END

// getVROverBeep BEGIN
TEST_F(VR_DECommon_test, getVROverBeep_0)
{
    EXPECT_FALSE(m_decommon->getVROverBeep());
    SUCCEED();
}
// getVROverBeep END

// init   BEGIN
TEST_F(VR_DECommon_test, init_0)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRRegion()).WillRepeatedly(Return(""));
    m_decommon->init(pConfigure_mock);
    SUCCEED();
}

TEST_F(VR_DECommon_test, init_1)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRRegion()).WillRepeatedly(Return("South-Africa"));
    m_decommon->init(pConfigure_mock);
    SUCCEED();
}

TEST_F(VR_DECommon_test, init_2)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRRegion()).WillRepeatedly(Return("Thailand"));
    m_decommon->init(pConfigure_mock);
    SUCCEED();
}

TEST_F(VR_DECommon_test, init_3)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRRegion()).WillRepeatedly(Return("Southeast-Asia"));
    m_decommon->init(pConfigure_mock);
    SUCCEED();
}

TEST_F(VR_DECommon_test, init_4)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRRegion()).WillRepeatedly(Return("HongkongMacau"));
    m_decommon->init(pConfigure_mock);
    SUCCEED();
}
// init   END

// getVRLanguage BEGN
TEST_F(VR_DECommon_test, getVRLanguage_00)
{
    m_decommon->m_language = "";
    m_decommon->getVRLanguage();
    SUCCEED();
}

TEST_F(VR_DECommon_test, getVRLanguage_01)
{
    m_decommon->m_language = "enu";
    m_decommon->getVRLanguage();
    SUCCEED();
}
// getVRLanguage END

// notifyVRLanguageChange BEGIN
TEST_F(VR_DECommon_test, notifyVRLanguageChange_00)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L1_5));
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}

TEST_F(VR_DECommon_test, notifyVRLanguageChange_01)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return("zh-cn"));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}

TEST_F(VR_DECommon_test, notifyVRLanguageChange_01__001)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return(""));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}


TEST_F(VR_DECommon_test, notifyVRLanguageChange_01_1)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    m_decommon->m_country = "Japan";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}

TEST_F(VR_DECommon_test, notifyVRLanguageChange_02)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return("zh-cn"));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}

TEST_F(VR_DECommon_test, notifyVRLanguageChange_02__001)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return(""));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}

TEST_F(VR_DECommon_test, notifyVRLanguageChange_02_1)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
    m_decommon->m_country = "Japan";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}

TEST_F(VR_DECommon_test, notifyVRLanguageChange_03)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L1));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return("zh-cn"));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}

TEST_F(VR_DECommon_test, notifyVRLanguageChange_03__001)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L1));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return(""));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}

TEST_F(VR_DECommon_test, notifyVRLanguageChange_03_1)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L1));
    m_decommon->m_country = "Japan";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    m_decommon->notifyVRLanguageChange();
    SUCCEED();
}
// notifyVRLanguageChange END

// isSupportVR BEGIN
TEST_F(VR_DECommon_test, isSupportVR_00)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L1_5));
    EXPECT_TRUE(m_decommon->isSupportVR());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isSupportVR_01)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return("zh-cn"));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_TRUE(m_decommon->isSupportVR());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isSupportVR_01__001)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return(""));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_FALSE(m_decommon->isSupportVR());
    SUCCEED();
}


TEST_F(VR_DECommon_test, isSupportVR_01_1)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    m_decommon->m_country = "Japan";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_TRUE(m_decommon->isSupportVR());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isSupportVR_02)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return("zh-cn"));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_TRUE(m_decommon->isSupportVR());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isSupportVR_02__001)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return(""));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_FALSE(m_decommon->isSupportVR());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isSupportVR_02_1)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
    m_decommon->m_country = "Japan";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_TRUE(m_decommon->isSupportVR());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isSupportVR_03)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L1));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return("zh-cn"));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_TRUE(m_decommon->isSupportVR());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isSupportVR_03__001)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L1));
    EXPECT_CALL(*pConfigure_mock, getVRLanguage()).WillRepeatedly(Return(""));
    m_decommon->m_country = "China";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_FALSE(m_decommon->isSupportVR());
    SUCCEED();
}

TEST_F(VR_DECommon_test, isSupportVR_03_1)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L1));
    m_decommon->m_country = "Japan";    // DE_Country::gen 
    m_decommon->getProductCountry();
    m_decommon->getCountryType();
    EXPECT_TRUE(m_decommon->isSupportVR());
    SUCCEED();
}
// isSupportVR END

// setNaviModel  BEGIN
TEST_F(VR_DECommon_test, setNaviModel_00)
{
    bool is = false;
    m_decommon->setNaviModel(is);
    SUCCEED();
}

TEST_F(VR_DECommon_test, setNaviModel_01)
{
    bool is = true;
    m_decommon->setNaviModel(is);
    SUCCEED();
}
// setNaviModel END

// getNaviModel BEGIN
TEST_F(VR_DECommon_test, getNaviModel_01)
{
    m_decommon->m_naviModel = true;
    EXPECT_TRUE(m_decommon->getNaviModel());
    SUCCEED();
}

TEST_F(VR_DECommon_test, getNaviModel_02)
{
    m_decommon->m_naviModel = false;
    EXPECT_FALSE(m_decommon->getNaviModel());
    SUCCEED();
}
// getNaviModel END

// getHybridVRFlag BEGIN
TEST_F(VR_DECommon_test, getHybridVRFlag_01)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getHybridVRFlag()).WillRepeatedly(Return(true));
    EXPECT_TRUE(m_decommon->getHybridVRFlag());
    SUCCEED();
}

TEST_F(VR_DECommon_test, getHybridVRFlag_02)
{
    m_decommon->m_pConfigure = pConfigure_mock;
    EXPECT_CALL(*pConfigure_mock, getHybridVRFlag()).WillRepeatedly(Return(false));
    EXPECT_FALSE(m_decommon->getHybridVRFlag());
    SUCCEED();
}
// getHybridVRFlag END

// getHeadUnitType BEGIN
// TEST_F(VR_DECommon_test, getHeadUnitType)
// {
//     m_decommon->m_pConfigure = pConfigure_mock;
//     const std::string  teststr = "test";
//     EXPECT_CALL(*pConfigure_mock, getHeadUnitType()).WillRepeatedly(Return(teststr));
//     m_decommon->getHeadUnitType();
//     SUCCEED();
// }
// getHeadUnitType END

// getTargetNice BEGIN
TEST_F(VR_DECommon_test, getTargetNice_00)
{
    std::string thName =  "findkey";
    m_decommon->m_mapNices[thName] = 10;
    m_decommon->getTargetNice(thName);
    SUCCEED();
}

TEST_F(VR_DECommon_test, getTargetNice_01)
{
    std::string thName = "findkey";
    m_decommon->getTargetNice(thName);
    SUCCEED();
}
// getTargetNice END
/* EOF */
