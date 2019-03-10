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
 * @file VR_NaviMessageHandler.cpp
 * @brief Declaration file of class VR_NaviMessageHandler.
 *
 * This file includes the declaration of class VR_NaviMessageHandler and Derived classes.
 *
 * @attention used for C++ only.
 */


#include "VR_NaviMessageHandler.h"
#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "EV_EventSender.h"
#include "ncore/NCString.h"
#include "VR_ConfigureIF.h"
#include "VR_XmlGenerate.h"

// request
#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"
#include "navi/NaviInfo/NaviInfoBase.pb.h"
#include "navi/NaviInfo/NaviInfoCancelRoute.pb.h"
#include "navi/NaviInfo/NaviInfoPoiHistory.pb.h"
#include "navi/NaviInfo/NaviInfoPoiHome.pb.h"
#include "navi/NaviInfo/NaviInfoPoiKeywordQuery.pb.h"
#include "navi/NaviInfo/NaviInfoPoiKeywordQueryResult.pb.h"
#include "navi/NaviInfo/NaviInfoPoiNearByGivenAddress.pb.h"
#include "navi/NaviInfo/NaviInfoPoiNearByQuery.pb.h"
#include "navi/NaviInfo/NaviInfoPoiPassPointSearch.pb.h"
#include "navi/NaviInfo/NaviInfoPoiPassPointTypeSearch.pb.h"
#include "navi/NaviInfo/NaviInfoPoiTime.pb.h"
#include "navi/NaviInfo/NaviInfoSetScaleLevel.pb.h"
#include "navi/NaviInfo/NaviInfoSetShowState.pb.h"
#include "navi/NaviInfo/NaviInfoStartNavigation.pb.h"
#include "navi/NaviInfo/NaviInfoPoiAddressBook.pb.h"
#include "navi/NaviInfo/NaviInfoSetRouteOption.pb.h"
#include "navi/NaviInfo/NaviInfoGetNavigationStatus.pb.h"
#include "navi/NaviInfo/NaviInfoPoiToDest.pb.h"


// response
#include "navi/NaviInfo/NaviInfoRespPoiAddressBook.pb.h"
#include "navi/NaviInfo/NaviInfoRespPoiTime.pb.h"
#include "navi/NaviInfo/NaviInfoRespResult.pb.h"
#include "navi/NaviInfo/NaviInfoRespNavigationStatus.pb.h"
#include "navi/NaviInfo/NaviInfoRespCarPositionInfo.pb.h"
#include "navi/NaviInfo/NaviInfoGetCarPosition.pb.h"

using namespace navi::NaviInfo;
using namespace nutshell;
using namespace navi::VoiceRecog;

#define VR_GetCarPOSITION_TIME  30000 // 30s

VR_GetCarPositionTimer::VR_GetCarPositionTimer() : BL_Timer(VR_GetCarPOSITION_TIME)
{
    VR_LOGD_FUNC();
}

VR_GetCarPositionTimer::~VR_GetCarPositionTimer()
{
    VR_LOGD_FUNC();
}

VOID VR_GetCarPositionTimer::OnTimer()
{
    VR_LOGI("Enter VR_GetCarPositionTimer and Get car position");

    std::unique_ptr<NaviInfoGetCarPosition> getCarPosition(NaviInfoGetCarPosition::default_instance().New());
    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_GETCARPOSITION, getCarPosition.release());
}

VR_NaviMessageHandler::VR_NaviMessageHandler()
    : m_strMapStatus("false")
{
    VR_LOGD_FUNC();

    m_spGetCarPositionTimer = spVR_GetCarPositionTimer(VR_new VR_GetCarPositionTimer());
    if (m_spGetCarPositionTimer) {
        VR_LOGI("CarPositionTimer Start");
        m_spGetCarPositionTimer->Start();
    }

    m_resultMap[0] = VR_NAVI_RESULT_ERROR;
    m_resultMap[1] = VR_NAVI_RESULT_OK;
    m_resultMap[2] = VR_NAVI_RESULT_TIMEOUT;

    m_PoiTimeTypeMap[VR_NAVI_QUERY_DESTINATION] = 0;
    m_PoiTimeTypeMap[VR_NAVI_QUERY_POI] = 1;

    m_addrTypeMap[VR_NAVI_ADDR_LOCAL] = 0;
    m_addrTypeMap[VR_NAVI_ADDR_DESTINATION] = 1;
    m_addrTypeMap[VR_NAVI_ADDR_HOME] = 2;
    m_addrTypeMap[VR_NAVI_ADDR_OFFICE] = 3;

    m_routeOptionToEnumMap[VR_NAVI_POLICY_LEAST_DISTANCE] = 0;
    m_routeOptionToEnumMap[VR_NAVI_POLICY_LEAST_FEE] = 1;
    m_routeOptionToEnumMap[VR_NAVI_POLICY_HIGHWAY_FIRST] = 2;
    m_routeOptionToEnumMap[VR_NAVI_POLICY_RECOMMEND_ROUTE] = 3;

    // can be used by find_if
    m_routeOptionToStringMap[0] = VR_NAVI_POLICY_LEAST_DISTANCE;
    m_routeOptionToStringMap[1] = VR_NAVI_POLICY_LEAST_FEE;
    m_routeOptionToStringMap[2] = VR_NAVI_POLICY_HIGHWAY_FIRST;
    m_routeOptionToStringMap[3] = VR_NAVI_POLICY_RECOMMEND_ROUTE;

    m_modeTypeToStringMap[NaviInfoModeType_2D] = VR_PROXY_TYPE_MODE_2D;
    m_modeTypeToStringMap[NaviInfoModeType_3D] = VR_PROXY_TYPE_MODE_3D;
    m_modeTypeToStringMap[NaviInfoModeType_HeadUp] = VR_PROXY_TYPE_MODE_HEADUP;
    m_modeTypeToStringMap[NaviInfoModeType_NorthUp] = VR_PROXY_TYPE_MODE_NORTHUP;
    m_modeTypeToStringMap[NaviInfoModeType_TmcOn] = VR_PROXY_TYPE_TMC_ON;
    m_modeTypeToStringMap[NaviInfoModeType_TmcOff] = VR_PROXY_TYPE_TMC_OFF;
    m_modeTypeToStringMap[NaviInfoModeType_ZoomIn_ToMax] = VR_PROXY_TYPE_ZOOMIN_TO_MAX;
    m_modeTypeToStringMap[NaviInfoModeType_ZoomOut_ToMin] = VR_PROXY_TYPE_ZOOMOUT_TO_MIN;

}

VR_NaviMessageHandler::~VR_NaviMessageHandler()
{
    VR_LOGD_FUNC();

    if (m_spGetCarPositionTimer) {
        m_spGetCarPositionTimer->Stop();
    }
}

/**********************************Message request from natp*******************************/
void VR_NaviMessageHandler::OnRequestVriAutoProxy(const VR_String& strXml)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    VR_String strType = xmlStrParse.GetXMLValue(VR_XPATH_TYPE);
    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);

    VoiceMap<VR_String, emVriAutoProxyType>::const_iterator iter = g_iAutoRouter.find(strType);
    if (iter == g_iAutoRouter.end()) {
        VR_LOGE("Type[%s] don't exist!", strType.c_str());
        return;
    }


    // 修改点较多，原有的parser未做修改。
    // 应该传入xml，内部进行解析。或者传入数值，内部直接使用
    switch (iter->second) {
    case VriAutoProxyType_Navi_ToDestination:
    {
        RequestNaviToDestination(xmlStrParse);
        break;
    }
    case VriAutoProxyType_Navi_WukongListData:
    {
        RequestNaviToDestination(xmlStrParse);
        break;
    }
    case VriAutoProxyType_Navi_SetMode:
    {
        QuerySetModeFromNaviInfo(xmlStrParse);
        break;
    }
    case VriAutoProxyType_Navi_GetAddrLocation:
    {
        RequestAddressBookAddr(strXml);
        break;
    }
    case VriAutoProxyType_Navi_DistTimeToAddr:
    {
        RequestDisTimeToAddr(strXml);
        break;
    }
    case VriAutoProxyType_Navi_SetRouteOption:
    {
        RequestSetRouteOption(strXml);
        break;
    }
    case VriAutoProxyType_Navi_GetNaviStatus:
    {
        RequestNavigationStatus(strXml);
        break;
    }
    default:
    {
        break;
    }
    }

}

void VR_NaviMessageHandler::RequestNavigationStatus(const VR_String& strXml)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    VR_LOGI("strId[%s]", strId.c_str());
    std::unique_ptr<NaviInfoGetNavigationStatus> getNaviStatus(NaviInfoGetNavigationStatus::default_instance().New());
    getNaviStatus->mutable_reqinfo()->set_reqid(atoi(strId.c_str()));

    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_GET_NAVISTATUS, getNaviStatus.release());
}

void VR_NaviMessageHandler::RequestNaviToDestination(VR_XmlStrParse& xmlStrParse)
{
    VR_LOGD_FUNC();

    std::unique_ptr<NaviInfoPoiToDest> poiToDest(NaviInfoPoiToDest::default_instance().New());
    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    VR_String strlongitude = xmlStrParse.GetXMLValue("addr_longitude");
    VR_String strlatitude = xmlStrParse.GetXMLValue("addr_latitude");
    VR_String strType = xmlStrParse.GetXMLValue("type");

    VR_LOGI("strId : %s", strId.c_str());
    VR_LOGI("strlongitude : %s", strlongitude.c_str());
    VR_LOGI("strlatitude : %s", strlatitude.c_str());
    VR_LOGI("strType : %s", strType.c_str());
    poiToDest->mutable_reqinfo()->set_reqid(atoi(strId.c_str()));
    poiToDest->mutable_point()->set_longitude(strlongitude);
    poiToDest->mutable_point()->set_latitude(strlatitude);
    poiToDest->set_type(strType);
    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_POITODEST, poiToDest.release());
}

void VR_NaviMessageHandler::QuerySetModeFromNaviInfo(VR_XmlStrParse& xmlStrParse)
{
    VR_LOGD_FUNC();

    VR_String navi_mode = xmlStrParse.GetXMLValue("navi_mode");
    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    VR_LOGI("navi_mode : %s", navi_mode.c_str());
    if ("NAVI_MODE_CANCEL_NAVI" == navi_mode) {
        QueryCancelRouteFromNaviInfo();
    }
    else if ("NAVI_MODE_START_NAVI" == navi_mode) {
        QueryStartNaviFromNaviInfo();
    }
    else if ("NAVI_MODE_ZOOMIN" == navi_mode) {
        VR_String scalelevel = "0";
        QuerySetScaleLevelFromNaviInfo(scalelevel, strId);
    }
    else if ("NAVI_MODE_ZOOMOUT" == navi_mode) {
        VR_String scalelevel = "1";
        QuerySetScaleLevelFromNaviInfo(scalelevel, strId);
    }
    else if ("NAVI_MODE_2D" == navi_mode) {
        VR_String state = "2D";
        QuerySetShowStateFromNaviInfo(state, strId);
    }
    else if ("NAVI_MODE_3D" == navi_mode) {
        VR_String state = "3D";
        QuerySetShowStateFromNaviInfo(state, strId);
    }
}

// void VR_NaviMessageHandler::QueryGetAddrLocationFromNaviInfo(VR_XmlStrParse& xmlStrParse)
// {
//     VR_LOGD_FUNC();

//     VR_String navi_addr_type = xmlStrParse.GetXMLValue("navi_addr_type");
//     std::unique_ptr<NaviInfoPoiSavedAddress> localAddr(NaviInfoPoiSavedAddress::default_instance().New());

//     localAddr->set_issavedaddress(navi_addr_type);
//     EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_POIHOME, localAddr.release());
// }

void VR_NaviMessageHandler::QueryCancelRouteFromNaviInfo()
{
    VR_LOGD_FUNC();

    std::unique_ptr<NaviInfoCancelRoute> cancelRoute(NaviInfoCancelRoute::default_instance().New());
    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_CANCELROUTE, cancelRoute.release());
}

void VR_NaviMessageHandler::QueryStartNaviFromNaviInfo()
{
    VR_LOGD_FUNC();

    std::unique_ptr<NaviInfoStartNavigation> startNavi(NaviInfoStartNavigation::default_instance().New());
    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_STARTNAVIGATION, startNavi.release());
}

void VR_NaviMessageHandler::QuerySetScaleLevelFromNaviInfo(const std::string& level, const VR_String& strId)
{
    VR_LOGD_FUNC();

    std::unique_ptr<NaviInfoSetScaleLevel> setScaleLevel(NaviInfoSetScaleLevel::default_instance().New());
    setScaleLevel->mutable_reqinfo()->set_reqid(atoi(strId.c_str()));
    setScaleLevel->set_scalelevel(level);
    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_SETSCALELEVEL, setScaleLevel.release());
}

void VR_NaviMessageHandler::QuerySetShowStateFromNaviInfo(const std::string& state, const VR_String& strId)
{
    VR_LOGD_FUNC();

    std::unique_ptr<NaviInfoSetShowState> setShowState(NaviInfoSetShowState::default_instance().New());
    setShowState->set_showstate(state);
    setShowState->mutable_reqinfo()->set_reqid(atoi(strId.c_str()));
    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_SETSHOWSTATE, setShowState.release());
}

void
VR_NaviMessageHandler::RequestAddressBookAddr(const VR_String& strXml)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    VR_String strAddrType = xmlStrParse.GetXMLValue(VR_NAVI_ADDR_TYPE);

    VR_LOGD("addrType: %s", strAddrType.c_str());

    std::unique_ptr<NaviInfoPoiAddressBook> reqAddressBook(NaviInfoPoiAddressBook::default_instance().New());
    reqAddressBook->mutable_reqinfo()->set_reqid(atoi(strId.c_str()));
    reqAddressBook->set_type(static_cast<NaviInfoPoiAddressBook_AddrType> (m_addrTypeMap[strAddrType]));
    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_POIADDRESSBOOK, reqAddressBook.release());

}

void VR_NaviMessageHandler::RequestDisTimeToAddr(const VR_String& strXml)
{

    VR_LOGD_FUNC();

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }


    VR_String queryType = xmlStrParse.GetXMLValue(VR_NAVI_QUERY_TYPE);
    VR_LOGD("wukong queryType is [%s]", queryType.c_str());
    // if (VR_NAVI_QUERY_DESTINATION == queryType) {
    //     return;
    // }

    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    VR_String addr_name = xmlStrParse.GetXMLValue(VR_ADDR_NAME);
    VR_String strLongitude = xmlStrParse.GetXMLValue(VR_ADDR_LONGITUDE);
    VR_String strLatitude = xmlStrParse.GetXMLValue(VR_ADDR_LATITUDE);

    std::unique_ptr<NaviInfoPoiTime> reqPoiTime(NaviInfoPoiTime::default_instance().New());
    reqPoiTime->mutable_reqinfo()->set_reqid(atoi(strId.c_str()));
    reqPoiTime->set_type(static_cast<NaviInfoPoiTime_QueryType> (m_PoiTimeTypeMap[queryType]));
    reqPoiTime->set_name(addr_name);
    reqPoiTime->set_longitude(strLongitude);
    reqPoiTime->set_latitude(strLatitude);

    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_POITIME, reqPoiTime.release());

}

void VR_NaviMessageHandler::RequestSetRouteOption(const VR_String& strXml)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    VR_String strId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    VR_String strRouteOption = xmlStrParse.GetXMLValue(VR_NAVI_SET_ROUTE_OPTION);
    VR_String strRouteFlag = xmlStrParse.GetXMLValue(VR_NAVI_SET_ROUTE_FLAG);
    bool bFlag = strRouteFlag == "true" ? true : false;

    std::unique_ptr<NaviInfoSetRouteOption> reqRouteOption(NaviInfoSetRouteOption::default_instance().New());
    reqRouteOption->mutable_reqinfo()->set_reqid(atoi(strId.c_str()));
    reqRouteOption->set_type(static_cast<NaviInfoRouteOptionType> (m_routeOptionToEnumMap[strRouteOption]));
    reqRouteOption->set_defaultflag(bFlag);

    EV_EventSender().SendEvent(EV_EVENT_NAVIINFO_SET_ROUTEOPTION, reqRouteOption.release());

}

/*********************************Message response from Navi*******************************/
void VR_NaviMessageHandler::OnMessageNaviResp(const VR_ProtoMessageBase &msg)
{
    if (msg.GetTypeName() == "navi.NaviInfo.NaviInfoRespPoiTime") {
        OnMessageNaviInfoRespPoiTime(msg);
    }
    else if (msg.GetTypeName() == "navi.NaviInfo.NaviInfoRespPoiAddressBook") {
        OnMessageNaviInfoRespAddressBook(msg);
    }
    else if (msg.GetTypeName() == "navi.NaviInfo.NaviInfoRespResult") {
        OnMessageNaviInfoRespResult(msg);
    }
    else if (msg.GetTypeName() == "navi.NaviInfo.NaviInfoRespNavigationStatus") {
        OnMessageNaviInfoRespNaviStatus(msg);
    }
    else if (msg.GetTypeName() == "navi.NaviInfo.NaviInfoRespCarPositionInfo") {
        VR_LOGI("it is car positon info");
        OnMessageNaviInfoRespCarPosition(msg);
    }
    else {
        VR_LOGE("invalid type!");
    }

}

void VR_NaviMessageHandler::OnMessageNaviInfoRespNaviStatus(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();

    const NaviInfoRespNavigationStatus& respNaviStatus = static_cast<const NaviInfoRespNavigationStatus&> (msg);
    int actionId = respNaviStatus.reqinfo().reqid();
    VR_String strActionId = std::to_string(actionId);
    NaviInfoResult result = respNaviStatus.result();
    // bool bMapOn = respNaviStatus.bmapon();
    bool bNaviStart = respNaviStatus.bnavistart();
    VR_String strMapOn = m_strMapStatus;
    VR_String strNaviStart = (bNaviStart ? "true" : "false");
    NaviInfoRouteOptionType routeOption = respNaviStatus.routeoption();
    std::vector<VR_String> vecMode;

    VR_LOGI("m_strMapStatus : %s", m_strMapStatus.c_str());
    int modeNum = respNaviStatus.modetype_size();
    for (int i = 0; i < modeNum; ++i) {
        vecMode.push_back(m_modeTypeToStringMap[respNaviStatus.modetype(i)]);
    }

    VR_String respNaviStatusXml;
    if (!VR_XmlGenerate::GetRespResultToNATPXml(respNaviStatusXml, strActionId, m_resultMap[(int)result], strMapOn, strNaviStart, m_routeOptionToStringMap[routeOption], vecMode)) {
        VR_LOGE("generate xml failed!");
        return;
    }

    SendReqToNATP(respNaviStatusXml);
}

void VR_NaviMessageHandler::OnMessageNaviInfoRespResult(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();

    const NaviInfoRespResult& respResult = static_cast<const NaviInfoRespResult&> (msg);
    int actionId = respResult.respinfo().reqid();
    VR_String strActionId = std::to_string(actionId);
    std::string strActionType = respResult.type();
    NaviInfoResult result = respResult.result();

    VR_String respResultXml;
    if (!VR_XmlGenerate::GetRespResultToNATPXml(respResultXml, strActionId, strActionType, m_resultMap[(int)result])) {
        VR_LOGE("generate xml failed!");
        return;
    }

    SendReqToNATP(respResultXml);
}


void VR_NaviMessageHandler::OnMessageNaviInfoRespPoiTime(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();

    const NaviInfoRespPoiTime& respPoiTime = static_cast<const NaviInfoRespPoiTime&> (msg);
    int actionId = respPoiTime.respinfo().reqid();
    NaviInfoResult result = respPoiTime.result();
    int distance = respPoiTime.distance();
    int hourTime = respPoiTime.time_to_dst_hour();
    int minTime = respPoiTime.time_to_dst_min();
    NaviInfoRespPoiTime_QueryType type = respPoiTime.type();


    VR_String respPoiTimeXml;
    if (!VR_XmlGenerate::GetRespPoiTimeToNATPXml(respPoiTimeXml, actionId, m_resultMap[(int)result], std::to_string(distance), std::to_string(hourTime), std::to_string(minTime))) {
        VR_LOGE("generate xml failed!");
        return;
    }

    SendReqToNATP(respPoiTimeXml);

}

void VR_NaviMessageHandler::OnMessageNaviInfoRespAddressBook(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();

    const NaviInfoRespPoiAddressBook& respAddrBook = static_cast<const NaviInfoRespPoiAddressBook&> (msg);

    std::string strName = "";
    std::string strLongitude = "";
    std::string strLatitude = "";
    NaviInfoResult result = respAddrBook.result();
    int actionId = respAddrBook.respinfo().reqid();

    if (NaviInfo_Result_OK == result) {
        if (respAddrBook.has_name()) {
            strName = respAddrBook.name();
        }

        strLongitude = respAddrBook.longitude();
        strLatitude = respAddrBook.latitude();
    }

    VR_String respAddrBookXml;
    if (!VR_XmlGenerate::GetRespAddrBookToNATPXml(respAddrBookXml, actionId, m_resultMap[(int)result], strName, strLongitude, strLatitude)) {
        VR_LOGE("generate xml failed!");
        return;
    }

    SendReqToNATP(respAddrBookXml);
}

void VR_NaviMessageHandler::OnMessageNaviInfoRespCarPosition(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();

    const NaviInfoRespCarPositionInfo& respCarPosition = static_cast<const NaviInfoRespCarPositionInfo&> (msg);

    std::string strProvinceName = respCarPosition.province();
    std::string strCityName = respCarPosition.city();
    std::string strLongitude = respCarPosition.longitude();
    std::string strLatitude = respCarPosition.latitude();

    VR_String respCarPositionInfoXml;
    if (!VR_XmlGenerate::GetRespCarPositionInfoToNATPXml(respCarPositionInfoXml, strProvinceName, strCityName, strLongitude, strLatitude)) {
        VR_LOGE("generate xml failed!");
        return;
    }

    SendReqToNATP(respCarPositionInfoXml);
}

// SendReqToNATP
VR_UINT32 VR_NaviMessageHandler::GetActionId()
{
    VR_IdGenerator nId;
    return nId.GetID();
}

void VR_NaviMessageHandler::SendReqToNATP(VR_String &strXml)
{
    VR_LOGD_FUNC();
    VR_LOGD("strXml=[%s]", strXml.c_str());
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(strXml);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

void VR_NaviMessageHandler::SetMapStatus(const VR_String& strMapStatus)
{
    VR_LOGD_FUNC();

    VR_LOGD("strMapStatus=[%s]", strMapStatus.c_str());
    m_strMapStatus = strMapStatus;
}
/* EOF */

