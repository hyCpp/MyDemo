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
 * @file VR_XmlGenerate.h
 * @brief Declaration file of class VR_XmlGenerate.
 *
 * This file includes the declaration of class VR_XmlGenerate.
 *
 * @attention used for C++ only.
 */

#ifndef VR_XMLGENERATE_H
#define VR_XMLGENERATE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

/**
 * @brief The VR_XmlGenerate class
 *
 * class declaration
 */
class VR_XmlGenerate
{
public:
    VR_XmlGenerate();
    ~VR_XmlGenerate();

    static bool GetRespPoiTimeToNATPXml(VR_String& strXml, int actionId, const VR_String& strResult,
        const VR_String& strDistance, const VR_String& strHour, const VR_String& strMin);
    static bool GetRespAddrBookToNATPXml(VR_String& strXml, int actionId, const VR_String& strResult,
        const VR_String& strName, const VR_String& strLongitude, const VR_String& strLatitude);
    static bool GetRespResultToNATPXml(VR_String& strXml, const VR_String& strActionId,
        const VR_String& strActionType, const VR_String& strResult);
    static bool GetRespResultToNATPXml(VR_String& strXml, const VR_String& strActionId, const VR_String& strResult,
        const VR_String& strMapOn, const VR_String& strNaviStart, const VR_String& strRouteOption, const std::vector<VR_String>& vecMode);
    static bool GetRespCarPositionInfoToNATPXml(VR_String& strXml, const VR_String& strProvinceName, const VR_String& strCityName, const VR_String& strLongitude, const VR_String& strLatitude);


    static VR_UINT32 GetActionId();

};


#endif // VR_XMLGENERATE_H
/* EOF */
