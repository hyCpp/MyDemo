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

#include "VR_AvcLanSenderIF.h"
#include "VR_AppsStartAction.h"
#include "VR_ActionEventSender.h"
#include "VR_DialogEngine_mock.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_AppsStartAction_Test
*
* The class is just for VR_AppsStartAction test.
*/
class VR_AppsStartAction_Test : public testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

public:
    spVR_ActionEventSender m_sp_EventSender;
    VR_AppsStartAction *m_appStartAction;
};

void VR_AppsStartAction_Test::SetUp()
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrStartApps> protoMsg(VrStartApps::default_instance().New());
    protoMsg->set_application_id(1);
    protoMsg->set_data_size(0);
    m_appStartAction = VR_new VR_AppsStartAction(m_sp_EventSender, 1, StartApps, *protoMsg);
}

void VR_AppsStartAction_Test::TearDown()
{
    if (NULL != m_appStartAction) {
        delete m_appStartAction;
        m_appStartAction = NULL;
    }
}

TEST_F(VR_AppsStartAction_Test, Run_StartApps)
{
    m_appStartAction->Run();
    SUCCEED();
}

TEST_F(VR_AppsStartAction_Test, Run_StartApps_DataSize_2)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrStartApps> protoMsg(VrStartApps::default_instance().New());
    protoMsg->set_application_id(1);
    protoMsg->set_data_size(2);
    protoMsg->set_data_content("12");
    m_appStartAction = VR_new VR_AppsStartAction(m_sp_EventSender, 1, StartApps, *protoMsg);
    m_appStartAction->Run();
    SUCCEED();
}

TEST_F(VR_AppsStartAction_Test, Run_StartApps_NoData)
{
    spVR_DialogEngineIF spEngine(VR_new VR_DialogEngine_mock);
    spVR_ActionEventSender m_sp_EventSender(VR_new VR_ActionEventSender(spEngine));
    std::unique_ptr<VrStartApps> protoMsg(VrStartApps::default_instance().New());
    protoMsg->set_application_id(1);
    protoMsg->set_data_size(2);
    m_appStartAction = VR_new VR_AppsStartAction(m_sp_EventSender, 1, StartApps, *protoMsg);
    m_appStartAction->Run();
    SUCCEED();
}

TEST_F(VR_AppsStartAction_Test, Run_default)
{
    m_appStartAction->m_ActionType = CallHistory;
    m_appStartAction->Run();
    SUCCEED();
}

TEST_F(VR_AppsStartAction_Test, OnReceive_Case)
{
    std::unique_ptr<VrRecogResultConfirm> protoMsg(VrRecogResultConfirm::default_instance().New());
    protoMsg->set_action_id(StartApps);
    protoMsg->set_processing_result(true);

    m_appStartAction->OnReceive(*protoMsg);
    SUCCEED();
}

/* EOF */
