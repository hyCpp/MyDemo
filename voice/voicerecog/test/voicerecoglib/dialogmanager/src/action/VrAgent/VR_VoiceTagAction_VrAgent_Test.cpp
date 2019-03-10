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
#include "VR_VoiceTagAction_VrAgent.h"
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
* VR_VoiceTagQueue_VrAgent_test
*
* The class is just for VR_VoiceTagQueue_VrAgent test.
*/
class VR_VoiceTagQueue_VrAgent_test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    VR_VoiceTagQueue_VrAgent *m_voiceTagQueue;
};

void VR_VoiceTagQueue_VrAgent_test::SetUp()
{
    m_voiceTagQueue = VR_new VR_VoiceTagQueue_VrAgent();
}

void VR_VoiceTagQueue_VrAgent_test::TearDown()
{
    if (NULL != m_voiceTagQueue) {
        delete m_voiceTagQueue;
        m_voiceTagQueue = NULL;
    }
}

TEST_F(VR_VoiceTagQueue_VrAgent_test, GetVoiceTagActionIDs_Case)
{
    m_voiceTagQueue->GetVoiceTagActionIDs();
    SUCCEED();
}

TEST_F(VR_VoiceTagQueue_VrAgent_test, RemoveAction_Case)
{
    m_voiceTagQueue->RemoveAction(1);
    SUCCEED();
}

/**
* VR_VoiceTagAction_VrAgent_Test
*
* The class is just for VR_VoiceTagAction_VrAgent test.
*/
class VR_VoiceTagAction_VrAgent_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_VoiceTagAction_VrAgent *m_vr_voiceTag_action;
};

void VR_VoiceTagAction_VrAgent_Test::SetUp()
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
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
}

void VR_VoiceTagAction_VrAgent_Test::TearDown()
{
    if (NULL != m_vr_voiceTag_action) {
        delete m_vr_voiceTag_action;
        m_vr_voiceTag_action = NULL;
    }
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, InitVoiceTagInfo_Case)
{
    m_vr_voiceTag_action->InitVoiceTagInfo();
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, InitVoiceTagInfo_phone_number_len0)
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
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, InitVoiceTagInfo_NoFunccode)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->add_voicetagid(0);
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, InitVoiceTagInfo_other)
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
    m_vr_voiceTag_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, ReplyVoiceTagResult_Case)
{
    m_vr_voiceTag_action->ReplyVoiceTagResult(true);
    SUCCEED();
}







TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Play_Max)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;

    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Play);
    for (int i = 0; i < 4; ++i) {
        protoMsg->add_voicetagid(9);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Play_Min)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;

    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Play);
    for (int i = 0; i < 4; ++i) {
        protoMsg->add_voicetagid(0);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Play_Exceed)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;

    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Play);
    for (int i = 0; i < 4; ++i) {
        protoMsg->add_voicetagid(10);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Stop_Max)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Stop);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(0x9999);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Stop_Min)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Stop);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(0);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    spVR_SessionManger spSessionManger(VR_new VR_SessionManger(spAvcSender));
    m_action->SetSessionManager(spSessionManger);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Stop_Exceed)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;
    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Stop);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(0x10000);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Register_Max)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;

    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Register);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(0x9999);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Register_Min)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;

    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Register);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(0);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, Run_Register_Exceed)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    const EV_EventContext context;

    std::unique_ptr<RequestVrVoiceTag> protoMsg(RequestVrVoiceTag::default_instance().New());
    protoMsg->set_sender("voiceTag");
    protoMsg->set_funccode(VrVoiceTagFunc_Register);
    for (int i = 0; i < 5; ++i) {
        protoMsg->add_voicetagid(0x10000);
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
    VR_VoiceTagAction_VrAgent *m_action = VR_new VR_VoiceTagAction_VrAgent(m_sp_EventSender, context, *protoMsg, spAvcSender);
    m_action->Run();
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
    SUCCEED();
}

TEST_F(VR_VoiceTagAction_VrAgent_Test, OnReceive_VoiceTagNotification_Min)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Register;
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());

    protoMsg->set_funccode(VrVoiceTagFunc_Register);

    protoMsg->clear_playresult();
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_Registered);
    protoMsg->mutable_registerresult()->add_voicetagid(0);

    m_vr_voiceTag_action->OnReceive(*protoMsg);
    SUCCEED();
}


TEST_F(VR_VoiceTagAction_VrAgent_Test, OnReceive_VoiceTagNotification_Max)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Register;
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());

    protoMsg->set_funccode(VrVoiceTagFunc_Register);

    protoMsg->clear_playresult();
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_Registered);
    protoMsg->mutable_registerresult()->add_voicetagid(0x9999);

    m_vr_voiceTag_action->OnReceive(*protoMsg);
    SUCCEED();
}



TEST_F(VR_VoiceTagAction_VrAgent_Test, OnReceive_VoiceTagNotification_Exceed)
{
    m_vr_voiceTag_action->m_nFuncCode = VrVoiceTagFunc_Register;
    std::unique_ptr<VrNotifyVoiceTagResult> protoMsg(VrNotifyVoiceTagResult::default_instance().New());

    protoMsg->set_funccode(VrVoiceTagFunc_Register);

    protoMsg->clear_playresult();
    protoMsg->mutable_registerresult()->set_result(VrNotifyRegisterResult_VrVoiceTagResultCode_Registered);
    protoMsg->mutable_registerresult()->add_voicetagid(0x10000);

    m_vr_voiceTag_action->OnReceive(*protoMsg);
    SUCCEED();
}
/* EOF */
