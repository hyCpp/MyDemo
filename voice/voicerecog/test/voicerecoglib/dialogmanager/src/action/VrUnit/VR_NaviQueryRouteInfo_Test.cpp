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

#include "gtest/gtest.h"

#include <stdlib.h>
#include "VR_NaviQueryRouteInfo.h"
#include "VR_ActionEventSender.h"
#include "VR_AvcLanSenderIF.h"
#include "VR_DialogEngineFactory.h"
#include "VR_DialogEngineNull.h"
#include "MEM_map.h"

#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"
#include "navi/Path/RouteStatus.pb.h"
#include "navi/Path/PathPointList.pb.h"
#include "navi/Path/NearLinkResult.pb.h"

using namespace navi::Path;

/**
* VR_NaviQueryRouteInfo_Test
*
* The class is just for VR_NaviQueryRouteInfo test.
*/
class VR_NaviQueryRouteInfo_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    static spVR_DialogEngineIF m_sp_EngineIF;
    static spVR_ActionEventSender m_sp_eventSender;
    static spVR_AvcLanSenderIF m_sp_avcLanSender;

    VR_NaviQueryRouteInfo *m_vr_queryRoute1;
    VR_NaviQueryRouteInfo *m_vr_queryRoute2;
    VR_NaviQueryRouteInfo *m_vr_queryRoute3;
    VR_NaviQueryRouteInfo *m_vr_queryRoute4;
    VR_NaviQueryRouteInfo *m_vr_queryRoute5;
    VR_NaviQueryRouteInfo *m_vr_queryRoute6;
};

spVR_DialogEngineIF VR_NaviQueryRouteInfo_Test::m_sp_EngineIF = boost::shared_ptr<VR_DialogEngineIF>(VR_new VR_DialogEngineNull());
spVR_ActionEventSender VR_NaviQueryRouteInfo_Test::m_sp_eventSender(VR_new VR_ActionEventSender(m_sp_EngineIF));
spVR_AvcLanSenderIF VR_NaviQueryRouteInfo_Test::m_sp_avcLanSender = NULL;

void VR_NaviQueryRouteInfo_Test::SetUp()
{
    // Query all drop points
    int taskId1 = 1;
    std::string msg1 = "<action agent = \"navi\" op = \"queryAllDestinationList\">"
                       "</action>";
    m_vr_queryRoute1 = VR_new VR_NaviQueryRouteInfo(m_sp_eventSender, taskId1, 25, msg1, m_sp_avcLanSender);

    // Query route exist
    int taskId2 = 2;
    std::string msg2 = "<action agent = \"navi\" op = \"queryRouteExist\">"
                       "</action>";
    m_vr_queryRoute2 = VR_new VR_NaviQueryRouteInfo(m_sp_eventSender, taskId2, 20, msg2, m_sp_avcLanSender);

    // Query unpassed drop points
    int taskId3 = 3;
    std::string msg3 = "<action agent = \"navi\" op = \"queryUnpassedDestinationList\">"
                       "</action>";
    m_vr_queryRoute3 = VR_new VR_NaviQueryRouteInfo(m_sp_eventSender, taskId3, 21, msg3, m_sp_avcLanSender);

    // Query guide point type
    int taskId4 = 4;
    std::string msg4 = "<action agent = \"navi\" op = \"queryPointType\">"
                           "<guidePoint>"
                               "<name>''</name>"
                               "<displayGuideType>''</displayGuideType>"
                               "<linkId>''</linkId>"
                               "<longitude>137439884</longitude>"
                               "<latitude>-33391764</latitude>"
                               "<list>"
                                   "<header>"
                                       "<pageSize>''</pageSize>"
                                       "<startIndex>''</startIndex>"
                                       "<count>1</count>"
                                   "</header>"
                                   "<items>"
                                       "<item>"
                                           "<guideType>''</guideType>"
                                           "<guideLongitude>''</guideLongitude>"
                                           "<guideLatitude>''</guideLatitude>"
                                       "</item>"
                                   "</items>"
                               "</list>"
                           "</guidePoint>"
                       "</action>";
    m_vr_queryRoute4 = VR_new VR_NaviQueryRouteInfo(m_sp_eventSender, taskId4, 22, msg4, m_sp_avcLanSender);

    // Query next destination
    int taskId5 = 5;
    std::string msg5 = "<action agent = \"navi\" op = \"queryNextDestination\">"
                       "</action>";
    m_vr_queryRoute5 = VR_new VR_NaviQueryRouteInfo(m_sp_eventSender, taskId5, 23, msg5, m_sp_avcLanSender);

    // Calculate route to a guide point
    int taskId6 = 6;
    std::string msg6 = "<action agent = \"navi\" op = \"calculateRoute\">"
                           "<guidePoint>"
                               "<name>''</name>"
                               "<displayGuideType>''</displayGuideType>"
                               "<linkId>''</linkId>"
                               "<longitude>137439884</longitude>"
                               "<latitude>-33391764</latitude>"
                               "<list>"
                                   "<header>"
                                       "<pageSize>''</pageSize>"
                                       "<startIndex>''</startIndex>"
                                       "<count>1</count>"
                                   "</header>"
                                   "<items>"
                                       "<item>"
                                           "<guideType>''</guideType>"
                                           "<guideLongitude>''</guideLongitude>"
                                           "<guideLatitude>''</guideLatitude>"
                                       "</item>"
                                   "</items>"
                               "</list>"
                           "</guidePoint>"
                       "</action>";
    m_vr_queryRoute6 = VR_new VR_NaviQueryRouteInfo(m_sp_eventSender, taskId6, 3, msg6, m_sp_avcLanSender);
}

void VR_NaviQueryRouteInfo_Test::TearDown()
{
    if (NULL != m_vr_queryRoute1) {
        delete m_vr_queryRoute1;
        m_vr_queryRoute1 = NULL;
    }
    if (NULL != m_vr_queryRoute2) {
        delete m_vr_queryRoute2;
        m_vr_queryRoute2 = NULL;
    }
    if (NULL != m_vr_queryRoute3) {
        delete m_vr_queryRoute3;
        m_vr_queryRoute3 = NULL;
    }
    if (NULL != m_vr_queryRoute4) {
        delete m_vr_queryRoute4;
        m_vr_queryRoute4 = NULL;
    }
    if (NULL != m_vr_queryRoute5) {
        delete m_vr_queryRoute5;
        m_vr_queryRoute5 = NULL;
    }
    if (NULL != m_vr_queryRoute6) {
        delete m_vr_queryRoute6;
        m_vr_queryRoute6 = NULL;
    }
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_Constructor)
{
    SUCCEED();
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_Run)
{
    VR_LOGD_FUNC();
    m_vr_queryRoute1->Run();
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_RunAction)
{
    VR_LOGD_FUNC();
    m_vr_queryRoute1->RunAction();
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_NaviQueryAllDestinationList)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc1;
    doc1.load((m_vr_queryRoute1->m_ReqMsg).c_str());
    m_vr_queryRoute1->NaviQueryAllDestinationList(doc1);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_NaviQueryRouteExist)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc2;
    doc2.load((m_vr_queryRoute2->m_ReqMsg).c_str());
    m_vr_queryRoute2->NaviQueryRouteExist(doc2);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_NaviQueryUnPassDestinationList)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc3;
    doc3.load((m_vr_queryRoute3->m_ReqMsg).c_str());
    m_vr_queryRoute3->NaviQueryUnPassDestinationList(doc3);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_NaviQueryPointType)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc4;
    doc4.load((m_vr_queryRoute4->m_ReqMsg).c_str());
    m_vr_queryRoute4->NaviQueryPointType(doc4);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_NaviQueryNextDestination)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc5;
    doc5.load((m_vr_queryRoute5->m_ReqMsg).c_str());
    m_vr_queryRoute5->NaviQueryNextDestination(doc5);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_NaviCalculateRoute)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc6;
    doc6.load((m_vr_queryRoute6->m_ReqMsg).c_str());
    m_vr_queryRoute6->NaviCalculateRoute(doc6);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_OnReceive)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RouteStatus> proto2;
    proto2.reset(RouteStatus::default_instance().New());
    proto2->set_object_id(PathDef_RequestObjectType_OBJECT_TYPE_VOICE);
    proto2->set_route_status(PathDef_RouteStatus_ROUTESTATUS_EXIST);
    proto2->set_sender_id(2);
    m_vr_queryRoute2->OnReceive(*proto2);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_RespQueryRouteExist)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RouteStatus> proto2;
    proto2.reset(RouteStatus::default_instance().New());
    proto2->set_object_id(PathDef_RequestObjectType_OBJECT_TYPE_VOICE);
    proto2->set_route_status(PathDef_RouteStatus_ROUTESTATUS_EXIST);
    proto2->set_sender_id(2);
    m_vr_queryRoute2->RespQueryRouteExist(*proto2);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_RespQueryAllDestinationList)
{
    VR_LOGD_FUNC();
    std::unique_ptr<PathPointList> proto1;
    proto1.reset(PathPointList::default_instance().New());
    PathDef_RequestObjKey *pReqObjKey = proto1->mutable_obj_key();
    pReqObjKey->set_req_obj(PathDef_RequestObjectType_OBJECT_TYPE_VOICE);
    pReqObjKey->set_req_id(1);
    // there's one route point
    PathDef_RoutePoint *pRoutePt = proto1->add_route_point();
    pRoutePt->set_point_use(PathDef_PointUse_POINT_USE);
    pRoutePt->set_point_type(PathDef_PointType_POINT_TYPE_DESTINATION);
    pRoutePt->set_point_type_index(2);
    PathDef_GuidePoint *pDispPt = pRoutePt->mutable_disp_point();
    PathDef_LonLat *pDispLonLat = pDispPt->mutable_lonlat();
    pDispLonLat->set_lon(137439884);
    pDispLonLat->set_lat(-33391764);
    pDispPt->set_link_id(2);
    pDispPt->set_link_side_info(PathDef_LinkSideInfo_LINK_SIDE_RIGHT);
    pDispPt->set_point_attribute(0);
    PathDef_GuidePoint *pGuidePt = pRoutePt->add_guide_point();
    PathDef_LonLat *pGuideLonLat = pGuidePt->mutable_lonlat();
    pGuideLonLat->set_lon(137439884);
    pGuideLonLat->set_lat(-33391764);
    pGuidePt->set_link_id(3);
    pGuidePt->set_link_side_info(PathDef_LinkSideInfo_LINK_ON_LINK);
    pGuidePt->set_point_attribute(0);
    pRoutePt->set_total_point_index(3);
    pRoutePt->set_point_name("Akubra");
    pRoutePt->set_uuid("Akubra");
    m_vr_queryRoute1->RespQueryAllDestinationList(*proto1);

    // there's no route point
    proto1->clear_route_point();
    m_vr_queryRoute1->RespQueryAllDestinationList(*proto1);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_RespQueryUnPassDestinationList)
{
    VR_LOGD_FUNC();
    std::unique_ptr<PathPointList> proto3;
    proto3.reset(PathPointList::default_instance().New());
    PathDef_RequestObjKey *pReqObjKey = proto3->mutable_obj_key();
    pReqObjKey->set_req_obj(PathDef_RequestObjectType_OBJECT_TYPE_VOICE);
    pReqObjKey->set_req_id(3);
    // there's one route point
    PathDef_RoutePoint *pRoutePt = proto3->add_route_point();
    pRoutePt->set_point_use(PathDef_PointUse_POINT_USE);
    pRoutePt->set_point_type(PathDef_PointType_POINT_TYPE_DESTINATION);
    pRoutePt->set_point_type_index(3);
    PathDef_GuidePoint *pDispPt = pRoutePt->mutable_disp_point();
    PathDef_LonLat *pDispLonLat = pDispPt->mutable_lonlat();
    pDispLonLat->set_lon(137439884);
    pDispLonLat->set_lat(-33391764);
    pDispPt->set_link_id(2);
    pDispPt->set_link_side_info(PathDef_LinkSideInfo_LINK_SIDE_RIGHT);
    pDispPt->set_point_attribute(0);
    PathDef_GuidePoint *pGuidePt = pRoutePt->add_guide_point();
    PathDef_LonLat *pGuideLonLat = pGuidePt->mutable_lonlat();
    pGuideLonLat->set_lon(137439884);
    pGuideLonLat->set_lat(-33391764);
    pGuidePt->set_link_id(3);
    pGuidePt->set_link_side_info(PathDef_LinkSideInfo_LINK_ON_LINK);
    pGuidePt->set_point_attribute(0);
    pRoutePt->set_total_point_index(3);
    pRoutePt->set_point_name("Akubra");
    pRoutePt->set_uuid("Akubra");
    m_vr_queryRoute3->RespQueryAllDestinationList(*proto3);

    // there's no route point
    proto3->clear_route_point();
    m_vr_queryRoute3->RespQueryAllDestinationList(*proto3);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_RespQueryPointType)
{
    VR_LOGD_FUNC();
    std::unique_ptr<NearLinkResult> proto4;
    proto4.reset(NearLinkResult::default_instance().New());
    PathDef_RequestObjKey *pReqObjKey = proto4->mutable_req_key();
    pReqObjKey->set_req_obj(PathDef_RequestObjectType_OBJECT_TYPE_VOICE);
    pReqObjKey->set_req_id(4);
    proto4->set_nearlink_nums(1);
    proto4->set_road_kind_nums(1);
    NearLinkResult_NearestLink *pNearestLink = proto4->add_nearlink_info();
    PathDef_LonLat *pLonLat = pNearestLink->mutable_point_lonlat();
    pLonLat->set_lon(137439884);
    pLonLat->set_lat(-33391764);
    PathDef_LonLat *pFootLonLat = pNearestLink->mutable_foot_lonlat();
    pFootLonLat->set_lon(137439884);
    pFootLonLat->set_lat(-33391764);
    pNearestLink->set_linkid(3);
    pNearestLink->set_road_kind(PathDef_RoadKind_RoadKind_Normal);
    pNearestLink->set_prep_length(10.0);
    pNearestLink->set_road_name("Akubra Place");
    m_vr_queryRoute4->RespQueryPointType(*proto4);
}

TEST_F(VR_NaviQueryRouteInfo_Test, Test_RespQueryNextDestination)
{
    VR_LOGD_FUNC();
    std::unique_ptr<PathPointList> proto5;
    proto5.reset(PathPointList::default_instance().New());
    PathDef_RequestObjKey *pReqObjKey = proto5->mutable_obj_key();
    pReqObjKey->set_req_obj(PathDef_RequestObjectType_OBJECT_TYPE_VOICE);
    pReqObjKey->set_req_id(5);
    // there's one route point
    PathDef_RoutePoint *pRoutePt = proto5->add_route_point();
    pRoutePt->set_point_use(PathDef_PointUse_POINT_USE);
    pRoutePt->set_point_type(PathDef_PointType_POINT_TYPE_DESTINATION);
    pRoutePt->set_point_type_index(3);
    PathDef_GuidePoint *pDispPt = pRoutePt->mutable_disp_point();
    PathDef_LonLat *pDispLonLat = pDispPt->mutable_lonlat();
    pDispLonLat->set_lon(137439884);
    pDispLonLat->set_lat(-33391764);
    pDispPt->set_link_id(2);
    pDispPt->set_link_side_info(PathDef_LinkSideInfo_LINK_SIDE_RIGHT);
    pDispPt->set_point_attribute(0);
    PathDef_GuidePoint *pGuidePt = pRoutePt->add_guide_point();
    PathDef_LonLat *pGuideLonLat = pGuidePt->mutable_lonlat();
    pGuideLonLat->set_lon(137439884);
    pGuideLonLat->set_lat(-33391764);
    pGuidePt->set_link_id(3);
    pGuidePt->set_link_side_info(PathDef_LinkSideInfo_LINK_ON_LINK);
    pGuidePt->set_point_attribute(0);
    pRoutePt->set_total_point_index(3);
    pRoutePt->set_point_name("Akubra");
    pRoutePt->set_uuid("Akubra");
    m_vr_queryRoute5->RespQueryAllDestinationList(*proto5);

    // there's no route point
    proto5->clear_route_point();
    m_vr_queryRoute5->RespQueryAllDestinationList(*proto5);
}

/* EOF */
