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

#include <memory>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "BL_AplSystem.h"
#include "CL_WaitObj.h"
#include "EV_EventSystem.h"
#include "VP_VoicePlayControl.h"
#include "VP_ChannelManager_mock.h"
#include "VP_SoundPlayer_mock.h"
#include "VP_DataManager_mock.h"
#include "VP_ModuleIF.h"

using namespace std;
using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::Mock;

#define T_LOG(...)    printf(__VA_ARGS__);\
                    printf("\n");

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    // testing::InitGoogleMock(&argc, argv);
    // Init App enviroment
    BL_AplSystem system;
    system.Initialize("VP");
   
    EV_EventConfigure config;
    EV_EventSystem::Instance()->Initialize("VP", config);

    EV_EventSystem::Instance()->Start();

    // Init VP module 
    T_LOG("Test::Instance begin ...\n");

    int counts = 0;
    shared_ptr<MockVP_ChannelManager> channelMgr = make_shared<MockVP_ChannelManager>();
    VP_ModuleIF *module = VP_ModuleIF::Instance();
    module->m_channelMgr = channelMgr;
    EXPECT_CALL(*channelMgr, registerChannel(_)).WillRepeatedly(Return(true));

    module->Initialize();
    counts =  RUN_ALL_TESTS();

    T_LOG("Test::Instance end ...\n");

    module->Stop();
    module->m_pPlayControl->m_channelMgr = nullptr;
    delete module;
    channelMgr = nullptr;

    return counts;
}

/* EOF */
