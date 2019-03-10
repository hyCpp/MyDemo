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
#include "VR_NaviQueryLocationInfo.h"
#include "VR_ActionEventSender.h"
#include "VR_AvcLanSenderIF.h"
#include "VR_DialogEngineFactory.h"
#include "VR_DialogEngineNull.h"
#include "MEM_map.h"

#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"
#include "navi/Search/DefaultInfoResult.pb.h"
#include "navi/Search/RespStateInfoVRResult.pb.h"
#include "navi/Search/RespCityInfoVRResult.pb.h"
#include "navi/Search/RespStreetInfoVRResult.pb.h"
#include "navi/Search/RespHouseNoInfoVRResult.pb.h"

using namespace navi::Search;

/**
* VR_NaviQueryLocationInfo_Test
*
* The class is just for VR_NaviQueryLocationInfo test.
*/
class VR_NaviQueryLocationInfo_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    static spVR_DialogEngineIF m_sp_EngineIF;
    static spVR_ActionEventSender m_sp_eventSender;
    static spVR_AvcLanSenderIF m_sp_avcLanSender;

    VR_NaviQueryLocationInfo *m_vr_queryLocation1;
    VR_NaviQueryLocationInfo *m_vr_queryLocation2;
    VR_NaviQueryLocationInfo *m_vr_queryLocation3;
    VR_NaviQueryLocationInfo *m_vr_queryLocation4;
    VR_NaviQueryLocationInfo *m_vr_queryLocation5;
    VR_NaviQueryLocationInfo *m_vr_queryLocation6;
};

spVR_DialogEngineIF VR_NaviQueryLocationInfo_Test::m_sp_EngineIF = boost::shared_ptr<VR_DialogEngineIF>(VR_new VR_DialogEngineNull());
spVR_ActionEventSender VR_NaviQueryLocationInfo_Test::m_sp_eventSender(VR_new VR_ActionEventSender(m_sp_EngineIF));
spVR_AvcLanSenderIF VR_NaviQueryLocationInfo_Test::m_sp_avcLanSender = NULL;

void VR_NaviQueryLocationInfo_Test::SetUp()
{
    // Query default info
    int taskId1 = 1;
    std::string msg1 = "<action agent = \"navi\" op = \"requestDefaultInfo\">"
                       "</action>";
    m_vr_queryLocation1 = VR_new VR_NaviQueryLocationInfo(m_sp_eventSender, taskId1, 26, msg1, m_sp_avcLanSender);

    // Query duplicative state
    int taskId2 = 2;
    std::string msg2 = "<action agent = \"navi\" op = \"queryStateInfo\">"
                           "<languageId>0</languageId>"
                           "<list>"
                               "<header>"
                                   "<pageSize>''</pageSize>"
                                   "<startIndex>''</startIndex>"
                                   "<count>1</count>"
                               "</header>"
                               "<items>"
                                   "<item>"
                                       "<addressId>"
                                           "<zoneId>4629770785681047552</zoneId>"
                                           "<cityId>0</cityId>"
                                           "<streetId>0</streetId>"
                                           "<houseNumber>0</houseNumber>"
                                       "</addressId>"
                                   "</item>"
                               "</items>"
                           "</list>"
                       "</action>";
    m_vr_queryLocation2 = VR_new VR_NaviQueryLocationInfo(m_sp_eventSender, taskId2, 8, msg2, m_sp_avcLanSender);

    // Query duplicative city
    int taskId3 = 3;
    std::string msg3 = "<action agent = \"navi\" op = \"queryCityInfo\">"
                           "<languageId>0</languageId>"
                           "<list>"
                               "<header>"
                                   "<pageSize>''</pageSize>"
                                   "<startIndex>''</startIndex>"
                                   "<count>1</count>"
                               "</header>"
                               "<items>"
                                   "<item>"
                                       "<addressId>"
                                           "<zoneId>4629770785681047552</zoneId>"
                                           "<cityId>4629770787828531200</cityId>"
                                           "<streetId>0</streetId>"
                                           "<houseNumber>0</houseNumber>"
                                       "</addressId>"
                                   "</item>"
                               "</items>"
                           "</list>"
                       "</action>";
    m_vr_queryLocation3 = VR_new VR_NaviQueryLocationInfo(m_sp_eventSender, taskId3, 7, msg3, m_sp_avcLanSender);

    // Query duplicative street
    int taskId4 = 4;
    std::string msg4 = "<action agent = \"navi\" op = \"queryStreetInfo\">"
                           "<languageId>0</languageId>"
                           "<list>"
                               "<header>"
                                   "<pageSize>''</pageSize>"
                                   "<startIndex>''</startIndex>"
                                   "<count>1</count>"
                               "</header>"
                               "<items>"
                                   "<item>"
                                       "<addressId>"
                                           "<zoneId>4629770785681047552</zoneId>"
                                           "<cityId>4629770787828531200</cityId>"
                                           "<streetId>186206</streetId>"
                                           "<houseNumber>0</houseNumber>"
                                       "</addressId>"
                                   "</item>"
                               "</items>"
                           "</list>"
                       "</action>";
    m_vr_queryLocation4 = VR_new VR_NaviQueryLocationInfo(m_sp_eventSender, taskId4, 6, msg4, m_sp_avcLanSender);

    // Query search address
    int taskId5 = 5;
    std::string msg5 = "<action agent = \"navi\" op = \"queryAddressInfo\">"
                           "<languageId>0</languageId>"
                           "<list>"
                               "<header>"
                                   "<pageSize>''</pageSize>"
                                   "<startIndex>''</startIndex>"
                                   "<count>1</count>"
                               "</header>"
                               "<items>"
                                   "<item>"
                                       "<addressInfo>"
                                           "<zoneId>4629770785681047552</zoneId>"
                                           "<country>''</country>"
                                           "<zone>''</zone>"
                                           "<cityId>4629770787828531200</cityId>"
                                           "<city>''</city>"
                                           "<streetExist>true</streetExist>"
                                           "<streetId>186206</streetId>"
                                           "<street>''</street>"
                                           "<houseNumberExist>true</houseNumberExist>"
                                           "<houseNumber>20</houseNumber>"
                                       "</addressInfo>"
                                   "</item>"
                               "</items>"
                           "</list>"
                       "</action>";
    m_vr_queryLocation5 = VR_new VR_NaviQueryLocationInfo(m_sp_eventSender, taskId5, 9, msg5, m_sp_avcLanSender);

    // Query show POI icon number
    int taskId6 = 6;
    std::string msg6 = "<action agent = \"navi\" op = \"queryShowingPOIIconNumber\">"
                       "</action>";
    m_vr_queryLocation6 = VR_new VR_NaviQueryLocationInfo(m_sp_eventSender, taskId6, 16, msg6, m_sp_avcLanSender);
}

void VR_NaviQueryLocationInfo_Test::TearDown()
{
    if (NULL != m_vr_queryLocation1) {
        delete m_vr_queryLocation1;
        m_vr_queryLocation1 = NULL;
    }
    if (NULL != m_vr_queryLocation2) {
        delete m_vr_queryLocation2;
        m_vr_queryLocation2 = NULL;
    }
    if (NULL != m_vr_queryLocation3) {
        delete m_vr_queryLocation3;
        m_vr_queryLocation3 = NULL;
    }
    if (NULL != m_vr_queryLocation4) {
        delete m_vr_queryLocation4;
        m_vr_queryLocation4 = NULL;
    }
    if (NULL != m_vr_queryLocation5) {
        delete m_vr_queryLocation5;
        m_vr_queryLocation5 = NULL;
    }
    if (NULL != m_vr_queryLocation6) {
        delete m_vr_queryLocation6;
        m_vr_queryLocation6 = NULL;
    }
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_Constructor)
{
    SUCCEED();
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_Run)
{
    VR_LOGD_FUNC();
    m_vr_queryLocation1->Run();
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_RunAction)
{
    VR_LOGD_FUNC();
    m_vr_queryLocation1->RunAction();
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_NaviQueryDefaultInfo)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc1;
    doc1.load((m_vr_queryLocation1->m_ReqMsg).c_str());
    m_vr_queryLocation1->NaviQueryDefaultInfo(doc1);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_NaviQueryStateInfo)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc2;
    doc2.load((m_vr_queryLocation2->m_ReqMsg).c_str());
    m_vr_queryLocation2->NaviQueryStateInfo(doc2);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_NaviQueryCityInfo)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc3;
    doc3.load((m_vr_queryLocation3->m_ReqMsg).c_str());
    m_vr_queryLocation3->NaviQueryCityInfo(doc3);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_NaviQueryStreetInfo)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc4;
    doc4.load((m_vr_queryLocation4->m_ReqMsg).c_str());
    m_vr_queryLocation4->NaviQueryStreetInfo(doc4);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_NaviQueryAddressInfo)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc5;
    doc5.load((m_vr_queryLocation5->m_ReqMsg).c_str());
    m_vr_queryLocation5->NaviQueryAddressInfo(doc5);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_NaviQueryShowPOIIconNo)
{
    VR_LOGD_FUNC();
    pugi::xml_document doc6;
    doc6.load((m_vr_queryLocation6->m_ReqMsg).c_str());
    m_vr_queryLocation6->NaviQueryShowPOIIconNo(doc6);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_OnReceive)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespStateInfoVRResult> proto2;
    proto2.reset(RespStateInfoVRResult::default_instance().New());
    proto2->set_errorcode(0);
    proto2->set_errortype(SearchDefine_ERROR_TYPE_NO_ERROR);
    RespStateInfoVRResult_StateInfo *pStateInfo = proto2->add_stateinfo();
    pStateInfo->set_stateid("4629770785681047552");
    pStateInfo->set_statename("Australian Capital Territory");
    pStateInfo->set_city_exist(true);
    SearchDefine_MapLonLat *pLonLat2 = pStateInfo->mutable_lonlat();
    pLonLat2->set_lon(137439494);
    pLonLat2->set_lat(-32514769);
    proto2->set_senderid(2);
    m_vr_queryLocation2->OnReceive(*proto2);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_OnMessageNaviReqDfltInfoResp)
{
    VR_LOGD_FUNC();
    std::unique_ptr<DefaultInfoResult> proto1;
    proto1.reset(DefaultInfoResult::default_instance().New());
    proto1->set_errorcode(0);
    proto1->set_errortype(SearchDefine_ERROR_TYPE_NO_ERROR);
    DefaultInfoResult_DefaultArea *pDfltArea = proto1->mutable_deafultarea();
    pDfltArea->set_name("Australian Capital Territory");
    pDfltArea->set_zoneid("4629770785681047552");
    pDfltArea->set_highzoneid("4629770785681047552");
    SearchDefine_MapLonLat* pLonLat1 = pDfltArea->mutable_centerlonlat();
    pLonLat1->set_lon(137439494);
    pLonLat1->set_lat(-32514769);
    pDfltArea->set_tel_code("86");
    pDfltArea->set_postcode_exist(true);
    pDfltArea->set_street_exist(true);
    pDfltArea->set_countryid(1);
    proto1->set_senderid(1);
    m_vr_queryLocation1->OnMessageNaviReqDfltInfoResp(*proto1);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_OnMessageNaviSearchStateResp)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespStateInfoVRResult> proto2;
    proto2.reset(RespStateInfoVRResult::default_instance().New());
    proto2->set_errorcode(0);
    proto2->set_errortype(SearchDefine_ERROR_TYPE_NO_ERROR);
    RespStateInfoVRResult_StateInfo *pStateInfo = proto2->add_stateinfo();
    pStateInfo->set_stateid("4629770785681047552");
    pStateInfo->set_statename("Australian Capital Territory");
    pStateInfo->set_city_exist(true);
    SearchDefine_MapLonLat *pLonLat2 = pStateInfo->mutable_lonlat();
    pLonLat2->set_lon(137439494);
    pLonLat2->set_lat(-32514769);
    proto2->set_senderid(2);
    m_vr_queryLocation2->OnMessageNaviSearchStateResp(*proto2);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_OnMessageNaviSearchCityResp)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespCityInfoVRResult> proto3;
    proto3.reset(RespCityInfoVRResult::default_instance().New());
    proto3->set_errorcode(0);
    proto3->set_errortype(SearchDefine_ERROR_TYPE_NO_ERROR);
    RespCityInfoVRResult_CityInfo *pCityInfo = proto3->add_cityinfo();
    pCityInfo->set_stateid("4629770785681047552");
    pCityInfo->set_statename("Australian Capital Territory");
    pCityInfo->set_cityid("4629770787828531200");
    pCityInfo->set_cityname("Canberra");
    pCityInfo->set_highcityname("canberra");
    pCityInfo->set_street_exist(true);
    SearchDefine_MapLonLat *pLonLat3 = pCityInfo->mutable_lonlat();
    pLonLat3->set_lon(137439494);
    pLonLat3->set_lat(-32514769);
    SearchDefine_MapLonLat *pGuideLonLat3 = pCityInfo->mutable_guidelonlat();
    pGuideLonLat3->set_lon(137439494);
    pGuideLonLat3->set_lat(-32514769);
    proto3->set_senderid(3);
    m_vr_queryLocation3->OnMessageNaviSearchCityResp(*proto3);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_OnMessageNaviSearchStreetResp)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespStreetInfoVRResult> proto4;
    proto4.reset(RespStreetInfoVRResult::default_instance().New());
    proto4->set_errorcode(0);
    proto4->set_errortype(SearchDefine_ERROR_TYPE_NO_ERROR);
    RespStreetInfoVRResult_StreetInfo *pStreetInfo = proto4->add_streetinfo();
    pStreetInfo->set_stateid("4629770785681047552");
    pStreetInfo->set_statename("Australian Capital Territory");
    pStreetInfo->set_cityid("4629770787828531200");
    pStreetInfo->set_cityname("Canberra");
    pStreetInfo->set_cityname1("");
    pStreetInfo->set_cityname2("Australian Capital Territory");
    pStreetInfo->set_cityname3("Canberra");
    pStreetInfo->set_cityname4("Dunlop");
    pStreetInfo->set_streetid("186206");
    pStreetInfo->set_streetname("Akubra Place");
    pStreetInfo->set_houseno_exist(true);
    SearchDefine_MapLonLat *pLonLat4 = pStreetInfo->mutable_lonlat();
    pLonLat4->set_lon(137334450);
    pLonLat4->set_lat(-32438082);
    SearchDefine_MapLonLat *pGuideLonLat4 = pStreetInfo->mutable_guidelonlat();
    pGuideLonLat4->set_lon(137334450);
    pGuideLonLat4->set_lat(-32438082);
    proto4->set_senderid(4);
    m_vr_queryLocation4->OnMessageNaviSearchStreetResp(*proto4);
}

TEST_F(VR_NaviQueryLocationInfo_Test, Test_OnMessageNaviSearchHouseNoResp)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespHouseNoInfoVRResult> proto5;
    proto5.reset(RespHouseNoInfoVRResult::default_instance().New());
    proto5->set_errorcode(0);
    proto5->set_errortype(SearchDefine_ERROR_TYPE_NO_ERROR);
    RespHouseNoInfoVRResult_HouseNoInfo *pHouseNoInfo = proto5->add_housenoinfo();
    pHouseNoInfo->set_stateid("4629770785681047552");
    pHouseNoInfo->set_statename("Australian Capital Territory");
    pHouseNoInfo->set_cityid("4629770787828531200");
    pHouseNoInfo->set_cityname("Canberra");
    pHouseNoInfo->set_cityname1("");
    pHouseNoInfo->set_cityname2("Australian Capital Territory");
    pHouseNoInfo->set_cityname3("Canberra");
    pHouseNoInfo->set_cityname4("Dunlop");
    pHouseNoInfo->set_streetid("186206");
    pHouseNoInfo->set_streetname("Akubra Place");
    pHouseNoInfo->set_houseno("20");
    SearchDefine_MapLonLat *pLonLat5 = pHouseNoInfo->mutable_lonlat();
    pLonLat5->set_lon(137334450);
    pLonLat5->set_lat(-32438082);
    SearchDefine_MapLonLat *pGuideLonLat5 = pHouseNoInfo->mutable_guidelonlat();
    pGuideLonLat5->set_lon(137334450);
    pGuideLonLat5->set_lat(-32438082);
    proto5->set_senderid(5);
    m_vr_queryLocation5->OnMessageNaviSearchHouseNoResp(*proto5);
}

/* EOF */
