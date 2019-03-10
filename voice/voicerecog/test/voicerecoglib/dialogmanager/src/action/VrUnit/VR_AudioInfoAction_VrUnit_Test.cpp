/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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
#include "stdafx.h"
#include "VR_ReceiverCb.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_AudioInfoAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_AudioInfoAction_VrUnit_Test
*
* The class is just for VR_AudioInfoAction_VrUnit test.
*/
class VR_AudioInfoAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_AudioInfoAction_VrUnit *m_action;
};

void
VR_AudioInfoAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = USBDeviceInfo;
     std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
     ProtoMsg->set_request_id(USBDeviceInfo);
     ProtoMsg->mutable_usb_device_info_req()->set_request_id(1);
     ProtoMsg->mutable_usb_device_info_req()->set_source_type(1);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
     m_action = VR_new VR_AudioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
}

void
VR_AudioInfoAction_VrUnit_Test::TearDown()
{
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
}

TEST_F(VR_AudioInfoAction_VrUnit_Test, Run_USBDeviceInfo)
{
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrUnit_Test, Run_ClimateConciergeMode)
{
    int nEngineId = 101;
    int nActionType = ClimateConciergeMode;
    std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
    ProtoMsg->set_request_id(USBDeviceInfo);
    ProtoMsg->mutable_usb_device_info_req()->set_request_id(1);
    ProtoMsg->mutable_usb_device_info_req()->set_source_type(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_AudioInfoAction_VrUnit *m_action1 = VR_new VR_AudioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_AudioInfoAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp> protoMsg(VrAdditionalInfoRsp::default_instance().New());
    protoMsg->set_response_id(USBDeviceInfo);
    protoMsg->mutable_usb_device_info_resp()->set_response_id(1);
    protoMsg->mutable_usb_device_info_resp()->set_play_status(1);

    m_action->OnReceive(*protoMsg);
    SUCCEED();
}
/* EOF */


