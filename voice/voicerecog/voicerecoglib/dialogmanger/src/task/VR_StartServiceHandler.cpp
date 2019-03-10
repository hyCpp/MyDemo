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
* @file  VR_StartServiceHandler.cpp
* @brief Declaration file of class VR_StartServiceHandler.
*
* This file includes the declaration of class VR_StartServiceHandler.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_EventSenderIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_StartServiceHandler.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyTSL.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrTSL.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyUICrash.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/RespVrVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "VR_DailogEngineMessageBuilder.h"
#include "VR_ConfigureIF.h"

#include "navi/Voice/VoiceRecog/RequestSDLVr.pb.h"
#include "navi/Voice/VoiceRecog/RespSDLVrResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifySDLVr.pb.h"

#include "navi/Voice/VoiceRecog/RequestMMVr.pb.h"
#include "navi/Voice/VoiceRecog/RespMMVrResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyMMVr.pb.h"

using namespace navi::VoiceRecog;

VR_StartServiceHandler::VR_StartServiceHandler(spVR_DialogEngineIF spDialogEngine, spVR_EventSenderIF spEventSender)
    : m_spDialogEngine(spDialogEngine)
    , m_spEventSender(spEventSender)
    , m_spEventContext()
    , m_bVoiceNotUsing(true)
{
    VR_LOGD_FUNC();
}

VR_StartServiceHandler::~VR_StartServiceHandler()
{

}

Start_Condition_Error  VR_StartServiceHandler::CheckStartData()
{
    VR_LOGD_FUNC();
    CheckStartStatus();
    m_errorCode = m_vrstart_condition.CheckConditionData();
    return m_errorCode;
}


void  VR_StartServiceHandler::OnMMVRRequestVRServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrGetMMVrServiceStatus& aReqServiceStatus = (const VrGetMMVrServiceStatus&)msg;
    MMVrDomainType domainType(MMVrDomainType_Global);
    if (aReqServiceStatus.has_domaintype()) {
        domainType = aReqServiceStatus.domaintype();
    }
    int userData(-1);
    if (aReqServiceStatus.has_userdata()) {
        userData = aReqServiceStatus.userdata();
    }
    VR_LOGD("MMVR domaintype: [%d], userdata: [%d]", domainType, userData);

    if (m_spEventContext.get()) {
        m_spEventContext.reset();
    }
    m_spEventContext = VR_EventSenderIF::CopyEventContext(&context);
    SetInitState();
    CheckStartData();
    OnReplyMMVRServiceStatus();
}

void  VR_StartServiceHandler::OnReplyMMVRServiceStatus()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespMMVrResult> MMVRResult(RespMMVrResult::default_instance().New());
    MMVRResult->set_sender("dm");
    MMVRResult->set_funccode(VrMMFunc_GetVrServiceStatus);
    VrGetMMVrServiceStatusResult* MMServiceResult = MMVRResult->mutable_getmmvrservicestatusresult();
    VrGetMMVrServiceStatusResult_VrMMServiceStatus serviceStatus = VrGetMMVrServiceStatusResult_VrMMServiceStatus_Unknown;
    switch (m_errorCode) {
    case Start_SearchFun_Error:
        {
            serviceStatus = VrGetMMVrServiceStatusResult_VrMMServiceStatus_SearchFuncError;
        break;
        }
    case Start_DictionData_Error:
        {
            serviceStatus = VrGetMMVrServiceStatusResult_VrMMServiceStatus_DictDataError;
        break;
        }
    case Start_LangData_Error:
        {
            serviceStatus = VrGetMMVrServiceStatusResult_VrMMServiceStatus_LangDataError;
        break;
        }
    case Start_MicroPhone_Error:
        {
            serviceStatus = VrGetMMVrServiceStatusResult_VrMMServiceStatus_MicrophoneError;
        break;
        }
    case Start_HigherInterput_Error:
    case Start_TslInstalling_Error:
        {
            serviceStatus = VrGetMMVrServiceStatusResult_VrMMServiceStatus_HigherInterruptError;
        break;
        }
    case Start_Condition_Ok:
        {
            serviceStatus = VrGetMMVrServiceStatusResult_VrMMServiceStatus_Avaiable;
        break;
        }
    default:
        {
            VR_LOGD("handle errorcode: [%d] is in default logic", m_errorCode);
        break;
        }
    }
    MMServiceResult->set_servicestatus(serviceStatus);
    m_spEventSender->Reply(*m_spEventContext, MMVRResult.release());
}

void VR_StartServiceHandler::OnSDLVRRequestVRServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const SDLVrGetSDLVrServiceStatus& serviceStatus = (const SDLVrGetSDLVrServiceStatus&)msg;
    VrDomainType domainType(VrDomainType_Golbal);
    int userData(-1);
    if (serviceStatus.has_domaintype()) {
        domainType = serviceStatus.domaintype();
    }
    if (serviceStatus.has_userdata()) {
        userData = serviceStatus.userdata();
    }
    VR_LOGD("SDLVR domaintype is: [%d], userdata is: [%d]", domainType, userData);

    if (m_spEventContext.get()) {
        m_spEventContext.reset();
    }
    m_spEventContext = VR_EventSenderIF::CopyEventContext(&context);
    SetInitState();
    CheckStartData();
    OnReplyMMVRServiceStatus();
}

void  VR_StartServiceHandler::OnReplySDLVRServiceStatus()
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespSDLVrResult>  responseResult(RespSDLVrResult::default_instance().New());
    responseResult->set_sender("dm");
    responseResult->set_funccode(SDLVrFunc_GetSDLVrServiceStatus);
    SDLVrGetSDLVrServiceStatusResult* statusResult = responseResult->mutable_getsdlvrservicestatusresult();
    SDLVrGetSDLVrServiceStatusResult_VrSDLServiceStatus serivceStatus(SDLVrGetSDLVrServiceStatusResult_VrSDLServiceStatus_Unknown);

    switch (m_errorCode) {
    case Start_SearchFun_Error:
        {
            serivceStatus = SDLVrGetSDLVrServiceStatusResult_VrSDLServiceStatus_SearchFuncError;
        break;
        }
    case Start_DictionData_Error:
        {
            serivceStatus = SDLVrGetSDLVrServiceStatusResult_VrSDLServiceStatus_DictDataError;
        break;
        }
    case Start_LangData_Error:
        {
            serivceStatus = SDLVrGetSDLVrServiceStatusResult_VrSDLServiceStatus_LangDataError;
        break;
        }
    case Start_MicroPhone_Error:
        {
            serivceStatus = SDLVrGetSDLVrServiceStatusResult_VrSDLServiceStatus_MicrophoneError;
        break;
        }
    case Start_TslInstalling_Error:
    case Start_HigherInterput_Error:
        {
            serivceStatus = SDLVrGetSDLVrServiceStatusResult_VrSDLServiceStatus_HigherInterruptError;
        break;
        }
    case Start_Condition_Ok:
        {
            serivceStatus = SDLVrGetSDLVrServiceStatusResult_VrSDLServiceStatus_Avaiable;
        break;
        }
    default:
        {
            VR_LOGD("Handle errorCode [%d] is in defualt statement", m_errorCode);
        break;
        }
    }
    statusResult->set_servicestatus(serivceStatus);
    m_spEventSender->Reply(*m_spEventContext, responseResult.release());
}

bool VR_StartServiceHandler::CheckAwakenStatus()
{
    VR_LOGD_FUNC();

    bool bRet = false;

    VR_LOGI("m_bInInitializing=[%d]", m_checkstart_status.m_bInInitializing);
    VR_LOGI("m_bNavigationDemo=[%d]", m_checkstart_status.m_bNavigationDemo);
    VR_LOGI("m_bNavigationUpdating=[%d]", m_checkstart_status.m_bNavigationUpdating);
    VR_LOGI("m_bDictionaryUpdating=[%d]", m_checkstart_status.m_bDictionaryUpdating);
    VR_LOGI("m_bEngineBusy=[%d]", m_checkstart_status.m_bEngineBusy);
    VR_LOGI("m_bEngineTask=[%d]", m_checkstart_status.m_bEngineTask);
    VR_LOGI("m_bSessionUsing=[%d]", m_checkstart_status.m_bSessionUsing);
    VR_LOGI("m_bStatusReserved=[%d]", m_checkstart_status.m_bStatusReserved);
    VR_LOGI("m_bTtsCrashStatus=[%d]", m_checkstart_status.m_bTtsCrashStatus);
//    VR_LOGI("getHigherInterruptStatus=[%d]", VR_ConfigureIF::Instance()->getHigherInterruptStatus());
    if (m_checkstart_status.m_bInInitializing || m_checkstart_status.m_bNavigationDemo
        || m_checkstart_status.m_bNavigationUpdating || m_checkstart_status.m_bDictionaryUpdating
        || m_checkstart_status.m_bEngineBusy || m_checkstart_status.m_bEngineTask
        || m_checkstart_status.m_bSessionUsing || m_checkstart_status.m_bStatusReserved
        /*|| m_checkstart_status.m_bTtsCrashStatus || VR_ConfigureIF::Instance()->getHigherInterruptStatus()*/) {
        bRet = false;
    }
    else {
        bRet = true;
    }

    VR_LOGI("bRet=[%d]", bRet);
    return bRet;
}

void  VR_StartServiceHandler::OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrCommonProperty& reqCommonProperty = (const RequestVrCommonProperty&)msg;
    if (!reqCommonProperty.has_reqservicestatus()) {
        VR_LOGI("has_reqservicestatus : [%d]", reqCommonProperty.has_reqservicestatus());
        return;
    }
    VR_LOGI("RequestVrCommonProperty,has_reqservicestatus:[%d]", reqCommonProperty.has_reqservicestatus());

    if (m_spEventContext.get()) {
        m_spEventContext.reset();
    }
    m_spEventContext = VR_EventSenderIF::CopyEventContext(&context);

    SetInitState();

    CheckStartData();
    OnReplyCommonProperty();
}

void VR_StartServiceHandler::OnReplyCommonProperty()
{
    VR_LOGD_FUNC();

    std::unique_ptr<RespVrCommonPropertyResult>  replyMeg(RespVrCommonPropertyResult::default_instance().New());
    VrReqServiceStatusResult *startResult = replyMeg->mutable_reqservicestatusresult();
    VrReqServiceStatusResult_VrServiceStatus serviceStatus = VrReqServiceStatusResult_VrServiceStatus_Unknown;
    switch (m_errorCode) {
    case Start_SearchFun_Error:
        {
            VR_LOGI("Start_SearchFun_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_SearchFuncError;
            break;
        }
    case Start_DictionData_Error:
        {
            VR_LOGI("Start_DictionData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_DictDataError;
            break;
        }
    case Start_LangData_Error:
        {
            VR_LOGI("Start_LangData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_LangDataError;
            break;
        }
    case Start_ContactData_Error:
        {
            VR_LOGI("Start_ContactData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_ContactDataError;
            break;
        }
    case Start_MediaVOCS_Error:
        {
            VR_LOGI("Start_MediaVOCS_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_MediaVOCSDataError;
            break;
        }
    case Start_MicroPhone_Error:
        {
            VR_LOGI("Start_MicroPhone_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_MicrophoneError;
            break;
        }
    case Start_PTTSW_Error:
        {
            VR_LOGI("Start_PTTSW_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_PTTSWError;
            break;
        }
    case Start_HigherInterput_Error:
        {
            VR_LOGI("Start_HigherInterput_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_HigherInterruptError;
            break;
        }
    case Start_ChinaLanguage_Error:
        {
            VR_LOGI("Start_ChinaLanguage_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_CNLanguageError;
            break;
        }
    case Start_TslInstalling_Error:  // tsl installing , we set it Start_HigherInterput_Error
        {
            VR_LOGI("Start_TslInstalling_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_HigherInterruptError;
            break;
        }
    case Start_Condition_Ok:
        {
            VR_LOGI("Start_Condition_Ok");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_Avaiable;
            break;
        }
    default:
        break;
    }

#ifdef VR_START_SERVICE_CONDITION_TEST
    serviceStatus = VrReqServiceStatusResult_VrServiceStatus_Avaiable;
#endif
    startResult->set_status(serviceStatus);
    m_spEventSender->Reply(*m_spEventContext.get(), replyMeg.release());
    // VR_LOGP("DM:Send ServiceStatus Response ");
}

void  VR_StartServiceHandler::OnMessage(const VrUnitStatusResult &msg)
{
    VR_LOGD_FUNC();

    const VrUnitStatusResult&  statusResult = (const VrUnitStatusResult)msg;
    if (!statusResult.has_type()) {
        VR_LOGD("VrUnitStatusResult has_type is : [%d]", statusResult.has_type());
        return;
    }

    VrUnitStatusType  statusType = statusResult.type();
    bool              bResult    = statusResult.result();
    VR_LOGD("statusType = [%d], bResult = [%d]", statusType, bResult);

    if (vr_status_ok == statusType) {
        m_checkstart_reason.SetCheckReason();
    }
    else if (engine_status_ok == statusType) {
        m_checkstart_reason.m_bInInitializing = false;
        m_checkstart_reason.m_bUpdatingDictionary = false;
        OnNotifyVoiceSettingAvailable();
    }
    else if (engine_updating == statusType) {
        m_checkstart_reason.m_bInInitializing = bResult;
    }
    else if (map_updateerror == statusType) {
        m_checkstart_reason.m_bMapUpdateError = bResult;
    }
    else if (updating_map_data == statusType) {
        m_checkstart_reason.m_bUpdatingMapData = bResult;
    }
    else if (navi_demo_mode == statusType) {
        m_checkstart_reason.m_bNavigationDemo = bResult;
    }
    else if (dictionary_updating == statusType) {
        m_checkstart_reason.m_bUpdatingDictionary = bResult;
    }
}


void  VR_StartServiceHandler::OnMessage(const VrStartStatusResult &statusResult)
{
    VR_LOGD_FUNC();

    bool  bHasType = statusResult.has_starttype();
    if (!bHasType) {
        VR_LOGD("VrStartStatusResult.has_starttype() = [%d]", bHasType);
        return;
    }

    VrStartCheckType  startType = statusResult.starttype();
    bool              bResult   = statusResult.startreuslt();
    VR_LOGD("startType = [%d], bResult = [%d]", startType, bResult);

    switch (startType) {
    case  Engine_Updating :
        {
            if (bResult != m_checkstart_status.m_bInInitializing) {
                m_checkstart_status.m_bInInitializing = bResult;
            }
            else {
                return;
            }
            break;
        }
    case  Engine_Busy :
        {
            if (bResult != m_checkstart_status.m_bEngineBusy) {
                m_checkstart_status.m_bEngineBusy = bResult;
            }
            else {
                return;
            }
            break;
        }
    case Engine_Task :
        {
            if (bResult != m_checkstart_status.m_bEngineTask) {
                m_checkstart_status.m_bEngineTask = bResult;
            }
            else {
                return;
            }
            break;
        }
    case  Session_Using :
        {
            if (bResult != m_checkstart_status.m_bSessionUsing) {
                m_checkstart_status.m_bSessionUsing = bResult;
            }
            else {
                return;
            }
            break;
        }
    case  Tts_Crash_Status :
        {
            if (bResult != m_checkstart_status.m_bTtsCrashStatus) {
                m_checkstart_status.m_bTtsCrashStatus = bResult;
            }
            else {
                return;
            }
            break;
        }
    case Micphone_Status:
        {
            if (bResult != m_vrstart_condition.m_bMicrophone) {
                m_vrstart_condition.m_bMicrophone = bResult;
            }
            else {
                return;
            }
        }
    case Internet_Status:
        {
            if (bResult != m_vrstart_condition.m_bInternetStatus) {
                m_vrstart_condition.m_bInternetStatus = bResult;
            }
            else {
                return;
            }
        }
    case Tsl_Updating:
        {
            if (bResult != m_vrstart_condition.m_bTslInstalling) {
                m_vrstart_condition.m_bTslInstalling = bResult;
            }
            else {
                return;
            }
        }
    default :
        {
            VR_LOGI("startType error: [%d]", startType);
            break;
        }
    }
//    OnNotifyMMVRServiceAvailable();
//    OnNotifySDLVRServiceAvailable();
}

void  VR_StartServiceHandler::CheckStartStatus()
{
    VR_LOGD_FUNC();

    if (m_checkstart_status.m_bInInitializing) {
        m_vrstart_condition.m_bDictionData = false;
    }
    else {
        m_vrstart_condition.m_bDictionData = true;
    }
    if (m_checkstart_status.m_bEngineBusy || m_checkstart_status.m_bEngineTask || m_checkstart_status.m_bSessionUsing
        || m_checkstart_status.m_bTtsCrashStatus) {
        m_vrstart_condition.m_bLangData = false;
    }
    else {
        m_vrstart_condition.m_bLangData = true;
    }
}

bool VR_StartServiceHandler::CheckHigerInterrupt()
{
    VR_LOGD_FUNC();
    m_vrstart_condition.m_bHigherInterrupt = VR_ConfigureIF::Instance()->getHigherInterruptStatus();

    return(m_vrstart_condition.m_bHigherInterrupt);
}

void  VR_StartServiceHandler::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_StartServiceHandler::CheckLangDicData()
{
    VR_LOGD_FUNC();
}

void VR_StartServiceHandler::Initialize()
{
    VR_LOGD_FUNC();

    this->InitializeData();
}

void  VR_StartServiceHandler::NotifyTslState()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyTSL> notifyTSL(VrNotifyTSL::default_instance().New());
    notifyTSL->set_sender("dm");
    notifyTSL->set_funccode(VrNotifyTSL_VrNotifyTSLFunc_VrNotifyTSLFunc_AppsInstallingState);
    VrNotifyQueryTSLAppsInstallingState* tslInstallingState = notifyTSL->mutable_appsinstallingstate();
    tslInstallingState->set_sender("dm");

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYTSL, notifyTSL.release());
}

void VR_StartServiceHandler::RequestVrTslState(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestVrTSL& requestTsl = (const RequestVrTSL&)msg;
    if (requestTsl.has_appsinstallingstate()) {
        VR_LOGI("requestTsl.has_appsinstallingstate()");
        const VrTSLAppsInstallingState& tslInstallingState = requestTsl.appsinstallingstate();
        const std::string strState = tslInstallingState.installingstate();
        VR_LOGI("strState = [%s]", strState.c_str());
        bool bResult(false);
        if ("start" == strState) {
            bResult = false;
        }
        else if ("end" == strState) {
            bResult = true;
        }
        std::unique_ptr<VrLoopBack>   loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* pControlMessage = loopMessage->mutable_m_controlmessage();
        pControlMessage->set_type(DMTaskMessageType_StartStatusResult);
        pControlMessage->mutable_startstatusresult()->set_starttype(Tsl_Updating);
        pControlMessage->mutable_startstatusresult()->set_startreuslt(bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
}

void VR_StartServiceHandler::NotifyUICrash(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const VrNotifyUICrash&  uiCrash = (const VrNotifyUICrash&)msg;
    const std::string& strPeer   = uiCrash.peer();
    u_int32_t          uPeerPID  = uiCrash.peerpid();
    VR_LOGD("strPeer = [%s], uPeerPID = [%d]", strPeer.c_str(), uPeerPID);

    bool  bVRCancle = (m_nPeerPID != uPeerPID);
    const static VR_String startXml =   "<event name=\"cancel\" option=\"allQuit\">"
                                        "</event>";
    if (bVRCancle) {
        m_nPeerPID = uPeerPID;
        m_spDialogEngine->SendMessage(startXml);
        m_bUICrashStatus = true;
    }
    else {
        VR_LOGD("the peerpid is same as last pid, [%d]", uPeerPID);
    }
}

bool VR_StartServiceHandler::OnVoiceSettingAvailable()
{
    VR_LOGD_FUNC();
    // 初始化Status值
    SetInitState();

    // 执行VR启动状态Check
    Start_Condition_Error errorCode = CheckStartData();
    // 启动状态OK之后 向DE获取Resource状态 等待getResource之后才返回FC测RESP
    if (Start_Condition_Ok == errorCode) {
         return true;
    }

    return false;
}

void VR_StartServiceHandler::OnNotifyVoiceSettingAvailable()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty>  notifyMsg(VrNotifyCommonProperty::default_instance().New());
    notifyMsg->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyIsVoiceSettingAvailable);
    VrNotifyIsVoiceSettingAvailable *pcIsVoiceSettingAvailableResult = notifyMsg->mutable_isvoicesettingavailable();
    bool bStartedFlg = false;
    // 初始化Status值
    SetInitState();
    // 执行VR启动状态Check
    Start_Condition_Error errorCode = CheckStartData();
    // 启动状态OK之后 向DE获取Resource状态 等待getResource之后才返回FC测RESP
    if (Start_Condition_Ok == errorCode) {
         bStartedFlg = true;
    }

    pcIsVoiceSettingAvailableResult->set_result(bStartedFlg);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyMsg.release());
}


void   VR_StartServiceHandler::OnNotifyTslServiceAvailable()
{
    VR_LOGD_FUNC();

    if (VR_ConfigureIF::VR_EngineType::VBT != VR_ConfigureIF::Instance()->getEngineType()) {
        return;
    }

    SetInitState();
    bool  bStartCondition(false);
    Start_Condition_Error  errorCode = CheckStartData();
    if (Start_Condition_Ok == errorCode) {
        bStartCondition = true;
    }

    VrReqServiceStatusResult_VrServiceStatus serviceStatus = VrReqServiceStatusResult_VrServiceStatus_Unknown;
    switch (errorCode) {
    case Start_SearchFun_Error:
        {
            VR_LOGI("Start_SearchFun_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_SearchFuncError;
            break;
        }
    case Start_DictionData_Error:
        {
            VR_LOGI("Start_DictionData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_DictDataError;
            break;
        }
    case Start_LangData_Error:
        {
            VR_LOGI("Start_LangData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_LangDataError;
            break;
        }
    case Start_ContactData_Error:
        {
            VR_LOGI("Start_ContactData_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_ContactDataError;
            break;
        }
    case Start_MediaVOCS_Error:
        {
            VR_LOGI("Start_MediaVOCS_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_MediaVOCSDataError;
            break;
        }
    case Start_MicroPhone_Error:
        {
            VR_LOGI("Start_MicroPhone_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_MicrophoneError;
            break;
        }
    case Start_PTTSW_Error:
        {
            VR_LOGI("Start_PTTSW_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_PTTSWError;
            break;
        }
    case Start_HigherInterput_Error:
        {
            VR_LOGI("Start_HigherInterput_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_HigherInterruptError;
            break;
        }
    case Start_ChinaLanguage_Error:
        {
            VR_LOGI("Start_ChinaLanguage_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_CNLanguageError;
            break;
        }
    case Start_TslInstalling_Error:  // tsl installing , we set it Start_HigherInterput_Error
        {
            VR_LOGI("Start_TslInstalling_Error");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_HigherInterruptError;
            break;
        }
    case Start_Condition_Ok:
        {
            VR_LOGI("Start_Condition_Ok");
            serviceStatus = VrReqServiceStatusResult_VrServiceStatus_Avaiable;
            break;
        }
    default:
        break;
    }

    VR_LOGD("tls startcondition = [%d], errorcode = [%d]", bStartCondition, serviceStatus);
    std::unique_ptr<VrNotifyCommonProperty>   noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("dm");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVoiceRecogServiceStatus);
    VrNotifyVoiceRecogServiceStatus*  pStartCondition = noitfyProperty->mutable_voicerecogservicestatus();
    pStartCondition->set_startconditionresult(bStartCondition);
    pStartCondition->set_status(serviceStatus);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
}

void VR_StartServiceHandler::OnReqestVRStatusWithHints(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const VrReqServiceInfo& rsReqServiceInfo = (const VrReqServiceInfo&)msg;
    int hintType = rsReqServiceInfo.type();
    VR_String hintsParam = VR_DialogEngineMessageBuilder::CreateGetHintsDialogMsg(hintType);
    m_cHintsString = m_spDialogEngine->getHints(hintsParam);

    // 直接替换原来的context
    m_spEventContext = VR_EventSenderIF::CopyEventContext(&context);

    // 初始化Status值
    SetInitState();

    // 执行VR启动状态Check
    CheckStartData();
    OnReplyCommonPropertyWithHints(rsReqServiceInfo);
}

void VR_StartServiceHandler::OnReqestVoiceTagStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<RespVrVoiceTagResult>  replyMsg(RespVrVoiceTagResult::default_instance().New());
    replyMsg->set_sender("dm");
    replyMsg->set_funccode(VrVoiceTagFunc_VoiceTagStatus);
    bool  bResult = this->CheckVoiceTagStatus();
    VR_LOGD("voice tag status is [%d]", bResult);
    replyMsg->set_result(bResult);
    m_spEventSender->Reply(context, replyMsg.release());
}

void   VR_StartServiceHandler::OnNotifyVoiceTagAvailable()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyVoiceTagResult>  notifyResult(VrNotifyVoiceTagResult::default_instance().New());
    notifyResult->set_sender("dm");
    notifyResult->set_funccode(VrVoiceTagFunc_VoiceTagStatus);
    VrNotifyVoiceTagStatus* pVoiceTagResult = notifyResult->mutable_voicetagstatus();
    bool bResult = this->CheckVoiceTagStatus();
    VR_LOGD("notify voice tag result is [%d]", bResult);
    pVoiceTagResult->set_voicetagstatus(bResult);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYVOICETAGRESULT, notifyResult.release());
}

void  VR_StartServiceHandler::OnNotifyMMVRServiceAvailable()
{
    VR_LOGD_FUNC();

    VrNotifyMMServiceStatus_VrMMServiceStatus status(VrNotifyMMServiceStatus_VrMMServiceStatus_Unknown);
    CheckStartData();
    switch (m_errorCode) {
    case Start_SearchFun_Error:
        {
            status = VrNotifyMMServiceStatus_VrMMServiceStatus_SearchFuncError;
        break;
        }
    case Start_DictionData_Error:
        {
            status = VrNotifyMMServiceStatus_VrMMServiceStatus_DictDataError;
        break;
        }
    case Start_LangData_Error:
        {
            status = VrNotifyMMServiceStatus_VrMMServiceStatus_LangDataError;
        break;
        }
    case Start_MicroPhone_Error:
        {
            status = VrNotifyMMServiceStatus_VrMMServiceStatus_MicrophoneError;
        break;
        }
    case Start_HigherInterput_Error:
    case Start_TslInstalling_Error:
        {
            status = VrNotifyMMServiceStatus_VrMMServiceStatus_HigherInterruptError;
        break;
        }
    case Start_Condition_Ok:
        {
            status = VrNotifyMMServiceStatus_VrMMServiceStatus_Avaiable;
        break;
        }
    default:
        {
            VR_LOGD("handle errorcode: [%d] is in default logic", m_errorCode);
        break;
        }
    }

    std::unique_ptr<VrNotifyMMVr>  notifyMMVR(VrNotifyMMVr::default_instance().New());
    notifyMMVR->set_sender("dm");
    notifyMMVR->set_funccode(VrNotifyMMVr_VrNotifyMMVrFunc_VrNotifyMMVrFunc_ServiceStatus);
    VrNotifyMMServiceStatus* serviceStatus = notifyMMVR->mutable_servicestatus();
    serviceStatus->set_status(status);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYMMVR, notifyMMVR.release());
}


void  VR_StartServiceHandler::OnNotifySDLVRServiceAvailable()
{
    VR_LOGD_FUNC();

    VrNotifySDLServiceStatus_VrSDLServiceStatus status(VrNotifySDLServiceStatus_VrSDLServiceStatus_Unknown);
    CheckStartData();
    switch (m_errorCode) {
    case Start_SearchFun_Error:
        {
            status = VrNotifySDLServiceStatus_VrSDLServiceStatus_SearchFuncError;
        break;
        }
    case Start_DictionData_Error:
        {
            status = VrNotifySDLServiceStatus_VrSDLServiceStatus_DictDataError;
        break;
        }
    case Start_LangData_Error:
        {
            status = VrNotifySDLServiceStatus_VrSDLServiceStatus_LangDataError;
        break;
        }
    case Start_MicroPhone_Error:
        {
            status = VrNotifySDLServiceStatus_VrSDLServiceStatus_MicrophoneError;
        break;
        }
    case Start_HigherInterput_Error:
    case Start_TslInstalling_Error:
        {
            status = VrNotifySDLServiceStatus_VrSDLServiceStatus_HigherInterruptError;
        break;
        }
    case Start_Condition_Ok:
        {
            status = VrNotifySDLServiceStatus_VrSDLServiceStatus_Avaiable;
        break;
        }
    default:
        {
            VR_LOGD("handle errorcode: [%d] is in default logic", m_errorCode);
        break;
        }
    }

    std::unique_ptr<VrNotifySDLVr> noitfySDLVR(VrNotifySDLVr::default_instance().New());
    noitfySDLVR->set_sender("dm");
    noitfySDLVR->set_funccode(VrNotifySDLVr_VrNotifySDLVrFunc_VrNotifySDLVrFunc_ServiceStatus);
    VrNotifySDLServiceStatus* serviceStatus = noitfySDLVR->mutable_servicestatus();
    serviceStatus->set_status(status);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYSDLVR, noitfySDLVR.release());
}

bool  VR_StartServiceHandler::CheckVoiceTagStatus()
{
    VR_LOGD_FUNC();

    if (m_checkstart_status.m_bInInitializing) {  // engine is in initializing
        return false;
    }
    else if (m_checkstart_status.m_bEngineBusy || m_checkstart_status.m_bSessionUsing) { // engine busy or session using
        return false;
    }
    else if (!VR_ConfigureIF::Instance()->getHigherInterruptStatus()) { // session is in higher interrupt
        return false;
    }
    else if (m_bVoiceNotUsing) {  // it was maintained by de
        return false;
    }
    return true;
}

void  VR_StartServiceHandler::OnReqestVRServiceIdleStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<RespVrCommonPropertyResult>  replyMsg(RespVrCommonPropertyResult::default_instance().New());
    VrReqServiceIdleStatusResult*  pReqServiceIdleResult = replyMsg->mutable_reqserviceidlestatusresult();
    bool  bResult = ((!m_checkstart_status.m_bSessionUsing) && (!m_checkstart_status.m_bEngineBusy));
    VR_LOGD("bResult = [%d]", bResult);
    pReqServiceIdleResult->set_result(bResult);
    m_spEventSender->Reply(context, replyMsg.release());
}


void  VR_StartServiceHandler::OnNotifyVRServiceIdleStatus()
{
    VR_LOGD_FUNC();

    if (m_bUICrashStatus) {
        bool bResult = ((!m_checkstart_status.m_bSessionUsing) && (!m_checkstart_status.m_bEngineBusy));
        if (bResult) {
            m_bUICrashStatus = false;
        }

        VR_LOGD("bResult = [%d]", bResult);
        std::unique_ptr<VrNotifyCommonProperty>   noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("dm");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVoiceServiceIdleStatus);
        VrNotifyVoiceServiceIdleStatus*  pServiceIdleStatus = noitfyProperty->mutable_voiceserviceidlestatus();
        pServiceIdleStatus->set_idlestatus(bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
    }
}

void VR_StartServiceHandler::OnReplyCommonPropertyWithHints(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrCommonPropertyResult>  replyMeg(RespVrCommonPropertyResult::default_instance().New());
    VrReqServiceInfoResult *pcReqServiceInfoResp = replyMeg->mutable_reqserviceinforesult();
    VrReqServiceInfoResult_VrInfoServiceStatus serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_Unknown;
    switch (m_errorCode) {
    case Start_SearchFun_Error:
        {
            VR_LOGI("Start_SearchFun_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_SearchFuncError;
            break;
        }
    case Start_DictionData_Error:
        {
            VR_LOGI("Start_DictionData_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_DictDataError;
            break;
        }
    case Start_LangData_Error:
        {
            VR_LOGI("Start_LangData_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_LangDataError;
            break;
        }
    case Start_ContactData_Error:
        {
            VR_LOGI("Start_ContactData_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_ContactDataError;
            break;
        }
    case Start_MediaVOCS_Error:
        {
            VR_LOGI("Start_MediaVOCS_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_MediaVOCSDataError;
            break;
        }
    case Start_MicroPhone_Error:
        {
            VR_LOGI("Start_MicroPhone_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_MicrophoneError;
            break;
        }
    case Start_PTTSW_Error:
        {
            VR_LOGI("Start_PTTSW_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_PTTSWError;
            break;
        }
    case Start_HigherInterput_Error:
        {
            VR_LOGI("Start_HigherInterput_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_HigherInterruptError;
            break;
        }
    case Start_ChinaLanguage_Error:
        {
            VR_LOGI("Start_ChinaLanguage_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_CNLanguageError;
            break;
        }
    case Start_TslInstalling_Error:  // tsl installing , we set it Start_HigherInterput_Error
        {
            VR_LOGI("Start_TslInstalling_Error");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_HigherInterruptError;
            break;
        }
    case Start_Condition_Ok:
        {
            VR_LOGI("Start_Condition_Ok");
            serviceStatus = VrReqServiceInfoResult_VrInfoServiceStatus_Avaiable;
            break;
        }
    default:
        break;
    }

    const  VrReqServiceInfo&   reqServiceInfo = (const VrReqServiceInfo&)msg;
    VR_LOGI("uitaskid = [%d]", reqServiceInfo.uitaskid());
    pcReqServiceInfoResp->set_status(serviceStatus);
    pcReqServiceInfoResp->set_hints(m_cHintsString);
    pcReqServiceInfoResp->set_uitaskid(reqServiceInfo.uitaskid());
    m_spEventSender->Reply(*m_spEventContext.get(), replyMeg.release());
}
/* EOF */
