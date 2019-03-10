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

#include "gtest/gtest.h"

#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include <memory>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "VR_EventSenderIF.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_ActionEventSender.h"
#include "VR_VoiceTagAction.h"
#include "VR_ActionTtsPlay.h"
#include "VR_ActionBeepPlay.h"

#include "navi/Voice/VoiceRecog/VrCommonDef.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"
#include "navi/Voice/VoiceRecog/RespVrVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyVoiceTagResult.pb.h"
#include "navi/Voice/TTS/TtsStop.pb.h"

using namespace navi::VoiceRecog;

const VR_String ADD_RECORD_VOICETAG    = VR_String("addRecordVoiceTag");
const VR_String EDIT_RECORD_VOICETAG   = VR_String("editRecordVoiceTag");
const VR_String SAVE_VOICETAG          = VR_String("saveVoiceTag");
const VR_String DELETE_VOICETAG        = VR_String("deleteVoiceTag");
const VR_String DELETE_ALL_VOICETAG    = VR_String("deleteAllVoiceTag");
const VR_String PLAY_VOICETAG          = VR_String("playVoiceTag");
const VR_String STOP_VOICETAG          = VR_String("stopVoiceTag");
const VR_String CANCEL_RECORD_VOICETAG = VR_String("cancelRecordVoiceTag");
const VR_String CANCEL_SAVE_VOICETAG   = VR_String("cancelSaveVoiceTag");
const VR_INT32  INVALID_VALUE          = -1;

/**
* VR_VoiceTagQueue_test
*
* The class is just for VR_VoiceTagQueue test.
*/
class VR_VoiceTagQueue_test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    VR_VoiceTagQueue *m_voiceTagQueue;
};

void VR_VoiceTagQueue_test::SetUp()
{
    m_voiceTagQueue = VR_new VR_VoiceTagQueue();
}

void VR_VoiceTagQueue_test::TearDown()
{
    if (NULL != m_voiceTagQueue) {
        delete m_voiceTagQueue;
        m_voiceTagQueue = NULL;
    }
}

TEST_F(VR_VoiceTagQueue_test, GetVoiceTagActionIDs_Case)
{
    m_voiceTagQueue->GetVoiceTagActionIDs();
    SUCCEED();
}

TEST_F(VR_VoiceTagQueue_test, RemoveAction_Case)
{
    m_voiceTagQueue->RemoveAction(1);
    SUCCEED();
}

/**
* VR_VoiceTagAction_Test
*
* The class is just for VR_VoiceTagAction test.
*/
class VR_VoiceTagAction_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_VoiceTagAction *m_vr_voiceTag_action;
};

void VR_VoiceTagAction_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_AddNewRecord);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(i);
    }
    protoMsg->mutable_voicetag()->set_delect_count(1);
    protoMsg->mutable_voicetag()->set_delect_accumulation_count(1);
    protoMsg->mutable_voicetag()->set_classification(ReqVrVoiceTagClassification_New);
    protoMsg->mutable_voicetag()->set_device_address_charset(ReqVrVoiceTagCharsets_UTF8);
    protoMsg->mutable_voicetag()->set_device_address("device_address");
    protoMsg->mutable_voicetag()->set_name_data_charset(ReqVrVoiceTagCharsets_UTF8);
    protoMsg->mutable_voicetag()->set_name_data("name_data");
    ReqVrVoiceTagPhoneInfo* phoneinfos1 = protoMsg->mutable_voicetag()->add_phone_infos();
    phoneinfos1->set_phone_number_len(9);
    phoneinfos1->set_phone_type(ReqVrVoiceTagPhoneType_MobileIcon1);
    phoneinfos1->set_phone_number("123456789");
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction(m_sp_EventSender, context, *protoMsg);
}

void VR_VoiceTagAction_Test::TearDown()
{
    if (NULL != m_vr_voiceTag_action) {
        delete m_vr_voiceTag_action;
        m_vr_voiceTag_action = NULL;
    }
}

TEST_F(VR_VoiceTagAction_Test, InitVoiceTagInfo_Case)
{
    m_vr_voiceTag_action->InitVoiceTagInfo();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, InitVoiceTagInfo_phone_number_len0)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_AddNewRecord);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(i);
    }
    protoMsg->mutable_voicetag()->set_delect_count(1);
    protoMsg->mutable_voicetag()->set_delect_accumulation_count(1);
    protoMsg->mutable_voicetag()->set_classification(ReqVrVoiceTagClassification_New);
    protoMsg->mutable_voicetag()->set_device_address_charset(ReqVrVoiceTagCharsets_UTF8);
    protoMsg->mutable_voicetag()->set_device_address("device_address");
    protoMsg->mutable_voicetag()->set_name_data_charset(ReqVrVoiceTagCharsets_UTF8);
    protoMsg->mutable_voicetag()->set_name_data("name_data");
    ReqVrVoiceTagPhoneInfo* phoneinfos1 = protoMsg->mutable_voicetag()->add_phone_infos();
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction(m_sp_EventSender, context, *protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, InitVoiceTagInfo_NoVoiceTag)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction(m_sp_EventSender, context, *protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, InitVoiceTagInfo_other)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_AddNewRecord);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(i);
    }
    protoMsg->mutable_voicetag();
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction(m_sp_EventSender, context, *protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, StopVoice_Case)
{
    m_vr_voiceTag_action->StopVoice(1);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, StopCurTtsAndBeep_Case)
{
    m_vr_voiceTag_action->StopCurTtsAndBeep();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_AddRecord)
{
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_EditRecord)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_EditRecord;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_Register)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Register;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_Delete)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Delete;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_DeleteAll)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_DeleteAll;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_Play)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Play;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_Stop)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Stop;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_CancelRecord)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_CancelRecord;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_CancelRegister)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_CancelRegister;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, Run_Faile)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Faile;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_AddNewRecord);

    m_vr_voiceTag_action->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyRequest_Case)
{
    std::unique_ptr<RespVrVoiceTagResult> voiceTagResult(RespVrVoiceTagResult::default_instance().New());
    voiceTagResult->set_sender("DM2FC");
    voiceTagResult->set_funccode(VrVoiceTagFunc_AddNewRecord);
    voiceTagResult->set_result(true);

    m_vr_voiceTag_action->ReplyRequest(voiceTagResult.release());
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyRequest_NULL)
{
    std::unique_ptr<RespVrVoiceTagResult> voiceTagResult(RespVrVoiceTagResult::default_instance().New());
    voiceTagResult->set_sender("DM2FC");
    voiceTagResult->set_funccode(VrVoiceTagFunc_AddNewRecord);
    voiceTagResult->set_result(true);

    m_vr_voiceTag_action->m_spContext = NULL;
    m_vr_voiceTag_action->ReplyRequest(voiceTagResult.release());
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, SendMessage2Engine_Case)
{
    m_vr_voiceTag_action->SendMessage2Engine("SendMessage2Engine");
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, VoiceTagRecordXml_Case)
{
    m_vr_voiceTag_action->VoiceTagRecordXml(ADD_RECORD_VOICETAG);
    m_vr_voiceTag_action->VoiceTagRecordXml(EDIT_RECORD_VOICETAG);
    m_vr_voiceTag_action->VoiceTagRecordXml(SAVE_VOICETAG);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, VoiceTagSaveXml_Case)
{
    m_vr_voiceTag_action->VoiceTagSaveXml();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, VoiceTagDeleteXml_Case)
{
    m_vr_voiceTag_action->VoiceTagDeleteXml(1);
    m_vr_voiceTag_action->VoiceTagDeleteXml(INVALID_VALUE);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, VoiceTagPlayBackXml_Case)
{
    m_vr_voiceTag_action->VoiceTagPlayBackXml("VoiceTagPlayBack");
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, VoiceTagCancelXml_Case)
{
    m_vr_voiceTag_action->VoiceTagCancelXml("VoiceTagCancel");
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagResult_Case)
{
    m_vr_voiceTag_action->ReplyVoiceTagResult(true);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_NoFuncCode)
{

    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(i);
    }
    protoMsg->mutable_voicetag()->set_delect_count(1);
    protoMsg->mutable_voicetag()->set_delect_accumulation_count(1);
    protoMsg->mutable_voicetag()->set_classification(ReqVrVoiceTagClassification_New);
    protoMsg->mutable_voicetag()->set_device_address_charset(ReqVrVoiceTagCharsets_UTF8);
    protoMsg->mutable_voicetag()->set_device_address("device_address");
    protoMsg->mutable_voicetag()->set_name_data_charset(ReqVrVoiceTagCharsets_UTF8);
    protoMsg->mutable_voicetag()->set_name_data("name_data");
    ReqVrVoiceTagPhoneInfo* phoneinfos1 = protoMsg->mutable_voicetag()->add_phone_infos();
    phoneinfos1->set_phone_number_len(9);
    phoneinfos1->set_phone_type(ReqVrVoiceTagPhoneType_MobileIcon1);
    phoneinfos1->set_phone_number("123456789");
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction(m_sp_EventSender, context, *protoMsg);

    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg1(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg1->mutable_playresult()->set_result(true);

    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Stop;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg1);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_registerresult)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_AddNewRecord);
    protoMsg->mutable_registerresult()->add_voicetagid(1);
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_Recorded);

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_registerresult_Failure)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_CancelRecord);
    protoMsg->mutable_registerresult()->add_voicetagid(1);
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_Failure);

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_registerresult_CancelRecord)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_CancelRecord);
    protoMsg->mutable_registerresult()->add_voicetagid(1);
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_RecordCanceled);

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_registerresult_DeleteAll)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_DeleteAll);
    protoMsg->mutable_registerresult()->add_voicetagid(1);
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_RecordCanceled);

    m_vr_voiceTag_action->m_nVoiceTagIDCount = 2;
    m_vr_voiceTag_action->m_voiceTagResultCode = VrVoiceTagFunc_CancelRecord;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_registerresult_CancelRegister)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_DeleteAll);
    protoMsg->mutable_registerresult()->add_voicetagid(1);
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_RecordCanceled);

    m_vr_voiceTag_action->m_nVoiceTagIDCount = 2;
    m_vr_voiceTag_action->m_voiceTagResultCode = VrVoiceTagFunc_CancelRegister;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_registerresult_Undefined)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_DeleteAll);
    protoMsg->mutable_registerresult()->add_voicetagid(1);
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_RecordCanceled);

    m_vr_voiceTag_action->m_nVoiceTagIDCount = 2;
    m_vr_voiceTag_action->m_voiceTagResultCode = VrVoiceTagFunc_Undefined;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_registerresult_CancelRegister1)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_CancelRegister);
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_RecordCanceled);

    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_CancelRegister;
    m_vr_voiceTag_action->m_nVoiceTagIDCount = 0;
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_playresult)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_Play);
    protoMsg->mutable_playresult()->set_result(true);

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, ReplyVoiceTagNotification_playresult_Stop)
{
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_Stop);
    protoMsg->mutable_playresult();

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->ReplyVoiceTagNotification(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_Test, DeleteVoiceTagAction_Case)
{
    m_vr_voiceTag_action->DeleteVoiceTagAction();
    SUCCEED();
}

/* EOF */
