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

/* Standard Header */
#include <cstring>

/* Suntec Header */
#ifndef VR_VOICEBOXXMLBUILDER_H
#    include "VR_VoiceBoxXmlBuilder.h"
#endif

// Constructor
VR_VoiceBoxXmlBuilder::VR_VoiceBoxXmlBuilder()
{
}

// Destructor
VR_VoiceBoxXmlBuilder::~VR_VoiceBoxXmlBuilder()
{
}

/**
*build the start tag of a display element
*/

pugi::xml_node VR_VoiceBoxXmlBuilder::buildDisplayElement(const std::string &strAgent, const std::string &strContent)
{
    pugi::xml_node root_node = m_docTargetXML.append_child("display");
    root_node.append_attribute("agent").set_value(strAgent.c_str());
    root_node.append_attribute("content").set_value(strContent.c_str());
    return root_node;
}

std::string VR_VoiceBoxXmlBuilder::getXmlString()
{
    pugi::xml_node rootNode = m_docTargetXML.first_child();
    xml_string_writer writer;
    if (rootNode) {
        m_docTargetXML.save(writer);
    }
    return writer.result;
}

pugi::xml_node VR_VoiceBoxXmlBuilder::buildGivenElement(pugi::xml_node node, const std::string &strElement,
    const std::string &strValue, const std::string &strAttribute, const std::string &strAttributeValue)
{
    pugi::xml_node givenNode = node.append_child(strElement.c_str());
    if (!strValue.empty()) {
        givenNode.append_child(pugi::node_pcdata).set_value(strValue.c_str());
    }
    if (!strAttribute.empty()) {
        givenNode.append_attribute(strAttribute.c_str()).set_value(strAttributeValue.c_str());
    }
    return givenNode;
}

/**
* build the start tag of a action  element
*/

pugi::xml_node VR_VoiceBoxXmlBuilder::buildStartActionElement(const std::string &strAgent, const std::string &strOp)
{
    pugi::xml_node root_node = m_docTargetXML.append_child("action");
    root_node.append_attribute("agent").set_value(strAgent.c_str());
    root_node.append_attribute("op").set_value(strOp.c_str());
    return root_node;
}

/* EOF */
