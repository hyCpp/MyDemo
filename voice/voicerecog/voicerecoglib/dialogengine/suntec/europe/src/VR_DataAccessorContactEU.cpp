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

#include "VR_DataAccessorContactEU.h"
#include "VR_Log.h"

#include <sqlite3.h>
#include <pugixml.hpp>
#include <sstream>

#define VR_MSG_BUILD_INFO_PHONE_LIST                "phoneList"
#define VR_MSG_BUILD_INFO_SCREEN_LIST               "screenList"

#define VR_MSG_RESPONSE_NODENAME                    "data"
#define VR_MSG_RESPONSE_PHONE_INFO_RESULT           "result"
#define VR_MSG_RESPONSE_REQUEST_PHONE_TYPE_NAME     "requestPhoneTypeName"

#define VR_MSG_PHONE_INFO_PHONETYPE_ID              "phoneType"


VR_DataAccessorContactEU::VR_DataAccessorContactEU()
    :VR_DataAccessorContact()
{
    m_operationMap[VR_OPERATION_BUILD_INFO_LIST] = ContactOperation::BuildInfoList;
    m_operationMap[VR_OPERATION_GET_INFO_BY_TYPE] = ContactOperation::GetInfoByType;
}

bool VR_DataAccessorContactEU::getInfo(const std::string &operation, const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();
    VR_LOGD("Operation [%s]\nreqMsg [%s]", operation.c_str(), reqMsg.c_str());
    ContactOperation op = ContactOperation::None;
    if (m_operationMap.end() != m_operationMap.find(operation)) {
        op = m_operationMap[operation];
    }
    switch (op) {
    case ContactOperation::BuildInfoList:
        {
            return buildInfoList(reqMsg, response);
        }
    case ContactOperation::GetInfoByType:
        {
            return getInfoByType(reqMsg, response);
        }
    default:
        {
            return VR_DataAccessorContact::getInfo(operation, reqMsg, response);
        }
    }
}

bool VR_DataAccessorContactEU::phoneTypeAvailable(int phoneTypeID)
{
    bool result = (m_phoneTypeSet.end() != m_phoneTypeSet.find(phoneTypeID));
    return result;
}

bool VR_DataAccessorContactEU::getPhoneInfo(const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();

    if (!VR_DataAccessorContact::getPhoneInfo(reqMsg, response)) {
        return false;
    }

    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    std::string phoneTypeID = msgDoc.select_node((std::string("//") + VR_MSG_PHONE_INFO_PHONETYPE_ID).c_str()).node().text().as_string();
    std::string requestPhoneTypeName;
    if (!phoneTypeID.empty()) {
        requestPhoneTypeName = getPhoneTypeName(phoneTypeID);
    }

    // add requestPhoneTypeName Node
    pugi::xml_document responseDoc;
    responseDoc.load_string(response.c_str());
    pugi::xml_node responseNode = responseDoc.child(VR_MSG_RESPONSE_NODENAME);
    responseNode.insert_child_after(VR_MSG_RESPONSE_REQUEST_PHONE_TYPE_NAME, responseNode.child(VR_MSG_RESPONSE_PHONE_INFO_RESULT)).text().set(requestPhoneTypeName.c_str());

    std::ostringstream oss;
    responseNode.print(oss);
    response = oss.str();

    return true;
}

bool VR_DataAccessorContactEU::buildInfoList(const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();
    m_phoneListStr.clear();
    m_phoneTypeSet.clear();

    // process request message
    VR_LOGD("reqMsg [%s]", reqMsg.c_str());
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    pugi::xml_node phoneListNode = msgDoc.select_node((std::string("//") + VR_MSG_BUILD_INFO_PHONE_LIST).c_str()).node();
    pugi::xml_node screenListNode = msgDoc.select_node((std::string("//") + VR_MSG_BUILD_INFO_SCREEN_LIST).c_str()).node();

    // create default response message
    std::ostringstream oss;
    screenListNode.print(oss);
    response = oss.str();
    oss.str("");

    if (!phoneListNode) {
        VR_LOGE("reqMsg do not have phoneList node");
        return false;
    }

    if (!screenListNode) {
        VR_LOGE("reqMsg do not have screenList node");
        return false;
    }

    pugi::xml_node screenCountNode = msgDoc.select_node((std::string("//") + VR_MSG_BUILD_INFO_SCREEN_LIST + "//count").c_str()).node();
    if (!screenCountNode) {
        VR_LOGE("screenList node do not have count subnode");
        return false;
    }
    int count = screenCountNode.text().as_int();

    pugi::xml_node screenItemsNode = msgDoc.select_node((std::string("//") + VR_MSG_BUILD_INFO_SCREEN_LIST + "//items").c_str()).node();
    if (!screenItemsNode) {
        VR_LOGE("screenList node do not have items subnode");
        return false;
    }

    pugi::xml_node tempScreenItemsNode = screenItemsNode.parent().append_copy(screenItemsNode);

    pugi::xpath_node_set nodeSet = screenItemsNode.select_nodes(".//item[@phoneType]");
    for (pugi::xpath_node_set::iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
        if (screenItemsNode.remove_child(it->node())) {
            --count;
        }
    }

    pugi::xpath_node_set phoneInfoNodes = phoneListNode.select_nodes("//phoneInfo");
    pugi::xml_node lastNode;
    for (pugi::xpath_node_set::iterator it = phoneInfoNodes.begin(); it != phoneInfoNodes.end(); ++it) {
        pugi::xml_text phoneTypeText = it->node().child("phoneType").text();
        if (phoneTypeText.empty()) {
            continue;
        }
        int phoneTypeID = phoneTypeText.as_int() % 4;
        m_phoneTypeSet.insert(phoneTypeID);
        pugi::xml_node tempNode = tempScreenItemsNode.find_child_by_attribute("item", "phoneType", getPhoneTypeString(phoneTypeID).c_str());
        if (!tempNode) {
            continue;
        }
        if (lastNode) {
            lastNode = screenItemsNode.insert_copy_after(tempNode, lastNode);
        }
        else {
            lastNode = screenItemsNode.prepend_copy(tempNode);
        }
        lastNode.append_copy(it->node());
        ++count;
    }

    tempScreenItemsNode.parent().remove_child(tempScreenItemsNode);
    screenCountNode.text().set(count);

    screenListNode.print(oss);
    response = oss.str();
    oss.str("");
    VR_LOGD("response msg is [%s]", response.c_str());

    phoneListNode.print(oss);
    m_phoneListStr = oss.str();

    return true;
}

bool VR_DataAccessorContactEU::getInfoByType(const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();

    // create default response message
    response = "<phoneInfo>"
                    "<contactId></contactId>"
                    "<contactName></contactName>"
                    "<phoneKind></phoneKind>"
                    "<phoneType>0</phoneType>"
                    "<phoneTypeName></phoneTypeName>"
                    "<number></number>"
                    "<listType>0</listType>"
                    "<timestamp></timestamp>"
                "</phoneInfo>";

    // process request message
    VR_LOGD("reqMsg [%s]", reqMsg.c_str());
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    pugi::xml_node typeNode = msgDoc.select_node("//type").node();
    if (!typeNode) {
        VR_LOGE("can not find type in reqMsg");
        return false;
    }

    if (m_phoneListStr.empty()) {
        VR_LOGE("error in buildInfoList or not invoke buildInfoList before, can not get phoneList");
        return false;
    }

    std::string type = typeNode.text().as_string();

    pugi::xml_document phoneListDoc;
    phoneListDoc.load_string(m_phoneListStr.c_str());

    pugi::xpath_node_set nodes = phoneListDoc.select_nodes("//phoneInfo");
    pugi::xml_node phoneInfoNode;
    for (pugi::xpath_node_set::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        pugi::xml_text phoneTypeText = it->node().child("phoneType").text();
        if (!phoneTypeText.empty()) {
            if (type == getPhoneTypeString(phoneTypeText.as_int() % 4)) {
                phoneInfoNode = it->node();
                break;
            }
        }
    }

    std::ostringstream oss;
    phoneInfoNode.print(oss);
    response = oss.str();
    VR_LOGD("response msg is [%s]", response.c_str());

    return true;
}

// get phone type string used for script
std::string VR_DataAccessorContactEU::getPhoneTypeString(int id)
{
    switch (id) {
    case 0:
        return "MOBILE";
    case 1:
        return "HOME";
    case 2:
        return "WORK";
    case 3:
        return "OTHER";
    default:
        return "OTHER";
    }
}

/* EOF */
