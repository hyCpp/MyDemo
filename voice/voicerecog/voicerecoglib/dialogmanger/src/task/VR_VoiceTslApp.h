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
 * @file VR_VoiceTslApp.h
 * @brief Declaration file of class VR_VoiceTslApp.
 *
 * This file includes the declaration of class VR_VoiceTslApp.
 *
 * @attention used for C++ only.
 */

#ifndef VR_VOICETSLAPP_H
#define VR_VOICETSLAPP_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#include "pugixml.hpp"

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_DIALOGENGINEIF_H
#    include "VR_DialogEngineIF.h"
#endif

#ifndef VR_SESSIONMANGER_H
#    include "VR_SessionManger.h"
#endif

#include "VR_DialogTaskBase.h"

class VR_VoiceTslSession;
class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_VoiceTslSession);
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SessionManger);

namespace navi
{
namespace VoiceRecog
{
    class RequestVrTSL;
    class RespVrTSLResult;
    class VrNotifyTSL;
}
}

/**
 * @brief The VR_VoiceTslApp class
 *
 * class declaration
 */
class VR_VoiceTslApp : public VR_DialogTaskBase
{
public:
    enum VR_TslStartStatus
    {
        VR_TslStartStatus_Default                    = 0,
        VR_TslStartStatus_Starting                   = 1,
        VR_TslStartStatus_Started                    = 2,
        VR_TslAppStatus_StartingRecvCancel = 3,
        VR_TslAppStatus_StartedRecvCancel  = 4,
    };

public:
    VR_VoiceTslApp(spVR_EventSenderIF pEventSender, spVR_DialogEngineIF pDialogSender, const spVR_SessionManger pVR_SessionManger);
    virtual ~VR_VoiceTslApp();
    virtual void Start ();
    virtual void Cancel (const EV_EventContext &context);

public:
    // this func many always dual work with fc.
    void OnRequestVrTsl(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    // this func may receive message from vr
    void OnMessage(const VR_ProtoMessageBase& msg);

protected:
    // these are some funcs notify meg to fc
    void RespSupportLanguage(const VR_String& strResult);
    void RespInstallAgent(const VR_String& strResult);
    void RespUninstallAgent(const VR_String& strResult);
    void RespGetSessionID(const VR_String& session);
    void RespStartRecog(bool bResult);
    void RespCanleRecog(bool bResult);
    void NotifyInstallProgress(const VR_String& strResult);
    void NotifyAppXmlMessage(const VR_String& strResult);
    void NotifyAppRecognitionForceEnd();
    void NotifyAppRecognitionEnd();
    void NotifyTSLVRInfo(const VR_String& strResult);

    // these are some funcs do detail work
    void GetSupportLanguage();
    void InstallAgent(const VR_String& strInfo);
    void UninstallAgent(const VR_String& strInfo);
    VR_String StartRecog(const VR_String& strInfo);
    void CanlRecog(const VR_String& strInfo);
    void SendMessage(const VR_String& strInfo);
    void UpdateAppList(const VR_String& strInfo);
    bool SendMessage2Engine(const std::string &msg);
    bool ReplyRequest(VR_ProtoMessageBase* respMsg);
    void NotifyTslAppIsInstalled(const VR_String &msg);
    void NotifyAppListState(const nutshell::BYTE byAvailable, bool bSetValue);
    void Checkapplist();
    bool CheckIsNeedOpenSession(const VR_String &info);
    VR_TslStartStatus GetStartStatus();
    void SetStartStatus(VR_TslStartStatus emStartStatus);
    VR_String GetPhoneConnectState();
    VR_String GetSendMsgXml(const VR_String& strInfo);

protected:
    spVR_EventSenderIF      m_spEventSenderIF;
    spVR_DialogEngineIF     m_spDialogEngineIF;
    spVR_SessionManger      m_spVR_SessionManger;
    VR_String               m_session;
    VR_TslStartStatus       m_emStartStatus;
    bool                    m_firstStartDialogAfterStartOver;
    pugi::xpath_query       m_queryStartOverAction;

private:
    spEV_EventContext                  m_spContext;
    VR_String                                   m_strCancelInfo;
    VoiceVector<VR_String>::type m_vecMessage;
    VR_String                                   m_strStartXml;
};

#endif // VR_VOICETSLAPP_H
/* EOF */
