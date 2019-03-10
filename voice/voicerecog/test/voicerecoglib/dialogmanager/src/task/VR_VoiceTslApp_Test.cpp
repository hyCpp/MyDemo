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
#include   "VR_VoiceTslApp.h"
#include   "gtest/gtest.h"
#include   "VR_DialogEngine_mock.h"
// #include "VR_AvcLanSender_mock.h"
#include   "VR_EventSenderIF.h"
#include   "VR_SessionManger.h"
#include   "EV_EventContext.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include   "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include   "navi/Voice/VoiceRecog/RequestVrTSL.pb.h"
#include   "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "cutils/properties.h"

using namespace navi::VoiceRecog;

/**
* VR_VoiceTslApp_Test.cpp
*
* The class is just for VR_VoiceTslApp test.
*/
class  VR_VoiceTslApp_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_VoiceTslApp*     m_VR_VoiceTslApp;
};


void
VR_VoiceTslApp_Test::SetUp()
{
    spVR_EventSenderIF  spEventSender(VR_EventSenderIF::GetSender());
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
 //   spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManager(VR_new VR_SessionManger());
    m_VR_VoiceTslApp =  VR_new  VR_VoiceTslApp(spEventSender, spEngine, spSessionManager);
}

void
VR_VoiceTslApp_Test::TearDown()
{
    if (m_VR_VoiceTslApp) {
        delete  m_VR_VoiceTslApp;
        m_VR_VoiceTslApp = NULL;
    }
}

TEST_F(VR_VoiceTslApp_Test, Start_Case)
{
    VR_String keyName = "local.navi.VR.startstatus";
    char keyValue[PROPERTY_VALUE_MAX] = { 0 };
    m_VR_VoiceTslApp->Start();
    property_set(keyName.c_str(), keyValue);
    m_VR_VoiceTslApp->Start();
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, Cancel_Case)
{
    EV_EventContext  context;
    m_VR_VoiceTslApp->Cancel(context);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_GetSupportLanguage)
{
    EV_EventContext  context;
    RequestVrTSL  vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_GetSupportLanguage);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_InstallAgent)
{
    EV_EventContext context;
    RequestVrTSL  vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_InstallAgent);
    VrTSLInstallAgent* installAgent = vrTsl.mutable_installagent();
    installAgent->set_content("this is only a test");
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_NoInstallAgent)
{
    EV_EventContext context;
    RequestVrTSL  vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_InstallAgent);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_UninstallAgent)
{
    EV_EventContext context;
    RequestVrTSL  vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_UninstallAgent);
    VrTSLUninstallAgent * unInstallAgent = vrTsl.mutable_uninstallagent();
    unInstallAgent->set_content("this is only a test");
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_NoUninstallAgent)
{
    EV_EventContext context;
    RequestVrTSL  vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_UninstallAgent);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_GetSessionID)
{
    EV_EventContext context;
    RequestVrTSL  vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_GetSessionID);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_StartRecog)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_StartRecog);
    VrTSLStartRecognition* startRecg = vrTsl.mutable_startrecog();
    startRecg->set_session("011100101010000101");
    startRecg->set_info("this is only a test");
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_StartRecog_DiffSession_Default)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    VR_String        session;
    VR_String        xml = "<CSVR>"
            "<Message source=\”Pandora\” class=\"System\">"
              "<Action type=\"VBT Send Command\">"
                "<Execution>"
                  "<Parameter name=\"Agent\" value=\"Pandora\"/>"
                  "<Parameter name=\"Command\" value=\"CommandName\"/>"
                "</Execution>"
              "</Action>"
            "</Message>"
            "<Message source=\"Pandora\" class=\"System\">"
              "<Action type=\"VBT Button Down\"/>"
            "</Message>"
          "</CSVR>";
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_StartRecog);
    VrTSLStartRecognition* startRecg = vrTsl.mutable_startrecog();
    m_VR_VoiceTslApp->m_spVR_SessionManger->CreateSession(session);
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus_Default);
    startRecg->set_session(session);
    startRecg->set_info(xml);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_StartRecog_DiffSession_Started)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    VR_String        session;
    VR_String        xml = "<CSVR>"
            "<Message source=\”Pandora\” class=\"System\">"
              "<Action type=\"VBT Send Command\">"
                "<Execution>"
                  "<Parameter name=\"Agent\" value=\"Pandora\"/>"
                  "<Parameter name=\"Command\" value=\"CommandName\"/>"
                "</Execution>"
              "</Action>"
            "</Message>"
            "<Message source=\"Pandora\" class=\"System\">"
              "<Action type=\"VBT Button Down\"/>"
            "</Message>"
          "</CSVR>";
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_StartRecog);
    VrTSLStartRecognition* startRecg = vrTsl.mutable_startrecog();
    m_VR_VoiceTslApp->m_spVR_SessionManger->CreateSession(session);
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus_Started);
    startRecg->set_session(session);
    startRecg->set_info(xml);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_StartRecog_DiffSession_StartOver)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    VR_String        session;
    VR_String        xml = "<CSVR>"
            "<Message source=\”Pandora\” class=\"System\">"
              "<Action type=\"VBT Send Command\">"
                "<Execution>"
                  "<Parameter name=\"Agent\" value=\"Pandora\"/>"
                  "<Parameter name=\"Command\" value=\"CommandName\"/>"
                "</Execution>"
              "</Action>"
            "</Message>"
            "<Message source=\"Pandora\" class=\"System\">"
              "<Action type=\"VBT Button Down\"/>"
            "</Message>"
          "</CSVR>";
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_StartRecog);
    VrTSLStartRecognition* startRecg = vrTsl.mutable_startrecog();
    m_VR_VoiceTslApp->m_spVR_SessionManger->CreateSession(session);
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus_Starting);
    startRecg->set_session(session);
    startRecg->set_info(xml);
    m_VR_VoiceTslApp->m_firstStartDialogAfterStartOver = true;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_StartRecog_DiffSession_StartOverStarted)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    VR_String        session;
    VR_String        xml = "<CSVR>"
            "<Message source=\”Pandora\” class=\"System\">"
              "<Action type=\"VBT Send Command\">"
                "<Execution>"
                  "<Parameter name=\"Agent\" value=\"Pandora\"/>"
                  "<Parameter name=\"Command\" value=\"CommandName\"/>"
                "</Execution>"
              "</Action>"
            "</Message>"
            "<Message source=\"Pandora\" class=\"System\">"
              "<Action type=\"VBT Button Down\"/>"
            "</Message>"
          "</CSVR>";
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_StartRecog);
    VrTSLStartRecognition* startRecg = vrTsl.mutable_startrecog();
    m_VR_VoiceTslApp->m_spVR_SessionManger->CreateSession(session);
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus_Started);
    startRecg->set_session(session);
    startRecg->set_info(xml);
    m_VR_VoiceTslApp->m_firstStartDialogAfterStartOver = true;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_StartRecog_DiffSession_NoOpenSession)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    VR_String        session;
    VR_String        xml = "<CSVR>"
            "<Message source=\"Pandora\" class=\"System\">"
              "<Action type=\"VBT Button Down\"/>"
            "</Message>"
          "</CSVR>";
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_StartRecog);
    VrTSLStartRecognition* startRecg = vrTsl.mutable_startrecog();
    m_VR_VoiceTslApp->m_spVR_SessionManger->CreateSession(session);
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus_Started);
    startRecg->set_session(session);
    startRecg->set_info(xml);
    m_VR_VoiceTslApp->m_firstStartDialogAfterStartOver = true;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_NoStartRecog)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_StartRecog);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_CanlRecog)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_CancelRecog);
    VrTSLCancelRecognition* cancelRecg = vrTsl.mutable_cancelrecog();
    cancelRecg->set_session("0101001100010000111");
    cancelRecg->set_info("this is only a test");
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_CanlRecog_started)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_CancelRecog);
    VrTSLCancelRecognition* cancelRecg = vrTsl.mutable_cancelrecog();
    cancelRecg->set_session("0101001100010000111");
    cancelRecg->set_info("this is only a test");
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus_Started);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_CanlRecog_starting)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_CancelRecog);
    VrTSLCancelRecognition* cancelRecg = vrTsl.mutable_cancelrecog();
    cancelRecg->set_session("0101001100010000111");
    cancelRecg->set_info("this is only a test");
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus_Starting);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_NoCanlRecog)
{
    EV_EventContext  context;
    RequestVrTSL     vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_CancelRecog);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_SendMessage)
{
    EV_EventContext context;
    RequestVrTSL    vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_SendMessage);
    VrTSLSendMessage* message = vrTsl.mutable_message();
    message->set_session("01010001001111000101");
    message->set_content("this is only a test");
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_NoSendMessage)
{
    EV_EventContext context;
    RequestVrTSL    vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_SendMessage);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_SendMessageSession)
{
    EV_EventContext context;
    RequestVrTSL  vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_GetSessionID);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);

    vrTsl.set_funccode(VrTSLFunc_SendMessage);
    VrTSLSendMessage* message = vrTsl.mutable_message();
    message->set_session(m_VR_VoiceTslApp->m_session);
    message->set_content("this is only a test");
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_SendMessage_defaultSession)
{
    EV_EventContext context;
    RequestVrTSL    vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_SendMessage);
    VrTSLSendMessage* message = vrTsl.mutable_message();
    message->set_session("VoiceRecogTSL");
    message->set_content("this is only a test");
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_UpdateAppList)
{
    EV_EventContext context;
    RequestVrTSL    vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_UpdateAppList);
    VrTSLUpdateAppList* appList = vrTsl.mutable_applist();
    appList->set_list("this is only a test");
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_NoUpdateAppList)
{
    EV_EventContext context;
    RequestVrTSL    vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_UpdateAppList);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnRequestVrTsl_Case_default)
{
    EV_EventContext context;
    RequestVrTSL    vrTsl;
    vrTsl.set_sender("vr_fc");
    vrTsl.set_funccode(VrTSLFunc_AppsInstallingState);
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, RespSupportLanguage)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->RespSupportLanguage(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, RespInstallAgent)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->RespInstallAgent(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, RespUninstallAgent)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->RespUninstallAgent(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, RespGetSessionID)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->RespGetSessionID(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, RespCanleRecog)
{
    m_VR_VoiceTslApp->RespCanleRecog(true);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, RespStartRecog)
{
    m_VR_VoiceTslApp->RespStartRecog(true);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, NotifyInstallProgress)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->NotifyInstallProgress(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, NotifyAppXmlMessage)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->NotifyAppXmlMessage(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, NotifyAppRecognitionForceEnd)
{
    m_VR_VoiceTslApp->NotifyAppRecognitionForceEnd();
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, NotifyAppRecognitionEnd)
{
    m_VR_VoiceTslApp->NotifyAppRecognitionEnd();
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, NotifyTSLVRInfo)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->NotifyTSLVRInfo(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, GetSupportLanguage)
{
    m_VR_VoiceTslApp->GetSupportLanguage();
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, InstallAgent)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->InstallAgent(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, UninstallAgent)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->UninstallAgent(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, StartRecog)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->StartRecog(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, CanlRecog)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->CanlRecog(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, UpdateAppList)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->UpdateAppList(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, SendMessage)
{
    VR_String str = "tsl";
    m_VR_VoiceTslApp->SendMessage(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_NoData)
{
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_NotXML)
{
    VR_String str = "tsl";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_install)
{
    VR_String str = "<event-result name=\"install\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_uninstall)
{
    VR_String str = "<event-result name=\"uninstall\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_getSupportedLanguage)
{
    VR_String str = "<event-result name=\"getSupportedLanguage\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_CancelAppRecognition)
{
    VR_String str = "<event-result name=\"CancelAppRecognition\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_CancelAppRecognition_TslSession)
{
    VR_String str = "<event-result name=\"CancelAppRecognition\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->m_spVR_SessionManger->SetSessionType(Session_TSL);
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_StartAppRecognition_StartingRecvCancel)
{
    VR_String str = "<event-result name=\"StartAppRecognition\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslAppStatus_StartingRecvCancel);
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_StartAppRecognition_VR_TslStartStatus_Starting)
{
    VR_String str = "<event-result name=\"StartAppRecognition\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslStartStatus_Starting);
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_StartAppRecognition_VR_TslAppStatus_StartedRecvCancel)
{
    VR_String str = "<event-result name=\"StartAppRecognition\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslAppStatus_StartedRecvCancel);
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_AudioStopByHighInterrupt)
{
    VR_String str = "<event-result name=\"AudioStopByHighInterrupt\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_AudioStopByHighInterrupt_StartingRecvCancel)
{
    VR_String str = "<event-result name=\"AudioStopByHighInterrupt\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslAppStatus_StartingRecvCancel);
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_AudioOffByHighInterrupt)
{
    VR_String str = "<event-result name=\"AudioOffByHighInterrupt\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_AudioOffByHighInterrupt_StartingRecvCancel)
{
    VR_String str = "<event-result name=\"AudioOffByHighInterrupt\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->SetStartStatus(VR_VoiceTslApp::VR_TslAppStatus_StartingRecvCancel);
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_AudioOffByTsl)
{
    VR_String str = "<event-result name=\"AudioOffByTsl\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_event_result_other)
{
    VR_String str = "<event-result name=\"SendAppXmlMessage\" errcode=\"\"/>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_notifyInstallProgress)
{
    VR_String str = "<action name=\"notifyInstallProgress\"></action>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_notifyInstallProgress_startOver)
{
    VR_String str = "<action name=\"notifyInstallProgress\"></action>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->m_firstStartDialogAfterStartOver = true;
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_notifyAppXmlMessage)
{
    VR_String str = "<action name=\"notifyAppXmlMessage\"></action>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_notifyAppRecognitionForceEnd)
{
    VR_String str = "<action name=\"notifyAppRecognitionForceEnd\"></action>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_notifyAppRecognitionEnd)
{
    VR_String str = "<action name=\"notifyAppRecognitionEnd\"></action>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_notifyTSLVRInfo)
{
    VR_String str = "<action name=\"notifyTSLVRInfo\"></action>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_notifyT_other)
{
    VR_String str = "<action name=\"other\"></action>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, OnMessage_other)
{
    VR_String str = "<action-result name=\"other\"></action-result>";
    std::unique_ptr<VrTransmitDataToTSLNotify> protoMsg(VrTransmitDataToTSLNotify::default_instance().New());
    protoMsg->set_data(str);
    protoMsg->set_data_length(str.length());
    m_VR_VoiceTslApp->OnMessage(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, NotifyTslAppIsInstalled)
{
    VR_String str = "[CDATA[tsl]";
    m_VR_VoiceTslApp->NotifyTslAppIsInstalled(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, NotifyTslAppIsInstalled_other)
{
    VR_String str = "[CDATA[tsl";
    m_VR_VoiceTslApp->NotifyTslAppIsInstalled(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, SendMessage2Engine_NULL)
{
    VR_String str = "[CDATA[tsl";
    m_VR_VoiceTslApp->m_spDialogEngineIF = NULL;
    m_VR_VoiceTslApp->SendMessage2Engine(str);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, Checkapplist)
{
    m_VR_VoiceTslApp->Checkapplist();
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, CheckIsNeedOpenSession_loadFailed)
{
    VR_String        xml = "<CSVR"
            "<Message source=\”Pandora\” class=\"System\">"
              "<Action type=\"VBT Send Command\">"
                "<Execution>"
                  "<Parameter name=\"Agent\" value=\"Pandora\"/>"
                  "<Parameter name=\"Command\" value=\"CommandName\"/>"
                "</Execution>"
              "</Action>"
            "</Message>"
            "<Message source=\"Pandora\" class=\"System\">"
              "<Action type=\"VBT Button Down\"/>"
            "</Message>"
          "</CSVR>";
    m_VR_VoiceTslApp->CheckIsNeedOpenSession(xml);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, CheckIsNeedOpenSession_NodeNull)
{
    VR_String        xml = "<CSVR>"
          "</CSVR>";
    m_VR_VoiceTslApp->CheckIsNeedOpenSession(xml);
    SUCCEED();
}

TEST_F(VR_VoiceTslApp_Test, VoiceTsl_0000_OtherCase)
{
    EV_EventContext  context;
    RequestVrTSL  vrTsl;
    vrTsl.set_sender("vrfc");
    vrTsl.set_funccode(VrTSLFunc_StartRecog);
    VrTSLStartRecognition*  startRecog = vrTsl.mutable_startrecog();
    startRecog->set_info("this is only a test.");
    startRecog->set_session("test");
    m_VR_VoiceTslApp->m_session = "test";
    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Default;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Started;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);

    // bNeedOpenSession = true;
    std::string  strInfo =  "<CSVR"
            "<Message source=\”Pandora\” class=\"System\">"
              "<Action type=\"VBT Send Command\">"
                "<Execution>"
                  "<Parameter name=\"Agent\" value=\"Pandora\"/>"
                  "<Parameter name=\"Command\" value=\"CommandName\"/>"
                "</Execution>"
              "</Action>"
            "</Message>"
            "<Message source=\"Pandora\" class=\"System\">"
              "<Action type=\"VBT Button Down\"/>"
            "</Message>"
          "</CSVR>";

    startRecog->set_info(strInfo);
    startRecog->set_session("test");
    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Default;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);

    m_VR_VoiceTslApp->m_firstStartDialogAfterStartOver = true;
    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Started;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);

    m_VR_VoiceTslApp->m_firstStartDialogAfterStartOver = false;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);

    // cancle recog
    vrTsl.set_funccode(VrTSLFunc_CancelRecog);
    VrTSLCancelRecognition* cancleRecog = vrTsl.mutable_cancelrecog();
    cancleRecog->set_info("this is only a test.");
    cancleRecog->set_session("test");
    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Default;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);
    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Started;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);

    // sendmessage
    vrTsl.set_funccode(VrTSLFunc_SendMessage);
    VrTSLSendMessage* sendMessage = vrTsl.mutable_message();
    sendMessage->set_session("helloworld");
    sendMessage->set_content("VBT Send Recognition State");
    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Started;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);

    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Starting;
    m_VR_VoiceTslApp->OnRequestVrTsl(context, vrTsl);

    // OnMessage
    VrEngineControlMessage::VrCheckApplist  applist;
    m_VR_VoiceTslApp->OnMessage(applist);

    VR_String xmlMsg = "<action name=\"OpenTslSession\"></action>";
    VrTransmitDataToTSLNotify  transmitData;
    transmitData.set_data(xmlMsg);
    transmitData.set_data_length(xmlMsg.length());
    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslAppStatus_StartingRecvCancel;
    m_VR_VoiceTslApp->OnMessage(transmitData);

    m_VR_VoiceTslApp->m_emStartStatus = VR_VoiceTslApp::VR_TslStartStatus::VR_TslStartStatus_Starting;
    m_VR_VoiceTslApp->OnMessage(transmitData);

    // GetPhoneConnectState
    m_VR_VoiceTslApp->GetPhoneConnectState();

    // GetSendMsgXml
    m_VR_VoiceTslApp->GetSendMsgXml("test");
    SUCCEED();
}

/* EOF */
