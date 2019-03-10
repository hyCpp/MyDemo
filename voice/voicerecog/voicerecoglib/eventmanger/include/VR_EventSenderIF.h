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
 * @file VR_EventSenderIF.h
 * @brief Declaration file of class VR_EventSenderIF.
 *
 * This file includes the declaration of class VR_EventSenderIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_EVENTSENDERIF_H
#define VR_EVENTSENDERIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Macro.h"
#include "VR_Def.h"
#include "EV_Define.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);

/**
 * @brief The VR_EventSenderIF class
 *
 * class declaration
 */
class VR_EventSenderIF
{
public:
    virtual ~VR_EventSenderIF();
    virtual void SendEvent(const char* msgName, VR_ProtoMessageBase *msg) = 0;
    virtual bool Reply(const EV_EventContext& context, VR_ProtoMessageBase *msg) = 0;

public:
    static spVR_EventSenderIF GetSender();
    static spEV_EventContext CopyEventContext(const EV_EventContext* pContext);
};

#endif /* VR_EVENTSENDERIF_H */
/* EOF */
