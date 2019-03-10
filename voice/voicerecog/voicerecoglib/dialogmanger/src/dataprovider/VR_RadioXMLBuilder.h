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
 * @file VR_RadioXMLBuilder.h
 * @brief Declaration file of class VR_RadioXMLBuilder.
 *
 * This file includes the declaration of class VR_RadioXMLBuilder.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RADIO_XML_BUILDER_H
#define VR_RADIO_XML_BUILDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#    include "VR_Macro.h"
#endif

#include "pugixml.hpp"

namespace navi
{
namespace dataprovider
{

/**
 * @brief The VR_RadioXMLBuilder class
 *
 * class declaration
 */
class VR_RadioXMLBuilder
{
public:
    VR_RadioXMLBuilder();
    ~VR_RadioXMLBuilder();

    bool AppendRadioAgentNode();
    bool AppendCategoryNode(VR_String categoryName);
    bool AppendFromalNode(VR_INT32 id, VR_String strName);
    bool AppendAliasNode(VR_String name, VR_String pron);

    void GetRadioDataXML(VR_String& strxml);

private:
    pugi::xml_document m_radioDocument;
    pugi::xml_node     m_agentNode;
    pugi::xml_node     m_categoryNode;
    pugi::xml_node     m_formalNode;
    pugi::xml_node     m_aliasNode;
};

} // dataprovider
} // navi

#endif /* VR_RADIO_XML_BUILDER_H */
/* EOF */
