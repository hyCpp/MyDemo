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
 * @file VR_DialogManger.h
 * @brief Declaration file of class VR_DialogManger.
 *
 * This file includes the declaration of class VR_DialogManger.
 *
 * @attention used for C++ only.
 */
#ifndef VR_DIALOGMANGER_H
#define VR_DIALOGMANGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_SESSIONMANGER_H
#    include "VR_SessionManger.h"
#endif

#include  "VR_XmlStrParse.h"
#include  "VR_SMSHandler.h"

#include <boost/function.hpp>
#include "VR_TaskFactoryIF.h"
#include "BL_File.h"
#include "BL_Dir.h"


namespace nutshell
{
namespace media
{
    class NMMediaDataProviderIF;
}
}

class EV_EventContext;
class VR_NaviMessageHandler;
class VR_StockInfoManager;
class VR_UserManager;
class VR_CMInfoManager;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineListener);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_TaskQueue);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogTaskBase);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_UIStartingCondition);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SessionManger);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RunableThread);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SpotlightSearchTask);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_MusicSpotLightUpdater);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_StartServiceHandler);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_NIMusicShareProxy);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_NaviStatusManager);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_TunerStatusManager);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_NaviMessageHandler);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_VehicleInfoManager);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SMSHandler);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_NCDrvInfoProxy);

#define VR_VUI_WORK_FILE_RWDISK_FOLDERNAME "VUI"

namespace nutshell
{
namespace tutorial
{
    class VR_NCTutorialManager;
}
}

/**
 * @brief The VR_DialogManger class
 *
 * class declaration
 */
class VR_DialogManger
{
public:
    typedef boost::function<bool(const VR_String&)> SessionChecker;

public:
    VR_DialogManger(const spVR_SessionManger pVR_SessionManger);
    virtual ~VR_DialogManger();
    virtual void Initialize();
    virtual void Start();
    virtual void CheckStatus();
    virtual void Stop();
    virtual void Destory();

public:
    void OnMessage(const navi::VoiceRecog::VrActionFC2DM &proto);
    void OnMsgBody(const VR_String& strMsgBody);
    void OnReqestStartDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg, SessionChecker checker);
    void OnReqestCancleDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnReqestUserOperation(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnReqestStartChangeLanguage(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnReqestVrPVRFlag(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnRequestVrTsl(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnReqestVrSettingState(const EV_EventContext &context, const VR_ProtoMessageBase& msg);
    void OnDEReplyMessage(const VR_ProtoMessageBase &msg);
    void OnSessionClosed(const VR_ProtoMessageBase &msg);
    void OnStartMessage(const VR_ProtoMessageBase& msg);
    void OnReceiveMessage(const VR_ProtoMessageBase& msg);
    void OnNotifyUICrash(const VR_ProtoMessageBase& msg); // notify ui crash,
    void TransforLanguage();
    void OnNotifySettingStatus();
    void OnRequestVoiceTagStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnMessageWeatherDataServiceRes(const VR_ProtoMessageBase &msg);
    void OnMessageWeatherDataServiceUpdate(const VR_ProtoMessageBase &msg);
    void OnMessageSpotlightSearchResult(const VR_ProtoMessageBase& msg);
    void OnMessageSearchModuleStatus(const VR_ProtoMessageBase& msg);
    void OnMessageNaviStatusUpdate(const VR_ProtoMessageBase& msg);
    void OnMessageNaviLocInfoUpdate(const VR_ProtoMessageBase& msg);
    void OnMessageCategySearchInfo(const VR_ProtoMessageBase& msg);

    spVR_DialogEngineIF GetEngine();

    /******************************* Manual VR ********************************************/
    void OnMMVRRequestVRServiceStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnMMVRRequestStartDialog(const EV_EventContext& context, const VR_ProtoMessageBase&  msg, SessionChecker checker);
    void OnMMVRRequestCancelDialog(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnMMVRRegistCommands(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

    /****************************** SDL VR **************************************************/
    void OnSDLVRRequestVRServiceStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnSDLVRRequestStartDialog(const EV_EventContext& context, const VR_ProtoMessageBase& msg, SessionChecker checker);
    void OnSDLVRRequestCancelDialog(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

    /****************************** VUI ******************************************/
    void OnRequestVriAutoProxy(const VR_String& strXml);
    void OnUpdateAircondStatus(const VR_ProtoMessageBase& msg);
    void OnUpdateVehicleStatus(const VR_ProtoMessageBase& msg);

    /****************************** NaviInfo resp*********************************/
    void OnMessageNaviInfoResp(const VR_ProtoMessageBase& msg);

    // amazon
    bool OnStartVRNotify();
    void OnVrStartResult(const bool bRet);
    void OnReqDrvInfoMessage();
    void OnReqDrvInfoMessage(const VR_String& strValue);

    void setNetWorkStatus(bool status);

protected:
    void AddTask2Session(const VR_String& session, wpVR_DialogTaskBase task);
    void ReplyRequestDebuginfo(const EV_EventContext &context);
    void SessionBlinkReset();
    void VRSessionBlinkReset();


    void RemoveVBTFlagFile();
    void ConfigureVRDESetting(VR_Settings& rstSetting, std::string& rstrVdataVersion);
    void UpdateErrorONS();
    void QueryNaviPathPointList();

    bool CheckDiagMode();
    void DiagCopyFinish();
    void DiagDeleteFinish();
    void DoDiagBlinkReset();

    /****************************** VUI    **************************************************/
    void OnRequestDomainTypeHints(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnRequestUpdateHintsActionResultToVR(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnRequestStartVUI(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnRequestStartVRDiagActionResultToVR(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnRequestStopVRDiag(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnRequestStopVRDiagActionResultToVR(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnRequestVrServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnRequestCommonActionResult(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnNotifyWeatherScreenChangedToNATP(const VR_ProtoMessageBase &msg);
    void NotifyUpdateWeatherData(const VR_String &strXml);
    void SendReqToNATP(VR_String &strXml);
    void ReplyDomainTypeHints(const EV_EventContext& context, const VR_ProtoMessageBase& msg, bool bRet);
    void ReplyUpdateHintsActionResultToVR(const EV_EventContext& context, const VR_ProtoMessageBase& msg, bool bRet);
    void ReplyStartVUI(const EV_EventContext& context, const VR_ProtoMessageBase& msg, bool bRet);
    void ReplyStartVRDiagActionResultToVR(const EV_EventContext& context, const VR_ProtoMessageBase& msg, bool bRet);
    void ReplyStopVRDiag(const EV_EventContext& context, const VR_ProtoMessageBase& msg, bool bRet);
    void ReplyStopVRDiagActionResultToVR(const EV_EventContext& context, const VR_ProtoMessageBase& msg, bool bRet);
    void ReplyVrServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg, bool bRet);
    void NotifyVrServiceStatus();
    void NotifyMicrophoneStatusToFC();
    void NotifyMicrophoneStatusToVR();
    void NotifyInternetStatusToVR();
    void NotifyLanguageStatusToVR();
    void RespLanguageStatusToVR(VR_String &strId);
    void NotifyAwakenWord();
    void NotifyWuwSwitch();
    void RespAwakenWord(VR_String &strId);
    void QueryAirconditionStatus();
    void ReplyDataPathToVR(VR_String &strId);
    void ReplyMicrophoneStatusToVR(VR_String &strId);
    void ReplyInternetStatusToVR(VR_String &strId);
    void QueryNaviAddrLocation(VR_String &strId, VR_String &addrType);
    void ReplyNaviStatusToVR(VR_String &strId);
    void ReplyVehicleRainyStateToVR(VR_String &strId);
    VR_String GetInternetStatus();

    VR_String MusicChangeSource(VR_String &strId, bool incNet = true);
    VR_String MusicPlayFavorites(VR_String &strId);
    void MusicPlayOnline(VR_String &strXml);

    VR_String GetVUIWorkRootPath();
    inline VR_UINT32 GetActionId();
    bool IsSourceSupportResume();
    void ReplyMusicPlayError(VR_String &strId);
    void ReplyAddUserError(VR_String &strId);

protected:
    typedef VoiceMultiMap<VR_String, wpVR_DialogTaskBase>::type VR_SessionTaskMap;
    spVR_DialogEngineIF         m_spEngineIF;
    spVR_DialogEngineListener   m_spEngineListener;
    spVR_TaskQueue              m_spTaskQueue;
    VR_SessionTaskMap           m_sessionTaskMap;
    spVR_EventSenderIF          m_spEventSenderIF;
    spVR_UIStartingCondition    m_spUIStartCondition;
    spVR_TaskFactoryIF          m_spTaskFactory;
    spVR_RunableThread          m_spEventHandlerThread;

    typedef boost::shared_ptr<nutshell::tutorial::VR_NCTutorialManager> spVR_NCTutorialManager;
    spVR_SessionManger             m_spSessionManger;
    spVR_SpotlightSearchTask       m_spSpotLightSearch;
    spVR_MusicSpotLightUpdater     m_spSpotLightUpdater;
    spVR_StartServiceBase          m_spStartServiceHandler;
    spVR_VoiceRecogStatus          m_spVoiceRecogStatus;
    spVR_NIMusicShareProxy         m_spMusicShareProxy;
    spVR_NaviStatusManager         m_spNaviStatusManager;
    spVR_TunerStatusManager        m_spTunerStatusManager;
    VR_String                      m_sVrServiceStatus;
    typedef boost::shared_ptr<nutshell::media::NMMediaDataProviderIF> spNMMediaDataProviderIF;
    spNMMediaDataProviderIF        m_mediaDataProvider;
    spVR_NaviMessageHandler        m_spNaviMsgHandler;
    spVR_VehicleInfoManager        m_spVehiInfoManager;
    spVR_SMSHandler                m_spSMSHandler;
    VR_StockInfoManager*           m_pVrStockInfoManager;
    VR_UserManager*                m_pVRUserManager;
    VR_CMInfoManager*              m_pVRCMInfoManager;
    spVR_NCDrvInfoProxy            m_spDrvInfoProxy;
    bool                           m_bNetworkStatus;
};

#endif /* VR_DIALOGMANGER_H */
/* EOF */
