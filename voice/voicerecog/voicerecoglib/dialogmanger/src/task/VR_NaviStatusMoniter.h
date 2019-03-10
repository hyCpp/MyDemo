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
 * @file VR_NaviStatusMoniter.h
 * @brief Declaration file of class VR_NaviStatusMoniter.h
 *
 * This file includes the declaration of class VR_NaviStatusMoniter.h
 *
 * @attention used for C++ only.
 */

#ifndef VR_NAVISTATUSMONITER_H
#define VR_NAVISTATUSMONITER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "VR_DialogEngineIF.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

/**
 * @brief The VR_NaviStatusMoniter_VrAgent class
 *
 * class declaration
 */
class VR_NaviStatusMoniter
{
public:
    VR_NaviStatusMoniter(spVR_DialogEngineIF dialogEngine);
    virtual ~VR_NaviStatusMoniter();

public:
    virtual void OnMessage(const VR_ProtoMessageBase& msg);
    virtual void Start();
    virtual void Stop();

protected:
    spVR_DialogEngineIF     m_spDialogEngine;
};

#endif // VR_NAVISTATUSMONITER_H
/* EOF */
