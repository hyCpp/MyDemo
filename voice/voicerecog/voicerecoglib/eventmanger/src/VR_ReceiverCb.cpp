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

#include "stdafx.h"
#include "VR_Def.h"

#include <boost/bind.hpp>
#include <boost/assign.hpp>
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"

#include "VR_ReceiverCb.h"
#include "VR_MessageProcessIF.h"
#include "EV_EventReceiver.h"
#include "VR_SettingListener.h"
#include "ncore/NCUri.h"
#include "VR_ContentProvider.h"
#include "VR_Configure.h"
#include "VR_NCTutorialManager.h"

#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCreateSession.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrOpenSession.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCloseSession.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrStartRecognition.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCancelRecognition.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrReqOperation.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrPVRFlag.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifySessionClosed.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyActionResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrTSL.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyUICrash.pb.h"
#include "navi/Voice/TTS/TtsRespServiceStatus.pb.h"
#include "navi/Traffic/WeatherCityListRes.pb.h"
#include "navi/Traffic/WeatherCityListUpdate.pb.h"
#include "navi/Traffic/WeatherInfoforVRRes.pb.h"
#include "navi/Traffic/WeatherDataServiceRes.pb.h"
#include "navi/Traffic/WeatherDataServiceUpdate.pb.h"

#include "navi/Location/LocInfoUpdate.pb.h"
#include "navi/Search/SpotlightSearchResult.pb.h"

#include "navi/Search/SearchModuleStatus.pb.h"

#include "navi/Voice/VoiceRecog/RequestSDLVr.pb.h"
#include "navi/Voice/VoiceRecog/RequestMMVr.pb.h"
#include "navi/Voice/VoiceRecog/RequestVriAutoProxy.pb.h"
#include "navi/NaviMain/NaviStatus.pb.h"
#include "navi/Guide/GudGuideStart.pb.h"
#include "navi/Guide/GudGuideAbortFinish.pb.h"
#include "navi/Path/RouteDetailInfo.pb.h"
#include "navi/Path/IVIGuideRouteInfoUpdate.pb.h"
#include "navi/Path/BackGroundRouteInfoUpdate.pb.h"
#include "navi/Map/MapInfo.pb.h"
#include "navi/Guide/GudETAListInfo.pb.h"
#include "navi/Point/PointRequest.pb.h"
#include "navi/Point/PointResponse.pb.h"
#include "navi/Search/RespCategorySearchVRResult.pb.h"



#if defined(VR_DM_TARGET_VRUNIT)
#include "navi/Search/RespStateInfoVRResult.pb.h"
#include "navi/Search/RespCityInfoVRResult.pb.h"
#include "navi/Search/RespStreetInfoVRResult.pb.h"
#include "navi/Search/RespHouseNoInfoVRResult.pb.h"
#include "navi/Search/DefaultInfoResult.pb.h"
#include "navi/Path/GuideStatus.pb.h"
#include "navi/Path/PathPointList.pb.h"
#include "navi/Path/NearLinkResult.pb.h"
#include "navi/Path/NotifyRouteStatus.pb.h"
#include "navi/Point/PointResponse.pb.h"
#include "navi/Location/LocInfoUpdate.pb.h"

#include "navi/Search/RespStateInfoVRResultCN.pb.h"
#include "navi/Search/RespCityInfoVRResultCN.pb.h"
#include "navi/Search/RespDistrictInfoVRResultCN.pb.h"
#include "navi/Search/RespStreetInfoVRResultCN.pb.h"
#include "navi/Search/RespHouseNoInfoVRResultCN.pb.h"
#include "navi/Search/RespAddressInfoVRResultCN.pb.h"
#include "navi/Search/RespPOINameListVRResultCN.pb.h"

using namespace navi::Search;
using namespace navi::Path;
using namespace navi::Point;
using namespace navi::Location;

#endif

#include "navi/NaviInfo/NaviInfoRespPoiAddressBook.pb.h"
#include "navi/NaviInfo/NaviInfoRespPoiTime.pb.h"
#include "navi/NaviInfo/NaviInfoRespResult.pb.h"
#include "navi/NaviInfo/NaviInfoRespNavigationStatus.pb.h"
#include "navi/NaviInfo/NaviInfoRespCarPositionInfo.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::TTS;
using namespace navi::Search;
using namespace navi::NaviInfo;



const VR_ReceiverCb::NameHandleMap VR_ReceiverCb::s_MsgHandleMap = boost::assign::map_list_of
        (EV_EVENT_VOICERECOG_REQUESTVRCREATESESSION,        &VR_ReceiverCb::OnReqestGetSeessionID)
        (EV_EVENT_VOICERECOG_REQUESTVROPENSESSION,          &VR_ReceiverCb::OnReqestOpenSeession)
        (EV_EVENT_VOICERECOG_REQUESTVRCLOSESESSION,         &VR_ReceiverCb::OnReqestCloseSeession)
        (EV_EVENT_VOICERECOG_REQUESTVRSTARTRECOGNITION,     &VR_ReceiverCb::OnReqestStartDialog)
        (EV_EVENT_VOICERECOG_REQUESTVRCANCELRECOGNITION,    &VR_ReceiverCb::OnReqestCancleDialog)
        (EV_EVENT_VOICERECOG_REQUESTVRREQOPERATION,         &VR_ReceiverCb::OnReqestUserOperation)
        (EV_EVENT_VOICERECOG_REQUESTVRCOMMONPROPERTY,       &VR_ReceiverCb::OnReqestCommonProperty)
        (EV_EVENT_VOICERECOG_REQUESTVRPVRFLAG,              &VR_ReceiverCb::OnReqestVrPVRFlag)
        (EV_EVENT_VOICERECOG_REQUESTVRVOICETAG,             &VR_ReceiverCb::OnReqestVrVoiceTag)
        (EV_EVENT_VOICERECOG_REQUESTVRTSL,                  &VR_ReceiverCb::OnReqestVrTsl)
/******** Manual VR *********/
        (EV_EVENT_VOICERECOG_REQUESTMMVR,                   &VR_ReceiverCb::OnMMVRRequestCommonProperty)
/********* SDL VR ************/
        (EV_EVENT_VOICERECOG_REQUESTSDLVR,                  &VR_ReceiverCb::OnSDLVRRequestCommonProperty)
/********* vui    ************/
        (EV_EVENT_VOICERECOG_REQUESTVRIAUTOPROXY,           &VR_ReceiverCb::OnRequestVriAutoProxy);


const VR_ReceiverCb::NameInnerHandleMap VR_ReceiverCb::s_MsgInnerHandleMap = boost::assign::map_list_of
        (EV_EVENT_VOICERECOG_VRLOOPBACK,                    &VR_ReceiverCb::OnMessageLoopBack)
        (EV_EVENT_VOICERECOG_NOTIFYSESSIONCLOSED,           &VR_ReceiverCb::OnMessageSessionClosed)
        (EV_EVENT_VOICERECOG_VRNOTIFYACTIONRESULT,          &VR_ReceiverCb::OnReplyActoinResult)
        (EV_EVENT_VOICERECOG_VRNOTIFYUICRASH,               &VR_ReceiverCb::OnNotifyUICrash)
        // (EV_EVENT_TTS_ONPLAYEND,                            &VR_ReceiverCb::OnMessageTtsPlayEnd)
        (EV_EVENT_TTS_PLAYSTATE_RESP,                       &VR_ReceiverCb::OnMessageTtsPlayState)
        (EV_EVENT_SEARCH_DEFAULTINFORESULT,                 &VR_ReceiverCb::OnMessageNaviReqDfltInfoResp)
        (EV_EVENT_SEARCH_RESPSTATEINFOVRRESULT,             &VR_ReceiverCb::OnMessageNaviSearchStateResp)
        (EV_EVENT_SEARCH_RESPCITYINFOVRRESULT,              &VR_ReceiverCb::OnMessageNaviSearchCityResp)
        (EV_EVENT_SEARCH_RESPSTREETINFOVRRESULT,            &VR_ReceiverCb::OnMessageNaviSearchStreetResp)
        (EV_EVENT_SEARCH_RESPHOUSENOINFOVRRESULT,           &VR_ReceiverCb::OnMessageNaviSearchHouseNoResp)

        (EV_EVENT_SEARCH_RESPSTATEINFOVRRESULTCN,           &VR_ReceiverCb::OnMessageNaviSearchStateRespCN)
        (EV_EVENT_SEARCH_RESPCITYINFOVRRESULTCN,            &VR_ReceiverCb::OnMessageNaviSearchCityRespCN)
        (EV_EVENT_SEARCH_RESPDISTRICTINFOVRRESULTCN,        &VR_ReceiverCb::OnMessageNaviSearchDistrictRespCN)
        (EV_EVENT_SEARCH_RESPSTREETINFOVRRESULTCN,          &VR_ReceiverCb::OnMessageNaviSearchStreetRespCN)
        (EV_EVENT_SEARCH_RESPHOUSENOINFOVRRESULTCN,         &VR_ReceiverCb::OnMessageNaviSearchHouseNoRespCN)
        (EV_EVENT_SEARCH_RESPADDRESSINFOVRRESULTCN,         &VR_ReceiverCb::OnMessageNaviSearchAddressRespCN)

        (EV_EVENT_SEARCH_RESPCATEGORYSEARCHVRRESULT,        &VR_ReceiverCb::OnMessageNaviSearchPoiInfoResp)
        (EV_EVENT_PATH_GUIDESTATUS,                         &VR_ReceiverCb::OnMessageNaviQueryRouteExistResp)
        (EV_EVENT_PATH_NEARLINKRESULT,                      &VR_ReceiverCb::OnMessageNaviQueryPointTypeResp)
        (EV_EVENT_PATH_PATHPOINTLIST,                       &VR_ReceiverCb::OnMessageNaviPathPointListNotify)
        (EV_EVENT_LOCATION_LOCINFOUPDATE,                   &VR_ReceiverCb::OnMessageNaviQueryCurrentPosResp)
        (EV_EVENT_SEARCH_RESPPOINAMELISTVRRESULTCN,         &VR_ReceiverCb::OnMessageNaviQueryPOIByNameRespCN)
        (EV_EVENT_TRAFFIC_WEATHERCITYLISTRES,               &VR_ReceiverCb::OnMessageWeatherCityListResp)
        (EV_EVENT_TRAFFIC_WEATHERCITYLISTUPDATE,            &VR_ReceiverCb::OnMessageNotifyWeatherCityList)
        (EV_EVENT_TRAFFIC_WEATHERINFOFORVRRES,              &VR_ReceiverCb::OnMessageWeatherInfoForVRResp)
        (EV_EVENT_TRAFFIC_WEATHERDATASERVICERES,            &VR_ReceiverCb::OnMessageWeatherDataServiceRes)
        (EV_EVENT_TRAFFIC_WEATHERDATASERVICEUPDATE,         &VR_ReceiverCb::OnMessageWeatherDataServiceUpdate)
        (EV_EVENT_SEARCH_SPOTLIGHTSEARCHRESULT,             &VR_ReceiverCb::OnMessageSpotlightSearchResult)
        (EV_EVENT_SEARCH_SEARCHMODULESTATUS,                &VR_ReceiverCb::OnMessageSearchModuleStatus)
        (EV_EVENT_TTS_RESPSERVICESTATUS,                    &VR_ReceiverCb::OnMessageTtsServiceStatus)
        (EV_EVENT_NAVIMAIN_NAVISTATUS,                      &VR_ReceiverCb::OnMessageNaviMapOn)
        (EV_EVENT_GUIDE_START,                              &VR_ReceiverCb::OnMessageNaviGuideStart)
        (EV_EVENT_GUIDE_ABORT_FINISH,                       &VR_ReceiverCb::OnMessageNaviGuideFinish)
//        (EV_EVENT_PATH_GUIDEROUTEINFOUPDATE,                &VR_ReceiverCb::OnMessageNaviRouteInfo)
        (EV_EVENT_MAP_MAPINFO,                              &VR_ReceiverCb::OnMessageNaviMapInfo)
        (EV_EVENT_MAP_MAPSCREENINFO,                        &VR_ReceiverCb::OnMessageNaviMapScreenInfo)
        (EV_EVENT_GUIDE_ETALISTINFO,                        &VR_ReceiverCb::OnMessageNaviGudETAListInfo)
        (EV_EVENT_POINT_POINTRESPONSE,                      &VR_ReceiverCb::OnMessageNaviAddrLocation)
        (EV_EVENT_PATH_BACKGROUNDROUTEINFOUPDATE,           &VR_ReceiverCb::OnMessageNaviBackgroundRoundInfo)
        (EV_EVENT_MAP_NEIGHBORNAME,                         &VR_ReceiverCb::OnMessageNaviMapNeighborName)

        // WUKONG
        (EV_EVENT_NAVIINFO_RESPPOIADDRESSBOOK,              &VR_ReceiverCb::OnMessageNaviInfoRespAddressBook)
        (EV_EVENT_NAVIINFO_RESPPOITIME,                     &VR_ReceiverCb::OnMessageNaviInfoRespPoiTime)
        (EV_EVENT_NAVIINFO_RESPRESULT,                      &VR_ReceiverCb::OnMessageNaviInfoRespResult)
        (EV_EVENT_NAVIINFO_RESPNAVISTATUS,                  &VR_ReceiverCb::OnMessageNaviInfoRespNaviStatus)
        (EV_EVENT_NAVIINFO_RESPCARPOSITIONINFO,             &VR_ReceiverCb::OnMessageNaviInfoRespCarPositionInfo)
        ;


VR_ReceiverCb::VR_ReceiverCb(spVR_MessageProcessIF process, spVR_CommandFactory spVrCommandFactory, spVR_DM_CommandHandle pCmdHandle)
    : m_spEvtRecv(VR_new EV_EventReceiver(EV_MODULE_VOICERECOG))
    , m_spMessageProcess(process)
#ifdef HAVE_NUTSHELL_OS
    , m_spRunableThread(VR_new VR_RunableThread(VR_VOICESETTINGLISTENER.c_str()))
    , m_spVoiceSettingListener(VR_new  VR_VoiceSettingListener(VR_VOICESETTINGLISTENER.c_str(), m_spRunableThread->getRunableLooper(),
                                                               process, spVrCommandFactory, pCmdHandle))
#endif
    , m_spDMCommandHandler(pCmdHandle)
    , m_spCommandFactory(spVrCommandFactory)  //  parameter list'order is the same as class member'order
    , m_nLocalAddrCount(1)
{
    VR_LOGD_FUNC();
}

VR_ReceiverCb::~VR_ReceiverCb()
{
    VR_LOGD_FUNC();
}

void VR_ReceiverCb::StartReceive()
{
    VR_LOGD_FUNC();
    Register();
    m_spEvtRecv->StartReceive();  // VR_ReceiverCb start receive ,  vr start over.
    // setting item listener regist
#ifdef HAVE_NUTSHELL_OS
    m_spVoiceSettingListener->Regist();
#endif
    this->StartOver();
    this->NotifyFullUpdate();
}

void VR_ReceiverCb::StartOver()
{
    VR_LOGD_FUNC();

    EV_EventContext context;
    std::unique_ptr<VrRequestSettingProperty> reqNotifyAll(VrRequestSettingProperty::default_instance().New());
    reqNotifyAll->set_type(VrDMVRSettingType_NotifyAll);
    VrRequestNotifyAll *pRequestNotifyAll = reqNotifyAll->mutable_notifyall();
    pRequestNotifyAll->set_notifyall(true);
    OnReqestVrSettingState(context, reqNotifyAll.release());

    // DE notify
    std::unique_ptr<VrRequestSettingProperty> reqStartedNotify(VrRequestSettingProperty::default_instance().New());
    reqStartedNotify->set_type(VrDMVRSettingType_StartedNotify);
    OnReqestVrSettingState(context, reqStartedNotify.release());
}

void VR_ReceiverCb::NotifyFullUpdate()
{
#ifdef HAVE_NUTSHELL_OS
    VR_ContentProvider vr_cp;
    std::string strMode = vr_cp.GetContentProviderValue(VR_FULLUPDATE_STATUS);
    VR_LOGD("strMode = [%s]", strMode.c_str());
    if ("navifulldata" == strMode) {
        EV_EventContext context;
        std::unique_ptr<VrRequestSettingProperty>  reqFullUpdate(VrRequestSettingProperty::default_instance().New());
        reqFullUpdate->set_type(VrDMVRSettingType_FullUpdate);
        VrDataFullUpdata* pFullUpdate = reqFullUpdate->mutable_full_update_status();
        pFullUpdate->set_fullupdatestatus(strMode);
        OnReqestVrSettingState(context, reqFullUpdate.release());
    }
#endif
}

void VR_ReceiverCb::StopReceive()
{
    VR_LOGD_FUNC();
    // setting item listener unregist
#ifdef HAVE_NUTSHELL_OS
    m_spVoiceSettingListener->UnRegist();
#endif

    m_spEvtRecv->StopReceive();
    Unregister();

    // 注销监听之后，停止Thread
#ifdef HAVE_NUTSHELL_OS
    m_spRunableThread->stopThread();
#endif
}

void VR_ReceiverCb::Register()
{
    VR_LOGD_FUNC();

    spEV_EventReceiver receiver = m_spEvtRecv;
    VR_ReceiverCb * pObj = this;

    std::for_each(s_MsgHandleMap.begin(), s_MsgHandleMap.end(),
    ([receiver, pObj](cNameHandleMapRef i) {
        if (!(i.first.empty())) {
            receiver->SubscribeWithContext(i.first.c_str(), boost::bind(i.second, pObj, _1, _2));
        }
    }));

    std::for_each(s_MsgInnerHandleMap.begin(), s_MsgInnerHandleMap.end(),
    ([receiver, pObj](cNameInnerHandleMapRef i) {
        if (!(i.first.empty())) {
            receiver->Subscribe(i.first.c_str(), boost::bind(i.second, pObj, _1));
        }
    }));
}

void VR_ReceiverCb::Unregister()
{
    VR_LOGD_FUNC();

    spEV_EventReceiver receiver = m_spEvtRecv;

    std::for_each(s_MsgHandleMap.begin(), s_MsgHandleMap.end(),
    ([receiver](cNameHandleMapRef i) {
        receiver->UnSubscribe(i.first.c_str());
    }));

    std::for_each(s_MsgInnerHandleMap.begin(), s_MsgInnerHandleMap.end(),
    ([receiver](cNameInnerHandleMapRef i) {
        receiver->UnSubscribe(i.first.c_str());
    }));
}

/************************** Manual VR *******************************************/
void VR_ReceiverCb::OnMMVRRequestCommonProperty(const EV_EventContext& context, VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestMMVr>  requestMMVR(new RequestMMVr);
    requestMMVR->CheckTypeAndMergeFrom(*(static_cast<RequestMMVr*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateMMVRRequestCommonProperty(pcContext,
                                                                         requestMMVR.release(),
                                                                         m_spMessageProcess
                                                                         );
    m_spDMCommandHandler->onCommand(pcCommand);
}

/************************** SDL VR *************************************************/
void VR_ReceiverCb::OnSDLVRRequestCommonProperty(const EV_EventContext& context, VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestSDLVr> requestSDLVr(new RequestSDLVr);
    requestSDLVr->CheckTypeAndMergeFrom(*(static_cast<RequestSDLVr*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateSDLVRRequestCommonProperty(pcContext,
                                                                          requestSDLVr.release(),
                                                                          m_spMessageProcess
                                                                         );
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnRequestVriAutoProxy(const EV_EventContext &context, VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVriAutoProxy> pRequestVriAutoProxy(new RequestVriAutoProxy);
    pRequestVriAutoProxy->CheckTypeAndMergeFrom(*(static_cast<RequestVriAutoProxy*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRequestVriAutoProxy(pcContext,
                                                                          pRequestVriAutoProxy.release(),
                                                                          m_spMessageProcess
                                                                         );
    m_spDMCommandHandler->onCommand(pcCommand);
}


void VR_ReceiverCb::OnMessageSpotlightSearchResult(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<SpotlightSearchResult>  spotSearchResult(VR_new SpotlightSearchResult);
    spotSearchResult->CheckTypeAndMergeFrom(*(static_cast<SpotlightSearchResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateSpotlightSearchResult(spotSearchResult.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void  VR_ReceiverCb::OnMessageSearchModuleStatus(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<SearchModuleStatus>  searchModuleStatus(VR_new  SearchModuleStatus);
    searchModuleStatus->CheckTypeAndMergeFrom(*(static_cast<SearchModuleStatus*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateSearchModuleStatus(searchModuleStatus.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestGetSeessionID(const EV_EventContext& context, VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    // VR_LOGP("GetSession");
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrCreateSession> createSession(VR_new RequestVrCreateSession);
    createSession->CheckTypeAndMergeFrom(*(static_cast<RequestVrCreateSession*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRequestGetSessionID(pcContext, createSession.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestOpenSeession(const EV_EventContext& context, VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    // VR_LOGP("OpenSession");
    // VR_LOGP("Session: Recive SessionOpenReq From FC Case:215-1 215-2");
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrOpenSession> openSession(VR_new RequestVrOpenSession);
    openSession->CheckTypeAndMergeFrom(*(static_cast<RequestVrOpenSession*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRequestOpenSession(pcContext, openSession.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestCloseSeession(const EV_EventContext& context, VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrCloseSession> closeSession(VR_new RequestVrCloseSession);
    closeSession->CheckTypeAndMergeFrom(*(static_cast<RequestVrCloseSession*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRequestCloseSession(pcContext, closeSession.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestStartDialog(const EV_EventContext& context, VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    // VR_LOGP("StartDialog");
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrStartRecognition> startDailog(VR_new RequestVrStartRecognition);
    startDailog->CheckTypeAndMergeFrom(*(static_cast<RequestVrStartRecognition*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateReqestStartDialog(pcContext, startDailog.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestCancleDialog(const EV_EventContext& context, VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrCancelRecognition> cancelDailog(VR_new RequestVrCancelRecognition);
    cancelDailog->CheckTypeAndMergeFrom(*(static_cast<RequestVrCancelRecognition*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateReqestCancleDialog(pcContext, cancelDailog.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestUserOperation(const EV_EventContext& context, VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrReqOperation> usrOperation(VR_new RequestVrReqOperation);
    usrOperation->CheckTypeAndMergeFrom(*(static_cast<RequestVrReqOperation*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateReqestUserOperation(pcContext, usrOperation.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestStartChangeLanguage(const EV_EventContext& context, VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
}

void VR_ReceiverCb::OnReqestCommonProperty(const EV_EventContext &context, VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    // VR_LOGP("PTT:Recive PTT PressDown Notify action case: 215-1");
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrCommonProperty> requestProperty(VR_new RequestVrCommonProperty);
    requestProperty->CheckTypeAndMergeFrom(*(static_cast<RequestVrCommonProperty*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRequestCommonProperty(pcContext, requestProperty.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestVrPVRFlag(const EV_EventContext &context, VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrPVRFlag> requestPVRFlag(VR_new RequestVrPVRFlag);
    requestPVRFlag->CheckTypeAndMergeFrom(*(static_cast<RequestVrPVRFlag*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateReqestVrPVRFlag(pcContext, requestPVRFlag.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}


void VR_ReceiverCb::OnReqestVrVoiceTag(const EV_EventContext &context, VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrVoiceTag> requestVoiceTag(VR_new RequestVrVoiceTag);
    requestVoiceTag->CheckTypeAndMergeFrom(*(static_cast<RequestVrVoiceTag*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateReqestVrVoiceTag(pcContext, requestVoiceTag.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestVrTsl(const EV_EventContext &context, VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<RequestVrTSL> requestTsl(VR_new RequestVrTSL);
    requestTsl->CheckTypeAndMergeFrom(*(static_cast<RequestVrTSL*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateReqestVrTsl(pcContext, requestTsl.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReqestVrSettingState(const EV_EventContext &context, VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    spEV_EventContext pcContext(VR_new EV_EventContext(context));
    std::unique_ptr<VrRequestSettingProperty> requestVrSet(VR_new VrRequestSettingProperty);
    requestVrSet->CheckTypeAndMergeFrom(*(static_cast<VrRequestSettingProperty*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(pcContext, requestVrSet.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnReplyActoinResult(VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrNotifyActionResult> actionResult(VR_new VrNotifyActionResult);
    actionResult->CheckTypeAndMergeFrom(*(static_cast<VrNotifyActionResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateReplyActoinResult(actionResult.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageTtsPlayEnd(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    TtsOnPlayEnd* pOldMsg = static_cast<TtsOnPlayEnd*> (pMsg);
    if (pOldMsg->sender() == EV_MODULE_VOICERECOG) {
        VrLoopBack* loopMsg(VrLoopBack::default_instance().New());
        if (NULL == loopMsg) {
            VR_LOGE("VrLoopBack::default_instance().New  FAIL!!");
            return;
        }
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *fc2dm = loopMsg->mutable_m_fc2dm();
        fc2dm->set_id(pOldMsg->reqid());
        fc2dm->set_vr_dm_action_type(ActionType_TTS_COMMAND_RESULT);
        VrTTSCommandResult* tts_resp = fc2dm->mutable_tts_result();
        tts_resp->set_func_code(TTSProcessType_Play);
        tts_resp->set_result(pOldMsg->result() == TtsOnPlayEnd_TTS_PLAY_RESULT_TTS_PLAY_RESULT_OK);

        OnMessageLoopBack(loopMsg);
        if (NULL != loopMsg) {
            delete loopMsg;
            loopMsg = NULL;
        }
    }
}

void VR_ReceiverCb::OnMessageTtsPlayState(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    TtsRespPlayState *pOldMsg = static_cast<TtsRespPlayState*>(pMsg);
    if ((pOldMsg->sender() == EV_MODULE_VOICERECOG)
        || (pOldMsg->sender() == VR_NCTUTORIAL_TTS_PLAY_SENDER_NAME)) {

        VrLoopBack* loopMsg(VrLoopBack::default_instance().New());
        if (NULL == loopMsg) {
            VR_LOGE("VrLoopBack::default_instance().New  FAIL!!");
            return;
        }

        loopMsg->set_sender(pOldMsg->sender());
        loopMsg->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
        VrActionFC2DM *fc2dm = loopMsg->mutable_m_fc2dm();
        fc2dm->set_id(pOldMsg->reqid());
        fc2dm->set_vr_dm_action_type(ActionType_TTS_COMMAND_RESULT);
        VrTTSCommandResult* tts_resp = fc2dm->mutable_tts_result();

        TtsRespPlayState_TTS_PLAY_STATE playState = pOldMsg->state();
        VR_LOGI("tts play state id=%d, type=%d", pOldMsg->reqid(), playState);
        switch (playState) {
        case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_START:
        {
            tts_resp->set_func_code(TTSProcessType_Play);
            tts_resp->set_result(true);
            break;
        }
        case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_STOPED:
        {
            tts_resp->set_func_code(TTSProcessType_Stop);
            tts_resp->set_result(true);
            break;
        }
        case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ABORT:
        {
            tts_resp->set_func_code(TTSProcessType_Pause); // stop by DE
            tts_resp->set_result(true);
            break;
        }
        case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR_ID:
        {
            tts_resp->set_func_code(TTSProcessType_Stop);
            tts_resp->set_result(true);
            break;
        }
        case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR_LANG:
        {
            tts_resp->set_func_code(TTSProcessType_Stop);
            tts_resp->set_result(true);
            break;
        }
        case TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR:
        {
            tts_resp->set_func_code(TTSProcessType_Stop);
            tts_resp->set_result(true);
            break;
        }
        default:
        {
            VR_LOGE("playState is  default = %d", playState);
            if (NULL != loopMsg) {
                delete loopMsg;
                loopMsg = NULL;
            }
            return;
        }
        }

        OnMessageLoopBack(loopMsg);
        if (NULL != loopMsg) {
            delete loopMsg;
            loopMsg = NULL;
        }
    }
}

void VR_ReceiverCb::OnMessageLoopBack(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrLoopBack> loopMessage(VR_new VrLoopBack);
    loopMessage->CheckTypeAndMergeFrom(*(static_cast<VrLoopBack*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateMessageLoopBack(loopMessage.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageSessionClosed(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrNotifySessionClosed> seesionClose(VR_new VrNotifySessionClosed);
    seesionClose->CheckTypeAndMergeFrom(*(static_cast<VrNotifySessionClosed*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateMessageSessionClosed(seesionClose.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

// when ui crash , vr service can receive this message;
void VR_ReceiverCb::OnNotifyUICrash(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrNotifyUICrash>  uiCrash(VR_new VrNotifyUICrash);
    uiCrash->CheckTypeAndMergeFrom(*(static_cast<VrNotifyUICrash*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNotifyUICrash(uiCrash.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand); // push in dm child thread
}

// unuseful funtcion
void VR_ReceiverCb::OnMessage2DialogEngine(const VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnMessage2DialogEngine(*pMsg);

}

// unuseful funtcion
void VR_ReceiverCb::OnMessage2DialogManger(const VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnMessage2DialogManger(*pMsg);

}

void VR_ReceiverCb::OnMessageNaviSearchStateResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespStateInfoVRResult> RespStateInfo(VR_new RespStateInfoVRResult);
    RespStateInfo->CheckTypeAndMergeFrom(*(static_cast<RespStateInfoVRResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespStateInfo(RespStateInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchCityResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespCityInfoVRResult> RespCityInfo(VR_new RespCityInfoVRResult);
    RespCityInfo->CheckTypeAndMergeFrom(*(static_cast<RespCityInfoVRResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespCityInfo(RespCityInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchStreetResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespStreetInfoVRResult> RespStreetInfo(VR_new RespStreetInfoVRResult);
    RespStreetInfo->CheckTypeAndMergeFrom(*(static_cast<RespStreetInfoVRResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespStreetInfo(RespStreetInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchHouseNoResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespHouseNoInfoVRResult> RespHouseNoInfo(VR_new RespHouseNoInfoVRResult);
    RespHouseNoInfo->CheckTypeAndMergeFrom(*(static_cast<RespHouseNoInfoVRResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespHouseNoInfo(RespHouseNoInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchStateRespCN(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespStateInfoVRResultCN> RespStateInfoCN(VR_new RespStateInfoVRResultCN);
    RespStateInfoCN->CheckTypeAndMergeFrom(*(static_cast<RespStateInfoVRResultCN*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespStateInfoCN(RespStateInfoCN.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchCityRespCN(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespCityInfoVRResultCN> RespCityInfoCN(VR_new RespCityInfoVRResultCN);
    RespCityInfoCN->CheckTypeAndMergeFrom(*(static_cast<RespCityInfoVRResultCN*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespCityInfoCN(RespCityInfoCN.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchDistrictRespCN(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespDistrictInfoVRResultCN> RespDistrictInfoCN(VR_new RespDistrictInfoVRResultCN);
    RespDistrictInfoCN->CheckTypeAndMergeFrom(*(static_cast<RespDistrictInfoVRResultCN*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespDistrictInfoCN(RespDistrictInfoCN.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchStreetRespCN(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespStreetInfoVRResultCN> RespStreetInfoCN(VR_new RespStreetInfoVRResultCN);
    RespStreetInfoCN->CheckTypeAndMergeFrom(*(static_cast<RespStreetInfoVRResultCN*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespStreetInfoCN(RespStreetInfoCN.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchHouseNoRespCN(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespHouseNoInfoVRResultCN> RespHouseNoInfoCN(VR_new RespHouseNoInfoVRResultCN);
    RespHouseNoInfoCN->CheckTypeAndMergeFrom(*(static_cast<RespHouseNoInfoVRResultCN*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespHouseNoInfoCN(RespHouseNoInfoCN.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchAddressRespCN(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespAddressInfoVRResultCN> RespAddressInfoCN(VR_new RespAddressInfoVRResultCN);
    RespAddressInfoCN->CheckTypeAndMergeFrom(*(static_cast<RespAddressInfoVRResultCN*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespAddressInfoCN(RespAddressInfoCN.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviSearchPoiInfoResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<navi::Search::RespCategorySearchVRResult> RespSearchInfo(VR_new navi::Search::RespCategorySearchVRResult);
    RespSearchInfo->CheckTypeAndMergeFrom(*(static_cast<navi::Search::RespCategorySearchVRResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateRespCategorySearchInfo(RespSearchInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviQueryPointCollectionResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PointResponse> RespPonitInfo(VR_new PointResponse);
    RespPonitInfo->CheckTypeAndMergeFrom(*(static_cast<PointResponse*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreartePointCollection(RespPonitInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviQueryDestHistoryResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PointResponse> RespPonitInfo(VR_new PointResponse);
    RespPonitInfo->CheckTypeAndMergeFrom(*(static_cast<PointResponse*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CrearteDestHistory(RespPonitInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviQueryRouteExistResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<GuideStatus> RespRouteStatus(VR_new GuideStatus);
    RespRouteStatus->CheckTypeAndMergeFrom(*(static_cast<GuideStatus*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CrearteRouteStatus(RespRouteStatus.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviQueryAllDestListResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PathPointList> protoPathPointList(VR_new PathPointList);
    protoPathPointList->CheckTypeAndMergeFrom(*(static_cast<PathPointList*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreartePathPointList(protoPathPointList.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void  VR_ReceiverCb::OnMessageNaviPathPointListNotify(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PathPointList> protoPathPointList(VR_new PathPointList);
    protoPathPointList->CheckTypeAndMergeFrom(*(static_cast<PathPointList*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreartePathPointList(protoPathPointList.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviQueryUnpassedDestListResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PathPointList> UnpassedPathPointList(VR_new PathPointList);
    UnpassedPathPointList->CheckTypeAndMergeFrom(*(static_cast<PathPointList*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CrearteUnpassedPathPointList(UnpassedPathPointList.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviQueryPointTypeResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<NearLinkResult> pNearLinkResult(VR_new NearLinkResult);
    pNearLinkResult->CheckTypeAndMergeFrom(*(static_cast<NearLinkResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CrearteNearLinkResult(pNearLinkResult.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviQueryNextDestResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PathPointList> NextPathPointList(VR_new PathPointList);
    NextPathPointList->CheckTypeAndMergeFrom(*(static_cast<PathPointList*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CrearteNextPathPointList(NextPathPointList.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviReqDfltInfoResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<DefaultInfoResult> DFInfoResult(VR_new DefaultInfoResult);
    DFInfoResult->CheckTypeAndMergeFrom(*(static_cast<DefaultInfoResult*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CrearteDefaultInfoResult(DFInfoResult.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviQueryCurrentPosResp(VR_ProtoMessageBase *pMsg)
{
    // LocationInfo的广播不放置到子线程去做
    // 否则导致子线程的始终处于Busy状态
    // 延缓DM的响应时间
    m_spMessageProcess->OnMessageNaviQueryCurrentPosResp(*pMsg);
    if (0 == (m_nLocalAddrCount % 150)) {  // notify every 30s
        m_nLocalAddrCount = 0;
        std::unique_ptr<navi::Location::LocInfoUpdate> pLocInfoUpdate(VR_new navi::Location::LocInfoUpdate);
        pLocInfoUpdate->CheckTypeAndMergeFrom(*(static_cast<navi::Location::LocInfoUpdate*>(pMsg)));
        auto pcCommand = m_spCommandFactory->CrearteLocInfoUpdate(pLocInfoUpdate.release(), m_spMessageProcess);
        m_spDMCommandHandler->onCommand(pcCommand);
    }
    ++m_nLocalAddrCount;
}

void VR_ReceiverCb::OnMessageNaviQueryPOIByNameRespCN(VR_ProtoMessageBase *pMsg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespPOINameListVRResultCN> POIInfoResult(VR_new RespPOINameListVRResultCN);
    POIInfoResult->CheckTypeAndMergeFrom(*(static_cast<RespPOINameListVRResultCN*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreartePOIByNameInfo(POIInfoResult.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageNaviNotifyRouteStatus(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<NotifyRouteStatus> upNotifyRouteStatus(VR_new NotifyRouteStatus);
    upNotifyRouteStatus->CheckTypeAndMergeFrom(*(static_cast<NotifyRouteStatus*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNotifyRouteStatus(upNotifyRouteStatus.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
#endif
}

void VR_ReceiverCb::OnMessageWeatherCityListResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<navi::Traffic::WeatherCityListRes> cityListRes(VR_new navi::Traffic::WeatherCityListRes);
    cityListRes->CheckTypeAndMergeFrom(*(static_cast<navi::Traffic::WeatherCityListRes*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateWeatherCityListRes(cityListRes.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNotifyWeatherCityList(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<navi::Traffic::WeatherCityListUpdate> cityListUpdate(VR_new navi::Traffic::WeatherCityListUpdate);
    cityListUpdate->CheckTypeAndMergeFrom(*(static_cast<navi::Traffic::WeatherCityListUpdate*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateWeatherCityListUpdate(cityListUpdate.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageWeatherInfoForVRResp(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<navi::Traffic::WeatherInfoforVRRes> weatherInfoRes(VR_new navi::Traffic::WeatherInfoforVRRes);
    weatherInfoRes->CheckTypeAndMergeFrom(*(static_cast<navi::Traffic::WeatherInfoforVRRes*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateWeatherInfoRes(weatherInfoRes.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);

}

void VR_ReceiverCb::OnMessageWeatherDataServiceRes(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<navi::Traffic::WeatherDataServiceRes> DataServiceResProto(VR_new navi::Traffic::WeatherDataServiceRes);
    DataServiceResProto->CheckTypeAndMergeFrom(*(static_cast<navi::Traffic::WeatherDataServiceRes*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateWeatherDataServiceRes(DataServiceResProto.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}


void VR_ReceiverCb::OnMessageWeatherDataServiceUpdate(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<navi::Traffic::WeatherDataServiceUpdate> DataServiceUpdateProto(VR_new navi::Traffic::WeatherDataServiceUpdate);
    DataServiceUpdateProto->CheckTypeAndMergeFrom(*(static_cast<navi::Traffic::WeatherDataServiceUpdate*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateWeatherDataServiceUpdate(DataServiceUpdateProto.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageTtsServiceStatus(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    TtsRespServiceStatus *pOldMsg = static_cast<TtsRespServiceStatus*>(pMsg);
    if (("TTS_MAIN" == pOldMsg->sender()) && (TtsRespServiceStatus_TTS_SERVICE_STATUS_TTS_SERVICE_STATUS_READY == pOldMsg->status())) {
        VrLoopBack *pLoopMessage(VrLoopBack::default_instance().New());
        if (NULL == pLoopMessage) {
            VR_LOGE("VrLoopBack::default_instance().New  FAIL!!");
            return;
        }
        pLoopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage*  controlMessage = pLoopMessage->mutable_m_controlmessage();
        controlMessage->set_type(DMTaskMessageType_StartStatusResult);
        controlMessage->mutable_startstatusresult()->set_starttype(Tts_Crash_Status);
        controlMessage->mutable_startstatusresult()->set_startreuslt(false);

        OnMessageLoopBack(pLoopMessage);
        if (NULL != pLoopMessage) {
            delete pLoopMessage;
            pLoopMessage = NULL;
        }
    }
}

void VR_ReceiverCb::OnMessageNaviMapOn(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<navi::NaviMain::NaviStatus>  naviStatusResult(VR_new navi::NaviMain::NaviStatus);
    naviStatusResult->CheckTypeAndMergeFrom(*(static_cast<navi::NaviMain::NaviStatus*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNaviStatusUpdate(naviStatusResult.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviGuideStart(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<navi::Guide::GudGuideStart>  gudGuideStart(VR_new navi::Guide::GudGuideStart);
    gudGuideStart->CheckTypeAndMergeFrom(*(static_cast<navi::Guide::GudGuideStart*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNaviStatusUpdate(gudGuideStart.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviGuideFinish(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<navi::Guide::GudGuideAbortFinish>  guideAbortFinish(VR_new navi::Guide::GudGuideAbortFinish);
    guideAbortFinish->CheckTypeAndMergeFrom(*(static_cast<navi::Guide::GudGuideAbortFinish*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNaviStatusUpdate(guideAbortFinish.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviRouteInfo(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<navi::Path::IVIGuideRouteInfoUpdate>  routeInfoUpdate(VR_new navi::Path::IVIGuideRouteInfoUpdate);
    routeInfoUpdate->CheckTypeAndMergeFrom(*(static_cast<navi::Path::IVIGuideRouteInfoUpdate*>(pMsg)));
    if (routeInfoUpdate->has_route_detail()) {
        auto pcCommand = m_spCommandFactory->CreateNaviStatusUpdate(routeInfoUpdate.release(), m_spMessageProcess);
        m_spDMCommandHandler->onCommand(pcCommand);
    }
}

void VR_ReceiverCb::OnMessageNaviMapInfo(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<navi::Map::MapInfo>  mapMap(VR_new navi::Map::MapInfo);
    mapMap->CheckTypeAndMergeFrom(*(static_cast<navi::Map::MapInfo*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNaviStatusUpdate(mapMap.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviMapScreenInfo(VR_ProtoMessageBase *pMsg)
{
    // MapInfo 的广播不放置到子线程去做
    // 否则导致子线程的始终处于Busy状态
    // 延缓DM的响应时间
    m_spMessageProcess->OnMessageNaviMapScreenInfo(*pMsg);
}

void VR_ReceiverCb::OnMessageNaviGudETAListInfo(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<navi::Guide::GudETAListInfo>  ETAListInfo(VR_new navi::Guide::GudETAListInfo);
    ETAListInfo->CheckTypeAndMergeFrom(*(static_cast<navi::Guide::GudETAListInfo*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNaviStatusUpdate(ETAListInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviAddrLocation(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<navi::Point::PointResponse>  pointResp(VR_new navi::Point::PointResponse);
    pointResp->CheckTypeAndMergeFrom(*(static_cast<navi::Point::PointResponse*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNaviStatusUpdate(pointResp.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviBackgroundRoundInfo(VR_ProtoMessageBase *pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<navi::Path::BackGroundRouteInfoUpdate>  roundInfo(VR_new navi::Path::BackGroundRouteInfoUpdate);
    roundInfo->CheckTypeAndMergeFrom(*(static_cast<navi::Path::BackGroundRouteInfoUpdate*>(pMsg)));
    auto pcCommand = m_spCommandFactory->CreateNaviStatusUpdate(roundInfo.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviMapNeighborName(VR_ProtoMessageBase *pMsg)
{
    // MapNeighborName 的广播不放置到子线程去做
    // 否则导致子线程的始终处于Busy状态
    // 延缓DM的响应时间
    m_spMessageProcess->OnMessageNaviMapNeighborName(*pMsg);
}

void VR_ReceiverCb::OnMessageNaviInfoRespAddressBook(VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    
    std::unique_ptr<NaviInfoRespPoiAddressBook> respAddrBook(VR_new NaviInfoRespPoiAddressBook);
    respAddrBook->CheckTypeAndMergeFrom(*(static_cast<NaviInfoRespPoiAddressBook*>(pMsg)));

    auto pcCommand = m_spCommandFactory->CreateNaviInfoMessageResp(respAddrBook.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviInfoRespPoiTime(VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<NaviInfoRespPoiTime> respPoiTime(VR_new NaviInfoRespPoiTime);
    respPoiTime->CheckTypeAndMergeFrom(*(static_cast<NaviInfoRespPoiTime*>(pMsg)));

    auto pcCommand = m_spCommandFactory->CreateNaviInfoMessageResp(respPoiTime.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviInfoRespResult(VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    
    std::unique_ptr<NaviInfoRespResult> respResult(VR_new NaviInfoRespResult);
    respResult->CheckTypeAndMergeFrom(*(static_cast<NaviInfoRespResult*>(pMsg)));

    auto pcCommand = m_spCommandFactory->CreateNaviInfoMessageResp(respResult.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviInfoRespNaviStatus(VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();
    
    std::unique_ptr<NaviInfoRespNavigationStatus> respNaviStatus(VR_new NaviInfoRespNavigationStatus);
    respNaviStatus->CheckTypeAndMergeFrom(*(static_cast<NaviInfoRespNavigationStatus*>(pMsg)));

    auto pcCommand = m_spCommandFactory->CreateNaviInfoMessageResp(respNaviStatus.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}

void VR_ReceiverCb::OnMessageNaviInfoRespCarPositionInfo(VR_ProtoMessageBase* pMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<NaviInfoRespCarPositionInfo> respCarPosition(VR_new NaviInfoRespCarPositionInfo);
    respCarPosition->CheckTypeAndMergeFrom(*(static_cast<NaviInfoRespCarPositionInfo*>(pMsg)));

    auto pcCommand = m_spCommandFactory->CreateNaviInfoMessageResp(respCarPosition.release(), m_spMessageProcess);
    m_spDMCommandHandler->onCommand(pcCommand);
}
/* EOF */
