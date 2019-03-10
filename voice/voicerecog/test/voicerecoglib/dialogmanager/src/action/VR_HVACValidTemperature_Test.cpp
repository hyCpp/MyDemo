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
#include "VR_HVACValidTemperature.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACValidTemperature_Test
*
* The class is just for VR_HVACValidTemperature test.
*/
class VR_HVACValidTemperature_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACValidTemperature *m_Vr_ValidTemperatureAction;
    VR_HVACValidTemperature *m_Vr_DecreaseTemp;
    VR_HVACValidTemperature *m_Vr_MaxTemp;
    VR_HVACValidTemperature *m_Vr_MinTemp;

    VR_HVACValidTemperature *m_Vr_TempOther;
};

void
VR_HVACValidTemperature_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = ValidTemperature;
     std::unique_ptr<VrValidTemperature> ProtoMsg(VrValidTemperature::default_instance().New());
     ProtoMsg->set_type(ValidTemperatureType_Increase);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_ValidTemperatureAction = VR_new VR_HVACValidTemperature(nEngineId, nActionType, *ProtoMsg, actionSender);

     std::unique_ptr<VrValidTemperature> DecreaseTemp(VrValidTemperature::default_instance().New());
     DecreaseTemp->set_type(ValidTemperatureType_Decrease);
     m_Vr_DecreaseTemp = VR_new VR_HVACValidTemperature(nEngineId, nActionType, *DecreaseTemp, actionSender);

     std::unique_ptr<VrValidTemperature> MaxTemp(VrValidTemperature::default_instance().New());
     DecreaseTemp->set_type(ValidTemperatureType_Max);
     m_Vr_MaxTemp = VR_new VR_HVACValidTemperature(nEngineId, nActionType, *MaxTemp, actionSender);

     std::unique_ptr<VrValidTemperature> MinTemp(VrValidTemperature::default_instance().New());
     DecreaseTemp->set_type(validTemperatureTeyp_Min);
     m_Vr_MinTemp = VR_new VR_HVACValidTemperature(nEngineId, nActionType, *MinTemp, actionSender);

     std::unique_ptr<VrValidTemperature> TempOther(VrValidTemperature::default_instance().New());
     TempOther->set_type((VrValidTemperatureType)100);
     m_Vr_TempOther = VR_new VR_HVACValidTemperature(nEngineId, nActionType, *TempOther, actionSender);
}

void
VR_HVACValidTemperature_Test::TearDown()
{
    if (NULL != m_Vr_ValidTemperatureAction) {
        delete m_Vr_ValidTemperatureAction;
        m_Vr_ValidTemperatureAction = NULL;
    }
    if (NULL != m_Vr_DecreaseTemp) {
        delete m_Vr_DecreaseTemp;
        m_Vr_DecreaseTemp = NULL;
    }
    if (NULL != m_Vr_MaxTemp) {
        delete m_Vr_MaxTemp;
        m_Vr_MaxTemp = NULL;
    }
    if (NULL != m_Vr_MinTemp) {
        delete m_Vr_MinTemp;
        m_Vr_MinTemp = NULL;
    }
    if (NULL != m_Vr_TempOther) {
        delete m_Vr_TempOther;
        m_Vr_TempOther = NULL;
    }
}

TEST_F(VR_HVACValidTemperature_Test, ReplyMsg_MinTemp_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x00);

    m_Vr_MinTemp->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, ReplyMsg_MaxTemp_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x3e);

    m_Vr_MaxTemp->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, ReplyMsg_DecreaseTemp_Min_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x00);

    m_Vr_DecreaseTemp->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, ReplyMsg_DecreaseTemp_Max_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x3e);

    m_Vr_DecreaseTemp->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, ReplyMsg_DecreaseTemp_02_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x08);

    m_Vr_DecreaseTemp->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, ReplyMsg_DecreaseTemp_01_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x26);

    m_Vr_DecreaseTemp->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, Run_Case)
{
    m_Vr_ValidTemperatureAction->Run();
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, SetTempRange_Case)
{
    m_Vr_ValidTemperatureAction->SetTempRange();
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, SetTempRange_1_Case)
{
    int nLow  = 15;
    int nHigh = 32;
    m_Vr_ValidTemperatureAction->SetTempRange(nLow, nHigh);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, GetReplyMsg_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x07);

    m_Vr_ValidTemperatureAction->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, GetReplyMsg_Case_Max)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x3e);

    m_Vr_ValidTemperatureAction->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, GetReplyMsg_Case_Min)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x00);

    m_Vr_ValidTemperatureAction->GetReplyMsg(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(Temperature);
    VrTemperatureResp *  pTemperatureResq = additionInfoResp->mutable_temperature_resp();
    pTemperatureResq->set_temperature_state(0x08);

    m_Vr_ValidTemperatureAction->OnReceive(*additionInfoResp);
    m_Vr_TempOther->OnReceive(*additionInfoResp);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, OnReceive_AtMax_Case)
{
    //////////////////////////////////////////////////////////////////////////////
    ///////// IncreaseTemp ///////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    m_Vr_ValidTemperatureAction->SetTempRange(15, 32);
    VrTemperatureResp  tempResp;
    tempResp.set_temperature_state(0x23);
    m_Vr_ValidTemperatureAction->GetReplyMsg(tempResp);

    VrTemperatureResp  currentResp;
    currentResp.set_temperature_state(0x07);
    m_Vr_ValidTemperatureAction->GetReplyMsg(currentResp);

    VrTemperatureResp fahResp;
    fahResp.set_temperature_state(0x3d);
    m_Vr_ValidTemperatureAction->GetReplyMsg(fahResp);

    VrTemperatureResp fahRespAdd;
    fahRespAdd.set_temperature_state(0x3b);
    m_Vr_ValidTemperatureAction->GetReplyMsg(fahRespAdd);

    VrTemperatureResp currentFahResp;
    currentFahResp.set_temperature_state(0x27);
    m_Vr_ValidTemperatureAction->GetReplyMsg(currentFahResp);

    VrTemperatureResp  minResp;
    minResp.set_temperature_state(0x00);
    m_Vr_ValidTemperatureAction->SetTempRange(60, 85);
    m_Vr_ValidTemperatureAction->GetReplyMsg(minResp);

    VrTemperatureResp invalidTemp;
    invalidTemp.set_temperature_state(0xff);
    m_Vr_ValidTemperatureAction->GetReplyMsg(invalidTemp);

    ///////////////////////////////////////////////////////////////////////////////
    /// DecreaseTemp //////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    m_Vr_DecreaseTemp->SetTempRange(60, 85);
    VrTemperatureResp  decreaseResp;
    decreaseResp.set_temperature_state(0x3e);
    m_Vr_DecreaseTemp->GetReplyMsg(decreaseResp);

    m_Vr_DecreaseTemp->SetTempRange(15, 32);
    VrTemperatureResp  decreaseMin;
    decreaseMin.set_temperature_state(0x01);
    m_Vr_DecreaseTemp->GetReplyMsg(decreaseMin);

    VrTemperatureResp  decreaseMinDec;
    decreaseMinDec.set_temperature_state(0x02);
    m_Vr_DecreaseTemp->GetReplyMsg(decreaseMinDec);

    m_Vr_DecreaseTemp->SetTempRange(60, 85);
    VrTemperatureResp  decreaseMinFah;
    decreaseMinFah.set_temperature_state(0x24);
    m_Vr_DecreaseTemp->GetReplyMsg(decreaseMinFah);

    VrTemperatureResp  decreaseMinFahDec;
    decreaseMinFahDec.set_temperature_state(0x26);
    m_Vr_DecreaseTemp->GetReplyMsg(decreaseMinFahDec);

    m_Vr_DecreaseTemp->GetReplyMsg(invalidTemp);

    /////////////////////////////////////////////////////////////////////////////////
    /// m_Vr_MaxTemp ////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////
    VrTemperatureResp TempAtMax;
    TempAtMax.set_temperature_state(0x3e);
    m_Vr_MaxTemp->GetReplyMsg(TempAtMax);

    VrTemperatureResp  TempAtMin;
    TempAtMin.set_temperature_state(0x00);
    m_Vr_MinTemp->GetReplyMsg(TempAtMin);
    SUCCEED();
}

TEST_F(VR_HVACValidTemperature_Test, OnReceive_Other_Case)
{
    //////////////////  test other ///////////////////////////////////////
    VrAdditionalInfoRsp  AdditionalInfo;
    AdditionalInfo.set_response_id(FanSpeed);
    VrFanSpeedStateResp *pState = AdditionalInfo.mutable_fan_speed_state_resp();
    pState->set_fan_speed_state(0x00);
    m_Vr_ValidTemperatureAction->OnReceive(AdditionalInfo);
}
/* EOF */



















