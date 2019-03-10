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
#include <sys/system_properties.h>
#include "gtest/gtest.h"
#include "BL_AplSystem.h"
#include "EV_EventSystem.h"
#include "ncapp/NCApplicationProcess.h"

int main(int argc, char* argv[])
{
  //  __system_properties_init();
    testing::InitGoogleTest(&argc, argv);
    // Init App enviroment
    BL_AplSystem system;
    system.Initialize("VR");
    system.Start();

    EV_EventConfigure config;
    EV_EventSystem::Instance()->Initialize("VR", config);
    // EV_EventSystem::Instance()->Start();

    // we may run some other appliacation
    // so start it
    nutshell::NCApplicationProcess   ncAppProcess("VR_AppProcessStart");
    ncAppProcess.initialize();
    ncAppProcess.start();


    int count = RUN_ALL_TESTS();

    // EV_EventSystem::Instance()->Stop();
    // EV_EventSystem::Destroy();
    ncAppProcess.stop();
    system.Stop();
    system.Uninitialize();
    return count;
}

TEST(vr_recog, test)
{
    ASSERT_EQ(0, 0);
    ASSERT_EQ(1, 1);
}

/* EOF */
