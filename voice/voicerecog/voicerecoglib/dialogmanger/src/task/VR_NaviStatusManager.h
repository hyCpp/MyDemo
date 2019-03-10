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
 * @file VR_NaviStatusManager.h
 * @brief Declaration file of class VR_NaviStatusManager.
 *
 * This file includes the declaration of class VR_NaviStatusManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_NAVISTATUSMANAGER_H
#define VR_NAVISTATUSMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_DialogTaskBase.h"

enum RounteType
{
    VR_NAVI_INVALID          = -1,
    VR_NAVI_POI_TEMP_ROUNT   = 0,
    VR_NAVI_CATEGORY_ROUNT   = 1,
};

/**
 * @brief The VR_NaviStatusManager class
 *
 * class declaration
 */
class VR_NaviStatusManager
{
public:
    VR_NaviStatusManager();
    ~VR_NaviStatusManager();

    VR_UINT32 GetActionId();
    void OnMessageNaviStatusUpdate(const VR_ProtoMessageBase &msg);
    void replyNaviStatusToVR(VR_String &strId);
    void OnMessageNaviLocInfoUpdate();
    bool GetNavigationStatusNotifyXML(VR_String& strMsg);
    bool GetNavigationStatusResponseXML(const VR_String &strId, VR_String &strMsg);
    bool GetNaviAddrLocationResponseXML(const VR_String& strId, const VR_String& strError, const VR_String& strName, const VR_String& strlongitude, const VR_String& strlatitude, VR_String& strMsg);
    void QueryNaviAddrLocation(VR_String &strId, VR_String &addrType);
    void ReplyNaviTimeToAddrToVR(VR_String &strId);
    void ReplyDestTimeToAddrErrorToVR(VR_String &strId);
    void SendReqToNATP(VR_String &strXml);
    void QueryNaviToDestinationFromFC(const VR_String &strXml);
    void ReplyNaviTimeToPOITOVR(const VR_ProtoMessageBase &msg);
    void QueryCategoryDestinationFromFC(const VR_String &strXml);
    void ReplyNaviSrchCategoryToVR(const VR_ProtoMessageBase &msg);
    void replyDestinationAddrToVR(VR_String &strId);
    void replyLocalAddrLocationToVR(VR_String &strId);
    void replyCursorLocationToVR(VR_String &strId);
    void OnMessageNaviStatusChanged(VR_String &strIsMapOn);
    void NotifyRouteOptionChanged();

private:
    VR_String    m_sMapStatus;
    VR_String    m_sNaviStatus;
    VR_String    m_sRouteOption;
    VR_String    m_sMapDirModeFirst;
    VR_String    m_sMapDirModeSecond;
    VR_String    m_sMapScaleMode;
    VR_String    m_sDistToDest;
    VR_String    m_sDestName;
    VR_String    m_sAddrName;
    VR_INT32     m_sDestLongit;
    VR_INT32     m_sDestlatit;
    VR_String    m_sEstTimeH;
    VR_String    m_sEstTimeM;
    VR_String    m_strPOIId;
    VR_String    m_strPOIName;
    VR_String    m_strPOILongit;
    VR_String    m_strPOILatit;
    VR_String    m_strCategyId;
    VR_String    m_strCategyName;
    VR_String    m_strSearchMethod;
    RounteType   m_emRounteType;
    VR_INT32     m_nCursorLon;
    VR_INT32     m_nCursorLat;
};


#endif // VR_NAVISTATUSMANAGER_H
/* EOF */
