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
#include   "gtest/gtest.h"

#include "VR_SessionManger.h"

#include <sstream>
#include <iomanip>
#include <boost/uuid/random_generator.hpp>

#include "VR_ConfigureIF.h"
#include "VR_EventSenderIF.h"
#include "VR_DialogEngineIF.h"
#include "action/VR_ActionEventSender.h"
#include "VR_ContentProvider.h"
#include "VR_DailogEngineMessageBuilder.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_DialogEngine_mock.h"
// #include "VR_AvcLanSender_mock.h"

#include "navi/Voice/VoiceRecog/RequestVrCreateSession.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCreateSessionResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrOpenSession.pb.h"
#include "navi/Voice/VoiceRecog/RespVrOpenSessionResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCloseSession.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCloseSessionResult.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifySessionClosed.pb.h"
#include "navi/Voice/VoiceRecog/RespVrVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/RespVrTSLResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"
// #include "VR_Common_VrUnitAgent.h"

using namespace navi::VoiceRecog;

/**
* VR_SessionManger_Test.cpp
*
* The class is just for VR_DailogEngineMessageBuilder test.
*/
class  VR_SessionManger_Test : public testing::Test
{
protected:

    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

    static VR_SessionManger*     m_VR_SessionManger;
};

VR_SessionManger* VR_SessionManger_Test::m_VR_SessionManger = NULL;

void
VR_SessionManger_Test::SetUpTestCase()
{
   //  spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_VR_SessionManger = VR_new  VR_SessionManger();
}

void
VR_SessionManger_Test::TearDownTestCase()
{
    if (m_VR_SessionManger) {
        delete  m_VR_SessionManger;
        m_VR_SessionManger = NULL;
    }
}

TEST_F(VR_SessionManger_Test, OnReqestGetSeessionID_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    std::unique_ptr<RequestVrCreateSession>  proto(RequestVrCreateSession::default_instance().New());
    m_VR_SessionManger->OnReqestGetSeessionID(*pcContext, *proto);

    EV_EventContext  context;
    RequestVrCreateSession  createSession;
    createSession.set_sender("dm");
    createSession.set_uitaskid(101);
    m_VR_SessionManger->OnReqestGetSeessionID(context, createSession);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnReqestOpenSeession_VrOpeResult_Invaild)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    std::unique_ptr<RequestVrOpenSession>  proto(RequestVrOpenSession::default_instance().New());
    m_VR_SessionManger->OnReqestOpenSeession(*pcContext, *proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnReqestOpenSeession_OpenSession)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    std::unique_ptr<RequestVrOpenSession>  proto(RequestVrOpenSession::default_instance().New());
    m_VR_SessionManger->OnReqestOpenSeession(*pcContext, *proto);

    EV_EventContext  context;
    RequestVrOpenSession   openSession;
    openSession.set_sender("VRUI");
    m_VR_SessionManger->m_spOpenEV_EventContext.reset();
    m_VR_SessionManger->OnReqestOpenSeession(context, openSession);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnReqestCloseSeession_VrOpeResult_Invaild)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    std::unique_ptr<RequestVrCloseSession>  proto(RequestVrCloseSession::default_instance().New());
    m_VR_SessionManger->OnReqestCloseSeession(*pcContext, *proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnReqestCloseSeession_CloseSession)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    std::unique_ptr<RequestVrCloseSession>  proto(RequestVrCloseSession::default_instance().New());
    m_VR_SessionManger->SetStartAgent(false);
    m_VR_SessionManger->m_CurSessionType = Session_VoiceRecog;
    // m_VR_SessionManger->SetTutorialCancel(false);
    m_VR_SessionManger->OnReqestCloseSeession(*pcContext, *proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnReqestCloseSeession_NoCancel2DE_GetStartAgent)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    std::unique_ptr<RequestVrCloseSession>  proto(RequestVrCloseSession::default_instance().New());
    m_VR_SessionManger->SetStartAgent(true);
    m_VR_SessionManger->m_CurSessionType = Session_VoiceRecog;
    // m_VR_SessionManger->SetTutorialCancel(false);
    m_VR_SessionManger->OnReqestCloseSeession(*pcContext, *proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnReqestCloseSeession_NoCancel2DE_SessionType)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    std::unique_ptr<RequestVrCloseSession>  proto(RequestVrCloseSession::default_instance().New());
    m_VR_SessionManger->SetStartAgent(false);
    m_VR_SessionManger->m_CurSessionType = Session_Tutorial;
    // m_VR_SessionManger->SetTutorialCancel(false);
    m_VR_SessionManger->OnReqestCloseSeession(*pcContext, *proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnReqestCloseSeession_NoCancel2DE_GetToTurialCancel)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    std::unique_ptr<RequestVrCloseSession>  proto(RequestVrCloseSession::default_instance().New());
    proto->set_session("123");
    m_VR_SessionManger->m_tmpSession = "123";
    m_VR_SessionManger->SetStartAgent(false);
    m_VR_SessionManger->m_CurSessionType = Session_VoiceRecog;
    // m_VR_SessionManger->SetTutorialCancel(true);
    m_VR_SessionManger->OnReqestCloseSeession(*pcContext, *proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnReqestCloseSession_OtherCase)
{
    EV_EventContext  context;
    RequestVrCloseSession   closeSession;
    m_VR_SessionManger->m_spCloseEV_EventContext.reset();
    // sourcestatus == vr_sourcestatus_openrecvstop
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_OpenRecvStop);
    m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);

    // sourcestatus == VR_SourceStatus_OpenRecvOff
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_OpenRecvOff);
    m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);

    // sourcestatus == VR_SourceStatus_AMPChanged
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_AMPChanged);
    m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);

    // sourcestatus == VR_SourceStatus_Idle
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);

    // other status -- VR_SourceStatus_Openning
    // VRUI closesession ,, sessiontype  == Session_VoiceRecog
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Openning);
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    closeSession.set_sender("VRUI");
    m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);

    m_VR_SessionManger->m_spCloseEV_EventContext.reset();
    // sessiontype other case
    m_VR_SessionManger->SetSessionType(Session_UnKnown);
    m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);

    m_VR_SessionManger->m_spCloseEV_EventContext.reset();
    closeSession.set_sender("TEST");
    m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_NoStatus)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_NoAudioName)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_status("on");
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_NoEqualAudioName)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_status("on");
    proto->set_audioname("aaa");
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_on_voiceTag_connected)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("on");
    m_VR_SessionManger->m_voiceTagSession = "voiceTagSession";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String session("123");
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_VR_SessionManger->SetCheckVoiceTagSession(pcContext, session, cXmlMsg, sp_EventSender, VrVoiceTagFunc_AddNewRecord, 1);
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_on_tsl_connected)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("on");
    m_VR_SessionManger->m_tslSession = "tslSession";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String session("123");
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_VR_SessionManger->SetCheckTSLSession(pcContext, session, cXmlMsg, spEngine);
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_on_vr)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("on");

    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_on_NoVr)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("On");

    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_off_NoCurrentSession)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("off");

    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_voiceTagSession = "voiceTagSession";
    m_VR_SessionManger->m_tslSession = "tslSession";
    m_VR_SessionManger->m_tmpSession = "tmpSession";
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->m_OpenningSession = session;
    VR_LOGD("m_spEventSenderIF=[%p]", m_VR_SessionManger->m_spEventSenderIF.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_off_CurrentSession)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("Off");

    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_CurrentSession = "m_CurrentSession";
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->m_OpenningSession = "session";
    VR_LOGD("m_spEventSenderIF=[%p]", m_VR_SessionManger->m_spEventSenderIF.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_off_FlagFalse)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("Off");

    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_CurrentSession = "m_CurrentSession";
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->m_OpenningSession = "session";
    VR_LOGD("m_spEventSenderIF=[%p]", m_VR_SessionManger->m_spEventSenderIF.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_off_m_OpenningSession)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("Off");

    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_CurrentSession = "m_CurrentSession";
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->m_OpenningSession = "";
    VR_LOGD("m_spEventSenderIF=[%p]", m_VR_SessionManger->m_spEventSenderIF.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_wait)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("wait");
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_continue)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("continue");
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_stop_voiceTagSession)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("stop");
    m_VR_SessionManger->m_voiceTagSession = "voiceTagSession";
    m_VR_SessionManger->m_nFuncCode = -1;
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_stop_voiceTagSession1)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("stop");
    m_VR_SessionManger->m_voiceTagSession = "voiceTagSession";
    m_VR_SessionManger->m_nFuncCode = 1;
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_stop_tslSession)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("stop");
    m_VR_SessionManger->m_tslSession = "tslSession";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_stop_OpenEV_EventContext)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("stop");
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_stop_NoOpenEV_EventContext)
{
    std::unique_ptr<VrAudioModeChageNotify>  proto(VrAudioModeChageNotify::default_instance().New());
    proto->set_audioname(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY.getString());
    proto->set_status("stop");
    m_VR_SessionManger->OnNotifyStatusChanged(*proto);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_OtherCase)
{
    VrAudioModeChageNotify  changeNofity;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    changeNofity.set_audioname("TEST");
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    changeNofity.set_audioname("0x85.VoiceRecognitionDisplay");
    changeNofity.set_status("on");
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Closing);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    //
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Openning);
    m_VR_SessionManger->m_voiceTagSession = "my name is test";
    VR_ConfigureIF::Instance()->setVRCancelStatus(true);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    VR_ConfigureIF::Instance()->setVRCancelStatus(false);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    // bconnect == false
    VR_PRODUCT_TYPE productType = VR_ConfigureIF::Instance()->getVRProduct();
    VR_ConfigureIF::Instance()->setVRProduct(VR_PRODUCT_TYPE_T0);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->m_tslEventContext.reset();
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    VR_ConfigureIF::Instance()->setVRProduct(productType);
    EV_EventContext context;
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(&context);

    // m_voicetagsession is empty
    m_VR_SessionManger->m_voiceTagSession = "";
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->m_strSender = "TEST";
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status_OK;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status_NG;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    // off
    changeNofity.set_status("off");
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Openning);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Closing);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status_Undef;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->m_CurrentSession = "";
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);
    m_VR_SessionManger->m_CurrentSession = "this is only a test";

    // stop
    changeNofity.set_status("stop");

    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status_Undef;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status_NG;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);

    m_VR_SessionManger->m_strSender = "Others";
    m_VR_SessionManger->m_tslSession = "my name is a test";

    VR_PRODUCT_TYPE  oldProductType = VR_ConfigureIF::Instance()->getVRProduct();
    VR_ConfigureIF::Instance()->setVRProduct(VR_PRODUCT_TYPE_T0);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);
    m_VR_SessionManger->m_tslEventContext.reset();
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(&context);


    VR_ConfigureIF::Instance()->setVRProduct(oldProductType);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNofity);
    SUCCEED();
}


TEST_F(VR_SessionManger_Test,  ResetState_Case)
{
    nutshell::NCString audioName("aaaaaa");
    nutshell::NCString audioStatus("bbbbbb");

    m_VR_SessionManger->CreateSession(m_VR_SessionManger->m_OpenningSession);
    m_VR_SessionManger->m_tslSession = "test";
    m_VR_SessionManger->ResetState(audioName, audioStatus);

    m_VR_SessionManger->m_tslSession = "";
    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->ResetState(audioName, audioStatus);

    m_VR_SessionManger->m_strSender = "Others";
    m_VR_SessionManger->ResetState(audioName, audioStatus);

    m_VR_SessionManger->m_OpenningSession = "this is only a test";
    m_VR_SessionManger->ResetState(audioName, audioStatus);
    SUCCEED();
}


TEST_F(VR_SessionManger_Test, SendQuitMsg2DE_Case)
{
    m_VR_SessionManger->m_bQuitMsg = true;
    m_VR_SessionManger->SendQuitMsg2DE();

    m_VR_SessionManger->m_bQuitMsg = false;
    m_VR_SessionManger->m_bAudioClosing = true;
    m_VR_SessionManger->SendQuitMsg2DE();

    m_VR_SessionManger->m_bAudioClosing = false;
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->SendQuitMsg2DE();

    m_VR_SessionManger->SetSessionType(Session_TSL);
    m_VR_SessionManger->SendQuitMsg2DE();

    m_VR_SessionManger->ResetCancelStatus();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, VRCurrentAudioSource_Case)
{
    nutshell::NCString  audioName("0x85.VoiceRecognitionDisplay");

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->VRCurrentAudioSource(audioName);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Closing);
    m_VR_SessionManger->VRCurrentAudioSource(audioName);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_AMPChanged);
    m_VR_SessionManger->VRCurrentAudioSource(audioName);


    nutshell::NCString  audioTestName("test");
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Openning);
    m_VR_SessionManger->VRCurrentAudioSource(audioTestName);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Opened);
    m_VR_SessionManger->VRCurrentAudioSource(audioTestName);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->VRCurrentAudioSource(audioTestName);
    SUCCEED();
}


TEST_F(VR_SessionManger_Test, VRAskAudioStatus_Case)
{
    m_VR_SessionManger->m_CurSessionType = Session_VoiceRecog;
    nutshell::NCString  audioName("0x85.VoiceRecognitionDisplay");

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->VRAskAudioStatus(audioName);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Openning);
    m_VR_SessionManger->VRAskAudioStatus(audioName);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Opened);
    m_VR_SessionManger->VRAskAudioStatus(audioName);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Closing);
    m_VR_SessionManger->VRAskAudioStatus(audioName);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_OpenRecvStop);
    m_VR_SessionManger->VRAskAudioStatus(audioName);
    SUCCEED();
}


TEST_F(VR_SessionManger_Test, OnAudioStatusNotify_Case)
{
    VrAudioStatusNotify  statusNoitfy;
    statusNoitfy.set_type(CurrentAudioSource);
    statusNoitfy.mutable_currentaudiosource()->set_audioname("0x85.VoiceRecognitionDisplay");
    m_VR_SessionManger->OnAudioStatusNotify(statusNoitfy);

    statusNoitfy.set_type(AskAudioStatus);
    statusNoitfy.mutable_askaudiostatus()->set_audioname("0x85.VoiceRecognitionDisplay");
    m_VR_SessionManger->OnAudioStatusNotify(statusNoitfy);

    statusNoitfy.set_type(NotifyAMPStsChanged);
    statusNoitfy.mutable_notifyampstschanged()->set_audioname("0x85.VoiceRecognitionDisplay");
    statusNoitfy.mutable_notifyampstschanged()->set_connect(true);
    m_VR_SessionManger->OnAudioStatusNotify(statusNoitfy);

    statusNoitfy.set_type(NotifyRequestAudioOff);
    statusNoitfy.mutable_notifyrequestaudiooff()->set_audioname("0x85.VoiceRecognitionDisplay");
    m_VR_SessionManger->OnAudioStatusNotify(statusNoitfy);

    statusNoitfy.set_type(NotifyCurAudioSource);
    statusNoitfy.mutable_notifycuraudiosource()->set_audioname("0x85.VoiceRecognitionDisplay");
    statusNoitfy.mutable_notifycuraudiosource()->set_curaudioname("test");
    m_VR_SessionManger->OnAudioStatusNotify(statusNoitfy);

    statusNoitfy.set_type(NotifyCheckAudioSource);
    statusNoitfy.mutable_notifycheckaudiosource()->set_audioname("test");
    statusNoitfy.mutable_notifycheckaudiosource()->set_connect(true);
    m_VR_SessionManger->OnAudioStatusNotify(statusNoitfy);

    VrAudioStatusNotify  noTypeNoitfy;
    m_VR_SessionManger->OnAudioStatusNotify(noTypeNoitfy);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, SetVehileInfoResp_Case)
{
    m_VR_SessionManger->SetSessionType(Session_VoiceTag);
    m_VR_SessionManger->SetVehileInfoResp();

    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->SetVehileInfoResp();

    m_VR_SessionManger->GetVehileInfoResp();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, notifyCheckAudioSource_Case)
{
    nutshell::NCString  audioName("0x85.VoiceRecognitionDisplay");
    m_VR_SessionManger->notifyCheckAudioSource(audioName, true);
    m_VR_SessionManger->notifyCheckAudioSource(audioName, false);

    audioName = "TEST";
    m_VR_SessionManger->notifyCheckAudioSource(audioName, false);
    SUCCEED();
}


TEST_F(VR_SessionManger_Test, notifyRequestAudioOff_Case)
{
    nutshell::NCString  audioName("0x85.VoiceRecognitionDisplay");
    nutshell::NCString  curraudioName("0x85.VoiceRecognitionDisplay");
    VR_PRODUCT_TYPE  oldProductType = VR_ConfigureIF::Instance()->getVRProduct();
    VR_ConfigureIF::Instance()->setVRProduct(VR_PRODUCT_TYPE_T0);
    m_VR_SessionManger->notifyRequestAudioOff(audioName);
    m_VR_SessionManger->notifyCurAudioSource(audioName, curraudioName);
    m_VR_SessionManger->notifyAMPStsChanged(audioName, true);
    m_VR_SessionManger->notifyAMPStsChanged(audioName, false);

    VR_ConfigureIF::Instance()->setVRProduct(oldProductType);
    m_VR_SessionManger->notifyRequestAudioOff(audioName);

    m_VR_SessionManger->VRNotifyRequestAudioOff(audioName);
    m_VR_SessionManger->notifyAMPStsChanged(audioName, true);
    m_VR_SessionManger->notifyAMPStsChanged(audioName, false);
    audioName = "TEST";
    m_VR_SessionManger->VRNotifyRequestAudioOff(audioName);

    m_VR_SessionManger->notifyCurAudioSource(audioName, curraudioName);

    m_VR_SessionManger->SendNotifyCurAudioSource(audioName, curraudioName);
    m_VR_SessionManger->RespServerVrStart(0);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, RespTslCancelRecognition_Case)
{
    m_VR_SessionManger->SetSessionType(Session_TSL);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_OpenRecvStop);
    m_VR_SessionManger->RespTslCancelRecognition();

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_OpenRecvOff);
    m_VR_SessionManger->RespTslCancelRecognition();

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->RespTslCancelRecognition();

    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->RespTslCancelRecognition();

    // VRNotifyCurAudioSource
    nutshell::NCString  audioName("0x85.VoiceRecognitionDisplay");
    nutshell::NCString  curraudioName("0x85.VoiceRecognitionDisplay");

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->VRNotifyCurAudioSource(audioName, curraudioName);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Closing);
    m_VR_SessionManger->VRNotifyCurAudioSource(audioName, curraudioName);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Opened);
    m_VR_SessionManger->VRNotifyCurAudioSource(audioName, curraudioName);

    audioName = "TEST";
    curraudioName = "TEST";
    m_VR_SessionManger->VRNotifyCurAudioSource(audioName, curraudioName);

    m_VR_SessionManger->GetCurAudioSource();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, VRNotifyAMPStsChanged_Case)
{

    nutshell::NCString  audioName("0x85.VoiceRecognitionDisplay");
    nutshell::NC_BOOL   bConnect(nutshell::NC_FALSE);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Openning);
    m_VR_SessionManger->VRNotifyAMPStsChanged(audioName, bConnect);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Opened);
    m_VR_SessionManger->VRNotifyAMPStsChanged(audioName, bConnect);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Closing);
    m_VR_SessionManger->VRNotifyAMPStsChanged(audioName, bConnect);
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->VRNotifyAMPStsChanged(audioName, bConnect);

    bConnect = nutshell::NC_TRUE;
    m_VR_SessionManger->VRNotifyAMPStsChanged(audioName, bConnect);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, ReplyOpenSession_Case)
{
    EV_EventContext  context;
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
    m_VR_SessionManger->ReplyOpenSession(VrOpeResult_Undefined);
    m_VR_SessionManger->ReplyOpenSession(VrOpeResult_Undefined);

    bool  bVRCancleStatus = VR_ConfigureIF::Instance()->getVRCancelStatus();
    m_VR_SessionManger->ReplyOpenSession(VrOpeResult_OK);

    VR_ConfigureIF::Instance()->setVRCancelStatus(false);
    m_VR_SessionManger->ReplyOpenSession(VrOpeResult_OK);

    VR_ConfigureIF::Instance()->setVRCancelStatus(bVRCancleStatus);
    m_VR_SessionManger->ReplyOpenSession(VrOpeResult_OK);
    SUCCEED();
}


TEST_F(VR_SessionManger_Test, SetPrepareState_Case)
{
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Openning);
    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status::VR_Status_AMPChanged;
    m_VR_SessionManger->SetPrepareState(0);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Opened);
    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status::VR_Status_OK;
    m_VR_SessionManger->SetPrepareState(0);

    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status::VR_Status_NG;
    m_VR_SessionManger->SetPrepareState(0);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->SetPrepareState(0);

    m_VR_SessionManger->GetCancelStatus();

    m_VR_SessionManger->SetCancelStatus(VR_SessionManger::VR_TslAppCancelStatus::VR_TslAppStatus_Cancel);

    VR_ContentProvider  vr_cp;
    std::string strValue = vr_cp.GetContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE + "?avsetting_mute_vr");
    vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, "avsetting_mute_vr", "on");
    m_VR_SessionManger->SetMuteVR();
    m_VR_SessionManger->SetUnMuteVR();
    vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, "avsetting_mute_vr", strValue);
    m_VR_SessionManger->SetMuteVR();


    m_VR_SessionManger->SendMessage2Engine("test");
    spVR_DialogEngineIF  spDialogEninge(m_VR_SessionManger->m_spVREngineIF);
    m_VR_SessionManger->m_spVREngineIF.reset();
    m_VR_SessionManger->SendMessage2Engine("test");
    m_VR_SessionManger->m_spVREngineIF = spDialogEninge;
    m_VR_SessionManger->SendMessage2Engine("test");


    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_AMPChanged);
    EV_EventContext  context;
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus_Idle);
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_Case)
{
     m_VR_SessionManger->m_CurrentSession = "TEST";
     m_VR_SessionManger->m_strSender = "VRUI";
     m_VR_SessionManger->SetSessionType(Session_VoiceTag);
     m_VR_SessionManger->OpenSession("TEST");

     m_VR_SessionManger->SetSessionType(Session_TSL);
     m_VR_SessionManger->m_tslSession = "TEST";
     m_VR_SessionManger->OpenSession("TEST");

     m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
     m_VR_SessionManger->m_strSender = "Others";

     EV_EventContext  context;
     m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
     m_VR_SessionManger->m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
     m_VR_SessionManger->OpenSession("TEST");

     m_VR_SessionManger->SetSessionType(Session_Tutorial);
     m_VR_SessionManger->m_strSender = "VRUI";

     m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
     m_VR_SessionManger->m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
     m_VR_SessionManger->OpenSession("TEST");

     m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
     m_VR_SessionManger->m_tslSession = "TEST";
     m_VR_SessionManger->m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
     m_VR_SessionManger->OpenSession("TEST");

     m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
     m_VR_SessionManger->m_strSender = "VRUI";
     m_VR_SessionManger->OpenSession("TEST");
     SUCCEED();
}

TEST_F(VR_SessionManger_Test, NotifyVoiceTagResult_AddRecord)
{
    VR_String session;
    m_VR_SessionManger->m_nFuncCode = VrVoiceTagFunc_AddNewRecord;
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_cXmlMsg = "";
    m_VR_SessionManger->NotifyVoiceTagResult();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, NotifyVoiceTagResult_EditRecord)
{
    VR_String session;
    m_VR_SessionManger->m_nFuncCode = VrVoiceTagFunc_EditRecord;
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_cXmlMsg = "<event name=\"editRecordVoiceTag\"><voiceTagId>1</voiceTagId></event>";
    m_VR_SessionManger->NotifyVoiceTagResult();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, NotifyVoiceTagResult_play)
{
    VR_String session;
    m_VR_SessionManger->m_nFuncCode = VrVoiceTagFunc_Play;
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_cXmlMsg = "<event name=\"editRecordVoiceTag\"><voiceTagId>1</voiceTagId></event>";
    m_VR_SessionManger->NotifyVoiceTagResult();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, NotifyVoiceTagResult_ptop)
{
    VR_String session;
    m_VR_SessionManger->m_nFuncCode = VrVoiceTagFunc_Stop;
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_cXmlMsg = "<event name=\"editRecordVoiceTag\"><voiceTagId>1</voiceTagId></event>";
    m_VR_SessionManger->NotifyVoiceTagResult();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, GetLastSession_Case)
{
    VR_String session;
    m_VR_SessionManger->m_SessionBuff.push_back("12345656");
    m_VR_SessionManger->GetLastSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, GetLastSession_NoSession)
{
    VR_String session;
    m_VR_SessionManger->GetLastSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, isCurrentSession_Case)
{
    m_VR_SessionManger->isCurrentSession("session");
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, isCurrentSession_empty)
{
    m_VR_SessionManger->isCurrentSession("");
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, isCurrentSession_equal)
{
    m_VR_SessionManger->m_SessionBuff.push_back("12345656");
    m_VR_SessionManger->isCurrentSession("12345656");
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, isCurrentSession_equal1)
{
    m_VR_SessionManger->m_CurrentSession = "12345656";
    m_VR_SessionManger->isCurrentSession("12345656");
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, currentAudioSource_Case)
{
    nutshell::NCString audioName = nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY;
    m_VR_SessionManger->currentAudioSource(audioName);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, notifyStatusChanged_Case)
{
    nutshell::NCString audioName = nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY;
    nutshell::NCString status = "on";
    m_VR_SessionManger->notifyStatusChanged(audioName, status);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, askAudioStatus_Case)
{
    nutshell::NCString audioName = nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY;
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->askAudioStatus(audioName);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, askAudioStatus_else)
{
    nutshell::NCString audioName = nutshell::SUBAUDIO_UIEGUIDANCEMODE;
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->askAudioStatus(audioName);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, askAudioStatus_empty)
{
    nutshell::NCString audioName = nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY;
    m_VR_SessionManger->m_CurrentSession = "";
    m_VR_SessionManger->askAudioStatus(audioName);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, CreateSession_Case)
{
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_CurrentSessionNotEmpty)
{
    VR_String session("1232143");
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_ClosedSessionNotEmpty)
{
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->m_ClosedSession = "123";
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_reqAudioSource)
{
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_NoCur)
{
    VR_String session("123");
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_sessionEmpty)
{
    VR_String session("");
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_flagTrue)
{
    VR_String session("");
    m_VR_SessionManger->m_OpenningSession = "true";
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_Session_UnKnown)
{
    VR_String session("");
    m_VR_SessionManger->m_CurSessionType = Session_UnKnown;
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_Session_VoiceTag)
{
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->m_CurSessionType = Session_VoiceTag;
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->m_voiceTagSession = "123";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_VR_SessionManger->SetCheckVoiceTagSession(pcContext, session, cXmlMsg, sp_EventSender, VrVoiceTagFunc_AddNewRecord, 1);
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_Session_VoiceTag0)
{
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->m_CurSessionType = Session_VoiceTag;
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->m_voiceTagSession = "123";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_VR_SessionManger->SetCheckVoiceTagSession(pcContext, session, cXmlMsg, sp_EventSender, -1, 1);
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_Session_VoiceTag1)
{
    VR_String session("aa");
    m_VR_SessionManger->m_CurSessionType = Session_VoiceTag;
    m_VR_SessionManger->m_CurrentSession = "123";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_VR_SessionManger->SetCheckVoiceTagSession(pcContext, session, cXmlMsg, sp_EventSender, VrVoiceTagFunc_AddNewRecord, 1);
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_Session_VoiceTag2)
{
    VR_String session;
    m_VR_SessionManger->CreateSession(session);
    m_VR_SessionManger->m_CurSessionType = Session_VoiceTag;
    m_VR_SessionManger->m_CurrentSession = "123";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_VR_SessionManger->SetCheckVoiceTagSession(pcContext, session, cXmlMsg, sp_EventSender, VrVoiceTagFunc_AddNewRecord, 1);
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_voiceTagSession.clear();
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_Session_VoiceTag3)
{
    VR_String session("aa");
    m_VR_SessionManger->m_CurSessionType = Session_VoiceTag;
    m_VR_SessionManger->m_CurrentSession = "123";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_VR_SessionManger->SetCheckVoiceTagSession(pcContext, session, cXmlMsg, sp_EventSender, VrVoiceTagFunc_AddNewRecord, 1);
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_voiceTagSession.clear();
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession_Session_tsl)
{
    VR_String session("aa");
    m_VR_SessionManger->m_CurSessionType = Session_TSL;
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, OpenSession)
{
    VR_String session("aa");
    m_VR_SessionManger->m_CurSessionType = Session_TSL;
    m_VR_SessionManger->OpenSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, CloseSession_false)
{
    VR_String session("123");
    m_VR_SessionManger->CloseSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, CloseSession_NoEqual)
{
    VR_String session("123");
    m_VR_SessionManger->m_tmpSession = "1";
    m_VR_SessionManger->CloseSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, CloseSession_true)
{
    VR_String session("123");
    m_VR_SessionManger->m_tmpSession = "123";
    m_VR_SessionManger->CloseSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, RealCloseSession_false)
{
    VR_String session("123");
    m_VR_SessionManger->RealCloseSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, RealCloseSession_NoEqual)
{
    VR_String session("1231");
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->RealCloseSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, RealCloseSession_true)
{
    VR_String session("123");
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->RealCloseSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, RealCloseSession_Session_VoiceRecog)
{
    VR_String session("123");
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->m_CurSessionType = Session_VoiceRecog;
    m_VR_SessionManger->RealCloseSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, ReqAudioSource_diffType)
{
    m_VR_SessionManger->SetSessionType(Session_UnKnown);
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, ReqAudioSource_reqAudioSource)
{
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->m_CurrentSession = "123";
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, ReqAudioSource_cfmAudioStatus_OpenContext)
{
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->m_CurrentSession = "123";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_bCfmAudio = true;
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, ReqAudioSource_cfmAudioStatus_VoiceTagContext)
{
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->m_CurrentSession = "123";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_bCfmAudio = true;
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, ReqAudioSource_cfmAudioStatus_TSLContext)
{
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->m_CurrentSession = "123";
    spEV_EventContext pcContext(VR_new EV_EventContext());
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(pcContext.get());
    m_VR_SessionManger->m_bCfmAudio = true;
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, ReqAudioSource_cfmAudioStatus)
{
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->m_bCfmAudio = true;
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, ReqAudioSource_other)
{
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->m_bCfmAudio = false;
    m_VR_SessionManger->ReqAudioSource(Session_VoiceRecog);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, GetStartAgent_Case)
{
    m_VR_SessionManger->GetStartAgent();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, SetCheckVoiceTagSession_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String session("123");
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    m_VR_SessionManger->SetCheckVoiceTagSession(pcContext, session, cXmlMsg, sp_EventSender, VrVoiceTagFunc_AddNewRecord, 1);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, SetCheckTSLSession_Case)
{
    spEV_EventContext pcContext(VR_new EV_EventContext());
    VR_String session("123");
    VR_String cXmlMsg("111");
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_VR_SessionManger->SetCheckTSLSession(pcContext, session, cXmlMsg, spEngine);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, FindSession_Case)
{
    VR_String session("123");
    m_VR_SessionManger->FindSession(session);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, SetMuteVR_Case)
{
    // m_VR_SessionManger->m_bSetMuteVR = false;
    m_VR_SessionManger->SetMuteVR();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, SetMuteVR_true)
{
    // m_VR_SessionManger->m_bSetMuteVR = true;
    m_VR_SessionManger->SetMuteVR();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, SetUnMuteVR_Case)
{
    // m_VR_SessionManger->m_bSetMuteVR = true;
    m_VR_SessionManger->SetUnMuteVR();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, SetUnMuteVR_false)
{
    // m_VR_SessionManger->m_bSetMuteVR = false;
    m_VR_SessionManger->SetUnMuteVR();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, GetAudioName)
{
    m_VR_SessionManger->GetAudioName(Session_UnKnown);
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, CheckAudioSource)
{
    m_VR_SessionManger->CheckAudioSource();
    SUCCEED();
}

TEST_F(VR_SessionManger_Test, notifyCheckAudioSource)
{
    m_VR_SessionManger->notifyCheckAudioSource(nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY, true);
    m_VR_SessionManger->notifyCheckAudioSource("VR_AUDIONAME_STR", true);
    SUCCEED();
}

// add for sessionmanager , 8/19
TEST_F(VR_SessionManger_Test, OnReqestCloseSeession_OtherCase_Opened)
{
    if (m_VR_SessionManger->m_spCloseEV_EventContext.get()) {
        m_VR_SessionManger->m_spCloseEV_EventContext.reset();
    }

    if (m_VR_SessionManger->GetSourceStatus() == VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Idle) {
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Opened);
    }

    EV_EventContext  context;
    RequestVrCloseSession   closeSession;
    // sender == VRUI
    closeSession.set_sender("VRUI");
    closeSession.set_session("Test");
    if (m_VR_SessionManger->GetSessionType() == Session_VoiceTag) {
        m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);
        m_VR_SessionManger->SetSessionType(Session_Tutorial);
        m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);
    }
    else {
        m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);
        // session_voicetag
        m_VR_SessionManger->SetSessionType(Session_VoiceTag);
        m_VR_SessionManger->OnReqestCloseSeession(context, closeSession);
    }

    SUCCEED();
}


TEST_F(VR_SessionManger_Test, OnNotifyStatusChanged_OtherCase_00)
{
    VrAudioModeChageNotify   changeNotify;
    changeNotify.set_audioname("0x85.VoiceRecognitionDisplay");
    changeNotify.set_status("on");

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Openning);
    m_VR_SessionManger->m_voiceTagSession = "This is only a test.";
    m_VR_SessionManger->m_nFuncCode = 0;
    // m_nFuncCode != -1
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);
    m_VR_SessionManger->m_nFuncCode = -1;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);

    m_VR_SessionManger->m_bTtsCrash = true;
    m_VR_SessionManger->m_nFuncCode = 0;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);
    m_VR_SessionManger->m_nFuncCode = -1;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);

    m_VR_SessionManger->m_voiceTagSession = "";
    m_VR_SessionManger->m_tslSession = "this is only a test.";

    // getVRCancelStatus == true
    VR_ConfigureIF::Instance()->setVRCancelStatus(true);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);

    m_VR_SessionManger->m_bTtsCrash = true;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);

    //
    m_VR_SessionManger->m_voiceTagSession = "";
    m_VR_SessionManger->m_tslSession = "";
    m_VR_SessionManger->m_spOpenEV_EventContext.reset();
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);
    EV_EventContext  context;
    m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);

    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);
    m_VR_SessionManger->m_strSender = "TEST";
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);
    m_VR_SessionManger->m_bTtsCrash = false;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);
    m_VR_SessionManger->m_bTtsCrash = true;
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);
    m_VR_SessionManger->SetSessionType(Session_Tutorial);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);
    m_VR_SessionManger->SetSessionType(Session_UnKnown);
    m_VR_SessionManger->OnNotifyStatusChanged(changeNotify);

    SUCCEED();
}


TEST_F(VR_SessionManger_Test, ResetState_OtherCase_00)
{
    //
    std::string  strSession("");
    m_VR_SessionManger->CreateSession(strSession);
    m_VR_SessionManger->m_OpenningSession = strSession;

    nutshell::NCString  strAudioName = "Test";
    nutshell::NCString  strStatus = "Test";
    m_VR_SessionManger->m_bTtsCrash = true;
    m_VR_SessionManger->ResetState(strAudioName, strStatus);
    m_VR_SessionManger->m_bTtsCrash = false;
    m_VR_SessionManger->ResetState(strAudioName, strStatus);

    m_VR_SessionManger->m_tslSession = "";
    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->ResetState(strAudioName, strStatus);
    m_VR_SessionManger->m_strSender = "Test";
    m_VR_SessionManger->ResetState(strAudioName, strStatus);

    SUCCEED();
}

TEST_F(VR_SessionManger_Test, SendQuitMsg2DE_OtherCase_00)
{
     // m_bQuitMsg = false
    m_VR_SessionManger->m_bQuitMsg = false;
    m_VR_SessionManger->m_bAudioClosing = false;

    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->SendQuitMsg2DE();

    m_VR_SessionManger->SetSessionType(Session_TSL);
    m_VR_SessionManger->SendQuitMsg2DE();

    m_VR_SessionManger->SetSessionType(Session_VoiceTag);
    m_VR_SessionManger->SendQuitMsg2DE();

    m_VR_SessionManger->SetSessionType(Session_Tutorial);
    m_VR_SessionManger->SendQuitMsg2DE();

    SUCCEED();
}


TEST_F(VR_SessionManger_Test, ReplyOpenSessionForTtsCrash_OtherCase_00)
{
    // voicetag session
    m_VR_SessionManger->m_voiceTagSession = "TEST";
    VR_REGION_TYPE  regionType = VR_ConfigureIF::Instance()->getVRContry();
    VR_PRODUCT_TYPE productType = VR_ConfigureIF::Instance()->getVRProduct();

    VR_ConfigureIF::Instance()->setVRProduct(VR_PRODUCT_TYPE_T0);
    m_VR_SessionManger->m_nFuncCode = 0;
    m_VR_SessionManger->ReplyOpenSessionForTtsCrash();
    m_VR_SessionManger->m_nFuncCode = -1;
    m_VR_SessionManger->ReplyOpenSessionForTtsCrash();

    VR_ConfigureIF::Instance()->setVRProduct(VR_PRODUCT_TYPE_T2);
    VR_ConfigureIF::Instance()->setVRContry(VR_REGION_US);
    m_VR_SessionManger->ReplyOpenSessionForTtsCrash();

    VR_ConfigureIF::Instance()->setVRProduct(productType);
    VR_ConfigureIF::Instance()->setVRContry(regionType);

    // tsl session
    m_VR_SessionManger->m_voiceTagSession = "";
    m_VR_SessionManger->m_tslSession = "TEST";

    m_VR_SessionManger->m_tslEventContext.reset();
    m_VR_SessionManger->ReplyOpenSessionForTtsCrash();
    EV_EventContext  context;
    m_VR_SessionManger->m_tslEventContext = VR_EventSenderIF::CopyEventContext(&context);
    m_VR_SessionManger->ReplyOpenSessionForTtsCrash();

    m_VR_SessionManger->m_voiceTagSession = "";
    m_VR_SessionManger->m_tslSession = "";
    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->ReplyOpenSessionForTtsCrash();
    m_VR_SessionManger->m_strSender = "TEST";
    m_VR_SessionManger->ReplyOpenSessionForTtsCrash();

    VR_ConfigureIF::Instance()->setVRContry(regionType);
    VR_ConfigureIF::Instance()->setVRProduct(productType);

    SUCCEED();
}


TEST_F(VR_SessionManger_Test, TtsBlinkReset_OtherCase_00)
{
    // bTtsCrash = false
    m_VR_SessionManger->TtsBlinkReset(false);

    //
    for (int nStatus = 1; nStatus <= 7; ++nStatus) {
        m_VR_SessionManger->SetSourceStatus((VR_SessionManger::VR_SourceStatus)nStatus);
        m_VR_SessionManger->m_bAudioClosing = true;
        m_VR_SessionManger->TtsBlinkReset(true);
        m_VR_SessionManger->m_bAudioClosing = false;
        for (int nSessionType = 0; nSessionType <= 4; ++nSessionType) {
            m_VR_SessionManger->SetSessionType((VrSessionType)nSessionType);
            m_VR_SessionManger->TtsBlinkReset(true);
        }
    }

    m_VR_SessionManger->IsTslCancel();

    SUCCEED();
}


TEST_F(VR_SessionManger_Test, VRNotifyAMPStsChanged_OtherCase_00)
{
    //
    nutshell::NCString  ncAduioName = "0x85.VoiceRecognitionDisplay";
    nutshell::NC_BOOL   ncConnect =  nutshell::NC_FALSE;

    // VR_SourceStatus_Openning
    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Openning);
    m_VR_SessionManger->m_tslSession = "TEST";
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);
    m_VR_SessionManger->m_tslSession = "";
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

    // VRUI
    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status::VR_Status_AMPChanged;
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);
    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status::VR_Status_Undef;
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

    m_VR_SessionManger->m_strSender = "TEST";
    m_VR_SessionManger->m_voiceTagSession = "TEST";
    m_VR_SessionManger->m_nFuncCode = 0;
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);
    m_VR_SessionManger->m_nFuncCode = -1;
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);
    m_VR_SessionManger->m_voiceTagSession = "";
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

    m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Opened);
    m_VR_SessionManger->m_tslSession = "TEST";
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

    m_VR_SessionManger->m_tslSession = "";
    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

     m_VR_SessionManger->m_strSender = "TEST";
     m_VR_SessionManger->m_voiceTagSession = "TEST";
     m_VR_SessionManger->m_nFuncCode = -1;
     m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

     m_VR_SessionManger->m_nFuncCode = 0;
     m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

     ncConnect = nutshell::NC_TRUE;
     m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

     ncAduioName = "TEST";
     ncConnect = nutshell::NC_TRUE;
     m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

     ncConnect = nutshell::NC_FALSE;
     m_VR_SessionManger->VRNotifyAMPStsChanged(ncAduioName, ncConnect);

     SUCCEED();
}


TEST_F(VR_SessionManger_Test, OpenSession_OtherCase_00)
{
    //
    m_VR_SessionManger->SetSessionType(Session_VoiceTagTts);
    m_VR_SessionManger->m_CurrentSession = "TEST";
    m_VR_SessionManger->m_voiceTagSession = "TEST";

    EV_EventContext  context;
    std::string  strSession = "TEST";
    VR_PRODUCT_TYPE  oldProduct = VR_ConfigureIF::Instance()->getVRProduct();
    VR_REGION_TYPE   oldRegion  = VR_ConfigureIF::Instance()->getVRContry();

    VR_ConfigureIF::Instance()->setVRProduct(VR_PRODUCT_TYPE_T0);

    for (int i = -1; i <= 0; ++i) {
        m_VR_SessionManger->m_nFuncCode = i;
        m_VR_SessionManger->OpenSession(strSession);

        // m_bttscrash
        m_VR_SessionManger->m_bTtsCrash = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bTtsCrash = false;
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_AMPChanged);
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Idle);
        m_VR_SessionManger->m_bRequestAudioOff = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bRequestAudioOff = false;
        m_VR_SessionManger->m_voiceTag_EventContext.reset();
        m_VR_SessionManger->OpenSession(strSession);
        EV_EventContext  context;
        m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_VR_SessionManger->OpenSession(strSession);
    }

    // Session_VoiceTag
    m_VR_SessionManger->SetSessionType(Session_VoiceTag);
    for (int i = -1; i <= 0; ++i) {
        m_VR_SessionManger->m_nFuncCode = i;
        m_VR_SessionManger->OpenSession(strSession);

        // m_bttscrash
        m_VR_SessionManger->m_bTtsCrash = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bTtsCrash = false;
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_AMPChanged);
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Idle);
        m_VR_SessionManger->m_bRequestAudioOff = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bRequestAudioOff = false;
        m_VR_SessionManger->m_voiceTag_EventContext.reset();
        m_VR_SessionManger->OpenSession(strSession);
        EV_EventContext  context;
        m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_VR_SessionManger->OpenSession(strSession);
    }

    // Session_VoiceTagTts
     m_VR_SessionManger->SetSessionType(Session_VoiceTagTts);
     m_VR_SessionManger->m_strSender = "VRUI";
     for (int i = -1; i <= 0; ++i) {
         m_VR_SessionManger->m_nFuncCode = i;
         m_VR_SessionManger->OpenSession(strSession);

         // m_bttscrash
         m_VR_SessionManger->m_bTtsCrash = true;
         m_VR_SessionManger->OpenSession(strSession);
         m_VR_SessionManger->m_bTtsCrash = false;
         m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_AMPChanged);
         m_VR_SessionManger->OpenSession(strSession);
         m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Idle);
         m_VR_SessionManger->m_bRequestAudioOff = true;
         m_VR_SessionManger->OpenSession(strSession);
         m_VR_SessionManger->m_bRequestAudioOff = false;
         m_VR_SessionManger->m_voiceTag_EventContext.reset();
         m_VR_SessionManger->OpenSession(strSession);
         EV_EventContext  context;
         m_VR_SessionManger->m_voiceTag_EventContext = VR_EventSenderIF::CopyEventContext(&context);
         m_VR_SessionManger->OpenSession(strSession);
     }

    // Session_VoiceRecog
    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->m_strSender = "Others";
    m_VR_SessionManger->m_spCloseEV_EventContext.reset();
    m_VR_SessionManger->OpenSession(strSession);
    m_VR_SessionManger->m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
    m_VR_SessionManger->OpenSession(strSession);

    for (int i = -1; i <= 0; ++i) {
        m_VR_SessionManger->m_nFuncCode = i;
        m_VR_SessionManger->OpenSession(strSession);

        // m_bttscrash
        m_VR_SessionManger->m_bTtsCrash = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bTtsCrash = false;
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_AMPChanged);
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Idle);
        m_VR_SessionManger->m_bRequestAudioOff = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bRequestAudioOff = false;
        m_VR_SessionManger->m_voiceTag_EventContext.reset();
        m_VR_SessionManger->OpenSession(strSession);
        EV_EventContext  context;
        m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_VR_SessionManger->OpenSession(strSession);
    }

    // Session_Tutorial
    m_VR_SessionManger->SetSessionType(Session_Tutorial);
    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->m_spCloseEV_EventContext.reset();
    m_VR_SessionManger->OpenSession(strSession);
    m_VR_SessionManger->m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
    m_VR_SessionManger->OpenSession(strSession);

    for (int i = -1; i <= 0; ++i) {
        m_VR_SessionManger->m_nFuncCode = i;
        m_VR_SessionManger->OpenSession(strSession);

        // m_bttscrash
        m_VR_SessionManger->m_bTtsCrash = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bTtsCrash = false;
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_AMPChanged);
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Idle);
        m_VR_SessionManger->m_bRequestAudioOff = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bRequestAudioOff = false;
        m_VR_SessionManger->m_voiceTag_EventContext.reset();
        m_VR_SessionManger->OpenSession(strSession);
        EV_EventContext  context;
        m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_VR_SessionManger->OpenSession(strSession);
    }

    m_VR_SessionManger->SetSessionType(Session_VoiceRecog);
    m_VR_SessionManger->m_tslSession = "TEST";
    m_VR_SessionManger->m_spCloseEV_EventContext.reset();
    m_VR_SessionManger->OpenSession(strSession);
    m_VR_SessionManger->m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
    m_VR_SessionManger->OpenSession(strSession);

    for (int i = -1; i <= 0; ++i) {
        m_VR_SessionManger->m_nFuncCode = i;
        m_VR_SessionManger->OpenSession(strSession);

        // m_bttscrash
        m_VR_SessionManger->m_bTtsCrash = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bTtsCrash = false;
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_AMPChanged);
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->SetSourceStatus(VR_SessionManger::VR_SourceStatus::VR_SourceStatus_Idle);
        m_VR_SessionManger->m_bRequestAudioOff = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bRequestAudioOff = false;
        m_VR_SessionManger->m_voiceTag_EventContext.reset();
        m_VR_SessionManger->OpenSession(strSession);
        EV_EventContext  context;
        m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_VR_SessionManger->OpenSession(strSession);
    }

    // current session is empty
    m_VR_SessionManger->m_CurrentSession = "";
    m_VR_SessionManger->m_ClosedSession = "";
    m_VR_SessionManger->m_OpenningSession = "TEST";

    m_VR_SessionManger->CreateSession(strSession);
    {
        m_VR_SessionManger->m_bTtsCrash = true;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_bTtsCrash = false;
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_tslSession = "";
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_tslSession = "TEST";
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_strSender = "VRUI";
        m_VR_SessionManger->OpenSession(strSession);
        m_VR_SessionManger->m_strSender = "Others";
        m_VR_SessionManger->OpenSession(strSession);

        m_VR_SessionManger->OpenSession("TEST");
        m_VR_SessionManger->m_spOpenEV_EventContext.reset();
        EV_EventContext  context;
        m_VR_SessionManger->m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_VR_SessionManger->OpenSession("TEST");
        m_VR_SessionManger->m_tslSession = "";
        m_VR_SessionManger->OpenSession("TEST");
        m_VR_SessionManger->m_tslSession = "TEST";
        m_VR_SessionManger->OpenSession("TEST");
    }

    VR_ConfigureIF::Instance()->setVRContry(oldRegion);
    VR_ConfigureIF::Instance()->setVRProduct(oldProduct);

    SUCCEED();
}


TEST_F(VR_SessionManger_Test, SetUnMuteVR_OtherCase_00)
{
    VR_ContentProvider  vr_cp;
    vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, "avsetting_mute_vr_set", "on");
    m_VR_SessionManger->SetUnMuteVR();

    vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, "avsetting_mute_vr", "on");
    m_VR_SessionManger->SetUnMuteVRByRealValue();

    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status::VR_Status_OK;
    m_VR_SessionManger->m_strSender = "TEST";
    m_VR_SessionManger->m_bTtsCrash = true;
    m_VR_SessionManger->CheckReplyState();
    m_VR_SessionManger->m_bTtsCrash = false;
    m_VR_SessionManger->CheckReplyState();

    m_VR_SessionManger->m_strSender = "VRUI";
    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status::VR_Status_OK;
    m_VR_SessionManger->m_nPrepareState = VR_SessionManger::VR_Status::VR_Status_OK;
    m_VR_SessionManger->m_bTtsCrash = true;
    m_VR_SessionManger->CheckReplyState();
    m_VR_SessionManger->m_bTtsCrash = false;
    m_VR_SessionManger->CheckReplyState();

    m_VR_SessionManger->m_nSessionState = VR_SessionManger::VR_Status::VR_Status_NG;
    m_VR_SessionManger->m_nPrepareState = VR_SessionManger::VR_Status::VR_Status_NG;
    m_VR_SessionManger->m_bTtsCrash = true;
    m_VR_SessionManger->CheckReplyState();
    m_VR_SessionManger->m_bTtsCrash = false;
    m_VR_SessionManger->CheckReplyState();

    SUCCEED();
}

/* EOF */
