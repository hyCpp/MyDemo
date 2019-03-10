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

#include "VR_RadioXMLBuilder_Test.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_RadioXMLBuilder* VR_RadioXMLBuilder_Test::m_testSuite = NULL;

void VR_RadioXMLBuilder_Test::SetUpTestCase()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_new VR_RadioXMLBuilder();
    }
}

void VR_RadioXMLBuilder_Test::TearDownTestCase()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
        m_testSuite = NULL;
    }
}

VR_RadioXMLBuilder_Test::VR_RadioXMLBuilder_Test()
{
    m_test = NULL;
}

VR_RadioXMLBuilder_Test::~VR_RadioXMLBuilder_Test()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

void VR_RadioXMLBuilder_Test::SetUp()
{
    if (NULL == m_test) {
        m_test = VR_new VR_RadioXMLBuilder();
    }
}

void VR_RadioXMLBuilder_Test::TearDown()
{
    if (NULL != m_test) {
        delete m_test;
        m_test = NULL;
    }
}

TEST_F(VR_RadioXMLBuilder_Test, AppendRadioAgentNode)
{
    ASSERT_NE(0, (__int64_t)m_testSuite);

    // bool isHdSubchannel = false;
    EXPECT_TRUE(m_testSuite->AppendRadioAgentNode());
}

// TEST_F(VR_RadioXMLBuilder_Test, AppendRadioAgentNode_HD)
// {
//    ASSERT_NE(0, (__int64_t)m_testSuite);

//    bool isHdSubchannel = true;
//    EXPECT_TRUE(m_testSuite->AppendRadioAgentNode());
// }

TEST_F(VR_RadioXMLBuilder_Test, AppendNodeException)
{
    ASSERT_NE(0, (INT64)m_test);

    // category node is empty
    EXPECT_FALSE(m_test->AppendCategoryNode("unkown"));

    // formal node is empty
    EXPECT_FALSE(m_test->AppendFromalNode(1, "unkown"));

    // alias node is empty
    EXPECT_FALSE(m_test->AppendAliasNode("unkown", "unkownPron"));
}

TEST_F(VR_RadioXMLBuilder_Test, AppendCategoryNode)
{
    ASSERT_NE(0, (__int64_t)m_testSuite);

    VR_String categoryName = "fmgenre";
    EXPECT_TRUE(m_testSuite->AppendCategoryNode(categoryName));
}

TEST_F(VR_RadioXMLBuilder_Test, AppendFromalNode)
{
    ASSERT_NE(0, (__int64_t)m_testSuite);

    VR_INT32 id = 1;
    VR_String name = "news";
    EXPECT_TRUE(m_testSuite->AppendFromalNode(id, name));

    id = 2;
    name = "country";
    EXPECT_TRUE(m_testSuite->AppendFromalNode(id, name));
}

TEST_F(VR_RadioXMLBuilder_Test, AppendAliasNode)
{
    ASSERT_NE(0, (__int64_t)m_testSuite);

    VR_String name = "countryAlias";
    VR_String pron = "countryPron";
    EXPECT_TRUE(m_testSuite->AppendAliasNode(name, pron));
}

TEST_F(VR_RadioXMLBuilder_Test, GetRadioDataXML)
{
    ASSERT_NE(0, (__int64_t)m_testSuite);

    VR_String xml;
    m_testSuite->GetRadioDataXML(xml);

    NCString tempXml = xml.c_str();
    EXPECT_STREQ(tempXml.getString(), tempXml.getString());
    SUCCEED();
}
}
}
/* EOF */
