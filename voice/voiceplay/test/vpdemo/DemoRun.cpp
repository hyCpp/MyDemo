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

#include "stdafx.h"
#include "DemoRun.h"
using ::nutshell::NCApplicationProcess;
using namespace ::navi::VoicePlay;
using namespace std;

DemoRun::DemoRun() : NCApplicationProcess("VPDemo")
{
}

DemoRun::~DemoRun()
{
}

void DemoRun::getBaseConfig(BASECONFIG& baseConfig)
{
    baseConfig.bHasIDL = CL_TRUE;
    baseConfig.processName = "VPDemo";
    baseConfig.eventReceiverName = "VPDemo";
    baseConfig.threadPoolConfig = 
    {
        BL_THREAD_PRI_APL_MIDDLE,
        5,
        20,
        10,
        2048,
        1000 * 5,
        BL_TASK_SCHEDULE_FIFO
    };

    baseConfig.eventConfig.Add(EV_PROXY_IPC, BL_PROCESSNAME_TTS);
}

void DemoRun::OnInitialize() 
{
    getBaseConfig(baseConfig);
    system.Initialize(baseConfig.processName);
    BL_AplSystem::InitThreadTable();

    EV_EventSystem::Instance()->Initialize(baseConfig.processName, baseConfig.eventConfig);
    EV_EventSystem::Instance()->Start();

    BL_ThreadPool::Initialize(baseConfig.threadPoolConfig);

    system.InitPath();
    system.Start();
}

void DemoRun::OnStart()
{
    DemoUtils::loadRegion();
    DemoUtils::loadLanguage();

    instance = VP_ModuleIF::Instance();
    instance->Initialize();    

    playControl = VP_Factory::Instance()->AcquirePlayControl();
    channel = playControl->getChannel("VPDemo");
    playSoundList = DemoUtils::createDemoPlayList();
    request = make_shared<DemoRequest>(playSoundList);
}

void DemoRun::OnStop()
{
    instance->Stop();

    EV_EventSystem::Instance()->Stop();
    EV_EventSystem::Instance()->Destroy();

    BL_ThreadPool::UnInitialize();
    system.Uninitialize();
}

void DemoRun::Restart()
{
    instance->Stop();
    OnStart();
}

void DemoRun::OnDeinitialize()
{
}

void DemoRun::Run(vector<string> commands)
{
    auto itr = commands.begin();
    for (; itr != commands.end(); ++itr) {
        shared_ptr<VP_Event> event = getEvent(*itr);
        playControl->PostEvent(event);
    }
}

shared_ptr<VP_Event> DemoRun::getEvent(string cmd)
{
    shared_ptr<VP_Event> event;
    if ("COPENG" == cmd) {
        event = request->getPlayListEvent(EV_MODULE_GUIDE);
        sender = EV_MODULE_GUIDE;
    }
    else if ("COPENT" == cmd) {
        event = request->getPlayListEvent(EV_MODULE_TRAFFIC, VP_SS_TRAFFIC_GUIDANCE);
        sender = EV_MODULE_TRAFFIC;
    }
    else if ("COPENTI" == cmd) {
        event = request->getPlayListEvent(EV_MODULE_TRAFFIC, VP_SS_TRAFFIC_INFORMATION_GUIDANCE);
        sender = EV_MODULE_TRAFFIC;
    }
    else if ("COPENN" == cmd) {
        event = request->getPlayListEvent(EV_MODULE_POINT);
    }
    else if ("PSTOP" == cmd) {
        event = make_shared<VP_PlayerStopEvent>(playControl->getChannel(sender));
    }
    else if ("PSTOPPED" == cmd) {
        event = make_shared<VP_PlayerStoppedEvent>(channel);
    }
    else if ("COPENED" == cmd) {
        event = make_shared<VP_ChannelOpenedEvent>(channel);
    }
    else if ("CCLOSED" == cmd) {
        event = make_shared<VP_ChannelClosedEvent>(channel);
    }
    else if ("CSTOP" == cmd) {
        event = make_shared<VP_ChannelStopEvent>(channel);
    }
    else if ("CWAIT" == cmd) {
        event = make_shared<VP_ChannelWaitingEvent>(channel);
    }
    else if ("SLEEP1" == cmd) {
        usleep(100000);
    }
    else if ("SLEEP2" == cmd) {
        usleep(200000);
    }
    else if ("SLEEP5" == cmd) {
        usleep(500000);
    }
    else if ("SLEEP10" == cmd) {
        sleep(1);
    }
    else if ("SLEEP20" == cmd) {
        sleep(2);
    }
    else if ("SLEEP50" == cmd) {
        sleep(5);
    }
    else if ("SLEEP100" == cmd) {
        sleep(10);
    }
    else if ("RESTART" == cmd) {
        Restart();
    }
    else {
        VP_LOGD("Unsupported command!\n");
    }

    return event;
}

/* EOF */
