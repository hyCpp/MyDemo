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
#include   "gtest/gtest.h"
#include "gmock/gmock.h"
#include <boost/bind.hpp>

#include   "VR_DialogEngine_mock.h"
#include "VR_EventSenderIF.h"
#include "VR_MessageProcess.h"
#include "VR_SessionManger.h"
#include "VR_DialogManger.h"
#include "VR_ActionManger.h"
#include "VR_DialogManger.h"
#include "VR_VoiceTslApp.h"
#include "dataprovider/VR_DataProvider.h"
#include "action/VR_ActionEventSender.h"
#include "action/VR_ActionFactoryIF.h"
#include "action/VR_ActionFactory.h"
#include "action/VR_InfoActionFactory.h"
// #include "VR_Common_VrUnitAgent.h"
#include "VR_TransformProtoToXml.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_DM_CommandHandle.h"
// #include "VR_AvcLanSender_mock.h"
#include "navi/Voice/VoiceRecog/VrNotifyEngineStatus.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCreateSession.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCreateSessionResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrOpenSession.pb.h"
#include "navi/Voice/VoiceRecog/RespVrOpenSessionResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCloseSession.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCloseSessionResult.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifySessionClosed.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"
#include "navi/Voice/VoiceRecog/RespVrVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/RespVrTSLResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrPVRFlag.pb.h"
#include "navi/Voice/VoiceRecog/RespVrPVRFlagResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrReqOperation.pb.h"
#include "navi/Voice/VoiceRecog/RespVrReqOperationResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrTSL.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrStartRecognition.pb.h"
#include "navi/Voice/VoiceRecog/RespVrStartRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrCancelRecognition.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCancelRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyActionResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyUICrash.pb.h"
#if defined(VR_DM_TARGET_VRUNIT)
#include "VR_VoiceTslApp_VrUnit.h"
#include "navi/Search/DefaultInfoResult.pb.h"
#include "navi/Search/RespStateInfoVRResult.pb.h"
#include "navi/Search/RespCityInfoVRResult.pb.h"
#include "navi/Search/RespStreetInfoVRResult.pb.h"
#include "navi/Search/RespHouseNoInfoVRResult.pb.h"
#include "navi/Search/RespCategorySearchVRResult.pb.h"
#include "navi/Point/PointResponse.pb.h"
#include "navi/Path/RouteStatus.pb.h"
#include "navi/Path/PathPointList.pb.h"
#include "navi/Path/NearLinkResult.pb.h"
#include "navi/Search/RespStateInfoVRResultCN.pb.h"
#include "navi/Search/RespCityInfoVRResultCN.pb.h"
#include "navi/Search/RespDistrictInfoVRResultCN.pb.h"
#include "navi/Search/RespStreetInfoVRResultCN.pb.h"
#include "navi/Search/RespHouseNoInfoVRResultCN.pb.h"
#include "navi/Search/RespAddressInfoVRResultCN.pb.h"
#include "navi/Search/RespPOINameListVRResultCN.pb.h"
#include "navi/Location/LocInfoUpdate.pb.h"
using namespace navi::Search;
using namespace navi::Path;
using namespace navi::Point;
using namespace navi::Location;
#endif
using namespace navi::VoiceRecog;
using namespace navi::dataprovider;
using ::testing::Return;


VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DM_CommandHandle);


/**
* VR_MessageProcess_Test.cpp
*
* The class is just for VR_MessageProcess test.
*/
class  VR_MessageProcess_Test : public testing::Test
{
protected:

    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

    static VR_MessageProcess*     m_VR_MessageProcess;
};

VR_MessageProcess* VR_MessageProcess_Test::m_VR_MessageProcess = NULL;

void
VR_MessageProcess_Test::SetUpTestCase()
{
    spVR_DM_CommandHandle spDMCommandHandler(VR_new VR_DM_CommandHandle);
    m_VR_MessageProcess = VR_new  VR_MessageProcess(spDMCommandHandler);
}

void
VR_MessageProcess_Test::TearDownTestCase()
{
    if (m_VR_MessageProcess) {
        delete  m_VR_MessageProcess;
        m_VR_MessageProcess = NULL;
    }
}

TEST_F(VR_MessageProcess_Test, Initialize_Case)
{
    // TODO
    m_VR_MessageProcess->Initialize();
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, Start_Case)
{
    // TODO
    m_VR_MessageProcess->Start();
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, CheckStatus_Case)
{
    m_VR_MessageProcess->CheckStatus();
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, Stop_Case)
{
    // TODO
    m_VR_MessageProcess->Stop();
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, Destory_Case)
{
    m_VR_MessageProcess->Destory();
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestGetSeessionID_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrCreateSession>  proto(RequestVrCreateSession::default_instance().New());
    m_VR_MessageProcess->OnReqestGetSeessionID(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestOpenSeession_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrOpenSession>  proto(RequestVrOpenSession::default_instance().New());
    m_VR_MessageProcess->OnReqestOpenSeession(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestCloseSeession_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrCloseSession>  proto(RequestVrCloseSession::default_instance().New());
    m_VR_MessageProcess->OnReqestCloseSeession(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestStartDialog_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrStartRecognition>  proto(RequestVrStartRecognition::default_instance().New());
    m_VR_MessageProcess->OnReqestStartDialog(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestCancleDialog_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrCancelRecognition>  proto(RequestVrCancelRecognition::default_instance().New());
    m_VR_MessageProcess->OnReqestCancleDialog(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestUserOperation_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrReqOperation>  proto(RequestVrReqOperation::default_instance().New());
    m_VR_MessageProcess->OnReqestUserOperation(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestStartChangeLanguage_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrCreateSession>  proto(RequestVrCreateSession::default_instance().New());
    m_VR_MessageProcess->OnReqestStartChangeLanguage(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestCommonProperty_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrCommonProperty>  proto(RequestVrCommonProperty::default_instance().New());
    m_VR_MessageProcess->OnReqestCommonProperty(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestVrPVRFlag_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrPVRFlag>  proto(RequestVrPVRFlag::default_instance().New());
    m_VR_MessageProcess->OnReqestVrPVRFlag(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestVrVoiceTag_Case)
{
    // TODO
    EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag>  proto(RequestVrVoiceTag::default_instance().New());
    m_VR_MessageProcess->OnReqestVrVoiceTag(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestVrTsl_Case)
{
    EV_EventContext context;
    std::unique_ptr<RequestVrTSL>  proto(RequestVrTSL::default_instance().New());
    m_VR_MessageProcess->OnReqestVrTsl(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReplyActoinResult_Case)
{
    std::unique_ptr<VrNotifyActionResult>  proto(VrNotifyActionResult::default_instance().New());
    m_VR_MessageProcess->OnReplyActoinResult(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnNotifyUICrash_Case)
{
    std::unique_ptr<VrNotifyUICrash>  proto(VrNotifyUICrash::default_instance().New());
    m_VR_MessageProcess->OnNotifyUICrash(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnReqestVrSettingState_Case)
{
    EV_EventContext context;
    std::unique_ptr<VrRequestSettingProperty>  proto(VrRequestSettingProperty::default_instance().New());
    m_VR_MessageProcess->OnReqestVrSettingState(context, *proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageStarted_Case)
{
    m_VR_MessageProcess->OnMessageStarted();
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_ENGINE_CONTROL_ChangeLanguage)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    proto->mutable_m_controlmessage()->set_type(DMTaskMessageType_ChangeLanguage);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_ENGINE_CONTROL_CloseSession)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    proto->mutable_m_controlmessage()->set_type(DMTaskMessageType_CloseSession);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_ENGINE_CONTROL_InitPersonalData)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    proto->mutable_m_controlmessage()->set_type(DMTaskMessageType_InitPersonalData);
    proto->mutable_m_controlmessage()->mutable_personalresult()->set_datatype(persondata_end);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_ENGINE_CONTROL_Prepare_0)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    proto->mutable_m_controlmessage()->set_type(DMTaskMessageType_prepare);
    proto->mutable_m_controlmessage()->mutable_prepareresult()->set_errcode(0);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_ENGINE_CONTROL_Prepare_1)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    proto->mutable_m_controlmessage()->set_type(DMTaskMessageType_prepare);
    proto->mutable_m_controlmessage()->mutable_prepareresult()->set_errcode(1);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_ENGINE_CONTROL_VehicleInfoResp)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    proto->mutable_m_controlmessage()->set_type(DMTaskMessageType_VehicleInfoResp);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_ENGINE_CONTROL_other)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    proto->mutable_m_controlmessage()->set_type(DMTaskMessageType_GBookUserType);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_ENGINE_CONTROL)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_DE2DM_Has_DICTIONARY_RESULT)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_DE2DM);
    proto->mutable_m_de2dm()->set_id(0);
    proto->mutable_m_de2dm()->set_vr_dm_action_type(ActionType_DP_DICTIONARY_RESULT);
    proto->mutable_m_de2dm()->mutable_dp_dictionary_result()->set_type(DP_TUNER_DICTIONARY_RESULT);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_DE2DM_No_DICTIONARY_RESULT)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_DE2DM);
    proto->mutable_m_de2dm()->set_id(0);
    proto->mutable_m_de2dm()->set_vr_dm_action_type(ActionType_DP_DICTIONARY_RESULT);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_DE2DM_other)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_DE2DM);
    proto->mutable_m_de2dm()->set_id(0);
    proto->mutable_m_de2dm()->set_vr_dm_action_type(ActionType_MESSAGE_INFO_RESP);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_DE2DM)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_DE2DM);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_FC2DM)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    proto->mutable_m_fc2dm();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_FC2DM_Tutorial)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_sender("VoiceRecogTutorial");
    proto->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    proto->mutable_m_fc2dm();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_FC2DM)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_FC2DM);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_SERVICE_STARTED)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_SERVICE_STARTED);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_AUDIO_MODE)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_MODE);
    proto->mutable_m_audiomode();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_AUDIO_MODE)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_MODE);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_USB_CONNECT_STATUS)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_DP_USB_CONNECT_STATUS);
    proto->mutable_m_agentusb_connected();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_USB_CONNECT_STATUS)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_DP_USB_CONNECT_STATUS);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_AGENT_ACTIVATION)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_AGENT_ACTIVATION);
    proto->mutable_m_agentactivation();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_AGENT_ACTIVATION)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_AGENT_ACTIVATION);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_METER_DM2FC)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_METER_DM2FC);
    proto->mutable_m_meterinfo();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_METER_DM2FC)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_METER_DM2FC);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_METER_DE2DM)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_METER_DE2DM);
    proto->mutable_m_meterinfo();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_METER_DE2DM)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_METER_DE2DM);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_UNIT_STATUS)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_UNIT_STATUS);
    proto->mutable_m_status();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_UNIT_STATUS)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_UNIT_STATUS);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_DM2DE)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_DM2DE);
    proto->mutable_m_vrsendmsgdm2de();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_DM2DE)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_DM2DE);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_HMIMODE_HIGHINTER_NOTIFY)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_HMIMODE_HIGHINTER_NOTIFY);
    proto->mutable_m_vrhigherhmimodenoitfy();
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_HMIMODE_HIGHINTER_NOTIFY)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_HMIMODE_HIGHINTER_NOTIFY);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_AUDIO_STATUS)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_STATUS);
    proto->mutable_m_audio_status_notify()->set_type(CurrentAudioSource);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_NO_AUDIO_STATUS)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_AUDIO_STATUS);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_LAZYSERVICE_NOTIFY)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_LAZYSERVICE_NOTIFY);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageLoopBack_default)
{
    std::unique_ptr<VrLoopBack>  proto(VrLoopBack::default_instance().New());
    proto->set_type(VrLoopBack_MSG_Type_MSG_UNKNOW);
    m_VR_MessageProcess->OnMessageLoopBack(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageSessionClosed_Case)
{
    std::unique_ptr<VrNotifySessionClosed>  proto(VrNotifySessionClosed::default_instance().New());
    proto->set_session("session");
    m_VR_MessageProcess->OnMessageSessionClosed(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessage2DialogEngine_Case)
{
    std::unique_ptr<VrNotifySessionClosed>  proto(VrNotifySessionClosed::default_instance().New());
    proto->set_session("session");
    m_VR_MessageProcess->OnMessage2DialogEngine(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessage2DialogManger_Case)
{
    std::unique_ptr<VrNotifySessionClosed>  proto(VrNotifySessionClosed::default_instance().New());
    proto->set_session("session");
    m_VR_MessageProcess->OnMessage2DialogManger(*proto);
    SUCCEED();
}

#if defined(VR_DM_TARGET_VRUNIT)
TEST_F(VR_MessageProcess_Test, OnMessageNaviReqDfltInfoResp_Case)
{
    std::unique_ptr<DefaultInfoResult>  proto(DefaultInfoResult::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviReqDfltInfoResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchStateResp_Case)
{
    std::unique_ptr<RespStateInfoVRResult>  proto(RespStateInfoVRResult::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchStateResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchCityResp_Case)
{
    std::unique_ptr<RespCityInfoVRResult>  proto(RespCityInfoVRResult::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchCityResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchStreetResp_Case)
{
    std::unique_ptr<RespStreetInfoVRResult>  proto(RespStreetInfoVRResult::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchStreetResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchHouseNoResp_Case)
{
    std::unique_ptr<RespHouseNoInfoVRResult>  proto(RespHouseNoInfoVRResult::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchHouseNoResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchPoiInfoResp_Case)
{
    std::unique_ptr<RespCategorySearchVRResult>  proto(RespCategorySearchVRResult::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchPoiInfoResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryPointCollectionResp_Case)
{
    std::unique_ptr<PointResponse>  proto(PointResponse::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryPointCollectionResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryDestHistoryResp_Case)
{
    std::unique_ptr<PointResponse>  proto(PointResponse::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryDestHistoryResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryRouteExistResp_Case)
{
    std::unique_ptr<RouteStatus>  proto(RouteStatus::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryRouteExistResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryAllDestListResp_Case)
{
    std::unique_ptr<PathPointList>  proto(PathPointList::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryAllDestListResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryUnpassedDestListResp_Case)
{
    std::unique_ptr<PathPointList>  proto(PathPointList::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryUnpassedDestListResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryPointTypeResp_Case)
{
    std::unique_ptr<NearLinkResult>  proto(NearLinkResult::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryPointTypeResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryNextDestResp_Case)
{
    std::unique_ptr<PathPointList>  proto(PathPointList::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryNextDestResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryCurrentPosResp_Case)
{
    std::unique_ptr<LocInfoUpdate>  proto(LocInfoUpdate::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryCurrentPosResp(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviQueryPOIByNameRespCN_Case)
{
    std::unique_ptr<RespPOINameListVRResultCN>  proto(RespPOINameListVRResultCN::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviQueryPOIByNameRespCN(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchStateRespCN_Case)
{
    std::unique_ptr<RespStateInfoVRResultCN>  proto(RespStateInfoVRResultCN::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchStateRespCN(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchCityRespCN_Case)
{
    std::unique_ptr<RespCityInfoVRResultCN>  proto(RespCityInfoVRResultCN::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchCityRespCN(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchDistrictRespCN_Case)
{
    std::unique_ptr<RespDistrictInfoVRResultCN>  proto(RespDistrictInfoVRResultCN::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchDistrictRespCN(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchStreetRespCN_Case)
{
    std::unique_ptr<RespStreetInfoVRResultCN>  proto(RespStreetInfoVRResultCN::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchStreetRespCN(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchHouseNoRespCN_Case)
{
    std::unique_ptr<RespHouseNoInfoVRResultCN>  proto(RespHouseNoInfoVRResultCN::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchHouseNoRespCN(*proto);
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnMessageNaviSearchAddressInfoCN_Case)
{
    std::unique_ptr<RespAddressInfoVRResultCN>  proto(RespAddressInfoVRResultCN::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviSearchAddressInfoCN(*proto);
    SUCCEED();
}

#include "navi/Path/NotifyRouteStatus.pb.h"
TEST_F(VR_MessageProcess_Test, OnMessageNaviNotifyRouteStatus_Case)
{
    std::unique_ptr<navi::Path::NotifyRouteStatus>  proto(navi::Path::NotifyRouteStatus::default_instance().New());
    m_VR_MessageProcess->OnMessageNaviNotifyRouteStatus(*proto);
    SUCCEED();
}

#endif

TEST_F(VR_MessageProcess_Test, OnMessageReqWebSearchEngine_Case)
{
    m_VR_MessageProcess->OnMessageReqWebSearchEngine();
    SUCCEED();
}

TEST_F(VR_MessageProcess_Test, OnStartedNotify_Case)
{
    // m_VR_MessageProcess->OnStartedNotify();
    SUCCEED();
}


/* EOF */
