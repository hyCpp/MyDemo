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
#include "action/VrAgent/VR_HFDGetMsgBodyAction_VrAgent.h"
#include "action/VR_ActionEventSender.h"
#include "VR_AvcLanSenderIF.h"
#include "navi/Voice/VoiceRecog/Internal/VrMessageTrans.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HFDGetMsgBodyAction_VrAgent_Test
*
* The class is just for VR_HFDGetMsgBodyAction_VrAgent test.
*/
class VR_HFDGetMsgBodyAction_VrAgent_Test : public testing::Test
{
public:
    VR_HFDGetMsgBodyAction_VrAgent_Test()
        : m_vr_hfd_msg_body_unit(NULL)
    {
    }

    virtual ~VR_HFDGetMsgBodyAction_VrAgent_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

public:
    VR_HFDGetMsgBodyAction_VrAgent *m_vr_hfd_msg_body_unit;

private:
    VR_HFDGetMsgBodyAction_VrAgent_Test(const VR_HFDGetMsgBodyAction_VrAgent_Test&);
    VR_HFDGetMsgBodyAction_VrAgent_Test& operator=(const VR_HFDGetMsgBodyAction_VrAgent_Test&);

};

void VR_HFDGetMsgBodyAction_VrAgent_Test::SetUp()
{
    VrMessageAcquisitionReq m_msgBodyReq;
    spVR_ActionEventSender m_sp_EventSender;
    spVR_AvcLanSenderIF m_sp_AvcLanSender;
    m_vr_hfd_msg_body_unit = VR_new VR_HFDGetMsgBodyAction_VrAgent(m_sp_EventSender,
                                                                  1,
                                                                  26,
                                                                  m_msgBodyReq,
                                                                  m_sp_AvcLanSender);
}

void VR_HFDGetMsgBodyAction_VrAgent_Test::TearDown()
{
    if (NULL != m_vr_hfd_msg_body_unit) {
        delete m_vr_hfd_msg_body_unit;
        m_vr_hfd_msg_body_unit = NULL;
    }
}

TEST_F(VR_HFDGetMsgBodyAction_VrAgent_Test, Test_Run)
{
    m_vr_hfd_msg_body_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetMsgBodyAction_VrAgent_Test, Test_RunAction)
{
    VrMessageAcquisitionReq msgBodyReq;

    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(1);
    notifyAction->set_actionid(26);

    std::string resultXml =
            "<action agent=\"phone\" op=\"getMessageDetail\">"\
                "<instanceId>0</instanceId>"\
                "<messageId>201500000E121</messageId>"\
            "</action>";

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_vr_hfd_msg_body_unit->RunAction(msgBodyReq);
    SUCCEED();
}

TEST_F(VR_HFDGetMsgBodyAction_VrAgent_Test, Test_OnReceive)
{
    SUCCEED();
}

/* EOF */
