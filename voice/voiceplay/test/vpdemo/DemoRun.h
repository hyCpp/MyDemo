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

/**
 * @file DemoRun.h                                            
 * @brief Summary for DemoRun.
 *
 * This file includes the declaration of class DemoRun and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef DEMO_RUN_H
#define DEMO_RUN_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "stdafx.h"
#include "ncapp/NCApplicationProcess.h"

#include "InstanceManager.h"
#include "EV_EventSystem.h"
#include "BL_AplSystem.h"
#include "BL_AplInfo.h"
#include "AplThreadName.h"
#include "BL_AplSystem.h"
#include "PublicInc.h"
#include "VP_ModuleIF.h"
#include "VP_Event.h"
#include "DemoRequest.h"
#include "DemoUtils.h"

typedef InstanceManager::BaseConfig     BASECONFIG;

/**
 * Summary for class DemoRun
 *
 * There should be a detail description for
 * class DemoRun
 */
class DemoRun : public nutshell::NCApplicationProcess
{
public:
    DemoRun();

    virtual ~DemoRun();

    virtual void OnInitialize();

    virtual void OnStart();

    virtual void OnStop();

    virtual void OnDeinitialize();

    void Restart();

    void Run(std::vector<std::string> commands);

private:
    std::shared_ptr<VP_Event> getEvent(std::string cmd);
    void getBaseConfig(BASECONFIG& baseConfig);

private:
    BL_AplSystem system;
    VP_ModuleIF *instance;
    BASECONFIG baseConfig;
    ::navi::VoicePlay::VpPlaySoundList *playSoundList;
    std::shared_ptr<DemoRequest> request;
    std::shared_ptr<VP_VoicePlayControl> playControl;
    std::shared_ptr<VP_Channel> channel;
    std::shared_ptr<VP_DataAccessorList> pAccessorList;
    std::string sender;
};

#endif
/* EOF */
