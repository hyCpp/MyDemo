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
 * @file VR_PhoneCallResultHandler.h
 * @brief Declaration file of class VR_PhoneCallResultHandler.
 *
 * This file includes the declaration of class VR_PhoneCallResultHandler.
 *
 * @attention used for C++ only.
 */
#ifndef VR_PHONECALLRESULTHANDLER_H
#define VR_PHONECALLRESULTHANDLER_H

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

class  VR_PhoneCallResultHandler;

/**
 *  @brief  The  VR_PhoneCallEventHandler class
 *
 *
 *   class  Declaration
 */
class  VR_PhoneCallEventHandler : public nutshell::NCEventHandler
{
public:
    VR_PhoneCallEventHandler(VR_PhoneCallResultHandler* pResultHandler, const nutshell::NCRunnableLooper& looper);
    virtual  ~VR_PhoneCallEventHandler();

    void  handleEvent(const nutshell::NCEvent& event);

private:
    VR_PhoneCallResultHandler*       m_pResultHandler;

private:
    VR_PhoneCallEventHandler(const VR_PhoneCallEventHandler&);
    VR_PhoneCallEventHandler& operator=(const VR_PhoneCallEventHandler&);

    bool    m_bPhoneStandBy;
    bool    m_bPhoneCall;
};

/**
 *  @brief  The  VR_PhoneCallResultHandler class
 *
 *
 *  class Declaration
 */
class   VR_PhoneCallResultHandler
{
public:
    VR_PhoneCallResultHandler(spVR_RunableThread  spRunableThread, spVR_DialogEngineIF  spDialogEngine);
    virtual  ~VR_PhoneCallResultHandler();

public:
    void  OnMessage(const VR_ProtoMessageBase& msg);
    void  SendMessage();
    void  OnStart();
    void  OnStop();

private:
    spVR_RunableThread                                 m_spRunableThread;
    spVR_DialogEngineIF                                m_spDialogEngine;
    nutshell::ncsp<VR_PhoneCallEventHandler>::sp       m_spPhoneCallEventHandler;
    nutshell::NCEvSysContext                           m_context;
    VoiceList<nutshell::NCString>::type                m_filter;
};

#endif // VR_PHONECALLRESULTHANDLER_H
/* EOF */
