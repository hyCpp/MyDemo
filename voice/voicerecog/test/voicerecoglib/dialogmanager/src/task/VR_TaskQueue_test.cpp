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

#include "gtest/gtest.h"

#include "VR_TaskQueue.h"
// #include "VR_DialogTaskBase.h"
#include "VR_RecogTask.h"
#include "VR_DialogEngine_mock.h"
#include "VR_DialogTaskBase_mock.h"

#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"

using ::testing::AtLeast;
using namespace navi::VoiceRecog;

/**
* VR_TaskQueue_Test
*
* The class is just for VR_VoiceBoxAgentApps test.
*/
class VR_TaskQueue_Test : public testing::Test
{
protected:

    virtual void SetUp();
    virtual void TearDown();

    VR_TaskQueue *m_vr_taskqueue;
};

void
VR_TaskQueue_Test::SetUp()
{
     m_vr_taskqueue = VR_new VR_TaskQueue();
}

void
VR_TaskQueue_Test::TearDown()
{
    if (NULL != m_vr_taskqueue) {
        delete m_vr_taskqueue;
        m_vr_taskqueue = NULL;
    }    
}

TEST_F(VR_TaskQueue_Test, Task_Queue_Case)
{
    EV_EventContext context;
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_DialogTaskBase dialogTask(VR_new VR_RecogTask(0, "1111111", context, spEngine));
    m_vr_taskqueue->PushTask(dialogTask);
    ////////////////////////////
    m_vr_taskqueue->FindTaskByID(0);
    m_vr_taskqueue->FindTaskByID(1);
    m_vr_taskqueue->FindTopTask();
    ////////////////////
    VrEngineControlMessage  engineMessage;
    engineMessage.set_type(DMTaskMessageType_CancelEnd);
    engineMessage.mutable_cancel_end();
    m_vr_taskqueue->OnMessage(engineMessage);
    //////////////
    dialogTask->SetComplete();
    ///////////////////
    SUCCEED();
}

TEST_F(VR_TaskQueue_Test, Test_Case)
{
    VR_TaskQueue   taskQueue;
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_DialogTaskBase dialogTask(VR_new VR_RecogTask(0, "test", EV_EventContext(), spEngine));
    dialogTask->SetComplete();
    taskQueue.PushTask(dialogTask);

    taskQueue.m_TaskList.clear();
    VrEngineControlMessage  controlMessage;
    controlMessage.set_type(DMTaskMessageType_CancelEnd);
    controlMessage.mutable_cancel_end();
    taskQueue.OnMessage(controlMessage);

    spVR_DialogTaskBase  dialogTask1(VR_new VR_RecogTask(0, "test", EV_EventContext(), spEngine));
    dialogTask1->SetComplete();
    taskQueue.m_TaskList.push_back(dialogTask1);
    taskQueue.OnMessage(controlMessage);

    spVR_DialogTaskBase  dialogTask2(VR_new VR_RecogTask(0, "test", EV_EventContext(), spEngine));
    taskQueue.m_TaskList.push_back(dialogTask2);
    spVR_DialogTaskBase  dialogTask3(VR_new VR_RecogTask(0, "test", EV_EventContext(), spEngine));
    taskQueue.m_TaskList.push_back(dialogTask3);
    taskQueue.OnMessage(controlMessage);
}

TEST_F(VR_TaskQueue_Test, Constructure_Case)
{
    VR_TaskQueue* pcvr_taskqueue = VR_new VR_TaskQueue();
    delete pcvr_taskqueue;
    SUCCEED();
}

TEST_F(VR_TaskQueue_Test, FindTaskByID_Case)
{
    m_vr_taskqueue->FindTaskByID(0);
    m_vr_taskqueue->FindTaskByID(1);
    SUCCEED();
}

TEST_F(VR_TaskQueue_Test, FindTopTask_Case)
{
    m_vr_taskqueue->FindTopTask();
    SUCCEED();
}
/* EOF */
