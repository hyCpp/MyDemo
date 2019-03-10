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
 * @file VR_CommandHandleIF.h
 * @brief Declaration file of class VR_CommandHandleIF.
 *
 * This file includes the declaration of class VR_CommandHandleIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_COMMANDHANDLEIF_H
#define VR_COMMANDHANDLEIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_COMMAND_H
#   include "VR_Command.h"
#endif

/**
 * @brief The VR_CommandHandleIF class
 *
 * class VR_CommandHandleIF declaration
 */

class VR_CommandHandleIF
{
public:
    VR_CommandHandleIF()
    {

    }

    virtual ~VR_CommandHandleIF()
    {

    }

public:
    virtual void onCommand(const spVR_Command cmd) = 0;
};

#endif // VR_COMMANDHANDLEIF_H
/* EOF */
