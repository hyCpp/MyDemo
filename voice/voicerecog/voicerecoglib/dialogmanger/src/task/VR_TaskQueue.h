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
 * @file VR_TaskQueue.h
 * @brief Declaration file of class VR_TaskQueue.
 *
 * This file includes the declaration of class VR_TaskQueue.
 *
 * @attention used for C++ only.
 */


#ifndef VR_TASKQUEUE_H
#define VR_TASKQUEUE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

class VR_DialogTaskBase;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogTaskBase);

/**
 * @brief The VR_TaskQueue class
 *
 * class declaration
 */
class VR_TaskQueue
{
public:
    VR_TaskQueue();
    spVR_DialogTaskBase FindTaskByID(int id);
    spVR_DialogTaskBase FindTopTask();
    void PushTask(spVR_DialogTaskBase task);
    void OnMessage(const VR_ProtoMessageBase& msg);

protected:

    VoiceList<spVR_DialogTaskBase>::type m_TaskList;

};

#endif // VR_TASKQUEUE_H
/* EOF */
