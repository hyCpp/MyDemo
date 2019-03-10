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
 * @file VR_EUDialogEngine.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_INIT_PERSON_DATA_H
#define VR_INIT_PERSON_DATA_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DE_Task.h"
#include "uscxml/concurrency/BlockingQueue.h"
#include "uscxml/plugins/invoker/vr/VRServiceRequestor.h"
#include "pugixml.hpp"
#include <string.h>

class VR_EUDialogEngine;

namespace uscxml
{
class Event;
}

/**
 * @brief The task class
 *
 * Person Date Delete class
 */
class VR_InitPersonDate : public voice::VR_DE_Task
{
public:
    VR_InitPersonDate();
    virtual ~VR_InitPersonDate();

/**
 * @return true: task complete
 *         false: task need wait for event
 */
    bool execute(VR_EUDialogEngine* engine);

/**
 * @return true: share event
 *         false: do not share the event
 */
    bool onEvent(VR_EUDialogEngine* engine, uscxml::Event evt);

    bool isFinished();

private:
    bool m_taskComplete;
};

#endif
/* EOF */
