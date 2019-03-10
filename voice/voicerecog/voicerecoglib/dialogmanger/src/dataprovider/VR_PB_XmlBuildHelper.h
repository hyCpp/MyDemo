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
#ifndef VR_PB_XMLBUILDHELPER_H
#define VR_PB_XMLBUILDHELPER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef NCVARIANT_H
#   include "ncore/NCVariant.h"
#endif

#ifndef NCURI_H
#   include "ncore/NCUri.h"
#endif

#ifndef NCENVIRONMENT_H
#   include "system/NCEnvironment.h"
#endif

#ifndef NCCONTENTVALUES_H
#   include "dbhelper/NCContentValues.h"
#endif

#ifndef NCCONTENTRESOLVER_H
#   include "contentresolver/NCContentResolver.h"
#endif

#ifndef NCSQLITEOPENHELPER_H
#   include "dbhelper/NCSQLiteOpenHelper.h"
#endif

// #ifndef NG_RESOURCE_MANAGER_H
// #   include "ngresource/NGResourceManager.h"
// #endif

#include "pugixml.hpp"

namespace navi
{
namespace dataprovider
{

/**
 * @brief The VR_PB_XmlBuildHelper class
 *
 * class declaration
 */
class VR_PB_XmlBuildHelper
{
public:
    VR_PB_XmlBuildHelper();
    virtual ~VR_PB_XmlBuildHelper();

public:

    /**
     * @brief AppendAgentXmlNode
     * @return
     */
    virtual bool AppendAgentXmlNode(const nutshell::NCString &gramName);

    /**
     * @brief AppendCategoryXmlNode
     * @param contactsNum
     * @return
     */
    virtual bool AppendCategoryXmlNode(const nutshell::NCString &categoryName);

    /**
     * @brief AppendMessageXmlNode for quickreply message
     * @return
     */
    virtual bool AppendMessageXmlNode(const nutshell::INT id, const nutshell::NCString &values);

    /**
     * @brief AppendFormalXmlNode for messageType
     * @return
     */
    virtual bool AppendFormalXmlNode(const nutshell::INT id, const nutshell::NCString& values);

    /**
     * @brief AppendAliasNode
     * @param name
     * @return
     */
    virtual bool AppendAliasXmlNode(nutshell::NCString &name);

    /**
     * @brief AppendListNode
     * @return
     */
    virtual bool AppendListXmlNode(const nutshell::INT &counts);

    /**
     * @brief AppendPhoneInfoXmlNode for call history list
     * @param values
     * @return
     */
    virtual bool AppendPhoneInfoXmlNode(const nutshell::NCContentValues& values);

    /**
     * @brief AppendMessageInfoXmlNode for message history list
     * @param values
     * @return
     */
    virtual bool AppendMessageInfoXmlNode(const nutshell::NCContentValues& values);

    /**
     * @brief Get the last xml info
     * @param xmlMsg
     */
    virtual void GetXml(VR_String& xmlMsg);

protected:
    pugi::xml_document m_xmlDocument;
    pugi::xml_node     m_xmlAgentNode;
    pugi::xml_node     m_xmlCategoryNode;
    pugi::xml_node     m_xmlMessageNode;
    pugi::xml_node     m_xmlFormalNode;

    // be used by query callhistory or messagehistory list
    pugi::xml_node     m_xmlListNode;
    pugi::xml_node     m_xmlHeaderNode;
    pugi::xml_node     m_xmlItemsNode;
    pugi::xml_node     m_xmlItemNode;
    pugi::xml_node     m_xmlPhoneInfoNode;
    pugi::xml_node     m_xmlMessageInfoNode;

};


} // dataprovider
} // navi

#endif /* VR_PB_XMLBUILDHELPER_H */
/* EOF */
