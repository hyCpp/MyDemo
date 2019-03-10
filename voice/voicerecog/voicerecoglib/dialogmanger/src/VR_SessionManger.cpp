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
#include "VR_SessionManger.h"

#include <sstream>
#include <iomanip>
#include <boost/format.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/assign/list_of.hpp>
#include "pugixml.hpp"

#include "ncore/NCFile.h"
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
#include "NMAudioSetting.h"

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
#include "navi/Voice/VoiceRecog/VrNotifyVoiceTagResult.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"

#include "navi/Voice/VoiceRecog/RequestSDLVr.pb.h"
#include "navi/Voice/VoiceRecog/RespSDLVrResult.pb.h"

#include "navi/Voice/VoiceRecog/RequestMMVr.pb.h"
#include "navi/Voice/VoiceRecog/RespMMVrResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrStartRecognition.pb.h"

// #include "VR_Common_VrUnitAgent.h"

using namespace navi::VoiceRecog;
using namespace nutshell;

const int VR_MAX_SESSION_CACHE = 10;
const nutshell::NCString VR_AUDIONAME_STR = nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY;
// const nutshell::NCString TSL_AUDIONAME_STR = nutshell::SUBAUDIO_CENTERVOICERECOGNITION;
const nutshell::NCString TSL_AUDIONAME_STR = nutshell::SUBAUDIO_VOICERECOGNITIONDISPLAY;
const nutshell::NCString MIC_AUDIONAME_STR = nutshell::SUBAUDIO_MICINPUT;
typedef VoiceMap<navi::VoiceRecog::VrSessionType, nutshell::NCString>::type VR_SESSION_MAP;
static const VR_SESSION_MAP sessionTypeMap = boost::assign::map_list_of
        (Session_VoiceRecog, VR_AUDIONAME_STR)
        (Session_VoiceTag, VR_AUDIONAME_STR)
        (Session_TSL, TSL_AUDIONAME_STR)
        (Session_Tutorial, VR_AUDIONAME_STR)
        (Session_VoiceTagTts, VR_AUDIONAME_STR);

const static VR_String sessionStateChanged = "<event name=\"sessionStateChanged\">"
                                                  "<state>%1%</state>"
                                              "</event>";
const static VR_INT32 nVoiceTagTtsType = -1;

VR_SessionManger::VR_SessionManger()
    : m_CurrentSession()
    , m_SessionBuff(VR_MAX_SESSION_CACHE)
    , m_spNCSubAudioManager(VR_new nutshell::NCSubAudioManager)
    , m_spEventSenderIF(VR_EventSenderIF::GetSender())
    , m_bStartAgent(false)
    , m_bCfmAudio(false)
    , m_bCloseSession(true)
    , m_bPvr(false)
    , m_CurSessionType(Session_UnKnown)
    , m_NextSessionType(Session_UnKnown)
    , m_nActionId(-1)
    , m_bAudioClosing(false)
    , m_nDomainType(-1)
    , m_nSessionState(VR_Status_Undef)
    , m_nPrepareState(VR_Status_Undef)
    , m_emSourceStatus(VR_SourceStatus_Idle)
    , m_bQuitMsg(false)
    , m_bRequestAudioOff(false)
    , m_bVehileInfoResp(false)
    , m_eSessionMode(VR_Mode)
    // amazon
    , m_bEngineInited(false)
    , m_bAudioIn(false)
    , m_emMicStatus(VR_SourceStatus_Idle)
    , m_bWithoutStart(false)
{
    VR_LOGD_FUNC();

    VR_ContentProvider vr_cp;
    VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
    VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
    if ("Nuance" != strEngineType) {
        typedef typename VoiceList<nutshell::NCString>::type VoiceList_NCString;
        VoiceList_NCString audio_name_list;
        audio_name_list.push_back(VR_AUDIONAME_STR);
        audio_name_list.push_back(MIC_AUDIONAME_STR);
        m_spNCSubAudioManager->registerAvm(audio_name_list, this);
    }
}

VR_SessionManger::~VR_SessionManger()
{
    VR_LOGD_FUNC();
    VR_ContentProvider vr_cp;
    VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
    VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
    if ("Nuance" != strEngineType) {
        typedef typename VoiceList<nutshell::NCString>::type VoiceList_NCString;
        VoiceList_NCString audio_name_list;
        audio_name_list.push_back(VR_AUDIONAME_STR);
        audio_name_list.push_back(MIC_AUDIONAME_STR);
        m_spNCSubAudioManager->unregister(audio_name_list);
    }
}

bool VR_SessionManger::OnReqestGetSeessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const RequestVrCreateSession &aVrReqCreateSession = (const RequestVrCreateSession &)msg;
    const std::string sender = aVrReqCreateSession.sender();

    VR_String session;
    this->CreateSession(session);

    std::unique_ptr<RespVrCreateSessionResult> replyMsg(RespVrCreateSessionResult::default_instance().New());
    replyMsg->set_sender(sender);
    replyMsg->set_session(session);

    if (aVrReqCreateSession.has_uitaskid()) {
        replyMsg->set_uitaskid(aVrReqCreateSession.uitaskid());
        VR_LOGI("uitaskid = [%d]", aVrReqCreateSession.uitaskid());
    }

    m_spEventSenderIF->Reply(context, replyMsg.release());
    return true;
}

bool VR_SessionManger::OnReqestOpenSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    const RequestVrOpenSession &aVrReqOpenSeession = (const RequestVrOpenSession &)msg;
    m_OpenningSession.clear();
    m_eSessionMode = VR_Mode;
    if (m_spOpenEV_EventContext.get()) {
        VR_LOGI("m_spOpenEV_EventContext.get()");
        std::unique_ptr<RespVrOpenSessionResult> replyMsg(RespVrOpenSessionResult::default_instance().New());
        replyMsg->set_session(aVrReqOpenSeession.session());
        replyMsg->set_result(VrOpeResult_Invaild);

        m_spEventSenderIF->Reply(context, replyMsg.release());
        const VR_String str = (boost::format(sessionStateChanged) % "off").str();
        SendMsg2DE(str);
    }
    else {
        m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_strSender = aVrReqOpenSeession.sender();
        m_nDomainType = aVrReqOpenSeession.type();
        if ("VRUI" == m_strSender) {
            SetUnitStatus(true);
        }
        bool bResult = this->OpenSession(aVrReqOpenSeession.session());
        VR_UNUSED_VAR(bResult);
    }
    return true;
}

bool VR_SessionManger::OnReqestCloseSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    const RequestVrCloseSession &aVrReqCloseSession = (const RequestVrCloseSession &)msg;

    if (m_spCloseEV_EventContext.get()) {
        VR_LOGI("m_spCloseEV_EventContext.get()");
        std::unique_ptr<RespVrCloseSessionResult> replyMsg(RespVrCloseSessionResult::default_instance().New());
        replyMsg->set_session(aVrReqCloseSession.session());
        replyMsg->set_result(VrOpeResult_Invaild);
        m_spEventSenderIF->Reply(context, replyMsg.release());
        const VR_String str = (boost::format(sessionStateChanged) % "off").str();
        SendMsg2DE(str);

    }
    else {
        if (VR_SourceStatus_OpenRecvStop == GetSourceStatus()) {
            ResetState(VR_AUDIONAME_STR, "stop");
        }
        else if (VR_SourceStatus_OpenRecvOff == GetSourceStatus()) {
            ResetState(VR_AUDIONAME_STR, "off");
        }
        else if (VR_SourceStatus_AMPChanged == GetSourceStatus()) {
            ResetState(VR_AUDIONAME_STR, "off");
        }
        else if (VR_SourceStatus_Idle != GetSourceStatus()) {
            m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);

            bool bSuccess = false;
            if ("VRUI" == m_strSender) {
                if (Session_VoiceRecog == GetSessionType()) {
                    VR_ConfigureIF::Instance()->setVRCancelStatus(false);
                    VR_LOGD("SendCancel2DE()");
                    const VR_String cancelXML = "<event name=\"cancel\" option=\"allQuit\"></event>";
                    SendMsg2DE(cancelXML);
                }
                else {
                    VR_ConfigureIF::Instance()->setVRCancelStatus(true);
                }
            }
            bSuccess = this->CloseSession(aVrReqCloseSession.session());
            if (!bSuccess) {
                VR_LOGI("bSuccess=[false]");
                std::unique_ptr<RespVrCloseSessionResult> replyMsg(RespVrCloseSessionResult::default_instance().New());
                replyMsg->set_session(aVrReqCloseSession.session());
                replyMsg->set_result(VrOpeResult_OK);

                // if open session ok then reply to ui
                // when  session opening, if off is coming, m_spOpenEV_EventContext be empty
                // then need not reply to ui
                if (m_spCloseEV_EventContext) {
                    m_spEventSenderIF->Reply(*m_spCloseEV_EventContext, replyMsg.release());
                    m_spCloseEV_EventContext.reset();
                }
                else {
                     VR_LOGI("m_spCloseEV_EventContext is empty");
                }
            }
        }
    }

    return true;
}

void VR_SessionManger::SendMsg2DE(const VR_String &msg)
{
    VR_LOGI("msg=[%s]", msg.c_str());
    std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_DM2DE);
    loopMessage->mutable_m_vrsendmsgdm2de()->set_msg(msg);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

bool VR_SessionManger::OnNotifyStatusChanged(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrAudioModeChageNotify &aVrAudioModeChageNotify = (const VrAudioModeChageNotify &)msg;
    if (!aVrAudioModeChageNotify.has_status() || !aVrAudioModeChageNotify.has_audioname()) {
        return false;
    }
    const nutshell::NCString audioName = aVrAudioModeChageNotify.audioname().c_str();
    const nutshell::NCString status = aVrAudioModeChageNotify.status().c_str();
    if (((aVrAudioModeChageNotify.audioname() == VR_AUDIONAME_STR.getString())
         || (aVrAudioModeChageNotify.audioname() == TSL_AUDIONAME_STR.getString()))) {
        const nutshell::NCString audioName = aVrAudioModeChageNotify.audioname().c_str();
        const nutshell::NCString status = aVrAudioModeChageNotify.status().c_str();
        VR_LOGD("step into if,state[%s],audioName=[%s]", status.getString(), audioName.getString());
        if (("on" == status) || ("On" == status)) {
            VR_LOGD("m_CurrentSession=[%s]", m_CurrentSession.c_str());
            if ((VR_SourceStatus_Idle == GetSourceStatus()) || VR_SourceStatus_Closing == GetSourceStatus()) {
                m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
                SetUnMuteVR();
                SetSourceStatus(VR_SourceStatus_Closing);
                m_spNCSubAudioManager->reqAudioSource(audioName, false);
                return true;
            }
            
            VR_LOGP("VR open audio session success");
            VR_LOGD("VR open audio session success");

            m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
            SetMuteVR();
            SetSourceStatus(VR_SourceStatus_Opened);
            if (!VR_ConfigureIF::Instance()->getVRCancelStatus()) {
                if (!m_bWithoutStart) {
                    if (m_strSuggestionXml.empty()) {
                        VR_String strMsg = VR_DialogEngineMessageBuilder::CreateStartDialogMsg(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
                        SendMessage2Engine(strMsg);
                    }
                    else {
                        SendMessage2Engine(m_strSuggestionXml);
                    }
                }
                else {
                    m_bWithoutStart = false;
                }
            }
            else {
                m_bWithoutStart = false;
                VR_ConfigureIF::Instance()->setVRCancelStatus(false);
                const VR_String cancelXML = "<event name=\"cancel\" option=\"allQuit\"></event>";
                SendMsg2DE(cancelXML);
                //                CloseChannel();
            }
            m_strSuggestionXml.clear();
            return true;
        }
        else if (("off" == status) || ("Off" == status)) {
            // VR_LOGP("audio session request VR confirm off");
            if ((VR_SourceStatus_Idle == GetSourceStatus()) || VR_SourceStatus_Openning == GetSourceStatus()) {
                SetSourceStatus(VR_SourceStatus_OpenRecvOff);
                m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
                m_strSuggestionXml.clear();
                const VR_String strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
                SendMessage2Engine(strMsg);
                return true;
            }

            m_strSuggestionXml.clear();
            if (VR_SourceStatus_Closing == GetSourceStatus()) {
                SetUnMuteVR();
                ResetState(VR_AUDIONAME_STR, status);
                if (m_bWithoutStart) {
                    m_bWithoutStart = false;
                    SetSourceStatus(VR_SourceStatus_Openning);
                    m_spNCSubAudioManager->reqAudioSource(VR_AUDIONAME_STR, true);
                }
            }
            else {
                m_bCfmAudio = true;
                const VR_String strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
                SendMessage2Engine(strMsg);
            }
            if (m_bEngineInited) {
                OnAudioInOpe(false);
            }

//            const VR_String strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
//            SendMessage2Engine(strMsg);
//            SetUnMuteVR();
//            ResetState(VR_AUDIONAME_STR, status);
            return true;
        }
        else if ("stop" == status) {
            SetSourceStatus(VR_SourceStatus_OpenRecvStop);
            const VR_String strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
            SendMessage2Engine(strMsg);
//            ResetState(VR_AUDIONAME_STR, status);
        }
        else if ("wait" == status) {
            m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
        }
        else {
            VR_LOGE("audioName=[%s] status=[%s]", audioName.getString(), status.getString());
        }
    }
    else if (audioName == MIC_AUDIONAME_STR.getString()) {
        if (("on" == status) || ("On" == status)) {
            VR_LOGD("m_CurrentSession=[%s]", m_CurrentSession.c_str());
            if ((VR_SourceStatus_Idle == GetMicStatus()) || VR_SourceStatus_Closing == GetMicStatus()) {
                m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
                SetMicStatus(VR_SourceStatus_Closing);
                m_spNCSubAudioManager->reqAudioSource(audioName, false);
                return true;
            }

            VR_LOGP("MIC open audio session success");
            VR_LOGD("MIC open audio session success");

            m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
            SetMicStatus(VR_SourceStatus_Opened);
            StartEchoCancel();
            if (m_bEngineInited) {
                OnAudioInOpe(true);
            }
        }
        else if (("off" == status) || ("Off" == status)) {
            if ((VR_SourceStatus_Idle == GetMicStatus()) || VR_SourceStatus_Openning == GetMicStatus()) {
                SetMicStatus(VR_SourceStatus_Idle);
                m_spNCSubAudioManager->reqAudioSource(audioName, false);
                return true;
            }

            if (m_bEngineInited) {
                OnAudioInOpe(false);
            }
            SetMicStatus(VR_SourceStatus_Idle);
            StopEchoCancel();
            m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
        }
        else if ("stop" == status) {
            if (m_bEngineInited) {
                OnAudioInOpe(false);
            }
            SetMicStatus(VR_SourceStatus_Idle);
            StopEchoCancel();
            m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
        }
        else if ("wait" == status) {
            if (m_bEngineInited) {
                OnAudioInOpe(false);
            }
            SetMicStatus(VR_SourceStatus_Idle);
            StopEchoCancel();
            m_spNCSubAudioManager->cfmAudioStatus(audioName, "wait");
        }
        else if ("continue" == status) {
            SetMicStatus(VR_SourceStatus_Openning);
            m_spNCSubAudioManager->reqAudioSource(audioName, true);
//            if (m_bEngineInited) {
//                OnAudioInOpe(true);
//            }
        }
        else {
            VR_LOGE("audioName=[%s] status=[%s]", audioName.getString(), status.getString());
        }
    }

    return true;
}

/************** Manual VR **********/
bool  VR_SessionManger::OnMMVRRequestGetSessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
     VR_LOGD_FUNC();
     const VrGetMMVrSession& aReqCreateSession = (const VrGetMMVrSession&)msg;

     VR_String strSession("");
     this->CreateSession(strSession);

     std::unique_ptr<RespMMVrResult> MMVRResult(RespMMVrResult::default_instance().New());
     MMVRResult->set_sender("dm");
     MMVRResult->set_funccode(VrMMFunc_CreateVrSession);
     VrCreateMMVrSessionResult* sessionResult = MMVRResult->mutable_createmmvrsessionresult();
     sessionResult->set_session(strSession);
     if (aReqCreateSession.has_userdata()) {
         sessionResult->set_userdata(aReqCreateSession.userdata());
     }
     m_spEventSenderIF->Reply(context, MMVRResult.release());
     return true;
}

bool  VR_SessionManger::OnMMVRRequestOpenSession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrOpenMMVrSession& aReqOpenSession = (const VrOpenMMVrSession&)msg;
    m_OpenningSession.clear();
    m_eSessionMode = VR_MM_Mode;

    if (m_spOpenEV_EventContext.get()) {
        VR_LOGD("m_spOpenEv_EventContext.get()");
        std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
        MMVRResult->set_sender("dm");
        MMVRResult->set_funccode(VrMMFunc_OpenVrSession);
        VrOpenMMVrSessionResult* openSessionResult = MMVRResult->mutable_openmmvrsessionresult();
        openSessionResult->set_operresult(VrOpeResult_Undefined);
        openSessionResult->set_session(aReqOpenSession.session());
        m_spEventSenderIF->Reply(context, MMVRResult.release());
        const VR_String str = (boost::format(sessionStateChanged) % "off").str();
        SendMsg2DE(str);
    }
    else {
        m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_strSender = "VRUI";
        SetUnitStatus(true);
        m_nDomainType = aReqOpenSession.domaintype();
        OpenSession(aReqOpenSession.session());
    }
    return true;
}

bool  VR_SessionManger::OnMMVRRequestCloseSession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrCloseMMVrSession& aReqCloseSession = (const VrCloseMMVrSession&)msg;

    if (m_spCloseEV_EventContext.get()) {
        std::unique_ptr<RespMMVrResult> MMVRResult(RespMMVrResult::default_instance().New());
        MMVRResult->set_sender("dm");
        MMVRResult->set_funccode(VrMMFunc_CloseVrSession);
        VrCloseMMVrSessionResult* closeSessionResult = MMVRResult->mutable_closemmvrsessionresult();
        closeSessionResult->set_operresult(VrOpeResult_Undefined);
        closeSessionResult->set_session(aReqCloseSession.session());
        m_spEventSenderIF->Reply(context, MMVRResult.release());
        const VR_String str = (boost::format(sessionStateChanged) % "off").str();
        SendMsg2DE(str);
    }

    if (VR_SourceStatus_OpenRecvStop == GetSourceStatus()) {
        ResetState(VR_AUDIONAME_STR, "stop");
    }
    else if (VR_SourceStatus_OpenRecvOff == GetSourceStatus()) {
        ResetState(VR_AUDIONAME_STR, "off");
    }
    else if (VR_SourceStatus_AMPChanged == GetSourceStatus()) {
        ResetState(VR_AUDIONAME_STR, "off");
    }
    else if (VR_SourceStatus_Idle != GetSourceStatus()) {
        m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);

        if ("VRUI" == m_strSender) {
            if (Session_VoiceRecog == GetSessionType()) {
                VR_ConfigureIF::Instance()->setVRCancelStatus(false);
                VR_LOGD("SendCancel2DE()");
                const VR_String cancelXML = "<event name=\"cancel\" option=\"allQuit\"></event>";
                SendMsg2DE(cancelXML);
            }
            else {
                VR_ConfigureIF::Instance()->setVRCancelStatus(true);
            }
        }
        bool bSuccess = CloseSession(aReqCloseSession.session());
        if (!bSuccess) {
            if (m_spCloseEV_EventContext.get()) {
                std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
                MMVRResult->set_sender("dm");
                MMVRResult->set_funccode(VrMMFunc_CloseVrSession);
                VrCloseMMVrSessionResult* closeSessionResult = MMVRResult->mutable_closemmvrsessionresult();
                closeSessionResult->set_operresult(VrOpeResult_OK);
                closeSessionResult->set_session(aReqCloseSession.session());
                m_spEventSenderIF->Reply(context, MMVRResult.release());
                m_spCloseEV_EventContext.reset();
            }
        }
    }
    return true;
}

/********************* SDL VR ********************/
bool  VR_SessionManger::OnSDLVRRequestGetSessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const SDLVrCreateSDLVrSession&  SDLCreateSession = (const SDLVrCreateSDLVrSession&)msg;

    VR_String strSession("");
    this->CreateSession(strSession);

    std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
    SDLVRResult->set_sender("dm");
    SDLVRResult->set_funccode(SDLVrFunc_CreateSDLVrSession);
    SDLVrCreateSDLVrSessionResult* sessionResult = SDLVRResult->mutable_createsdlvrsessionresult();

    sessionResult->set_session(strSession);
    if (SDLCreateSession.has_userdata()) {
        sessionResult->set_userdata(SDLCreateSession.userdata());
    }
    m_spEventSenderIF->Reply(context, SDLVRResult.release());
    return true;
}

bool  VR_SessionManger::OnSDLVRRequestOpenSession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const SDLVrOpenSDLVrSession &SDLOpenSession = (const SDLVrOpenSDLVrSession&)msg;
    m_OpenningSession.clear();
    m_eSessionMode = VR_SDL_Mode;

    if (m_spOpenEV_EventContext.get()) {
        VR_LOGD("m_spOpenEV_EventContext.get()");
        std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
        SDLVRResult->set_sender("dm");
        SDLVRResult->set_funccode(SDLVrFunc_OpenSDLVrSession);
        SDLVrOpenSDLVrSessionResult* sessionResult = SDLVRResult->mutable_opensdlvrsessionresult();
        sessionResult->set_session(SDLOpenSession.session());
        sessionResult->set_operresult(VrOpeResult_Invaild);
        m_spEventSenderIF->Reply(context, SDLVRResult.release());

        const VR_String str = (boost::format(sessionStateChanged) % "off").str();
        SendMsg2DE(str);
    }
    else {
        m_spOpenEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);
        m_strSender = "VRUI";
        SetUnitStatus(true);
        m_nDomainType = SDLOpenSession.domaintype();
        OpenSession(SDLOpenSession.session());
    }
    return true;
}

bool VR_SessionManger::OnSDLVRRequestCloseSession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const SDLVrCloseSDLVrSession& aReqCloseSession = (const SDLVrCloseSDLVrSession&)msg;

    if (m_spCloseEV_EventContext.get()) {
        VR_LOGI("m_spCloseEV_EventContext.get()");
        std::unique_ptr<RespSDLVrResult> SDLVRResult(RespSDLVrResult::default_instance().New());
        SDLVRResult->set_sender("dm");
        SDLVRResult->set_funccode(SDLVrFunc_CloseSDLVrSession);
        SDLVrCloseSDLVrSessionResult* sessionResult = SDLVRResult->mutable_closesdlvrsessionresult();
        sessionResult->set_session(aReqCloseSession.session());
        sessionResult->set_operresult(VrOpeResult_Invaild);
        m_spEventSenderIF->Reply(context, SDLVRResult.release());

        const VR_String str = (boost::format(sessionStateChanged) % "off").str();
        SendMsg2DE(str);
    }
    else {
        if (VR_SourceStatus_OpenRecvStop == GetSourceStatus()) {
            ResetState(VR_AUDIONAME_STR, "stop");
        }
        else if (VR_SourceStatus_OpenRecvOff == GetSourceStatus()) {
            ResetState(VR_AUDIONAME_STR, "off");
        }
        else if (VR_SourceStatus_AMPChanged == GetSourceStatus()) {
            ResetState(VR_AUDIONAME_STR, "off");
        }
        else if (VR_SourceStatus_Idle != GetSourceStatus()) {
            m_spCloseEV_EventContext = VR_EventSenderIF::CopyEventContext(&context);

            if ("VRUI" == m_strSender) {
                if (Session_VoiceRecog == GetSessionType()) {
                    VR_ConfigureIF::Instance()->setVRCancelStatus(false);
                    VR_LOGD("SendCancel2DE()");
                    const VR_String cancelXML = "<event name=\"cancel\" option=\"allQuit\"></event>";
                    SendMsg2DE(cancelXML);
                }
                else {
                    VR_ConfigureIF::Instance()->setVRCancelStatus(true);
                }
            }
            bool bSuccess = this->CloseSession(aReqCloseSession.session());
            if (!bSuccess) {
                if (m_spCloseEV_EventContext.get()) {
                    std::unique_ptr<RespSDLVrResult> replyMsg(RespSDLVrResult::default_instance().New());
                    replyMsg->set_sender("dm");
                    replyMsg->set_funccode(SDLVrFunc_CloseSDLVrSession);
                    SDLVrCloseSDLVrSessionResult* closeSession = replyMsg->mutable_closesdlvrsessionresult();
                    closeSession->set_session(aReqCloseSession.session());
                    closeSession->set_operresult(VrOpeResult_OK);
                    m_spEventSenderIF->Reply(*m_spCloseEV_EventContext, replyMsg.release());
                    m_spCloseEV_EventContext.reset();
                }
            }
        }
    }
    return true;
}

void VR_SessionManger::NotifyVoiceTagResult()
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::ResetState(const nutshell::NCString &audioName, const nutshell::NCString &status)
{
    VR_LOGD_FUNC();
    if (m_spCloseEV_EventContext.get()) {
        VR_LOGD("m_spCloseEV_EventContext.get()");
        ResponseVRCloseSessionResult(m_ClosedSession, VrOpeResult_OK);
        m_spCloseEV_EventContext.reset();
        VR_LOGD("m_spEventSenderIF->Reply(*m_spCloseEV_EventContext, replyMsg.release())");
    }
    if (m_OpenningSession.empty()) {
        m_spOpenEV_EventContext.reset();
    }
    m_strSuggestionXml.clear();
    m_nSessionState = VR_Status_Undef;
    m_nPrepareState = VR_Status_Undef;
    m_bCfmAudio = false;
    SetUnMuteVR();
    m_spNCSubAudioManager->cfmAudioStatus(audioName, status);
    m_nSessionState = VR_Status_Undef;
    m_bAudioClosing = false;
    VR_ConfigureIF::Instance()->setVRCancelStatus(false);
    ResetCancelStatus();
    RespTslCancelRecognition();
    SetSessionType(Session_UnKnown);
    SetStartAgent(false);
    m_ClosedSession.clear();
    SetSourceStatus(VR_SourceStatus_Idle);
    SetUnitStatus(false);
    m_bQuitMsg = false;
    m_bRequestAudioOff = false;
    m_bVehileInfoResp = false;
    NotifyVoiceTagSuccess();
    SetTutorialCancel(false);
    if (!m_OpenningSession.empty()) {
        BufferFindResult result = this->FindSession(m_OpenningSession);
        VR_LOGD("!m_OpenningSession.empty()");
        if (result.first) {
            VR_LOGD("result.first && (!m_OpenningSession.empty())");
            if ("VRUI" == m_strSender) {
                SetUnitStatus(true);
                m_nPrepareState = VR_Status_Undef;
                VR_String strPrepare = VR_DialogEngineMessageBuilder::CreatePrepareDialogMsg(m_nDomainType);
                SendMessage2Engine(strPrepare);
            }
            m_spNCSubAudioManager->reqAudioSource(VR_AUDIONAME_STR, true);
            SetSourceStatus(VR_SourceStatus_Openning);
            m_SessionBuff.erase(result.second);
            m_CurrentSession = m_OpenningSession;
            m_tmpSession = m_OpenningSession;
            m_OpenningSession.clear();
        }
    }
    else {
        m_strSender.clear();
        m_nDomainType = -1;
    }
}

void VR_SessionManger::SendQuitMsg2DE()
{
    VR_LOGD("GetSessionType()=[%d]", GetSessionType());
    if (!m_bQuitMsg) {
        m_bQuitMsg = true;
        NotifySessionClosed();

        if (m_bAudioClosing) {
            return;
        }
        if (Session_VoiceRecog == GetSessionType()) {
            const VR_String cancelXML = "<event name=\"cancel\"></event>";
            SendMsg2DE(cancelXML);
        }
        else if (Session_TSL == GetSessionType()) {
            m_bRequestAudioOff = true;
            const VR_String strQuitXml = "<event name=\"CancelAppRecognition\"></event>";
            SendMsg2DE(strQuitXml);
        }
        else if (Session_Tutorial == GetSessionType()) {
            ReqAudioSource(Session_Tutorial);
        }
        else if (Session_VoiceTag == GetSessionType()) {
            const VR_String msg = "<event name=\"CancelVoiceTag\"></event>";
            SendMsg2DE(msg);
        }
    }
}

void VR_SessionManger::NotifySessionClosed()
{
    VR_LOGD_FUNC();
    const VR_String msg = "<event name=\"sessionOff\" reply=\"true\">"
                                           "</event>";
    SendMsg2DE(msg);

    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("dm sender");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyClosedSession);
    VrNotifyClosedSession* closedSession = noitfyProperty->mutable_closedsession();
    closedSession->set_session(m_tmpSession);
    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());

    std::unique_ptr<VrNotifySessionClosed> notifyMsg(VrNotifySessionClosed::default_instance().New());
    notifyMsg->set_sender(EV_MODULE_VOICERECOG);
    notifyMsg->set_session(m_tmpSession);
    notifyMsg->set_reason(VrNotifySessionClosed_VrReason_VrReason_SERVICECLOSE);
    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_NOTIFYSESSIONCLOSED, notifyMsg.release());
}

void VR_SessionManger::ReplyOpenSessionForTtsCrash()
{
}

void VR_SessionManger::ReplyTslStartApp()
{
}

void VR_SessionManger::NotifyTslResult(const VR_String &strXml)
{
}

bool VR_SessionManger::GetVoiceTagCancelStatus()
{
    return false;
}

void VR_SessionManger::ResetCancelStatus()
{
    VR_LOGD_FUNC();
}

bool VR_SessionManger::GetLastSession(VR_String &session)
{
    VR_LOGD("m_SessionBuff.empty()=[%d]", m_SessionBuff.empty());
    if (!m_SessionBuff.empty()) {
        session = m_SessionBuff.back();
    }

    return m_SessionBuff.empty();
}

bool VR_SessionManger::isCurrentSession(const VR_String &session)
{
    VR_LOGD("isCurrentSession()=[%d]", (!session.empty()) && (session == m_CurrentSession));
    return (!session.empty()) && (session == m_CurrentSession);
}

void VR_SessionManger::currentAudioSource(const nutshell::NCString &audioName)
{
    VR_LOGD_FUNC();
//    bool bConnStatus = VR_ConfigureIF::Instance()->getInnerAvcLanConnectStatus();
//    if (bConnStatus) {
//#if defined(VR_DM_TARGET_VRUNIT)
//        SendCurrentAudioSource(audioName);
//#elif defined(VR_DM_TARGET_VRAGENT)
//        CheckAudioSource();
//#endif
//    }
//    else {
//        SendCurrentAudioSource(audioName);
//        CheckAudioSource();
//    }
}

void VR_SessionManger::SendCurrentAudioSource(const nutshell::NCString &audioName)
{
    VR_LOGD("audioName=[%s]", audioName.getString());
    std::unique_ptr<VrLoopBack> notifyMsg(VrLoopBack::default_instance().New());
    notifyMsg->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_STATUS);
    notifyMsg->mutable_m_audio_status_notify()->set_type(CurrentAudioSource);
    notifyMsg->mutable_m_audio_status_notify()->mutable_currentaudiosource()->set_audioname(audioName.getString());

    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, notifyMsg.release());
}

void VR_SessionManger::VRCurrentAudioSource(const nutshell::NCString &audioName)
{
    VR_LOGI("audioName=[%s],m_emSourceStatus=[%d]", audioName.getString(), GetSourceStatus());
    if ((audioName == VR_AUDIONAME_STR) || (audioName == TSL_AUDIONAME_STR)) {
        if ((VR_SourceStatus_Idle == GetSourceStatus()) || (VR_SourceStatus_Closing == GetSourceStatus())) {
            SetUnMuteVR();
            VR_LOGI("VR_SourceStatus_Idle,reqAudioSource(audioName, false)");
            m_spNCSubAudioManager->reqAudioSource(audioName, false);
            SetSourceStatus(VR_SourceStatus_Closing);
        }
        else if (VR_SourceStatus_Openning == GetSourceStatus()) {
            SetUnMuteVR();
            VR_LOGI("VR_SourceStatus_Idle,reqAudioSource(audioName, false)");
            m_CurrentSession.clear();
            m_spNCSubAudioManager->reqAudioSource(audioName, false);
            SetSourceStatus(VR_SourceStatus_Closing);
        }
    }
    else {
        if ((VR_SourceStatus_Openning == GetSourceStatus()) || (VR_SourceStatus_Opened == GetSourceStatus())) {
            VR_LOGI("VR_SourceStatus_Openning || VR_SourceStatus_Opened,reqAudioSource(audioName, true)");
            m_spNCSubAudioManager->reqAudioSource(VR_AUDIONAME_STR, true);
            SetSourceStatus(VR_SourceStatus_Openning);
        }
    }
}

void VR_SessionManger::notifyStatusChanged(const nutshell::NCString &audioName, const nutshell::NCString &status)
{
    VR_LOGI("audioName=[%s],status=[%s]", audioName.getString(), status.getString());
    std::unique_ptr<VrLoopBack> notifyMsg(VrLoopBack::default_instance().New());
    notifyMsg->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_MODE);
    notifyMsg->mutable_m_audiomode()->set_audioname(audioName.getString());
    notifyMsg->mutable_m_audiomode()->set_status(status.getString());

    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, notifyMsg.release());
}

void VR_SessionManger::askAudioStatus(const nutshell::NCString &audioName)
{
    VR_LOGD_FUNC();
    bool bConnStatus = VR_ConfigureIF::Instance()->getInnerAvcLanConnectStatus();
    if (bConnStatus) {
#if defined(VR_DM_TARGET_VRUNIT)
        SendAskAudioStatus(audioName);
#endif
    }
    else {
        SendAskAudioStatus(audioName);
    }
}

void VR_SessionManger::SendAskAudioStatus(const nutshell::NCString &audioName)
{
    VR_LOGD("audioName=[%s]", audioName.getString());
    std::unique_ptr<VrLoopBack> notifyMsg(VrLoopBack::default_instance().New());
    notifyMsg->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_STATUS);
    notifyMsg->mutable_m_audio_status_notify()->set_type(AskAudioStatus);
    notifyMsg->mutable_m_audio_status_notify()->mutable_askaudiostatus()->set_audioname(audioName.getString());

    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, notifyMsg.release());
}

void VR_SessionManger::VRAskAudioStatus(const nutshell::NCString &audioName)
{
    VR_LOGI("audioName=[%s]", audioName.getString());

    if (VR_AUDIONAME_STR == audioName) {
        if (VR_SourceStatus_Idle == GetSourceStatus()) {
            SetUnMuteVR();
            VR_LOGI("VR_SourceStatus_Idle,reqAudioSource(audioName, false)");
            m_spNCSubAudioManager->reqAudioSource(audioName, false);
            SetSourceStatus(VR_SourceStatus_Closing);
        }
        else if (VR_SourceStatus_Openning == GetSourceStatus()) {
            VR_LOGI("VR_SourceStatus_Openning,cfmAudioStatus(audioName, continue)");
            m_spNCSubAudioManager->cfmAudioStatus(audioName, "continue");
        }
        else if (VR_SourceStatus_Opened == GetSourceStatus()) {
            VR_LOGI("VR_SourceStatus_Opened,cfmAudioStatus(audioName, continue)");
            m_spNCSubAudioManager->cfmAudioStatus(audioName, "continue");
        }
        else if (VR_SourceStatus_Closing == GetSourceStatus()) {
            VR_LOGI("VR_SourceStatus_Closing,reqAudioSource(audioName, false)");
            m_spNCSubAudioManager->cfmAudioStatus(audioName, "continue");
            SetUnMuteVR();
            m_spNCSubAudioManager->reqAudioSource(audioName, false);
            SetSourceStatus(VR_SourceStatus_Closing);
        }
    }
}

void VR_SessionManger::OnAudioStatusNotify(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrAudioStatusNotify &audioStatusNotify = (const VrAudioStatusNotify &)msg;
    if (audioStatusNotify.has_type()) {
        VR_LOGD("audioStatusNotify.type=[%d]", audioStatusNotify.type());
        if ((CurrentAudioSource == audioStatusNotify.type()) && audioStatusNotify.has_currentaudiosource()) {
            VRCurrentAudioSource(audioStatusNotify.currentaudiosource().audioname().c_str());
        }
        else  if ((AskAudioStatus == audioStatusNotify.type()) && audioStatusNotify.has_askaudiostatus()) {
            VRAskAudioStatus(audioStatusNotify.askaudiostatus().audioname().c_str());
        }
        else  if ((NotifyAMPStsChanged == audioStatusNotify.type()) && audioStatusNotify.has_notifyampstschanged()) {
            VRNotifyAMPStsChanged(audioStatusNotify.notifyampstschanged().audioname().c_str(), audioStatusNotify.notifyampstschanged().connect());
        }
        else  if ((NotifyRequestAudioOff == audioStatusNotify.type()) && audioStatusNotify.has_notifyrequestaudiooff()) {
            VRNotifyRequestAudioOff(audioStatusNotify.notifyrequestaudiooff().audioname().c_str());
        }
        else  if ((NotifyCurAudioSource == audioStatusNotify.type()) && audioStatusNotify.has_notifycuraudiosource()) {
            VRNotifyCurAudioSource(audioStatusNotify.notifycuraudiosource().audioname().c_str(), audioStatusNotify.notifycuraudiosource().curaudioname().c_str());
        }
    }
    else {
        VR_LOGD("VrAudioStatusNotify has no type");
    }
}

void VR_SessionManger::TtsBlinkReset(bool bTtsCrash)
{ 
}

void VR_SessionManger::SetVoiceTagStatus(VR_INT32 nVoiceTagId, bool bStatus)
{
}

void VR_SessionManger::OpenChannel(VR_String strXml)
{
    VR_LOGD_FUNC();

    if (VR_SourceStatus_Idle == GetSourceStatus()) {
        SetSourceStatus(VR_SourceStatus_Openning);
        m_spNCSubAudioManager->reqAudioSource(VR_AUDIONAME_STR, true);
        m_strSuggestionXml = strXml;
    }
    else if (VR_SourceStatus_Opened == GetSourceStatus()) {
        if (!strXml.empty()) {
             SendMessage2Engine(strXml);
        }
    }
    else if (VR_SourceStatus_Openning == GetSourceStatus()) {
        m_strSuggestionXml = strXml;
    }
    else {
        const VR_String strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
        SendMessage2Engine(strMsg);
    }
//    else if (VR_SourceStatus_Opened == GetSourceStatus()) {
//        if (!VR_ConfigureIF::Instance()->getVRCancelStatus()) {
//            VR_String strMsg = VR_DialogEngineMessageBuilder::CreateStartDialogMsg(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
//            SendMessage2Engine(strMsg);
//        }
////        else {
////            VR_ConfigureIF::Instance()->setVRCancelStatus(false);
////            CloseChannel();
////        }
//    }
}

void VR_SessionManger::CloseChannel()
{
    VR_LOGD_FUNC();
    if (m_bCfmAudio) {
        SetUnMuteVR();
        m_spNCSubAudioManager->cfmAudioStatus(VR_AUDIONAME_STR, "off");
        SetSourceStatus(VR_SourceStatus_Idle);
        m_bCfmAudio = false;
    }
    else if (VR_SourceStatus_Opened == GetSourceStatus()) {
        SetUnMuteVR();
        SetSourceStatus(VR_SourceStatus_Closing);
        m_spNCSubAudioManager->reqAudioSource(VR_AUDIONAME_STR, false);
    }
    else if ((VR_SourceStatus_OpenRecvStop == GetSourceStatus()) || (VR_SourceStatus_OpenRecvOff == GetSourceStatus()) || (VR_SourceStatus_AMPChanged == GetSourceStatus())) {
        ResetState(VR_AUDIONAME_STR, "off");
    }
    else if (VR_SourceStatus_Openning == GetSourceStatus()) {
        VR_ConfigureIF::Instance()->setVRCancelStatus(true);
    }
}

void VR_SessionManger::OnEngineInitedMsg()
{
    VR_LOGI("m_bEngineInited=[%d]", m_bEngineInited);
    m_bEngineInited = true;
    if (VR_SourceStatus_Opened == GetMicStatus()) {
        OnAudioInOpe(true);
    }
}

void VR_SessionManger::OnAudioInOpe(bool bOpen)
{
    VR_LOGI("bOpen=[%d],m_bAudioIn=[%d]", bOpen, m_bAudioIn);
    if (m_bAudioIn != bOpen) {
        m_bAudioIn = bOpen;
        VR_String strMsg = VR_DialogEngineMessageBuilder::CreateAudioInOpeMsg(bOpen);
        SendMessage2Engine(strMsg);
    }
}

void VR_SessionManger::OnNotifyCheckAudioSource(bool bStatus)
{
    VR_LOGI("bStatus=[%s]", bStatus);
    if (m_bEngineInited) {
        OnAudioInOpe(bStatus);
    }
}

void VR_SessionManger::OpenMic()
{
    VR_LOGD_FUNC();

    if (VR_SourceStatus_Idle == GetMicStatus()) {
        OnAudioInOpe(true);
        // SetMicStatus(VR_SourceStatus_Openning);
        // m_spNCSubAudioManager->reqAudioSource(MIC_AUDIONAME_STR, true);
    }
}

void VR_SessionManger::SetMicStatus(VR_SessionManger::VR_SourceStatus status)
{
    VR_LOGI("m_emMicStatus=[%d],status=[%d]", m_emMicStatus, status);
    m_emMicStatus = status;
}

void VR_SessionManger::StartEchoCancel()
{
    VR_LOGD_FUNC();
    // NMAudioSetting setting;
    // NMEchoCancelConfig ec_cfg;
    // memset(&ec_cfg, 0x00, sizeof(ec_cfg));
    // ec_cfg.format = EAUDIO_STREAM_FORMAT_TYPE_S16LE;
    // ec_cfg.rate = 16000;
    // ec_cfg.channels = 1;
    // ec_cfg.token = 888;
    // ec_cfg.input = EAUDIO_STREAM_DEVICE_TYPE_TOTAL_NUM;
    // ec_cfg.output = EAUDIO_STREAM_DEVICE_TYPE_TOTAL_NUM;
    // setting.startEchoCancel(NMAUDIO_ChannelId_Record, ec_cfg);
}

void VR_SessionManger::StopEchoCancel()
{
    VR_LOGD_FUNC();

    // NMAudioSetting setting;
    // setting.stopEchoCancel(NMAUDIO_ChannelId_Record);
}

VR_SessionManger::VR_SourceStatus VR_SessionManger::GetMicStatus()
{
    VR_LOGI("m_emMicStatus=[%d]", m_emMicStatus);
    return m_emMicStatus;
}

void VR_SessionManger::OpenChannelWithoutStart()
{
    VR_LOGD_FUNC();
    if (VR_SourceStatus_Idle == GetSourceStatus()) {
        SetSourceStatus(VR_SourceStatus_Openning);
        m_bWithoutStart = true;
        m_spNCSubAudioManager->reqAudioSource(VR_AUDIONAME_STR, true);
    }
    else if (VR_SourceStatus_Closing == GetSourceStatus()) {
        m_bWithoutStart = true;
    }
}

bool VR_SessionManger::IsTutorialSessionClosed()
{
    return false;
}

bool VR_SessionManger::GetVoiceTagStatus()
{
    return false;
}

void VR_SessionManger::NotifyVoiceTagSuccess()
{
}


void VR_SessionManger::SetTutorialCancel(bool bStatus)
{
}

bool VR_SessionManger::GetTutorialCancel()
{
    return false;
}

void VR_SessionManger::SetVehileInfoResp()
{
    if (Session_VoiceRecog == GetSessionType()) {
        m_bVehileInfoResp = true;
        VR_Status nState = CheckReplyState();
        if (VR_Status_AMPChanged == nState) {
            ReplyOpenSession(VrOpeResult_Undefined);
        }
        else if (VR_Status_OK == nState) {
            ReplyOpenSession(VrOpeResult_OK);
        }
        else if (VR_Status_NG == nState) {
            ReplyOpenSession(VrOpeResult_Invaild);
        }
    }
    VR_LOGI("m_bVehileInfoResp=[%d]", m_bVehileInfoResp);
}

bool VR_SessionManger::GetVehileInfoResp()
{
    VR_LOGI("m_bVehileInfoResp=[%d]", m_bVehileInfoResp);
    return m_bVehileInfoResp;
}

bool VR_SessionManger::IsTslCancel()
{
    return false;
}

void VR_SessionManger::SetVoiceTagCancelStatus(bool status)
{
}

void VR_SessionManger::notifyCheckAudioSource(const nutshell::NCString &audioName, nutshell::NC_BOOL result)
{
    VR_LOGI("audioName=[%s],result=[%d]", audioName.getString(), result);
    if (VR_AUDIONAME_STR == audioName) {
        bool bLowerInterFlg = result;
        bool bCurLowerFlg = VR_ConfigureIF::Instance()->getHigherInterruptStatus();
        if (bCurLowerFlg != bLowerInterFlg) {
            VR_ConfigureIF::Instance()->setHigherInterruptStatus(bLowerInterFlg);
            std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
            loopMessage->set_type(VrLoopBack_MSG_Type_MSG_HMIMODE_HIGHINTER_NOTIFY);
            loopMessage->mutable_m_vrhigherhmimodenoitfy();
            EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
        }
    }
}

void VR_SessionManger::notifyRequestAudioOff(const nutshell::NCString &audioName)
{
    VR_LOGI("audioName=[%s]", audioName.getString());
    bool bConnStatus = VR_ConfigureIF::Instance()->getInnerAvcLanConnectStatus();
    if (!bConnStatus) {
        std::unique_ptr<VrLoopBack> notifyMsg(VrLoopBack::default_instance().New());
        notifyMsg->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_STATUS);
        notifyMsg->mutable_m_audio_status_notify()->set_type(NotifyRequestAudioOff);
        notifyMsg->mutable_m_audio_status_notify()->mutable_notifyrequestaudiooff()->set_audioname(audioName.getString());

        m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, notifyMsg.release());
    }
}

void VR_SessionManger::VRNotifyRequestAudioOff(const nutshell::NCString &audioName)
{
    VR_LOGI("audioName=[%s]", audioName.getString());
    if (VR_AUDIONAME_STR == audioName) {
        m_bRequestAudioOff = true;
//        SendQuitMsg2DE();
        const VR_String strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
        SendMessage2Engine(strMsg);
//        CloseChannel();
    }
    else if (MIC_AUDIONAME_STR == audioName) {
        m_spNCSubAudioManager->reqAudioSource(audioName, false);
        SetMicStatus(VR_SourceStatus_Closing);
    }
}

void VR_SessionManger::notifyCurAudioSource(const nutshell::NCString &audioName, const nutshell::NCString &curAudioName)
{
//    VR_LOGI("audioName=[%s]", audioName.getString());

    bool bConnStatus = VR_ConfigureIF::Instance()->getInnerAvcLanConnectStatus();
    if (bConnStatus) {
#if defined(VR_DM_TARGET_VRUNIT)
        SendNotifyCurAudioSource(audioName, curAudioName);
#endif
    }
    else {
        SendNotifyCurAudioSource(audioName, curAudioName);
    }

}

void VR_SessionManger::SendNotifyCurAudioSource(const nutshell::NCString &audioName, const nutshell::NCString &curAudioName)
{
//    VR_LOGD_FUNC();
    std::unique_ptr<VrLoopBack> notifyMsg(VrLoopBack::default_instance().New());
    notifyMsg->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_STATUS);
    notifyMsg->mutable_m_audio_status_notify()->set_type(NotifyCurAudioSource);
    notifyMsg->mutable_m_audio_status_notify()->mutable_notifycuraudiosource()->set_audioname(audioName.getString());
    notifyMsg->mutable_m_audio_status_notify()->mutable_notifycuraudiosource()->set_curaudioname(curAudioName.getString());

    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, notifyMsg.release());

}

void VR_SessionManger::RespTslCancelRecognition()
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::RespServerVrStart(VR_INT32 nSuccess)
{
}

void VR_SessionManger::VRNotifyCurAudioSource(const nutshell::NCString &audioName, const nutshell::NCString &curAudioName)
{
    VR_LOGI("audioName=[%s],curAudioName=[%s]", audioName.getString(), curAudioName.getString());
     if ((curAudioName == VR_AUDIONAME_STR) || (curAudioName == TSL_AUDIONAME_STR)) {
         if ((VR_SourceStatus_Idle == GetSourceStatus()) || (VR_SourceStatus_Closing == GetSourceStatus())) {
             SetUnMuteVR();
             VR_LOGI("VR_SourceStatus_Idle,reqAudioSource(CENTER_AUDIONAME_STR, false)");
             m_spNCSubAudioManager->reqAudioSource(curAudioName, false);
             SetSourceStatus(VR_SourceStatus_Closing);
         }
     }
     else if (curAudioName == MIC_AUDIONAME_STR) {
             if ((VR_SourceStatus_Idle == GetMicStatus()) || (VR_SourceStatus_Closing == GetMicStatus())) {
                 VR_LOGI("VR_SourceStatus_Idle,reqAudioSource(MIC_AUDIONAME_STR, false)");
                 m_spNCSubAudioManager->reqAudioSource(curAudioName, false);
                 SetMicStatus(VR_SourceStatus_Closing);
             }
         }
     else {
         SetUnitStatus(false);
     }
}

void VR_SessionManger::GetCurAudioSource()
{
    VR_LOGD_FUNC();
    m_spNCSubAudioManager->getCurAudioSource(VR_AUDIONAME_STR);
    m_spNCSubAudioManager->getCurAudioSource(MIC_AUDIONAME_STR);
}

void VR_SessionManger::notifyAMPStsChanged(const nutshell::NCString &audioName, nutshell::NC_BOOL connect)
{
//    VR_LOGD_FUNC();
//    bool bConnStatus = VR_ConfigureIF::Instance()->getInnerAvcLanConnectStatus();
//    if (bConnStatus) {
//#if defined(VR_DM_TARGET_VRUNIT)
//        SendNotifyAMPStsChanged(audioName, connect);
//#endif
//    }
//    else {
//        SendNotifyAMPStsChanged(audioName, connect);
//    }
}

void VR_SessionManger::SendNotifyAMPStsChanged(const nutshell::NCString &audioName, nutshell::NC_BOOL connect)
{
    VR_LOGI("audioName=[%s]", audioName.getString());
    std::unique_ptr<VrLoopBack> notifyMsg(VrLoopBack::default_instance().New());
    notifyMsg->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_STATUS);
    notifyMsg->mutable_m_audio_status_notify()->set_type(NotifyAMPStsChanged);
    notifyMsg->mutable_m_audio_status_notify()->mutable_notifyampstschanged()->set_audioname(audioName.getString());
    notifyMsg->mutable_m_audio_status_notify()->mutable_notifyampstschanged()->set_connect(nutshell::NC_TRUE == connect);

    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, notifyMsg.release());
}

void VR_SessionManger::VRNotifyAMPStsChanged(const nutshell::NCString &audioName, nutshell::NC_BOOL connect)
{
    VR_LOGI("audioName=[%s],connect=[%d]", audioName.getString(), connect);
    if ((VR_AUDIONAME_STR == audioName) && (nutshell::NC_FALSE == connect)) {
        m_strSuggestionXml.clear();
        if (m_bEngineInited) {
            OnAudioInOpe(false);
        }
        if (VR_SourceStatus_Openning == GetSourceStatus()) {
            SetSourceStatus(VR_SourceStatus_AMPChanged);
            ResetState(audioName, "off");
        }
        else if (VR_SourceStatus_Opened == GetSourceStatus()) {
            SetSourceStatus(VR_SourceStatus_AMPChanged);
            const VR_String strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
            SendMessage2Engine(strMsg);
            ResetState(audioName, "off");
        }
        else if (VR_SourceStatus_Closing == GetSourceStatus()) {
            ResetState(audioName, "off");
        }
    }
    else if ((VR_AUDIONAME_STR == audioName) && (nutshell::NC_TRUE == connect)) {
        SetUnMuteVRByRealValue();
        if (m_bEngineInited) {
            OnAudioInOpe(true);
        }
    }
}

bool VR_SessionManger::CreateSession(VR_String &session)
{
    VR_LOGI("session=[%s]", session.c_str());
    VR_String identifier;
    boost::uuids::uuid uid = boost::uuids::random_generator()();
    std::stringstream stream;
    for (boost::uuids::uuid::const_iterator it = uid.begin(); it != uid.end(); ++it) {
        stream << std::setfill('0') << std::setw(2) << std::hex << (short) (*it);
    }

    identifier = stream.str();
    m_SessionBuff.push_back(identifier);
    session.swap(identifier);
    VR_LOGI("session=[%s]", session.c_str());
    return true;
}

bool VR_SessionManger::OpenSession(const VR_String &session)
{
    VR_LOGI("session=[%s],m_CurrentSession=[%s],m_CurSessionType=[%d],m_NextSessionType=[%d]", session.c_str()
                                                                                             , m_CurrentSession.c_str()
                                                                                             , GetSessionType()
                                                                                             , m_NextSessionType);

    if (Session_UnKnown == GetSessionType()
        && !m_CurrentSession.empty()) {
        VR_LOGD("*******************Session_UnKnown == GetSessionType()");
        m_CurrentSession.clear();
    }
    else if (!m_CurrentSession.empty()) {
        VR_LOGI("ready to close session, after session closed and open new session");
        m_OpenningSession = session;
        if ("VRUI" == m_strSender) {
            m_nSessionState = VR_Status_Undef;
            m_nPrepareState = VR_Status_Undef;
            VR_String strPrepare = VR_DialogEngineMessageBuilder::CreatePrepareDialogMsg(m_nDomainType);
            SendMessage2Engine(strPrepare);
        }
        this->RealCloseSession(m_CurrentSession);
        return true;
    }
    else if (!(m_ClosedSession.empty())) {
        VR_LOGI("closing session, after session closed and open new session");
        m_OpenningSession = session;
        return true;
    }
    if (m_OpenningSession.empty()) {
        BufferFindResult result = this->FindSession(session);
        if (result.first && (!session.empty())) {
            VR_LOGI("ready to open audio session");
            m_nSessionState = VR_Status_Undef;
            m_spNCSubAudioManager->reqAudioSource(VR_AUDIONAME_STR, true);
            SetSourceStatus(VR_SourceStatus_Openning);
            if ("VRUI" == m_strSender) {
                m_nPrepareState = VR_Status_Undef;
                VR_String strPrepare = VR_DialogEngineMessageBuilder::CreatePrepareDialogMsg(m_nDomainType);
                SendMessage2Engine(strPrepare);
            }
            m_SessionBuff.erase(result.second);
            m_CurrentSession = session;
            m_tmpSession = session;
            return true;
        }
        else {
            if (m_spOpenEV_EventContext.get()) {
                VR_LOGI("m_spOpenEV_EventContext.get()");
                ResponseVRSessionResult(session, VrOpeResult_Invaild);
            }
        }
        VR_LOGI("not find session or session is empty");
    }
    return false;
}

bool VR_SessionManger::CloseSession(const VR_String &session)
{
    VR_LOGI("session=[%s],m_CurrentSession=[%s]", session.c_str(), m_CurrentSession.c_str());
    if ((!m_tmpSession.empty())
        && (m_tmpSession == session)) {
        m_ClosedSession = session;
        return true;
    }
    else {
        VR_LOGI("return false");
        return false;
    }
}

bool VR_SessionManger::RealCloseSession(const VR_String &session)
{
    VR_LOGI("session=[%s],m_CurSessionType=[%d]", session.c_str(), GetSessionType());
    if ((!m_CurrentSession.empty())
        && (m_CurrentSession == session)) {
        m_ClosedSession = session;
        if (Session_UnKnown != GetSessionType()) {
            nutshell::NCString audioName = GetAudioName(GetSessionType());
            SetUnMuteVR();
            m_spNCSubAudioManager->reqAudioSource(audioName, false);
            SetSourceStatus(VR_SourceStatus_Closing);
            m_SessionBuff.push_back(m_CurrentSession);
            VR_LOGI("return true");
            m_CurrentSession.clear();
        }
        return true;
    }
    else {
        VR_LOGI("return false");
        return false;
    }
}

void VR_SessionManger::ReqAudioSource(const VrSessionType &type)
{
    VR_LOGI("m_CurSessionType=[%d], type=[%d]", GetSessionType(), type);
    if (VR_SourceStatus_AMPChanged == GetSourceStatus()) {
        if (m_spOpenEV_EventContext.get()) {
            std::unique_ptr<RespVrOpenSessionResult> replyMsg(RespVrOpenSessionResult::default_instance().New());
            replyMsg->set_session(m_CurrentSession);
            replyMsg->set_result(VrOpeResult_Invaild);
            m_spEventSenderIF->Reply(*m_spOpenEV_EventContext, replyMsg.release());
            m_spOpenEV_EventContext.reset();
        }
        ResetState(VR_AUDIONAME_STR, "off");
        return;
    }

    VR_LOGI("m_bCfmAudio=[%d],m_spOpenEV_EventContext=[%p]",
            m_bCfmAudio, m_spOpenEV_EventContext.get()
            );
    nutshell::NCString audioName("");
    audioName = GetAudioName(GetSessionType());
    if (!m_CurrentSession.empty()) {
        VR_LOGD("!m_CurrentSession.empty()");
        // VR_LOGP("Request close audio session ...");
        SetUnMuteVR();
        m_spNCSubAudioManager->reqAudioSource(audioName, false);
        m_bAudioClosing = true;
        SetSourceStatus(VR_SourceStatus_Closing);
        m_SessionBuff.push_back(m_CurrentSession);
        m_CurrentSession.clear();
    }
    else if (m_bCfmAudio) {
        VR_LOGD("audio session be interrupted, confirm off!");
        // VR_LOGP("audio session be interrupted, confirm off!");
        ResetState(audioName, "off");
    }
}

nutshell::NCString VR_SessionManger::GetAudioName(navi::VoiceRecog::VrSessionType sessionType)
{
    VR_LOGD("sessionType=[%d]", sessionType);
    nutshell::NCString audioName = nutshell::NCString("");
    VoiceMap<navi::VoiceRecog::VrSessionType, nutshell::NCString>::const_iterator it = sessionTypeMap.find(sessionType);
    if (it != sessionTypeMap.end()) {
        audioName = it->second;
    }
    return audioName;
}

void VR_SessionManger::SetDialogEngine(spVR_DialogEngineIF spDialogEngine)
{
    VR_LOGD("spDialogEngine.get()=[%p]", spDialogEngine.get());
    m_spVREngineIF = spDialogEngine;
}

bool VR_SessionManger::SendMessage2Engine(const std::string &msg)
{
    VR_LOGI("m_spVREngineIF=[%p], msg=[%s]",
           m_spVREngineIF.get(), msg.c_str());

    if (NULL != m_spVREngineIF) {
        m_spVREngineIF->SendMessage(msg);
    }
    return (NULL != m_spVREngineIF);
}

void VR_SessionManger::SetStartAgent(bool bStart)
{
    m_bStartAgent = bStart;
    VR_LOGI("m_bStartAgent=[%d]", m_bStartAgent);
}

bool VR_SessionManger::GetStartAgent()
{
    VR_LOGI("m_bStartAgent=[%d]", m_bStartAgent);
    return m_bStartAgent;
}

void VR_SessionManger::SetSessionType(const VrSessionType &type)
{
    m_CurSessionType = type;
    VR_LOGI("m_CurSessionType=[%d]", m_CurSessionType);
}

navi::VoiceRecog::VrSessionType VR_SessionManger::GetSessionType()
{
    VR_LOGI("m_CurSessionType=[%d]", m_CurSessionType);
    return m_CurSessionType;
}

// Set Check Session Name And Xml Message for current session check
void VR_SessionManger::SetCheckVoiceTagSession(spEV_EventContext context,
                                               const VR_String &cSession,
                                               const VR_String &cXmlMsg,
                                               spVR_ActionEventSender sender,
                                               int nFuncCode,
                                               int nActionId
                                               )
{
}

void VR_SessionManger::SetCheckTSLSession(spEV_EventContext context,
                                          const VR_String &cSession,
                                          const VR_String &cXmlMsg,
                                          spVR_DialogEngineIF sender
                                          )
{
}

VR_SessionManger::BufferFindResult VR_SessionManger::FindSession(const VR_String &session)
{
    VR_LOGI("session=[%s]", session.c_str());
    BufferIterator itr = std::find(m_SessionBuff.begin(), m_SessionBuff.end(), session);

    bool bFind = (itr != m_SessionBuff.end());

    return BufferFindResult(bFind, itr);
}


void VR_SessionManger::SetMuteVR()
{
//    VR_ContentProvider vr_cp;
//    VR_String strMute = vr_cp.GetContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE + "?avsetting_mute_vr_set");
//    VR_LOGI("strMute=[%s]", strMute.c_str());
//    if ("off" == strMute) {
//        VR_LOGI("VR_SessionManger::SetMuteVR");
//        vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, "avsetting_mute_vr_set", "on");
//         nutshell::NMAudioSetting nMAudioSetting;
//         nMAudioSetting.sessionStartEchoCancel(NMAUDIO_ChannelId_VoiceRecognition);
//         VR_LOGD("nutshell::NMAudioSetting::sessionStartEchoCancel(NMAUDIO_ChannelId_VoiceRecognition)");
//    }
}

void VR_SessionManger::SetUnMuteVR()
{
//    VR_ContentProvider vr_cp;
//    VR_String strMute = vr_cp.GetContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE + "?avsetting_mute_vr_set");
//    VR_LOGI("strMute=[%s]", strMute.c_str());
//    if ("on" == strMute) {
//         nutshell::NMAudioSetting nMAudioSetting;
//         nMAudioSetting.sessionStopEchoCancel(NMAUDIO_ChannelId_VoiceRecognition);
//         VR_LOGD("nutshell::NMAudioSetting::sessionStopEchoCancel(NMAUDIO_ChannelId_VoiceRecognition);");
//         vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, "avsetting_mute_vr_set", "off");
//  //       VR_LOGI("VR_SessionManger::SetUnMuteVR");
//    }
}

void VR_SessionManger::SetUnMuteVRByRealValue()
{
    VR_ContentProvider vr_cp;
    VR_String strMute = vr_cp.GetContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE + "?avsetting_mute_vr");
    VR_LOGI("SetUnMuteVRByRealValue :: strMute=[%s]", strMute.c_str());
    if ("on" == strMute) {
         nutshell::NMAudioSetting nMAudioSetting;
         nMAudioSetting.sessionStopEchoCancel(NMAUDIO_ChannelId_VoiceRecognition);
         VR_LOGD("nutshell::NMAudioSetting::sessionStopEchoCancel(NMAUDIO_ChannelId_VoiceRecognition);");
         vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, "avsetting_mute_vr", "off");
//         VR_LOGI("VR_SessionManger::SetUnMuteVRByRealValue");
    }
}

void VR_SessionManger::CheckAudioSource()
{
    VR_LOGD_FUNC();
    m_spNCSubAudioManager->checkAudioSource(VR_AUDIONAME_STR);
}

void VR_SessionManger::SetUnitStatus(bool status)
{
    VR_LOGI("status=[%d]", status);
    std::unique_ptr<VrLoopBack>    loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage*  controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_StartStatusResult);
    controlMessage->mutable_startstatusresult()->set_starttype(Session_Using);
    controlMessage->mutable_startstatusresult()->set_startreuslt(status);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

VR_SessionManger::VR_TslAppCancelStatus VR_SessionManger::GetCancelStatus()
{
    return VR_TslAppStatus_Cancel;
}

void VR_SessionManger::SetCancelStatus(VR_SessionManger::VR_TslAppCancelStatus status)
{
}

VR_SessionManger::VR_Status VR_SessionManger::CheckReplyState()
{
    VR_LOGI("m_strSender=[%s]", m_strSender.c_str());
    VR_Status state = VR_Status_Undef;
    if (VR_Status_AMPChanged == m_nSessionState) {
        state = VR_Status_AMPChanged;
    }
    else if ((VR_Status_OK == m_nSessionState) && (VR_Status_OK == m_nPrepareState)) {
        state = VR_Status_OK;
    }
    else if ((VR_Status_Undef != m_nSessionState) && (VR_Status_Undef != m_nPrepareState)) {
        state = VR_Status_NG;
    }
    else {
        state = VR_Status_Undef;
    }
    VR_LOGI("state=[%d]", state);
    return state;
}

void VR_SessionManger::SetPrepareState(VR_INT32 state)
{
    VR_LOGI("m_nPrepareState=[%d],status=[%d]", m_nPrepareState, state);
    if ((VR_SourceStatus_Openning == GetSourceStatus()) || (VR_SourceStatus_Opened == GetSourceStatus())
        || (VR_SourceStatus_OpenRecvStop == GetSourceStatus()) || (VR_SourceStatus_OpenRecvOff == GetSourceStatus())) {
        m_nPrepareState = (VR_Status)state;
        VR_Status nState = CheckReplyState();
        if (VR_Status_AMPChanged == nState) {
            ReplyOpenSession(VrOpeResult_Undefined);
        }
        else if (VR_Status_OK == nState) {
            ReplyOpenSession(VrOpeResult_OK);
        }
        else if (VR_Status_NG == nState) {
            ReplyOpenSession(VrOpeResult_Invaild);
        }
    }
}

void VR_SessionManger::ReplyOpenSession(navi::VoiceRecog::VrOpeResult nRet)
{
    if (VrOpeResult_Undefined == nRet) {
        if (m_spOpenEV_EventContext.get()) {
            ResponseVRSessionResult(m_CurrentSession, nRet);
        }
        m_spOpenEV_EventContext.reset();
        m_nSessionState = VR_Status_Undef;
        m_nPrepareState = VR_Status_Undef;
        return;
    }
    if (!VR_ConfigureIF::Instance()->getVRCancelStatus()) {
        VR_LOGI("getVRCancelStatus == false");
        ReplyVRSessionResult(m_CurrentSession, nRet);
    }
    else {
        const VR_String cancelXML = "<event name=\"cancel\" option=\"allQuit\"></event>";
        SendMsg2DE(cancelXML);
        VR_ConfigureIF::Instance()->setVRCancelStatus(false);
    }
    m_spOpenEV_EventContext.reset();
    m_nSessionState = VR_Status_Undef;
    m_nPrepareState = VR_Status_Undef;
}

void VR_SessionManger::SetSourceStatus(VR_SessionManger::VR_SourceStatus status)
{
    VR_LOGI("m_emSourceStatus=[%d],status=[%d]", m_emSourceStatus, status);
    m_emSourceStatus = status;
}

VR_SessionManger::VR_SourceStatus VR_SessionManger::GetSourceStatus()
{
    VR_LOGI("m_emSourceStatus=[%d]", m_emSourceStatus);
    return m_emSourceStatus;
}

void VR_SessionManger::ResponseVRSessionResult(const VR_String& session, VrOpeResult opeResult)
{
    VR_LOGD_FUNC();
    if (VR_Mode == m_eSessionMode) {
        std::unique_ptr<RespVrOpenSessionResult> replyMsg(RespVrOpenSessionResult::default_instance().New());
        replyMsg->set_session(session);
        replyMsg->set_result(opeResult);

        m_spEventSenderIF->Reply(*m_spOpenEV_EventContext, replyMsg.release());
    }
    else if (VR_SDL_Mode == m_eSessionMode) {
        std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
        SDLVRResult->set_sender("dm");
        SDLVRResult->set_funccode(SDLVrFunc_OpenSDLVrSession);
        SDLVrOpenSDLVrSessionResult* sessionResult = SDLVRResult->mutable_opensdlvrsessionresult();
        sessionResult->set_session(session);
        sessionResult->set_operresult(opeResult);
        m_spEventSenderIF->Reply(*m_spOpenEV_EventContext, SDLVRResult.release());
    }
    else if (VR_MM_Mode == m_eSessionMode) {
        std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
        MMVRResult->set_sender("dm");
        MMVRResult->set_funccode(VrMMFunc_OpenVrSession);
        VrOpenMMVrSessionResult* sessionResult = MMVRResult->mutable_openmmvrsessionresult();
        sessionResult->set_session(session);
        sessionResult->set_operresult(opeResult);
        m_spEventSenderIF->Reply(*m_spOpenEV_EventContext, MMVRResult.release());
    }
}

void VR_SessionManger::ReplyVRSessionResult(const VR_String &session, VrOpeResult opResult)
{
    VR_LOGD_FUNC();

    VR_String str("");
    if (VrOpeResult_OK == opResult) {
        str =  (boost::format(sessionStateChanged) % "on").str();
    }
    else {
        str =  (boost::format(sessionStateChanged) % "off").str();
    }
    SendMsg2DE(str);

    if (m_spOpenEV_EventContext.get()) {
        if (VR_Mode == m_eSessionMode) {
            std::unique_ptr<RespVrOpenSessionResult> replyMsg(RespVrOpenSessionResult::default_instance().New());
            replyMsg->set_session(session);
            replyMsg->set_result(opResult);
            m_spEventSenderIF->Reply(*m_spOpenEV_EventContext, replyMsg.release());
        }
        else if (VR_SDL_Mode == m_eSessionMode) {
            std::unique_ptr<RespSDLVrResult>  SDLVRResult(RespSDLVrResult::default_instance().New());
            SDLVRResult->set_sender("dm");
            SDLVRResult->set_funccode(SDLVrFunc_OpenSDLVrSession);
            SDLVrOpenSDLVrSessionResult* sessionResult = SDLVRResult->mutable_opensdlvrsessionresult();
            sessionResult->set_session(session);
            sessionResult->set_operresult(opResult);

            m_spEventSenderIF->Reply(*m_spOpenEV_EventContext, SDLVRResult.release());
        }
        else if (VR_MM_Mode == m_eSessionMode) {
            std::unique_ptr<RespMMVrResult>  MMVRResult(RespMMVrResult::default_instance().New());
            MMVRResult->set_sender("dm");
            MMVRResult->set_funccode(VrMMFunc_OpenVrSession);
            VrOpenMMVrSessionResult* sessionResult = MMVRResult->mutable_openmmvrsessionresult();
            sessionResult->set_session(session);
            sessionResult->set_operresult(opResult);
            m_spEventSenderIF->Reply(*m_spOpenEV_EventContext, MMVRResult.release());
        }
    }
}

void  VR_SessionManger::ResponseVRCloseSessionResult(const VR_String &session, VrOpeResult opResult)
{
     VR_LOGD_FUNC();
     if (VR_Mode == m_eSessionMode) {
         std::unique_ptr<RespVrCloseSessionResult> replyMsg(RespVrCloseSessionResult::default_instance().New());
         replyMsg->set_session(session);
         replyMsg->set_result(opResult);
         m_spEventSenderIF->Reply(*m_spCloseEV_EventContext, replyMsg.release());
     }
     else if (VR_SDL_Mode == m_eSessionMode) {
         std::unique_ptr<RespSDLVrResult> SDLVRResult(RespSDLVrResult::default_instance().New());
         SDLVRResult->set_sender("dm");
         SDLVRResult->set_funccode(SDLVrFunc_CloseSDLVrSession);
         SDLVrCloseSDLVrSessionResult* closeSession = SDLVRResult->mutable_closesdlvrsessionresult();
         closeSession->set_session(session);
         closeSession->set_operresult(opResult);
         m_spEventSenderIF->Reply(*m_spCloseEV_EventContext, SDLVRResult.release());
     }
     else if (VR_MM_Mode == m_eSessionMode) {
         std::unique_ptr<RespMMVrResult> MMVRResult(RespMMVrResult::default_instance().New());
         MMVRResult->set_sender("dm");
         MMVRResult->set_funccode(VrMMFunc_CloseVrSession);
         VrCloseMMVrSessionResult* closeSession = MMVRResult->mutable_closemmvrsessionresult();
         closeSession->set_session(session);
         closeSession->set_operresult(opResult);
         m_spEventSenderIF->Reply(*m_spCloseEV_EventContext, MMVRResult.release());
     }
}
/* EOF */
