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
#include "VR_NaviQueryPoiInfo.h"
#include "VR_ActionEventSender.h"
#include "VR_AvcLanSenderIF.h"
#include "VR_DialogEngineFactory.h"
#include "VR_DialogEngineNull.h"
#include "MEM_map.h"

#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"
#include "navi/Search/RespCategorySearchVRResult.pb.h"

using namespace navi::Search;

/**
* VR_NaviQueryPoiInfo_Test
*
* The class is just for VR_NaviQueryPoiInfo test.
*/
class VR_NaviQueryPoiInfo_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    static spVR_DialogEngineIF m_sp_EngineIF;
    static spVR_ActionEventSender m_sp_eventSender;
    static spVR_AvcLanSenderIF m_sp_avcLanSender;

    VR_NaviQueryPoiInfo *m_vr_queryPoi1;
    VR_NaviQueryPoiInfo *m_vr_queryPoi2;
    VR_NaviQueryPoiInfo *m_vr_queryPoi3;
    VR_NaviQueryPoiInfo *m_vr_queryPoi4;
    VR_NaviQueryPoiInfo *m_vr_queryPoi5;
    VR_NaviQueryPoiInfo *m_vr_queryPoi6;
    VR_NaviQueryPoiInfo *m_vr_queryPoi7;
};

spVR_DialogEngineIF VR_NaviQueryPoiInfo_Test::m_sp_EngineIF = boost::shared_ptr<VR_DialogEngineIF>(VR_new VR_DialogEngineNull());
spVR_ActionEventSender VR_NaviQueryPoiInfo_Test::m_sp_eventSender(VR_new VR_ActionEventSender(m_sp_EngineIF));
spVR_AvcLanSenderIF VR_NaviQueryPoiInfo_Test::m_sp_avcLanSender = NULL;

void VR_NaviQueryPoiInfo_Test::SetUp()
{
    // Query POI collection nearby
    int taskId1 = 1;
    std::string msg1 = "<action agent = \"navi\" op = \"queryPOICollectionNearby\">"
                           "<languageId>3</languageId>"
                           "<categoryId>18087941</categoryId>"
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
    m_vr_queryPoi1 = VR_new VR_NaviQueryPoiInfo(m_sp_eventSender, taskId1, 13, msg1, m_sp_avcLanSender);

    // Query POI collection near destination
    int taskId2 = 2;
    std::string msg2 = "<action agent = \"navi\" op = \"queryPOICollectionNearDestination\">"
                           "<languageId>3</languageId>"
                           "<categoryId>18087941</categoryId>"
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
    m_vr_queryPoi2 = VR_new VR_NaviQueryPoiInfo(m_sp_eventSender, taskId2, 14, msg2, m_sp_avcLanSender);

    // Query POI collection along route
    int taskId3 = 3;
    std::string msg3 = "<action agent = \"navi\" op = \"queryPOICollectionAlongRoute\">"
                           "<languageId>3</languageId>"
                           "<categoryId>18087941</categoryId>"
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
    m_vr_queryPoi3 = VR_new VR_NaviQueryPoiInfo(m_sp_eventSender, taskId3, 15, msg3, m_sp_avcLanSender);

    // Show POI icon
    int taskId4 = 4;
    std::string msg4 = "<action agent = \"navi\" op = \"showPOIIcon\">"
                           "<categoryId>18087941</categoryId>"
                       "</action>";
    m_vr_queryPoi4 = VR_new VR_NaviQueryPoiInfo(m_sp_eventSender, taskId4, 1, msg4, m_sp_avcLanSender);

    // Hide POI icon
    int taskId5 = 5;
    std::string msg5 = "<action agent = \"navi\" op = \"hidePOIIcon\">"
                           "<categoryId>18087941</categoryId>"
                       "</action>";
    m_vr_queryPoi5 = VR_new VR_NaviQueryPoiInfo(m_sp_eventSender, taskId5, 2, msg5, m_sp_avcLanSender);

    // Add a destination to a guide point
    int taskId6 = 6;
    std::string msg6 = "<action agent = \"navi\" op = \"addToRoute\">"
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
    m_vr_queryPoi6 = VR_new VR_NaviQueryPoiInfo(m_sp_eventSender, taskId6, 5, msg6, m_sp_avcLanSender);

    // Delete destination
    int taskId7 = 7;
    std::string msg7 = "<action agent = \"navi\" op = \"deleteDestination\">"
                           "<dropPointId>"
                               "<id>2</id>"
                           "</dropPointId>"
                       "</action>";
    m_vr_queryPoi7 = VR_new VR_NaviQueryPoiInfo(m_sp_eventSender, taskId7, 4, msg7, m_sp_avcLanSender);
}

void VR_NaviQueryPoiInfo_Test::TearDown()
{
    if (NULL != m_vr_queryPoi1) {
        delete m_vr_queryPoi1;
        m_vr_queryPoi1 = NULL;
    }
    if (NULL != m_vr_queryPoi2) {
        delete m_vr_queryPoi2;
        m_vr_queryPoi2 = NULL;
    }
    if (NULL != m_vr_queryPoi3) {
        delete m_vr_queryPoi3;
        m_vr_queryPoi3 = NULL;
    }
    if (NULL != m_vr_queryPoi4) {
        delete m_vr_queryPoi4;
        m_vr_queryPoi4 = NULL;
    }
    if (NULL != m_vr_queryPoi5) {
        delete m_vr_queryPoi5;
        m_vr_queryPoi5 = NULL;
    }
    if (NULL != m_vr_queryPoi6) {
        delete m_vr_queryPoi6;
        m_vr_queryPoi6 = NULL;
    }
    if (NULL != m_vr_queryPoi7) {
        delete m_vr_queryPoi7;
        m_vr_queryPoi7 = NULL;
    }
}

TEST_F(VR_NaviQueryPoiInfo_Test, Test_Constructor)
{
    SUCCEED();
}

TEST_F(VR_NaviQueryPoiInfo_Test, Test_Run)
{
    VR_LOGD_FUNC();
    m_vr_queryPoi1->Run();
    m_vr_queryPoi2->Run();
    m_vr_queryPoi3->Run();
    m_vr_queryPoi4->Run();
    m_vr_queryPoi5->Run();
    m_vr_queryPoi6->Run();
    m_vr_queryPoi7->Run();
}

TEST_F(VR_NaviQueryPoiInfo_Test, Test_RunAction)
{
    VR_LOGD_FUNC();
    m_vr_queryPoi1->RunAction();
    m_vr_queryPoi2->RunAction();
    m_vr_queryPoi3->RunAction();
}

TEST_F(VR_NaviQueryPoiInfo_Test, Test_RunActionToFC)
{
    VR_LOGD_FUNC();
    m_vr_queryPoi4->RunActionToFC();
    m_vr_queryPoi5->RunActionToFC();
    m_vr_queryPoi6->RunActionToFC();
    m_vr_queryPoi7->RunActionToFC();
}

TEST_F(VR_NaviQueryPoiInfo_Test, Test_OnReceive)
{
    VR_LOGD_FUNC();
    // error code is zero
    std::unique_ptr<RespCategorySearchVRResult> proto;
    proto.reset(RespCategorySearchVRResult::default_instance().New());
    proto->set_senderid(1);
    proto->set_errorcode(0);
    proto->set_errortype(SearchDefine_ERROR_TYPE_NO_ERROR);
    proto->set_hitcount(1);
    RespCategorySearchVRResult_POIListItem *pPOIListItem = proto->add_poi_data();
    pPOIListItem->set_poiid(1);
    pPOIListItem->set_name("KFC");
    SearchDefine_MapLonLat *pLonLat = pPOIListItem->mutable_pos();
    pLonLat->set_lon(137439884);
    pLonLat->set_lat(-33391764);
    SearchDefine_GuidePoint *pGuidePt = pPOIListItem->add_guidepoint();
    pGuidePt->set_guidelink(10);
    pGuidePt->set_roadtype(1);
    pGuidePt->set_linkdirect(1);
    SearchDefine_MapLonLat *pGuideLonLat = pGuidePt->mutable_guidelonlat();
    pGuideLonLat->set_lon(137439884);
    pGuideLonLat->set_lat(-33391764);
    pPOIListItem->set_tel("999");
    pPOIListItem->set_address("Akubra Place");
    pPOIListItem->set_genreid(18087941);
    pPOIListItem->set_distance(12);
    pPOIListItem->set_angle(30);
    m_vr_queryPoi1->OnReceive(*proto);
    m_vr_queryPoi2->OnReceive(*proto);
    m_vr_queryPoi3->OnReceive(*proto);
    // error code is nonzero
    proto->clear_errorcode();
    proto->set_errorcode(1);
    m_vr_queryPoi1->OnReceive(*proto);
}

TEST_F(VR_NaviQueryPoiInfo_Test, Test_Encode)
{
    VR_LOGD_FUNC();
    std::string str1 = "a&b";
    m_vr_queryPoi1->Encode(str1);
}

/* EOF */
