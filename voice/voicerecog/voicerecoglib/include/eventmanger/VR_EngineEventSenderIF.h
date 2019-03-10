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
 * @file VR_EngineEventSenderIF.h
 * @brief Declaration file of class VR_EngineEventSenderIF.
 *
 * This file includes the declaration of class VR_EngineEventSenderIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ENGINEEVENTSENDERIF_H
#define VR_ENGINEEVENTSENDERIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Macro.h"
#include "VR_Def.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EngineEventSenderIF);
/**
 * @brief The VR_EngineEventSenderIF class
 *
 * class declaration
 */
class VR_EngineEventSenderIF
{
public:
    virtual ~VR_EngineEventSenderIF();
    virtual void SendEvent(int taskId, int id, const VR_String &msg) = 0;
    virtual void SendNotifyEvent(int taskId, const VR_String &msg) = 0;
    virtual bool SendReqEvent(int taskId, int id, const VR_String &msg) = 0;
    virtual void SendReplyEvent(int id, const VR_String &msg) = 0;

public:
    static spVR_EngineEventSenderIF GetSender();
};

#endif /* VR_ENGINEEVENTSENDERIF_H */
/* EOF */
