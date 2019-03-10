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
 * @file VR_ReceiverCb.h
 * @brief Declaration file of class VR_ReceiverCb.
 *
 * This file includes the declaration of class VR_ReceiverCb.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RECEIVECB_H
#define VR_RECEIVECB_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "MEM_map.h"
#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_CommandFactory.h"
#include "VR_DM_CommandHandle.h"
#include "VR_RunableThread.h"

class EV_EventContext;
class VR_SettingListener;
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventReceiver);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_MessageProcessIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SettingListener);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DM_CommandHandle);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RequestGetSessionID);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandFactory);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RunableThread);

/**
 * @brief The VR_ReceiverCb class
 *
 * class declaration
 */
class VR_ReceiverCb
{
public:
    explicit VR_ReceiverCb(spVR_MessageProcessIF process, spVR_CommandFactory pVrCommandFactory, spVR_DM_CommandHandle pCmdHandle);
    ~VR_ReceiverCb();

public:
    void StartReceive();
    void StopReceive();

protected:
    void Register();
    void Unregister();
    void StartOver();
    void NotifyFullUpdate();

protected:
    /********************** Normal VR  ******************************************************/
    void OnReqestGetSeessionID(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestOpenSeession(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestCloseSeession(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestStartDialog(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestCancleDialog(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestUserOperation(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestStartChangeLanguage(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestCommonProperty(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestVrPVRFlag(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestVrVoiceTag(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestVrTsl(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnReqestVrSettingState(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);

    /*****************************  Manual VR *************************************************/
    void OnMMVRRequestCommonProperty(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);

    /***************************  SDL VR  ****************************************************/
    void OnSDLVRRequestCommonProperty(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);

    /***************************  vui     ****************************************************/
    void OnRequestVriAutoProxy(const EV_EventContext& context, VR_ProtoMessageBase* pMsg);
    void OnMessageNaviInfoRespAddressBook(VR_ProtoMessageBase* pMsg);
    void OnMessageNaviInfoRespPoiTime(VR_ProtoMessageBase* pMsg);
    void OnMessageNaviInfoRespResult(VR_ProtoMessageBase* pMsg);
    void OnMessageNaviInfoRespNaviStatus(VR_ProtoMessageBase* pMsg);
    void OnMessageNaviInfoRespCarPositionInfo(VR_ProtoMessageBase* pMsg);

    /*************************  Normal VR ******************************************************/
    void OnMessageTtsPlayEnd(VR_ProtoMessageBase* pMsg);
    void OnMessageTtsPlayState(VR_ProtoMessageBase* pMsg);
    void OnMessageLoopBack(VR_ProtoMessageBase* pMsg);
    void OnMessageSessionClosed(VR_ProtoMessageBase *pMsg);
    void OnReplyActoinResult(VR_ProtoMessageBase* pMsg);
    void OnMessage2DialogEngine(const VR_ProtoMessageBase* pMsg);
    void OnMessage2DialogManger(const VR_ProtoMessageBase* pMsg);
    void OnNotifyUICrash(VR_ProtoMessageBase* pMsg);

    void OnMessageNaviReqDfltInfoResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchStateResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchCityResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchStreetResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchHouseNoResp(VR_ProtoMessageBase *pMsg);
    
    void OnMessageNaviSearchStateRespCN(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchCityRespCN(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchDistrictRespCN(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchStreetRespCN(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchHouseNoRespCN(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviSearchAddressRespCN(VR_ProtoMessageBase *pMsg);

    void OnMessageNaviSearchPoiInfoResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryPointCollectionResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryDestHistoryResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryRouteExistResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryAllDestListResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryUnpassedDestListResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryPointTypeResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryNextDestResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryCurrentPosResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviQueryPOIByNameRespCN(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviNotifyRouteStatus(VR_ProtoMessageBase *pMsg);
    void OnMessageTtsServiceStatus(VR_ProtoMessageBase* pMsg);
    void OnMessageNaviMapOn(VR_ProtoMessageBase* pMsg);
    void OnMessageNaviGuideStart(VR_ProtoMessageBase* pMsg);
    void OnMessageNaviGuideFinish(VR_ProtoMessageBase* pMsg);
    void OnMessageNaviRouteInfo(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviMapInfo(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviMapScreenInfo(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviGudETAListInfo(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviAddrLocation(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviBackgroundRoundInfo(VR_ProtoMessageBase *pMsg);
    void OnMessageNaviMapNeighborName(VR_ProtoMessageBase *pMsg);
    void OnMessageWeatherCityListResp(VR_ProtoMessageBase *pMsg);
    void OnMessageNotifyWeatherCityList(VR_ProtoMessageBase *pMsg);
    void OnMessageWeatherInfoForVRResp(VR_ProtoMessageBase *pMsg);
    void OnMessageWeatherDataServiceRes(VR_ProtoMessageBase *pMsg);
    void OnMessageWeatherDataServiceUpdate(VR_ProtoMessageBase *pMsg);
    // add path point list notify
    void OnMessageNaviPathPointListNotify(VR_ProtoMessageBase* pMsg);
    void OnMessageSpotlightSearchResult(VR_ProtoMessageBase* pMsg);
    void OnMessageSearchModuleStatus(VR_ProtoMessageBase* pMsg);

protected:
    typedef void(VR_ReceiverCb::*MsgHandle)(const EV_EventContext&, VR_ProtoMessageBase*);
    typedef void(VR_ReceiverCb::*InnerMsgHandle)(VR_ProtoMessageBase*);
    typedef VoiceMap<std::string, MsgHandle>::type NameHandleMap;
    typedef VoiceMap<std::string, InnerMsgHandle>::type NameInnerHandleMap;
    typedef NameHandleMap::const_reference cNameHandleMapRef;
    typedef NameInnerHandleMap::const_reference cNameInnerHandleMapRef;
    static const NameHandleMap      s_MsgHandleMap;
    static const NameInnerHandleMap s_MsgInnerHandleMap;

protected:
    spEV_EventReceiver          m_spEvtRecv;
    spVR_MessageProcessIF       m_spMessageProcess;
#ifdef HAVE_NUTSHELL_OS
   spVR_RunableThread          m_spRunableThread;
   spVR_SettingListener        m_spVoiceSettingListener;
#endif
    spVR_DM_CommandHandle       m_spDMCommandHandler;
    spVR_CommandFactory         m_spCommandFactory;
    VR_UINT32                   m_nLocalAddrCount;

private:
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_ReceiverCb);

};

#endif /* VR_RECEIVECB_H */
/* EOF */
