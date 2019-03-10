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

#include "VR_NCTutorialXMLBuilder.h"

#ifndef VR_DEF_H
#include "VR_Def.h"
#endif

#ifndef NCSTRING_H
#include "NCString.h"
#endif

namespace nutshell
{
namespace tutorial
{

/**
 * @brief The VR_NCTutorialXMLBuilder_Test class
 *
 * class declaration.
 */
class VR_NCTutorialXMLBuilder_Test: public testing::Test
{
public:
    VR_NCTutorialXMLBuilder_Test();
    ~VR_NCTutorialXMLBuilder_Test();
    virtual VOID SetUp();
    virtual VOID TearDown();

private:
    VR_NCTutorialXMLBuilder_Test(const VR_NCTutorialXMLBuilder_Test&);
    VR_NCTutorialXMLBuilder_Test& operator =(const VR_NCTutorialXMLBuilder_Test&);

private:

    VR_NCTutorialXMLBuilder * m_testSuite = NULL;
};

VR_NCTutorialXMLBuilder_Test::VR_NCTutorialXMLBuilder_Test()
{
}

VR_NCTutorialXMLBuilder_Test::~VR_NCTutorialXMLBuilder_Test()
{
}

VOID VR_NCTutorialXMLBuilder_Test::SetUp()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_new VR_NCTutorialXMLBuilder();
    }
}

VOID VR_NCTutorialXMLBuilder_Test::TearDown()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
    }
    m_testSuite = NULL;
}

TEST_F(VR_NCTutorialXMLBuilder_Test, AddTutorialNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    EXPECT_TRUE(m_testSuite->AddTutorialNode("listdata"));
}

TEST_F(VR_NCTutorialXMLBuilder_Test, AddTutorialNode_loadStringError)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    EXPECT_FALSE(m_testSuite->AddTutorialNode("\"</<<</"));
}

TEST_F(VR_NCTutorialXMLBuilder_Test, AddItemNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    NCContentValues value;

    value.put("tutorial_index", 1);
    value.put("name", "gtest");
    value.put("version", "gtest");
    value.put("screen_number", 1);
    value.put("location_index", 1);

    m_testSuite->AddTutorialNode("listdata");
    EXPECT_TRUE(m_testSuite->AddItemNode(value));
}

TEST_F(VR_NCTutorialXMLBuilder_Test, AddItemNode_appendChildError)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    NCContentValues value;

    m_testSuite->m_agentNode = pugi::xml_node();

    EXPECT_FALSE(m_testSuite->AddItemNode(value));
}

TEST_F(VR_NCTutorialXMLBuilder_Test, AddScreenNode)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    NCContentValues value;

    value.put("screen_index", 1);
    value.put("audio", "gtest");
    value.put("sequence_number", 1);

    m_testSuite->AddTutorialNode("listdata");
    EXPECT_TRUE(m_testSuite->AddScreenNode(value));
}

TEST_F(VR_NCTutorialXMLBuilder_Test, AddScreenNode_appendChildError)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    NCContentValues value;

    m_testSuite->m_agentNode = pugi::xml_node();
    EXPECT_FALSE(m_testSuite->AddScreenNode(value));
}

TEST_F(VR_NCTutorialXMLBuilder_Test, GetXML)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    std::string xml;
    m_testSuite->GetXML(xml);
    SUCCEED();
}

TEST_F(VR_NCTutorialXMLBuilder_Test, BuildErrorXML)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    INT ERRCODE = 1;
    NCString DescStr = "Error";
    std::string xml;
    m_testSuite->BuildErrorXML(ERRCODE, DescStr, xml);
    EXPECT_STREQ(xml.c_str(), "<tutorial agent=\"errorinfo\"><error code=\"1\" desc=\"Error\"></tutorial>");
}

}
}
/* EOF */
