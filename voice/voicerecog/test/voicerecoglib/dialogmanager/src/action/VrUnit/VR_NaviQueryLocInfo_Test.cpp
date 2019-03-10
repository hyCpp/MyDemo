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
#include "VR_NaviQueryLocInfo.h"
#include "VR_ActionEventSender.h"
#include "VR_AvcLanSenderIF.h"
#include "VR_DialogEngineFactory.h"
#include "VR_DialogEngineNull.h"
#include "MEM_map.h"

#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"
#include "navi/Point/PointResponse.pb.h"

using namespace navi::Point;

/**
* VR_NaviQueryLocInfo_Test
*
* The class is just for VR_NaviQueryLocInfo test.
*/
class VR_NaviQueryLocInfo_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    static spVR_DialogEngineIF m_sp_EngineIF;
    static spVR_ActionEventSender m_sp_eventSender;
    static spVR_AvcLanSenderIF m_sp_avcLanSender;

    VR_NaviQueryLocInfo *m_vr_queryLocInfo1;
    VR_NaviQueryLocInfo *m_vr_queryLocInfo2;
    VR_NaviQueryLocInfo *m_vr_queryLocInfo3;
    VR_NaviQueryLocInfo *m_vr_queryLocInfo4;
};

spVR_DialogEngineIF VR_NaviQueryLocInfo_Test::m_sp_EngineIF = boost::shared_ptr<VR_DialogEngineIF>(VR_new VR_DialogEngineNull());
spVR_ActionEventSender VR_NaviQueryLocInfo_Test::m_sp_eventSender(VR_new VR_ActionEventSender(m_sp_EngineIF));
spVR_AvcLanSenderIF VR_NaviQueryLocInfo_Test::m_sp_avcLanSender = NULL;

void VR_NaviQueryLocInfo_Test::SetUp()
{
    // Query point collections
    int taskId1 = 1;
    std::string msg1 = "<action agent = \"navi\" op = \"queryPointCollection\">"
                           "<type>HOME</type>"
                       "</action>";
    m_vr_queryLocInfo1 = VR_new VR_NaviQueryLocInfo(m_sp_eventSender, taskId1, 18, msg1, m_sp_avcLanSender);

    // Query destination history
    int taskId2 = 2;
    std::string msg2 = "<action agent = \"navi\" op = \"queryDestinationHistory\">"
                           "<count>2</count>"
                       "</action>";
    m_vr_queryLocInfo2 = VR_new VR_NaviQueryLocInfo(m_sp_eventSender, taskId2, 17, msg2, m_sp_avcLanSender);

    // Query current position
    int taskId3 = 3;
    std::string msg3 = "<action agent = \"navi\" op = \"queryCurrentPosition\">"
                       "</action>";
    m_vr_queryLocInfo3 = VR_new VR_NaviQueryLocInfo(m_sp_eventSender, taskId3, 24, msg3, m_sp_avcLanSender);

    // Query map data
    int taskId4 = 4;
    std::string msg4 = "<action agent = \"navi\" op = \"queryPointDataArea\">"
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
    m_vr_queryLocInfo4 = VR_new VR_NaviQueryLocInfo(m_sp_eventSender, taskId4, 19, msg4, m_sp_avcLanSender);
}

void VR_NaviQueryLocInfo_Test::TearDown()
{
    if (NULL != m_vr_queryLocInfo1) {
        delete m_vr_queryLocInfo1;
        m_vr_queryLocInfo1 = NULL;
    }
    if (NULL != m_vr_queryLocInfo2) {
        delete m_vr_queryLocInfo2;
        m_vr_queryLocInfo2 = NULL;
    }
    if (NULL != m_vr_queryLocInfo3) {
        delete m_vr_queryLocInfo3;
        m_vr_queryLocInfo3 = NULL;
    }
    if (NULL != m_vr_queryLocInfo4) {
        delete m_vr_queryLocInfo4;
        m_vr_queryLocInfo4 = NULL;
    }
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_Constructor)
{
    SUCCEED();
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_Run)
{
    VR_LOGD_FUNC();
    m_vr_queryLocInfo1->Run();
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_RunAction)
{
    VR_LOGD_FUNC();
    m_vr_queryLocInfo1->RunAction();
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_NaviQueryPointCollection)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc1;
    doc1.load((m_vr_queryLocInfo1->m_ReqMsg).c_str());
    m_vr_queryLocInfo1->NaviQueryPointCollection(doc1);
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_NaviQueryDestHistory)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc2;
    doc2.load((m_vr_queryLocInfo2->m_ReqMsg).c_str());
    m_vr_queryLocInfo2->NaviQueryDestHistory(doc2);
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_NaviQueryCurrentPosition)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc3;
    doc3.load((m_vr_queryLocInfo3->m_ReqMsg).c_str());
    m_vr_queryLocInfo3->NaviQueryCurrentPosition(doc3);
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_NaviQueryPointDataArea)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc4;
    doc4.load((m_vr_queryLocInfo4->m_ReqMsg).c_str());
    m_vr_queryLocInfo4->NaviQueryPointDataArea(doc4);
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_OnReceive)
{
    VR_LOGD_FUNC();
    std::unique_ptr<PointResponse> proto1;
    proto1.reset(PointResponse::default_instance().New());
    PointBasicRespInfo *pRspInfo = proto1->mutable_respinfo();
    pRspInfo->set_requestid(1);
    pRspInfo->set_result(POINT_REQ_RESULT_SUCCESS);
    RespGetDetailInfo *pGetDtlInfo = proto1->mutable_respgetdetail();
    DetailInfoDisplay *pDtlInfoDisp = pGetDtlInfo->mutable_detailinfo();
    PointRecord *pPtRecord = pDtlInfoDisp->mutable_recinfo();
    pPtRecord->set_uuid("123456");
    pPtRecord->set_recordtype(POINT_RECORD_TYPE_HOME);
    pPtRecord->set_recordstatus(POINT_RECORD_STATUS_NORMAL);
    pPtRecord->set_dispname("HOME");
    pPtRecord->set_longitude(137334450);
    pPtRecord->set_latitude(-32438082);
    GuidePointInfo *pGuidePtInfo = pPtRecord->add_guidepoint();
    pGuidePtInfo->set_guidepointlon(137334450);
    pGuidePtInfo->set_guidepointlat(-32438082);
    pGuidePtInfo->set_type(1);
    m_vr_queryLocInfo1->OnReceive(*proto1);
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_RespQueryPointCollection)
{
    VR_LOGD_FUNC();
    std::unique_ptr<PointResponse> proto1;
    proto1.reset(PointResponse::default_instance().New());
    // result success
    PointBasicRespInfo *pRspInfo = proto1->mutable_respinfo();
    pRspInfo->set_requestid(1);
    pRspInfo->set_result(POINT_REQ_RESULT_SUCCESS);
    RespGetDetailInfo *pGetDtlInfo = proto1->mutable_respgetdetail();
    DetailInfoDisplay *pDtlInfoDisp = pGetDtlInfo->mutable_detailinfo();
    PointRecord *pPtRecord = pDtlInfoDisp->mutable_recinfo();
    pPtRecord->set_uuid("123456");
    pPtRecord->set_recordtype(POINT_RECORD_TYPE_HOME);
    pPtRecord->set_recordstatus(POINT_RECORD_STATUS_NORMAL);
    pPtRecord->set_dispname("HOME");
    pPtRecord->set_longitude(137334450);
    pPtRecord->set_latitude(-32438082);
    GuidePointInfo *pGuidePtInfo = pPtRecord->add_guidepoint();
    pGuidePtInfo->set_guidepointlon(137334450);
    pGuidePtInfo->set_guidepointlat(-32438082);
    pGuidePtInfo->set_type(1);
    m_vr_queryLocInfo1->RespQueryPointCollection(*proto1);
    // result failed
    pRspInfo->clear_result();
    pRspInfo->set_result(POINT_REQ_RESULT_FAILURE);
    m_vr_queryLocInfo1->RespQueryPointCollection(*proto1);
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_RespQueryDestHistory)
{
    VR_LOGD_FUNC();
    std::unique_ptr<PointResponse> proto2;
    proto2.reset(PointResponse::default_instance().New());
    // result success
    PointBasicRespInfo *pRspInfo = proto2->mutable_respinfo();
    pRspInfo->set_requestid(1);
    pRspInfo->set_result(POINT_REQ_RESULT_SUCCESS);
    RespGetRecordList *pRspRecList = proto2->mutable_respgetlist();
    pRspRecList->set_list_type(POINT_LIST_TYPE_HISTORY);
    ListRecord *pListRec = pRspRecList->add_point_info();
    PointRecord *pPtRecord = pListRec->mutable_recinfo();
    pPtRecord->set_uuid("654321");
    pPtRecord->set_recordtype(POINT_RECORD_TYPE_HISTORY);
    pPtRecord->set_recordstatus(POINT_RECORD_STATUS_NORMAL);
    pPtRecord->set_dispname("COMPANY");
    pPtRecord->set_longitude(137334450);
    pPtRecord->set_latitude(-32438082);
    GuidePointInfo *pGuidePtInfo = pPtRecord->add_guidepoint();
    pGuidePtInfo->set_guidepointlon(137334450);
    pGuidePtInfo->set_guidepointlat(-32438082);
    pGuidePtInfo->set_type(1);
    m_vr_queryLocInfo2->RespQueryDestHistory(*proto2);
    // result failed
    pRspInfo->clear_result();
    pRspInfo->set_result(POINT_REQ_RESULT_FAILURE);
    m_vr_queryLocInfo2->RespQueryPointCollection(*proto2);
}

TEST_F(VR_NaviQueryLocInfo_Test, Test_RespQueryCurrentPosition)
{
    SUCCEED();
}

/* EOF */
