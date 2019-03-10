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
#include "VR_ClimateCheckTemperature_VrUnit.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_ClimateCheckTemperature_VrUnit_Test
*
* The class is just for VR_ClimateCheckTemperature_VrUnit test.
*/
class VR_ClimateCheckTemperature_VrUnit_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_ClimateCheckTemperature_VrUnit *m_Vr_CheckTemperatureAction;
    VR_ClimateCheckTemperature_VrUnit *m_Vr_OverMaxTemp;
    VR_ClimateCheckTemperature_VrUnit *m_Vr_UnderMinTemp;
};

void
VR_ClimateCheckTemperature_VrUnit_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = CheckTemperature;
     std::unique_ptr<VrCheckTemperature> ProtoMsg(VrCheckTemperature::default_instance().New());
     ProtoMsg->set_degree(15.5);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     spVR_AvcLanSenderIF  avclanSender(VR_new VR_AvcLanSender_mock());
     m_Vr_CheckTemperatureAction = VR_new VR_ClimateCheckTemperature_VrUnit(nEngineId, nActionType, *ProtoMsg, actionSender, avclanSender);
     /////////// over max temp ///////////
     std::unique_ptr<VrCheckTemperature>  OverMaxTemp(VrCheckTemperature::default_instance().New());
     OverMaxTemp->set_degree(88.5);
     m_Vr_OverMaxTemp = VR_new VR_ClimateCheckTemperature_VrUnit(nEngineId, nActionType, *OverMaxTemp, actionSender, avclanSender);
     /////////// under min temp ////////////
     std::unique_ptr<VrCheckTemperature>  UnderMinTemp(VrCheckTemperature::default_instance().New());
     UnderMinTemp->set_degree(10.5);
     m_Vr_UnderMinTemp = VR_new VR_ClimateCheckTemperature_VrUnit(nEngineId, nActionType, *UnderMinTemp, actionSender, avclanSender);
}

void
VR_ClimateCheckTemperature_VrUnit_Test::TearDown()
{
    if (NULL != m_Vr_CheckTemperatureAction) {
        delete m_Vr_CheckTemperatureAction;
        m_Vr_CheckTemperatureAction = NULL;
    }
    if (NULL != m_Vr_OverMaxTemp) {
        delete m_Vr_OverMaxTemp;
        m_Vr_OverMaxTemp = NULL;
    }
    if (NULL != m_Vr_UnderMinTemp) {
        delete m_Vr_UnderMinTemp;
        m_Vr_UnderMinTemp = NULL;
    }
}

TEST_F(VR_ClimateCheckTemperature_VrUnit_Test, Run_Case)
{
    m_Vr_CheckTemperatureAction->Run();
    SUCCEED();
}

TEST_F(VR_ClimateCheckTemperature_VrUnit_Test, GetReplyMsg_Case)
{
    m_Vr_CheckTemperatureAction->GetReplyMsg();
    SUCCEED();
}

TEST_F(VR_ClimateCheckTemperature_VrUnit_Test, GetCheckTemp_Case)
{
   m_Vr_CheckTemperatureAction->GetCheckTemp();
   SUCCEED();
}

TEST_F(VR_ClimateCheckTemperature_VrUnit_Test, SetTempRange_Case)
{
    m_Vr_CheckTemperatureAction->SetTempRange();
    SUCCEED();
}

TEST_F(VR_ClimateCheckTemperature_VrUnit_Test, SetTempRange_1_Case)
{
    std::unique_ptr<VrTemperatureRangeResp> tempRange(VrTemperatureRangeResp::default_instance().New());
    tempRange->set_min_temperature(15);
    tempRange->set_max_temperature(32);

    m_Vr_CheckTemperatureAction->SetTempRange(*tempRange);
    SUCCEED();
}

TEST_F(VR_ClimateCheckTemperature_VrUnit_Test, SetCurrTemp_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x06);

    m_Vr_CheckTemperatureAction->SetCurrTemp(*tempResp);
    SUCCEED();
}

TEST_F(VR_ClimateCheckTemperature_VrUnit_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(Temperature);
    VrTemperatureResp *  pTemperatureResq = additionInfoResp->mutable_temperature_resp();
    pTemperatureResq->set_temperature_state(0x07);
    m_Vr_CheckTemperatureAction->OnReceive(*additionInfoResp);
    /////////////////////// fah temp /////////////////////////////////////////////
    VrAdditionalInfoRsp  FahAdditional;
    FahAdditional.set_response_id(Temperature);
    VrTemperatureResp *pFahTemperature = FahAdditional.mutable_temperature_resp();
    pFahTemperature->set_temperature_state(0x28);
    m_Vr_CheckTemperatureAction->OnReceive(FahAdditional);
    ////////////////////// equal temp ///////////////////////////////////////////
    VrAdditionalInfoRsp  EqualAdditional;
    EqualAdditional.set_response_id(Temperature);
    VrTemperatureResp * pEqualTemp = EqualAdditional.mutable_temperature_resp();
    pEqualTemp->set_temperature_state(0x02);
    m_Vr_CheckTemperatureAction->OnReceive(EqualAdditional);
    ///////////////////// over max temp ////////////////////////////////////////
    VrTemperatureRangeResp  RangeTemp;
    RangeTemp.set_min_temperature(15);
    RangeTemp.set_max_temperature(32);
    m_Vr_OverMaxTemp->SetTempRange(RangeTemp);
    VrAdditionalInfoRsp  OverMaxAdditional;
    OverMaxAdditional.set_response_id(Temperature);
    VrTemperatureResp *pOverMaxTemp = OverMaxAdditional.mutable_temperature_resp();
    pOverMaxTemp->set_temperature_state(0x01);
    m_Vr_OverMaxTemp->OnReceive(OverMaxAdditional);
    /////////////////// under min temp /////////////////////////////////////////
    m_Vr_UnderMinTemp->SetTempRange(RangeTemp);
    VrAdditionalInfoRsp  UnderMinTemp;
    UnderMinTemp.set_response_id(Temperature);
    VrTemperatureResp *pUnderMin = UnderMinTemp.mutable_temperature_resp();
    pUnderMin->set_temperature_state(0x01);
    m_Vr_UnderMinTemp->OnReceive(UnderMinTemp);
    SUCCEED();
}

TEST_F(VR_ClimateCheckTemperature_VrUnit_Test, OnReceive_Other_Case)
{
    ////////////////////// test other ///////////////////////////////////////////
    VrAdditionalInfoRsp  AdditionInfo;
    AdditionInfo.set_response_id(FanSpeed);
    m_Vr_CheckTemperatureAction->SetCurrTemp(AdditionInfo);

    VrAdditionalInfoRsp  AdditionOther;
    AdditionOther.set_response_id(Temperature);
    VrTemperatureResp *pTemperature = AdditionOther.mutable_temperature_resp();
    pTemperature->set_temperature_state(0xff);
    m_Vr_CheckTemperatureAction->OnReceive(AdditionOther);
    SUCCEED();
}
/* EOF */


