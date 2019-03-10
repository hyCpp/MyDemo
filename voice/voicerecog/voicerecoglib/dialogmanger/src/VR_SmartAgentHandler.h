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
 * @file VR_StartServiceCondition.h
 * @brief Declaration file of class VR_StartServiceCondition.
 *
 * This file includes the declaration of class VR_StartServiceCondition.
 *
 * @attention used for C++ only.
 */

#ifndef VR_SMARTAGENTHANDLER_H
#define VR_SMARTAGENTHANDLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "nceventsys/NCEventHandler.h"
#include "nceventsys/NCRunnableLooper.h"
#include "nceventsys/NCRunnableThread.h"
#include "nceventsys/NCEvSysContext.h"
#include "VR_DialogEngineIF.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RunableThread);

enum  UserDataType
{
    ADD_USER    =  0,
    DELETE_USER  = 1,
    CHANGE_USER  = 2
};

class VR_SmartAgentHandler;

/**
 * @brief The VR_DriverSettingEventHandler class
 *
 * class declaration
 */
class VR_DriverSettingEventHandler : public nutshell::NCEventHandler
{
public:
    VR_DriverSettingEventHandler(VR_SmartAgentHandler* pAgentHandle,  const nutshell::NCRunnableLooper& looper);
    virtual ~VR_DriverSettingEventHandler();
    void  handleEvent(const nutshell::NCEvent &event);

private:
    VR_SmartAgentHandler*     m_pSmartAgent;

private:
    VR_DriverSettingEventHandler(const VR_DriverSettingEventHandler&);
    VR_DriverSettingEventHandler& operator=(const VR_DriverSettingEventHandler&);
};

/**
 * @brief The VR_SmartAgentHander class
 *
 * class declaration
 */
class VR_SmartAgentHandler
{
public:
    VR_SmartAgentHandler(spVR_RunableThread spRunableThread, spVR_DialogEngineIF dialogEngine);
    virtual ~VR_SmartAgentHandler();

public:
    void OnMessage(const VR_ProtoMessageBase& msg);
    void NotifyDftUser();
    void Start();
    void Stop();

private:
    spVR_RunableThread                                   m_spRunableThread;
    spVR_DialogEngineIF                                  m_spDialogEngine;
    nutshell::ncsp<VR_DriverSettingEventHandler>::sp     m_pcEventHandler;
    nutshell::NCEvSysContext                             m_context;
    VoiceList<nutshell::NCString>::type                  m_filter;
};

#endif // VR_SMARTAGENTHANDLER_H
/* EOF */
