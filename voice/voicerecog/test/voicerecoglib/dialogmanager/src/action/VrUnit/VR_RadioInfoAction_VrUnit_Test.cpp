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
#include "VR_RadioInfoAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrTtsCommand.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_RadioInfoAction_VrUnit_Test
*
* The class is just for VR_RadioInfoAction_VrUnit test.
*/
class VR_RadioInfoAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_RadioInfoAction_VrUnit *m_action;
};

void
VR_RadioInfoAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = RadioBandState;
     std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
     ProtoMsg->set_request_id(RadioBandState);
     ProtoMsg->mutable_radio_band_state_req()->set_radio_band_kind(1);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
     m_action = VR_new VR_RadioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
}

void
VR_RadioInfoAction_VrUnit_Test::TearDown()
{
    if (NULL != m_action) {
        delete m_action;
        m_action = NULL;
    }
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, Run_RadioBandState)
{
    m_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, Run_CurFMChannelState)
{
    int nEngineId = 101;
    int nActionType = CurFMChannelState;
    std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
    ProtoMsg->set_request_id(CurFMChannelState);
    ProtoMsg->mutable_cur_fm_channel_state_req()->set_multicast_channel(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_RadioInfoAction_VrUnit *m_action1 = VR_new VR_RadioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, Run_SpecifiedFMState)
{
    int nEngineId = 101;
    int nActionType = SpecifiedFMState;
    std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
    ProtoMsg->set_request_id(SpecifiedFMState);
    ProtoMsg->mutable_specified_fm_state_req()->set_frequency(10170);
    ProtoMsg->mutable_specified_fm_state_req()->set_multicast_channel(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_RadioInfoAction_VrUnit *m_action1 = VR_new VR_RadioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, Run_PresetState)
{
    int nEngineId = 101;
    int nActionType = PresetState;
    std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
    ProtoMsg->set_request_id(PresetState);
    ProtoMsg->mutable_preset_state_req();
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_RadioInfoAction_VrUnit *m_action1 = VR_new VR_RadioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, Run_SpecifiedPresetState)
{
    int nEngineId = 101;
    int nActionType = SpecifiedPresetState;
    std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
    ProtoMsg->set_request_id(SpecifiedPresetState);
    ProtoMsg->mutable_specified_preset_state_req()->set_preset_num(1);
    ProtoMsg->mutable_specified_preset_state_req()->set_radio_band_kind(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_RadioInfoAction_VrUnit *m_action1 = VR_new VR_RadioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, Run_RadioState)
{
    int nEngineId = 101;
    int nActionType = RadioState;
    std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
    ProtoMsg->set_request_id(RadioState);
    ProtoMsg->mutable_radio_state_req();
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_RadioInfoAction_VrUnit *m_action1 = VR_new VR_RadioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, Run_GenreStationState)
{
    int nEngineId = 101;
    int nActionType = GenreStationState;
    std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
    ProtoMsg->set_request_id(GenreStationState);
    ProtoMsg->mutable_genre_station_state_req()->set_genre_name("genre");
    ProtoMsg->mutable_genre_station_state_req()->set_genre_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_RadioInfoAction_VrUnit *m_action1 = VR_new VR_RadioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, Run_default)
{
    int nEngineId = 101;
    int nActionType = AirConditionState;
    std::unique_ptr<VrAdditionalInfoReq> ProtoMsg(VrAdditionalInfoReq::default_instance().New());
    ProtoMsg->set_request_id(GenreStationState);
    ProtoMsg->mutable_genre_station_state_req()->set_genre_name("genre");
    ProtoMsg->mutable_genre_station_state_req()->set_genre_id(1);
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    spVR_AvcLanSenderIF   spAvcSender(VR_new VR_AvcLanSender_mock());
    VR_RadioInfoAction_VrUnit *m_action1 = VR_new VR_RadioInfoAction_VrUnit(actionSender, nEngineId, nActionType, *ProtoMsg, spAvcSender);
    m_action1->Run();
    if (NULL != m_action1) {
        delete m_action1;
        m_action1 = NULL;
    }
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp> protoMsg(VrAdditionalInfoRsp::default_instance().New());
    protoMsg->set_response_id(RadioBandState);
    protoMsg->mutable_radio_band_state_resp()->set_radio_band_state(0);

    m_action->OnReceive(*protoMsg);
    SUCCEED();
}
/* EOF */


