/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_DE_Task.h
 * @brief process delete person data in DE
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_DE_TASK_H
#define VR_DE_TASK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "config.h"
#include <DOM/Document.hpp>
#include <string.h>

class VR_EUDialogEngine;
namespace uscxml
{
class Event;
}
namespace voice
{
/**
 * @brief The VR_DE_Task class
 *
 * for VR DE Task
 */
class VR_DE_Task
{
public:
    virtual ~VR_DE_Task() {}

    virtual bool execute(VR_EUDialogEngine * de) = 0;
    virtual bool onEvent(VR_EUDialogEngine * de, uscxml::Event evt) = 0;
    virtual bool isFinished() = 0;
};

}

#endif // VR_DE_TASK_H
/* EOF */
