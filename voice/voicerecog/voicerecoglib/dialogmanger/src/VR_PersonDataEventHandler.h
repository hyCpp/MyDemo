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
 * @file VR_PersonDataEventHandler.h
 * @brief Declaration file of class VR_PersonDataEventHandler.
 *
 * This file includes the declaration of class VR_PersonDataEventHandler.
 *
 * @attention used for C++ only.
 */

#ifndef VR_PERSONDATAEVENTHANDLER_H
#define VR_PERSONDATAEVENTHANDLER_H

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

class  VR_PersonDataEventHandler;

/**
 *  @brief  The  VR_PersonSettingEventHandler  class
 *
 *   class  declaration
 */
class  VR_PersonSettingEventHandler  :  public  nutshell::NCEventHandler
{
public:
    VR_PersonSettingEventHandler(VR_PersonDataEventHandler* pEventHandler, const nutshell::NCRunnableLooper& looper);
    virtual  ~VR_PersonSettingEventHandler();
    void   handleEvent(const nutshell::NCEvent&  event);

private:
    VR_PersonDataEventHandler          *m_pPersonHandler;

private:
    VR_PersonSettingEventHandler(const  VR_PersonSettingEventHandler&);
    VR_PersonSettingEventHandler&  operator=(const VR_PersonSettingEventHandler&);
};

/**
 *  @brief  The  VR_PersonDataEventHandler class
 *
 *  class declaration
 */
class  VR_PersonDataEventHandler
{
public:
    VR_PersonDataEventHandler(spVR_RunableThread  spRunableThread, spVR_DialogEngineIF  spDialogEngine);
    virtual  ~VR_PersonDataEventHandler();

public:
    void  OnMessage(const VR_ProtoMessageBase&  msg);
    void  OnStart();
    void  OnStop();

private:
    spVR_RunableThread                                  m_spRunableThread;
    spVR_DialogEngineIF                                 m_spDialogEngine;
    nutshell::ncsp<VR_PersonSettingEventHandler>::sp    m_pcEventHandler;
    nutshell::NCEvSysContext                            m_context;
    VoiceList<nutshell::NCString>::type                 m_filter;
};

#endif // VR_PERSONDATAEVENTHANDLER_H
/* EOF */
