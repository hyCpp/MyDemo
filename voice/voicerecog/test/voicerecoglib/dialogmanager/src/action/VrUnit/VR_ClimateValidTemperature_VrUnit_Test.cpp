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
#include "VR_HVACXMLMessageBuilder.h"
#include "VR_DialogEngine_mock.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_ClimateValidTemperature_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateValidTemperature_VrUnit_Test
*
* The class is just for VR_ClimateValidTemperature_VrUnit test.
*/
class VR_ClimateValidTemperature_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateValidTemperature_VrUnit *m_Vr_ValidTemperatureAction;
    VR_ClimateValidTemperature_VrUnit *m_Vr_DecreaseTemp;
    VR_ClimateValidTemperature_VrUnit *m_Vr_TemperatureMax;
    VR_ClimateValidTemperature_VrUnit *m_Vr_TemperatureMin;

    VR_ClimateValidTemperature_VrUnit *m_Vr_TempTestOther;
};

void
VR_ClimateValidTemperature_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = ValidTemperature;
     std::unique_ptr<VrValidTemperature> ProtoMsg(VrValidTemperature::default_instance().New());
     ProtoMsg->set_type(ValidTemperatureType_Increase);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF    avclanSender(VR_new VR_AvcLanSender_mock);
     m_Vr_ValidTemperatureAction = VR_new VR_ClimateValidTemperature_VrUnit(nEngineId, nActionType, *ProtoMsg, actionSender, avclanSender);
     //////////////////////////////////// ValidTemperatureType_Decrease ////////////////////////////////////////////
     nActionType = ValidTemperature;
     std::unique_ptr<VrValidTemperature> DecreaseTemp(VrValidTemperature::default_instance().New());
     DecreaseTemp->set_type(ValidTemperatureType_Decrease);
     m_Vr_DecreaseTemp = VR_new VR_ClimateValidTemperature_VrUnit(nEngineId, nActionType, *DecreaseTemp, actionSender, avclanSender);
     //////////////////////////////////// ValidTemperatureType_Max ////////////////////////////////////////////
     nActionType = ValidTemperature;
     std::unique_ptr<VrValidTemperature> TemperatureMax(VrValidTemperature::default_instance().New());
     TemperatureMax->set_type(ValidTemperatureType_Max);
     m_Vr_TemperatureMax = VR_new VR_ClimateValidTemperature_VrUnit(nEngineId, nActionType, *TemperatureMax, actionSender, avclanSender);
     //////////////////////////////////// validTemperatureTeyp_Min ////////////////////////////////////////////
     nActionType = ValidTemperature;
     std::unique_ptr<VrValidTemperature> TempMin(VrValidTemperature::default_instance().New());
     TempMin->set_type(validTemperatureTeyp_Min);
     m_Vr_TemperatureMin = VR_new VR_ClimateValidTemperature_VrUnit(nEngineId, nActionType, *TempMin, actionSender, avclanSender);

     nActionType = ValidTemperature;
     std::unique_ptr<VrValidTemperature> TestOther(VrValidTemperature::default_instance().New());
     TestOther->set_type((VrValidTemperatureType)100);
     m_Vr_TempTestOther = VR_new VR_ClimateValidTemperature_VrUnit(nEngineId, nActionType, *TestOther, actionSender, avclanSender);
}

void
VR_ClimateValidTemperature_VrUnit_Test::TearDown()
{
    if (NULL != m_Vr_ValidTemperatureAction) {
        delete m_Vr_ValidTemperatureAction;
        m_Vr_ValidTemperatureAction = NULL;
    }
    if (NULL != m_Vr_DecreaseTemp) {
        delete m_Vr_DecreaseTemp;
        m_Vr_DecreaseTemp = NULL;
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

TEST_F(VR_ClimateValidTemperature_VrUnit_Test, Run_Case)
{
    m_Vr_ValidTemperatureAction->Run();
    m_Vr_TempTestOther->Run();
    SUCCEED();
}

TEST_F(VR_ClimateValidTemperature_VrUnit_Test, SetTempRange_Case)
{
    m_Vr_ValidTemperatureAction->SetTempRange();
    SUCCEED();
}

TEST_F(VR_ClimateValidTemperature_VrUnit_Test, SetTempRange_1_Case)
{
    m_Vr_ValidTemperatureAction->SetTempRange();
    SUCCEED();
}

TEST_F(VR_ClimateValidTemperature_VrUnit_Test, GetReplyMsg_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x07);
    m_Vr_ValidTemperatureAction->GetReplyMsg(*tempResp);
    /////////////////////////// already at max /////////////////////////////////////////////////////////
    VrTemperatureResp  MaxTemp;
    MaxTemp.set_temperature_state(0x3e);
    m_Vr_ValidTemperatureAction->GetReplyMsg(MaxTemp);
    ///////////////////////////  cel temp //////////////////////////////////////////////////////////////
    VrTemperatureResp  CelTemp;
    CelTemp.set_temperature_state(0x02);
    m_Vr_ValidTemperatureAction->GetReplyMsg(CelTemp);
    ///////////////////////// cel at max //////////////////////////////////////////////////////////////
    VR_HVACXMLMessageBuilder::GetMessageBuilder().SetLowTemp(15);
    VR_HVACXMLMessageBuilder::GetMessageBuilder().SetLowTemp(32);
    m_Vr_ValidTemperatureAction->SetTempRange();
    VrTemperatureResp  CelMaxTemp;
    CelMaxTemp.set_temperature_state(0x23);
    m_Vr_ValidTemperatureAction->GetReplyMsg(CelMaxTemp);
    /////////////////////// Cel will at max ///////////////////////////////////////////////////////////
    VrTemperatureResp CelToMax;
    CelTemp.set_temperature_state(0x22);
    m_Vr_ValidTemperatureAction->GetReplyMsg(CelToMax);
    /////////////////////// fah temp /////////////////////////////////////////////////////////////////
    VrTemperatureResp FahTemp;
    FahTemp.set_temperature_state(0x25);
    m_Vr_ValidTemperatureAction->GetReplyMsg(FahTemp);
    ////////////////////// fah at max ///////////////////////////////////////////////////////////////
    VrTemperatureResp  FahAtMax;
    FahAtMax.set_temperature_state(0x3d);
    m_Vr_ValidTemperatureAction->GetReplyMsg(FahAtMax);
    ///////////////////// fah to max ///////////////////////////////////////////////////////////////
    VrTemperatureResp  FahToMax;
    FahToMax.set_temperature_state(0x3c);
    m_Vr_ValidTemperatureAction->GetReplyMsg(FahToMax);
    //////////////////// temp is min ///////////////////////////////////////////////////////////////
    VrTemperatureResp TempIsMin;
    TempIsMin.set_temperature_state(0x00);
    m_Vr_ValidTemperatureAction->GetReplyMsg(TempIsMin);
    ////////////////////////
    m_Vr_ValidTemperatureAction->SetTempRange();
    m_Vr_ValidTemperatureAction->GetReplyMsg(TempIsMin);
    /////////////////////////////// decrease ///////////////////////////////////////////////////////
    VrTemperatureResp  DecMinTemp;
    DecMinTemp.set_temperature_state(0x00);
    m_Vr_DecreaseTemp->GetReplyMsg(DecMinTemp);
    /////////////////// Dec CelTemp ////////////////////////////////////////////////////////////////
    VrTemperatureResp DecCelTemp;
    DecCelTemp.set_temperature_state(0x02);
    m_Vr_DecreaseTemp->GetReplyMsg(DecCelTemp);
    //////////////////// Dec at low ///////////////////////////////////////////////////////////////
    VR_HVACXMLMessageBuilder::GetMessageBuilder().SetLowTemp(15);
    VR_HVACXMLMessageBuilder::GetMessageBuilder().SetLowTemp(32);
    m_Vr_DecreaseTemp->SetTempRange();
    VrTemperatureResp DecAtLow;
    DecAtLow.set_temperature_state(0x01);
    m_Vr_DecreaseTemp->GetReplyMsg(DecAtLow);
    ////////////////// Dec to low ////////////////////////////////////////////////////////////////////
    VrTemperatureResp DecToLow;
    DecAtLow.set_temperature_state(0x02);
    m_Vr_DecreaseTemp->GetReplyMsg(DecToLow);
    /////////////////// Dec fah temp //////////////////////////////////////////////////////////////////
    VrTemperatureResp  DecFahTemp;
    DecFahTemp.set_temperature_state(0x25);
    m_Vr_DecreaseTemp->GetReplyMsg(DecFahTemp);
    /////////////////// Dec Fah at Temp /////////////////////////////////////////////////////////////
    VrTemperatureResp  DecFahAtMin;
    DecFahAtMin.set_temperature_state(0x24);
    m_Vr_DecreaseTemp->GetReplyMsg(DecFahAtMin);
    ///////m_Vr_DecreaseTemp/////////////////////////////////////////////////////////////////////////
    VR_HVACXMLMessageBuilder::GetMessageBuilder().SetLowTemp(60);
    VR_HVACXMLMessageBuilder::GetMessageBuilder().SetLowTemp(85);
    m_Vr_DecreaseTemp->SetTempRange();
    m_Vr_DecreaseTemp->GetReplyMsg(DecFahAtMin);
    m_Vr_DecreaseTemp->GetReplyMsg(DecFahTemp);
    ///////////////////////////  valid_max //////////////////////////////////////////////////////////
    m_Vr_TemperatureMax->GetReplyMsg(MaxTemp);
    ////////////////////////// valid_min //////////////////////////////////////////////////////////////
    m_Vr_TemperatureMin->GetReplyMsg(DecMinTemp);
    SUCCEED();
}

TEST_F(VR_ClimateValidTemperature_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(Temperature);
    VrTemperatureResp *  pTemperatureResq = additionInfoResp->mutable_temperature_resp();
    pTemperatureResq->set_temperature_state(0x08);
    m_Vr_ValidTemperatureAction->OnReceive(*additionInfoResp);
    VrAdditionalInfoRsp  InvalidTemp;
    m_Vr_ValidTemperatureAction->OnReceive(InvalidTemp);
    SUCCEED();
}
/* EOF */


