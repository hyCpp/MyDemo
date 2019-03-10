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
 * @file VR_EngineClientEventSenderIF.h
 * @brief Declaration file of class VR_EngineClientEventSenderIF.
 *
 * This file includes the declaration of class VR_EngineClientEventSenderIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ENGINECLIENTEVENTSENDERIF_H
#define VR_ENGINECLIENTEVENTSENDERIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Macro.h"
#include "VR_Def.h"

#include <string>

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EngineClientEventSenderIF);
/**
 * @brief The VR_EngineClientEventSenderIF class
 *
 * class declaration
 */
class VR_EngineClientEventSenderIF
{
public:
    virtual ~VR_EngineClientEventSenderIF();
    virtual void SendReqEvent(int& id, const VR_String& msg) = 0;
    virtual void SendReplyEvent(int id, const VR_String& msg) = 0;

};

#endif /* VR_ENGINECLIENTEVENTSENDERIF_H */
/* EOF */
