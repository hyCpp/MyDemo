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
#include   "VR_RecogTaskState.h"
#include   "gtest/gtest.h"
#include   "EV_EventContext.h"
#include   "VR_DialogEngine_mock.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_RecogTaskStateBase_Test.cpp
*
* The class is just for VR_RecogTaskStateBase test.
*/
class  VR_RecogTaskStateBase_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_RecogTaskStateBase*     m_VR_RecogTaskStateBase;
    VR_RecogTaskStateStart*    m_VR_RecogTaskStateStart;
    VR_RecogTaskStateCancel*   m_VR_RecogTaskStateCancel;
    VR_RecogTaskStateWork*     m_VR_RecogTaskStateWork;
};


void
VR_RecogTaskStateBase_Test::SetUp()
{
    const int nType = 1;
    const VR_String  strSession = "010101101010101";
    EV_EventContext  context;
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    VR_RecogTaskIF *pTask(VR_new  VR_RecogTask(nType, strSession, context, spEngine));
    m_VR_RecogTaskStateBase = VR_new  VR_RecogTaskStateBase(pTask);
    m_VR_RecogTaskStateStart = VR_new VR_RecogTaskStateStart(pTask);
    m_VR_RecogTaskStateCancel = VR_new VR_RecogTaskStateCancel(pTask);
    m_VR_RecogTaskStateWork   = VR_new  VR_RecogTaskStateWork(pTask);
}

void
VR_RecogTaskStateBase_Test::TearDown()
{
    if (m_VR_RecogTaskStateBase) {
        delete  m_VR_RecogTaskStateBase;
        m_VR_RecogTaskStateBase = NULL;
    }
    if (m_VR_RecogTaskStateStart) {
        delete m_VR_RecogTaskStateStart;
        m_VR_RecogTaskStateStart = NULL;
    }
    if (m_VR_RecogTaskStateCancel) {
        delete m_VR_RecogTaskStateCancel;
        m_VR_RecogTaskStateCancel = NULL;
    }
    if (m_VR_RecogTaskStateWork) {
        delete m_VR_RecogTaskStateWork;
        m_VR_RecogTaskStateWork = NULL;
    }
}

TEST_F(VR_RecogTaskStateBase_Test, Test_Other_Case)
{
    bool bResult = true;
    m_VR_RecogTaskStateStart->Cancel();
    m_VR_RecogTaskStateStart->OnStarted(bResult);
    m_VR_RecogTaskStateCancel->OnCanceled();
    m_VR_RecogTaskStateWork->Cancel();
    m_VR_RecogTaskStateWork->UserOperation("This is a test");
    m_VR_RecogTaskStateWork->OnNotifyResult();
    m_VR_RecogTaskStateWork->OnNotifyComplete();
    m_VR_RecogTaskStateWork->OnUsrOperationEnd("This is a test");
    SUCCEED();
}


TEST_F(VR_RecogTaskStateBase_Test, Start_Case)
{
    const int nType = 1;
    m_VR_RecogTaskStateBase->Start(nType);
    SUCCEED();
}

TEST_F(VR_RecogTaskStateBase_Test, Cancel_Case)
{
    m_VR_RecogTaskStateBase->Cancel();
    SUCCEED();
}

TEST_F(VR_RecogTaskStateBase_Test, UserOperation_Case)
{
    const  VR_String  strType = "abc";
    m_VR_RecogTaskStateBase->UserOperation(strType);
    SUCCEED();
}

TEST_F(VR_RecogTaskStateBase_Test, OnStarted_Case)
{
    bool bResult = true;
    m_VR_RecogTaskStateBase->OnStarted(bResult);
    SUCCEED();
}

TEST_F(VR_RecogTaskStateBase_Test, OnCanceled_Case)
{
    m_VR_RecogTaskStateBase->OnCanceled();
    SUCCEED();
}

TEST_F(VR_RecogTaskStateBase_Test, OnNotifyResult_Case)
{
    m_VR_RecogTaskStateBase->OnNotifyResult();
    SUCCEED();
}

TEST_F(VR_RecogTaskStateBase_Test, OnNotifyComplete_Case)
{
    m_VR_RecogTaskStateBase->OnNotifyComplete();
    SUCCEED();
}

TEST_F(VR_RecogTaskStateBase_Test, OnUsrOperationEnd_Case)
{
    const VR_String strType = "test";
    m_VR_RecogTaskStateBase->OnUsrOperationEnd(strType);
    SUCCEED();
}

TEST_F(VR_RecogTaskStateBase_Test, GetRecogTask_Case)
{
    m_VR_RecogTaskStateBase->GetRecogTask();
    SUCCEED();
}
/* EOF */

