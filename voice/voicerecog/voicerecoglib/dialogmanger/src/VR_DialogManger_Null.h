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
 * @file VR_DialogManger_Null.h
 * @brief Declaration file of class VR_DialogManger_Null.
 *
 * This file includes the declaration of class VR_DialogManger_Null.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DIALOGMANGER_NULL_H
#define VR_DIALOGMANGER_NULL_H


#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif


#include   "VR_DebugRecorderEventHandler_Null.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DebugRecorderEventHandler_Null);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RunableThread);

/**
 *
 *   @brief  The VR_DialogManger_Null  class
 *
 *   this class most of the time it is no use,
 *   but you can add to some acquiescent logic.
 *
 *   class  Declaration
 */
class   VR_DialogManger_Null
{
public:
    VR_DialogManger_Null();
    virtual ~VR_DialogManger_Null() {}
    virtual void Initialize();
    virtual void Start();
    virtual void Stop();
    virtual void Destory();

private:
    spVR_RunableThread                      m_spRunnableThread;
    spVR_DialogEngineIF                     m_spDialogEngine;
    spVR_DebugRecorderEventHandler_Null     m_spDebugEventHandler;
};

#endif // VR_DIALOGMANGER_NULL_H
/* EOF */
