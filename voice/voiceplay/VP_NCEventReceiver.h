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
 * @file VP_NCEventHandler.h                                            
 * @brief Summary for VP_NCEventHandler.
 *
 * This file includes the declaration of class VP_NCEventHandler and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_NCEVENTHANDLER_H
#define VP_NCEVENTHANDLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>

#ifdef  HAVE_NUTSHELL_OS
#include "NCRefBase.h"
#include "nceventsys/NCRunnableLooper.h"
#include "nceventsys/NCRunnableThread.h"
#include "nceventsys/NCEvent.h"

class NCTtsStatusEventHandler;
#endif

/**
 * Summary for class VP_NCEventReceiver
 * 
 * This is a detail description for class VP_NCEventReceiver
 *
 */
class VP_NCEventReceiver
{
public:
    VP_NCEventReceiver()
#ifdef HAVE_NUTSHELL_OS
        : m_thread(nullptr)
#endif
    {
    }

    ~VP_NCEventReceiver();

    void initialize();

    void stopRecv();

private:
    VP_NCEventReceiver(const VP_NCEventReceiver& rhs);
    VP_NCEventReceiver& operator=(const VP_NCEventReceiver& rhs);
    void queryTtsStatus();

private:

#ifdef HAVE_NUTSHELL_OS
    nutshell::ncsp<NCTtsStatusEventHandler>::sp m_pcEventHandler;
    nutshell::NCRunnableThread *m_thread;
#endif

    const static std::string TTS_SERVICE;
    const static std::string TTS_LISTENER_THREAD;
};

#ifdef  HAVE_NUTSHELL_OS

/**
 * Summary for class NCTtsStatusEventHandler
 * 
 * This is a detail description for class NCTtsStatusEventHandler
 *
 */
class NCTtsStatusEventHandler : public nutshell::NCEventHandler
{
public:
    NCTtsStatusEventHandler(const nutshell::NCRunnableLooper& looper)
        : NCEventHandler(looper)
    {
    }

    void handleEvent(const nutshell::NCEvent& event);
};

#endif      // end of HAVE_NUTSHELL_OS

#endif
/* EOF */
