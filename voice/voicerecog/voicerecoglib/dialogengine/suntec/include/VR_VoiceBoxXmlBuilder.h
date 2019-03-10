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

/**
 * @file VR_VoiceBoxXmlBuilder.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_VOICEBOXXMLBUILDER_H
#define VR_VOICEBOXXMLBUILDER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>
#include <iostream>

/* Suntec Header */
#ifndef MEM_VECTOR_H
#    include "MEM_vector.h"
#endif

#ifndef HEADER_PUGIXML_HPP
#    include "pugixml.hpp"
#endif

struct xml_string_writer : pugi::xml_writer
{
    std::string result;
    virtual void write(const void* data, size_t size)
    {
        result += std::string(static_cast<const char*>(data), size);
    }
};

/**
 * @brief The VR_VoiceBoxXmlBuilder class
 *
 * class declaration
 */
class VR_VoiceBoxXmlBuilder
{
public:
    VR_VoiceBoxXmlBuilder();
    // message
    virtual ~VR_VoiceBoxXmlBuilder();


    /*
     * build the node of display
     *
     * @param strAgent : the value of agent
     * @param strContent : the value of strContent
     * @return the child's node of display
     */

    pugi::xml_node buildDisplayElement(const std::string &strAgent, const std::string &strContent);

    pugi::xml_node buildGivenElement(pugi::xml_node node, const std::string &strElement, const std::string &strValue,
        const std::string &strAttribute, const std::string &strAttributeValue);
    /*
     * build the node of action-result
     *
     * @param strAgent : the value of agent
     * @param strOp : the value of op
     * @return the child's node of action-result
     */

    pugi::xml_node buildStartActionElement(const std::string &strAgent, const std::string &strOp);

    // get the string format of building xml
    std::string getXmlString();
    
private:
    // build the xml
    pugi::xml_document m_xml_document;
    pugi::xml_document m_docTargetXML;
};

#endif // VR_VOICEBOXXMLBUILDER_H
/* EOF */
