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
 * @file VR_NCTutorialXMLBuilder.h
 * @brief Declaration file of class VR_NCTutorialXMLBuilder.
 *
 * This file includes the declaration of class VR_NCTutorialXMLBuilder.
 *
 * @attention used for C++ only.
 */

#ifndef VR_NC_TUTORIAL_XML_BUILDER_H
#define VR_NC_TUTORIAL_XML_BUILDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "ncore/NCTypesDefine.h"
#include "dbhelper/NCContentValues.h"
#include "ncore/NCString.h"
#include "pugixml.hpp"

namespace nutshell
{
namespace tutorial
{

#define VR_TUTORIAL_LIST_DATA_XML    "<tutorial agent=\"%1%\"></tutorial>"
#define VR_TUTORIAL_PLAY_INFO_XML    "<tutorial agent=\"%1%\" itemid=\"%2%\"></tutorial>"
#define VR_TUTORIAL_ERROR_INFO_XML   "<tutorial agent=\"errorinfo\"><error code=\"%1%\" desc=\"%2%\"></tutorial>"

/**
 * @brief The VR_NCTutorialXMLBuilder class
 *
 * class declaration.
 */
class VR_NCTutorialXMLBuilder
{
public:
    VR_NCTutorialXMLBuilder();
    virtual ~VR_NCTutorialXMLBuilder();

    bool AddTutorialNode(NCString agentName, INT sourceId = 0);
    bool AddItemNode(const NCContentValues& values);
    bool AddScreenNode(const NCContentValues& values);
    void GetXML(std::string& xmlStr);

    void BuildErrorXML(INT errorCode, const NCCHAR* errorDesc, std::string& xmlStr);

private:
    pugi::xml_document m_diffDocument;
    pugi::xml_node     m_agentNode;
    pugi::xml_node     m_itemNode;
    pugi::xml_node     m_screenNode;
};

}
}
#endif /* VR_NC_TUTORIAL_XML_BUILDER */
/* EOF */

