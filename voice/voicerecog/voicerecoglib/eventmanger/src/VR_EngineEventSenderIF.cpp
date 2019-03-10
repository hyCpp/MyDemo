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

#include "VR_EngineEventSender.h"
#include "eventmanger/VR_EngineEventSenderIF.h"

VR_EngineEventSenderIF::~VR_EngineEventSenderIF()
{
}

spVR_EngineEventSenderIF VR_EngineEventSenderIF::GetSender()
{
    spVR_EngineEventSenderIF retv(VR_new VR_EngineEventSender());
    return retv;
}

/* EOF */
