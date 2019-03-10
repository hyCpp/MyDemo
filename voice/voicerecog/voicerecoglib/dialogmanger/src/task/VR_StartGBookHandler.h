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
 * @file VR_StartGBookHandler.h
 * @brief Declaration file of class VR_StartGBookHandler.
 *
 * This file includes the declaration of class VR_StartGBookHandler.
 *
 * @attention used for C++ only.
 */
#ifndef VR_STARTGBOOKHANDLER_H
#define VR_STARTGBOOKHANDLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

#include   "VR_StartGBookBase.h"

/**
 *  @brief  The  VR_StartGBookHandler class
 *
 *  class  Declaration
 */
class  VR_StartGBookHandler  :  public  VR_StartGBookBase
{
public:
    VR_StartGBookHandler(spVR_EventSenderIF   spEventSender, spVR_DialogEngineIF spDialogEngine);
    virtual  ~VR_StartGBookHandler();

    void   OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void   OnMessage(const VR_ProtoMessageBase &msg);
    void   OnStart();
    void   OnStop();
    std::string getServerVRUrl();
};

#endif // VR_STARTGBOOKHANDLER_H
/* EOF */
