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
 * @file VR_MessageProcess.cpp
 * @brief Declaration file of class VR_MessageProcess.
 *
 * This file includes the declaration of class VR_MessageProcess.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"

#include "VR_VoiceTslApp.h"
#include "VR_ConfigureIF.h"
#include "VR_XmlStrParse.h"
#include "VR_DialogManger.h"
#include "VR_ActionManger.h"
#include "VR_DialogManger.h"
#include "VR_EventSenderIF.h"
#include "VR_SessionManger.h"
#include "VR_MessageProcess.h"
#include "VR_NCTutorialManager.h"
#include "VR_TransformProtoToXml.h"
#include "action/VR_ActionFactoryIF.h"
#include "action/VR_ActionEventSender.h"
#include "action/VR_InfoActionFactory.h"
#include "dataprovider/VR_DataProvider.h"
#include "VR_TransformXmlToXml.h"
#include "VR_DailogEngineMessageBuilder.h"

#include "EV_Define.h"
#include "boost/bind.hpp"
#include "EV_EventSender.h"
#include "MediaDataProviderIF/include/NMMediaDataProviderIF.h"

#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/RequestMMVr.pb.h"
#include "navi/Voice/VoiceRecog/RequestSDLVr.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"
#include "navi/Voice/VoiceRecog/RequestVriAutoProxy.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyEngineStatus.pb.h"
#include "navi/Voice/VoiceRecog/VrAirconditionStatus.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyActionResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"

using namespace nutshell;
using namespace nutshell::media;
using namespace navi::VoiceRecog;
using namespace navi::dataprovider;

VR_MessageProcess::VR_MessageProcess(spVR_CommandHandleIF spCommandHandle)
    : m_SessionManger(VR_new VR_SessionManger())
    , m_spDialogManger(VR_new VR_DialogManger(m_SessionManger))
#ifdef HAVE_NUTSHELL_OS
    , m_spDataProvider(VR_new VR_DataProvider())
#else
    , m_spDataProvider(VR_new VR_DataProvider())
#endif
{
    VR_LOGD_FUNC();
    spVR_ActionEventSender sender(VR_new VR_ActionEventSender(m_spDialogManger->GetEngine()));

    // create action factory
    spVR_ActionFactoryIF spActionFactory = VR_ActionFactoryIF::getActionFactory(sender);
    m_spActionManger.reset(VR_new VR_ActionManger(spActionFactory, sender, m_SessionManger));
}

VR_MessageProcess::~VR_MessageProcess()
{
    VR_LOGD_FUNC();
}

void VR_MessageProcess::Initialize()
{
    VR_LOGD_FUNC();
    m_spDialogManger->Initialize();
    m_spDataProvider->Initialize();
}

void VR_MessageProcess::Start()
{
    VR_LOGD_FUNC();
    m_spDialogManger->Start();
    m_spDataProvider->StartDPThread();
}

void VR_MessageProcess::CheckStatus()
{
    VR_LOGD_FUNC();
    this->OnStartedNotify();
}

void VR_MessageProcess::Stop()
{
    VR_LOGD_FUNC();
    m_spDataProvider->StopDPThread();
    m_spDialogManger->Stop();
}

void VR_MessageProcess::Destory()
{
    VR_LOGD_FUNC();
    m_spDialogManger->Destory();
}

void VR_MessageProcess::OnReqestGetSeessionID(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_SessionManger->OnReqestGetSeessionID(context, msg);
}

void VR_MessageProcess::OnReqestOpenSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_SessionManger->OnReqestOpenSeession(context, msg);
}

void VR_MessageProcess::OnReqestCloseSeession(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_SessionManger->OnReqestCloseSeession(context, msg);
}

void VR_MessageProcess::OnReqestStartDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    wpVR_SessionManger wpSessionManger(m_SessionManger);
    m_spDialogManger->OnReqestStartDialog(context, msg,
                                          ([wpSessionManger](const std::string& session)->bool {
                                                spVR_SessionManger spSessionManger = wpSessionManger.lock();
                                                if (spSessionManger.get()) {
                                                    return spSessionManger->isCurrentSession(session);
                                                }
                                                return false;
                                            }));
}

void VR_MessageProcess::OnReqestCancleDialog(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnReqestCancleDialog(context, msg);
}

bool  VR_MessageProcess::SessionChecker(const std::string &session)
{
    VR_LOGD_FUNC();
    if (m_SessionManger.get()) {
        return m_SessionManger->isCurrentSession(session);
    }
    return false;
}

void VR_MessageProcess::OnMMVRRequestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const RequestMMVr&  requestMMVR = (const RequestMMVr&)msg;
    VrMMFunc funccode = requestMMVR.funccode();
    VR_LOGD("mmvr funccode : [%d]", funccode);

    switch (funccode) {
    case VrMMFunc_GetVrServiceStatus:
        {
            if (requestMMVR.has_getmmvrservicestatus()) {
                const VrGetMMVrServiceStatus& aReqServiceStatus = requestMMVR.getmmvrservicestatus();
                m_spDialogManger->OnMMVRRequestVRServiceStatus(context, aReqServiceStatus);
            }
            else {
                VR_LOGD("requestmmvr has_mmvrservicestatus is abnormal");
            }
        break;
        }
    case VrMMFunc_CreateVrSession:
        {
            if (requestMMVR.has_getmmvrsession()) {
                const VrGetMMVrSession& aReqGetSession = requestMMVR.getmmvrsession();
                m_SessionManger->OnMMVRRequestGetSessionID(context, aReqGetSession);
            }
            else {
                VR_LOGD("requestmmvr has_getmmvrsession is abnormal");
            }
        break;
        }
    case VrMMFunc_OpenVrSession:
        {
            if (requestMMVR.has_openmmvrsession()) {
                const VrOpenMMVrSession& aReqOpenSession = requestMMVR.openmmvrsession();
                m_SessionManger->OnMMVRRequestOpenSession(context, aReqOpenSession);
            }
            else {
                VR_LOGD("requestmmvr has_openmmvrsession is abnormal");
            }
        break;
        }
    case VrMMFunc_RegistVrCommands:
        {
            VR_LOGD("requestmmvr RegistVrCommands");
            if (requestMMVR.has_registmmvrcommands()) {
                const VrRegistMMVrCommands& registCommands = requestMMVR.registmmvrcommands();
                m_spDialogManger->OnMMVRRegistCommands(context, registCommands);
            }
        break;
        }
    case VrMMFunc_StartVoiceRecog:
        {
            if (requestMMVR.has_startmmvoicerecog()) {
                const VrStartMMVoiceRecog& aReqStartRecog = requestMMVR.startmmvoicerecog();
                m_spDialogManger->OnMMVRRequestStartDialog(context,
                                                           aReqStartRecog,
                                                           boost::bind(&VR_MessageProcess::SessionChecker, this, _1)
                                                           );
            }
            else {
                VR_LOGD("requestmmvr has_startmmvoicerecog is abnormal");
            }
        break;
        }
    case VrMMFunc_CloseVrSession:
        {
            if (requestMMVR.has_closemmvrsession()) {
                const VrCloseMMVrSession& aReqCloseSession = requestMMVR.closemmvrsession();
                m_SessionManger->OnMMVRRequestCloseSession(context, aReqCloseSession);
            }
            else {
                VR_LOGD("requestmmvr has_closemmvrsession is abnormal");
            }
        break;
        }
    case VrMMFunc_CancelVoiceRecog:
        {
            if (requestMMVR.has_cancelmmvoicerecog()) {
                const VrCancelMMVoiceRecog& aReqCancelVR = requestMMVR.cancelmmvoicerecog();
                m_spDialogManger->OnMMVRRequestCancelDialog(context, aReqCancelVR);
            }
            else {
                VR_LOGD("requestmmvr has_cancelvoicerecog is abnormal");
            }
        break;
        }
    default:
        {
            VR_LOGD("requestmmvr funccode in defualt logic");
        break;
        }
    }
}

void VR_MessageProcess::OnSDLVRRequestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const RequestSDLVr& requestSDLVR = (const RequestSDLVr&)msg;
    SDLVrFunc  funcCode = requestSDLVR.funccode();
    VR_LOGD("sdlvr funccode = [%d]", funcCode);

    switch (funcCode) {
    case SDLVrFunc_GetSDLVrSetting:
        {
            VR_LOGD("reqsdlvr funccode is GetSDLVrSetting");
        break;
        }
    case SDLVrFunc_SetSDLVrSetting:
        {
            VR_LOGD("reqsdlvr funccode is SetSDLVrSetting");
        break;
        }
    case SDLVrFunc_CancelSDLVrTTS:
        {
            VR_LOGD("reqsdlvr funccode is CancelSDLVrTTS");
        break;
        }
    case SDLVrFunc_GetSDLVrServiceStatus:
        {
            if (requestSDLVR.has_getsdlvrservicestatus()) {
                const SDLVrGetSDLVrServiceStatus& serviceStatus = requestSDLVR.getsdlvrservicestatus();
                m_spDialogManger->OnSDLVRRequestVRServiceStatus(context, serviceStatus);
            }
            else {
                VR_LOGD("reqsdlvr has_getsdlvrservicestatus is abnormal");
            }
        break;
        }
    case SDLVrFunc_CreateSDLVrSession:
        {
            if (requestSDLVR.has_createsdlvrsession()) {
                const SDLVrCreateSDLVrSession& createSession = requestSDLVR.createsdlvrsession();
                m_SessionManger->OnSDLVRRequestGetSessionID(context, createSession);
            }
            else {
                VR_LOGD("reqsdlvr has_createsdlvrsession is abnormal");
            }
        break;
        }
    case SDLVrFunc_OpenSDLVrSession:
        {
            if (requestSDLVR.has_opensdlvrsession()) {
                const SDLVrOpenSDLVrSession& openSession = requestSDLVR.opensdlvrsession();
                m_SessionManger->OnSDLVRRequestOpenSession(context, openSession);
            }
            else {
                VR_LOGD("reqsdlvr has_opensdlvrsession is abnormal");
            }
        break;
        }
    case SDLVrFunc_RegistSDLVrCommands:
        {
            VR_LOGD("reqsdlvr funccode is registvrcommands");
        break;
        }
    case SDLVrFunc_StartSDLVoiceRecog:
        {
            if (requestSDLVR.has_startsdlvoicerecog()) {
                const SDLVrStartSDLVoiceRecog& startVoice = requestSDLVR.startsdlvoicerecog();
                m_spDialogManger->OnSDLVRRequestStartDialog(context,
                                                            startVoice,
                                                            boost::bind(&VR_MessageProcess::SessionChecker, this, _1)
                                                            );
            }
            else {
                VR_LOGD("reqsdlvr has_startsdlvoicerecog is abnormal");
            }
        break;
        }
    case SDLVrFunc_CloseSDLVrSession:
        {
            if (requestSDLVR.has_closesdlvrsession()) {
                const SDLVrCloseSDLVrSession& closeSession = requestSDLVR.closesdlvrsession();
                m_SessionManger->OnMMVRRequestCloseSession(context, closeSession);
            }
            else {
                VR_LOGD("reqsdlvr has_closesdlvrsession is abnormal");
            }
        break;
        }
    case SDLVrFunc_CancelSDLVoiceRecog:
        {
            if (requestSDLVR.has_cancelsdlvoicerecog()) {
                const SDLVrCancelSDLVoiceRecog& cancelVoice = requestSDLVR.cancelsdlvoicerecog();
                m_spDialogManger->OnMMVRRequestCancelDialog(context, cancelVoice);
            }
            else {
                VR_LOGD("reqsdlvr has_cancelsdlvoicerecog is abnormal");
            }
        break;
        }
    default:
        {
            VR_LOGD("sdlvr funcode is in default logic.");
        break;
        }
    }
}

void VR_MessageProcess::OnRequestVriAutoProxy(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const  RequestVriAutoProxy& requestVriAutoProxy = (const RequestVriAutoProxy&)msg;
    if ("ms_proxy" == requestVriAutoProxy.sender()) {
        m_spDataProvider->NotifyCheckUDiskSuccess();
        return;
    }

    if (!requestVriAutoProxy.has_postrecogmsg()) {
        VR_LOGE("has_postrecogmsg is abnormal");
        return;
    }
    if (!requestVriAutoProxy.postrecogmsg().has_xml_in()) {
        VR_LOGE("has_xml_in is abnormal");
        return;
    }
    VR_String strXml = requestVriAutoProxy.postrecogmsg().xml_in();
    VR_XmlStrParse xmlStrParse;
    if (!xmlStrParse.LoadString(strXml)) {
        VR_LOGE("load_string error");
        return;
    }

    VR_String strType = xmlStrParse.GetXMLValue(VR_XPATH_TYPE);
    VoiceMap<VR_String, emVriAutoProxyType>::const_iterator iter = g_iAutoRouter.find(strType);
    if (iter == g_iAutoRouter.end()) {
        VR_LOGE("Type[%s] don't exist!", strType.c_str());
        return;
    }

    VR_String id = xmlStrParse.GetXMLValue(VR_XPATH_QUERY_ID);
    switch (iter->second) {
    case VriAutoProxyType_Media_GetSourceStatus: {
        VR_String srcType;
        xmlStrParse.GetNodeText("//source_type", srcType);
        m_spDataProvider->OnReplyMusicDevStatus(id, srcType);
        return;
    }
    case VriAutoProxyType_Media_GetLocalInfor: {
        m_spDataProvider->OnReplyMusicGetLocalInfo(strXml);
        return;
    }
    case VriAutoProxyType_Media_GetDBData: {
        VR_String srcId;
        xmlStrParse.GetNodeText("//source_id", srcId);
        m_spDataProvider->OnReplyMusicGetDBData(id, srcId);
        return;
    }
    case VriAutoProxyType_Phone_GetStatus: {
        m_spDataProvider->OnReplyPhoneStatus(id);
        return;
    }
    case VriAutoProxyType_Phone_GetDBData: {
        m_spDataProvider->OnReplyPhoneDBData(id);
        return;
    }
    case VriAutoProxyType_Phone_GetNumber: {
        VR_String strContactId;
        xmlStrParse.GetNodeText("//phone_contact_id", strContactId);
        VR_String strName;
        xmlStrParse.GetNodeText("//contact_name", strName);
        m_spDataProvider->OnReplyPhoneNumberGet(id, strContactId, strName);
        return;
    }
    case VriAutoProxyType_Natp_Started:{
        m_spDataProvider->NotifyFinishedData();
        break;
    }
    default:
        break;
    }

    m_spDialogManger->OnRequestVriAutoProxy(strXml);
}

void VR_MessageProcess::OnReqestUserOperation(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnReqestUserOperation(context, msg);
}

void VR_MessageProcess::OnReqestStartChangeLanguage(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnReqestStartChangeLanguage(context, msg);
}

void VR_MessageProcess::OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnReqestCommonProperty(context, msg);
}

void VR_MessageProcess::OnReqestVrPVRFlag(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnReqestVrPVRFlag(context, msg);
}

void VR_MessageProcess::OnReqestVrVoiceTag(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const RequestVrVoiceTag&  requestVoiceTag = (const RequestVrVoiceTag&)msg;
    if (VrVoiceTagFunc_VoiceTagStatus == requestVoiceTag.funccode()) {
        m_spDialogManger->OnRequestVoiceTagStatus(context, msg);
        return;
    }
    m_spActionManger->VoiceTagAction(context, msg);
}

void VR_MessageProcess::OnReqestVrTsl(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnRequestVrTsl(context, msg);
}

void VR_MessageProcess::OnReplyActoinResult(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrNotifyActionResult&  actionResult = (const VrNotifyActionResult &)(msg);
    VR_String strResult = actionResult.actionresult();
    if (std::string::npos != strResult.find("getMessageDetail")) {
        m_spDialogManger->OnMsgBody(strResult);
    }
    else if (std::string::npos != strResult.find("MAPProtocol")) {
        VR_LOGI("action result[%s]", strResult.c_str());
        m_spDataProvider->OnSetSMSSupportState(strResult);
    }
    else {
        m_spActionManger->OnReplyActoinResult(msg);
    }
}

void VR_MessageProcess::OnNotifyUICrash(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnNotifyUICrash(msg);
}

void VR_MessageProcess::OnReqestVrSettingState(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnReqestVrSettingState(context, msg);
}

///
/// FOR TEST
///
//    #include "EV_EventSender.h"
//    #include "navi/Voice/VoiceRecog/RequestVrCreateSession.pb.h"
//    #include "navi/Voice/VoiceRecog/RequestVrOpenSession.pb.h"
//    #include "navi/Voice/VoiceRecog/RequestVrStartRecognition.pb.h"
void VR_MessageProcess::OnMessageStarted()
{
    ///
    /// FOR TEST
    ///
    VR_LOGI("THREAD ID=[%d]", gettid());
//    RequestVrCreateSession msg1;
//    msg1.set_sender("test");
//    m_SessionManger->OnReqestGetSeessionID(EV_EventContext(), &msg1);

//    VR_String session;
//    m_SessionManger->GetLastSession(session);

//    RequestVrOpenSession msg2;
//    msg2.set_sender("test");
//    msg2.set_session(session);
//    m_SessionManger->OnReqestOpenSeession(EV_EventContext(), &msg2);
//    sleep(1);

//    RequestVrStartRecognition msg3;
//    msg3.set_sender("test");
//    msg3.set_session(session);
//    msg3.set_type(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
//    m_spDialogManger->OnReqestStartDialog(EV_EventContext(), &msg3,
//                                          boost::bind(&VR_SessionManger::isCurrentSession, m_SessionManger, _1));
}

void VR_MessageProcess::OnMessageLoopBack(const VR_ProtoMessageBase &msg)
{
    const VrLoopBack& loopMsg = (const VrLoopBack&)msg;

    VR_LOGI("loopMsg.type()=[%d]", loopMsg.type());
    switch (loopMsg.type()) {
    case VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL:
        if ((loopMsg.has_m_controlmessage())) {
            const VrEngineControlMessage& controlMessage = loopMsg.m_controlmessage();
            if (DMTaskMessageType_ChangeLanguage == controlMessage.type()) {
                const VrChangeLangueResult& changelanguage = controlMessage.change_language_result();
                VR_String strLanguage = changelanguage.language();
                VR_LOGI("strLanguage = [%s]", strLanguage.c_str());
                // send msg update grammer

                m_spDialogManger->TransforLanguage();
                m_spDialogManger->OnNotifySettingStatus();
           //     m_spDataProvider->UpdateLanguage();
                return;
            }
            else if (DMTaskMessageType_CloseSession == controlMessage.type()) {
                m_SessionManger->ReqAudioSource(controlMessage.sessiontype().sessiontype());
                return;
            }
            else if (DMTaskMessageType_InitPersonalData == controlMessage.type()) {
                const VrPersonalDataResult& dataResult = controlMessage.personalresult();
                if (persondata_end == dataResult.datatype()) {
            //        m_spDataProvider->InitPersonalData();
                }
            }
            else if (DMTaskMessageType_prepare == controlMessage.type()) {
                VR_INT32 state = controlMessage.prepareresult().errcode();
                if (0 == state) {
                    m_SessionManger->SetPrepareState(VR_SessionManger::VR_Status_OK);
                }
                else {
                    m_SessionManger->SetPrepareState(VR_SessionManger::VR_Status_NG);
                }
                return;
            }
            else if (DMTaskMessageType_VehicleInfoResp == controlMessage.type()) {
                m_SessionManger->SetVehileInfoResp();
                return;
            }
            else if (DMTaskMessageType_StartStatusResult == controlMessage.type()) {
                OnMessageTtsBlinkRest(loopMsg.m_controlmessage());
//                if (controlMessage.has_startstatusresult()) {
//                    if (controlMessage.startstatusresult().has_starttype() && controlMessage.startstatusresult().has_startreuslt()) {
//                        VrStartCheckType nVrStartCheckType =controlMessage.startstatusresult().starttype();
//                        bool bRet =controlMessage.startstatusresult().startreuslt();
//                        if ((Engine_Busy == nVrStartCheckType) && !bRet) {
//                            m_SessionManger->OnEngineInitedMsg();
//                        }
//                    }
//                }
            }
            else if (DMTaskMessageType_StartEnd == controlMessage.type()) {
                if (controlMessage.has_start_end()) {
                    m_spDialogManger->OnVrStartResult(controlMessage.start_end().result());
                }
                else {
                    VR_LOGD("false == controlMessage.has_start_end()");
                }
                return;
            }
            m_spDialogManger->OnDEReplyMessage(loopMsg.m_controlmessage());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_DE2DM:
        if (loopMsg.has_m_de2dm()) {
            // hook dataprovider dictionary result
            const VrActionDE2DM& actionMsg = loopMsg.m_de2dm();
            const VrDMActionType& actionType = actionMsg.vr_dm_action_type();
            if (ActionType_DP_DICTIONARY_RESULT == actionType) {
                if (!actionMsg.has_dp_dictionary_result()) {
                    VR_LOGD("data provider dictionary result is null");
                    return;
                }

          //      m_spDataProvider->OnMessage(actionMsg.dp_dictionary_result());
                return;
            }
            else if (ActionType_DP_DICTIONARY_REQUEST == actionType) {
                if (!actionMsg.has_dp_dictionary_request()) {
                    VR_LOGD("data provider dictionary result is null");
                    return;
                }

          //      m_spDataProvider->OnMessage(actionMsg.dp_dictionary_request());
                return;
            }
            else if (ActionType_SESSION_REQ == actionType) {
                if (!actionMsg.has_session_req()) {
                    VR_LOGD("has_session_req is null");
                    return;
                }
                if (VrSessionReq_VrSessionType_open == actionMsg.session_req().func_code()) {
//                    bool bRet = m_spDialogManger->OnStartVRNotify();
//                    VR_String strMsg = VR_DialogEngineMessageBuilder::CreateAwakenResultMsg(bRet);
//                    m_spDialogManger->GetEngine()->SendMessage(strMsg, actionMsg.id());

                    m_SessionManger->OpenChannel();
                }
                else if (VrSessionReq_VrSessionType_close == actionMsg.session_req().func_code()) {
                    VR_String strMsg = VR_DialogEngineMessageBuilder::CreateQuitResultMsg(true);
                    m_spDialogManger->GetEngine()->SendMessage(strMsg, actionMsg.id());
                    strMsg = "<event name=\"cancel\" option=\"allQuit\"></event>";
                    m_spDialogManger->GetEngine()->SendMessage(strMsg);

                    VR_String strState = "SYSTEM_STATE_DIALOG";
                    VR_String strStateAction = "SYSTEM_STATE_END";
                    strMsg = VR_TransformXmlToXml().CreateStateXML(actionMsg.id(), strState, strStateAction);
                    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
                    noitfyProperty->set_sender("Alexa");
                    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVRUpdateData);
                    noitfyProperty->mutable_vrupdatedata()->set_updatedata(strMsg);
                    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
                }
                return;
            }
            else if (ActionType_ACTIVE_INFO_REQ == actionType) {
                if (actionMsg.has_active_info_req()) {
                    const VrActiveInfoReq& _activeInfoReq = actionMsg.active_info_req();
                    if (_activeInfoReq.request_id() == DEStatusResult) {
                        const VrDEStatusResult& deStatus = _activeInfoReq.destatus_result();
                        std::string strType = deStatus.type();
                        int nStatus = deStatus.result();

                        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
                        noitfyProperty->set_sender("fc");
                        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyDictationVrServiceStatus);

                        VR_LOGD("DrvInfo status Type is: [%s], value is: [%d]", strType.c_str(), nStatus);
                        if (strType.compare("engine") == 0 && nStatus == 0) {
                            m_spDialogManger->setNetWorkStatus(true);
                            m_spDialogManger->OnReqDrvInfoMessage();
                            noitfyProperty->mutable_vrservicestatus()->set_status(VrNotifyDictationVrServiceStatus_VrDictationVrServiceStatus_Idle);
                            EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
                        }
                        else if (strType.compare("engine") == 0 && nStatus == 1) {
                            m_spDialogManger->setNetWorkStatus(false);
                            noitfyProperty->mutable_vrservicestatus()->set_status(VrNotifyDictationVrServiceStatus_VrDictationVrServiceStatus_Invalid);
                            EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
                        }
                    }
                }
            }
            else if (ActionType_ALEXA_ACTION == actionType) {
                const VrAlexaAction& alexaAction = actionMsg.alexa_action();
                VR_String strMsg = alexaAction.msg();
                VR_LOGI("alexa action message is  %s", strMsg.c_str());
                pugi::xml_document doc;
                if (doc.load(strMsg.c_str())) {
                    VR_String strOp;
                    pugi::xml_node msgNode = doc.root().first_child();
                    if (msgNode) {
                        strOp = msgNode.attribute("op").value();
                    }
                    if ("DirveScore" == strOp) {
                        VR_String strQueryperiod = doc.select_node("//queryperiod").node().child_value();
                        if (NULL != m_spDialogManger) {
                            m_spDialogManger->OnReqDrvInfoMessage(strQueryperiod);
                        }
                    }
                }
            }

            m_spActionManger->OnMessage(loopMsg.m_de2dm());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_FC2DM:
        if (loopMsg.has_m_fc2dm()) {
            if (VR_NCTUTORIAL_TTS_PLAY_SENDER_NAME == loopMsg.sender()) {
                m_spDialogManger->OnMessage(loopMsg.m_fc2dm());
            }
            else {
                m_spActionManger->OnMessage(loopMsg.m_fc2dm());
            }
        }
        break;
    case VrLoopBack_MSG_Type_MSG_SERVICE_STARTED:
        this->OnMessageStarted();
        break;
    case VrLoopBack_MSG_Type_MSG_AUDIO_MODE:
        if (loopMsg.has_m_audiomode()) {
            m_SessionManger->OnNotifyStatusChanged(loopMsg.m_audiomode());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_DP_USB_CONNECT_STATUS:
        if (loopMsg.has_m_agentusb_connected()) {
            // m_spDataProvider->OnMessage(loopMsg.m_agentusb_connected());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_AGENT_ACTIVATION:
        if (loopMsg.has_m_agentactivation()) {
            m_spDialogManger->OnStartMessage(loopMsg.m_agentactivation());
            // m_spDataProvider->OnMessage(loopMsg.m_agentactivation());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_METER_DM2FC :
        if (loopMsg.has_m_meterinfo()) {
            VrMetMMStatus metMMStatus = loopMsg.m_meterinfo();
            VR_String  resultXml = VR_TransformProtoToXml().Transform(metMMStatus);
            std::unique_ptr<VrNotifyEngineStatus> protoMsg(VrNotifyEngineStatus::default_instance().New());
            protoMsg->set_sender("vr_s");
            protoMsg->set_taskid(-1);
            protoMsg->set_enginestatus(resultXml);

            EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYENGINESTATUS, protoMsg.release());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_METER_DE2DM :
        if (loopMsg.has_m_meterinfo()) {
            VrMetMMStatus metMMStatus = loopMsg.m_meterinfo();
      //      m_spAvcSender->sendProto(metMMStatus);
        }
        break;
    case VrLoopBack_MSG_Type_MSG_UNIT_STATUS :
        if (loopMsg.has_m_status()) {
            m_spDialogManger->OnReceiveMessage(loopMsg.m_status());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_DM2DE :
        if (loopMsg.has_m_vrsendmsgdm2de()) {
            m_spDialogManger->GetEngine()->SendMessage(loopMsg.m_vrsendmsgdm2de().msg());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_HMIMODE_HIGHINTER_NOTIFY:
        if (loopMsg.has_m_vrhigherhmimodenoitfy()) {
            m_spDialogManger->OnNotifySettingStatus();
            m_SessionManger->OnNotifyCheckAudioSource(VR_ConfigureIF::Instance()->getHigherInterruptStatus());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_AUDIO_STATUS:
        if (loopMsg.has_m_audio_status_notify()) {
            m_SessionManger->OnAudioStatusNotify(loopMsg.m_audio_status_notify());
        }
        break;
    case VrLoopBack_MSG_Type_MSG_LAZYSERVICE_NOTIFY:
        {
      //      m_spDataProvider->OnLazyServiceNotify();
        }
        break;
    case VrLoopBack_MSG_Type_MSG_VEHICLE_INFO_RESP:
        {
            if (loopMsg.has_m_airconditonstatus()) {
                const VrAirconditionStatus& airCondStatus = loopMsg.m_airconditonstatus();
                m_spDialogManger->OnUpdateAircondStatus(airCondStatus);
            }
            else if (loopMsg.has_m_vehicleinfo()) {
                const VrVehicleInfo& vehicleInfo = loopMsg.m_vehicleinfo();
                m_spDialogManger->OnUpdateVehicleStatus(vehicleInfo);
            }
        }
        break;
    default:
        {
            VR_LOGE("loopMsg.type()=[%d]", loopMsg.type());
        }
        break;
    }
}

void VR_MessageProcess::OnMessageSpotlightSearchResult(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnMessageSpotlightSearchResult(msg);
}

void VR_MessageProcess::OnMessageSearchModuleStatus(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnMessageSearchModuleStatus(msg);
}

void VR_MessageProcess::OnMessageSessionClosed(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spDialogManger->OnSessionClosed(msg);
}

void VR_MessageProcess::OnMessage2DialogEngine(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_MessageProcess::OnMessage2DialogManger(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_MessageProcess::OnMessageNaviReqDfltInfoResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviReqDfltInfoResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchStateResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchStateResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchCityResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchCityResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchStreetResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchStreetResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchHouseNoResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchHouseNoResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchPoiInfoResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchPoiInfoResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviQueryPointCollectionResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryPointCollectionResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviQueryDestHistoryResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryDestHistoryResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviQueryRouteExistResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryRouteExistResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviQueryAllDestListResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryAllDestListResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviPathPointListNotify(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviPathPointListNotify(msg);
#endif
}


void VR_MessageProcess::OnMessageNaviQueryUnpassedDestListResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryUnpassedDestListResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviQueryPointTypeResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryPointTypeResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviQueryNextDestResp(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryNextDestResp(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviQueryCurrentPosResp(const VR_ProtoMessageBase &msg)
{
//    VR_LOGD_FUNC();
// #if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryCurrentPosResp(msg);
// #endif
}

void VR_MessageProcess::OnMessageNaviMapScreenInfo(const VR_ProtoMessageBase &msg)
{
    m_spActionManger->OnMessageNaviMapScreenInfo(msg);
}

void VR_MessageProcess::OnMessageNaviMapNeighborName(const VR_ProtoMessageBase &msg)
{
    m_spActionManger->OnMessageNaviMapNeighborName(msg);
}

void VR_MessageProcess::OnMessageNaviQueryPOIByNameRespCN(const VR_ProtoMessageBase &msg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviQueryPOIByNameRespCN(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchStateRespCN(const VR_ProtoMessageBase &msg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchStateRespCN(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchCityRespCN(const VR_ProtoMessageBase &msg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchCityRespCN(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchDistrictRespCN(const VR_ProtoMessageBase &msg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchDistrictRespCN(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchStreetRespCN(const VR_ProtoMessageBase &msg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchStreetRespCN(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchHouseNoRespCN(const VR_ProtoMessageBase &msg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchHouseNoRespCN(msg);
#endif
}

void VR_MessageProcess::OnMessageNaviSearchAddressInfoCN(const VR_ProtoMessageBase &msg)
{
#if defined(VR_DM_TARGET_VRUNIT)
    m_spActionManger->OnMessageNaviSearchAddressInfoCN(msg);
#endif
}

void VR_MessageProcess::OnMessageReqWebSearchEngine()
{
    VR_LOGD_FUNC();
}

void VR_MessageProcess::OnStartedNotify()
{
    m_spDialogManger->CheckStatus();
    m_spDataProvider->OnStartedNotify();
}

void VR_MessageProcess::OnMessageNaviNotifyRouteStatus(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spActionManger->OnMessageNaviNotifyRouteStatus(msg);
}

void VR_MessageProcess::OnMessageTtsBlinkRest(const VR_ProtoMessageBase &msg)
{
    const VrEngineControlMessage& engineMessage = (const VrEngineControlMessage&)msg;
    if (Tts_Crash_Status == engineMessage.startstatusresult().starttype()) {
        bool bRet = engineMessage.startstatusresult().startreuslt();
        m_SessionManger->TtsBlinkReset(bRet);
        if (!bRet) {
            m_spActionManger->ClearTtsAction();
        }
    }
}

void VR_MessageProcess::OnMessageWeatherCityListResp(const VR_ProtoMessageBase &msg)
{
  //  m_spDataProvider->OnMessageWeatherCityListResp(msg);
}

void VR_MessageProcess::OnMessageNotifyWeatherCityList(const VR_ProtoMessageBase &msg)
{
   // m_spDataProvider->OnMessageNotifyWeatherCityList(msg);
}

void VR_MessageProcess::OnMessageWeatherInfoResp(const VR_ProtoMessageBase &msg)
{
     m_spActionManger->OnMessageWeatherInfoResp(msg);
}

void VR_MessageProcess::OnMessageWeatherDataServiceRes(const VR_ProtoMessageBase &msg)
{
     m_spDialogManger->OnMessageWeatherDataServiceRes(msg);
}

void VR_MessageProcess::OnMessageWeatherDataServiceUpdate(const VR_ProtoMessageBase &msg)
{
     m_spDialogManger->OnMessageWeatherDataServiceUpdate(msg);
}

void VR_MessageProcess::OnMessageNaviStatusUpdate(const VR_ProtoMessageBase &msg)
{
    m_spDialogManger->OnMessageNaviStatusUpdate(msg);
}

void VR_MessageProcess::OnMessageNaviLocInfoUpdate(const VR_ProtoMessageBase &msg)
{
    m_spDialogManger->OnMessageNaviLocInfoUpdate(msg);
}

void VR_MessageProcess::OnMessageCategySearchInfo(const VR_ProtoMessageBase &msg)
{
    m_spDialogManger->OnMessageCategySearchInfo(msg);
}


void VR_MessageProcess::OnMessageNaviInfoResp(const VR_ProtoMessageBase& msg)
{
    m_spDialogManger->OnMessageNaviInfoResp(msg);
}

/* EOF */
