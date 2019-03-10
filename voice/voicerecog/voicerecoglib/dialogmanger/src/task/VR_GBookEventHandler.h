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
 * @file VR_GBookEventHandler.h
 * @brief Declaration file of class VR_GBookEventHandler.
 *
 * This file includes the declaration of class VR_GBookEventHandler.
 *
 * @attention used for C++ only.
 */

#ifndef VR_GBOOKEVENTHANDLER_H
#define VR_GBOOKEVENTHANDLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef  VR_DEF_H
#    include  "VR_Def.h"
#endif

#ifndef  VR_MACRO_H
#    include  "VR_Macro.h"
#endif

#include  "nceventsys/NCEventHandler.h"
#include  "nceventsys/NCRunnableLooper.h"
#include  "nceventsys/NCRunnableThread.h"
#include  "nceventsys/NCEvSysContext.h"

class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RunableThread);

class  VR_GBookEventHandler;

/**
 *   @brief   The class of  VR_GBookComfirmHandler
 *
 *   class  Declaration
 */
class VR_GBookComfirmHandler  :  public  nutshell::NCEventHandler
{
public:
    VR_GBookComfirmHandler(VR_GBookEventHandler* pGBookHandler, const nutshell::NCRunnableLooper& looper);
    virtual  ~VR_GBookComfirmHandler();

    void  handleEvent(const nutshell::NCEvent &event);

private:
    VR_GBookEventHandler*     m_GBookHandler;

private:
    VR_GBookComfirmHandler(const VR_GBookComfirmHandler&);
    VR_GBookComfirmHandler& operator =(const VR_GBookComfirmHandler&);
};

/**
 *   @brief  The VR_GBookEventHandler class
 *
 *   class Declaration
 */
class VR_GBookEventHandler
{
public:
    VR_GBookEventHandler(spVR_RunableThread  spRunableThread, spVR_EventSenderIF   spEventSender);
    virtual  ~VR_GBookEventHandler();

public:
    void  OnMessage(const VR_ProtoMessageBase& msg);
    void  OnStart();
    void  OnStop();

private:
    spVR_RunableThread                              m_spRunnableThread;
    spVR_EventSenderIF                              m_Vr_EventSender;
    nutshell::ncsp<VR_GBookComfirmHandler>::sp      m_pcGBookEventHandler;
    nutshell::NCEvSysContext                        m_context;
    VoiceList<nutshell::NCString>::type             m_filter;
};

#endif // VR_GBOOKEVENTHANDLER_H
/* EOF */
