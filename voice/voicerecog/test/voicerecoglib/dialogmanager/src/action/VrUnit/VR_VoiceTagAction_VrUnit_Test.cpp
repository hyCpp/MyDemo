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
#include "VR_VoiceTagAction_VrUnit.h"
#include "VR_ActionTtsPlay.h"
#include "VR_ActionBeepPlay.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_SessionManger.h"

#include "navi/Voice/VoiceRecog/VrCommonDef.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/RequestVrVoiceTag.pb.h"
#include "navi/Voice/VoiceRecog/RespVrVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyVoiceTagResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrVoiceTag.pb.h"
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
* VR_VoiceTagQueue_VrUnit_test
*
* The class is just for VR_VoiceTagQueue_VrUnit test.
*/
class VR_VoiceTagQueue_VrUnit_test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    VR_VoiceTagQueue_VrUnit *m_voiceTagQueue;
};

void VR_VoiceTagQueue_VrUnit_test::SetUp()
{
    m_voiceTagQueue = VR_new VR_VoiceTagQueue_VrUnit();
}

void VR_VoiceTagQueue_VrUnit_test::TearDown()
{
    if (NULL != m_voiceTagQueue) {
        delete m_voiceTagQueue;
        m_voiceTagQueue = NULL;
    }
}

TEST_F(VR_VoiceTagQueue_VrUnit_test, GetVoiceTagActionIDs_Case)
{
    m_voiceTagQueue->GetVoiceTagActionIDs();
    SUCCEED();
}

TEST_F(VR_VoiceTagQueue_VrUnit_test, RemoveAction_Case)
{
    m_voiceTagQueue->RemoveAction(1);
    SUCCEED();
}

/**
* VR_VoiceTagAction_VrUnit_Test
*
* The class is just for VR_VoiceTagAction_VrUnit test.
*/
class VR_VoiceTagAction_VrUnit_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_VoiceTagAction_VrUnit *m_vr_voiceTag_action;
};

void VR_VoiceTagAction_VrUnit_Test::SetUp()
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
}

void VR_VoiceTagAction_VrUnit_Test::TearDown()
{
    if (NULL != m_vr_voiceTag_action) {
        delete m_vr_voiceTag_action;
        m_vr_voiceTag_action = NULL;
    }
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, InitVoiceTagInfo_Case)
{
    m_vr_voiceTag_action->InitVoiceTagInfo();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, InitVoiceTagInfo_phone_number_len0)
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, InitVoiceTagInfo_NoFunccode)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->add_voicetagid(0);
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, InitVoiceTagInfo_other)
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, VoiceTagRegisterResp_Case)
{
    m_vr_voiceTag_action->VoiceTagRegisterResp(true);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, VoiceTagPlayBackResp_Case)
{
    m_vr_voiceTag_action->VoiceTagPlayBackResp(true);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_AddRecord)
{
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->Run();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_EditRecord)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_EditRecord);
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_Register)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Register);
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_Delete)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Delete);
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_DeleteAll)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_DeleteAll);
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_Play)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Play);
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_Stop)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Stop);
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

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_CancelRecord)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_CancelRecord);
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_CancelRegister)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_CancelRegister);
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, Run_Undefined)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Undefined);
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
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_VoiceTagAction_VrUnit *m_action = VR_new VR_VoiceTagAction_VrUnit(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, OnReceive_Case)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Play;
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_Play);
    protoMsg->mutable_playresult()->set_result(true);

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, SetSessionManager_Case)
{
    spVR_SessionManger spSessionManger;
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, SendMessage2Engine_Case)
{
    m_vr_voiceTag_action->SendMessage2Engine("SendMessage2Engine");
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, VoiceTagRecordXml_Case)
{
    m_vr_voiceTag_action->VoiceTagRecordXml(ADD_RECORD_VOICETAG);
    m_vr_voiceTag_action->VoiceTagRecordXml(EDIT_RECORD_VOICETAG);
    m_vr_voiceTag_action->VoiceTagRecordXml(SAVE_VOICETAG);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, VoiceTagSaveXml_Case)
{
    m_vr_voiceTag_action->VoiceTagSaveXml();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, VoiceTagDeleteXml_Case)
{
    m_vr_voiceTag_action->VoiceTagDeleteXml(1);
    m_vr_voiceTag_action->VoiceTagDeleteXml(INVALID_VALUE);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, VoiceTagPlayBackXml_Case)
{
    m_vr_voiceTag_action->VoiceTagPlayBackXml("VoiceTagPlayBack");
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, VoiceTagCancelXml_Case)
{
    m_vr_voiceTag_action->VoiceTagCancelXml("VoiceTagCancel");
    SUCCEED();
}

 TEST_F(VR_VoiceTagAction_VrUnit_Test, ReplyVoiceTagNotification_registerresult)
 {
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Register;
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_Register);
    protoMsg->mutable_registerresult()->add_voicetagid(1);
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_Recorded);

    m_vr_voiceTag_action->VoiceTagNotification(*protoMsg);
    SUCCEED();
 }

 TEST_F(VR_VoiceTagAction_VrUnit_Test, ReplyVoiceTagNotification_NoRegisterresult)
 {
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Register;
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_Register);

    m_vr_voiceTag_action->VoiceTagNotification(*protoMsg);
    SUCCEED();
 }

 TEST_F(VR_VoiceTagAction_VrUnit_Test, ReplyVoiceTagNotification_playresult)
 {
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Play;
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_Play);
    protoMsg->mutable_playresult()->set_result(true);

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->VoiceTagNotification(*protoMsg);
    SUCCEED();
 }

 TEST_F(VR_VoiceTagAction_VrUnit_Test, ReplyVoiceTagNotification_NoPlayresult)
 {
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Play;
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());
    protoMsg->set_funccode(VrVoiceTagFunc_Play);

    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_vr_voiceTag_action->SetSessionManager(spSessionManger);
    m_vr_voiceTag_action->VoiceTagNotification(*protoMsg);
    SUCCEED();
 }

TEST_F(VR_VoiceTagAction_VrUnit_Test, DeleteVoiceTagAction_Case)
{
    m_vr_voiceTag_action->DeleteVoiceTagAction();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrUnit_Test, ChangeToAvaLanResult_Case)
{
    VR_INT32 nRegisterResult = VrNotifyRegisterResult_VrVoiceTagResultCode_Recorded;
    m_vr_voiceTag_action->ChangeToAvaLanResult(nRegisterResult);
    EXPECT_EQ(Recorded, nRegisterResult);
    nRegisterResult = VrNotifyRegisterResult_VrVoiceTagResultCode_Registered;
    m_vr_voiceTag_action->ChangeToAvaLanResult(nRegisterResult);
    EXPECT_EQ(Registered, nRegisterResult);
    nRegisterResult = VrNotifyRegisterResult_VrVoiceTagResultCode_Deleted;
    m_vr_voiceTag_action->ChangeToAvaLanResult(nRegisterResult);
    EXPECT_EQ(Deleted, nRegisterResult);
    nRegisterResult = VrNotifyRegisterResult_VrVoiceTagResultCode_RecordCanceled;
    m_vr_voiceTag_action->ChangeToAvaLanResult(nRegisterResult);
    EXPECT_EQ(RecordingCanceled, nRegisterResult);
    nRegisterResult = VrNotifyRegisterResult_VrVoiceTagResultCode_RegisterCanceled;
    m_vr_voiceTag_action->ChangeToAvaLanResult(nRegisterResult);
    EXPECT_EQ(RegisterCanceled, nRegisterResult);
    nRegisterResult = VrNotifyRegisterResult_VrVoiceTagResultCode_Failure;
    m_vr_voiceTag_action->ChangeToAvaLanResult(nRegisterResult);
    EXPECT_EQ(Failure, nRegisterResult);
    SUCCEED();
}

/* EOF */
