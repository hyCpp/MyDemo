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
/**
 * @file VR_PhoneBookListener.h
 * @brief Declaration file of class VR_PhoneBookListener.
 *
 * This file includes the declaration of class VR_PhoneBookListener.
 *
 * @attention used for C++ only.
 */

#include "VR_Log.h"

#include <boost/format.hpp>

#include "VR_PB_XmlBuildHelper.h"
#include "VR_PhoneBookXMLFormat.h"
#include "VR_ConfigureIF.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{

VR_PB_XmlBuildHelper::VR_PB_XmlBuildHelper()
{
}

VR_PB_XmlBuildHelper::~VR_PB_XmlBuildHelper()
{

}

bool VR_PB_XmlBuildHelper::AppendAgentXmlNode(const NCString &gramName)
{
    // m_xmlDocument.reset();
    // m_xmlAgentNode = pugi::xml_node();

    // pugi::xml_parse_result Result = m_xmlDocument.load_string(VR_PB_XML_GRAMMAR_NEW_STRING);
    // if (pugi::status_ok != Result.status) {
    //     VR_LOGD("[---- AppendAgentXmlNode: xml_Document.load_string(...) failed.ErrorCode:%d ----]", Result.status);
    //     return false;
    // }

    // m_xmlAgentNode = m_xmlDocument.child(VR_PB_XML_GRAMMAR_NEW_NODE);
    // if (m_xmlAgentNode.empty()) {
    //     VR_LOGD("[---- AppendAgentXmlNode: Add node grammar_new failed ----]");
    //     return false;
    // }

    return true;
}

bool VR_PB_XmlBuildHelper::AppendCategoryXmlNode(const NCString &categoryName)
{
    // m_xmlCategoryNode = pugi::xml_node();

    // m_xmlCategoryNode = m_xmlAgentNode.append_child(VR_PB_XML_CATEGORY_NODE);
    // if (m_xmlCategoryNode.empty()) {
    //     VR_LOGD("[---- AppendCategoryXmlNode: add node category failed ----]");
    //     return false;
    // }

    // pugi::xml_attribute attrName = m_xmlCategoryNode.append_attribute(VR_PB_XML_NAME_ATTRIBUTE);
    // if (attrName.empty()) {
    //     VR_LOGD("[---- AppendCategoryXmlNode: add attrbute name failed ----]");
    //     return false;
    // }

    // attrName.set_value(categoryName.getString());
    return true;
}

bool VR_PB_XmlBuildHelper::AppendMessageXmlNode(const INT id, const NCString &values)
{
    return false;
}

bool VR_PB_XmlBuildHelper::AppendFormalXmlNode(const INT id, const NCString &values)
{
    return false;
}

bool VR_PB_XmlBuildHelper::AppendAliasXmlNode(NCString &name)
{
    return false;
}

bool VR_PB_XmlBuildHelper::AppendListXmlNode(const INT &counts)
{
    // // append list node
    // m_xmlListNode = pugi::xml_node();

    // m_xmlListNode = m_xmlAgentNode.append_child(VR_PB_XML_LIST_NODE);
    // if (m_xmlListNode.empty()) {
    //     VR_LOGD("[----AppendListXmlNode: add list node failed ----]");
    //     return false;
    // }

    // // append header node base on list node
    // m_xmlHeaderNode = pugi::xml_node();

    // m_xmlHeaderNode = m_xmlListNode.append_child(VR_PB_XML_HEADER_NODE);
    // if (m_xmlHeaderNode.empty()) {
    //     VR_LOGD("[----AppendListXmlNode: add header node failed ----]");
    //     return false;
    // }

    // pugi::xml_node pageSize   = m_xmlHeaderNode.append_child(VR_PB_XML_PAGESIZE_NODE);
    // pugi::xml_node startIndex = m_xmlHeaderNode.append_child(VR_PB_XML_STARTINDEX_NODE);
    // pugi::xml_node count      = m_xmlHeaderNode.append_child(VR_PB_XML_COUNT_NODE);

    // pageSize.text().set(0);
    // startIndex.text().set(0);
    // count.text().set(counts);

    // // append items node base on list node
    // m_xmlItemsNode = pugi::xml_node();

    // m_xmlItemsNode = m_xmlListNode.append_child(VR_PB_XML_ITEMS_NODE);
    // if (m_xmlItemsNode.empty()) {
    //     VR_LOGD("[----AppendListXmlNode: add items node failed ----]");
    //     return false;
    // }

    // // append item node base on items node
    // m_xmlItemNode  = pugi::xml_node();

    // m_xmlItemNode = m_xmlItemsNode.append_child(VR_PB_XML_ITEM_NODE);
    // if (m_xmlItemNode.empty()) {
    //     VR_LOGD("[----AppendListXmlNode: add item node failed ----]");
    //     return false;
    // }

    return true;
}

bool VR_PB_XmlBuildHelper::AppendPhoneInfoXmlNode(const NCContentValues &values)
{
    return false;
}

bool VR_PB_XmlBuildHelper::AppendMessageInfoXmlNode(const NCContentValues &values)
{
    return false;
}

void VR_PB_XmlBuildHelper::GetXml(VR_String &xmlMsg)
{

}

} // dataprovider
} // navi

/* EOF */
