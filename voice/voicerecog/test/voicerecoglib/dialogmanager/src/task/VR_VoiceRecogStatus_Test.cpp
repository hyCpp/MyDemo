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

#include   "stdafx.h"
#include   "VR_VoiceRecogStatus.h"
#include   "gtest/gtest.h"
#include   "VR_DialogEngine_mock.h"
#include   "VR_EventSenderIF.h"
#include   "EV_EventContext.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_VoiceRecogStatus_Test.cpp
*
* The class is just for VR_VoiceRecogStatus test.
*/
class  VR_VoiceRecogStatus_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_VoiceRecogStatus*     m_VR_VoiceRecogStatus;
};


void
VR_VoiceRecogStatus_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_VR_VoiceRecogStatus = VR_new  VR_VoiceRecogStatus(spEngine);
}

void
VR_VoiceRecogStatus_Test::TearDown()
{
    if (m_VR_VoiceRecogStatus) {
        delete  m_VR_VoiceRecogStatus;
        m_VR_VoiceRecogStatus = NULL;
    }
}

TEST_F(VR_VoiceRecogStatus_Test, Start_Case)
{
    m_VR_VoiceRecogStatus->Start();
    SUCCEED();
}

TEST_F(VR_VoiceRecogStatus_Test, Stop_Case)
{
    m_VR_VoiceRecogStatus->Stop();
    SUCCEED();
}

TEST_F(VR_VoiceRecogStatus_Test, OnRecevieMessage_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(HVACFrontScreen);
    actionComfirm->set_processing_result(true);
    m_VR_VoiceRecogStatus->OnRecevieMessage(*actionComfirm);
    SUCCEED();
}

TEST_F(VR_VoiceRecogStatus_Test, GetStatus_Case)
{
    m_VR_VoiceRecogStatus->GetStatus();
    SUCCEED();
}

TEST_F(VR_VoiceRecogStatus_Test, SetStatus_Case)
{
    m_VR_VoiceRecogStatus->SetStatus(true);
    SUCCEED();
}
/* EOF */
