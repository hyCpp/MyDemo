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
 * @file VR_ActionManger.h
 * @brief Declaration file of class VR_ActionManger.
 *
 * This file includes the declaration of class VR_ActionManger.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONMANGER_H
#define VR_ACTIONMANGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef CXX_BL_SYNCOBJECT_H
#   include "BL_SyncObject.h"
#endif

#include "VR_SessionManger.h"

#if defined(VR_DM_TARGET_VRUNIT)
#include "navi/Location/LocInfoUpdate.pb.h"
#endif

class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionManger);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RequestQueue);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionFactoryIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionEventSender);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SessionManger);

namespace navi
{
namespace dataprovider
{
    VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DataProvider);
}
}

namespace navi
{
namespace VoiceRecog
{
    class VrActionDE2DM;
    class VrActionFC2DM;
    class VrRecogResult;
    class VrAdditionalInfoReq;
    class VrActiveInfoReq;
    class VrTTSCommandReq;
    class VrBeepCommandReq;
    class VrActionTestMsg;
    class RequestVrVoiceTag;
    class VrMessageAcquisitionReq;
    class VrActionNavi;
}
}

#if defined(VR_DM_TARGET_VRUNIT)
namespace navi
{
namespace Location
{
    class LocInfoUpdate;
}
}
#endif

/**
 * @brief The VR_ActionManger class
 *
 * class declaration
 */
class VR_ActionManger
{
public:
    VR_ActionManger(spVR_ActionFactoryIF factory, spVR_ActionEventSender sender,
                    spVR_SessionManger spSessionManager);
    ~VR_ActionManger();

public:
    void OnMessage(const navi::VoiceRecog::VrActionDE2DM &proto);
    void OnMessage(const navi::VoiceRecog::VrActionFC2DM &proto);
    void OnReplyActoinResult(const VR_ProtoMessageBase &msg);
    void VoiceTagAction(const EV_EventContext &context, const VR_ProtoMessageBase &proto);

    void OnMessageNaviReqDfltInfoResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchStateResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchCityResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchStreetResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchHouseNoResp(const VR_ProtoMessageBase &msg);

    void OnMessageNaviSearchPoiInfoResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryPointCollectionResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryDestHistoryResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryRouteExistResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryAllDestListResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryUnpassedDestListResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryPointTypeResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryNextDestResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryCurrentPosResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviMapScreenInfo(const VR_ProtoMessageBase &msg);
    void OnMessageNaviMapNeighborName(const VR_ProtoMessageBase &msg);
    void OnMessageNaviQueryPOIByNameRespCN(const VR_ProtoMessageBase &msg);

    void OnMessageNaviSearchStateRespCN(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchCityRespCN(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchDistrictRespCN(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchStreetRespCN(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchHouseNoRespCN(const VR_ProtoMessageBase &msg);
    void OnMessageNaviSearchAddressInfoCN(const VR_ProtoMessageBase &msg);
    void OnMessageNaviNotifyRouteStatus(const VR_ProtoMessageBase &msg);
    void OnMessageWeatherInfoResp(const VR_ProtoMessageBase &msg);
    void OnMessageNaviPathPointListNotify(const VR_ProtoMessageBase& msg);
    void UpdateNaviPathPointList(const VR_ProtoMessageBase& msg);
    void ClearTtsAction();


protected:

    // the state can/cannot send a message
    void OnReceiveMsgAvaliableState(const VR_String &strMsgState);

    // message type can be supported by current bt_connected_tel
    void OnReceiveSupportMsgTypeInfo(const VR_String &strMsgType);

    // phone connected state notify (only meu)
    void OnReceiveTelConnectedState(const VR_String &strConState);

    // time value where be used by the first quickreply_message (only meu)
    void OnReceiveArriveTimeNotify(const VR_String &strArriveTime);

protected:
    void RecogResultAction(int actionId, const navi::VoiceRecog::VrRecogResult &proto);
    void AdditionalInfoAction(int actionId, const navi::VoiceRecog::VrAdditionalInfoReq &proto);
    void ActiveInfoAction(int actionId, const navi::VoiceRecog::VrActiveInfoReq &proto);
    void TtsAction(int actionId, const navi::VoiceRecog::VrTTSCommandReq &proto);
    void BeepAction(int actionId, const navi::VoiceRecog::VrBeepCommandReq &proto);
    void TestActionMsg(int actionId, const navi::VoiceRecog::VrActionTestMsg &proto);
    void MessageDetailAction(int actionId, const navi::VoiceRecog::VrMessageAcquisitionReq &proto);
    void NaviActionHandle(int actionId, const navi::VoiceRecog::VrActionNavi &proto);
#if defined(VR_DM_TARGET_VRUNIT)
    void QueryCurrentPosAction(int actionId);
#endif
    void TtsListAction(int actionId, const navi::VoiceRecog::VrTtsListCommandReq &proto);

protected:
    spVR_ActionFactoryIF    m_pActionFactory;
    spVR_RequestQueue       m_pRequestQueue;
    spVR_ActionEventSender  m_spVR_ActionEventSender;
    spVR_SessionManger      m_spSessionManager;
    navi::dataprovider::spVR_DataProvider m_spDataProvider;
    bool                    m_bDemoOn;
    bool                    m_bIsPhoneConFromFC;
};

#endif // VR_ACTIONMANGER_H
/* EOF */
