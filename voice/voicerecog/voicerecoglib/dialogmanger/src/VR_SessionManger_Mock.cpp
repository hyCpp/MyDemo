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

// #include "VR_Common_VrUnitAgent.h"

using namespace navi::VoiceRecog;

VR_SessionManger::VR_SessionManger()
#if HAVE_NUTSHELL_OS    // for codecheck
    : m_bStartAgent(fase)
    , m_bCfmAudio(false)
    , m_bCloseSession(false)
    , m_bPvr(false)
    , m_nActionId(false)
    , m_bAudioClosing(false)
    , m_nSessionState(false)
    , m_nPrepareState(false)
    , m_emSourceStatus(false)
    , m_bQuitMsg(false)
    , m_bRequestAudioOff(false)
    , m_bVehileInfoResp(false)
    , m_eSessionMode(VR_Mode)
#endif
{
    VR_LOGD_FUNC();
}

VR_SessionManger::~VR_SessionManger()
{
    VR_LOGD_FUNC();
}

bool VR_SessionManger::OnReqestGetSeessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::OnReqestOpenSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::OnReqestCloseSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::OnNotifyStatusChanged(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::CreateSession(VR_String &session)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::OpenSession(const VR_String &session)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::CloseSession(const VR_String &session)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::RealCloseSession(const VR_String &session)
{
    VR_LOGD_FUNC();
    return true;
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
    VR_LOGD_FUNC();
}

void VR_SessionManger::SetCheckTSLSession(spEV_EventContext context,
                                          const VR_String &cSession,
                                          const VR_String &cXmlMsg,
                                          spVR_DialogEngineIF sender
                                          )
{
    VR_LOGD_FUNC();
}


void VR_SessionManger::ReqAudioSource(const VrSessionType &type)
{
    VR_LOGD_FUNC();
}


void VR_SessionManger::SetStartAgent(bool bStart)
{
    VR_LOGD_FUNC();
}

bool VR_SessionManger::GetStartAgent()
{
    VR_LOGD_FUNC();
    return true;
}


void VR_SessionManger::SendMsg2DE(const VR_String &msg)
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::SetSessionType(const VrSessionType &type)
{
    VR_LOGD_FUNC();
}

navi::VoiceRecog::VrSessionType VR_SessionManger::GetSessionType()
{
    VR_LOGD_FUNC();
    return navi::VoiceRecog::VrSessionType::Session_UnKnown;
}


void VR_SessionManger::SetDialogEngine(spVR_DialogEngineIF spDialogEngine)
{
    VR_LOGD_FUNC();
}

bool VR_SessionManger::SendMessage2Engine(const std::string &msg)
{
    VR_LOGD_FUNC();
    return true;
}

void VR_SessionManger::OnAudioStatusNotify(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::TtsBlinkReset(bool bTtsCrash)
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::SetVoiceTagStatus(VR_INT32 nVoiceTagId, bool bStatus)
{
    VR_LOGD_FUNC();
}

bool VR_SessionManger::IsTutorialSessionClosed()
{
    VR_LOGD_FUNC();
    return false;
}

bool  VR_SessionManger::OnMMVRRequestGetSessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool  VR_SessionManger::OnMMVRRequestOpenSession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool  VR_SessionManger::OnMMVRRequestCloseSession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool  VR_SessionManger::OnSDLVRRequestGetSessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool  VR_SessionManger::OnSDLVRRequestOpenSession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::OnSDLVRRequestCloseSession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    return true;
}


bool VR_SessionManger::GetLastSession(VR_String &session)
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::isCurrentSession(const VR_String &session)
{
    VR_LOGD_FUNC();
    return true;
}

void VR_SessionManger::SetMuteVR()
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::SetUnMuteVR()
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::SetUnMuteVRByRealValue()
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::CheckAudioSource()
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::SetUnitStatus(bool status)
{
    VR_LOGD_FUNC();
}

VR_SessionManger::VR_TslAppCancelStatus VR_SessionManger::GetCancelStatus()
{
    return VR_TslAppStatus_Cancel;
}

VR_SessionManger::VR_Status VR_SessionManger::CheckReplyState()
{
    VR_LOGD_FUNC();
    return VR_Status_OK;
}

void VR_SessionManger::SetPrepareState(VR_INT32 state)
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::ReplyOpenSession(navi::VoiceRecog::VrOpeResult nRet)
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::SetSourceStatus(VR_SessionManger::VR_SourceStatus status)
{
    VR_LOGD_FUNC();
}

VR_SessionManger::VR_SourceStatus VR_SessionManger::GetSourceStatus()
{
    VR_LOGD_FUNC();
    return VR_SourceStatus_Idle;
}

void VR_SessionManger::GetCurAudioSource()
{
    VR_LOGD_FUNC();
}

void VR_SessionManger::SetVehileInfoResp()
{
    VR_LOGD_FUNC();
}

bool VR_SessionManger::GetVehileInfoResp()
{
    VR_LOGD_FUNC();
    return true;
}

bool VR_SessionManger::IsTslCancel()
{
    VR_LOGD_FUNC();
    return false;
}

void VR_SessionManger::SetVoiceTagCancelStatus(bool status)
{
    VR_LOGD_FUNC();
}

/* EOF */
