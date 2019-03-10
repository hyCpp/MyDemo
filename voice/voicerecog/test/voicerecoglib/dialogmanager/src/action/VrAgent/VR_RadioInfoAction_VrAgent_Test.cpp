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
#include "VR_RadioInfoAction_VrAgent.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_AvcLanSenderIF.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"

using namespace navi::VoiceRecog;
using namespace nutshell::media;

/**
* VR_RadioInfoAction_VrAgent_Test
*
* The class is just for VR_RadioInfoAction_VrAgent test.
*/
class VR_RadioInfoAction_VrAgent_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_RadioInfoAction_VrAgent *m_vr_radio_info_action;
};

void VR_RadioInfoAction_VrAgent_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioBandStateReq> protoMsg(VrRadioBandStateReq::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioBandState, *protoMsg, avclanSender);
}

void VR_RadioInfoAction_VrAgent_Test::TearDown()
{
    if (NULL != m_vr_radio_info_action) {
        delete m_vr_radio_info_action;
        m_vr_radio_info_action = NULL;
    }
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioBandState_AM)
{
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioBandState_FM)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioBandStateReq> protoMsg(VrRadioBandStateReq::default_instance().New());
    protoMsg->set_radio_band_kind(1);
    struct MediaInfo_CurSrc curSrc;
    curSrc.curSrc = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_CurSrc, &curSrc, sizeof(curSrc));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioBandState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioBandState_XM)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioBandStateReq> protoMsg(VrRadioBandStateReq::default_instance().New());
    protoMsg->set_radio_band_kind(2);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioBandState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioBandState_DAB)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioBandStateReq> protoMsg(VrRadioBandStateReq::default_instance().New());
    protoMsg->set_radio_band_kind(3);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioBandState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_CurFMChannelState)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrCurFMChannelStateReq> protoMsg(VrCurFMChannelStateReq::default_instance().New());
    protoMsg->set_multicast_channel(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, CurFMChannelState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_CurFMChannelState_true_available)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrCurFMChannelStateReq> protoMsg(VrCurFMChannelStateReq::default_instance().New());
    protoMsg->set_multicast_channel(1);
    MediaInfo_TunerPlayInfo tunerPlayInfo;
    memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));
    tunerPlayInfo.frequence = 10170;
    tunerPlayInfo.curChAvailable[0] = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, CurFMChannelState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_CurFMChannelState_true_notAvailable)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrCurFMChannelStateReq> protoMsg(VrCurFMChannelStateReq::default_instance().New());
    protoMsg->set_multicast_channel(1);
    MediaInfo_TunerPlayInfo tunerPlayInfo;
    memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));
    tunerPlayInfo.frequence = 10170;
    tunerPlayInfo.curChAvailable[1] = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, CurFMChannelState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_CurFMChannelState_channel0)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrCurFMChannelStateReq> protoMsg(VrCurFMChannelStateReq::default_instance().New());
    protoMsg->set_multicast_channel(0);
    MediaInfo_TunerPlayInfo tunerPlayInfo;
    memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));
    tunerPlayInfo.frequence = 10170;
    tunerPlayInfo.curChAvailable[1] = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, CurFMChannelState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedFMState)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedFMStateReq> protoMsg(VrSpecifiedFMStateReq::default_instance().New());
    protoMsg->set_frequency(10170);
    protoMsg->set_multicast_channel(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedFMState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedFMState_true)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedFMStateReq> protoMsg(VrSpecifiedFMStateReq::default_instance().New());
    protoMsg->set_frequency(10170);
    protoMsg->set_multicast_channel(1);
    MediaInfo_HDRadioStatus hdRadioStatus;
    memset(&hdRadioStatus, 0, sizeof(hdRadioStatus));
    hdRadioStatus.hdRadioStatus[0].channelAvailable[0] = 1;
    hdRadioStatus.hdRadioStatus[0].frequence = 10170;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_HDRadioStatus, &hdRadioStatus, sizeof(hdRadioStatus));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedFMState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedFMState_channel0)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedFMStateReq> protoMsg(VrSpecifiedFMStateReq::default_instance().New());
    protoMsg->set_frequency(10170);
    protoMsg->set_multicast_channel(0);
    MediaInfo_HDRadioStatus hdRadioStatus;
    memset(&hdRadioStatus, 0, sizeof(hdRadioStatus));
    hdRadioStatus.hdRadioStatus[0].channelAvailable[0] = 1;
    hdRadioStatus.hdRadioStatus[0].frequence = 10170;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_HDRadioStatus, &hdRadioStatus, sizeof(hdRadioStatus));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedFMState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedFMState_diffFre)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedFMStateReq> protoMsg(VrSpecifiedFMStateReq::default_instance().New());
    protoMsg->set_frequency(10170);
    protoMsg->set_multicast_channel(1);
    MediaInfo_HDRadioStatus hdRadioStatus;
    memset(&hdRadioStatus, 0, sizeof(hdRadioStatus));
    hdRadioStatus.hdRadioStatus[0].channelAvailable[0] = 1;
    hdRadioStatus.hdRadioStatus[0].frequence = 10180;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_HDRadioStatus, &hdRadioStatus, sizeof(hdRadioStatus));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedFMState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedFMState_unavailable)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedFMStateReq> protoMsg(VrSpecifiedFMStateReq::default_instance().New());
    protoMsg->set_frequency(10170);
    protoMsg->set_multicast_channel(1);
    MediaInfo_HDRadioStatus hdRadioStatus;
    memset(&hdRadioStatus, 0, sizeof(hdRadioStatus));
    hdRadioStatus.hdRadioStatus[0].frequence = 10170;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_HDRadioStatus, &hdRadioStatus, sizeof(hdRadioStatus));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedFMState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_PresetState)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPresetStateReq> protoMsg(VrPresetStateReq::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, PresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_PresetState_available)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPresetStateReq> protoMsg(VrPresetStateReq::default_instance().New());
    MediaInfo_MixPresetList mixPresetList;
    memset(&mixPresetList, 0, sizeof(mixPresetList));
    mixPresetList.presetListSize = 1;
    mixPresetList.mixPresetList[0].isBlank = 0;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_MixPresetList, &mixPresetList, sizeof(mixPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, PresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_PresetState_notAvailable)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrPresetStateReq> protoMsg(VrPresetStateReq::default_instance().New());
    MediaInfo_MixPresetList mixPresetList;
    memset(&mixPresetList, 0, sizeof(mixPresetList));
    mixPresetList.presetListSize = 1;
    mixPresetList.mixPresetList[0].isBlank = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_MixPresetList, &mixPresetList, sizeof(mixPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, PresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_AM)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(0);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_AM_available)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(0);
    MediaInfo_AmPresetList amPresetList;
    memset(&amPresetList, 0, sizeof(amPresetList));
    amPresetList.presetListSize = 1;
    amPresetList.amPresetList[0].band = 0;
    amPresetList.amPresetList[0].PChNO = 1;
    amPresetList.amPresetList[0].isBlank = 0;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_AmPresetList, &amPresetList, sizeof(amPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_AM_isBlank)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(0);
    MediaInfo_AmPresetList amPresetList;
    memset(&amPresetList, 0, sizeof(amPresetList));
    amPresetList.presetListSize = 1;
    amPresetList.amPresetList[0].band = 0;
    amPresetList.amPresetList[0].PChNO = 1;
    amPresetList.amPresetList[0].isBlank = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_AmPresetList, &amPresetList, sizeof(amPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_AM_diffNum)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(0);
    MediaInfo_AmPresetList amPresetList;
    memset(&amPresetList, 0, sizeof(amPresetList));
    amPresetList.presetListSize = 1;
    amPresetList.amPresetList[0].band = 0;
    amPresetList.amPresetList[0].PChNO = 2;
    amPresetList.amPresetList[0].isBlank = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_AmPresetList, &amPresetList, sizeof(amPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_FM)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_FM_available)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(1);
    MediaInfo_FmPresetList fmPresetList;
    memset(&fmPresetList, 0, sizeof(fmPresetList));
    fmPresetList.presetListSize = 1;
    fmPresetList.fmPresetList[0].band = 1;
    fmPresetList.fmPresetList[0].PChNO = 1;
    fmPresetList.fmPresetList[0].isBlank = 0;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_FmPresetList, &fmPresetList, sizeof(fmPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_FM_diffNum)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(1);
    MediaInfo_FmPresetList fmPresetList;
    memset(&fmPresetList, 0, sizeof(fmPresetList));
    fmPresetList.presetListSize = 1;
    fmPresetList.fmPresetList[0].band = 1;
    fmPresetList.fmPresetList[0].PChNO = 2;
    fmPresetList.fmPresetList[0].isBlank = 0;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_FmPresetList, &fmPresetList, sizeof(fmPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_FM_isBlank)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(1);
    MediaInfo_FmPresetList fmPresetList;
    memset(&fmPresetList, 0, sizeof(fmPresetList));
    fmPresetList.presetListSize = 1;
    fmPresetList.fmPresetList[0].band = 1;
    fmPresetList.fmPresetList[0].PChNO = 1;
    fmPresetList.fmPresetList[0].isBlank = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_FmPresetList, &fmPresetList, sizeof(fmPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_XM)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(2);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_DAB)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(3);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_DAB_available)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(3);
    bool presetState[12] = { false };
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_DabPresetList, &presetState, sizeof(presetState));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_DAB_num13)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(13);
    protoMsg->set_radio_band_kind(3);
    bool presetState[12] = { false };
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_DabPresetList, &presetState, sizeof(presetState));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_DAB_num0)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(0);
    protoMsg->set_radio_band_kind(3);
    bool presetState[12] = { false };
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_DabPresetList, &presetState, sizeof(presetState));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_default)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(0xff);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_default_available)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(0xff);
    MediaInfo_MixPresetList mixPresetList;
    memset(&mixPresetList, 0, sizeof(mixPresetList));
    mixPresetList.presetListSize = 1;
    mixPresetList.mixPresetList[0].PChNO = 1;
    mixPresetList.mixPresetList[0].isBlank = 0;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_MixPresetList, &mixPresetList, sizeof(mixPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_default_diffNum)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(0xff);
    MediaInfo_MixPresetList mixPresetList;
    memset(&mixPresetList, 0, sizeof(mixPresetList));
    mixPresetList.presetListSize = 1;
    mixPresetList.mixPresetList[0].PChNO = 2;
    mixPresetList.mixPresetList[0].isBlank = 0;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_MixPresetList, &mixPresetList, sizeof(mixPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_default_isBlank)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(0xff);
    MediaInfo_MixPresetList mixPresetList;
    memset(&mixPresetList, 0, sizeof(mixPresetList));
    mixPresetList.presetListSize = 1;
    mixPresetList.mixPresetList[0].PChNO = 1;
    mixPresetList.mixPresetList[0].isBlank = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_MixPresetList, &mixPresetList, sizeof(mixPresetList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState_NoBand)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_SpecifiedPresetState)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrSpecifiedPresetStateReq> protoMsg(VrSpecifiedPresetStateReq::default_instance().New());
    protoMsg->set_preset_num(1);
    protoMsg->set_radio_band_kind(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, SpecifiedPresetState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioState)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioStateReq> protoMsg(VrRadioStateReq::default_instance().New());
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioState_Radio)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioStateReq> protoMsg(VrRadioStateReq::default_instance().New());
    struct MediaInfo_CurSrc curSrc;
    memset(&curSrc, 0, sizeof(curSrc));
    curSrc.curSrc = MediaInfo_SrcID_Radio;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_CurSrc, &curSrc, sizeof(curSrc));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioState_AM)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioStateReq> protoMsg(VrRadioStateReq::default_instance().New());
    MediaInfo_LastRadioBand lastRadioBand;
    memset(&lastRadioBand, 0, sizeof(lastRadioBand));
    lastRadioBand.radioBand = MediaInfo_SrcID_AM;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_LastRadioBand, &lastRadioBand, sizeof(lastRadioBand));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioState_FM)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioStateReq> protoMsg(VrRadioStateReq::default_instance().New());
    MediaInfo_LastRadioBand lastRadioBand;
    memset(&lastRadioBand, 0, sizeof(lastRadioBand));
    lastRadioBand.radioBand = MediaInfo_SrcID_FM;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_LastRadioBand, &lastRadioBand, sizeof(lastRadioBand));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioState_XM)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioStateReq> protoMsg(VrRadioStateReq::default_instance().New());
    MediaInfo_LastRadioBand lastRadioBand;
    memset(&lastRadioBand, 0, sizeof(lastRadioBand));
    lastRadioBand.radioBand = MediaInfo_SrcID_SXM;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_LastRadioBand, &lastRadioBand, sizeof(lastRadioBand));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_RadioState_DAB)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioStateReq> protoMsg(VrRadioStateReq::default_instance().New());
    MediaInfo_LastRadioBand lastRadioBand;
    memset(&lastRadioBand, 0, sizeof(lastRadioBand));
    lastRadioBand.radioBand = MediaInfo_SrcID_DAB;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_LastRadioBand, &lastRadioBand, sizeof(lastRadioBand));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, RadioState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_GenreStationState)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrGenreStationStateReq> protoMsg(VrGenreStationStateReq::default_instance().New());
    protoMsg->set_genre_name("genre");
    protoMsg->set_genre_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, GenreStationState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_GenreStationState_available)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrGenreStationStateReq> protoMsg(VrGenreStationStateReq::default_instance().New());
    protoMsg->set_genre_name("genre");
    protoMsg->set_genre_id(1);
    MediaInfo_FMGenreList fmGenreList;
    memset(&fmGenreList, 0, sizeof(fmGenreList));
    fmGenreList.fmGenreList[0].genreID = 1;
    strcpy((char*)fmGenreList.fmGenreList[0].genreName, "genre");
    fmGenreList.fmGenreList[0].isAvailable = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_FMGenreList, &fmGenreList, sizeof(fmGenreList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, GenreStationState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_GenreStationState_NotEqual)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrGenreStationStateReq> protoMsg(VrGenreStationStateReq::default_instance().New());
    protoMsg->set_genre_name("genre");
    protoMsg->set_genre_id(1);
    MediaInfo_FMGenreList fmGenreList;
    memset(&fmGenreList, 0, sizeof(fmGenreList));
    fmGenreList.fmGenreList[0].genreID = 1;
    strcpy((char*)fmGenreList.fmGenreList[0].genreName, "");
    fmGenreList.fmGenreList[0].isAvailable = 1;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_FMGenreList, &fmGenreList, sizeof(fmGenreList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, GenreStationState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_GenreStationState_NotAvailable)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrGenreStationStateReq> protoMsg(VrGenreStationStateReq::default_instance().New());
    protoMsg->set_genre_name("genre");
    protoMsg->set_genre_id(1);
    MediaInfo_FMGenreList fmGenreList;
    memset(&fmGenreList, 0, sizeof(fmGenreList));
    fmGenreList.fmGenreList[0].genreID = 1;
    strcpy((char*)fmGenreList.fmGenreList[0].genreName, "genre");
    fmGenreList.fmGenreList[0].isAvailable = 0;
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_FMGenreList, &fmGenreList, sizeof(fmGenreList));
    VR_LOGD("[bret]=[%d]", bret);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, GenreStationState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, Run_default)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrGenreStationStateReq> protoMsg(VrGenreStationStateReq::default_instance().New());
    protoMsg->set_genre_name("genre");
    protoMsg->set_genre_id(1);
    spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock);
    m_vr_radio_info_action = VR_new VR_RadioInfoAction_VrAgent(actionSender, 1, ACState, *protoMsg, avclanSender);
    m_vr_radio_info_action->Run();
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(RadioBandState);
    protoMsg->set_processing_result(true);

    m_vr_radio_info_action->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, ChangeProviderCurBand_Case)
{
    int providerBand = MediaInfo_SrcID_Radio;
    int providerSubBand = VR_RadioInfoAction_VrAgent::VR_RadioBand_AM;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_AM, m_vr_radio_info_action->ChangeProviderCurBand(providerBand, providerSubBand));
    providerSubBand = VR_RadioInfoAction_VrAgent::VR_RadioBand_FM;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_FM, m_vr_radio_info_action->ChangeProviderCurBand(providerBand, providerSubBand));
    providerSubBand = VR_RadioInfoAction_VrAgent::VR_RadioBand_XM;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_Undef, m_vr_radio_info_action->ChangeProviderCurBand(providerBand, providerSubBand));
    providerBand = MediaInfo_SrcID_SXM;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_XM, m_vr_radio_info_action->ChangeProviderCurBand(providerBand, providerSubBand));
    providerBand = MediaInfo_SrcID_DAB;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_DAB, m_vr_radio_info_action->ChangeProviderCurBand(providerBand, providerSubBand));
    providerBand = MediaInfo_SrcID_AUX;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_Undef, m_vr_radio_info_action->ChangeProviderCurBand(providerBand, providerSubBand));
    SUCCEED();
}

TEST_F(VR_RadioInfoAction_VrAgent_Test, ChangeProviderLastBand_Case)
{
    int providerBand = MediaInfo_SrcID_AM;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_AM, m_vr_radio_info_action->ChangeProviderLastBand(providerBand));
    providerBand = MediaInfo_SrcID_FM;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_FM, m_vr_radio_info_action->ChangeProviderLastBand(providerBand));
    providerBand = MediaInfo_SrcID_SXM;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_XM, m_vr_radio_info_action->ChangeProviderLastBand(providerBand));
    providerBand = MediaInfo_SrcID_DAB;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_DAB, m_vr_radio_info_action->ChangeProviderLastBand(providerBand));
    providerBand = MediaInfo_SrcID_Invalid;
    EXPECT_EQ(VR_RadioInfoAction_VrAgent::VR_RadioBand_Undef, m_vr_radio_info_action->ChangeProviderLastBand(providerBand));
    SUCCEED();
}

/* EOF */
