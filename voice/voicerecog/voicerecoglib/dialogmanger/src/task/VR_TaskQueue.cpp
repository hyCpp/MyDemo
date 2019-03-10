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
 * @file VR_TaskQueue.cpp
 * @brief Declaration file of class VR_TaskQueue.
 *
 * This file includes the declaration of class VR_TaskQueue.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"
#include <algorithm>

#include "VR_TaskQueue.h"
#include "VR_DialogTaskBase.h"

VR_TaskQueue::VR_TaskQueue()
{
    VR_LOGD_FUNC();
}

spVR_DialogTaskBase VR_TaskQueue::FindTaskByID(int id)
{
    VR_LOGI("taskId=[%d]", id);
    spVR_DialogTaskBase result;
    VoiceList<spVR_DialogTaskBase>::iterator itr =
        std::find_if(m_TaskList.begin(), m_TaskList.end(),
                 ([id](const spVR_DialogTaskBase& task)->bool {
                      return (task->GetTaskID() == id);
                  }));
    if (itr != m_TaskList.end()) {
        result = *itr;
    }
    VR_LOGI("result=[%p]", result.get());
    return result;
}

spVR_DialogTaskBase VR_TaskQueue::FindTopTask()
{
    VR_LOGD_FUNC();
    spVR_DialogTaskBase result;
    if (!m_TaskList.empty()) {
        result = m_TaskList.front();
    }
    VR_LOGI("result=[%p]", result.get());
    return result;
}

void VR_TaskQueue::PushTask(spVR_DialogTaskBase task)
{
    m_TaskList.push_back(task);
    VR_LOGI("m_TaskList.size()=[%lu]", m_TaskList.size());

    while (!m_TaskList.empty()) {
        VR_LOGI("m_TaskList.front()->IsComplete()=[%d]", m_TaskList.front()->IsComplete());
        if (m_TaskList.front()->IsComplete()) {
            m_TaskList.pop_front();
        }
        else if (m_TaskList.front() == task) {
            VR_LOGI("m_TaskList.front() == task");
            task->Start();
            task.reset();
        }
        else {
            break;
        }
    }
}

void VR_TaskQueue::OnMessage(const VR_ProtoMessageBase &msg)
{
    if (!m_TaskList.empty()) {
        VR_LOGI("!m_TaskList.empty()");
        spVR_DialogTaskBase task = m_TaskList.front();
        task->OnMessage(msg);

        while (!m_TaskList.empty()) {
            VR_LOGI("m_TaskList.front()->IsComplete()=[%d]", m_TaskList.front()->IsComplete());
            if (m_TaskList.front()->IsComplete()) {
                m_TaskList.pop_front();
            }
            else if (m_TaskList.front() == task) {
                VR_LOGI("m_TaskList.front() == task");
                break;
            }
            else {
                task = m_TaskList.front();
                task->Start();
            }
        }
    }
}

/* EOF */
