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
 * @file VR_Command.h
 * @brief Declaration file of class VR_Command.
 *
 * This file includes the declaration of class VR_Command.
 *
 * @attention used for C++ only.
 */

#ifndef VR_COMMAND_H
#define VR_COMMAND_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "google/protobuf/message_lite.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_Command);

/**
 * @brief The VR_Command class
 *
 * class declaration
 */
class VR_Command
{
public:
    VR_Command(VR_ProtoMessageBase* pMsg);
    virtual ~VR_Command();

public:
    virtual void perform() = 0;
    virtual void replay() = 0;

protected:
    VR_Command();
    VR_Command(const VR_Command&);
    VR_Command& operator=(const VR_Command&);
    VR_ProtoMessageBase* m_pMessage;
};

#endif // VR_COMMAND_H
/* EOF */
