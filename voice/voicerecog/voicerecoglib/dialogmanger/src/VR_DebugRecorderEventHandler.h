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
 * @file VR_DebugRecorderEventHandler.h
 * @brief Declaration file of class VR_DebugRecorderEventHandler.
 *
 * This file includes the declaration of class VR_DebugRecorderEventHandler.
 *
 * @attention used for C++ only.
 */
#ifndef VR_DEBUGRECORDEREVENTHANDLER_H
#define VR_DEBUGRECORDEREVENTHANDLER_H

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

class  VR_DebugRecorderEventHandler;
#ifdef HAVE_NUTSHELL_OS
const  nutshell::NCString  DialogServiceSpaceName   =  nutshell::NCString("diagservice.mmdiag");
const  nutshell::NCString  Dialog_Event_Copy        =  nutshell::NCString("event_copy");
const  nutshell::NCString  Dialog_Event_Delete      =  nutshell::NCString("event_delete");

const  nutshell::NCString  VoiceRecogDialogSpace    =  nutshell::NCString("voicerecog.debugrecorder");
const  nutshell::NCString  Voice_Event_Copy         =  nutshell::NCString("event_copyready");
const  nutshell::NCString  Voice_Event_Delete       =  nutshell::NCString("event_deletefinished");
#endif

/**
 * @brief  The  VR_DiagEventHandler class
 *
 *  class  Declaration
 */
class  VR_DiagEventHandler : public nutshell::NCEventHandler
{
public:
    VR_DiagEventHandler(VR_DebugRecorderEventHandler* pDebugHandler, const nutshell::NCRunnableLooper& looper);
    virtual  ~VR_DiagEventHandler();

    virtual void  handleEvent(const nutshell::NCEvent& event);

private:
    VR_DebugRecorderEventHandler*     m_pDebugEventHandler;

private:
    VR_DiagEventHandler(const  VR_DiagEventHandler&);
    VR_DiagEventHandler& operator=(const VR_DiagEventHandler&);
};

/**
 *   @brief  The  VR_DebugRecorderEventHandler class
 *
 *   class  Declaration
 */
class  VR_DebugRecorderEventHandler
{
public:
    VR_DebugRecorderEventHandler(spVR_RunableThread  spRunableThread, spVR_DialogEngineIF   spDialogEngine);
    virtual    ~VR_DebugRecorderEventHandler();

public:
    virtual void  OnMessage(const VR_ProtoMessageBase& msg);
    virtual void  OnStart();
    virtual void  OnStop();

protected:
    spVR_RunableThread                            m_spRunableThread;
    spVR_DialogEngineIF                           m_spDialogEngine;
    nutshell::ncsp<VR_DiagEventHandler>::sp       m_spDiagEventHandler;
    nutshell::NCEvSysContext                      m_context;
    VoiceList<nutshell::NCString>::type           m_filter;
};

#endif // VR_DEBUGRECORDEREVENTHANDLER_H
/* EOF */
