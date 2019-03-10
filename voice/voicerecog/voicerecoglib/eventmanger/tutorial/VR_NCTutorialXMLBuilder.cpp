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
#include <sstream>
#include <boost/format.hpp>

#include "VR_Log.h"
#include "VR_NCTutorialXMLBuilder.h"

namespace nutshell
{
namespace tutorial
{

VR_NCTutorialXMLBuilder::VR_NCTutorialXMLBuilder()
    : m_agentNode()
    , m_itemNode()
    , m_screenNode()
{
    VR_LOGD_FUNC();
}

VR_NCTutorialXMLBuilder::~VR_NCTutorialXMLBuilder()
{
}

bool VR_NCTutorialXMLBuilder::AddTutorialNode(NCString agentName, INT sourceId)
{
    m_agentNode = pugi::xml_node();
    m_itemNode = pugi::xml_node();
    m_screenNode = pugi::xml_node();
    m_diffDocument.reset();

    // 1.trans string
    std::string xmlStr = "";
    if (0 == sourceId) {
        xmlStr = (boost::format(VR_TUTORIAL_LIST_DATA_XML) % agentName).str();
    }
    else {
        xmlStr = (boost::format(VR_TUTORIAL_PLAY_INFO_XML) % agentName % sourceId).str();
    }

    VR_LOGD("xml_document begin load string:%s", xmlStr.c_str());
    pugi::xml_parse_result Result = m_diffDocument.load_string(xmlStr.c_str());
    if (pugi::status_ok != Result.status) {
        VR_LOGD("xml_document load string failed. ErrorCode:%d", Result.status);
        return false;
    }
    VR_LOGD("xml_document end load string");

    m_agentNode = m_diffDocument.child("tutorial");
    return true;
}

bool VR_NCTutorialXMLBuilder::AddItemNode(const NCContentValues& values)
{
    // item node
    VR_LOGD("xml_document begin append item node");
    pugi::xml_node itemNode = m_agentNode.append_child("item");
    if (pugi::xml_node() == itemNode) {
        VR_LOGD("xml_document append item node failed");
        return false;
    }

//    itemNode.append_attribute("id") = values.getIntegerValue("tutorial_index");
//    itemNode.append_attribute("name") = values.getStringValue("name");
//    itemNode.append_attribute("version") = values.getStringValue("version");
//    itemNode.append_attribute("screennum") = values.getIntegerValue("screen_number");
//    itemNode.append_attribute("position") = values.getIntegerValue("location_index");
//    VR_LOGD("xml_document end append item attrbute");

    return true;
}

bool VR_NCTutorialXMLBuilder::AddScreenNode(const NCContentValues& values)
{
    // screen node
    VR_LOGD("xml_document begin append add screen node");
    pugi::xml_node item = m_agentNode.append_child("screen");
    if (pugi::xml_node() == item) {
        VR_LOGD("xml_document append item node failed");
        return false;
    }

//    item.append_attribute("id") = values.getIntegerValue("screen_index");
//    item.append_attribute("audio") = values.getStringValue("audio");
//    item.append_attribute("sequenceNumber") = values.getIntegerValue("sequence_number");
//    VR_LOGD("xml_document end append screen attrbute");

    return true;
}

void VR_NCTutorialXMLBuilder::GetXML(std::string& xmlStr)
{
    std::stringstream ss;
    m_diffDocument.print(ss);
    xmlStr = ss.str();
}

void VR_NCTutorialXMLBuilder::BuildErrorXML(INT errorCode, const NCCHAR* errorDesc, std::string& xmlStr)
{
    VR_LOGD("tutorial error xml. errorCode:%d desc:%s", errorCode, errorDesc);
    xmlStr = (boost::format(VR_TUTORIAL_ERROR_INFO_XML) % errorCode % errorDesc).str();
}
    
}
}
/* EOF */

