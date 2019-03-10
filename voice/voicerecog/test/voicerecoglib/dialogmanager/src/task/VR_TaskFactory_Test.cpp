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
#include   "VR_TaskFactory.h"
// #include   "VR_AvcListenerIF.h"
#include   "gtest/gtest.h"
#include   "VR_DialogEngine_mock.h"
// #include   "VR_AvcLanSender_mock.h"
#include   "VR_EventSenderIF.h"
#include   "VR_SessionManger.h"
#include   "EV_EventContext.h"
#include   "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_TaskFactory_Test.cpp
*
* The class is just for VR_TaskFactory test.
*/
class  VR_TaskFactory_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_TaskFactory*     m_VR_TaskFactory;
};


void
VR_TaskFactory_Test::SetUp()
{
    m_VR_TaskFactory = VR_new  VR_TaskFactory();
}

void
VR_TaskFactory_Test::TearDown()
{
    if (m_VR_TaskFactory) {
        delete  m_VR_TaskFactory;
        m_VR_TaskFactory = NULL;
    }
}

TEST_F(VR_TaskFactory_Test, Test_Other_Case)
{
     spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
     m_VR_TaskFactory->CreateGViewProxy(spEngine);

     spVR_EventSenderIF  spEventSender(VR_EventSenderIF::GetSender());

     spVR_RunableThread spThread = spVR_RunableThread(VR_new  VR_RunableThread("gTest", false));
     m_VR_TaskFactory->CreateStartGBook(spThread, spEventSender, spEngine);
}

TEST_F(VR_TaskFactory_Test, CreateDictationTask_Case)
{
   //  spVR_AvcLanSenderIF spAvcSender(VR_new VR_AvcLanSender_mock());
 //   spVR_EventSenderIF  spEventSender(VR_EventSenderIF::GetSender());
 //   m_VR_TaskFactory->CreateDictationTask(spEventSender);
    SUCCEED();
}

TEST_F(VR_TaskFactory_Test, CreateRecogTask_Case)
{
  //  const int nType = 1;
  //  const VR_String   strSession  = "0101010100001111";
  //  EV_EventContext   context;
  //  spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
  //  m_VR_TaskFactory->CreateRecogTask(nType, strSession, context, spEngine);
    SUCCEED();
}

TEST_F(VR_TaskFactory_Test, CreateTslApp_Case)
{
    // spVR_AvcLanSenderIF spAvcSender(VR_new VR_AvcLanSender_mock());
    // spVR_EventSenderIF  spEventSender(VR_EventSenderIF::GetSender());
    // spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    // spVR_SessionManger  spSessionManger(VR_new  VR_SessionManger());
    // spVR_SessionManger  spSessionManger;
    // m_VR_TaskFactory->CreateTslApp(spAvcSender, spEventSender, spEngine, spSessionManger);
    SUCCEED();
}

TEST_F(VR_TaskFactory_Test, CreateStartService_Case)
{
   // spVR_AvcLanSenderIF spAvcSender(VR_new VR_AvcLanSender_mock());
   // spVR_EventSenderIF  spEventSender(VR_EventSenderIF::GetSender());
   // spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
   // m_VR_TaskFactory->CreateStartService(spAvcSender, spEventSender, spEngine);
    SUCCEED();
}

TEST_F(VR_TaskFactory_Test, CreateVoiceStatus_Case)
{
   // spVR_AvcLanSenderIF spAvcSender(VR_new VR_AvcLanSender_mock());
   // spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
   // m_VR_TaskFactory->CreateVoiceStatus(spAvcSender, spEngine);
    SUCCEED();
}
/* EOF */
