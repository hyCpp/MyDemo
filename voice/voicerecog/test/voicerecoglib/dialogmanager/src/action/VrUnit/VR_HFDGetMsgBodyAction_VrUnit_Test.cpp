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
#include "action/VrUnit/VR_HFDGetMsgBodyAction_VrUnit.h"
#include "action/VR_ActionEventSender.h"
#include "VR_AvcLanSenderIF.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrMessageTrans.pb.h"
using namespace navi::VoiceRecog;

/**
* VR_HFDGetMsgBodyAction_VrUnit_Test
*
* The class is just for VR_HFDGetMsgBodyAction_VrUnit test.
*/
class VR_HFDGetMsgBodyAction_VrUnit_Test : public testing::Test
{
public:
    VR_HFDGetMsgBodyAction_VrUnit_Test()
        : m_vr_hfd_msg_body_unit(NULL)
    {
    }

    virtual ~VR_HFDGetMsgBodyAction_VrUnit_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

public:
    VR_HFDGetMsgBodyAction_VrUnit *m_vr_hfd_msg_body_unit;
    VR_HFDGetMsgBodyAction_VrUnit *m_vr_hfd_msg_body_unit1;

private:
    VR_HFDGetMsgBodyAction_VrUnit_Test(const VR_HFDGetMsgBodyAction_VrUnit_Test&);
    VR_HFDGetMsgBodyAction_VrUnit_Test& operator=(const VR_HFDGetMsgBodyAction_VrUnit_Test&);
};

void VR_HFDGetMsgBodyAction_VrUnit_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF  m_sp_AvcLanSender(VR_new VR_AvcLanSender_mock);

    std::unique_ptr<VrMessageAcquisitionReq> protoMsg(VrMessageAcquisitionReq::default_instance().New());
    protoMsg->set_message_no(0);
    protoMsg->set_instance_id(0);
    protoMsg->set_message_id("2015T1125E12");
    protoMsg->set_service_id(1);
    m_vr_hfd_msg_body_unit = VR_new VR_HFDGetMsgBodyAction_VrUnit(m_sp_EventSender, 1, 26, *protoMsg, m_sp_AvcLanSender);
    m_vr_hfd_msg_body_unit1 = VR_new VR_HFDGetMsgBodyAction_VrUnit(m_sp_EventSender, 2, 27, *protoMsg, m_sp_AvcLanSender);
}

void VR_HFDGetMsgBodyAction_VrUnit_Test::TearDown()
{
    if (NULL != m_vr_hfd_msg_body_unit) {
        delete m_vr_hfd_msg_body_unit;
        m_vr_hfd_msg_body_unit = NULL;
    }

    if (NULL != m_vr_hfd_msg_body_unit1) {
        delete m_vr_hfd_msg_body_unit1;
        m_vr_hfd_msg_body_unit1 = NULL;
    }
}

TEST_F(VR_HFDGetMsgBodyAction_VrUnit_Test, Test_Run)
{
    m_vr_hfd_msg_body_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetMsgBodyAction_VrUnit_Test, Test_OnReceive)
{
    std::unique_ptr<VrMessageAcquisitionResp> proto(VrMessageAcquisitionResp::default_instance().New());
    proto->set_op_result(true);
    proto->set_service_id(0);
    proto->set_title_character_set(UTF8);
    proto->set_body_character_set(UTF8);
    proto->set_len_addr(16);
    proto->set_address("xxxxxx");
    proto->set_len_title(16);
    proto->set_title("xxxx13ss");
    proto->set_len_body(72);
    proto->set_body("123456sdfsdfdgd");

    m_vr_hfd_msg_body_unit1->OnReceive(*proto);
    SUCCEED();
}
/* EOF */
