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

#include "VR_EventSenderIF.h"
#include "VR_EventSender.h"
#include "EV_EventContext.h"

VR_EventSenderIF::~VR_EventSenderIF()
{
}

spVR_EventSenderIF VR_EventSenderIF::GetSender()
{
    spVR_EventSenderIF retv(VR_new VR_EventSender);
    return retv;
}

spEV_EventContext VR_EventSenderIF::CopyEventContext(const EV_EventContext *pContext)
{
    spEV_EventContext retv;
    if (pContext) {
        retv.reset(VR_new EV_EventContext(*pContext));
    }
    else {
        retv.reset(VR_new EV_EventContext());
    }
    return retv;
}

/* EOF */

