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
 * @file VR_Message.h
 * @brief Declaration file of class VR_Message.
 *
 * This file includes the declaration of class VR_Message.
 *
 * @attention used for C++ only.
 */

#ifndef VR_MESSAGE_H
#define VR_MESSAGE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "VR_MessageProcessIF.h"

#ifndef VR_COMMAND_H
#   include "VR_Command.h"
#endif

/**
 * @brief The VR_Message class
 *
 * class declaration
 */
class VR_Message:public VR_Command
{
public:
    VR_Message(VR_ProtoMessageBase* pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual ~VR_Message();

    virtual void perform();
    virtual void replay();

protected:
    spVR_MessageProcessIF m_spMessageProcess;
};

#endif // VR_MESSAGE_H
/* EOF */
