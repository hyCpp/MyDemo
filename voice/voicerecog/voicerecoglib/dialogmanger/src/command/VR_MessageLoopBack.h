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
 * @file VR_MessageLoopBack.h
 * @brief Declaration file of class VR_MessageLoopBack.
 *
 * This file includes the declaration of class VR_MessageLoopBack.
 *
 * @attention used for C++ only.
 */

#ifndef VR_MESSAGELOOPBACK_H
#define VR_MESSAGELOOPBACK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "VR_Message.h"

/**
 * @brief The VR_MessageLoopBack class
 *
 * class declaration
 */
class VR_MessageLoopBack : public VR_Message
{
public:
    VR_MessageLoopBack(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual ~VR_MessageLoopBack();

    virtual void perform();
    virtual void replay();
};

#endif // VR_MESSAGELOOPBACK_H
/* EOF */
