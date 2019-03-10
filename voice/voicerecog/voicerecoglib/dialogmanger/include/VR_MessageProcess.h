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
 * @file VR_MessageProcess.h
 * @brief Declaration file of class VR_MessageProcess.
 *
 * This file includes the declaration of class VR_MessageProcess.
 *
 * @attention used for C++ only.
 */

#ifndef VR_MESSAGEPROCESS_H
#define VR_MESSAGEPROCESS_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_MessageProcessIF.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogManger);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionManger);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SessionManger);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandHandleIF);

namespace navi
{
namespace dataprovider
{
    class VR_DataProvider;
}
}


/**
 * @brief The VR_MessageProcess class
 *
 * class declaration
 */
class VR_MessageProcess : public VR_MessageProcessIF
{
public:
    VR_MessageProcess(spVR_CommandHandleIF spCommandHandle);
    virtual ~VR_MessageProcess();
    virtual void Initialize();
    virtual void Start();
    virtual void CheckStatus();
    virtual void Stop();
    virtual void Destory();

public:
    /***************************** Normal VR ********************************************************/
    virtual void OnReqestGetSeessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestOpenSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestCloseSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestStartDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestCancleDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestUserOperation(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestStartChangeLanguage(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestVrPVRFlag(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReplyActoinResult(const VR_ProtoMessageBase &msg);
    virtual void OnReqestVrVoiceTag(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestVrTsl(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    virtual void OnReqestVrSettingState(const EV_EventContext &context, const VR_ProtoMessageBase& msg);

    /***************************** Manual VR *********************************************************/
    virtual void OnMMVRRequestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

    /**************************** SDL VR *************************************************************/
    virtual void OnSDLVRRequestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

    /**************************** VUI    *************************************************************/
    virtual void OnRequestVriAutoProxy(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

    /***************************** Normal VR ********************************************************/
    virtual void OnMessageStarted();
    virtual void OnMessageLoopBack(const VR_ProtoMessageBase &msg);
    virtual void OnNotifyUICrash(const VR_ProtoMessageBase& msg);
    virtual void OnMessageSessionClosed(const VR_ProtoMessageBase &msg);
    virtual void OnMessage2DialogEngine(const VR_ProtoMessageBase &msg);
    virtual void OnMessage2DialogManger(const VR_ProtoMessageBase &msg);

    virtual void OnMessageNaviReqDfltInfoResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchStateResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchCityResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchStreetResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchHouseNoResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchPoiInfoResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryPointCollectionResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryDestHistoryResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryRouteExistResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryAllDestListResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryUnpassedDestListResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryPointTypeResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryNextDestResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryCurrentPosResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviMapScreenInfo(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviMapNeighborName(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviQueryPOIByNameRespCN(const VR_ProtoMessageBase &msg);

    virtual void OnMessageNaviSearchStateRespCN(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchCityRespCN(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchDistrictRespCN(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchStreetRespCN(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchHouseNoRespCN(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNaviSearchAddressInfoCN(const VR_ProtoMessageBase &msg);

    virtual void OnMessageReqWebSearchEngine();

    virtual void OnStartedNotify();
    virtual void OnMessageNaviNotifyRouteStatus(const VR_ProtoMessageBase &msg);
    virtual void OnMessageTtsBlinkRest(const VR_ProtoMessageBase &msg);
    virtual void OnMessageWeatherCityListResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageNotifyWeatherCityList(const VR_ProtoMessageBase &msg);
    virtual void OnMessageWeatherInfoResp(const VR_ProtoMessageBase &msg);
    virtual void OnMessageWeatherDataServiceRes(const VR_ProtoMessageBase &msg);
    virtual void OnMessageWeatherDataServiceUpdate(const VR_ProtoMessageBase &msg);

    virtual void OnMessageNaviPathPointListNotify(const VR_ProtoMessageBase& msg);
    virtual void OnMessageSpotlightSearchResult(const VR_ProtoMessageBase& msg);
    virtual void OnMessageSearchModuleStatus(const VR_ProtoMessageBase& msg);
    virtual void OnMessageNaviStatusUpdate(const VR_ProtoMessageBase& msg);
    virtual void OnMessageNaviLocInfoUpdate(const VR_ProtoMessageBase& msg);
    virtual void OnMessageCategySearchInfo(const VR_ProtoMessageBase& msg);

    // naviinfo for wukong
    virtual void OnMessageNaviInfoResp(const VR_ProtoMessageBase& msg);

protected:
    bool  SessionChecker(const std::string& session);

protected:
    spVR_SessionManger  m_SessionManger;
    spVR_DialogManger   m_spDialogManger;
    spVR_ActionManger   m_spActionManger;

    typedef boost::shared_ptr<navi::dataprovider::VR_DataProvider> spVR_DataProvider;
    spVR_DataProvider   m_spDataProvider;

protected:
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_MessageProcess);

};

#endif // VR_MESSAGEPROCESS_H
/* EOF */
