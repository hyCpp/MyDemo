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
 * @file VR_TransformXmlToXml.h
 * @brief Declaration file of class VR_TransformXmlToXml.
 *
 * This file includes the declaration of class VR_TransformXmlToXml.
 *
 * @attention used for C++ only.
 */

#ifndef VR_TRANSFORMXMLTOXML_H
#define VR_TRANSFORMXMLTOXML_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_XmlStrParse.h"
#include "PublicConvertLibrary.h"

/**
 * @brief VR_TransformXmlToXml class
 *
 * class declaration
 */
class VR_TransformXmlToXml
{
public:
    VR_TransformXmlToXml();
    ~VR_TransformXmlToXml();

public:
    VR_String Transform(int id, const std::string& msg, emVriAutoProxyType& emType);
    VR_String CreateStateXML(int id, VR_String& strState, VR_String& strStateAction);

protected:
    // amazon
    VR_String CreateVolumeXML(int id, VR_String& strVolume);
    VR_String CreateSunroofXML(int id, VR_String& strState);
    VR_String CreatePlayMusicXML(int id);
    VR_String CreateQuitMusicXML(int id);
    VR_String CreatePhoneCallXML(int id, VR_String& strName, VR_String& strNumber);
    VR_String CreateShowPhoneCallXML(int id, VR_String& strName, VR_String& strNumber, VR_String& strType);
    VR_String CreateCalculateRouteXML(int id, VR_String& strLon, VR_String& strLat);

    VR_String TransferLonAndLat(VR_String& strValue);
    VR_String GetPoiListXml(VR_String &strXml);
    VR_INT32 GetDirection(VR_String &strlon, VR_String &strlat, ULONG& distance, WORD& direction);
};

#endif /* VR_TRANSFORMXMLTOXML_H */
/* EOF */
