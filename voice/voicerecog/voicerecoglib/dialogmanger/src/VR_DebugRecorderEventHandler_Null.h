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
 * @file VR_DebugRecorderEventHandler_Null.h
 * @brief Declaration file of class VR_DebugRecorderEventHandler_Null.
 *
 * This file includes the declaration of class VR_DebugRecorderEventHandler_Null.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DEBUGRECORDEREVENTHANDLER_NULL_H
#define VR_DEBUGRECORDEREVENTHANDLER_NULL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include   "VR_DebugRecorderEventHandler.h"

/**
 *   @brief  The  VR_DiagEventHandler_Null  class
 *
 *    this class is used for Null Controller,like, voicetag in meu. for listening the diag envet , copy or delete .
 *
 *    class  Declaration
 *
 */
class  VR_DiagEventHandler_Null : public  VR_DiagEventHandler
{
public:
    VR_DiagEventHandler_Null(VR_DebugRecorderEventHandler*  pDebugRecorderEventHandler, const nutshell::NCRunnableLooper& looper);
    virtual   ~VR_DiagEventHandler_Null();

    virtual   void   handleEvent(const nutshell::NCEvent& event);
};

/**
 *   @brief   The  VR_DebugRecorderEventHandler_Null  class
 *
 *    this class is used for  Null Controller
 *
 *   class   Declaration
 */
class   VR_DebugRecorderEventHandler_Null  : public  VR_DebugRecorderEventHandler
{
public:
    VR_DebugRecorderEventHandler_Null(spVR_RunableThread  spRunnableThread, spVR_DialogEngineIF    spDialogEngine);
    virtual   ~VR_DebugRecorderEventHandler_Null();

    virtual   void  OnStart();
    virtual   void  OnStop();
};

#endif // VR_DEBUGRECORDEREVENTHANDLER_NULL_H
/* EOF */
