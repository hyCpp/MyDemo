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
 * @file VR_NaviStatusManager.cpp
 * @brief Declaration file of class VR_NaviStatusManager.
 *
 * This file includes the declaration of class VR_NaviStatusManager and Derived classes.
 *
 * @attention used for C++ only.
 */




#include <boost/assign/list_of.hpp>
#include "VR_NaviStatusManager.h"
#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "EV_EventSender.h"
#include "ncore/NCString.h"
#include "VR_ConfigureIF.h"
#include "DSPDataIF.h"
#include "VR_ContentProvider.h"

#include "navi/NaviMain/NaviStatus.pb.h"
#include "navi/Guide/GudGuideStart.pb.h"
#include "navi/Guide/GudGuideAbortFinish.pb.h"
#include "navi/Path/RouteDetailInfo.pb.h"
#include "navi/Path/PathDef.pb.h"
#include "navi/Path/IVIGuideRouteInfoUpdate.pb.h"
#include "navi/Map/MapInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"
#include "navi/Guide/GudETAListInfo.pb.h"
#include "navi/Point/PointRequest.pb.h"
#include "navi/Point/PointResponse.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Path/BackGroundRouteInfoUpdate.pb.h"
#include "navi/Search/ReqCategorySearchVR.pb.h"
#include "navi/Search/RespCategorySearchVRResult.pb.h"
#include "navi/Location/LocInfoUpdate.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::Location;
using namespace nutshell;

#define CONVERSIONRATIO_D  (60*60*256)
#define INVAKLID_VALUE     0xFFFFFFFF

static const VoiceMap<VR_String, VR_INT32>::type g_strRouteOptionMap = boost::assign::map_list_of
        ("NAVI_POLICY_LEAST_DISTANCE", 0)
        ("NAVI_POLICY_LEAST_FEE", 0)
        ("NAVI_POLICY_HIGHWAY_FIRST", 0)
        ("NAVI_POLICY_RECOMMEND_ROUTE", 0)
        ("NAVI_POLICY_OTHER_ROUTE", 0);

VR_NaviStatusManager::VR_NaviStatusManager()
    : m_sMapStatus("false")
    , m_sNaviStatus("false")
    , m_sRouteOption("NAVI_POLICY_RECOMMEND_ROUTE")
    , m_sMapDirModeFirst("NAVI_MODE_2D")
    , m_sMapDirModeSecond("NAVI_MODE_NORTHUP")
    , m_sMapScaleMode("")
    , m_sDistToDest("")
    , m_sDestName("")
    , m_sAddrName("")
    , m_sDestLongit(INVAKLID_VALUE)
    , m_sDestlatit(INVAKLID_VALUE)
    , m_sEstTimeH("")
    , m_sEstTimeM("")
    , m_strPOIId("")
    , m_strPOIName("")
    , m_strPOILongit("")
    , m_strPOILatit("")
    , m_strCategyId("")
    , m_strCategyName("")
    , m_strSearchMethod("")
    , m_emRounteType(VR_NAVI_INVALID)
    , m_nCursorLon(INVAKLID_VALUE)
    , m_nCursorLat(INVAKLID_VALUE)
{
    VR_LOGD_FUNC();
}

VR_NaviStatusManager::~VR_NaviStatusManager()
{
    VR_LOGD_FUNC();
}

void VR_NaviStatusManager::OnMessageNaviStatusUpdate(const VR_ProtoMessageBase &msg)
{
    if (msg.GetTypeName() == "navi.NaviMain.NaviStatus") {
        return;
//        const navi::NaviMain::NaviStatus& naviStatus = (const navi::NaviMain::NaviStatus&)msg;
//        if (naviStatus.has_enavistartstatus()) {
//            navi::NaviMain::NaviStatus_StartStatus naviStartStatus = naviStatus.enavistartstatus();
//            VR_INT32 nStartFlag = navi::NaviMain::NaviStatus_StartStatus_StartFlag_FLG_STARTCOMPLETED;
//            VR_INT32 nErrorFlag = navi::NaviMain::NaviStatus_StartStatus_ErrorFlag_ERRFLG_ALLMODULE_STARTED;
//            VR_INT32 nStartReason = navi::NaviMain::NaviStatus_Reason_NORMAL;

//            if (naviStartStatus.has_estartflag()) {
//                nStartFlag = naviStartStatus.estartflag();
//            }
//            if (naviStartStatus.has_eerrorflag()) {
//                nErrorFlag = naviStartStatus.eerrorflag();
//            }
//            if (naviStartStatus.has_estartreason()) {
//                nStartReason = naviStartStatus.estartreason();
//            }
//            VR_String strMapstatus;
//            if ((navi::NaviMain::NaviStatus_StartStatus_StartFlag_FLG_STARTCOMPLETED == nStartFlag)
//                && (navi::NaviMain::NaviStatus_StartStatus_ErrorFlag_ERRFLG_ALLMODULE_STARTED == nErrorFlag)
//                && (navi::NaviMain::NaviStatus_Reason_NORMAL == nStartReason)) {
//                strMapstatus = "true";
//            }
//            else {
//                strMapstatus = "false";
//            }
//            if (strMapstatus != m_sMapStatus) {
//                m_sMapStatus = strMapstatus;
//                VR_String notifyXML;
//                if (GetNavigationStatusNotifyXML(notifyXML)) {
//                    SendReqToNATP(notifyXML);
//                }
//            }
//        }
    }
    else if (msg.GetTypeName() == "navi.Guide.GudGuideStart") {
        VR_String sGuideStart = "true";
        VR_LOGD("m_sNaviStatus=[%s]", m_sNaviStatus.c_str());
        if (sGuideStart != m_sNaviStatus) {
            m_sNaviStatus = sGuideStart;
            VR_String notifyXML;
            if (GetNavigationStatusNotifyXML(notifyXML)) {
                SendReqToNATP(notifyXML);
            }
        }
    }
//    else if (msg.GetTypeName() == "navi.Path.IVIGuideRouteInfoUpdate") {
//        const navi::Path::IVIGuideRouteInfoUpdate&  routeInfoUpdate = (const navi::Path::IVIGuideRouteInfoUpdate&)msg;

//        navi::Path::RouteDetailInfo routeDetailInfo;
//        if (routeInfoUpdate.has_route_detail()) {
//            routeDetailInfo = routeInfoUpdate.route_detail();
//        }
//        else {
//            return;
//        }

//        VR_INT32 nRoute_weight = navi::Path::PathDef_PathFindWeight_PATH_FIND_DEFAULT;
//        if (routeDetailInfo.has_route_weight()) {
//            nRoute_weight = routeDetailInfo.route_weight();
//        }
//        else {
//            return;
//        }
//        VR_String sRouteOption;
//        switch (nRoute_weight) {
//        case navi::Path::PathDef_PathFindWeight_PATH_FIND_DEFAULT :
//        {
//            sRouteOption = "NAVI_POLICY_RECOMMEND_ROUTE";
//        }
//            break;
//        case navi::Path::PathDef_PathFindWeight_PATH_FIND_SHORTEST :
//        {
//            sRouteOption = "NAVI_POLICY_LEAST_DISTANCE";
//        }
//            break;
//        case navi::Path::PathDef_PathFindWeight_PATH_FIND_AVOIDHIGHWAY :
//        case navi::Path::PathDef_PathFindWeight_PATH_FIND_ECO :
//        {
//            sRouteOption = "NAVI_POLICY_LEAST_FEE";
//        }
//            break;
//        case navi::Path::PathDef_PathFindWeight_PATH_FIND_WIDTH :
//        {
//            sRouteOption = "NAVI_POLICY_HIGHWAY_FIRST";
//        }
//            break;
//        case navi::Path::PathDef_PathFindWeight_PATH_FIND_ALTERNATIVE :
//        {
//            sRouteOption = "NAVI_POLICY_HIGHWAY_FIRST";
//        }
//            break;
//        default:
//        {
//            VR_LOGD("nRoute_weight=[%d]", nRoute_weight);
//            break;
//        }
//        }
//        if (sRouteOption != m_sRouteOption) {
//            m_sRouteOption = sRouteOption;
//            VR_String notifyXML;
//            if (GetNavigationStatusNotifyXML(notifyXML)) {
//                SendReqToNATP(notifyXML);
//            }
//        }
//    }
    else if (msg.GetTypeName() == "navi.Map.MapInfo") {
        const navi::Map::MapInfo& mapInfo = (const navi::Map::MapInfo&)msg;
        VR_INT32 nMapDirMode = navi::Map::MapInfo_MapDirMode_North_Up;

        VR_String sMapDirModeFirst;
        VR_String sMapDirModeSecond;
        VR_INT32 nMapScaleMode = navi::Map::MapDefine_Scale_MAP_SCALE_INVALID;
        VR_String sScaleMode = "";
        if (!mapInfo.has_map_view_id()) {
            VR_LOGD("mapInfo has no map_view_id");
            return;
        }
        navi::Map::MapDefine_ViewID nViewID = mapInfo.map_view_id();
        if (navi::Map::MapDefine_ViewID_MAIN_VIEW != nViewID) {
            VR_LOGD("MapDefine_ViewID_MAIN_VIEW != nViewID");
            return;
        }
        if (mapInfo.has_map_lonlat()) {
            m_nCursorLon = mapInfo.map_lonlat().lon();
            m_nCursorLat = mapInfo.map_lonlat().lat();
        }
        if (mapInfo.has_map_dir()) {
            nMapDirMode = mapInfo.map_dir_mode();
            if (navi::Map::MapInfo_MapDirMode_North_Up == nMapDirMode) {
                sMapDirModeSecond = "NAVI_MODE_NORTHUP";
                sMapDirModeFirst = "NAVI_MODE_2D";
            }
            else if (navi::Map::MapInfo_MapDirMode_Heading_Up == nMapDirMode) {
                sMapDirModeSecond = "NAVI_MODE_HEADUP";
                sMapDirModeFirst = "NAVI_MODE_2D";
            }
            else if (navi::Map::MapInfo_MapDirMode_Sky_View == nMapDirMode) {
                sMapDirModeSecond = "";
                sMapDirModeFirst = "NAVI_MODE_3D";
            }
            if ((sMapDirModeSecond != m_sMapDirModeSecond) || (sMapDirModeFirst != m_sMapDirModeFirst)) {
                m_sMapDirModeSecond = sMapDirModeSecond;
                m_sMapDirModeFirst = sMapDirModeFirst;
                VR_String notifyXML;
                if (GetNavigationStatusNotifyXML(notifyXML)) {
                    SendReqToNATP(notifyXML);
                }
            }
        }
        if (mapInfo.has_map_scale()) {
            nMapScaleMode = mapInfo.map_scale();
            if (navi::Map::MapDefine_Scale_MAP_SCALE_1_2048W == nMapScaleMode) {
                sScaleMode = "NAVI_MODE_ZOOMOUT_TO_MIN";
                if (m_sMapScaleMode != sScaleMode) {
                    m_sMapScaleMode = sScaleMode;
                    VR_String notifyXML;
                    if (GetNavigationStatusNotifyXML(notifyXML)) {
                        SendReqToNATP(notifyXML);
                    }
                }
            }
            else if (navi::Map::MapDefine_Scale_MAP_SCALE_1_5000 == nMapScaleMode) {
                sScaleMode = "NAVI_MODE_ZOOMIN_TO_MAX";
                if (m_sMapScaleMode != sScaleMode) {
                    m_sMapScaleMode = sScaleMode;
                    VR_String notifyXML;
                    if (GetNavigationStatusNotifyXML(notifyXML)) {
                        SendReqToNATP(notifyXML);
                    }
                }
            }
            else {
                sScaleMode = "";
                if (m_sMapScaleMode != sScaleMode) {
                    m_sMapScaleMode = sScaleMode;
                    VR_String notifyXML;
                    if (GetNavigationStatusNotifyXML(notifyXML)) {
                        SendReqToNATP(notifyXML);
                    }
                }
            }

        }
    }
    else if (msg.GetTypeName() == "navi.Guide.GudETAListInfo") {
        const navi::Guide::GudETAListInfo& gudETAListInfo = (const navi::Guide::GudETAListInfo&)msg;
        VR_INT32 listSize = gudETAListInfo.m_cetainfolist_size();

        navi::Guide::GudETAListInfo_GudETAInfo  gudETAInfo;
        INT64 nEstTime;
        if (listSize > 0) {
            gudETAInfo = gudETAListInfo.m_cetainfolist(--listSize);
            if (gudETAInfo.has_m_lesttime()) {
                nEstTime = gudETAInfo.m_lesttime();
                m_sEstTimeH = std::to_string(nEstTime / 60);
                m_sEstTimeM = std::to_string(nEstTime % 60);
            }
            if (gudETAInfo.has_m_lalongroaddist()) {
                m_sDistToDest = std::to_string(gudETAInfo.m_lalongroaddist());
            }
            if (gudETAInfo.has_m_strdestinationname()) {
                m_sDestName = gudETAInfo.m_strdestinationname();
            }
            if (gudETAInfo.has_m_ilongitude()) {
                m_sDestLongit = gudETAInfo.m_ilongitude();
            }
            if (gudETAInfo.has_m_ilatitude()) {
                m_sDestlatit = gudETAInfo.m_ilatitude();
            }
            if (gudETAInfo.has_m_straddressname()) {
                m_sAddrName = gudETAInfo.m_straddressname();
            }
        }
        else {
            return;
        }
    }
    else if (msg.GetTypeName() == "navi.Point.PointResponse") {
        const navi::Point::PointResponse& resultproto = (const navi::Point::PointResponse&)msg;

        VR_String strId = "";
        VR_String strError = VR_NAVI_RESULT_ERROR;
        VR_String strName = "";
        VR_String strlongitude = "";
        VR_String strlatitude = "";
        if (resultproto.respinfo().result() == navi::Point::POINT_REQ_RESULT_SUCCESS && resultproto.has_respgetdetail()) {
            const navi::Point::RespGetDetailInfo &respGetDtlInfo = resultproto.respgetdetail();
            const navi::Point::DetailInfoDisplay &dtlInfoDisp = respGetDtlInfo.detailinfo();
            const navi::Point::PointRecord &pointRec = dtlInfoDisp.recinfo();
            if (pointRec.has_dispname()) {
                strName = pointRec.dispname();
            }
            if (pointRec.has_longitude()) {
                nutshell::NCString longitude;
                longitude.format("%.4f", static_cast<double>(pointRec.longitude()) / CONVERSIONRATIO_D);
                strlongitude = longitude.getString();
            }
            if (pointRec.has_latitude()) {
                nutshell::NCString latitude;
                latitude.format("%.4f", static_cast<double>(pointRec.latitude()) / CONVERSIONRATIO_D);
                strlatitude = latitude.getString();
            }
            strError = VR_NAVI_RESULT_OK;

        }
        if (resultproto.respinfo().has_requestid()) {
            strId = std::to_string(resultproto.respinfo().requestid());
        }
        VR_String strMsg;
        if (GetNaviAddrLocationResponseXML(strId, strError, strName, strlongitude, strlatitude, strMsg)) {
            SendReqToNATP(strMsg);
        }
    }
    else if (msg.GetTypeName() == "navi.Path.BackGroundRouteInfoUpdate") {
        VR_LOGD("msg.type is navi.Path.BackGroundRouteInfoUpdate");
        const navi::Path::BackGroundRouteInfoUpdate& resultproto = (const navi::Path::BackGroundRouteInfoUpdate&)msg;
        if (resultproto.has_error_code()) {
            VR_LOGD("resultproto.error_code() is [%d]", resultproto.error_code());
        }
        if (VR_NAVI_POI_TEMP_ROUNT == m_emRounteType) {
            navi::Path::RouteDetailInfo routeDetailInfo;
            if (resultproto.has_route_detail()) {
                routeDetailInfo = resultproto.route_detail();
                ReplyNaviTimeToPOITOVR(routeDetailInfo);
            }
            else {
                VR_LOGD("resultproto don't has route_detail");
                return;
            }
        }
        else if (VR_NAVI_CATEGORY_ROUNT == m_emRounteType) {
            VR_LOGD("m_emRounteType is VR_NAVI_CATEGORY_ROUNT");
            m_emRounteType = VR_NAVI_INVALID;
            std::unique_ptr<navi::Search::ReqCategorySearchVR> categorySearch(navi::Search::ReqCategorySearchVR::default_instance().New());
            if (!resultproto.has_route_detail()) {
                VR_LOGD("resultproto don't has route_detail");
                return;
            }
            navi::Path::BackGroundRouteInfoUpdate* pRouteInfoUpdate = categorySearch->mutable_routeinfo();
            pRouteInfoUpdate->CheckTypeAndMergeFrom(resultproto);
            categorySearch->set_senderid(atoi(m_strCategyId.c_str()));
            categorySearch->add_genrename(m_strCategyName);
            categorySearch->set_count(25);
            if (VR_NAVI_SEARCH_ALONG_ROUTE == m_strSearchMethod) {
                categorySearch->set_searchtype(navi::Search::ReqCategorySearchVR_SEARCH_TYPE_ROUTESIDE);
            }
            else {
                VR_LOGD("m_strSearchMethod is [%s]", m_strSearchMethod.c_str());
                return;
            }

            nutshell::INT32  nLon(0);
            nutshell::INT32  nLat(0);
            nutshell::INT32  nRoadKind(0);
            bool  bDemonOn(false);
            VR_ConfigureIF::Instance()->getLocateInfo(nRoadKind, nLon, nLat, bDemonOn);
            navi::Search::SearchDefine_MapLonLat* mapLonLat = categorySearch->mutable_map_lonlat();
            mapLonLat->set_lon(nLon);
            mapLonLat->set_lat(nLat);
            EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQCATEGORYSEARCHVR, categorySearch.release());
        }
    }
    else if (msg.GetTypeName() == "navi.Guide.GudGuideAbortFinish") {
        const navi::Guide::GudGuideAbortFinish& guideAbortFinish = (const navi::Guide::GudGuideAbortFinish&)msg;
        VR_INT32 nAbort_reason = navi::Guide::GudGuideAbortFinish_GUD_ABORT_REASON_GUD_ABORT_ARRIVE_DESTINATION;
        VR_INT32 nReroute_type = navi::Guide::GudGuideAbortFinish_GUD_REROUTE_TYPE_GUD_NOT_REROUTE;
        if (guideAbortFinish.has_reroute_type_()) {
            nReroute_type = guideAbortFinish.reroute_type_();
        }
        if (guideAbortFinish.has_abort_reason_()) {
            nAbort_reason = guideAbortFinish.abort_reason_();
        }
        VR_String sGuideStart = m_sNaviStatus;
        switch (nAbort_reason) {
        case navi::Guide::GudGuideAbortFinish_GUD_ABORT_REASON_GUD_ABORT_DEMO_ARRIVE:
            break;
        default:
            sGuideStart = "false";
            break;
        }
        VR_LOGD("nReroute_type=[%d],nAbort_reason=[%d]", nReroute_type, nAbort_reason);

        if (sGuideStart != m_sNaviStatus) {
            m_sNaviStatus = sGuideStart;
            VR_String notifyXML;
            if (GetNavigationStatusNotifyXML(notifyXML)) {
                SendReqToNATP(notifyXML);
            }
        }
    }
}

void VR_NaviStatusManager::replyNaviStatusToVR(VR_String &strId)
{
    VR_String strXml = "";
    if (GetNavigationStatusResponseXML(strId, strXml)) {
        SendReqToNATP(strXml);
    }
}

void VR_NaviStatusManager::OnMessageNaviLocInfoUpdate()
{
    const VR_INT32 nProvince = 1;
    const VR_INT32 nCity     = 2;
    const VR_INT32 nDistrict = 8;
    VR_String strProvince;
    VR_String strCity;

    DSPDataIF* m_DSPDataIF = DSPDataIF::Instance();
    if (nullptr != m_DSPDataIF) {
        StdVector<DSPAdminName>::type a_AdminName;
        nutshell::INT32  nLon(0);
        nutshell::INT32  nLat(0);
        nutshell::INT32  nRoadKind(0);
        bool  bDemonOn(false);
        VR_ConfigureIF::Instance()->getLocateInfo(nRoadKind, nLon, nLat, bDemonOn);

        nutshell::NCString strlongitude;
        nutshell::NCString strlatitude;
        strlongitude.format("%.4f", static_cast<double>(nLon) / CONVERSIONRATIO_D);
        strlatitude.format("%.4f", static_cast<double>(nLat) / CONVERSIONRATIO_D);

        DSPLongPoint cCurPoint;
        cCurPoint.Lat = nLat;
        cCurPoint.Lon = nLon;
        DSP_ERROR emErrCode = m_DSPDataIF->GetAdminName(cCurPoint, DSP_LANG_DEFAULT, a_AdminName, DSP_RM_SEARCH);
        if (DSP_ERR_NO_ERROR != emErrCode) {
            VR_LOGD("DSP_ERROR=[%d]", emErrCode);
            return;
        }

        VR_XmlStrParse xmlStrParse;
        if (!xmlStrParse.LoadString(VR_NAVI_LOACL_ADDR_NOTIFY_XML)) {
            return;
        }

        for (DWORD  i = 0; i < a_AdminName.size(); ++i) {
            DSPAdminName cAdminName = a_AdminName[i];
            if (nProvince == cAdminName.AdminOrder) {
                strProvince = cAdminName.AdminName.GetString();
            }
            else if (nCity == cAdminName.AdminOrder) {
                strCity = cAdminName.AdminName.GetString();
            }
            else if (nDistrict == cAdminName.AdminOrder) {
                if (strCity.empty()) {
                    strCity = cAdminName.AdminName.GetString();
                }
            }
        }
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_NAVI_PROVINCE, strProvince);
        xmlStrParse.setTextTo(VR_NAVI_CITY, strCity);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, strlongitude.getString());
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, strlatitude.getString());
        VR_String strXml = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strXml);
    }
}

bool VR_NaviStatusManager::GetNavigationStatusNotifyXML(VR_String& strMsg)
{
    VR_String strXml = VR_NAVI_STATUS_NOTIFY_XML;
    bool getResult = false;
    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        std::vector<VR_String> mapOption;
        mapOption.push_back(m_sMapDirModeFirst);
        if (!m_sMapDirModeSecond.empty()) {
            mapOption.push_back(m_sMapDirModeSecond);
        }
        mapOption.push_back(m_sMapScaleMode);

        xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_NAVI_MAP_STATUS, m_sMapStatus);
        xmlStrParse.setTextTo(VR_NAVI_START_STAUS, m_sNaviStatus);
        xmlStrParse.setTextTo(VR_NAVI_ROUTE_OPTION, m_sRouteOption);
        xmlStrParse.setTextListTo(VR_NAVI_MAP_OPTION, mapOption);

        strMsg = xmlStrParse.TransferXMLtoString();
        getResult = true;
    }
    else {
        strMsg = "";
        getResult = false;
    }
    return getResult;
}

bool VR_NaviStatusManager::GetNavigationStatusResponseXML(const VR_String& strId, VR_String& strMsg)
{
    VR_String strXml = VR_NAVI_STATUS_RESP_XML;
    bool getResult = false;
    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        std::vector<VR_String> mapOption;
        mapOption.push_back(m_sMapDirModeFirst);
        if (!m_sMapDirModeSecond.empty()) {
            mapOption.push_back(m_sMapDirModeSecond);
        }
        mapOption.push_back(m_sMapScaleMode);

        xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, strId);
        xmlStrParse.setTextTo(VR_NAVI_MAP_STATUS, m_sMapStatus);
        xmlStrParse.setTextTo(VR_NAVI_START_STAUS, m_sNaviStatus);
        xmlStrParse.setTextTo(VR_NAVI_ROUTE_OPTION, m_sRouteOption);
        xmlStrParse.setTextListTo(VR_NAVI_MAP_OPTION, mapOption);

        strMsg = xmlStrParse.TransferXMLtoString();
        getResult = true;
    }
    else {
        strMsg = "";
        getResult = false;
    }
    return getResult;
}

bool VR_NaviStatusManager::GetNaviAddrLocationResponseXML(const VR_String& strId, const VR_String& strError, const VR_String& strName, const VR_String& strlongitude, const VR_String& strlatitude, VR_String& strMsg)
{
    VR_String strXml = VR_NAVI_GET_ADDR_LOCAT;

    bool getResult = false;
    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, strId);
        xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, strError);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_NAME, strName);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, strlongitude);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, strlatitude);

        strMsg = xmlStrParse.TransferXMLtoString();
        getResult = true;
    }
    else {
        strMsg = "";
        getResult = false;
    }
    return getResult;
}

void VR_NaviStatusManager::QueryNaviAddrLocation(VR_String &strId, VR_String &addrType)
{
    std::unique_ptr<navi::Point::PointRequest> proto;
    proto.reset(navi::Point::PointRequest::default_instance().New());

    navi::Point::PointBasicReqInfo *reqInfo = proto->mutable_reqinfo();
    reqInfo->set_reqtype(navi::Point::POINT_REQ_TYPE_GET_RECORD_DETAIL);
    reqInfo->set_requestid(atoi(strId.c_str()));
    navi::Point::ReqGetDetailInfo *reqGetDtlList = proto->mutable_reqgetdetail();
    VR_LOGD("QueryNaviAddrLocation Type is [%s]", addrType.c_str());
    if (addrType.compare(VR_NAVI_ADDR_HOME) == 0) {
        reqGetDtlList->set_recordtype(navi::Point::POINT_RECORD_TYPE_HOME);
    }
    else if (addrType.compare(VR_NAVI_ADDR_OFFICE) == 0) {
        reqGetDtlList->set_recordtype(navi::Point::POINT_RECORD_TYPE_PRESET1);
    }
    else if (addrType.compare(VR_NAVI_ADDR_DESTINATION) == 0) {
        replyDestinationAddrToVR(strId);
        return;
    }
    else if (addrType.compare(VR_NAVI_ADDR_LOCAL) == 0) {
        replyLocalAddrLocationToVR(strId);
        return;
    }
    else if (addrType.compare(VR_NAVI_ADDR_CURSOR) == 0) {
        replyCursorLocationToVR(strId);
        return;
    }
    else {
        return;
    }


    EV_EventSender().SendEvent(EV_EVENT_POINT_POINTREQUEST, proto.release());
}

void VR_NaviStatusManager::ReplyNaviTimeToAddrToVR(VR_String &strId)
{
    VR_String strXml = VR_NAVI_DICT_TIME_TO_ADDR;
    VR_XmlStrParse xmlStrParse;
    VR_String strError = VR_NAVI_RESULT_OK;
    if ((m_sDistToDest.compare("") == 0) && ((m_sEstTimeH.compare("") == 0) || (m_sEstTimeM.compare("") == 0))) {
        strError = VR_NAVI_RESULT_ERROR;
    }
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, strError);
        xmlStrParse.setTextTo(VR_NAVI_DIST_TO_DEST, m_sDistToDest);
        xmlStrParse.setTextTo(VR_NAVI_TIME_TO_DST_H, m_sEstTimeH);
        xmlStrParse.setTextTo(VR_NAVI_TIME_TO_DST_M, m_sEstTimeM);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_NaviStatusManager::ReplyDestTimeToAddrErrorToVR(VR_String &strId)
{
    VR_String strXml = VR_NAVI_DICT_TIME_TO_ADDR;
    VR_XmlStrParse xmlStrParse;
    VR_String strError = VR_NAVI_RESULT_ERROR;

    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, strId);
        xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, strError);
        xmlStrParse.setTextTo(VR_NAVI_DIST_TO_DEST, "");
        xmlStrParse.setTextTo(VR_NAVI_TIME_TO_DST_H, "");
        xmlStrParse.setTextTo(VR_NAVI_TIME_TO_DST_M, "");
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_NaviStatusManager::QueryNaviToDestinationFromFC(const VR_String &strXml)
{
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    m_strPOIId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    m_strPOIName = xmlStrParse.GetXMLValue(VR_ADDR_NAME);
    m_strPOILongit = xmlStrParse.GetXMLValue(VR_ADDR_LONGITUDE);
    m_strPOILatit = xmlStrParse.GetXMLValue(VR_ADDR_LATITUDE);

    VR_String strXmlToFC = VR_TEMP_TO_DESTINATION_XML;

    if (xmlStrParse.LoadString(strXmlToFC)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, m_strPOIId);
        xmlStrParse.setTextTo(VR_NAVI_DEST_ADDR, m_strPOIName);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, m_strPOILongit);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, m_strPOILatit);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();

        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("NATP");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(strMsg);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
        m_emRounteType = VR_NAVI_POI_TEMP_ROUNT;
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
}

void VR_NaviStatusManager::ReplyNaviTimeToPOITOVR(const VR_ProtoMessageBase &msg)
{
    const navi::Path::RouteDetailInfo& routeDetailInfo = (const navi::Path::RouteDetailInfo &)msg;
    VR_String strPOIdist = "";
    VR_String strPOIHour = "";
    VR_String strPOIMin  = "";
    if (routeDetailInfo.has_route_length()) {
        strPOIdist = std::to_string(routeDetailInfo.route_length());
    }
    if (routeDetailInfo.has_route_static_time()) {
        strPOIHour = std::to_string(routeDetailInfo.route_static_time() / 3600);
        strPOIMin  = std::to_string(routeDetailInfo.route_static_time() % 3600 / 60);
    }

    VR_String strXml = VR_NAVI_DICT_TIME_TO_ADDR;
    VR_XmlStrParse xmlStrParse;
    VR_String strError = VR_NAVI_RESULT_OK;
    if ((strPOIdist.compare("") == 0) && ((strPOIHour.compare("") == 0) || (strPOIMin.compare("") == 0))) {
        strError = VR_NAVI_RESULT_ERROR;
    }
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_XPATH_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_XPATH_ACTION_ID, m_strPOIId);
        xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, strError);
        xmlStrParse.setTextTo(VR_NAVI_DIST_TO_DEST, strPOIdist);
        xmlStrParse.setTextTo(VR_NAVI_TIME_TO_DST_H, strPOIHour);
        xmlStrParse.setTextTo(VR_NAVI_TIME_TO_DST_M, strPOIMin);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();
        SendReqToNATP(strMsg);
    }
    else {
        VR_LOGD("LoadString [%s] error", strXml.c_str());
    }
    m_emRounteType = VR_NAVI_INVALID;
}

void VR_NaviStatusManager::QueryCategoryDestinationFromFC(const VR_String &strXml)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    m_strCategyId = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    m_strSearchMethod = xmlStrParse.GetXMLValue(VR_NAVI_SEARCH_METHOD);
    VR_String addr_name = xmlStrParse.GetXMLValue(VR_ADDR_NAME_CATEG);
    VR_String strLongitude = xmlStrParse.GetXMLValue(VR_ADDR_LONGITUDE);
    VR_String strLatitude = xmlStrParse.GetXMLValue(VR_ADDR_LATITUDE);
    m_strCategyName = xmlStrParse.GetXMLValue(VR_NAVI_CATEGORY);

    // Query navi search local poi
    if (VR_NAVI_SEARCH_LOCAL == m_strSearchMethod) {
        const int MAX_ITEMS = 10;
        std::unique_ptr<navi::Search::ReqCategorySearchVR> categorySearch(navi::Search::ReqCategorySearchVR::default_instance().New());
        categorySearch->set_senderid(atoi(m_strCategyId.c_str()));
        categorySearch->add_genrename(m_strCategyName);
        categorySearch->set_count(MAX_ITEMS);
        categorySearch->set_searchtype(navi::Search::ReqCategorySearchVR_SEARCH_TYPE_NEAR_HERE);

        nutshell::INT32  nLon(0);
        nutshell::INT32  nLat(0);
        nutshell::INT32  nRoadKind(0);
        bool  bDemonOn(false);
        VR_ConfigureIF::Instance()->getLocateInfo(nRoadKind, nLon, nLat, bDemonOn);
        VR_LOGD("cur pos: lon:%d  lat:%d", nLon, nLat);
        navi::Search::SearchDefine_MapLonLat* mapLonLat = categorySearch->mutable_map_lonlat();
        mapLonLat->set_lon(nLon);
        mapLonLat->set_lat(nLat);

        EV_EventSender().SendEvent(EV_EVENT_SEARCH_REQCATEGORYSEARCHVR, categorySearch.release());
        VR_LOGD("query near here poi: %s", m_strCategyName.c_str());
        return;
    }

    // Query navi search along route
    if (VR_NAVI_SEARCH_ALONG_ROUTE == m_strSearchMethod) {
        bool res = xmlStrParse.LoadString(VR_TEMP_TO_DESTINATION_XML);
        if (!res) {
            VR_LOGD("load error");
            return;
        }

        xmlStrParse.setTextTo(VR_XPATH_ID, m_strCategyId);
        xmlStrParse.setTextTo(VR_NAVI_DEST_ADDR, addr_name);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, strLongitude);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, strLatitude);
        VR_String strMsg = xmlStrParse.TransferXMLtoString();

        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("NATP");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
        noitfyProperty->mutable_vuicommonaction()->set_action(strMsg);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
        m_emRounteType = VR_NAVI_CATEGORY_ROUNT;
        return;
    }

    // exception case
    VR_LOGD("unknown search type: %s", m_strSearchMethod.c_str());
    xmlStrParse.Reset();
    bool res = xmlStrParse.LoadString(VR_NAVI_SRCH_CAT_BY_ADDR);
    if (!res) {
        VR_LOGD("load string error. 1");
        return;
    }

    xmlStrParse.SetNodeText(VR_XPATH_ID, GetActionId());
    xmlStrParse.SetNodeText(VR_XPATH_ACTION_ID, m_strCategyId.c_str());
    return;
}

void VR_NaviStatusManager::ReplyNaviSrchCategoryToVR(const VR_ProtoMessageBase &msg)
{
    if ("navi.Search.RespCategorySearchVRResult" != msg.GetTypeName()) {
        VR_LOGD("msg error: %s", msg.GetTypeName().c_str());
        return;
    }

    const navi::Search::RespCategorySearchVRResult& categoryResult =
            dynamic_cast<const navi::Search::RespCategorySearchVRResult&>(msg);

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_NAVI_SRCH_CAT_BY_ADDR);
    if (!res) {
        VR_LOGD("load string error. 1");
        return;
    }

    parser.SetNodeText(VR_XPATH_ID, GetActionId());
    parser.SetNodeText(VR_XPATH_ACTION_ID, categoryResult.senderid());

    int size = categoryResult.poi_data_size();
    for (int i = 0; i < size; i++) {
        VR_String addrName = categoryResult.poi_data(i).name();
        VR_String strAddr = categoryResult.poi_data(i).address();
        nutshell::NCString longitude;
        longitude.format("%.4f", static_cast<double>(categoryResult.poi_data(i).pos().lon()) / CONVERSIONRATIO_D);
        nutshell::NCString latitude;
        latitude.format("%.4f", static_cast<double>(categoryResult.poi_data(i).pos().lat()) / CONVERSIONRATIO_D);

        VR_XmlStrParse nodeParser;
        res = nodeParser.LoadString(VR_NAVI_POI_INFO_NODE);
        if (!res) {
            VR_LOGD("load string error. 2");
            continue;
        }

        nodeParser.setTextTo(VR_ADDR_NAME_CATEG_SET, addrName);
        nodeParser.setTextTo(VR_ADDR_CATEG_SET, strAddr);
        nodeParser.setTextTo(VR_NAVI_ADDR_LONGITUDE, longitude.getString());
        nodeParser.setTextTo(VR_NAVI_ADDR_LATITUDE, latitude.getString());

        VR_String nodeXml;
        nodeParser.GetXmlString(nodeXml);
        parser.AddNode(VR_NAVI_ADDNODE_POI_INFO_LIST_PATH, nodeXml);
    }

    VR_String strMsg;
    parser.GetXmlString(strMsg);
    SendReqToNATP(strMsg);
    VR_LOGD("send mesg:%s", strMsg.c_str());

    return;
}

void VR_NaviStatusManager::replyDestinationAddrToVR(VR_String &strId)
{
    VR_String strMsg = "";
    VR_String strXml = VR_NAVI_GET_ADDR_LOCAT;
    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, strId);

        VR_String strDestName = m_sDestName;
        nutshell::NCString strlongitude = "";
        nutshell::NCString strlatitude = "";
        if (strDestName == "") {
            strDestName = m_sAddrName;
        }
        if (m_sDestLongit != INVAKLID_VALUE) {
            strlongitude.format("%.4f", static_cast<double>(m_sDestLongit) / CONVERSIONRATIO_D);
        }
        if (m_sDestlatit != INVAKLID_VALUE) {
            strlatitude.format("%.4f", static_cast<double>(m_sDestlatit) / CONVERSIONRATIO_D);
        }
        xmlStrParse.setTextTo(VR_NAVI_ADDR_NAME, strDestName);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, strlongitude.getString());
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, strlatitude.getString());
        strMsg = xmlStrParse.TransferXMLtoString();
    }
    else {
        VR_LOGD("Loading VR_NAVI_GET_ADDR_LOCAT failed!");
        return;
    }
    SendReqToNATP(strMsg);
}

void VR_NaviStatusManager::replyLocalAddrLocationToVR(VR_String &strId)
{
    nutshell::INT32  nLon(0);
    nutshell::INT32  nLat(0);
    nutshell::INT32  nRoadKind(0);
    bool  bDemonOn(false);
    VR_ConfigureIF::Instance()->getLocateInfo(nRoadKind, nLon, nLat, bDemonOn);
    nutshell::NCString strlongitude;
    strlongitude.format("%.4f", static_cast<double>(nLon) / CONVERSIONRATIO_D);
    nutshell::NCString strlatitude;
    strlatitude.format("%.4f", static_cast<double>(nLat) / CONVERSIONRATIO_D);

    VR_String strMsg = "";
    VR_String strName = "";
    VR_String strXml = VR_NAVI_GET_ADDR_LOCAT;
    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, strId);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_NAME, strName);
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, strlongitude.getString());
        xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, strlatitude.getString());
        strMsg = xmlStrParse.TransferXMLtoString();
    }
    else {
        VR_LOGD("Loading VR_NAVI_GET_ADDR_LOCAT failed!");
        return;
    }
    SendReqToNATP(strMsg);
}

void VR_NaviStatusManager::replyCursorLocationToVR(VR_String &strId)
{
    VR_String strMsg = "";
    VR_String strXml = VR_NAVI_GET_ADDR_LOCAT;
    VR_XmlStrParse xmlStrParse;
    if (xmlStrParse.LoadString(strXml)) {
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ID, std::to_string(GetActionId()));
        xmlStrParse.setTextTo(VR_NAVI_STATUS_ACTID, strId);
        if (VR_ConfigureIF::Instance()->getCursorFlg() &&  (m_nCursorLon != INVAKLID_VALUE)) {
            nutshell::NCString strlongitude;
            nutshell::NCString strlatitude;
            strlongitude.format("%.4f", static_cast<double>(m_nCursorLon) / CONVERSIONRATIO_D);
            strlatitude.format("%.4f", static_cast<double>(m_nCursorLat) / CONVERSIONRATIO_D);
            xmlStrParse.setTextTo(VR_NAVI_ADDR_NAME, VR_ConfigureIF::Instance()->getStrRoadName().c_str());
            xmlStrParse.setTextTo(VR_NAVI_ADDR_LONGITUDE, strlongitude.getString());
            xmlStrParse.setTextTo(VR_NAVI_ADDR_LATITUDE, strlatitude.getString());
        }
        else {
            xmlStrParse.setTextTo(VR_NAVI_ERROR_NO, VR_NAVI_RESULT_ERROR);
        }
        strMsg = xmlStrParse.TransferXMLtoString();
    }
    else {
        VR_LOGD("Loading VR_NAVI_GET_ADDR_LOCAT failed!");
        return;
    }
    SendReqToNATP(strMsg);
}

void VR_NaviStatusManager::OnMessageNaviStatusChanged(VR_String &strIsMapOn)
{
    if ("true" != strIsMapOn) {
        strIsMapOn = "false";
    }

    if (strIsMapOn != m_sMapStatus) {
        m_sMapStatus = strIsMapOn;
        VR_String notifyXML;
        if (GetNavigationStatusNotifyXML(notifyXML)) {
            SendReqToNATP(notifyXML);
        }
    }
}

void VR_NaviStatusManager::NotifyRouteOptionChanged()
{
    // get system route option
    VR_ContentProvider vr_cp;
    VR_String strRouteOption = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ROUTEOPTION);
    VR_LOGI("strRouteOption = [%s]", strRouteOption.c_str());

    auto iter = g_strRouteOptionMap.find(strRouteOption);
    if (g_strRouteOptionMap.end() == iter) {
        strRouteOption = "NAVI_POLICY_RECOMMEND_ROUTE";
    }

    if (strRouteOption != m_sRouteOption) {
        m_sRouteOption = strRouteOption;
        VR_String notifyXML;
        if (GetNavigationStatusNotifyXML(notifyXML)) {
            SendReqToNATP(notifyXML);
        }
    }
}

VR_UINT32 VR_NaviStatusManager::GetActionId()
{
    VR_IdGenerator nId;
    return nId.GetID();
}

void VR_NaviStatusManager::SendReqToNATP(VR_String &strXml)
{
    VR_LOGD_FUNC();
    VR_LOGD("strXml=[%s]", strXml.c_str());
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(strXml);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

/* EOF */

