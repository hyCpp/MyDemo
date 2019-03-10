/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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
 * @file  VR_EventSender_mock.h
 * @brief Mock class for VR_DialogEngineIF.
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_EVENTSENDER_MOCK_H
#define VR_EVENTSENDER_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "VR_EventSenderIF.h"
#include "EV_EventContext.h"

/**
*  VR_EventSender_mock
*
* The class is a mock class for VR_DialogEngine_mock.
*/
class VR_EventSender_mock : public VR_EventSenderIF
{
public:
    // mock methods
    MOCK_METHOD2(SendEvent, void(const char* msgName, VR_ProtoMessageBase *msg));
    MOCK_METHOD2(Reply, bool(const EV_EventContext& context, VR_ProtoMessageBase *msg));
};

#endif // VR_EVENTSENDER_MOCK_H
/* EOF */
