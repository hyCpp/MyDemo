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
#include "VR_HVACCheckTemperature.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_HVACCheckTemperature_Test
*
* The class is just for VR_HVACCheckTemperature test.
*/
class VR_HVACCheckTemperature_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_HVACCheckTemperature *m_Vr_CheckTemperatureAction;
    VR_HVACCheckTemperature *m_Vr_CheckUnderMinAction;
    VR_HVACCheckTemperature *m_Vr_CheckTempEqualAction;
    VR_HVACCheckTemperature *m_Vr_CheckTempOther;
};

void
VR_HVACCheckTemperature_Test::SetUp()
{
     int nEngineId = 101;
     int nActionType = CheckTemperature;
     std::unique_ptr<VrCheckTemperature> ProtoMsg(VrCheckTemperature::default_instance().New());
     ProtoMsg->set_degree(27.5);
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     spVR_ActionEventSender actionSender(VR_new VR_ActionEventSender(spEngine));
     m_Vr_CheckTemperatureAction = VR_new VR_HVACCheckTemperature(nEngineId, nActionType, *ProtoMsg, actionSender);

     std::unique_ptr<VrCheckTemperature> UnderMin(VrCheckTemperature::default_instance().New());
     UnderMin->set_degree(10.0);
     m_Vr_CheckUnderMinAction = VR_new VR_HVACCheckTemperature(nEngineId, nActionType, *UnderMin, actionSender);

     std::unique_ptr<VrCheckTemperature> TempEqual(VrCheckTemperature::default_instance().New());
     TempEqual->set_degree(15.5);
     m_Vr_CheckTempEqualAction = VR_new VR_HVACCheckTemperature(nEngineId, nActionType, *TempEqual, actionSender);

     ////////////////////////////////// other test //////////////////////////////////////////////////////////////
     std::unique_ptr<VrCheckTemperature>  TempOther(VrCheckTemperature::default_instance().New());
     TempOther->set_degree(15.5);
     m_Vr_CheckTempOther = VR_new VR_HVACCheckTemperature(nEngineId, nActionType, *TempOther, actionSender);
     ///////////////////////////////// END //////////////////////////////////////////////////////////////////////
}

void
VR_HVACCheckTemperature_Test::TearDown()
{
    if (NULL != m_Vr_CheckTemperatureAction) {
        delete m_Vr_CheckTemperatureAction;
        m_Vr_CheckTemperatureAction = NULL;
    }
    if (NULL != m_Vr_CheckUnderMinAction) {
        delete m_Vr_CheckUnderMinAction;
        m_Vr_CheckUnderMinAction = NULL;
    }
    if (NULL != m_Vr_CheckTempEqualAction) {
        delete m_Vr_CheckTempEqualAction;
        m_Vr_CheckTempEqualAction = NULL;
    }
    if (NULL != m_Vr_CheckTempOther) {
        delete m_Vr_CheckTempOther;
        m_Vr_CheckTempOther = NULL;
    }
}

TEST_F(VR_HVACCheckTemperature_Test, Run_Case)
{
    m_Vr_CheckTemperatureAction->Run();
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, GetReplyMsg_Case)
{
    m_Vr_CheckTemperatureAction->GetReplyMsg();
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, GetCheckTemp_Case)
{
   m_Vr_CheckTemperatureAction->GetCheckTemp();
   SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, SetTempRange_Case)
{
    m_Vr_CheckTemperatureAction->SetTempRange();
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, SetTempRange_1_Case)
{
    std::unique_ptr<VrTemperatureRangeResp> tempRange(VrTemperatureRangeResp::default_instance().New());
    tempRange->set_min_temperature(15);
    tempRange->set_max_temperature(32);

    m_Vr_CheckTemperatureAction->SetTempRange(*tempRange);
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, SetCurrTemp_Case)
{
    std::unique_ptr<VrTemperatureResp> tempResp(VrTemperatureResp::default_instance().New());
    tempResp->set_temperature_state(0x06);

    m_Vr_CheckTemperatureAction->SetCurrTemp(*tempResp);
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, OnReceive_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(Temperature);
    VrTemperatureResp *  pTemperatureResq = additionInfoResp->mutable_temperature_resp();
    pTemperatureResq->set_temperature_state(0x07);

    m_Vr_CheckTemperatureAction->OnReceive(*additionInfoResp);
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, OnReceive_FAH_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(Temperature);
    VrTemperatureResp *  pTemperatureResq = additionInfoResp->mutable_temperature_resp();
    pTemperatureResq->set_temperature_state(0x25);

    m_Vr_CheckTemperatureAction->OnReceive(*additionInfoResp);
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, OnReceive_UnderMin_Case)
{
    VrTemperatureRangeResp tempRange;
    tempRange.set_min_temperature(15);
    tempRange.set_max_temperature(32);
    m_Vr_CheckUnderMinAction->SetTempRange(tempRange);

    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(Temperature);
    VrTemperatureResp *  pTemperatureResq = additionInfoResp->mutable_temperature_resp();
    pTemperatureResq->set_temperature_state(0x25);

    m_Vr_CheckUnderMinAction->OnReceive(*additionInfoResp);
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, OnReceive_TempEqual_Case)
{
    VrTemperatureRangeResp tempRange;
    tempRange.set_min_temperature(15);
    tempRange.set_max_temperature(32);
    m_Vr_CheckUnderMinAction->SetTempRange(tempRange);

    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    additionInfoResp->set_response_id(Temperature);
    VrTemperatureResp *  pTemperatureResq = additionInfoResp->mutable_temperature_resp();
    pTemperatureResq->set_temperature_state(0x02);

    m_Vr_CheckUnderMinAction->OnReceive(*additionInfoResp);
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, OnReceive_else_Case)
{
    std::unique_ptr<VrAdditionalInfoRsp>  additionInfoResp(VrAdditionalInfoRsp::default_instance().New());
    m_Vr_CheckTemperatureAction->OnReceive(*additionInfoResp);
    SUCCEED();
}

TEST_F(VR_HVACCheckTemperature_Test, OnReceive_Other_Case)
{
    ///////////////////////// check temp equal current temp //////////////////////////////////////////
    VrAdditionalInfoRsp  AdditionInfo;
    AdditionInfo.set_response_id(Temperature);
    VrTemperatureResp *pTemperature = AdditionInfo.mutable_temperature_resp();
    pTemperature->set_temperature_state(0x02);
    m_Vr_CheckTempOther->OnReceive(AdditionInfo);
    /////////////////////////////  normal   ///////////////////////////////////////////////////////////
    VrTemperatureRangeResp tempRange;
    tempRange.set_min_temperature(15);
    tempRange.set_max_temperature(32);
    m_Vr_CheckTempOther->SetTempRange(tempRange);

    VrAdditionalInfoRsp  AdditionNormal;
    AdditionNormal.set_response_id(Temperature);
    VrTemperatureResp *pTempNormal = AdditionNormal.mutable_temperature_resp();
    pTempNormal->set_temperature_state(0x07);
    m_Vr_CheckTempOther->OnReceive(AdditionNormal);
    ////////////////////////////   end   ///////////////////////////////////////////////////////////////
    VrAdditionalInfoRsp  AdditionInvalid;
    AdditionInvalid.set_response_id(Temperature);
    VrTemperatureResp* pTempInvalid = AdditionInvalid.mutable_temperature_resp();
    pTempInvalid->set_temperature_state(0xff);
    m_Vr_CheckTempOther->OnReceive(AdditionInvalid);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
}
/* EOF */









