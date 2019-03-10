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
 * @file VR_MessageProcessIF.h
 * @brief Declaration file of class VR_MessageProcessIF.
 *
 * This file includes the declaration of class VR_MessageProcessIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_MESSAGEPROCESSIF_H
#define VR_MESSAGEPROCESSIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

class EV_EventContext;

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_MessageProcessIF);

/**
 * @brief The VR_MessageProcessIF class
 *
 * class VR_MessageProcessIF declaration
 */
class VR_MessageProcessIF
{
public:
    /******************************* Normal VR ***********************************************************/
    virtual ~VR_MessageProcessIF() {}
    virtual void OnReqestGetSeessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg)       = 0;
    virtual void OnReqestOpenSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)        = 0;
    virtual void OnReqestCloseSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)       = 0;
    virtual void OnReqestStartDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg)         = 0;
    virtual void OnReqestCancleDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg)        = 0;
    virtual void OnReqestUserOperation(const EV_EventContext &context, const VR_ProtoMessageBase &msg)       = 0;
    virtual void OnReqestStartChangeLanguage(const EV_EventContext &context, const VR_ProtoMessageBase &msg) = 0;
    virtual void OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)      = 0;
    virtual void OnReqestVrPVRFlag(const EV_EventContext &context, const VR_ProtoMessageBase &msg)           = 0;
    virtual void OnReqestVrVoiceTag(const EV_EventContext &context, const VR_ProtoMessageBase &msg)          = 0;
    virtual void OnReqestVrTsl(const EV_EventContext &context, const VR_ProtoMessageBase &msg)               = 0;
    virtual void OnReqestVrSettingState(const EV_EventContext& content, const VR_ProtoMessageBase& msg)      = 0;

    /******************************* Maual VR ***********************************************************/
    virtual void OnMMVRRequestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& pMsg)      = 0;

    /******************************* SDL VR *************************************************************/
    virtual void OnSDLVRRequestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& pMsg)     = 0;

    /******************************* vui    *************************************************************/
    virtual void OnRequestVriAutoProxy(const EV_EventContext& context, const VR_ProtoMessageBase& pMsg)      = 0;

    virtual void OnReplyActoinResult(const VR_ProtoMessageBase &msg)                                         = 0;
    virtual void OnNotifyUICrash(const VR_ProtoMessageBase& msg)                                             = 0;
    virtual void OnMessageLoopBack(const VR_ProtoMessageBase &msg)                                           = 0;
    virtual void OnMessageSessionClosed(const VR_ProtoMessageBase &msg)                                      = 0;
    virtual void OnMessage2DialogEngine(const VR_ProtoMessageBase &msg)                                      = 0;
    virtual void OnMessage2DialogManger(const VR_ProtoMessageBase &msg)                                      = 0;

    virtual void OnMessageNaviReqDfltInfoResp(const VR_ProtoMessageBase &msg)            = 0;
    virtual void OnMessageNaviSearchStateResp(const VR_ProtoMessageBase &msg)            = 0;
    virtual void OnMessageNaviSearchCityResp(const VR_ProtoMessageBase &msg)             = 0;
    virtual void OnMessageNaviSearchStreetResp(const VR_ProtoMessageBase &msg)           = 0;
    virtual void OnMessageNaviSearchHouseNoResp(const VR_ProtoMessageBase &msg)          = 0;
    virtual void OnMessageNaviSearchPoiInfoResp(const VR_ProtoMessageBase &msg)          = 0;
    virtual void OnMessageNaviQueryPointCollectionResp(const VR_ProtoMessageBase &msg)   = 0;
    virtual void OnMessageNaviQueryDestHistoryResp(const VR_ProtoMessageBase &msg)       = 0;
    virtual void OnMessageNaviQueryRouteExistResp(const VR_ProtoMessageBase &msg)        = 0;
    virtual void OnMessageNaviQueryAllDestListResp(const VR_ProtoMessageBase &msg)       = 0;
    virtual void OnMessageNaviQueryUnpassedDestListResp(const VR_ProtoMessageBase &msg)  = 0;
    virtual void OnMessageNaviQueryPointTypeResp(const VR_ProtoMessageBase &msg)         = 0;
    virtual void OnMessageNaviQueryNextDestResp(const VR_ProtoMessageBase &msg)          = 0;
    virtual void OnMessageNaviQueryCurrentPosResp(const VR_ProtoMessageBase &msg)        = 0;
    virtual void OnMessageNaviMapScreenInfo(const VR_ProtoMessageBase &msg)              = 0;
    virtual void OnMessageNaviMapNeighborName(const VR_ProtoMessageBase &msg)            = 0;
    virtual void OnMessageNaviQueryPOIByNameRespCN(const VR_ProtoMessageBase &msg)       = 0;

    virtual void OnMessageNaviSearchStateRespCN(const VR_ProtoMessageBase &msg)          = 0;
    virtual void OnMessageNaviSearchCityRespCN(const VR_ProtoMessageBase &msg)           = 0;
    virtual void OnMessageNaviSearchDistrictRespCN(const VR_ProtoMessageBase &msg)       = 0;
    virtual void OnMessageNaviSearchStreetRespCN(const VR_ProtoMessageBase &msg)         = 0;
    virtual void OnMessageNaviSearchHouseNoRespCN(const VR_ProtoMessageBase &msg)        = 0;
    virtual void OnMessageNaviSearchAddressInfoCN(const VR_ProtoMessageBase &msg)        = 0;
    virtual void OnMessageNaviNotifyRouteStatus(const VR_ProtoMessageBase &msg)          = 0;

    virtual void OnMessageTtsBlinkRest(const VR_ProtoMessageBase &msg)                   = 0;
    virtual void OnMessageWeatherCityListResp(const VR_ProtoMessageBase &msg)            = 0;
    virtual void OnMessageNotifyWeatherCityList(const VR_ProtoMessageBase &msg)          = 0;
    virtual void OnMessageWeatherInfoResp(const VR_ProtoMessageBase &msg)                = 0;
    virtual void OnMessageWeatherDataServiceRes(const VR_ProtoMessageBase &msg)          = 0;
    virtual void OnMessageWeatherDataServiceUpdate(const VR_ProtoMessageBase &msg)       = 0;
    virtual void OnMessageNaviPathPointListNotify(const VR_ProtoMessageBase& msg)        = 0;
    virtual void OnMessageSpotlightSearchResult(const VR_ProtoMessageBase& msg)          = 0;
    virtual void OnMessageNaviStatusUpdate(const VR_ProtoMessageBase& msg)               = 0;
    virtual void OnMessageNaviLocInfoUpdate(const VR_ProtoMessageBase& msg)              = 0;
    virtual void OnMessageCategySearchInfo(const VR_ProtoMessageBase& msg)               = 0;


    virtual void OnMessageSearchModuleStatus(const VR_ProtoMessageBase& msg) = 0;

    virtual void OnMessageReqWebSearchEngine() = 0;
    virtual void OnStartedNotify()             = 0;

    // naviinfo for wukong
    virtual void OnMessageNaviInfoResp(const VR_ProtoMessageBase& msg) = 0;
};

#endif // VR_MESSAGEPROCESSIF_H
/* EOF */
