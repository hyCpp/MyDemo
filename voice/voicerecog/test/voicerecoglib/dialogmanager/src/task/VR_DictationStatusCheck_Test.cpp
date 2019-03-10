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

#include  "stdafx.h"
#include  "gtest/gtest.h"
#include  "VR_EventSenderIF.h"
#include  "EV_EventContext.h"
#include  "system/NCEnvironment.h"
#include  "VR_DictationStatusCheck.h"
#include  "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_DictationStatusCheck_Test.cpp
*
* The class is just for VR_DictationStatusCheck test.
*/
class VR_DictationStatusCheck_Test : public testing::Test
{
protected:
    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_DictationStatusCheck*      m_Vr_DictationStatusCheck;
};

void
VR_DictationStatusCheck_Test::SetUp()
{
    spVR_EventSenderIF  EventSender(VR_EventSenderIF::GetSender());
    m_Vr_DictationStatusCheck = VR_new  VR_DictationStatusCheck(EventSender);
}

void
VR_DictationStatusCheck_Test::TearDown()
{
    if (m_Vr_DictationStatusCheck) {
        delete m_Vr_DictationStatusCheck;
        m_Vr_DictationStatusCheck = NULL;
    }
}

TEST_F(VR_DictationStatusCheck_Test, Test_Other_Case)
{
     m_Vr_DictationStatusCheck->GetProductCheck();
     m_Vr_DictationStatusCheck->GetUnitCheck();
     m_Vr_DictationStatusCheck->GetDcmCheck();
}

TEST_F(VR_DictationStatusCheck_Test, OnReqestCommonProperty_Case)
{
    EV_EventContext context;
    RequestVrCommonProperty  CommonProperty;
    CommonProperty.set_sender("vr_fc");
    CommonProperty.set_funccode(RequestVrCommonProperty_VrRequestCommonFunc_ReqDictationStatus);
    VrReqDictationStatus* DictationStatus = CommonProperty.mutable_dictationstatus();
    DictationStatus->set_sender("vr_fc");
    m_Vr_DictationStatusCheck->OnReqestCommonProperty(context, CommonProperty);
    //////////////////////////////
    RequestVrCommonProperty  CommonPerty_no;
    CommonPerty_no.set_sender("vr_fc");
    CommonPerty_no.set_funccode(RequestVrCommonProperty_VrRequestCommonFunc_ReqDictationStatus);
    m_Vr_DictationStatusCheck->OnReqestCommonProperty(context, CommonPerty_no);
    ///////////// set america
    nutshell::NCEnvironment  ncEnvironment;

    SUCCEED();
}
/* EOF */
