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
 * @file VR_NaviMessageHandler.h
 * @brief Declaration file of class VR_NaviMessageHandler.
 *
 * This file includes the declaration of class VR_NaviMessageHandler.
 *
 * @attention used for C++ only.
 */

#ifndef VR_NAVIMESSAGEHANDLER_H
#define VR_NAVIMESSAGEHANDLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#    include "VR_Macro.h"
#endif

#include "VR_DialogTaskBase.h"
#include "VR_XmlStrParse.h"
#include "navi/NaviInfo/NaviInfoBase.pb.h"
#include "BL_Timer.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_GetCarPositionTimer);

/**
 * @brief The VR_GetCarPositionTimer class
 *
 * class declaration
 */
class  VR_GetCarPositionTimer : public BL_Timer
{
public:
    VR_GetCarPositionTimer();
    virtual ~VR_GetCarPositionTimer();
    virtual VOID OnTimer();
};

/**
 * @brief The VR_NaviMessageHandler class
 *
 * class declaration
 */
class VR_NaviMessageHandler
{
public:
    VR_NaviMessageHandler();
    ~VR_NaviMessageHandler();

    void OnRequestVriAutoProxy(const VR_String& strXml);
    void OnMessageNaviResp(const VR_ProtoMessageBase &msg);

    void SendReqToNATP(VR_String &strXml);
    VR_UINT32 GetActionId();
    void SetMapStatus(const VR_String& strMapStatus);

private:
    // request from natp
    void RequestAddressBookAddr(const VR_String& strXml);
    void RequestDisTimeToAddr(const VR_String& strXml);
    void RequestNaviToDestination(VR_XmlStrParse& xmlStrParse);
    void RequestSetRouteOption(const VR_String& strXml);
    void RequestNavigationStatus(const VR_String& strXml);

    void QuerySetModeFromNaviInfo(VR_XmlStrParse& xmlStrParse);
    void QueryCancelRouteFromNaviInfo();
    void QueryStartNaviFromNaviInfo();
    void QuerySetScaleLevelFromNaviInfo(const std::string& level, const VR_String& strId);
    void QuerySetShowStateFromNaviInfo(const std::string& state, const VR_String& strId);

    // response from navi
    void OnMessageNaviInfoRespPoiTime(const VR_ProtoMessageBase& msg);
    void OnMessageNaviInfoRespAddressBook(const VR_ProtoMessageBase& msg);
    void OnMessageNaviInfoRespResult(const VR_ProtoMessageBase& msg);
    void OnMessageNaviInfoRespNaviStatus(const VR_ProtoMessageBase& msg);
    void OnMessageNaviInfoRespCarPosition(const VR_ProtoMessageBase& msg);

    std::map<int, VR_String> m_resultMap;
    std::map<VR_String, int> m_PoiTimeTypeMap;
    std::map<VR_String, int> m_addrTypeMap;
    std::map<VR_String, int> m_routeOptionToEnumMap;
    std::map<int, VR_String> m_routeOptionToStringMap;
    std::map<navi::NaviInfo::NaviInfoModeType, VR_String> m_modeTypeToStringMap;
    spVR_GetCarPositionTimer                    m_spGetCarPositionTimer;
    VR_String                m_strMapStatus;
};


#endif // VR_NAVIMESSAGEHANDLER_H
/* EOF */
