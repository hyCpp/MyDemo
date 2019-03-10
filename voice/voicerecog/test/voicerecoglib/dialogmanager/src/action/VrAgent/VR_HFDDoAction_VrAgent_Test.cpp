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

#include <string>
#include "gtest/gtest.h"

#include "action/VrAgent/VR_HFDDoAction_VrAgent.h"
#include "action/VR_ActionEventSender.h"
#include "VR_ActionSenderDef.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HFDDoAction_VrAgent_Test
*
* The class is just for VR_HFDDoAction_VrAgent test.
*/
class VR_HFDDoAction_VrAgent_Test : public testing::Test
{
public:
    VR_HFDDoAction_VrAgent_Test();
    virtual ~VR_HFDDoAction_VrAgent_Test();

    virtual void SetUp();
    virtual void TearDown();

private:
    VR_HFDDoAction_VrAgent *m_vr_hfd_do_1_agent;
    VR_HFDDoAction_VrAgent *m_vr_hfd_do_2_agent;
    VR_HFDDoAction_VrAgent *m_vr_hfd_do_3_agent;
    VR_HFDDoAction_VrAgent *m_vr_hfd_do_4_agent;
    VR_HFDDoAction_VrAgent *m_vr_hfd_do_5_agent;
    VR_HFDDoAction_VrAgent *m_vr_hfd_do_6_agent;

private:
    VR_HFDDoAction_VrAgent_Test(const VR_HFDDoAction_VrAgent_Test&);
    VR_HFDDoAction_VrAgent_Test& operator=(const VR_HFDDoAction_VrAgent_Test&);

};

void VR_HFDDoAction_VrAgent_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF  m_sp_AvcLanSender(VR_new VR_AvcLanSender_mock);

    std::unique_ptr<VrPhoneCall> protoMsg1(VrPhoneCall::default_instance().New());
    protoMsg1->set_name_length(16);
    protoMsg1->set_phone_number_length(16);
    protoMsg1->set_phone_kind(1);
    protoMsg1->set_name("james Devid");
    protoMsg1->set_phone_number("12131314152");
    m_vr_hfd_do_1_agent = VR_new VR_HFDDoAction_VrAgent(m_sp_EventSender, 1, Call, *protoMsg1, m_sp_AvcLanSender);

    std::unique_ptr<VrMuteON> protoMsg2(VrMuteON::default_instance().New());
    m_vr_hfd_do_2_agent = VR_new VR_HFDDoAction_VrAgent(m_sp_EventSender, 2, MUTEON, *protoMsg2, m_sp_AvcLanSender);

    std::unique_ptr<VrSendDTMF> protoMsg3(VrSendDTMF::default_instance().New());
    protoMsg3->set_tone_data_length(16);
    protoMsg3->set_tone_data("1234");
    m_vr_hfd_do_3_agent = VR_new VR_HFDDoAction_VrAgent(m_sp_EventSender, 3, SendDTMF, *protoMsg3, m_sp_AvcLanSender);

    std::unique_ptr<VrSendMessage> protoMsg4(VrSendMessage::default_instance().New());
    protoMsg4->set_message_type(1);
    protoMsg4->set_address_length(16);
    protoMsg4->set_title_length(16);
    protoMsg4->set_message_body_length(16);
    protoMsg4->set_address("123456789");
    protoMsg4->set_title(" ");
    protoMsg4->set_message_body("I love you");
    m_vr_hfd_do_4_agent = VR_new VR_HFDDoAction_VrAgent(m_sp_EventSender, 4, SendMessage, *protoMsg4, m_sp_AvcLanSender);

    std::unique_ptr<VrAddVoiceTag> protoMsg5(VrAddVoiceTag::default_instance().New());
    m_vr_hfd_do_5_agent = VR_new VR_HFDDoAction_VrAgent(m_sp_EventSender, 5, AddVoiceTag, *protoMsg5, m_sp_AvcLanSender);

    std::unique_ptr<VrCallHistory> protoMsg6(VrCallHistory::default_instance().New());
    m_vr_hfd_do_6_agent = VR_new VR_HFDDoAction_VrAgent(m_sp_EventSender, 6, CallHistory, *protoMsg6, m_sp_AvcLanSender);
}

VR_HFDDoAction_VrAgent_Test::VR_HFDDoAction_VrAgent_Test()
{

}

VR_HFDDoAction_VrAgent_Test::~VR_HFDDoAction_VrAgent_Test()
{

}

void VR_HFDDoAction_VrAgent_Test::TearDown()
{
    if (NULL != m_vr_hfd_do_1_agent) {
        delete m_vr_hfd_do_1_agent;
        m_vr_hfd_do_1_agent = NULL;
    }

    if (NULL != m_vr_hfd_do_2_agent) {
        delete m_vr_hfd_do_2_agent;
        m_vr_hfd_do_2_agent = NULL;
    }

    if (NULL != m_vr_hfd_do_3_agent) {
        delete m_vr_hfd_do_3_agent;
        m_vr_hfd_do_3_agent = NULL;
    }

    if (NULL != m_vr_hfd_do_4_agent) {
        delete m_vr_hfd_do_4_agent;
        m_vr_hfd_do_4_agent = NULL;
    }

    if (NULL != m_vr_hfd_do_5_agent) {
        delete m_vr_hfd_do_5_agent;
        m_vr_hfd_do_5_agent = NULL;
    }

    if (NULL != m_vr_hfd_do_6_agent) {
        delete m_vr_hfd_do_6_agent;
        m_vr_hfd_do_6_agent = NULL;
    }
}

// call
TEST_F(VR_HFDDoAction_VrAgent_Test, Run_call)
{
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(1);
    notifyAction->set_actionid(Call);

    VR_String resultXml =
            "<action agent=\"phone\" op=\"dial\">"\
                "<dialType>CALL</dialType>"\
                "<phoneInfo>"\
                    "<contactName>xxxx</contactName>"\
                    "<number>1234564789</number>"\
                    "<phoneType>1</phoneType>"\
                    "<contactId>0</contactId>"\
                    "<listType>0</listType>"\
                "</phoneInfo>"\
            "</action>";

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_vr_hfd_do_1_agent->Run();
    SUCCEED();
}

// mute on
TEST_F(VR_HFDDoAction_VrAgent_Test, Run_mute_on)
{
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(2);
    notifyAction->set_actionid(MUTEON);

    VR_String resultXml =
            "<action agent=\"phone\" op=\"muteCall\">"\
            "</action>";

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_vr_hfd_do_2_agent->Run();
    SUCCEED();
}

// send dtmf
TEST_F(VR_HFDDoAction_VrAgent_Test, Run_send_dtmf)
{
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(3);
    notifyAction->set_actionid(SendDTMF);

    VR_String resultXml =
            "<action agent=\"phone\" op=\"SendDTMFTones\">"\
                "<tones>1234</tones>"\
            "</action>";

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_vr_hfd_do_3_agent->Run();
    SUCCEED();
}

// send message
TEST_F(VR_HFDDoAction_VrAgent_Test, Run_send_msg)
{
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(4);
    notifyAction->set_actionid(SendMessage);

    VR_String resultXml =
            "<action agent=\"phone\" op=\"sendMessage\">"\
                "<messageInfo>"\
                    "<messageBody>xx</messageBody>"\
                    "<number>123456</number>"\
                    "<instanceId>0</instanceId>"\
                    "<messageType>0</messageType>"\
                    "<name>xxxx</name>"\
                "</messageInfo>"\
            "</action>";

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_vr_hfd_do_4_agent->Run();
    SUCCEED();
}

// add voice tag
TEST_F(VR_HFDDoAction_VrAgent_Test, Run_add_voicetag)
{
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(5);
    notifyAction->set_actionid(AddVoiceTag);

    VR_String resultXml =
            "<action agent=\"phone\" op=\"showAddVoiceTagUI\">"\
            "</action>";

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_vr_hfd_do_5_agent->Run();
    SUCCEED();
}

// call history
TEST_F(VR_HFDDoAction_VrAgent_Test, Run_call_history)
{
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(6);
    notifyAction->set_actionid(CallHistory);

    VR_String resultXml =
            "<action agent=\"phone\" op=\"showRecentCallList\">"\
            "</action>";

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_vr_hfd_do_6_agent->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrAgent_Test, OnReceive)
{
    SUCCEED();
}

/* EOF */
