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
#include "VR_Def.h"
#include "gtest/gtest.h"

#include "action/VrUnit/VR_HFDDoAction_VrUnit.h"
#include "action/VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "avclanobs/VR_AvcLanSender_VrUnitAgent.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HFDDoAction_VrUnit_Test
*
* The class is just for VR_HFDDoAction_VrUnit test.
*/
class VR_HFDDoAction_VrUnit_Test : public testing::Test
{
public:
    VR_HFDDoAction_VrUnit_Test()
    {
    }

    virtual ~VR_HFDDoAction_VrUnit_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

public:
    VR_HFDDoAction_VrUnit *m_vr_hfd_do_1_unit;
    VR_HFDDoAction_VrUnit *m_vr_hfd_do_2_unit;
    VR_HFDDoAction_VrUnit *m_vr_hfd_do_3_unit;
    VR_HFDDoAction_VrUnit *m_vr_hfd_do_4_unit;
    VR_HFDDoAction_VrUnit *m_vr_hfd_do_5_unit;
    VR_HFDDoAction_VrUnit *m_vr_hfd_do_6_unit;
    VR_HFDDoAction_VrUnit *m_CallWithNo_unit;
    VR_HFDDoAction_VrUnit *m_CallRecent_unit;
    VR_HFDDoAction_VrUnit *m_MUTEOFF_unit;
    VR_HFDDoAction_VrUnit *m_CallShortRing_unit;
    VR_HFDDoAction_VrUnit *m_Redial_unit;
    VR_HFDDoAction_VrUnit *m_SendFixMsg_unit;
    VR_HFDDoAction_VrUnit *m_PhoneListFav_unit;
    VR_HFDDoAction_VrUnit *m_ListAllContact_unit;

private:
    VR_HFDDoAction_VrUnit_Test(const VR_HFDDoAction_VrUnit_Test&);
    VR_HFDDoAction_VrUnit_Test& operator=(const VR_HFDDoAction_VrUnit_Test&);

};

void VR_HFDDoAction_VrUnit_Test::SetUp()
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
    m_vr_hfd_do_1_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 1, Call, *protoMsg1, m_sp_AvcLanSender);

    std::unique_ptr<VrMuteON> protoMsg2(VrMuteON::default_instance().New());
    m_vr_hfd_do_2_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 2, MUTEON, *protoMsg2, m_sp_AvcLanSender);

    std::unique_ptr<VrSendDTMF> protoMsg3(VrSendDTMF::default_instance().New());
    protoMsg3->set_tone_data_length(16);
    protoMsg3->set_tone_data("1234");
    m_vr_hfd_do_3_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 3, SendDTMF, *protoMsg3, m_sp_AvcLanSender);

    std::unique_ptr<VrSendMessage> protoMsg4(VrSendMessage::default_instance().New());
    protoMsg4->set_message_type(1);
    protoMsg4->set_address_length(16);
    protoMsg4->set_title_length(16);
    protoMsg4->set_message_body_length(16);
    protoMsg4->set_address("123456789");
    protoMsg4->set_title(" ");
    protoMsg4->set_message_body("I love you");
    m_vr_hfd_do_4_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 4, SendMessage, *protoMsg4, m_sp_AvcLanSender);

    std::unique_ptr<VrAddVoiceTag> protoMsg5(VrAddVoiceTag::default_instance().New());
    m_vr_hfd_do_5_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 5, AddVoiceTag, *protoMsg5, m_sp_AvcLanSender);

    std::unique_ptr<VrCallHistory> protoMsg6(VrCallHistory::default_instance().New());
    m_vr_hfd_do_6_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 6, CallHistory, *protoMsg6, m_sp_AvcLanSender);

    std::unique_ptr<VrPhoneCallWithNo> protoMsg7(VrPhoneCallWithNo::default_instance().New());
    protoMsg7->set_phone_number_length(16);
    protoMsg7->set_phone_kind(1);
    protoMsg7->set_phone_number("123456789");
    m_CallWithNo_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 7, CallWithNo, *protoMsg7, m_sp_AvcLanSender);

    std::unique_ptr<VrPhoneCallRecent> protoMsg8(VrPhoneCallRecent::default_instance().New());
    protoMsg8->set_name_length(16);
    protoMsg8->set_phone_number_length(16);
    protoMsg8->set_phone_kind(1);
    protoMsg8->set_name("james Devid");
    protoMsg8->set_phone_number("12131314152");
    m_CallRecent_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 8, CallRecent, *protoMsg8, m_sp_AvcLanSender);

    std::unique_ptr<VrMuteOFF> protoMsg9(VrMuteOFF::default_instance().New());
    m_MUTEOFF_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 9, MUTEOFF, *protoMsg9, m_sp_AvcLanSender);

    std::unique_ptr<VrPhoneCallShortRing> protoMsg10(VrPhoneCallShortRing::default_instance().New());
    protoMsg10->set_name_length(16);
    protoMsg10->set_phone_number_length(16);
    protoMsg10->set_phone_kind(1);
    protoMsg10->set_name("james Devid");
    protoMsg10->set_phone_number("12131314152");
    m_CallShortRing_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 10, CallShortRing, *protoMsg10, m_sp_AvcLanSender);

    std::unique_ptr<VrPhoneRedial> protoMsg11(VrPhoneRedial::default_instance().New());
    protoMsg11->set_name_length(16);
    protoMsg11->set_phone_number_length(16);
    protoMsg11->set_phone_kind(1);
    protoMsg11->set_name("james Devid");
    protoMsg11->set_phone_number("12131314152");
    m_Redial_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 11, Redial, *protoMsg11, m_sp_AvcLanSender);

    std::unique_ptr<VrSendFixedMessage> protoMsg12(VrSendFixedMessage::default_instance().New());
    protoMsg12->set_message_type(1);
    protoMsg12->set_address_length(16);
    protoMsg12->set_title_length(16);
    protoMsg12->set_message_body_length(16);
    protoMsg12->set_address("123456789");
    protoMsg12->set_title(" ");
    protoMsg12->set_message_body("I love you");
    m_SendFixMsg_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 12, SendFixedMessage, *protoMsg12, m_sp_AvcLanSender);

    std::unique_ptr<VrPhoneListFavourites> protoMsg13(VrPhoneListFavourites::default_instance().New());
    m_PhoneListFav_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 13, PhoneListFavourites, *protoMsg13, m_sp_AvcLanSender);

    std::unique_ptr<VrListAllContact> protoMsg14(VrListAllContact::default_instance().New());
    m_ListAllContact_unit = VR_new VR_HFDDoAction_VrUnit(m_sp_EventSender, 14, ListAllContact, *protoMsg14, m_sp_AvcLanSender);
}

void VR_HFDDoAction_VrUnit_Test::TearDown()
{
    if (NULL != m_vr_hfd_do_1_unit) {
        delete m_vr_hfd_do_1_unit;
        m_vr_hfd_do_1_unit = NULL;
    }

    if (NULL != m_vr_hfd_do_2_unit) {
        delete m_vr_hfd_do_2_unit;
        m_vr_hfd_do_2_unit = NULL;
    }

    if (NULL != m_vr_hfd_do_3_unit) {
        delete m_vr_hfd_do_3_unit;
        m_vr_hfd_do_3_unit = NULL;
    }

    if (NULL != m_vr_hfd_do_4_unit) {
        delete m_vr_hfd_do_4_unit;
        m_vr_hfd_do_4_unit = NULL;
    }

    if (NULL != m_vr_hfd_do_5_unit) {
        delete m_vr_hfd_do_5_unit;
        m_vr_hfd_do_5_unit = NULL;
    }

    if (NULL != m_vr_hfd_do_6_unit) {
        delete m_vr_hfd_do_6_unit;
        m_vr_hfd_do_6_unit = NULL;
    }

    if (NULL != m_CallWithNo_unit) {
        delete m_CallWithNo_unit;
        m_CallWithNo_unit = NULL;
    }

    if (NULL != m_CallRecent_unit) {
        delete m_CallRecent_unit;
        m_CallRecent_unit = NULL;
    }

    if (NULL != m_MUTEOFF_unit) {
        delete m_MUTEOFF_unit;
        m_MUTEOFF_unit = NULL;
    }

    if (NULL != m_CallShortRing_unit) {
        delete m_CallShortRing_unit;
        m_CallShortRing_unit = NULL;
    }

    if (NULL != m_Redial_unit) {
        delete m_Redial_unit;
        m_Redial_unit = NULL;
    }

    if (NULL != m_SendFixMsg_unit) {
        delete m_SendFixMsg_unit;
        m_SendFixMsg_unit = NULL;
    }

    if (NULL != m_PhoneListFav_unit) {
        delete m_PhoneListFav_unit;
        m_PhoneListFav_unit = NULL;
    }

    if (NULL != m_ListAllContact_unit) {
        delete m_ListAllContact_unit;
        m_ListAllContact_unit = NULL;
    }
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_call)
{
    m_vr_hfd_do_1_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_mute_on)
{
    m_vr_hfd_do_2_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_send_dtmf)
{
    m_vr_hfd_do_3_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_send_msg)
{
    m_vr_hfd_do_4_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_add_voicetag)
{
    m_vr_hfd_do_5_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_call_history)
{
    m_vr_hfd_do_6_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_call_WithNo)
{
    m_CallWithNo_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_call_Recent)
{
    m_CallRecent_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_MUTEOFF)
{
    m_MUTEOFF_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_call_ShortRing)
{
    m_CallShortRing_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_Redial)
{
    m_Redial_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_SendFixMsg)
{
    m_SendFixMsg_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_ListFav)
{
    m_PhoneListFav_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, Run_ListAllContact)
{
    m_ListAllContact_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDDoAction_VrUnit_Test, OnReceive)
{
    SUCCEED();
}

/* EOF */
