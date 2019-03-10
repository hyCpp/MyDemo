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
#include "action/VR_HFDGetInfoAction.h"
#include "VR_PhoneBookQueryListener.h"
#include "action/VR_ActionEventSender.h"
#include "VR_ActionBase_mock.h"
#include "VR_DialogEngine_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::dataprovider;
using ::testing::Return;
using ::testing::_;

/**
* VR_HFDGetInfoAction_Test
*
* The class is just for VR_HFDGetInfoAction test.
*/
class VR_HFDGetInfoAction_Test : public testing::Test
{
public:
    VR_HFDGetInfoAction_Test()
    {

    }

    virtual ~VR_HFDGetInfoAction_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

public:
    VR_HFDGetInfoAction *m_vr_hfd_info_1_unit;
    VR_HFDGetInfoAction *m_vr_hfd_info_2_unit;
    VR_HFDGetInfoAction *m_vr_hfd_info_3_unit;
    VR_HFDGetInfoAction *m_vr_hfd_info_4_unit;
    VR_HFDGetInfoAction *m_vr_hfd_info_5_unit;
    VR_HFDGetInfoAction *m_vr_hfd_info_6_unit;
    VR_HFDGetInfoAction *m_vr_hfd_info_7_unit;

private:
    VR_HFDGetInfoAction_Test(const VR_HFDGetInfoAction_Test&);
    VR_HFDGetInfoAction_Test& operator=(const VR_HFDGetInfoAction_Test&);

};

void VR_HFDGetInfoAction_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));

    std::unique_ptr<VrRecentIncomingCallReq> protoMsg1(VrRecentIncomingCallReq::default_instance().New());
    m_vr_hfd_info_1_unit = VR_new VR_HFDGetInfoAction(m_sp_EventSender, 1, RecentIncomingCall, *protoMsg1);

    std::unique_ptr<VrRecentOutgoingCallReq> protoMsg2(VrRecentOutgoingCallReq::default_instance().New());
    m_vr_hfd_info_2_unit = VR_new VR_HFDGetInfoAction(m_sp_EventSender, 2, RecentOutgoingCall, *protoMsg2);

    std::unique_ptr<VrPhoneVehicleInMontion> protoMsg3(VrPhoneVehicleInMontion::default_instance().New());
    m_vr_hfd_info_3_unit = VR_new VR_HFDGetInfoAction(m_sp_EventSender, 3, PhoneVehicleInMontion, *protoMsg3);

    std::unique_ptr<VrValidTemperature> protoMsg4(VrValidTemperature::default_instance().New());
    protoMsg4->set_type(ValidTemperatureType_Increase);
    m_vr_hfd_info_4_unit = VR_new VR_HFDGetInfoAction(m_sp_EventSender, 4, ValidTemperature, *protoMsg4);

    std::unique_ptr<VrRecentCallListReq> protoMsg5(VrRecentCallListReq::default_instance().New());
    protoMsg5->set_count(25);
    m_vr_hfd_info_5_unit = VR_new VR_HFDGetInfoAction(m_sp_EventSender, 5, RecentCallList, *protoMsg5);

    std::unique_ptr<VrRecentMessageListReq> protoMsg6(VrRecentMessageListReq::default_instance().New());
    protoMsg6->set_count(25);
    m_vr_hfd_info_6_unit = VR_new VR_HFDGetInfoAction(m_sp_EventSender, 6, RecentMessageList, *protoMsg6);

    std::unique_ptr<VrRecentMessageListByTypeReq> protoMsg7(VrRecentMessageListByTypeReq::default_instance().New());
    protoMsg7->set_type(1);
    protoMsg7->set_count(25);
    m_vr_hfd_info_7_unit = VR_new VR_HFDGetInfoAction(m_sp_EventSender, 7, RecentMessageListByType, *protoMsg7);
}

void VR_HFDGetInfoAction_Test::TearDown()
{
    if (NULL != m_vr_hfd_info_1_unit) {
        delete m_vr_hfd_info_1_unit;
        m_vr_hfd_info_1_unit = NULL;
    }

    if (NULL != m_vr_hfd_info_2_unit) {
        delete m_vr_hfd_info_2_unit;
        m_vr_hfd_info_2_unit = NULL;
    }

    if (NULL != m_vr_hfd_info_3_unit) {
        delete m_vr_hfd_info_3_unit;
        m_vr_hfd_info_3_unit = NULL;
    }

    if (NULL != m_vr_hfd_info_4_unit) {
        delete m_vr_hfd_info_4_unit;
        m_vr_hfd_info_4_unit = NULL;
    }

    if (NULL != m_vr_hfd_info_5_unit) {
        delete m_vr_hfd_info_5_unit;
        m_vr_hfd_info_5_unit = NULL;
    }

    if (NULL != m_vr_hfd_info_6_unit) {
        delete m_vr_hfd_info_6_unit;
        m_vr_hfd_info_6_unit = NULL;
    }

    if (NULL != m_vr_hfd_info_7_unit) {
        delete m_vr_hfd_info_7_unit;
        m_vr_hfd_info_7_unit = NULL;
    }
}

TEST_F(VR_HFDGetInfoAction_Test, Run_RecentCall)
{
    m_vr_hfd_info_5_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_Test, Run_incomcall)
{
    m_vr_hfd_info_1_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_Test, Run_outgocall)
{
    m_vr_hfd_info_2_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_Test, Run_recentmsg)
{
    m_vr_hfd_info_6_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_Test, Run_recentmsgbytype)
{
    m_vr_hfd_info_7_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_Test, Run_VehicleInMontion)
{
    m_vr_hfd_info_3_unit->Run();
    SUCCEED();
}

TEST_F(VR_HFDGetInfoAction_Test, Run_Null)
{
    m_vr_hfd_info_4_unit->Run();
    SUCCEED();
}

// OnReceive Test
TEST_F(VR_HFDGetInfoAction_Test, OnReceive)
{
    std::unique_ptr<VrPhoneVehicleInMontion> protoMsg3(VrPhoneVehicleInMontion::default_instance().New());
    m_vr_hfd_info_3_unit->OnReceive(*protoMsg3);
    SUCCEED();
}

/* EOF */
