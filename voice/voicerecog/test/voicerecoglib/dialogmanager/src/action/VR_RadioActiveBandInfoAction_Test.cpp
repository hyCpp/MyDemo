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
#include "VR_RadioActiveBandInfoAction.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"

using namespace navi::VoiceRecog;
using namespace nutshell::media;

/**
* VR_RadioActiveBandInfoAction_Test
*
* The class is just for VR_RadioActiveBandInfoAction test.
*/
class VR_RadioActiveBandInfoAction_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_RadioActiveBandInfoAction *m_activeRadioINfoAction;
};

void VR_RadioActiveBandInfoAction_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioActiveBandReq> protoMsg(VrRadioActiveBandReq::default_instance().New());
    m_activeRadioINfoAction = VR_new VR_RadioActiveBandInfoAction(actionSender, 1, RadioActiveBand, *protoMsg);
}

void VR_RadioActiveBandInfoAction_Test::TearDown()
{
    if (NULL != m_activeRadioINfoAction) {
        delete m_activeRadioINfoAction;
        m_activeRadioINfoAction = NULL;
    }
}

TEST_F(VR_RadioActiveBandInfoAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(ChangeHDSubChannel);
    protoMsg->set_processing_result(true);

    m_activeRadioINfoAction->OnReceive(*protoMsg);
    SUCCEED();
}

TEST_F(VR_RadioActiveBandInfoAction_Test, ChangeProviderBand_Case)
{
    int providerBand = MediaInfo_SrcID_Radio;
    int providerSubBand = VR_RadioActiveBandInfoAction::VR_RadioBand_AM;
    EXPECT_EQ(VR_RadioActiveBandInfoAction::VR_RadioBand_AM, m_activeRadioINfoAction->ChangeProviderBand(providerBand, providerSubBand));
    providerSubBand = VR_RadioActiveBandInfoAction::VR_RadioBand_FM;
    EXPECT_EQ(VR_RadioActiveBandInfoAction::VR_RadioBand_FM, m_activeRadioINfoAction->ChangeProviderBand(providerBand, providerSubBand));
    providerSubBand = VR_RadioActiveBandInfoAction::VR_RadioBand_XM;
    EXPECT_EQ(VR_RadioActiveBandInfoAction::VR_RadioBand_Undef, m_activeRadioINfoAction->ChangeProviderBand(providerBand, providerSubBand));
    providerBand = MediaInfo_SrcID_SXM;
    EXPECT_EQ(VR_RadioActiveBandInfoAction::VR_RadioBand_XM, m_activeRadioINfoAction->ChangeProviderBand(providerBand, providerSubBand));
    providerBand = MediaInfo_SrcID_DAB;
    EXPECT_EQ(VR_RadioActiveBandInfoAction::VR_RadioBand_DAB, m_activeRadioINfoAction->ChangeProviderBand(providerBand, providerSubBand));
    providerBand = MediaInfo_SrcID_AUX;
    EXPECT_EQ(VR_RadioActiveBandInfoAction::VR_RadioBand_Undef, m_activeRadioINfoAction->ChangeProviderBand(providerBand, providerSubBand));
    SUCCEED();
}

TEST_F(VR_RadioActiveBandInfoAction_Test, Run_VrRadioActiveBandReq_NotRadio)
{
    m_activeRadioINfoAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioActiveBandInfoAction_Test, Run_VrRadioActiveBandReq_Radio)
{
    struct MediaInfo_CurSrc curSrc;
    memset(&curSrc, 0, sizeof(curSrc));
    curSrc.curSrc = MediaInfo_SrcID_Radio;
    bool ret = NMMediaInfoHelper::save(MediaInfoKey_CurSrc, &curSrc, sizeof(curSrc));
    m_activeRadioINfoAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioActiveBandInfoAction_Test, Run_default)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrRadioActiveBandReq> protoMsg(VrRadioActiveBandReq::default_instance().New());
    m_activeRadioINfoAction = VR_new VR_RadioActiveBandInfoAction(actionSender, 1, ValidTemperature, *protoMsg);
    m_activeRadioINfoAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioActiveBandInfoAction_Test, Run_VrTuneFMStationName)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMStationName> protoMsg(VrTuneFMStationName::default_instance().New());
    m_activeRadioINfoAction = VR_new VR_RadioActiveBandInfoAction(actionSender, 1, TuneFMStationName, *protoMsg);
    m_activeRadioINfoAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioActiveBandInfoAction_Test, Run_VrTuneFMStationName_equalName)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMStationName> protoMsg(VrTuneFMStationName::default_instance().New());
    protoMsg->set_station_name("start");

    struct MediaInfo_TunerStationList tunerStationList;
    memset(&tunerStationList, 0, sizeof(tunerStationList));
    tunerStationList.stationNum = 1;
    strcpy((char*)tunerStationList.station[0].psName, "start");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_TunerStationList, &tunerStationList, sizeof(tunerStationList));
    VR_LOGD("[bret]=[%d]", bret);

    m_activeRadioINfoAction = VR_new VR_RadioActiveBandInfoAction(actionSender, 1, TuneFMStationName, *protoMsg);
    m_activeRadioINfoAction->Run();
    SUCCEED();
}

TEST_F(VR_RadioActiveBandInfoAction_Test, Run_VrTuneFMStationName_diffName)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrTuneFMStationName> protoMsg(VrTuneFMStationName::default_instance().New());
    protoMsg->set_station_name("start");

    struct MediaInfo_TunerStationList tunerStationList;
    memset(&tunerStationList, 0, sizeof(tunerStationList));
    tunerStationList.stationNum = 1;
    strcpy((char*)tunerStationList.station[0].psName, "stop");
    bool bret = NMMediaInfoHelper::save(MediaInfoKey_TunerStationList, &tunerStationList, sizeof(tunerStationList));
    VR_LOGD("[bret]=[%d]", bret);

    m_activeRadioINfoAction = VR_new VR_RadioActiveBandInfoAction(actionSender, 1, TuneFMStationName, *protoMsg);
    m_activeRadioINfoAction->Run();
    SUCCEED();
}

/* EOF */
