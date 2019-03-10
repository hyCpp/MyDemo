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
#include "stdafx.h"
#include <sstream>

#include <boost/format.hpp>
#include "pugixml.hpp"

#include "VR_Log.h"
#include "VR_RadioXMLBuilder.h"
#include "VR_RadioXMLFormat.h"

using namespace pugi; 
using namespace nutshell;
namespace navi
{
namespace dataprovider
{

VR_RadioXMLBuilder::VR_RadioXMLBuilder()
    : m_agentNode()
    , m_categoryNode()
    , m_formalNode()
    , m_aliasNode()
{
    VR_LOGD_FUNC();
}

VR_RadioXMLBuilder::~VR_RadioXMLBuilder()
{
    VR_LOGD_FUNC();
}

bool VR_RadioXMLBuilder::AppendRadioAgentNode()
{
    m_agentNode = pugi::xml_node();
    m_categoryNode = pugi::xml_node();
    m_formalNode = pugi::xml_node();
    m_aliasNode = pugi::xml_node();
    m_radioDocument.reset();

    pugi::xml_parse_result Result;

    VR_LOGD("radio xml document load string:%s", VR_RADIO_NEW_XML);
    Result = m_radioDocument.load_string(VR_RADIO_NEW_XML);

    if (pugi::status_ok != Result.status) {
        VR_LOGD("xml_document load string failed. ErrorCode:%d", Result.status);
        return false;
    }
    VR_LOGD("xml_document end load string");

    m_agentNode = m_radioDocument.child(VR_RADIO_AGENT_FIELD_PHRASE);
    if (pugi::xml_node() == m_agentNode) {
        VR_LOGD("xml_document append grammar_new child failed");
        return false;
    }

    return true;
}

bool VR_RadioXMLBuilder::AppendCategoryNode(VR_String categoryName)
{
    m_categoryNode = pugi::xml_node();
    m_formalNode = pugi::xml_node();
    m_aliasNode = pugi::xml_node();

    m_categoryNode = m_agentNode.append_child(VR_RADIO_CATEGORY_FIELD_PHRASE);
    if (pugi::xml_node() ==  m_categoryNode) {
        VR_LOGD("xml_document append category child failed");
        return false;
    }

    pugi::xml_attribute attrName = m_categoryNode.append_attribute(VR_RADIO_CATEGORY_ATTR);
    if (pugi::xml_attribute() == attrName) {
        VR_LOGD("xml_document append category attrbute failed");
        return false;
    }
    attrName = categoryName.c_str();

    return true;
}

bool VR_RadioXMLBuilder::AppendFromalNode(VR_INT32 id, VR_String strName)
{
    m_formalNode = pugi::xml_node();
    m_aliasNode = pugi::xml_node();

    m_formalNode = m_categoryNode.append_child(VR_RADIO_FORMAL_FIELD_PHRASE);
    if (pugi::xml_node() == m_formalNode) {
        VR_LOGD("xml_document append formal child failed");
        return false;
    }

    pugi::xml_attribute attrId = m_formalNode.append_attribute(VR_RADIO_FORMAL_ATTR_ID);
    pugi::xml_attribute attrName = m_formalNode.append_attribute(VR_RADIO_FORMAL_ATTR_NAME);
    if ((pugi::xml_attribute() == attrId) || (pugi::xml_attribute() == attrName)) {
        VR_LOGD("xml_document append formal attrbute failed");
        return false;
    }
    attrId  = id;
    attrName = strName.c_str();

    return true;
}

bool VR_RadioXMLBuilder::AppendAliasNode(VR_String name, VR_String pron)
{
    m_aliasNode = m_formalNode.append_child(VR_RADIO_ALIAS_FIELD_PHRASE);
    if (pugi::xml_node() == m_aliasNode) {
        VR_LOGD("xml_document append delete child failed");
        return false;
    }

    pugi::xml_attribute attrName = m_aliasNode.append_attribute(VR_RADIO_ALIAS_ATTR_NAME);
    pugi::xml_attribute attrPron = m_aliasNode.append_attribute(VR_RADIO_ALIAS_ATTR_PRON);
    if ((pugi::xml_attribute() == attrName) || (pugi::xml_attribute() == attrPron)) {
        VR_LOGD("xml_document append category attrbute failed");
        return false;
    }

    attrName = name.c_str();
    attrPron = pron.c_str();
    return true;
}

void VR_RadioXMLBuilder::GetRadioDataXML(VR_String& strxml)
{
    std::stringstream ss;
    m_radioDocument.print(ss);

    VR_LOGD("%s", ss.str().c_str());
    strxml = ss.str();
}

}
}

/* EOF */
