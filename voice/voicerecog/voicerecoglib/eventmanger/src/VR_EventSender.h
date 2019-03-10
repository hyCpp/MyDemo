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
 * @file VR_EventSender.h
 * @brief Declaration file of class VR_EventSender.
 *
 * This file includes the declaration of class VR_EventSender.
 *
 * @attention used for C++ only.
 */

#ifndef VR_EVENTSENDER_H
#define VR_EVENTSENDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_EventSenderIF.h"

/**
 * @brief The VR_EventSender class
 *
 * class declaration
 */
class VR_EventSender : public VR_EventSenderIF
{
public:
    VR_EventSender();
    virtual ~VR_EventSender();
    virtual void SendEvent(const char* msgName, VR_ProtoMessageBase *msg);
    virtual bool Reply(const EV_EventContext& context, VR_ProtoMessageBase *msg);
};

#endif // VR_EVENTSENDER_H
/* EOF */
