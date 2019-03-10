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
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_ClimateTuneAction_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateTuneAction_VrUnit_Test
*
* The class is just for VR_ClimateTuneAction_VrUnit test.
*/
class VR_ClimateTuneAction_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateTuneAction_VrUnit *m_Vr_TuneClimateAction;
    VR_ClimateTuneAction_VrUnit *m_Vr_ChangeTemperature;
    VR_ClimateTuneAction_VrUnit *m_Vr_IncreaseTemperature;
    VR_ClimateTuneAction_VrUnit *m_Vr_AFewWarmer;
    VR_ClimateTuneAction_VrUnit *m_Vr_AFewCooler;
    VR_ClimateTuneAction_VrUnit *m_Vr_TemperatureMax;
    VR_ClimateTuneAction_VrUnit *m_Vr_TemperatureMin;

    VR_ClimateTuneAction_VrUnit* m_Vr_TempTestOther;
};

void
VR_ClimateTuneAction_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = DecreaseTemperature;
     std::unique_ptr<VrDecreaseTemperature> ProtoMsg(VrDecreaseTemperature::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock());
     m_Vr_TuneClimateAction = VR_new VR_ClimateTuneAction_VrUnit(nEngineId, nActionType, *ProtoMsg, actionSender, avclanSender);
     /////////////////////////////// ChangeTemperature //////////////////////////////////////////////////////////////////////////
     nActionType = ChangeTemperature;
     std::unique_ptr<VrChangeTemperature> ChangeTemperatureReq(VrChangeTemperature::default_instance().New());
     m_Vr_ChangeTemperature = VR_new VR_ClimateTuneAction_VrUnit(nEngineId, nActionType, *ChangeTemperatureReq, actionSender, avclanSender);
     /////////////////////////////// IncreaseTemperature //////////////////////////////////////////////////////////////////////////
     nActionType = IncreaseTemperature;
     std::unique_ptr<VrIncreaseTemperature> IncreaseTemperatureReq(VrIncreaseTemperature::default_instance().New());
     m_Vr_IncreaseTemperature = VR_new VR_ClimateTuneAction_VrUnit(nEngineId, nActionType, *IncreaseTemperatureReq, actionSender, avclanSender);
     /////////////////////////////// AFewWarmer //////////////////////////////////////////////////////////////////////////
     nActionType = AFewWarmer;
     std::unique_ptr<VrAFewWarmer> AFewWarmerReq(VrAFewWarmer::default_instance().New());
     m_Vr_AFewWarmer = VR_new VR_ClimateTuneAction_VrUnit(nEngineId, nActionType, *AFewWarmerReq, actionSender, avclanSender);
     /////////////////////////////// AFewCooler //////////////////////////////////////////////////////////////////////////
     nActionType = AFewCooler;
     std::unique_ptr<VrAFewCooler> AFewCoolerReq(VrAFewCooler::default_instance().New());
     m_Vr_AFewCooler = VR_new VR_ClimateTuneAction_VrUnit(nEngineId, nActionType, *AFewCoolerReq, actionSender, avclanSender);
     /////////////////////////////// TemperatureMax //////////////////////////////////////////////////////////////////////////
     nActionType = TemperatureMax;
     std::unique_ptr<VrTemperatureMax> TemperatureMaxReq(VrTemperatureMax::default_instance().New());
     m_Vr_TemperatureMax = VR_new VR_ClimateTuneAction_VrUnit(nEngineId, nActionType, *TemperatureMaxReq, actionSender, avclanSender);
     /////////////////////////////// TemperatureMin //////////////////////////////////////////////////////////////////////////
     nActionType = TemperatureMin;
     std::unique_ptr<VrTemperatureMin> TemperatureMinReq(VrTemperatureMin::default_instance().New());
     m_Vr_TemperatureMin = VR_new VR_ClimateTuneAction_VrUnit(nEngineId, nActionType, *TemperatureMinReq, actionSender, avclanSender);

     nActionType = AudioON;
     std::unique_ptr<VrAudioON> AudioONReq(VrAudioON::default_instance().New());
     m_Vr_TempTestOther = VR_new VR_ClimateTuneAction_VrUnit(nEngineId, nActionType, *AudioONReq, actionSender, avclanSender);
}

void
VR_ClimateTuneAction_VrUnit_Test::TearDown()
{
    if (NULL != m_Vr_TuneClimateAction) {
        delete m_Vr_TuneClimateAction;
        m_Vr_TuneClimateAction = NULL;
    }
    if (NULL != m_Vr_ChangeTemperature) {
        delete m_Vr_ChangeTemperature;
        m_Vr_ChangeTemperature = NULL;
    }
    if (NULL != m_Vr_IncreaseTemperature) {
        delete m_Vr_IncreaseTemperature;
        m_Vr_IncreaseTemperature = NULL;
    }
    if (NULL != m_Vr_AFewWarmer) {
        delete m_Vr_AFewWarmer;
        m_Vr_AFewWarmer = NULL;
    }
    if (NULL != m_Vr_AFewCooler) {
        delete m_Vr_AFewCooler;
        m_Vr_AFewCooler = NULL;
    }
    if (NULL != m_Vr_TemperatureMax) {
        delete m_Vr_TemperatureMax;
        m_Vr_TemperatureMax = NULL;
    }
    if (NULL != m_Vr_TemperatureMin) {
        delete m_Vr_TemperatureMin;
        m_Vr_TemperatureMin = NULL;
    }
    if (NULL != m_Vr_TempTestOther) {
        delete m_Vr_TempTestOther;
        m_Vr_TempTestOther = NULL;
    }
}

TEST_F(VR_ClimateTuneAction_VrUnit_Test, Run_Case)
{
    m_Vr_TuneClimateAction->Run();
    m_Vr_ChangeTemperature->Run();
    m_Vr_IncreaseTemperature->Run();
    m_Vr_AFewWarmer->Run();
    m_Vr_AFewCooler->Run();
    m_Vr_TemperatureMax->Run();
    m_Vr_TemperatureMin->Run();
    m_Vr_TempTestOther->Run();
    SUCCEED();
}

TEST_F(VR_ClimateTuneAction_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(DecreaseTemperature);
    actionComfirm->set_processing_result(true);

    m_Vr_TuneClimateAction->OnReceive(*actionComfirm);
    SUCCEED();
}
/* EOF */


