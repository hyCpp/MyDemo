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
 * @file VR_DictationTask.h
 * @brief Declaration file of class VR_DictationTask.
 *
 * This file includes the declaration of class VR_DictationTask.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DICTATIONTASK_H
#define VR_DICTATIONTASK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DialogTaskBase.h"

/**
 * @brief The VR_DictationTask class
 *
 * class declaration
 */
class VR_DictationTask : public VR_DialogTaskBase
{
public:
    VR_DictationTask(const int type, const EV_EventContext &context, spVR_DialogEngineIF engine);
    VR_DictationTask();
    virtual ~VR_DictationTask();

public:
    virtual void Start();
    virtual void Cancel(const EV_EventContext &context);
    virtual void OnMessage(const VR_ProtoMessageBase& msg);

protected:
    void OnNotifyState (const VR_ProtoMessageBase& msg);
    void OnNotifyDictationResult();

private:
    int                     m_nType;
    spEV_EventContext       m_spContext;
    spVR_DialogEngineIF    m_spEngineIf;

};

#endif // VR_DICTATIONTASK_H
/* EOF */
