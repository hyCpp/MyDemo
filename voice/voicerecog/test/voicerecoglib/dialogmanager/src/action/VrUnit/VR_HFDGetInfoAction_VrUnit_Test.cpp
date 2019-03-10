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

#include "VR_AvcLanSenderIF.h"
#include "action/VrUnit/VR_HFDGetInfoAction_VrUnit.h"
#include "VR_PhoneBookQueryListener_VrUnit.h"
#include "action/VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::dataprovider;

/**
* VR_HFDGetInfoAction_VrUnit_Test
*
* The class is just for VR_HFDGetInfoAction_VrUnit test.
*/
class VR_HFDGetInfoAction_VrUnit_Test : public testing::Test
{
public:
    VR_HFDGetInfoAction_VrUnit_Test()
    {
    }

    virtual ~VR_HFDGetInfoAction_VrUnit_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

public:
    VR_HFDGetInfoAction_VrUnit *m_vr_hfd_1_unit;
    VR_HFDGetInfoAction_VrUnit *m_vr_hfd_2_unit;
    VR_HFDGetInfoAction_VrUnit *m_vr_hfd_3_unit;
    VR_HFDGetInfoAction_VrUnit *m_vr_hfd_4_unit;
    VR_HFDGetInfoAction_VrUnit *m_vr_hfd_5_unit;

private:
    VR_HFDGetInfoAction_VrUnit_Test(const VR_HFDGetInfoAction_VrUnit_Test&);
    VR_HFDGetInfoAction_VrUnit_Test& operator=(const VR_HFDGetInfoAction_VrUnit_Test&);
};

void VR_HFDGetInfoAction_VrUnit_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));

    std::unique_ptr<VrRecentCallListReq> protoMsg1(VrRecentCallListReq::default_instance().New());
    protoMsg1->set_count(25);
    m_vr_hfd_1_unit = VR_new VR_HFDGetInfoAction_VrUnit(m_sp_EventSender, 1, RecentCallList, *protoMsg1);

    std::unique_ptr<VrRecentIncomingCallReq> protoMsg2(VrRecentIncomingCallReq::default_instance().New());
    m_vr_hfd_2_unit = VR_new VR_HFDGetInfoAction_VrUnit(m_sp_EventSender, 2, RecentIncomingCall, *protoMsg2);

    std::unique_ptr<VrRecentOutgoingCallReq> protoMsg3(VrRecentOutgoingCallReq::default_instance().New());
    m_vr_hfd_3_unit = VR_new VR_HFDGetInfoAction_VrUnit(m_sp_EventSender, 3, RecentOutgoingCall, *protoMsg3);

    std::unique_ptr<VrRecentMessageListReq> protoMsg4(VrRecentMessageListReq::default_instance().New());
    protoMsg4->set_count(25);
    m_vr_hfd_4_unit = VR_new VR_HFDGetInfoAction_VrUnit(m_sp_EventSender, 4, RecentMessageList, *protoMsg4);

    std::unique_ptr<VrRecentMessageListByTypeReq> protoMsg5(VrRecentMessageListByTypeReq::default_instance().New());
    protoMsg5->set_count(25);
    protoMsg5->set_type(1);
    m_vr_hfd_5_unit = VR_new VR_HFDGetInfoAction_VrUnit(m_sp_EventSender, 5, RecentMessageListByType, *protoMsg5);

}

void VR_HFDGetInfoAction_VrUnit_Test::TearDown()
{
    if (NULL != m_vr_hfd_1_unit) {
        delete m_vr_hfd_1_unit;
        m_vr_hfd_1_unit = NULL;
    }

    if (NULL != m_vr_hfd_2_unit) {
        delete m_vr_hfd_2_unit;
        m_vr_hfd_2_unit = NULL;
    }

    if (NULL != m_vr_hfd_3_unit) {
        delete m_vr_hfd_3_unit;
        m_vr_hfd_3_unit = NULL;
    }

    if (NULL != m_vr_hfd_4_unit) {
        delete m_vr_hfd_4_unit;
        m_vr_hfd_4_unit = NULL;
    }

    if (NULL != m_vr_hfd_5_unit) {
        delete m_vr_hfd_5_unit;
        m_vr_hfd_5_unit = NULL;
    }
}

TEST_F(VR_HFDGetInfoAction_VrUnit_Test, Run_calllist)
{
    m_vr_hfd_1_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_VrUnit_Test, Run_incomcall)
{
    m_vr_hfd_2_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_VrUnit_Test, Run_outgocall)
{
    m_vr_hfd_3_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_VrUnit_Test, Run_msglist)
{
    m_vr_hfd_4_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_VrUnit_Test, Run_typemsglist)
{
    m_vr_hfd_5_unit->Run();
    SUCCEED();
}

// OnReceive Test
TEST_F(VR_HFDGetInfoAction_VrUnit_Test, OnReceive)
{
    std::unique_ptr<VrRecentMessageListByTypeReq> protoMsg(VrRecentMessageListByTypeReq::default_instance().New());
    protoMsg->set_count(25);
    protoMsg->set_type(1);
    m_vr_hfd_5_unit->OnReceive(*protoMsg);
    SUCCEED();
}

/* EOF */
