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
#include   "VR_StartServiceHandler.h"
#include   "gtest/gtest.h"
#include   "VR_DialogEngine_mock.h"
#include   "VR_EventSenderIF.h"
#include   "VR_ContentProvider.h"
#include   "VR_EventSender_mock.h"
#include   "EV_EventContext.h"
#include   "VR_Configure.h"
#include   "VR_ConfigureIF.h"

#include   "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include   "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include   "navi/Voice/VoiceRecog/RequestVrTSL.pb.h"
#include   "navi/Voice/VoiceRecog/VrNotifyUICrash.pb.h"
#include   "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"

using namespace navi::VoiceRecog;

/**
 *   @brief  the class of VR_StartServiceHandler_Substitute
 *
 *    overwrite  func of CheckStartData
 */
class  VR_StartServiceHandler_Substitute : public  VR_StartServiceHandler
{
public:
    VR_StartServiceHandler_Substitute(spVR_DialogEngineIF spDialogEngine, spVR_EventSenderIF spEventSender)
        : VR_StartServiceHandler(spDialogEngine, spEventSender)
        , m_startCodeError(Start_Condition_Ok)
    {

    }

protected:
     virtual Start_Condition_Error CheckStartData()
     {
         return m_startCodeError;
     }

private:
     Start_Condition_Error   m_startCodeError;
};


/**
* VR_StartServiceHandler_Test.cpp
*
* The class is just for VR_StartServiceHandler test.
*/
class  VR_StartServiceHandler_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_StartServiceHandler_Substitute*     m_VR_StartServiceHandler;
};


void
VR_StartServiceHandler_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_EventSenderIF  spSender(VR_new VR_EventSender_mock);
    m_VR_StartServiceHandler = VR_new  VR_StartServiceHandler_Substitute(spEngine, spSender);
}

void
VR_StartServiceHandler_Test::TearDown()
{
    if (m_VR_StartServiceHandler) {
        delete  m_VR_StartServiceHandler;
        m_VR_StartServiceHandler = NULL;
    }
}

TEST_F(VR_StartServiceHandler_Test, OnReqestCommonProperty_Case)
{
    SUCCEED();

    m_VR_StartServiceHandler->m_spEventContext = spEV_EventContext(VR_new EV_EventContext());
    m_VR_StartServiceHandler->m_errorCode = Start_SearchFun_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_DictionData_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_LangData_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_ContactData_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_MediaVOCS_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_MicroPhone_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_PTTSW_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_HigherInterput_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_ChinaLanguage_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_TslInstalling_Error;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_Condition_Ok;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    m_VR_StartServiceHandler->m_errorCode = Start_Condition_Unkown;
    m_VR_StartServiceHandler->OnReplyCommonProperty();
    VrStartServiceResult  result;
    result.set_code(StartService_dic_lang);
    result.set_result(2);
    m_VR_StartServiceHandler->OnMessage(result);
    result.set_code(StartService_Conditon_ok);
    result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(result);

    RequestVrCommonProperty  comomProperty;
    m_VR_StartServiceHandler->OnReqestCommonProperty(EV_EventContext(), comomProperty);

}

TEST_F(VR_StartServiceHandler_Test, OnMessage_Case)
{
    VrStartServiceResult  Result;
    m_VR_StartServiceHandler->OnMessage(Result);
    ///////////////
    Result.set_code(StartService_SearchFunc);
    Result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(Result);
    ///////////////
    Result.set_code(StartService_DictionData);
    Result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(Result);
    ///////////////
    Result.set_code(StartService_LangData);
    Result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(Result);
    ///////////////
    Result.set_code(StartService_ContactData);
    Result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(Result);
    ///////////////
    Result.set_code(StartService_MediaVOCS);
    Result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(Result);
    ///////////////
    Result.set_code(StartService_MicPhone);
    Result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(Result);
    ///////////////
    Result.set_code(StartService_PTTSW);
    Result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(Result);
    ///////////////
    Result.set_code(StartService_Higher);
    Result.set_result(0);
    m_VR_StartServiceHandler->OnMessage(Result);
    SUCCEED();
}

TEST_F(VR_StartServiceHandler_Test, InitializNotifyUICrashe_Case)
{
    m_VR_StartServiceHandler->Initialize();
    SUCCEED();
}

TEST_F(VR_StartServiceHandler_Test, NotifyTslState_Case)
{
    m_VR_StartServiceHandler->NotifyTslState();
    SUCCEED();
}

TEST_F(VR_StartServiceHandler_Test, RequestVrTslState_Case)
{
    EV_EventContext  context;
    RequestVrTSL     requestVrTsl;
    requestVrTsl.set_sender("vr_fc");
    requestVrTsl.set_funccode(VrTSLFunc_AppsInstallingState);
    VrTSLAppsInstallingState* appState = requestVrTsl.mutable_appsinstallingstate();
    appState->set_installingstate("end");
    m_VR_StartServiceHandler->RequestVrTslState(context, requestVrTsl);
    //////////////////////
    appState->set_installingstate("start");
    m_VR_StartServiceHandler->RequestVrTslState(context, requestVrTsl);
    //////////////////////
    appState->set_installingstate("other");
    m_VR_StartServiceHandler->RequestVrTslState(context, requestVrTsl);
    /////////////////////
    RequestVrTSL   requestTsl_No;
    requestTsl_No.set_sender("vr_fc");
    requestTsl_No.set_funccode(VrTSLFunc_AppsInstallingState);
    m_VR_StartServiceHandler->RequestVrTslState(context, requestTsl_No);
    SUCCEED();
}

TEST_F(VR_StartServiceHandler_Test, NotifyUICrash_Case)
{
    VrNotifyUICrash  uiCrash;
    uiCrash.set_peer("this is a test for dm");
    uiCrash.set_peerpid(1111);
    m_VR_StartServiceHandler->NotifyUICrash(uiCrash);
    //////////////////////
    m_VR_StartServiceHandler->NotifyUICrash(uiCrash);
    SUCCEED();
}

TEST_F(VR_StartServiceHandler_Test, CheckLangDicData_Case)
{
    m_VR_StartServiceHandler->CheckLangDicData();
    SUCCEED();
}

TEST_F(VR_StartServiceHandler_Test,  SetCheckReason_Case)
{
    VR_CheckStartReason   startReason;
    startReason.SetCheckReason();
    SUCCEED();
}


TEST_F(VR_StartServiceHandler_Test,  OnReqestCommonProperty_OtherCase)
{
    EV_EventContext  context;
    RequestVrCommonProperty   commonProperty;
    commonProperty.mutable_reqservicestatus();
    m_VR_StartServiceHandler->OnReqestCommonProperty(context, commonProperty);

    // OnMessage
    VrUnitStatusResult  noTypeResult;
    m_VR_StartServiceHandler->OnMessage(noTypeResult);

    VrUnitStatusResult  startResult;
    for (int type = 0; type <= 7; ++type) {
        startResult.set_type((VrUnitStatusType)type);
        startResult.set_result(true);
        m_VR_StartServiceHandler->OnMessage(startResult);
    }

    // vrstartstatusresult
    VrStartStatusResult  statusResult;
    m_VR_StartServiceHandler->OnMessage(statusResult);

    for (int type = 0; type <= 11; ++type) {
        statusResult.set_starttype((VrStartCheckType)type);
        statusResult.set_startreuslt(true);
        m_VR_StartServiceHandler->OnMessage(statusResult);
        m_VR_StartServiceHandler->OnMessage(statusResult);
        statusResult.set_startreuslt(false);
        m_VR_StartServiceHandler->OnMessage(statusResult);
    }

    VR_Configure* pConfig = (VR_Configure*)VR_ConfigureIF::Instance();
    pConfig->m_currentEngineType = VR_ConfigureIF::VR_EngineType::SUNTEC;
    m_VR_StartServiceHandler->OnNotifyTslServiceAvailable();

    pConfig->m_currentEngineType = VR_ConfigureIF::VR_EngineType::VBT;

    for (int code = 0; code <= 15; ++code) {
        m_VR_StartServiceHandler->m_startCodeError = (Start_Condition_Error)code;
        m_VR_StartServiceHandler->OnNotifyTslServiceAvailable();
    }


    //
    m_VR_StartServiceHandler->m_checkstart_status.m_bSessionUsing = true;
    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineBusy = true;
    RequestVrCommonProperty  ReqCommonProperty;
    m_VR_StartServiceHandler->OnReqestVRServiceIdleStatus(context, ReqCommonProperty);

    m_VR_StartServiceHandler->m_checkstart_status.m_bSessionUsing = false;
    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineBusy = false;
    m_VR_StartServiceHandler->OnReqestVRServiceIdleStatus(context, ReqCommonProperty);

    //
    m_VR_StartServiceHandler->m_bUICrashStatus = true;
    m_VR_StartServiceHandler->m_checkstart_status.m_bSessionUsing = true;
    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineBusy = true;
    m_VR_StartServiceHandler->OnNotifyTslServiceAvailable();

    m_VR_StartServiceHandler->m_checkstart_status.m_bSessionUsing = false;
    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineBusy = false;
    m_VR_StartServiceHandler->OnNotifyTslServiceAvailable();

    m_VR_StartServiceHandler->m_bUICrashStatus = false;
    m_VR_StartServiceHandler->OnNotifyTslServiceAvailable();

    VrReqServiceInfo  serviceInfo;
    for (int code = 0; code <= 15; ++code) {
        m_VR_StartServiceHandler->m_errorCode = (Start_Condition_Error)code;
        m_VR_StartServiceHandler->OnReplyCommonPropertyWithHints(serviceInfo);
    }
    SUCCEED();
}

TEST_F(VR_StartServiceHandler_Test,  OnRequestCommonProperty_123_OtherCase)
{
    // m_spEventContext is not null
    RequestVrCommonProperty  commonProperty;
    EV_EventContext   context;
    m_VR_StartServiceHandler->OnReqestCommonProperty(context, commonProperty);
    m_VR_StartServiceHandler->OnReqestCommonProperty(context, commonProperty);
    
    // checkstartstatus
    m_VR_StartServiceHandler->m_checkstart_status.m_bInInitializing = true;
    m_VR_StartServiceHandler->CheckStartStatus();

    m_VR_StartServiceHandler->m_checkstart_status.m_bInInitializing = false;
    m_VR_StartServiceHandler->CheckStartStatus();

    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineBusy = true;
    m_VR_StartServiceHandler->CheckStartStatus();

    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineBusy = false;
    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineTask = false;
    m_VR_StartServiceHandler->m_checkstart_status.m_bSessionUsing = false;
    m_VR_StartServiceHandler->m_checkstart_status.m_bTtsCrashStatus = false;
    m_VR_StartServiceHandler->CheckStartStatus();

    // OnVoiceSettingAvailable
    m_VR_StartServiceHandler->m_startCodeError = Start_Condition_Ok;
    m_VR_StartServiceHandler->OnVoiceSettingAvailable();
    m_VR_StartServiceHandler->m_startCodeError = Start_Condition_Unkown;
    m_VR_StartServiceHandler->OnVoiceSettingAvailable();

    // OnReqestVRStatusWithHints
    VrReqServiceInfo  message;
    m_VR_StartServiceHandler->OnReqestVRStatusWithHints(context, message);

    // OnReqestVoiceTagStatus
    RequestVrVoiceTag  voicetag;
    m_VR_StartServiceHandler->OnReqestVoiceTagStatus(context, voicetag);

    // OnNotifyVRServiceIdleStatus
    m_VR_StartServiceHandler->m_bUICrashStatus = true;
    m_VR_StartServiceHandler->m_checkstart_status.m_bSessionUsing = false;
    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineBusy = false;
    m_VR_StartServiceHandler->OnNotifyVRServiceIdleStatus();

    m_VR_StartServiceHandler->m_checkstart_status.m_bSessionUsing = true;
    m_VR_StartServiceHandler->m_checkstart_status.m_bEngineBusy = true;
    m_VR_StartServiceHandler->OnNotifyVRServiceIdleStatus();

    m_VR_StartServiceHandler->m_bUICrashStatus = false;
    m_VR_StartServiceHandler->OnNotifyVRServiceIdleStatus();

    SUCCEED();
}

/* EOF */
