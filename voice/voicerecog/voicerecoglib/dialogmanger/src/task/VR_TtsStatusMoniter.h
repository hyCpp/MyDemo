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
 * @file VR_TtsStatusMoniter.h
 * @brief Declaration file of class VR_TtsStatusMoniter.
 *
 * This file includes the declaration of class VR_TtsStatusMoniter.
 *
 * @attention used for C++ only.
 */

#ifndef VR_TTSSTATUSMONITER_H
#define VR_TTSSTATUSMONITER_H

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

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RunableThread);

class VR_TtsStatusMoniter;

/**
 * @brief The VR_TtsEventHandler class
 *
 * class declaration
 */
class VR_TtsEventHandler : public nutshell::NCEventHandler
{
public:
    VR_TtsEventHandler(VR_TtsStatusMoniter* pTtsStatusHandle,  const nutshell::NCRunnableLooper& looper);
    virtual ~VR_TtsEventHandler();
    void  handleEvent(const nutshell::NCEvent &event);

private:
    VR_TtsStatusMoniter*     m_pTtsStatusMoniter;
    bool                     m_bTtsCrash;

private:
    VR_TtsEventHandler(const VR_TtsEventHandler&);
    VR_TtsEventHandler& operator=(const VR_TtsEventHandler&);
};

/**
 * @brief The VR_TtsStatusMoniter class
 *
 * class declaration
 */
class VR_TtsStatusMoniter
{
public:
    VR_TtsStatusMoniter(spVR_RunableThread  spRunnableThread);
    virtual ~VR_TtsStatusMoniter();

public:
    virtual void Start();
    virtual void Stop();

protected:
    spVR_RunableThread                                   m_spRunnableThread;
    nutshell::ncsp<VR_TtsEventHandler>::sp               m_pcEventHandler;
    nutshell::NCEvSysContext                             m_context;
    VoiceList<nutshell::NCString>::type                  m_filter;
};

#endif // VR_TTSSTATUSMONITER_H
/* EOF */
