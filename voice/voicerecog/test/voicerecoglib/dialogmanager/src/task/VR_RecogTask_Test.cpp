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
#include   "VR_RecogTask.h"
#include   "gtest/gtest.h"
#include   "VR_RecogTaskState.h"
#include   "EV_EventContext.h"
#include   "VR_DialogEngine_mock.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include   "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_RecogTask_Test.cpp
*
* The class is just for VR_RecogTask test.
*/
class  VR_RecogTask_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_RecogTask*     m_VR_RecogTask;
    VR_RecogTask*     m_VR_RecogTask_1;
};


void
VR_RecogTask_Test::SetUp()
{
    const int nType = 1;
    const VR_String strSession = "012cbc120000546aaa";
    EV_EventContext context;
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    m_VR_RecogTask = VR_new  VR_RecogTask(nType, strSession, context, spEngine);

    m_VR_RecogTask_1 = VR_new  VR_RecogTask(nType, strSession, context, NULL);
}

void
VR_RecogTask_Test::TearDown()
{
    if (m_VR_RecogTask) {
        delete  m_VR_RecogTask;
        m_VR_RecogTask = NULL;
    }
    if (m_VR_RecogTask_1) {
        delete  m_VR_RecogTask_1;
        m_VR_RecogTask_1 = NULL;
    }
}

TEST_F(VR_RecogTask_Test, Start_Case)
{
    m_VR_RecogTask->Start();
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, UserOperation_Case)
{
    EV_EventContext context;
    VR_String       strType = "test";
    m_VR_RecogTask->UserOperation(context, strType);
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, GetTaskId_Case)
{
    m_VR_RecogTask->GetTaskID();
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, GetTaskSession_Case)
{
    m_VR_RecogTask->GetTaskSession();
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, SetTaskComplete_Case)
{
    m_VR_RecogTask->SetComplete();
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, Cancel_Case)
{
    EV_EventContext context;
    m_VR_RecogTask->Cancel(context);
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, OnMessage_Case)
{
    VrEngineControlMessage  engineMessage;
    engineMessage.set_type(DMTaskMessageType_CancelEnd);
    engineMessage.mutable_cancel_end();
    m_VR_RecogTask->OnMessage(engineMessage);
    /////////////////////////////
    VrEngineControlMessage  message1;
    message1.set_type(DMTaskMessageType_StartEnd);
    message1.mutable_start_end();
    m_VR_RecogTask->OnMessage(message1);
    /////////////////////////////
    VrEngineControlMessage  message2;
    message2.set_type(DMTaskMessageType_NotifyResult);
    message1.mutable_notify_result();
    m_VR_RecogTask->OnMessage(message2);
    /////////////////////////////
    VrEngineControlMessage  message3;
    message3.set_type(DMTaskMessageType_NotifyComplete);
    message3.mutable_notify_complete();
    m_VR_RecogTask->OnMessage(message3);
    /////////////////////////////
    VrEngineControlMessage  message4;
    message4.set_type(DMTaskMessageType_RepOperCommand);
    message4.mutable_reply_command();
    m_VR_RecogTask->OnMessage(message4);
    ////////////////////
    VrEngineControlMessage  message5;
    m_VR_RecogTask->OnMessage(message5);
    VrACON  AcOn;
    m_VR_RecogTask->OnMessage(AcOn);
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, SetTaskState_Case)
{
    spVR_RecogTaskStateBase  taskState(VR_new VR_RecogTaskStateIDL(m_VR_RecogTask));
    m_VR_RecogTask->SetTaskState(taskState);
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, SendMessage2Engine_Case)
{
    const VR_String msgToDE = "this is a test for gtest";
    m_VR_RecogTask->SendMessage2Engine(msgToDE);
    m_VR_RecogTask_1->SendMessage2Engine(msgToDE);
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, GetTaskId_1_Case)
{
    m_VR_RecogTask->GetTaskId();
    SUCCEED();
}

TEST_F(VR_RecogTask_Test, Test_For_Other)
{
    std::unique_ptr<VrEngineControlMessage> engineMessage(VrEngineControlMessage::default_instance().New());
    engineMessage->set_type(DMTaskMessageType_CancelEnd);
    engineMessage->mutable_cancel_end();
    m_VR_RecogTask->m_spContext = NULL;
    m_VR_RecogTask->ReplyRequest(engineMessage.release());
}

/* EOF */




























