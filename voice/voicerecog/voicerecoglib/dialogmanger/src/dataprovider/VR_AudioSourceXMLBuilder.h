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
 * @file VR_AudioSourceXMLBuilder.h
 * @brief Declaration file of class VR_AudioSourceXMLBuilder.
 *
 * This file includes the declaration of class VR_AudioSourceXMLBuilder.
 *
 * @attention used for C++ only.
 */

#ifndef VR_AUDIO_SOURCE_XML_BUILDER_H
#define VR_AUDIO_SOURCE_XML_BUILDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include "VR_XmlStrParse.h"
#include "dbhelper/NCContentValues.h"

namespace navi
{
namespace dataprovider
{

/**
 * @brief The VR_AudioSourceXMLBuilder class
 *
 * class declaration
 */
class VR_AudioSourceXMLBuilder
{
public:
    VR_AudioSourceXMLBuilder();
    ~VR_AudioSourceXMLBuilder();

    void GetXml(std::string& xml);
    bool AppendSourceStatusXML();
    bool UpdateSourceStatusNode(const nutshell::NCContentValues& values);

private:
    VR_XmlStrParse m_parse;
};

} // dataprovider
} // navi

#endif /* VR_AUDIO_SOURCE_XML_BUILDER_H */
/* EOF */
