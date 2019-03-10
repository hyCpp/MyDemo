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
#include   "gtest/gtest.h"
#include   "VR_DialogEngine_mock.h"
#include   "eventmanger/VR_EngineEventSenderIF.h"
#include   "EV_EventContext.h"
#include   "VR_EventSender.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_EventSender_Test.cpp
*
* The class is just for VR_EventSender test.
*/
class  VR_EventSender_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_EventSender*     m_VR_EventSender;
};


void
VR_EventSender_Test::SetUp()
{
    m_VR_EventSender = VR_new  VR_EventSender();
}

void
VR_EventSender_Test::TearDown()
{
    if (m_VR_EventSender) {
        delete  m_VR_EventSender;
        m_VR_EventSender = NULL;
    }
}

TEST_F(VR_EventSender_Test, SendEvent_Case)
{
    const char* msgName = "this is only a test";
     std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
     actionComfirm->set_action_id(HVACFrontScreen);
     actionComfirm->set_processing_result(true);
     m_VR_EventSender->SendEvent(msgName, actionComfirm.release());
    SUCCEED();
}

TEST_F(VR_EventSender_Test, Reply_Case)
{
     EV_EventContext context;
     std::unique_ptr<VrRecogResultConfirm>  actionComfirm(VrRecogResultConfirm::default_instance().New());
     actionComfirm->set_action_id(HVACFrontScreen);
     actionComfirm->set_processing_result(true);
     m_VR_EventSender->Reply(context, actionComfirm.release());
    SUCCEED();
}
/* EOF */

