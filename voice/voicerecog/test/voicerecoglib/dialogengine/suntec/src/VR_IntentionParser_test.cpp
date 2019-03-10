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

#include "VR_IntentionParser.h"
#include "VR_DECommon_mock.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>

#include "VR_DataAccessorManager.h"
#include "Vr_Asr_Engine.h"
#include "VR_AsrRequestor_mock.h"
#include "VR_Configure_mock.h";

using ::testing::Return;
using ::testing::_;
using namespace pugi;

/**
* VR_IntentionParser_test.cpp
*
* The class for VR_IntentionParser_test.
*/
class VR_IntentionParser_test : public testing::Test
{
public:
    VR_IntentionParser_test()
    {
        m_intentionparser = NULL;
        m_pDECommon_mock = NULL;
    }

    virtual ~VR_IntentionParser_test()
    {
    }

    virtual void SetUp()
    {
        m_pDECommon_mock = new VR_DECommon_mock();
        m_intentionparser = new VR_IntentionParser(m_pDECommon_mock);
        m_configureIF = new VR_Configure_mock();
        m_asrRequestorMock = new VR_AsrRequestor_mock();
        ON_CALL(*m_pDECommon_mock, getCountryType()).WillByDefault(Return(VR_DECommonIF::cn));
        EXPECT_CALL(*m_pDECommon_mock, getCountryType()).Times(::testing::AnyNumber());
        EXPECT_CALL(*m_configureIF, getVRContry()).WillRepeatedly(Return(VR_REGION_SA));
        EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("en-gb"));
        EXPECT_CALL(*m_configureIF, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
        EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("/tmp/"));
        EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/data/PP_Data"));
        system("mkdir /tmp/VR_DataAccessorDETest");
        EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("/tmp/VR_DataAccessorDETest"));
    }

    virtual void TearDown()
    {
        if (m_intentionparser != NULL) {
            delete m_intentionparser;
            m_intentionparser = NULL;
        }
        if (m_pDECommon_mock != NULL) {
            delete m_pDECommon_mock;
            m_pDECommon_mock = NULL;
        }
        if (m_configureIF != NULL) {
            delete m_configureIF;
            m_configureIF = NULL;
        }
        if (m_asrRequestorMock != NULL) {
            delete m_asrRequestorMock;
            m_asrRequestorMock = NULL;
        }
    }

    pugi::xml_node getXmlNode(const pugi::char_t * contents)
    {
        m_resultDoc.load_string(contents);
        return m_resultDoc.root().first_child();
    }

    std::string getNodeString(const pugi::xml_node& node)
    {
        m_strstrm.str("");
        node.print(m_strstrm);
        return m_strstrm.str();
    }

protected:
    pugi::xml_document  m_resultDoc;
    std::stringstream   m_strstrm;
    VR_IntentionParser* m_intentionparser;
    VR_DECommon_mock*   m_pDECommon_mock;
    VR_Configure_mock* m_configureIF;
    VR_AsrRequestor_mock *m_asrRequestorMock;

private:
    VR_IntentionParser_test(const VR_IntentionParser_test&);
    VR_IntentionParser_test& operator= (const VR_IntentionParser_test&);
};

// preProcAsrResult BEGIN
TEST_F(VR_IntentionParser_test, preProcAsrResult)
{
    // Match CMN2005
    std::string xml1 = "<item><node value=\"CMN20052\">def</node></item>";
    pugi::xml_document doc1;
    doc1.load_string(xml1.c_str());
    m_intentionparser->preProcAsrResult(doc1.root());

    // Not Match CMN2005
    std::string xml2 = "<item><node value=\"CMN2025\">def</node></item>";
    pugi::xml_document doc2;
    doc2.load_string(xml2.c_str());
    m_intentionparser->preProcAsrResult(doc2.root());
}
// preProcAsrResult  END

// changePOINameByPOIId BEGIN
TEST_F(VR_IntentionParser_test, changePOINameByPOIId)
{
    std::string xml1 = "<ro>\
        <node>\
            <POI_category_id>11</POI_category_id>\
        </node>\
        <node>\
            <POI_category_id>22</POI_category_id>\
        </node>\
    </ro>";
    pugi::xml_document doc1;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    VR_DataAccessorManager* ptr_dataAccManager = new VR_DataAccessorManager(m_asrRequestorMock, m_pDECommon_mock, m_configureIF);
    // PoiIdNodes  Not empty
    m_intentionparser->changePOINameByPOIId(asrNode1.first_child(), ptr_dataAccManager);
}
// changePOINameByPOIId  END

// compareItem BEGIN
TEST_F(VR_IntentionParser_test, compareItem)
{
    std::string xml1 = "<item>abc</item>";
    std::string xml2 = "<item>def</item>";
    pugi::xml_document doc1, doc2;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();

    // at least One Node is empty
    EXPECT_TRUE(m_intentionparser->compareItem(getXmlNode("<node name=\"intent\" />"), getXmlNode("<node name=\"intent\" />")));
    // both nodes Not Null, value equival
    EXPECT_TRUE(m_intentionparser->compareItem(asrNode1, asrNode1));
    // both nodes Not Null, value Not equival
    EXPECT_FALSE(m_intentionparser->compareItem(asrNode1, asrNode2));
}
// compareItem  END

// removeCurItemFollowSameItem BEGIN
TEST_F(VR_IntentionParser_test, removeCurItemFollowSameItem)
{
    std::string xml1 = "<item>abc</item>";
    std::string xml2 = "<item1>abc</item1><item2>abc</item2>";
    std::string xml3 = "<item1>abc</item1><item2>def</item2>";
    pugi::xml_document doc1, doc2, doc3;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    doc3.load_string(xml3.c_str());
    pugi::xml_node asrNode3 = doc3.root();
    // empty
    m_intentionparser->removeCurItemFollowSameItem(getXmlNode("<node name=\"intent\" />"));
    // not nextItem
    m_intentionparser->removeCurItemFollowSameItem(asrNode1.first_child());
    // same
    // m_intentionparser->removeCurItemFollowSameItem(getXmlNode("<item1>abc</item1><item2>abc</item2>"));
    m_intentionparser->removeCurItemFollowSameItem(asrNode2.first_child());
    // not same
    m_intentionparser->removeCurItemFollowSameItem(asrNode3.first_child());
}
// removeCurItemFollowSameItem  END

// removeSameItemInNBest BEGIN
TEST_F(VR_IntentionParser_test, removeSameItemInNBest)
{
    std::string xml1 = "<items>abc</items><items>def</items>";
    std::string xml2 = "<notitems>abc</notitems>";
    pugi::xml_document doc1, doc2;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    // not empty, not same
    m_intentionparser->removeSameItemInNBest(asrNode1.first_child());
    // empty
    m_intentionparser->removeSameItemInNBest(asrNode2.first_child());
}
// removeSameItemInNBest  END

// filterIntention BEGIN
TEST_F(VR_IntentionParser_test, filterIntention_Null)
{
    pugi::xml_node asrNode = getXmlNode("<node name=\"intent\" />");
    EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, true, true));
}
// filterIntention END

// filterNaviCMN BEGIN
#define CMNXmlStr(cmn) "<node name=\"intent\" ><node value=\"" cmn "\" /></node>"
TEST_F(VR_IntentionParser_test, filterNaviCMN)
{
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN2002")), true, false));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN0001")), true, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN4001")), true, false));
}
// filterNaviCMN END

// filterMusicCMN BEGIN
TEST_F(VR_IntentionParser_test, filterMusicCMN)
{
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3002")), false, true));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3006")), false, true));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3007")), false, true));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3010")), false, true));

    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3001")), false, true));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3003")), false, true));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3004")), false, true));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3005")), false, true));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3008")), false, true));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3009")), false, true));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3011")), false, true));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3012")), false, true));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(CMNXmlStr("CMN3023")), false, true));
}
// filterMusicCMN END
#undef CMNXmlStr

TEST_F(VR_IntentionParser_test, filterRadioFreqGen_LANG)
{
    pugi::xml_node asrNode = getXmlNode("<node name=\"intent\" />");

    EXPECT_CALL(*m_pDECommon_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::eu));
    EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, false, false));

    EXPECT_CALL(*m_pDECommon_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::cn));
    EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, false, false));

    EXPECT_CALL(*m_pDECommon_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::gen));
    EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, false, false));

    EXPECT_CALL(*m_pDECommon_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::vt));
    EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, false, false));
}

#define AMXmlStr(freq) "<node name=\"intent\"><count>1</count><AM_frequency>" #freq "</AM_frequency></node>"
TEST_F(VR_IntentionParser_test, filterRadioFreqGen_AM)
{
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(AMXmlStr(531)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(AMXmlStr(999)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(AMXmlStr(1602)), false, false));

    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(AMXmlStr(997)), false, false));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(AMXmlStr(530)), false, false));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(AMXmlStr(1603)), false, false));
}

#undef AMXmlStr

#define AMPresetXmlStr(freq) "<node name=\"intent\"><count>1</count><node value=\"1234567_media_AM_preset_num_1\"><preset_number>" #freq "</preset_number></node></node>"
TEST_F(VR_IntentionParser_test, filterRadioFreqGen_AMPreset_checkRemoveNode)
{
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(AMPresetXmlStr(1)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(AMPresetXmlStr(4)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(AMPresetXmlStr(6)), false, false));

    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(AMPresetXmlStr(0)), false, false));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(AMPresetXmlStr(7)), false, false));
}

#undef AMPresetXmlStr

#define FMXmlStr(freq) "<node name=\"intent\"><count>1</count><FM_frequency>" #freq "</FM_frequency></node>"
TEST_F(VR_IntentionParser_test, filterRadioFreqGen_FM)
{
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(FMXmlStr(87.0)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(FMXmlStr(101.7)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(FMXmlStr(108.0)), false, false));

    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(FMXmlStr(101.23)), false, false));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(FMXmlStr(86.9)), false, false));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(FMXmlStr(108.1)), false, false));
}

// replaceSentenceValueCMN2034 BEGIN
TEST_F(VR_IntentionParser_test, replaceSentenceValueCMN2034)
{
    std::string xml1 = "<item><node value = \"CMN2034\">xxxx</node></item>";
    std::string xml2 = "<item>abc</item>";
    pugi::xml_document doc1, doc2;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    // not empty, not same
    m_intentionparser->replaceSentenceValueCMN2034(asrNode1.first_child());
    // empty
    m_intentionparser->replaceSentenceValueCMN2034(asrNode2.first_child());
}
// replaceSentenceValueCMN2034  END

// intelligentConventRadio BEGIN
TEST_F(VR_IntentionParser_test, intelligentConventRadio)
{
    pugi::xml_node asrNode;
    std::string inXml, outXml;

    asrNode = getXmlNode(FMXmlStr(1017.032));
    inXml = getNodeString(asrNode);
    EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, false, false));
    outXml = getNodeString(asrNode);
    EXPECT_NE(inXml, outXml);

    asrNode = getXmlNode(FMXmlStr(10812.34));
    inXml = getNodeString(asrNode);
    EXPECT_FALSE(m_intentionparser->filterIntention(asrNode, false, false));
    outXml = getNodeString(asrNode);
    EXPECT_NE(inXml, outXml);
}
// intelligentConventRadio END

// intelligentConventRadioCN BEGIN
TEST_F(VR_IntentionParser_test, intelligentConventRadioCN) // not used actually
{
    pugi::xml_node asrNode;
    std::string inXml, outXml;

    asrNode = getXmlNode(FMXmlStr(1017.032));
    inXml = getNodeString(asrNode);
    m_intentionparser->intelligentConventRadioCN(asrNode);
    outXml = getNodeString(asrNode);
    EXPECT_NE(inXml, outXml);

    asrNode = getXmlNode(FMXmlStr(10812.34));
    inXml = getNodeString(asrNode);
    m_intentionparser->intelligentConventRadioCN(asrNode);
    outXml = getNodeString(asrNode);
    EXPECT_NE(inXml, outXml);

    asrNode = getXmlNode(FMXmlStr(101.7));
    inXml = getNodeString(asrNode);
    m_intentionparser->intelligentConventRadioCN(asrNode);
    outXml = getNodeString(asrNode);
    // EXPECT_NE(inXml, outXml);
}
// intelligentConventRadioCN END
#undef FMXmlStr

// removeOtherItemsWithOutWell BEGIN
TEST_F(VR_IntentionParser_test, removeOtherItemsWithOutWell)
{
    std::string xml1 = "<root sentenceValue = \"CMN2034\"><list><items><data1>1234</data1><data2>5678</data2></items></list></root>";
    std::string xml2 = "<root sentenceValue = \"CMN ## 2034\"><list><items><data1>1234</data1><data2>5678</data2></items></list></root>";
    std::string xml3 = "<root sentenceValue = \"CMN2034\"><list><items><data1>1234</data1></items></list></root>";
    std::string xml4 = "<item><node sentenceValue = \"C ## MN2034\">xxxx</node></item>";
    pugi::xml_document doc1, doc2, doc3, doc4;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    doc3.load_string(xml3.c_str());
    pugi::xml_node asrNode3 = doc3.root();
    doc4.load_string(xml4.c_str());
    pugi::xml_node asrNode4 = doc4.root();
    // no ##, has next_sibling
    EXPECT_TRUE(m_intentionparser->removeOtherItemsWithOutWell(asrNode1.first_child()));
    // has ##, return false
    EXPECT_FALSE(m_intentionparser->removeOtherItemsWithOutWell(asrNode2.first_child()));
    // no ##, no next_sibling
    EXPECT_TRUE(m_intentionparser->removeOtherItemsWithOutWell(asrNode3.first_child()));
    // not match
    EXPECT_TRUE(m_intentionparser->removeOtherItemsWithOutWell(asrNode4.first_child()));
}
// removeOtherItemsWithOutWell  END

// removeNotSameConfidenceItem BEGIN
TEST_F(VR_IntentionParser_test, removeNotSameConfidenceItem)
{
    std::string xml1 = "<root sentenceValue = \"CMN2034\">\
    <list>\
     <items>\
       <data1>1234<POI_category_name confidence='4'/></data1>\
       <data2>5678<POI_category_name confidence='5'/></data2>\
      </items>\
     </list>\
    </root>";
    std::string xml2 = "<root sentenceValue = \"CMN2034\">\
    <list>\
     <items>\
       <data1><POI_category_name confidence = '4'/></data1>\
      </items>\
     </list>\
    </root>";
    std::string xml3 = "<root sentenceValue = \"CMN2034\">\
    <list>\
     <items>\
       <data1><POI_category_name confidence = '4'></data1>\
       <data2><POI_category_name confidence = '4'></data2>\
      </items>\
     </list>\
    </root>";
    pugi::xml_document doc1, doc2, doc3;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    doc3.load_string(xml3.c_str());
    pugi::xml_node asrNode3 = doc3.root();

    m_intentionparser->removeNotSameConfidenceItem(asrNode1.first_child());
    m_intentionparser->removeNotSameConfidenceItem(asrNode2.first_child());
    m_intentionparser->removeNotSameConfidenceItem(asrNode3.first_child());
}
// removeNotSameConfidenceItem  END

// replaceWellInSentence BEGIN
TEST_F(VR_IntentionParser_test, replaceWellInSentence)
{
    std::string xml1 = "<root sentenceValue = \"CMN1234567\">\
    <list>\
     <items>\
       <data1>\
            <POI_category_name confidence='4'>123456789</POI_category_name>\
       </data1>\
       <data2>5678<POI_category_name confidence='5'/></data2>\
      </items>\
     </list>\
    </root>";
    std::string xml2 = "<root sentenceValue = \"CMN ## 1234567\">\
    <list>\
     <items>\
       <data1>\
            <POI_category_name confidence='4'>CMN1234567</POI_category_name>\
       </data1>\
      </items>\
     </list>\
    </root>";
    std::string xml3 = "<root sentenceValue = \"CMN ## 1234567\">\
    <list>\
     <items>\
       <data1>\
            <POI_category_name confidence='4'>12 ## 3456789</POI_category_name>\
       </data1>\
       <data2><POI_category_name confidence = '4'></data2>\
      </items>\
     </list>\
    </root>";
    std::string xml4 = "<root sentenceValue = \"CMN ## 1234567\">\
    <list>\
     <items>\
       <data1>\
            <POI_category_name confidence='4'>CMN ## 123</POI_category_name>\
       </data1>\
       <data2><POI_category_name confidence = '4'></data2>\
      </items>\
     </list>\
    </root>";
    std::string xml5 = "<root sentenceValue = \"CMN2034\">\
    <list>\
     <items>\
       <data1><POI_category_name confidence = '4'/></data1>\
       <data2><POI_category_name confidence = '3'/></data2>\
      </items>\
     </list>\
    </root>";

    pugi::xml_document doc1, doc2, doc3, doc4, doc5;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    doc3.load_string(xml3.c_str());
    pugi::xml_node asrNode3 = doc3.root();
    doc4.load_string(xml4.c_str());
    pugi::xml_node asrNode4 = doc4.root();
    doc5.load_string(xml5.c_str());
    pugi::xml_node asrNode5 = doc5.root();

    m_intentionparser->replaceWellInSentence(asrNode1.first_child());
    m_intentionparser->replaceWellInSentence(asrNode2.first_child());
    m_intentionparser->replaceWellInSentence(asrNode3.first_child());
    m_intentionparser->replaceWellInSentence(asrNode4.first_child());
    m_intentionparser->replaceWellInSentence(asrNode5.first_child());
}
// replaceWellInSentence  END

// replaceWellInPoiNode BEGIN
TEST_F(VR_IntentionParser_test, replaceWellInPoiNode)
{
    std::string xml1 = "<root sentenceValue = \"CMN1234567\">\
    <list>\
     <items>\
       <data1>\
            <POI_category_name confidence='4'>123456789</POI_category_name>\
       </data1>\
       <data2>5678<POI_category_name confidence='5'/></data2>\
      </items>\
     </list>\
    </root>";
    std::string xml2 = "<root sentenceValue = \"CMN ## 1234567\">\
    <list>\
     <items>\
       <data1>\
            <POI_category_name confidence='4'>CMN ## 1234567</POI_category_name>\
       </data1>\
      </items>\
     </list>\
    </root>";
    std::string xml3 = "<root sentenceValue = \"CMN ## 1234567\"></root>";

    pugi::xml_document doc1, doc2, doc3;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    doc3.load_string(xml3.c_str());
    pugi::xml_node asrNode3 = doc3.root();

    m_intentionparser->replaceWellInPoiNode(asrNode1.first_child());
    m_intentionparser->replaceWellInPoiNode(asrNode2.first_child());
    m_intentionparser->replaceWellInPoiNode(asrNode3.first_child());
}
// replaceWellInPoiNode  END

// replaceWellNumPOI BEGIN
TEST_F(VR_IntentionParser_test, replaceWellNumPOI)
{
    std::string xml1 = "<node  value = 'CMN2006' sentenceValue = \"CMN1234567\"></node>";
    std::string xml2 = "<node  value = 'CMN2006' sentenceValue = \"CMN ## 4567\"></node>";
    std::string xml3 = "<root sentenceValue = \"CMN ## 1234567\"></root>";

    pugi::xml_document doc1, doc2, doc3;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    doc3.load_string(xml3.c_str());
    pugi::xml_node asrNode3 = doc3.root();

    m_intentionparser->replaceWellNumPOI(asrNode1.first_child());
    m_intentionparser->replaceWellNumPOI(asrNode2.first_child());
    m_intentionparser->replaceWellNumPOI(asrNode3.first_child());
}
// replaceWellNumPOI  END

#define FMPresetXmlStr(freq) "<node name=\"intent\"><count>1</count><node value=\"1234567_media_FM_preset_num_1\"><preset_number>" #freq "</preset_number></node></node>"
TEST_F(VR_IntentionParser_test, filterRadioFreqGen_FMPreset_checkRemoveNode)
{
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(FMPresetXmlStr(1)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(FMPresetXmlStr(6)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(FMPresetXmlStr(12)), false, false));

    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(FMPresetXmlStr(0)), false, false));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(FMPresetXmlStr(13)), false, false));
}

#undef FMPresetXmlStr

#define DABPresetXmlStr(freq) "<node name=\"intent\"><count>1</count><node value=\"1234567_media_DAB_preset_num_1\"><preset_number>" #freq "</preset_number></node></node>"
TEST_F(VR_IntentionParser_test, filterRadioFreqGen_DABPreset_checkRemoveNode)
{
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(DABPresetXmlStr(0)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(DABPresetXmlStr(1234)), false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(getXmlNode(DABPresetXmlStr(99999)), false, false));

    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(DABPresetXmlStr(-1)), false, false));
    EXPECT_FALSE(m_intentionparser->filterIntention(getXmlNode(DABPresetXmlStr(100000)), false, false));
}

#undef DABPresetXmlStr

// mergePhoneContactType BEGIN
#define ContactPhoneType(typ) "<contact_phone_type>" typ "</contact_phone_type>"
TEST_F(VR_IntentionParser_test, mergePhoneContactType)
{
    pugi::xml_node asrNode = getXmlNode("<node name=\"intent\"><node value=\"1234567_phone_contact\">"
        ContactPhoneType("preset") ContactPhoneType("default")
        ContactPhoneType("mobile") ContactPhoneType("phone")
        ContactPhoneType("home")   ContactPhoneType("house")
        ContactPhoneType("work")   ContactPhoneType("company")
        ContactPhoneType("other")  ContactPhoneType("others")   ContactPhoneType("other phone")
        ContactPhoneType("iphone") ContactPhoneType("unknown")
        "</node></node>");
    std::string inXml = getNodeString(asrNode);
    EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, false, false));
    std::string outXml = getNodeString(asrNode);
    EXPECT_NE(inXml, outXml);
}
// mergePhoneContactType END

// removeSameNode BEGIN
TEST_F(VR_IntentionParser_test, removeSameNode) // behavior probably wrong
{
    pugi::xml_node asrNode = getXmlNode("<node name=\"intent\" value=\"1234567_phone_contact\">\
        <count>3</count>\
        <item>\
          <contact_phone_type>company</contact_phone_type>\
          <contact_id>123</contact_id>\
        </item>\
        <item>\
          <contact_phone_type>company</contact_phone_type>\
          <contact_id>127</contact_id>\
        </item>\
        <item>\
          <contact_phone_type>company</contact_phone_type>\
          <contact_id>123</contact_id>\
        </item>\
        </node>");
    std::string inXml = getNodeString(asrNode);
    EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, false, false));
    std::string outXml = getNodeString(asrNode);
    EXPECT_NE(inXml, outXml);
}
// removeSameNode END

// filterValueNull BEGIN
#define NaviAddrXmlStr(state,city,district,street) "<node name=\"intent\">\
        <item>\
            <CMN2031_navi_addr_state_x>" state "</CMN2031_navi_addr_state_x>\
            <CMN2031_navi_addr_city_x>" city "</CMN2031_navi_addr_city_x>\
            <CMN2031_navi_addr_area_x>" district "</CMN2031_navi_addr_area_x>\
            <CMN2031_navi_addr_street_x>" street "</CMN2031_navi_addr_street_x>\
        </item>\
    </node>"
TEST_F(VR_IntentionParser_test, filterValueNull)
{
    EXPECT_FALSE(m_intentionparser->filterIntention(
        getXmlNode(NaviAddrXmlStr("", "", "", ""))
        , false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(
        getXmlNode(NaviAddrXmlStr("jiangsu", "nanjing", "xuanwu", "chengxian"))
        , false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(
        getXmlNode(NaviAddrXmlStr("jiangsu", "", "", ""))
        , false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(
        getXmlNode(NaviAddrXmlStr("", "nanjing", "", ""))
        , false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(
        getXmlNode(NaviAddrXmlStr("", "", "xuanwu", "chengxian"))
        , false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(
        getXmlNode(NaviAddrXmlStr("jiangsu", "nanjing", "", ""))
        , false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(
        getXmlNode(NaviAddrXmlStr("jiangsu", "", "xuanwu", "chengxian"))
        , false, false));
    EXPECT_TRUE(m_intentionparser->filterIntention(
        getXmlNode(NaviAddrXmlStr("", "nanjing", "xuanwu", "chengxian"))
        , false, false));
}

#undef NaviAddrXmlStr
// filterValueNull END

// replaceWellNum BEGIN
// TEST_F(VR_IntentionParser_test, replaceWellNum)
// {
//     pugi::xml_node asrNode = getXmlNode("<node name=\"intent\" sentenceValue=\"## 1 ## 2##3 # 4 ## \" />");
//     std::string inXml = getNodeString(asrNode);
//     EXPECT_TRUE(m_intentionparser->filterIntention(asrNode, false, false));
//     std::string outXml = getNodeString(asrNode);
//     EXPECT_NE(inXml, outXml);
// }
// replaceWellNum END
/* EOF */
