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
#include "VR_HVACTuneClimateAction.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACTuneClimateAction_Test
*
* The class is just for VR_HVACTuneClimateAction test.
*/
class VR_HVACTuneClimateAction_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACTuneClimateAction *m_Vr_TuneClimateAction;
    VR_HVACTuneClimateAction *m_Vr_ChangeClimateTemp;
    VR_HVACTuneClimateAction *m_Vr_IncreaseTemperature;
    VR_HVACTuneClimateAction *m_Vr_AFewWarmer;
    VR_HVACTuneClimateAction *m_Vr_AFewCooler;
    VR_HVACTuneClimateAction *m_Vr_TemperatureMax;
    VR_HVACTuneClimateAction *m_Vr_TemperatureMin;
    VR_HVACTuneClimateAction *m_Vr_ChangeClimateTempMax;
    VR_HVACTuneClimateAction *m_Vr_ChangeClimateTempMin;

    //////////////////////////// Tune Climate Other //////////////////////////////////
    VR_HVACTuneClimateAction *m_Vr_TuneClimateOther;
    VR_HVACTuneClimateAction *m_Vr_TuneClimateOther1;
    VR_HVACTuneClimateAction *m_Vr_TuneClimateOther2;
};

void
VR_HVACTuneClimateAction_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = DecreaseTemperature;
     std::unique_ptr<VrDecreaseTemperature> ProtoMsg(VrDecreaseTemperature::default_instance().New());
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_TuneClimateAction = VR_new VR_HVACTuneClimateAction(nEngineId, nActionType, *ProtoMsg, actionSender);

     nActionType = ChangeTemperature;
     std::unique_ptr<VrChangeTemperature> ChangeMsg(VrChangeTemperature::default_instance().New());
     ChangeMsg->set_temperature(0x25);
     m_Vr_ChangeClimateTemp = VR_new VR_HVACTuneClimateAction(nEngineId, nActionType, *ChangeMsg, actionSender);

     nActionType = ChangeTemperature;
     std::unique_ptr<VrChangeTemperature> ChangeMax(VrChangeTemperature::default_instance().New());
     ChangeMax->set_temperature(0x3e);
     m_Vr_ChangeClimateTempMax = VR_new VR_HVACTuneClimateAction(nEngineId, nActionType, *ChangeMax, actionSender);

     nActionType = ChangeTemperature;
     std::unique_ptr<VrChangeTemperature> ChangeMin(VrChangeTemperature::default_instance().New());
     ChangeMin->set_temperature(0x00);
     m_Vr_ChangeClimateTempMin = VR_new VR_HVACTuneClimateAction(nEngineId, nActionType, *ChangeMin, actionSender);

     nActionType = IncreaseTemperature;
     std::unique_ptr<VrIncreaseTemperature>  increaseTemp(VrIncreaseTemperature::default_instance().New());
     increaseTemp->set_temperature_unit(0);
     increaseTemp->set_temperature(0x25);
     m_Vr_IncreaseTemperature = VR_new VR_HVACTuneClimateAction(nEngineId, nActionType, *increaseTemp, actionSender);

     nActionType = AFewWarmer;
     std::unique_ptr<VrAFewWarmer>  afewWarmer(VrAFewWarmer::default_instance().New());
     afewWarmer->set_temperature_unit(0);
     afewWarmer->set_temperature(0x25);
     m_Vr_AFewWarmer = VR_new  VR_HVACTuneClimateAction(nEngineId, nActionType, *afewWarmer, actionSender);

     nActionType = AFewCooler;
     std::unique_ptr<VrAFewCooler>  afewCooler(VrAFewCooler::default_instance().New());
     afewCooler->set_temperature_unit(0);
     afewCooler->set_temperature(0x25);
     m_Vr_AFewCooler = VR_new  VR_HVACTuneClimateAction(nEngineId, nActionType, *afewCooler, actionSender);

     nActionType = TemperatureMax;
     std::unique_ptr<VrTemperatureMax>  temperatureMax(VrTemperatureMax::default_instance().New());
     m_Vr_TemperatureMax = VR_new  VR_HVACTuneClimateAction(nEngineId, nActionType, *temperatureMax, actionSender);

     nActionType = TemperatureMin;
     std::unique_ptr<VrTemperatureMin>  temperatureMin(VrTemperatureMin::default_instance().New());
     m_Vr_TemperatureMin = VR_new  VR_HVACTuneClimateAction(nEngineId, nActionType, *temperatureMin, actionSender);

     //////////////////////////// Tune Climate Other /////////////////////////////////////////////////////////////
     nActionType = ChangeTemperature;
     std::unique_ptr<VrChangeTemperature>  TempOther(VrChangeTemperature::default_instance().New());
     TempOther->set_temperature(0x02);
     m_Vr_TuneClimateOther = VR_new VR_HVACTuneClimateAction(nEngineId, nActionType, *TempOther, actionSender);

     nActionType = ChangeTemperature;
     std::unique_ptr<VrChangeTemperature>  TempOther1(VrChangeTemperature::default_instance().New());
     TempOther1->set_temperature(0xff);
     m_Vr_TuneClimateOther1 = VR_new VR_HVACTuneClimateAction(nEngineId, nActionType, *TempOther1, actionSender);

     nActionType = AudioON;
     std::unique_ptr<VrAudioON>  TempOther2(VrAudioON::default_instance().New());
     m_Vr_TuneClimateOther2 = VR_new VR_HVACTuneClimateAction(nEngineId, nActionType, *TempOther2, actionSender);
}

void
VR_HVACTuneClimateAction_Test::TearDown()
{
    if (NULL != m_Vr_TuneClimateAction) {
        delete m_Vr_TuneClimateAction;
        m_Vr_TuneClimateAction = NULL;
    }
    if (NULL != m_Vr_ChangeClimateTemp) {
        delete m_Vr_ChangeClimateTemp;
        m_Vr_ChangeClimateTemp = NULL;
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
    if (NULL != m_Vr_ChangeClimateTempMax) {
        delete m_Vr_ChangeClimateTempMax;
        m_Vr_ChangeClimateTempMax = NULL;
    }
    if (NULL != m_Vr_ChangeClimateTempMin) {
        delete m_Vr_ChangeClimateTempMin;
        m_Vr_ChangeClimateTempMin = NULL;
    }
    if (NULL != m_Vr_TuneClimateOther) {
        delete m_Vr_TuneClimateOther;
        m_Vr_TuneClimateOther = NULL;
    }
    if (NULL != m_Vr_TuneClimateOther1) {
        delete m_Vr_TuneClimateOther1;
        m_Vr_TuneClimateOther1 = NULL;
    }
    if (NULL != m_Vr_TuneClimateOther2) {
        delete m_Vr_TuneClimateOther2;
        m_Vr_TuneClimateOther2 = NULL;
    }
}

TEST_F(VR_HVACTuneClimateAction_Test, Run_Case)
{
    m_Vr_TuneClimateAction->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, Run_Change_Case)
{
    m_Vr_ChangeClimateTemp->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, IncreaseTemperature_Case)
{
    m_Vr_IncreaseTemperature->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, AFewWarmer_Case)
{
    m_Vr_AFewWarmer->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, AFewCooler_Case)
{
    m_Vr_AFewCooler->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, TemperatureMax_Case)
{
    m_Vr_TemperatureMax->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, TemperatureMin_Case)
{
    m_Vr_TemperatureMin->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, TemperatureChangeMax_Case)
{
    m_Vr_ChangeClimateTempMax->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, TemperatureChangeMin_Case)
{
    m_Vr_ChangeClimateTempMin->Run();
    SUCCEED();
}

/////////////////////////////////////  Tune climate other /////////////////////////////////////////////
TEST_F(VR_HVACTuneClimateAction_Test,  TuneClimateOther_Case)
{
    m_Vr_TuneClimateOther->Run();
    m_Vr_TuneClimateOther1->Run();
    m_Vr_TuneClimateOther2->Run();
    SUCCEED();
}

TEST_F(VR_HVACTuneClimateAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(DecreaseTemperature);
    actionComfirm->set_processing_result(true);
    m_Vr_TuneClimateAction->OnReceive(*actionComfirm);
    SUCCEED();
}
/* EOF */

