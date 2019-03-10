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

#include   "stdafx.h"
#include   "VR_ConfigureDEF.h"
#include   "VR_ConfigureIF.h"
#include   "VR_ReceiverCb.h"
#include   "gtest/gtest.h"
#include   "gmock/gmock.h"
#include   "VR_DialogEngine_mock.h"
// #include "VR_AvcLanSender_mock.h"
// #include   "VR_AvcListenerIF.h"
// #include   "VR_AvcLanSenderIF.h"
#include   "VR_MessageProcess.h"
#include   "EV_EventContext.h"
#include "VR_ContentProvider.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
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
// #include "VR_Common_VrUnitAgent.h"
#if defined(VR_DM_TARGET_VRUNIT)
#include "navi/Search/RespStateInfoVRResult.pb.h"
#include "navi/Search/RespCityInfoVRResult.pb.h"
#include "navi/Search/RespStreetInfoVRResult.pb.h"
#include "navi/Search/RespHouseNoInfoVRResult.pb.h"
#include "navi/Search/DefaultInfoResult.pb.h"
#include "navi/Search/RespCategorySearchVRResult.pb.h"
#include "navi/Path/RouteStatus.pb.h"
#include "navi/Path/PathPointList.pb.h"
#include "navi/Path/NearLinkResult.pb.h"
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
using namespace navi::VoiceRecog;
using namespace navi::TTS;
using ::testing::Return;

/**
* VR_ReceiverCb_Test.cpp
*
* The class is just for VR_ReceiverCb test.
*/
class  VR_ReceiverCb_Test : public testing::Test
{
protected:

    // virtual   void  SetUp();
    // virtual   void  TearDown();
    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

    static VR_ReceiverCb*     m_VR_ReceiverCb;
  //  static VR_AvcLanSender_mock*  m_pAvcSendMock;
};

VR_ReceiverCb* VR_ReceiverCb_Test::m_VR_ReceiverCb = NULL;
// VR_AvcLanSender_mock* VR_ReceiverCb_Test::m_pAvcSendMock = NULL;

VOID
VR_ReceiverCb_Test::SetUpTestCase()
{
    spVR_DM_CommandHandle spDM_CommandHandle(VR_new VR_DM_CommandHandle);
    VR_LOGD("1");
    spVR_CommandFactory   spVrCommandFactory(VR_CommandFactory::getCommandFactory());
 //   m_pAvcSendMock = VR_new VR_AvcLanSender_mock();
 //   EXPECT_CALL(*m_pAvcSendMock, GetSenderType()).WillRepeatedly(Return(VR_AvcLanSenderTypeNULL));
 //   spVR_AvcLanSenderIF   spAvcSender(m_pAvcSendMock);
    spVR_MessageProcessIF spMessageProcess(VR_new VR_MessageProcess(spDM_CommandHandle));
    m_VR_ReceiverCb = VR_new  VR_ReceiverCb(spMessageProcess, spVrCommandFactory, spDM_CommandHandle);
}

VOID
VR_ReceiverCb_Test::TearDownTestCase()
{
    if (m_VR_ReceiverCb) {
        delete  m_VR_ReceiverCb;
        m_VR_ReceiverCb = NULL;
    }

 //   if (m_pAvcSendMock) {
 //       delete  m_pAvcSendMock;
 //       m_pAvcSendMock = NULL;
 //   }
}

TEST_F(VR_ReceiverCb_Test, StartReceive_Case)
{
    // TODO
    // m_VR_ReceiverCb->StartReceive();
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, StopReceive_Case)
{
    m_VR_ReceiverCb->StopReceive();
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, StartOver_Case)
{
    // TODO
    // m_VR_ReceiverCb->StartOver();
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, NotifyFullUpdate_navifulldata)
{
    VR_ContentProvider vr_cp;
    vr_cp.UpdateContentProviderValue("content://system/info", "sys/updatemode", "navifulldata");
    m_VR_ReceiverCb->NotifyFullUpdate();
    SUCCEED();
}

 TEST_F(VR_ReceiverCb_Test, NotifyFullUpdate_Case)
 {
    VR_ContentProvider vr_cp;
    vr_cp.UpdateContentProviderValue("content://system/info", "sys/updatemode", "111");
    m_VR_ReceiverCb->NotifyFullUpdate();
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, Register_Case)
{
    m_VR_ReceiverCb->Register();
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, Unregister_Case)
{
    m_VR_ReceiverCb->Unregister();
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestGetSeessionID_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrCreateSession> createSession(VR_new RequestVrCreateSession);
    m_VR_ReceiverCb->OnReqestGetSeessionID(*pcContext, createSession.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestOpenSeession_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrOpenSession> openSession(VR_new RequestVrOpenSession);
    m_VR_ReceiverCb->OnReqestOpenSeession(*pcContext, openSession.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestCloseSeession_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrCloseSession> closeSession(VR_new RequestVrCloseSession);
    m_VR_ReceiverCb->OnReqestCloseSeession(*pcContext, closeSession.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestStartDialog_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrStartRecognition> startDailog(VR_new RequestVrStartRecognition);
    m_VR_ReceiverCb->OnReqestStartDialog(*pcContext, startDailog.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestCancleDialog_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrCancelRecognition> cancelDailog(VR_new RequestVrCancelRecognition);
    m_VR_ReceiverCb->OnReqestCancleDialog(*pcContext, cancelDailog.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestUserOperation_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrReqOperation> usrOperation(VR_new RequestVrReqOperation);
    m_VR_ReceiverCb->OnReqestUserOperation(*pcContext, usrOperation.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestStartChangeLanguage_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrReqOperation> usrOperation(VR_new RequestVrReqOperation);
    m_VR_ReceiverCb->OnReqestStartChangeLanguage(*pcContext, usrOperation.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestCommonProperty_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrCommonProperty> requestProperty(VR_new RequestVrCommonProperty);
    m_VR_ReceiverCb->OnReqestCommonProperty(*pcContext, requestProperty.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestVrPVRFlag_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrPVRFlag> requestPVRFlag(VR_new RequestVrPVRFlag);
    m_VR_ReceiverCb->OnReqestVrPVRFlag(*pcContext, requestPVRFlag.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestVrVoiceTag_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrVoiceTag> requestVoiceTag(VR_new RequestVrVoiceTag);
    m_VR_ReceiverCb->OnReqestVrVoiceTag(*pcContext, requestVoiceTag.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestVrTsl_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrTSL> requestTsl(VR_new RequestVrTSL);
    m_VR_ReceiverCb->OnReqestVrTsl(*pcContext, requestTsl.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReqestVrSettingState_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<VrRequestSettingProperty> requestVrSet(VR_new VrRequestSettingProperty);
    m_VR_ReceiverCb->OnReqestVrSettingState(*pcContext, requestVrSet.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnReplyActoinResult_Case)
{
    std::unique_ptr<VrNotifyActionResult> actionResult(VR_new VrNotifyActionResult);
    m_VR_ReceiverCb->OnReplyActoinResult(actionResult.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayEnd_Case)
{
    std::unique_ptr<TtsOnPlayEnd> msg(VR_new TtsOnPlayEnd);
    msg->set_sender("other");
    m_VR_ReceiverCb->OnMessageTtsPlayEnd(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayEnd_EV_MODULE_VOICERECOG)
{
    std::unique_ptr<TtsOnPlayEnd> msg(VR_new TtsOnPlayEnd);
    msg->set_sender(EV_MODULE_VOICERECOG);
    m_VR_ReceiverCb->OnMessageTtsPlayEnd(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayState_VOICERECOG_STOPED)
{
    std::unique_ptr<TtsRespPlayState> msg(VR_new TtsRespPlayState);
    msg->set_sender(EV_MODULE_VOICERECOG);
    msg->set_state(TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_STOPED);
    m_VR_ReceiverCb->OnMessageTtsPlayState(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayState_VOICERECOG_ABORT)
{
    std::unique_ptr<TtsRespPlayState> msg(VR_new TtsRespPlayState);
    msg->set_sender(EV_MODULE_VOICERECOG);
    msg->set_state(TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ABORT);
    m_VR_ReceiverCb->OnMessageTtsPlayState(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayState_VOICERECOG_ERROR_ID)
{
    std::unique_ptr<TtsRespPlayState> msg(VR_new TtsRespPlayState);
    msg->set_sender(EV_MODULE_VOICERECOG);
    msg->set_state(TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR_ID);
    m_VR_ReceiverCb->OnMessageTtsPlayState(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayState_VOICERECOG_ERROR_LANG)
{
    std::unique_ptr<TtsRespPlayState> msg(VR_new TtsRespPlayState);
    msg->set_sender(EV_MODULE_VOICERECOG);
    msg->set_state(TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR_LANG);
    m_VR_ReceiverCb->OnMessageTtsPlayState(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayState_VOICERECOG_ERROR)
{
    std::unique_ptr<TtsRespPlayState> msg(VR_new TtsRespPlayState);
    msg->set_sender(EV_MODULE_VOICERECOG);
    msg->set_state(TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_ERROR);
    m_VR_ReceiverCb->OnMessageTtsPlayState(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayState_VOICERECOG_default)
{
    std::unique_ptr<TtsRespPlayState> msg(VR_new TtsRespPlayState);
    msg->set_sender(EV_MODULE_VOICERECOG);
    msg->set_state(TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_WAIT);
    m_VR_ReceiverCb->OnMessageTtsPlayState(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayState_NCTUTORIAL)
{
    std::unique_ptr<TtsRespPlayState> msg(VR_new TtsRespPlayState);
    msg->set_sender("VoiceRecogTutorial");
    m_VR_ReceiverCb->OnMessageTtsPlayState(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageTtsPlayState_other)
{
    std::unique_ptr<TtsRespPlayState> msg(VR_new TtsRespPlayState);
    msg->set_sender("other");
    m_VR_ReceiverCb->OnMessageTtsPlayState(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageLoopBack_Case)
{
    std::unique_ptr<VrLoopBack> msg(VR_new VrLoopBack);
    m_VR_ReceiverCb->OnMessageLoopBack(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageSessionClosed_Case)
{
    std::unique_ptr<VrNotifySessionClosed> msg(VR_new VrNotifySessionClosed);
    m_VR_ReceiverCb->OnMessageSessionClosed(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnNotifyUICrash)
{
    std::unique_ptr<VrNotifyUICrash> msg(VR_new VrNotifyUICrash);
    m_VR_ReceiverCb->OnNotifyUICrash(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessage2DialogEngine_Case)
{
    std::unique_ptr<VrNotifySessionClosed> msg(VR_new VrNotifySessionClosed);
    m_VR_ReceiverCb->OnMessage2DialogEngine(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessage2DialogManger_Case)
{
    std::unique_ptr<VrNotifySessionClosed> msg(VR_new VrNotifySessionClosed);
    m_VR_ReceiverCb->OnMessage2DialogManger(msg.get());
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchStateResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespStateInfoVRResult> msg(VR_new RespStateInfoVRResult);
    m_VR_ReceiverCb->OnMessageNaviSearchStateResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchCityResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespCityInfoVRResult> msg(VR_new RespCityInfoVRResult);
    m_VR_ReceiverCb->OnMessageNaviSearchCityResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchStreetResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespStreetInfoVRResult> msg(VR_new RespStreetInfoVRResult);
    m_VR_ReceiverCb->OnMessageNaviSearchStreetResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchHouseNoResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespHouseNoInfoVRResult> msg(VR_new RespHouseNoInfoVRResult);
    m_VR_ReceiverCb->OnMessageNaviSearchHouseNoResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchStateRespCN_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespStateInfoVRResultCN> msg(VR_new RespStateInfoVRResultCN);
    m_VR_ReceiverCb->OnMessageNaviSearchStateRespCN(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchCityRespCN_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespCityInfoVRResultCN> msg(VR_new RespCityInfoVRResultCN);
    m_VR_ReceiverCb->OnMessageNaviSearchCityRespCN(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchDistrictRespCN_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespDistrictInfoVRResultCN> msg(VR_new RespDistrictInfoVRResultCN);
    m_VR_ReceiverCb->OnMessageNaviSearchDistrictRespCN(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchStreetRespCN_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespStreetInfoVRResultCN> msg(VR_new RespStreetInfoVRResultCN);
    m_VR_ReceiverCb->OnMessageNaviSearchStreetRespCN(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchHouseNoRespCN_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespHouseNoInfoVRResultCN> msg(VR_new RespHouseNoInfoVRResultCN);
    m_VR_ReceiverCb->OnMessageNaviSearchHouseNoRespCN(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchAddressRespCN_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespAddressInfoVRResultCN> msg(VR_new RespAddressInfoVRResultCN);
    m_VR_ReceiverCb->OnMessageNaviSearchAddressRespCN(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviSearchPoiInfoResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RespCategorySearchVRResult> msg(VR_new RespCategorySearchVRResult);
    m_VR_ReceiverCb->OnMessageNaviSearchPoiInfoResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryPointCollectionResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PointResponse> msg(VR_new PointResponse);
    m_VR_ReceiverCb->OnMessageNaviQueryPointCollectionResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryDestHistoryResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PointResponse> msg(VR_new PointResponse);
    m_VR_ReceiverCb->OnMessageNaviQueryDestHistoryResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryRouteExistResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<RouteStatus> msg(VR_new RouteStatus);
    m_VR_ReceiverCb->OnMessageNaviQueryRouteExistResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryAllDestListResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PathPointList> msg(VR_new PathPointList);
    m_VR_ReceiverCb->OnMessageNaviQueryAllDestListResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryUnpassedDestListResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PathPointList> msg(VR_new PathPointList);
    m_VR_ReceiverCb->OnMessageNaviQueryUnpassedDestListResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryPointTypeResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<NearLinkResult> msg(VR_new NearLinkResult);
    m_VR_ReceiverCb->OnMessageNaviQueryPointTypeResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryNextDestResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<PathPointList> msg(VR_new PathPointList);
    m_VR_ReceiverCb->OnMessageNaviQueryNextDestResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviReqDfltInfoResp_Case)
{
#if defined(VR_DM_TARGET_VRUNIT)
    std::unique_ptr<DefaultInfoResult> msg(VR_new DefaultInfoResult);
    m_VR_ReceiverCb->OnMessageNaviReqDfltInfoResp(msg.get());
#endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryCurrentPosResp_Case)
{
// #if defined(VR_DM_TARGET_VRUNIT)
//    std::unique_ptr<DefaultInfoResult> msg(VR_new DefaultInfoResult);
//    m_VR_ReceiverCb->OnMessageNaviQueryCurrentPosResp(msg.get());
// #endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, OnMessageNaviQueryPOIByNameRespCN_Case)
{
// #if defined(VR_DM_TARGET_VRUNIT)
//    std::unique_ptr<RespPOINameListVRResultCN> msg(VR_new RespPOINameListVRResultCN);
//    m_VR_ReceiverCb->OnMessageNaviQueryPOIByNameRespCN(msg.get());
// #endif
    SUCCEED();
}

TEST_F(VR_ReceiverCb_Test, VR_CommandFactory_OtherCase_0001)
{

    SUCCEED();
}

/* EOF */
