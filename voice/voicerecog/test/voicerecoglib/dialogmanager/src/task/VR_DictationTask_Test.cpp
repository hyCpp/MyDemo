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
#include   "VR_DictationTask.h"
#include   "gtest/gtest.h"
#include   "EV_EventContext.h"
#include    "VR_DialogEngine_mock.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_DictationTask_Test.cpp
*
* The class is just for VR_DictationTask test.
*/
class  VR_DictationTask_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_DictationTask*     m_Vr_DictationTask;
};


void
VR_DictationTask_Test::SetUp()
{
    EV_EventContext context;
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_Vr_DictationTask = VR_new  VR_DictationTask(1, context, spEngine);
}

void
VR_DictationTask_Test::TearDown()
{
    if (m_Vr_DictationTask) {
        delete  m_Vr_DictationTask;
        m_Vr_DictationTask = NULL;
    }
}

TEST_F(VR_DictationTask_Test, Start_Case)
{
    m_Vr_DictationTask->Start();
    SUCCEED();
}

TEST_F(VR_DictationTask_Test, Cancel_Case)
{
    EV_EventContext context;
    m_Vr_DictationTask->Cancel(context);
    SUCCEED();
}

TEST_F(VR_DictationTask_Test, OnMessage_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(HVACFrontScreen);
    actionComfirm->set_processing_result(true);
    m_Vr_DictationTask->OnMessage(*actionComfirm);
    SUCCEED();
}

TEST_F(VR_DictationTask_Test, OnNotifyDictationResult_Case)
{
    m_Vr_DictationTask->OnNotifyDictationResult();
    SUCCEED();
}

TEST_F(VR_DictationTask_Test, OnNotifyState_Case)
{
    std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
    actionComfirm->set_action_id(HVACFrontScreen);
    actionComfirm->set_processing_result(true);
    m_Vr_DictationTask->OnNotifyState(*actionComfirm);
    SUCCEED();
}
/* EOF */
