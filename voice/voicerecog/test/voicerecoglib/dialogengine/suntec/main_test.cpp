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

#include "debugger/crash_handler.h"

#include "BL_AplSystem.h"
#include "BL_ThreadPool.h"
#include "AplThreadName.h"
// #include "EV_EventSystem.h"
// #include "EV_EventConfigure.h"

#include <sys/system_properties.h>

int main(int argc, char* argv[])
{
    __system_properties_init();

    crash_handler_register();

    testing::InitGoogleTest(&argc, argv);
    BL_AplSystem cAplSystem;
    cAplSystem.InitPath();

    cAplSystem.Initialize("VR");
    // BL_AplSystem::InitThreadTable();

    // BL_ThreadPoolConfig config =
    // {
    //     BL_THREAD_PRI_APL_MIDDLE,  // thread priority
    //     4,  // minimum thread count in a threadpool
    //     8,  // maximum thread count in a threadpool
    //     16,  // default thread count in a threadpool
    //     1024,  // max size of wait task list
    //     1000*5,  // keep alive time for an idle thread
    //     BL_TASK_SCHEDULE_FIFO  // FIFO, IFO or RIO
    // };
    // BL_ThreadPool::Initialize(config);

    cAplSystem.Start();


    // eventsys initialize
    // EV_EventConfigure config_ev;
    // EV_EventSystem::Instance()->Initialize("VR", config_ev);
    // EV_EventSystem::Instance()->Start();
    // usleep(1000*1000);

    int count = RUN_ALL_TESTS();

    // stop EV_EventSystem
    // EV_EventSystem::Instance()->Stop();
    // EV_EventSystem::Instance()->Destroy();

    // stop BL_ThreadPool
    // BL_ThreadPool::UnInitialize();

    // stop BL_AplSystem
    cAplSystem.Stop();
    cAplSystem.Uninitialize();

    return count;
}

/* EOF */