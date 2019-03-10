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
 * @file VR_XmlGenerate.cpp
 * @brief Declaration file of class VR_XmlGenerate.
 *
 * This file includes the declaration of class VR_XmlGenerate and Derived classes.
 *
 * @attention used for C++ only.
 */


#include "VR_XmlGenerate.h"
#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "EV_EventSender.h"
#include "ncore/NCString.h"
#include "VR_ConfigureIF.h"
#include "VR_Indentifier.hpp"

using namespace nutshell;


VR_XmlGenerate::VR_XmlGenerate()
{
}

VR_XmlGenerate::~VR_XmlGenerate()
{
}

bool VR_XmlGenerate::GetRespPoiTimeToNATPXml(VR_String& strXml, int actionId, const VR_String& strResult,
        const VR_String& strDistance, const VR_String& strHour, const VR_String& strMin)
{

    VR_LOGD("actionId[%d], strResult[%s], strDistance[%s], strHour[%s], strMin[%s]",
        actionId, strResult.c_str(), strDistance.c_str(), strHour.c_str(), strMin.c_str());

    VR_String strTemplate = VR_NAVI_DICT_TIME_TO_ADDR;
    VR_XmlStrParse xmlStrParse;

    if (!xmlStrParse.LoadString(strTemplate)) {
        VR_LOGE("Loading VR_NAVI_DICT_TIME_TO_ADDR failed!");
        return false;
    }

    xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(VR_XmlGenerate::GetActionId()));
    xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, std::to_string(actionId));
    VR_LOGD("strActionId[%s]", std::to_string(actionId));
    xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, strResult);
    xmlStrParse.setTextTo(VR_NAVI_DIST_TO_DEST, strDistance);
    xmlStrParse.setTextTo(VR_NAVI_TIME_TO_DST_H, strHour);
    xmlStrParse.setTextTo(VR_NAVI_TIME_TO_DST_M, strMin);
    strXml = xmlStrParse.TransferXMLtoString();

    return true;
}

bool VR_XmlGenerate::GetRespAddrBookToNATPXml(VR_String& strXml, int actionId, const VR_String& strResult,
        const VR_String& strName, const VR_String& strLongitude, const VR_String& strLatitude)
{
    VR_LOGD("actionId[%d], strResult[%s], strName[%s], strLongitude[%s], strLatitude[%s]",
        actionId, strResult.c_str(), strName.c_str(), strLongitude.c_str(), strLatitude.c_str());

    VR_String strTemplate = VR_NAVI_GET_ADDR_LOCAT;
    VR_XmlStrParse xmlStrParse;

    if (!xmlStrParse.LoadString(strTemplate)) {
        VR_LOGE("Loading VR_NAVI_GET_ADDR_LOCAT failed!");
        return false;
    }

    xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(VR_XmlGenerate::GetActionId()));
    xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, std::to_string(actionId));
    VR_LOGD("strActionId[%s]", std::to_string(actionId));

    xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, strResult);
    if (strName != std::string("")) {
        xmlStrParse.setTextTo(VR_NAVI_ADDR_NAME, strName);
    }  
    xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, strLongitude);
    xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, strLatitude);
    strXml = xmlStrParse.TransferXMLtoString();

    return true;
}

bool VR_XmlGenerate::GetRespCarPositionInfoToNATPXml(VR_String& strXml, const VR_String& strProvinceName, const VR_String& strCityName, const VR_String& strLongitude, const VR_String& strLatitude)
{
    VR_LOGD("strProvinceName[%s], strCityName[%s], strLongitude[%s], strLatitude[%s]",
        strProvinceName.c_str(), strCityName.c_str(), strLongitude.c_str(), strLatitude.c_str());

    VR_String strTemplate = VR_NAVI_LOACL_ADDR_NOTIFY_XML;
    VR_XmlStrParse xmlStrParse;

    if (!xmlStrParse.LoadString(strTemplate)) {
        VR_LOGE("Loading VR_NAVI_GET_ADDR_LOCAT failed!");
        return false;
    }

    xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(GetActionId()));
    xmlStrParse.setTextTo(VR_NAVI_PROVINCE, strProvinceName);
    xmlStrParse.setTextTo(VR_NAVI_CITY, strCityName);
    xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, strLongitude);
    xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, strLatitude);
    strXml = xmlStrParse.TransferXMLtoString();

    VR_LOGI("strXml : %s", strXml.c_str());

    return true;
}

bool VR_XmlGenerate::GetRespResultToNATPXml(VR_String& strXml, const VR_String& strActionId,
        const VR_String& strActionType, const VR_String& strResult)
{
    VR_LOGD("actionId[%s], actionType[%s], strResult[%s]",
        strActionId.c_str(), strActionType.c_str(), strResult.c_str());

    VR_String strTemplate = VR_DATA_MEDIA_RESPDEVICEPLAYINFO;
    VR_XmlStrParse xmlStrParse;

    if (!xmlStrParse.LoadString(strTemplate)) {
        VR_LOGE("Loading VR_NAVI_GET_ADDR_LOCAT failed!");
        return false;
    }

    xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(VR_XmlGenerate::GetActionId()));
    xmlStrParse.setTextTo(VR_XPATH_ACTION_TYPE, strActionType);
    xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, strActionId);
    xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, strResult);

    strXml = xmlStrParse.TransferXMLtoString();

    return true;
}

bool VR_XmlGenerate::GetRespResultToNATPXml(VR_String& strXml, const VR_String& strActionId, const VR_String& strResult,
        const VR_String& strMapOn, const VR_String& strNaviStart, const VR_String& strRouteOption, const std::vector<VR_String>& vecMode)
{
    VR_LOGD("actionid[%s], result[%s], mapon[%s], navistart[%s], routeOption[%s]",
        strActionId, strResult, strMapOn, strNaviStart, strRouteOption);
    if (vecMode.empty()) {
        VR_LOGE("do not have naviMode!");
        return false;
    }

    std::vector<VR_String>::const_iterator it = vecMode.begin();
    for (; it != vecMode.end(); ++it) {
        VR_LOGD("navi mode is [%s]", (*it).c_str());
    }

    VR_String strTemplate = VR_NAVI_STATUS_RESP_XML;
    VR_XmlStrParse xmlStrParse;

    if (!xmlStrParse.LoadString(strTemplate)) {
        VR_LOGE("Loading VR_NAVI_GET_ADDR_LOCAT failed!");
        return false;
    }

    xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(VR_XmlGenerate::GetActionId()));
    xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, strActionId);
    xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, strResult);
    xmlStrParse.setTextTo(VR_NAVI_MAP_STATUS, strMapOn);
    xmlStrParse.setTextTo(VR_NAVI_START_STAUS, strNaviStart);
    xmlStrParse.setTextTo(VR_NAVI_ROUTE_OPTION, strRouteOption);
    xmlStrParse.setTextListTo(VR_NAVI_MAP_OPTION, vecMode);

    strXml = xmlStrParse.TransferXMLtoString();

    return true;
}

// SendReqToNATP
VR_UINT32 VR_XmlGenerate::GetActionId()
{
    VR_IdGenerator nId;
    return nId.GetID();
}

/* EOF */

