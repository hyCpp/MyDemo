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
 * @file VR_EventHandleIF.h
 * @brief Declaration file of class VR_EventHandleIF.
 *
 * This file includes the declaration of class VR_EventHandleIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_EVENTREGISTERIF_H
#define VR_EVENTREGISTERIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Macro.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventHandleIF);

/**
 * @brief The VR_EventRegisterIF class
 *
 * class declaration
 */
class VR_EventRegisterIF
{
public:
    virtual ~VR_EventRegisterIF();
    virtual void Register(wpVR_EventHandleIF handle) = 0;

};

#endif /* VR_EVENTREGISTERIF_H */
/* EOF */
