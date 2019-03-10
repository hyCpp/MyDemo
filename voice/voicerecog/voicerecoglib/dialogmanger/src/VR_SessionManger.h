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
 * @file VR_SessionManger.h
 * @brief Declaration file of class VR_SessionManger.
 *
 * This file includes the declaration of class VR_SessionManger.
 *
 * @attention used for C++ only.
 */
#ifndef VR_SESSIONMANGER_H
#define VR_SESSIONMANGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include <boost/scoped_ptr.hpp>
#include <boost/circular_buffer.hpp>
#ifdef HAVE_NUTSHELL_OS
#include "avmanager/NCSubAudioManager.h"
#include "avmanager/NCSubAudioListener.h"
#endif
// #include "VR_AvcLanSenderIF.h"
#include "navi/Voice/VoiceRecog/Internal/VrTslVrEngine.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrCommonDef.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrVoiceTag.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"

#ifdef HAVE_NUTSHELL_OS
typedef boost::shared_ptr<nutshell::NCSubAudioManager> spNCSubAudioManager;
#endif
class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionEventSender);
// VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AvcLanSenderIF);

/**
 * @brief The VR_SessionManger class
 *
 * class VR_SessionManger
 */
#ifdef HAVE_NUTSHELL_OS
class VR_SessionManger : public nutshell::NCSubAudioListener
#else
class VR_SessionManger
#endif
{
public:
    enum VR_TslAppCancelStatus
    {
        VR_TslAppStatus_Canceled    = 0,
        VR_TslAppStatus_Cancel         = 1,
        VR_TslAppStatus_Cancelling   = 2
    };
	
    enum VR_Status
    {
        VR_Status_Undef  = 0,
        VR_Status_OK = 1,
        VR_Status_NG = 2,
        VR_Status_AMPChanged = 3
    };
	
    enum VR_SourceStatus
    {
        VR_SourceStatus_Idle  = 1,
        VR_SourceStatus_Openning = 2,
        VR_SourceStatus_Opened = 3,
        VR_SourceStatus_Closing = 4,
        VR_SourceStatus_AMPChanged = 5,
        VR_SourceStatus_OpenRecvStop = 6,
        VR_SourceStatus_OpenRecvOff = 7
    };

    enum VR_Session_Mode
    {
        VR_Mode = 0,
        VR_SDL_Mode = 1,
        VR_MM_Mode = 2
    };

public:
    VR_SessionManger();
    virtual ~VR_SessionManger();
    bool OnReqestGetSeessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    bool OnReqestOpenSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    bool OnReqestCloseSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    bool OnNotifyStatusChanged(const VR_ProtoMessageBase &msg);
    bool CreateSession(VR_String& session);
    bool OpenSession(const VR_String &session);
    bool CloseSession(const VR_String &session);
    bool RealCloseSession(const VR_String &session);
    void SetCheckVoiceTagSession(spEV_EventContext context,
                                 const VR_String &cSession,
                                 const VR_String &cXmlMsg,
                                 spVR_ActionEventSender sender,
                                 int nFuncCode, int nActionId);
    void SetCheckTSLSession(spEV_EventContext context,
                            const VR_String &cSession,
                            const VR_String &cXmlMsg,
                            spVR_DialogEngineIF sender);
    void ReqAudioSource(const navi::VoiceRecog::VrSessionType &type);
    void SetStartAgent(bool bStart);
    bool GetStartAgent();
    void SendMsg2DE(const VR_String &msg);
    void SetSessionType(const navi::VoiceRecog::VrSessionType &type);
    navi::VoiceRecog::VrSessionType GetSessionType();
    void SetDialogEngine(spVR_DialogEngineIF spDialogEngine);
    bool SendMessage2Engine(const std::string &msg);
    void OnAudioStatusNotify(const VR_ProtoMessageBase &msg);
    void TtsBlinkReset(bool bTtsCrash);
    void SetVoiceTagStatus(VR_INT32 nVoiceTagId, bool bStatus);
    bool IsTutorialSessionClosed();
#ifdef HAVE_NUTSHELL_OS
    nutshell::NCString GetAudioName(navi::VoiceRecog::VrSessionType sessionType);
    void VRCurrentAudioSource(const nutshell::NCString &audioName);
    void VRAskAudioStatus(const nutshell::NCString &audioName);
#endif
    /********************************* Manual VR *****************************************************/
    bool  OnMMVRRequestGetSessionID(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    bool  OnMMVRRequestOpenSession(const EV_EventContext& context, const VR_ProtoMessageBase&  msg);
    bool  OnMMVRRequestCloseSession(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

    /******************************** SDL VR *********************************************************/
    bool  OnSDLVRRequestGetSessionID(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    bool  OnSDLVRRequestOpenSession(const EV_EventContext&  context, const VR_ProtoMessageBase& msg);
    bool  OnSDLVRRequestCloseSession(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

public:
    bool GetLastSession(VR_String &session);
    bool isCurrentSession(const VR_String &session);
    void SetMuteVR();
    void SetUnMuteVR();
    void SetUnMuteVRByRealValue();
    void CheckAudioSource();
    void SetUnitStatus(bool status);
    VR_TslAppCancelStatus GetCancelStatus();
    void SetCancelStatus(VR_TslAppCancelStatus status);
    VR_Status CheckReplyState();
    void SetPrepareState(VR_INT32 state);
    void ReplyOpenSession(navi::VoiceRecog::VrOpeResult nRet);
    void SetSourceStatus(VR_SourceStatus status);
    VR_SourceStatus GetSourceStatus();
    void GetCurAudioSource();
    void SetVehileInfoResp();
    bool GetVehileInfoResp();
    bool IsTslCancel();
    void SetVoiceTagCancelStatus(bool status);
    // amazon
    void OpenChannel(VR_String strXml = "");
    void CloseChannel();
    void OnEngineInitedMsg();
    void OnAudioInOpe(bool bOpen);
    void OnNotifyCheckAudioSource(bool bStatus);
    void OpenMic();
    void SetMicStatus(VR_SourceStatus status);
    void StartEchoCancel();
    void StopEchoCancel();
    VR_SourceStatus GetMicStatus();
    void OpenChannelWithoutStart();
#ifdef HAVE_NUTSHELL_OS

public:
    virtual VOID currentAudioSource(const nutshell::NCString &audioName);
    virtual VOID notifyStatusChanged(const nutshell::NCString &audioName, const nutshell::NCString &status);
    virtual VOID askAudioStatus(const nutshell::NCString &audioName);
    virtual VOID notifyCheckAudioSource(const nutshell::NCString &audioName, nutshell::NC_BOOL result);
    virtual VOID notifyRequestAudioOff(const nutshell::NCString &audioName);
    virtual VOID notifyAMPStsChanged(const nutshell::NCString &audioName, nutshell::NC_BOOL connect);
    virtual VOID notifyCurAudioSource(const nutshell::NCString &audioName, const nutshell::NCString &curAudioName);

protected:
    typedef boost::circular_buffer<VR_String>   BufferType;
    typedef BufferType::iterator                BufferIterator;
    typedef std::pair<bool, BufferIterator>     BufferFindResult;

protected:
    BufferFindResult FindSession(const VR_String &session);
    void NotifyVoiceTagResult();
    void ResetCancelStatus();
    void ResetState(const nutshell::NCString &audioName, const nutshell::NCString &status);
    void SendQuitMsg2DE();
    void VRNotifyAMPStsChanged(const nutshell::NCString &audioName, bool connect);
    void SendCurrentAudioSource(const nutshell::NCString &audioName);
    void SendAskAudioStatus(const nutshell::NCString &audioName);
    void SendNotifyAMPStsChanged(const nutshell::NCString &audioName, nutshell::NC_BOOL connect);
    void VRNotifyRequestAudioOff(const nutshell::NCString &audioName);
    void VRNotifyCurAudioSource(const nutshell::NCString &audioName, const nutshell::NCString &curAudioName);
    void SendNotifyCurAudioSource(const nutshell::NCString &audioName, const nutshell::NCString &curAudioName);
    void RespTslCancelRecognition();
    void RespServerVrStart(VR_INT32 nSuccess);
    void NotifySessionClosed();
    void NotifyTslResult(const VR_String &strXml);
    void ReplyOpenSessionForTtsCrash();
    void ReplyTslStartApp();
    bool GetVoiceTagCancelStatus();
    bool GetVoiceTagStatus();
    void NotifyVoiceTagSuccess();
    void SetTutorialCancel(bool bStatus);
    bool GetTutorialCancel();

    void ResponseVRSessionResult(const VR_String& session,
                                 navi::VoiceRecog::VrOpeResult opResult = navi::VoiceRecog::VrOpeResult_Undefined);

    void ReplyVRSessionResult(const VR_String& session,
                                 navi::VoiceRecog::VrOpeResult opResult = navi::VoiceRecog::VrOpeResult_Undefined);

    void ResponseVRCloseSessionResult(const VR_String& session,
                                      navi::VoiceRecog::VrOpeResult opResult = navi::VoiceRecog::VrOpeResult_Undefined);

protected:
    VR_String               m_CurrentSession;
    VR_String               m_tmpSession;
    VR_String               m_ClosedSession;
    BufferType              m_SessionBuff;
    spNCSubAudioManager     m_spNCSubAudioManager;
    spEV_EventContext       m_spOpenEV_EventContext;
    spEV_EventContext       m_spCloseEV_EventContext;
    spVR_EventSenderIF      m_spEventSenderIF;
    VR_String               m_cXmlMsg;
    spVR_DialogEngineIF     m_spEngineIF;
    spVR_ActionEventSender  m_sender;
    spEV_EventContext       m_voiceTag_EventContext;
    spEV_EventContext       m_tslEventContext;
    VR_String               m_OpenningSession;
    bool                    m_bStartAgent;
    bool                    m_bCfmAudio;
    bool                    m_bCloseSession;
    bool                    m_bPvr;
    navi::VoiceRecog::VrSessionType m_CurSessionType;
    navi::VoiceRecog::VrSessionType m_NextSessionType;
    int                     m_nActionId;
//  spVR_AvcLanSenderIF m_spAvcLanSender;
    bool                    m_bAudioClosing;
    spVR_DialogEngineIF     m_spVREngineIF;
    VR_String               m_strSender;
    VR_INT32                m_nDomainType;
    VR_Status               m_nSessionState;
    VR_Status               m_nPrepareState;
    VR_SourceStatus m_emSourceStatus;
    bool                    m_bQuitMsg;
    bool                    m_bRequestAudioOff;
    bool                    m_bVehileInfoResp;
    VR_Session_Mode         m_eSessionMode;
    // amazon
    bool                    m_bEngineInited;
    bool                    m_bAudioIn;
    VR_String               m_strSuggestionXml;
    VR_SourceStatus         m_emMicStatus;
    bool                    m_bWithoutStart;
#endif
};

#endif // VR_SESSIONMANGER_H
/* EOF */
