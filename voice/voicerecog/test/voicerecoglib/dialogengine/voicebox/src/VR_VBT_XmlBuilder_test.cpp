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
#include <string>
#include "gtest/gtest.h"
#include "VBTExternalErrors.h"
#include "VR_VBT_EngineIF_mock.h"
#include "VR_VoiceBoxXmlBuilder.h"
#include "VR_Log.h"

/**
* VR_VBT_XmlBuilder_Test
*
* The class is just for VR_VoiceBoxXmlBuilder test.
*/
class VR_VBT_XmlBuilder_Test : public testing::Test
{
public:
    VR_VBT_XmlBuilder_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

private:
    VR_VoiceBoxXmlBuilder* m_xmlBuilder;
};

void
VR_VBT_XmlBuilder_Test::SetUp()
{
    m_xmlBuilder = new VR_VoiceBoxXmlBuilder();
}

void
VR_VBT_XmlBuilder_Test::TearDown()
{
    delete m_xmlBuilder;
    m_xmlBuilder = NULL;
}

TEST_F(VR_VBT_XmlBuilder_Test, buildStartMessageElement)
{
    std::string strSource = "source";
    std::string strClass = "class";
    std::string strRef = "ref";

    m_xmlBuilder->buildStartMessageElement(strSource, strClass, strRef);

    // strRefTest is null
    std::string strSourceTest = "source";
    std::string strClassTest = "class";
    std::string strRefTest = "";

    m_xmlBuilder->buildStartMessageElement(strSourceTest, strClassTest, strRefTest);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildStartActionResultElement)
{
    std::string strAgent = "agent";
    std::string strOp = "op";
    std::string strErrCode = "errcode";

    m_xmlBuilder->buildStartActionResultElement(strAgent, strOp, strErrCode);

    // strErrCodeTest is null
    std::string strAgentTest = "agent";
    std::string strOpTest = "op";
    std::string strErrCodeTest = "";

    m_xmlBuilder->buildStartActionResultElement(strAgentTest, strOpTest, strErrCodeTest);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildStartActionElement)
{
    std::string strAgent = "agent";
    std::string strOp = "op";

    m_xmlBuilder->buildStartActionElement(strAgent, strOp);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildDisplayElement)
{
    std::string strAgent = "agent";
    std::string strContent = "content";

    m_xmlBuilder->buildDisplayElement(strAgent, strContent);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildSystemResultElement)
{
    std::string element = "element";
    std::string strErrCode = "errcode";

    m_xmlBuilder->buildSystemResultElement(element, strErrCode);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildStartDisplayElement)
{
    std::string strAgent = "agent";
    std::string strContent = "errcode";

    m_xmlBuilder->buildStartDisplayElement(strAgent, strContent);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildGivenElement_Node)
{
    pugi::xml_node node;
    std::string strElement = "element";
    std::string strValue = "agent";
    std::string strAttribute = "attribute";
    std::string strAttributeValue = "attribute";

    m_xmlBuilder->buildGivenElement(node, strElement, strValue, strAttribute, strAttributeValue);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildGivenElement_Node_ManyAttribute)
{
    // node hava value and attribute
    pugi::xml_node node;
    std::string strElement = "element";
    std::string strValue = "agent";
    StructNode attribute;
    attribute.Name = "name";
    attribute.Value = "value";
    VoiceVector<StructNode>::type attributeVector;
    attributeVector.push_back(attribute);

    m_xmlBuilder->buildGivenElement(node, strElement, strValue, attributeVector);
    // node have no attribute and have no value
    pugi::xml_node nodeNoValue;
    std::string strElementNoValue = "element";
    std::string strNoValue = "";
    StructNode attr;
    attr.Name = "";
    attr.Value = "value";
    VoiceVector<StructNode>::type attributeVectorNoName;
    attributeVectorNoName.push_back(attr);

    m_xmlBuilder->buildGivenElement(nodeNoValue, strElementNoValue, strNoValue, attributeVectorNoName);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildGivenElement_Vector)
{
    std::string strElement = "element";
    VoiceVector<StructNode>::type attributeVector;
    StructNode attribute;
    attribute.Name = "name";
    attribute.Value = "value";
    attributeVector.push_back(attribute);

    m_xmlBuilder->buildGivenElement(strElement, attributeVector);

    // node have no attribute
    std::string strElementNoAttr = "element";
    VoiceVector<StructNode>::type attributeVectorEmpty;
    m_xmlBuilder->buildGivenElement(strElementNoAttr, attributeVectorEmpty);

    // node have attribute value
    std::string strElementNoName = "element";
    VoiceVector<StructNode>::type attributeVectorNoName;
    StructNode attr;
    attr.Name = "";
    attr.Value = "value";
    attributeVectorNoName.push_back(attr);

    m_xmlBuilder->buildGivenElement(strElementNoName, attributeVectorNoName);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildListItemElement)
{
    pugi::xml_node node;
    std::string strListItem = "element";
    VoiceVector<std::string>::type strItem;
    std::string item = "Message";
    strItem.push_back(item);

    m_xmlBuilder->buildListItemElement(node, strListItem, strItem);

    // strItemTest is empty
    pugi::xml_node nodeTest;
    std::string strListItemTest = "element";
    VoiceVector<std::string>::type strItemTest;

    m_xmlBuilder->buildListItemElement(nodeTest, strListItemTest, strItemTest);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildListItemChildElement)
{
    pugi::xml_node node;
    std::string strElementType = "elementType";
    std::string nodeString[] = { "nodeString", "" };
    VoiceVector<StructNode>::type m_vector;
    StructNode attribute;
    attribute.Name = "name";
    attribute.Value = "value";
    m_vector.push_back(attribute);
    attribute.Name = "nameSec";
    attribute.Value = "valueSec";
    m_vector.push_back(attribute);

    m_xmlBuilder->buildListItemChildElement(node, strElementType, nodeString, m_vector);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildMsgParameterElement)
{
    pugi::xml_node node;
    std::string name = "name";
    std::string value = "value";

    m_xmlBuilder->buildMsgParameterElement(node, name, value);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildMessageActionElement)
{
    pugi::xml_node node;
    std::string strType = "type";

    m_xmlBuilder->buildMessageActionElement(node, strType);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, buildMsgParameterList)
{
    pugi::xml_node node;
    VoiceVector<Parameter>::type parameter;
    Parameter para;
    para.name = "name";
    para.nameValue = "nameValue";
    para.parameterValue = "paraValue";
    para.value = "value";
    parameter.push_back(para);
    m_xmlBuilder->buildMsgParameterList(node, parameter);

    // no parameter
    pugi::xml_node nodeTest;
    VoiceVector<Parameter>::type parameterTest;
    m_xmlBuilder->buildMsgParameterList(nodeTest, parameterTest);
    SUCCEED();
}

TEST_F(VR_VBT_XmlBuilder_Test, getXmlString)
{
    std::string strAgent = "agent";
    std::string strOp = "op";
    m_xmlBuilder->buildStartActionElement(strAgent, strOp);
    m_xmlBuilder->getXmlString();
    SUCCEED();
}
/* EOF */
