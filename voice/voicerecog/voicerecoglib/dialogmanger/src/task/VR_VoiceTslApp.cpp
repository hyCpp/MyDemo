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
 * @file VR_VoiceTslApp.cpp
 * @brief Declaration file of class VR_VoiceTslApp.
 *
 * This file includes the declaration of class VR_VoiceTslApp.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#include <sstream>

#include "ncore/NCFile.h"
#include "VR_VoiceTslApp.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_EventSenderIF.h"
#include "VR_ConfigureIF.h"
#include "BL_DataStorage.h"
#include "cutils/properties.h"
#include <contentresolver/NCContentResolver.h>
#include "navi/Voice/VoiceRecog/RequestVrTSL.pb.h"
#include "navi/Voice/VoiceRecog/RespVrTSLResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyTSL.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTslVrEngine.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace navi::VoiceRecog;
using namespace nutshell;

const static VR_String defaultSession = "VoiceRecogTSL";
const nutshell::BYTE byAppListUnAvailable = 0;
const nutshell::BYTE byAppListAvailable = 1;

VR_VoiceTslApp::VR_VoiceTslApp(spVR_EventSenderIF pEventSender, spVR_DialogEngineIF pDialogSender, const spVR_SessionManger pVR_SessionManger)
    : m_spEventSenderIF(pEventSender)
    , m_spDialogEngineIF(pDialogSender)
    , m_spVR_SessionManger(pVR_SessionManger)
    , m_emStartStatus(VR_TslStartStatus_Default)
    , m_firstStartDialogAfterStartOver(false)
    , m_queryStartOverAction("//action[@name='notifyAppXmlMessage']/Message/Action/@type='Start Over'")
{
    VR_LOGD_FUNC();
}

VR_VoiceTslApp::~VR_VoiceTslApp()
{
    VR_LOGD_FUNC();
}

void VR_VoiceTslApp::Start()
{
    VR_LOGD_FUNC();
    VR_String keyName = "local.navi.VR.startstatus";
    VR_String strDefault = "firststart";
    char keyValue[PROPERTY_VALUE_MAX] = { 0 };
    property_get(keyName.c_str(), keyValue, strDefault.c_str());
    VR_LOGI("keyValue : [%s]", keyValue);
    VR_String strValue = keyValue;

    if ((strValue != strDefault) && ("connected" == GetPhoneConnectState())) {
        std::unique_ptr<VrLoopBack>   loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
        controlMessage->set_type(DMTaskMessageType_CheckAppListExist);
        controlMessage->mutable_checkapplist();
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
}

void VR_VoiceTslApp::Cancel(const EV_EventContext &context)
{
    VR_LOGD_FUNC();
}

void VR_VoiceTslApp::OnRequestVrTsl(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    m_spContext = VR_EventSenderIF::CopyEventContext(&context);
    const RequestVrTSL& requestVrTSL = (const RequestVrTSL&) msg;

    VR_INT32 funccode = requestVrTSL.funccode();
    VR_LOGI("funccode=[%d]", funccode);
    switch (funccode) {
    case VrTSLFunc_GetSupportLanguage :
        {
            VR_LOGI("VrTSLFunc_GetSupportLanguage");
            GetSupportLanguage();
        }
        break;
    case VrTSLFunc_InstallAgent :
        {
            VR_LOGI("VrTSLFunc_InstallAgent");
            VR_String strInfo = "";
            if (requestVrTSL.has_installagent()) {
                strInfo = requestVrTSL.installagent().content();
                VR_LOGI("strInfo=[%s]", strInfo.c_str());
            }
            InstallAgent(strInfo);
        }
        break;
    case VrTSLFunc_UninstallAgent :
        {
            VR_LOGI("VrTSLFunc_UninstallAgent");
            VR_String strInfo = "";
            if (requestVrTSL.has_uninstallagent()) {
                strInfo = requestVrTSL.uninstallagent().content();
                VR_LOGI("strInfo=[%s]", strInfo.c_str());
            }
            UninstallAgent(strInfo);
        }
        break;
    case VrTSLFunc_GetSessionID :
        {
            VR_LOGI("VrTSLFunc_GetSessionID");
            m_spVR_SessionManger->CreateSession(m_session);
            VR_LOGI("m_session=[%s]", m_session.c_str());
            RespGetSessionID(m_session);
        }
        break;
    case VrTSLFunc_StartRecog :
        {
            VR_LOGI("VrTSLFunc_StartRecog");
            VR_String session = "";
            VR_String info = "";
            if (requestVrTSL.has_startrecog()) {
                session = requestVrTSL.startrecog().session();
                if (session != m_session) {
                    VR_LOGI("session=[%s],m_session=[%s]", session.c_str(), m_session.c_str());
                    RespStartRecog(false);
                    return;
                }
                info = requestVrTSL.startrecog().info();
                bool bNeedOpenSession = CheckIsNeedOpenSession(info);
                VR_String xml = StartRecog(info);
                if (bNeedOpenSession) {
                    if (VR_TslStartStatus_Default == GetStartStatus()) {
                        SetStartStatus(VR_TslStartStatus_Starting);
                        m_spVR_SessionManger->SetCheckTSLSession(m_spContext, session, xml, m_spDialogEngineIF);
                        bool isSuccess = m_spVR_SessionManger->OpenSession(session);
                        m_strStartXml = xml;
                        VR_LOGI("isSuccess=[%d],session=[%s], info=[%s]", isSuccess, session.c_str(), info.c_str());
                    }
                    else {
                        if (m_firstStartDialogAfterStartOver && (VR_TslStartStatus_Started == GetStartStatus())) {
                            // if user say "start over", TslApp will send OpenSession command during Dialoging.
                            // so we need send the "OpenSession" to VBT.
                            // DE-VBT will avoid restart dialoging during "Cancel".
                            m_firstStartDialogAfterStartOver = false;
                            SendMessage2Engine(xml);
                        }
                        else {
                            RespStartRecog(false);
                        }
                    }
                }
                else {
                    if (VR_TslStartStatus_Started == GetStartStatus()) {
                        SendMessage2Engine(xml);
                    }
                }
            }
            else {
                RespStartRecog(false);
            }
        }
        break;
    case VrTSLFunc_CancelRecog :
        {
            VR_LOGI("VrTSLFunc_CancelRecog");
            VR_String strInfo = "";
            VR_String session = "";
            if (requestVrTSL.has_cancelrecog()) {
                session = requestVrTSL.cancelrecog().session();
                strInfo = requestVrTSL.cancelrecog().info();
                VR_LOGI("session=[%s],m_session=[%s],strInfo=[%s]", session.c_str(), m_session.c_str(), strInfo.c_str());
            }
            if (session != m_session) {
                RespCanleRecog(false);
                return;
            }
            if (VR_TslStartStatus_Started == GetStartStatus()) {
                SetStartStatus(VR_TslAppStatus_StartedRecvCancel);
                CanlRecog(strInfo);
            }
            else if (VR_TslStartStatus_Starting == GetStartStatus()) {
                SetStartStatus(VR_TslAppStatus_StartingRecvCancel);
                const VR_String xml = "<event name=\"CancelAppRecognition\">"
                                                 "%1%"
                                             "</event>";
                m_strCancelInfo = (boost::format(xml) % strInfo).str();
            }
            else {
                RespCanleRecog(false);
            }
        }
        break;
    case VrTSLFunc_SendMessage :
        {
            VR_LOGI("VrTSLFunc_SendMessage");
            VR_String strInfo = "";
            VR_String session = "";
            if (requestVrTSL.has_message()) {
                strInfo = requestVrTSL.message().content();
                session = requestVrTSL.message().session();
                VR_LOGI("session=[%s],strInfo=[%s]", session.c_str(), strInfo.c_str());
            }
            if ((0 != session.compare(m_session)) && (0 != session.compare(defaultSession))) {
                VR_LOGI("if ((0 != session.compare(m_session)) && (0 != session.compare(defaultSession)))");
                NotifyAppRecognitionForceEnd();
            }
            else {
                if (VR_TslStartStatus_Started == GetStartStatus()) {
                    SendMessage(strInfo);
                }
                else if ((VR_TslStartStatus_Starting == GetStartStatus()) || (VR_TslStartStatus_Default == GetStartStatus())) {
                    if (VR_String::npos != strInfo.find("VBT Send Recognition State")) {
                        m_vecMessage.push_back(GetSendMsgXml(strInfo));
                    }
                    else {
                        SendMessage(strInfo);
                    }
                }
            }
        }
        break;
    case VrTSLFunc_UpdateAppList :
        {
            VR_LOGI("VrTSLFunc_UpdateAppList");
            VR_String strInfo = "";
            if (requestVrTSL.has_applist()) {
                strInfo = requestVrTSL.applist().list();
                VR_LOGI("strInfo=[%s]", strInfo.c_str());
            }
            UpdateAppList(strInfo);
            NotifyTslAppIsInstalled(strInfo);
        }
        break;
    default:
        {
            VR_LOGI("default");
        }
        break;
    }

}

void VR_VoiceTslApp::RespSupportLanguage(const VR_String& strResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrTSLResult> respVrTSLResult(RespVrTSLResult::default_instance().New());
    respVrTSLResult->set_funccode(VrTSLFunc_GetSupportLanguage);
    VrTSLGetSupportLanguageResult* getSupportLanguageResult = respVrTSLResult->mutable_supportlanguage();
    getSupportLanguageResult->set_info(strResult);
    ReplyRequest(respVrTSLResult.release());
}

void VR_VoiceTslApp::RespInstallAgent(const VR_String &strResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrTSLResult> respVrTSLResult(RespVrTSLResult::default_instance().New());
    respVrTSLResult->set_funccode(VrTSLFunc_InstallAgent);
    VrTSLInstallAgentResult* installAgentResult = respVrTSLResult->mutable_installagent();
    installAgentResult->set_result(strResult);
    ReplyRequest(respVrTSLResult.release());
}

void VR_VoiceTslApp::RespUninstallAgent(const VR_String &strResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrTSLResult> respVrTSLResult(RespVrTSLResult::default_instance().New());
    respVrTSLResult->set_funccode(VrTSLFunc_UninstallAgent);
    VrTSLUninstallAgentResult* unInstallAgentResult = respVrTSLResult->mutable_uninstallagent();
    unInstallAgentResult->set_result(strResult);
    ReplyRequest(respVrTSLResult.release());
}

void VR_VoiceTslApp::RespGetSessionID(const VR_String &session)
{
    VR_LOGD_FUNC();
    std::unique_ptr<RespVrTSLResult> respVrTSLResult(RespVrTSLResult::default_instance().New());
    respVrTSLResult->set_funccode(VrTSLFunc_GetSessionID);
    VrTSLGetSeesionIDResult* getSeesionID = respVrTSLResult->mutable_session();
    getSeesionID->set_session(session);
    ReplyRequest(respVrTSLResult.release());
}

void VR_VoiceTslApp::RespCanleRecog(bool bResult)
{
    VR_LOGI("bResult=[%d]", bResult);
    std::unique_ptr<RespVrTSLResult> respVrTSLResult(RespVrTSLResult::default_instance().New());
    respVrTSLResult->set_funccode(VrTSLFunc_CancelRecog);
    VrTSLCancelRecognitionResult* cancelRecog = respVrTSLResult->mutable_cancelrecog();
    cancelRecog->set_result(bResult);
    ReplyRequest(respVrTSLResult.release());
}

void VR_VoiceTslApp::RespStartRecog(bool bResult)
{
    VR_LOGI("bResult=[%d]", bResult);
    std::unique_ptr<RespVrTSLResult> respVrTSLResult(RespVrTSLResult::default_instance().New());
    respVrTSLResult->set_funccode(VrTSLFunc_StartRecog);
    VrTSLStartRecognitionResult* startRecog = respVrTSLResult->mutable_startrecog();
    startRecog->set_result(bResult);
    ReplyRequest(respVrTSLResult.release());
}

void VR_VoiceTslApp::NotifyInstallProgress(const VR_String& strResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrNotifyTSL> notifyTSL(VrNotifyTSL::default_instance().New());
    notifyTSL->set_funccode(VrNotifyTSL_VrNotifyTSLFunc_VrNotifyTSLFunc_InstallProcess);
    VrNotifyTSLInstallProcess* installProcess = notifyTSL->mutable_installprocess();
    installProcess->set_process(strResult);
    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYTSL, notifyTSL.release());
}

void VR_VoiceTslApp::NotifyAppXmlMessage(const VR_String& strResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrNotifyTSL> notifyTSL(VrNotifyTSL::default_instance().New());
    notifyTSL->set_funccode(VrNotifyTSL_VrNotifyTSLFunc_VrNotifyTSLFunc_SendMessage);
    VrNotifyTSLMessage* message = notifyTSL->mutable_message();
    message->set_session(m_session);
    message->set_content(strResult);
    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYTSL, notifyTSL.release());
}

void VR_VoiceTslApp::NotifyAppRecognitionForceEnd()
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrNotifyTSL> notifyTSL(VrNotifyTSL::default_instance().New());
    notifyTSL->set_funccode(VrNotifyTSL_VrNotifyTSLFunc_VrNotifyTSLFunc_RecognitionForceEnd);
    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYTSL, notifyTSL.release());
}

void VR_VoiceTslApp::NotifyAppRecognitionEnd()
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrNotifyTSL> notifyTSL(VrNotifyTSL::default_instance().New());
    notifyTSL->set_funccode(VrNotifyTSL_VrNotifyTSLFunc_VrNotifyTSLFunc_RecognitionEnd);
    VrNotifyTSLRecognitionEnd* recognitionEnd = notifyTSL->mutable_recognitionend();
    recognitionEnd->set_session(m_session);
    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYTSL, notifyTSL.release());
}

void VR_VoiceTslApp::NotifyTSLVRInfo(const VR_String& strResult)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrNotifyTSL> notifyTSL(VrNotifyTSL::default_instance().New());
    notifyTSL->set_funccode(VrNotifyTSL_VrNotifyTSLFunc_VrNotifyTSLFunc_VRInfo);
    VrNotifyTSLVRInfo* tslVRInfo = notifyTSL->mutable_vrinfo();
    tslVRInfo->set_info(strResult);
    m_spEventSenderIF->SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYTSL, notifyTSL.release());
}

void VR_VoiceTslApp::GetSupportLanguage()
{
    VR_LOGD_FUNC();
    const VR_String xml = "<event name=\"getSupportedLanguage\"/>";

    SendMessage2Engine(xml);
}

void VR_VoiceTslApp::InstallAgent(const VR_String& strInfo)
{
    VR_LOGD_FUNC();
    const VR_String xml = "<event name=\"install\">"
                                     "%1%"
                                 "</event>";
    VR_String resultXml = (boost::format(xml) % strInfo).str();

    SendMessage2Engine(resultXml);
}

void VR_VoiceTslApp::UninstallAgent(const VR_String& strInfo)
{
    VR_LOGD_FUNC();
    const VR_String xml = "<event name=\"uninstall\">"
                                     "%1%"
                                 "</event>";
    VR_String resultXml = (boost::format(xml) % strInfo).str();

    SendMessage2Engine(resultXml);
}

VR_String VR_VoiceTslApp::StartRecog(const VR_String& strInfo)
{
    VR_LOGD_FUNC();
    const VR_String xml = "<event name=\"StartAppRecognition\">"
                                     "%1%"
                                 "</event>";
    VR_String resultXml = (boost::format(xml) % strInfo).str();

    return resultXml;
}

void VR_VoiceTslApp::CanlRecog(const VR_String& strInfo)
{
    VR_LOGD_FUNC();
    const VR_String xml = "<event name=\"CancelAppRecognition\">"
                                     "%1%"
                                 "</event>";
    VR_String resultXml = (boost::format(xml) % strInfo).str();

    SendMessage2Engine(resultXml);
}

void VR_VoiceTslApp::UpdateAppList(const VR_String& strInfo)
{
    VR_LOGD_FUNC();
    const VR_String xml = "<event name=\"updateAppList\">"
                                     "%1%"
                                 "</event>";
    VR_String resultXml = (boost::format(xml) % strInfo).str();

    SendMessage2Engine(resultXml);
    VR_ConfigureIF::Instance()->setUpdateState(VR_TSL_APPLIST, resultXml);
}

void VR_VoiceTslApp::SendMessage(const VR_String& strInfo)
{
    VR_LOGD_FUNC();
    const VR_String xml = "<event name=\"SendAppXmlMessage\">"
                                     "%1%"
                                 "</event>";
    VR_String resultXml = (boost::format(xml) % strInfo).str();

    SendMessage2Engine(resultXml);
}

void VR_VoiceTslApp::OnMessage(const VR_ProtoMessageBase &msg)
{
    if ("navi.VoiceRecog.VrEngineControlMessage.VrCheckApplist" == msg.GetTypeName()) {
        Checkapplist();
        return;
    }
    const VrTransmitDataToTSLNotify& transmitDataToTSLNotify = (const VrTransmitDataToTSLNotify&)msg;

    if (transmitDataToTSLNotify.has_data()) {
        VR_String xml = transmitDataToTSLNotify.data();
        VR_LOGD("TSL Action: VBT->TSL: %s", xml.c_str());
        pugi::xml_document doc;
        if (doc.load(xml.c_str())) {
            pugi::xml_node root = doc.root().first_child();
            pugi::xml_node firstNode = root.first_child();
            std::string actionName = root.name();
            std::string attrValue = root.attribute("name").value();
            std::string strErrCode = root.attribute("errcode").value();
            VR_LOGI("actionName=[%s],attrValue=[%s],errcode=[%s]", actionName.c_str(), attrValue.c_str(), strErrCode.c_str());
            std::ostringstream oss;
            firstNode.print(oss);
            std::string strInfo = oss.str();
            VR_LOGI("tslapp: get oss value is [%s]", strInfo.c_str());

            if (0 == actionName.compare("event-result")) {
                if (0 == attrValue.compare("install")) {
                    RespInstallAgent(strInfo);
                }
                else if (0 == attrValue.compare("uninstall")) {
                    RespUninstallAgent(strInfo);
                }
                else if (0 == attrValue.compare("getSupportedLanguage")) {
                    RespSupportLanguage(strInfo);
                }
                else if (0 == attrValue.compare("CancelAppRecognition")) {
                    if (Session_TSL == m_spVR_SessionManger->GetSessionType()) {
                        VR_ConfigureIF::Instance()->setVRCancelStatus(false);
                        std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
                        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
                        loopMessage->mutable_m_controlmessage()->set_type(DMTaskMessageType_CloseSession);
                        loopMessage->mutable_m_controlmessage()->mutable_sessiontype()->set_sessiontype(Session_TSL);
                        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
                    }
                    else {
                        RespCanleRecog(false);
                    }
                }
                else if (0 == attrValue.compare("OpenTslSession")) {
                    RespStartRecog(true);
                    if (VR_TslAppStatus_StartingRecvCancel == GetStartStatus()) {
                        m_spVR_SessionManger->ReqAudioSource(Session_TSL);
                    }
                    else if (VR_TslStartStatus_Starting == GetStartStatus()) {
                        SetStartStatus(VR_TslStartStatus_Started);
                        VoiceVector<VR_String>::type::iterator iter = m_vecMessage.begin();
                        for (; iter != m_vecMessage.end(); iter++) {
                            SendMessage2Engine(*iter);
                        }
                        SendMessage2Engine(m_strStartXml);
                        m_vecMessage.clear();
                        m_strStartXml.clear();
                    }
                }
                else if (0 == attrValue.compare("AudioStopByHighInterrupt")) {
                    RespStartRecog(false);
                    if (VR_TslAppStatus_StartingRecvCancel == GetStartStatus()) {
                        RespCanleRecog(false);
                    }
                    m_firstStartDialogAfterStartOver = false;
                    m_session.clear();
                    m_strCancelInfo.clear();
                    m_strStartXml.clear();
                    m_vecMessage.clear();
                    SetStartStatus(VR_TslStartStatus_Default);
                }
                else if (0 == attrValue.compare("AudioOffByHighInterrupt")) {
                    if (VR_TslAppStatus_StartingRecvCancel == GetStartStatus()) {
                        RespCanleRecog(false);
                    }
                    NotifyAppRecognitionForceEnd();
                    m_firstStartDialogAfterStartOver = false;
                    m_session.clear();
                    m_strCancelInfo.clear();
                    m_strStartXml.clear();
                    m_vecMessage.clear();
                    SetStartStatus(VR_TslStartStatus_Default);
                }
                else if (0 == attrValue.compare("AudioOffByTsl")) {
                    RespCanleRecog(true);
                    m_firstStartDialogAfterStartOver = false;
                    m_session.clear();
                    m_strCancelInfo.clear();
                    m_strStartXml.clear();
                    m_vecMessage.clear();
                    SetStartStatus(VR_TslStartStatus_Default);
                }
                else {
                    VR_LOGI("in event-result, has no attribute value!");
                }
            }
            else if (0 == actionName.compare("action")) {
                if (0 == attrValue.compare("notifyInstallProgress")) {
                    NotifyInstallProgress(strInfo);
                }
                else if (0 == attrValue.compare("notifyAppXmlMessage")) {
                    const bool isStartOverAction = m_queryStartOverAction.evaluate_boolean(doc);
                    VR_LOGD("TSL Action: isStartOverAction: %s", isStartOverAction ? "true" : "false");
                    if (isStartOverAction) {
                        m_firstStartDialogAfterStartOver = true;
                    }
                    NotifyAppXmlMessage(strInfo);
                }
                else if (0 == attrValue.compare("notifyAppRecognitionForceEnd")) {
                    m_firstStartDialogAfterStartOver = false;
                    NotifyAppRecognitionForceEnd();
                }
                else if (0 == attrValue.compare("notifyAppRecognitionEnd")) {
                    m_firstStartDialogAfterStartOver = false;
                    NotifyAppRecognitionEnd();
                }
                else if (0 == attrValue.compare("notifyTSLVRInfo")) {
                    NotifyTSLVRInfo(strInfo);
                }
                else {
                    VR_LOGI("in action, has no attribute value!");
                }
            }
        }
        else {
            VR_LOGI("Load xml string error!");
        }
    }
    else {
        VR_LOGI("VrTransmitDataToTSLNotify has no data");
    }
}

void VR_VoiceTslApp::Checkapplist()
{
    VR_LOGD_FUNC();
    nutshell::BYTE byAvailable = byAppListUnAvailable;
    BL_DataStorage dataStorage;
    INT nSize = sizeof(byAvailable);
    CL_BOOL bRet = dataStorage.GetValue(BL_DATA_KEY_VR_TSLAPPEXIST, &byAvailable, nSize);
    if (!bRet) {
        VR_LOGD("GetValue BL_DATA_KEY_VR_TSLAPPEXIST failed");
        return;
    }
    NotifyAppListState(byAvailable, false);
}

bool VR_VoiceTslApp::CheckIsNeedOpenSession(const VR_String &info)
{
    VR_LOGD_FUNC();
    bool bNeedOpenSession = false;
    pugi::xml_document doc;
    if (doc.load(info.c_str())) {
        pugi::xml_node node = doc.select_node("//CSVR/Message/Action[@type='VBT Send Command']").node();
        if (NULL != node) {
            bNeedOpenSession = true;
        }
    }

    VR_LOGI("bNeedOpenSession=[%d]", bNeedOpenSession);
    return bNeedOpenSession;
}

VR_VoiceTslApp::VR_TslStartStatus VR_VoiceTslApp::GetStartStatus()
{
    VR_LOGI("m_emStartStatus=[%d]", m_emStartStatus);
    return m_emStartStatus;
}

void VR_VoiceTslApp::SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus emStartStatus)
{
    VR_LOGI("m_emStartStatus=[%d],emStartStatus=[%d]", m_emStartStatus, emStartStatus);
    m_emStartStatus = emStartStatus;
}

VR_String VR_VoiceTslApp::GetPhoneConnectState()
{
    VR_String strPhoneConnectState = "";
    NCUri uri("content://system/info?bt/hfp-main");
    NCContentResolver resolver;
    ncsp<NCCursor>::sp queryResult = resolver.query(uri, NCList<NCString>(), NCString(),
                                                    NCList<NCString>(), NCString());
    if ((queryResult != NULL) && (1 == queryResult->getCount()) && (queryResult->moveToFirst())) {
        strPhoneConnectState = queryResult->getString(0);
    }
    VR_LOGI("strPhoneConnectState=[%s]", strPhoneConnectState.c_str());
    return strPhoneConnectState;
}

VR_String VR_VoiceTslApp::GetSendMsgXml(const VR_String &strInfo)
{
    const VR_String xml = "<event name=\"SendAppXmlMessage\">"
                                     "%1%"
                                 "</event>";
    VR_String resultXml = (boost::format(xml) % strInfo).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

bool VR_VoiceTslApp::SendMessage2Engine(const std::string &msg)
{
    VR_LOGI("m_spEngineIf=[%p], msg=[%s]",
           m_spDialogEngineIF.get(), msg.c_str());

    if (NULL != m_spDialogEngineIF) {
        m_spDialogEngineIF->SendMessage(msg);
    }
    return (NULL != m_spDialogEngineIF);
}

bool VR_VoiceTslApp::ReplyRequest(VR_ProtoMessageBase* respMsg)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VR_ProtoMessageBase> msg(respMsg);
    if (NULL != m_spContext) {
        EV_EventSender().Reply(*m_spContext, msg.release());
    }
    return (NULL != m_spContext);
}

void VR_VoiceTslApp::NotifyTslAppIsInstalled(const VR_String &msg)
{
    VR_LOGD_FUNC();
    VR_String data = "";
    nutshell::BYTE byAvailable = byAppListUnAvailable;
    static const VR_String strData = "[CDATA[";
    VR_INT32 nPos = msg.find(strData);
    if (0 <= nPos) {
        VR_INT32 nPos1 = msg.find("]", nPos);
        if (0 <= nPos1) {
            VR_INT32 len = nPos + strData.length();
            data = msg.substr(len, nPos1 - len);
        }
    }
    if (!data.empty()) {
        byAvailable = byAppListAvailable;
    }
    NotifyAppListState(byAvailable, true);
}

void VR_VoiceTslApp::NotifyAppListState(const nutshell::BYTE byAvailable, bool bSetValue)
{
    VR_LOGI("byAvailable=[%d]", byAvailable);
    VR_String xml = "<display agent=\"Common\" content=\"TSLState\">"
                        "<enable>%1%</enable>"
                    "<display>";
    if (bSetValue) {
        BL_DataStorage dataStorage;
        CL_BOOL bRet = dataStorage.PutValue(BL_DATA_KEY_VR_TSLAPPEXIST, &byAvailable, sizeof(byAvailable));
        if (!bRet) {
            VR_LOGD("PutValue BL_DATA_KEY_VR_TSLAPPEXIST failed");
        }
    }
    VR_INT32 nAvailable = byAvailable;
    VR_String result = (boost::format(xml) % std::to_string(nAvailable)).str();
    VR_LOGI("result=[%s]", result.c_str());
    std::unique_ptr<VrNotifyRecogResult> protoMsg(VrNotifyRecogResult::default_instance().New());
    protoMsg->set_sender("tslStatus");
    protoMsg->set_taskid(-1);
    protoMsg->set_recogresult(result);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYRECOGRESULT, protoMsg.release());

    xml = "<event name=\"updateState\">"\
            "<item key=\"TSL_APPS_AVAILABLE\" value=\"%1%\"/>"\
     "</event>";
    VR_String tmp = (nAvailable ? "true" : "false");
    result = (boost::format(xml) % tmp).str();
    SendMessage2Engine(result);
    VR_ConfigureIF::Instance()->setUpdateState(VR_TSL_APPS_AVAILABLE, tmp);
}
/* EOF */
