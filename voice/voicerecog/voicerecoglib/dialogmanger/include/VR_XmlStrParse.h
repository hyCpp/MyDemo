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
 * @file VR_XmlStrParse.h
 * @brief Declaration file of class VR_XmlStrParse.
 *
 * This file includes the declaration of class VR_XmlStrParse.
 *
 * @attention used for C++ only.
 */

#ifndef VR_XMLSTRPARSE_H
#define VR_XMLSTRPARSE_H

#ifndef __cplusplus
#error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "pugixml.hpp"
#include "VR_XmlStrParseDEF.h"

typedef std::map<VR_String, VR_String> XmlMap;
typedef std::vector<VR_String>         XmlVector;
typedef std::vector<XmlMap>            XmlMapVector;

extern const std::map<VR_String, emVriAutoProxyType> g_iAutoRouter;

/**
 * @brief The VR_XmlStrParse class
 *
 * class VR_XmlStrParse
 */
class VR_XmlStrParse
{
public:
    struct VRModelXMLNodeData
    {
        VR_String id;
        VR_String type;
        VR_String domain;
        VR_String status;
    };

public:
    VR_XmlStrParse();
    virtual ~VR_XmlStrParse();

    VR_String TransferXMLtoString();
    VR_String makeQueryXml(const VRModelXMLNodeData& data);
    XmlMapVector GetXMLListValue(const std::string& nodePath) const;
    VR_String GetXMLValue(const VR_String &tag, const VR_String& attributeName = "") const;

    bool LoadString(const VR_String & xmlStr);
    void addTextTo(const VR_String& path, const VR_String& content);
    void setTextTo(const VR_String& path, const VR_String& content);
    void setTextListTo(const VR_String &path, const std::vector<VR_String> &strvector);

    virtual void Reset();
    virtual void GetXmlString(VR_String& xml);
    virtual bool SaveFile(const VR_String& strPath);
    virtual bool LoadFile(const VR_String& strPath);
    virtual bool GetNodeBlock(const VR_String& xpath, VR_String& content);
    virtual bool GetNodeText(const VR_String& xpath, VR_String& content);
    virtual bool GetNodeText(const VR_String& xpath, int& content);
    virtual bool GetNodeText(const VR_String& xpath, unsigned int& content);
    virtual bool GetAttributeValue(const VR_String& xpath, const VR_String& attr, VR_String& content);
    virtual bool SetNodeText(const VR_String& xpath, const float& content);
    virtual bool SetNodeText(const VR_String& xpath, const int& content);
    virtual bool SetNodeText(const VR_String& xpath, const unsigned int& content);
    virtual bool SetNodeText(const VR_String& xpath, const VR_String& content);
    virtual bool AddNode(const VR_String& xpath, const VR_String& xmlNode);
    virtual bool AddNode(const VR_String& xpath, const VR_String& node, const int& content);
    virtual bool AddNode(const VR_String& xpath, const VR_String& node, const unsigned int& content);
    virtual bool AddNode(const VR_String& xpath, const VR_String& node, const float& content);
    virtual bool AddNode(const VR_String& xpath, const VR_String& node, const VR_String& content);
    virtual bool AddAttr(const VR_String& xpath, const VR_String& attr, const int& content);
    virtual bool AddAttr(const VR_String& xpath, const VR_String& attr, const unsigned int& content);
    virtual bool AddAttr(const VR_String& xpath, const VR_String& attr, const VR_String& content);

private:
    void addTextTo(pugi::xml_node node, const VR_String& content);
    void addTextTo(pugi::xpath_node_set nodes, const VR_String& content);
    void addTextTo(pugi::xml_node node, const VR_String& xpath, const VR_String& content);

private:
    pugi::xml_document m_dom;
};

#endif // VR_XMLSTRPARSE_H
/* EOF */
