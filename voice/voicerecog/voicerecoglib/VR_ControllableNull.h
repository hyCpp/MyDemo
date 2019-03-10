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
 * @file VR_ControllableIF.h
 * @brief Declaration file of class VR_ControllableIF.
 *
 * This file includes the declaration of class VR_ControllableIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_CONTROLLABLENULL_H
#define VR_CONTROLLABLENULL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_CONTROLLABLEIF_H
#   include "VR_ControllableIF.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogManger_Null);

/**
 * @brief The VR_ControllableNull class
 *
 * class VR_ControllableNull define
 */
class VR_ControllableNull : public VR_ControllableIF
{
public:
    VR_ControllableNull();
    virtual void Initialize();
    virtual void Start();
    virtual void Stop();
    virtual void Destory();

// private:
//    spVR_DialogManger_Null    m_spDialogManger;
};

#endif /* VR_CONTROLLABLENULL_H */
/* EOF */
