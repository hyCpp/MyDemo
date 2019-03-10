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
 * @file VR_AsrEngineIF.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_ASRENGINEIF_H
#define VR_ASRENGINEIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_ControllerBase.h"

/**
 * @brief The VR_IFlytekImpl class
 *
 * dialog controller class
 */
class VR_AsrEngineIF
{
public:

    virtual ~VR_AsrEngineIF() {};
    virtual bool Initialize(VR_ControllerBase* controller) = 0;
    virtual bool Start() = 0;
    virtual void Stop() = 0;
    virtual void UnInitialize() = 0;
    virtual void ProcessMessage(const std::string& message) = 0;
    virtual bool PostMessage(const std::string& message, int actionId = -1) = 0;
    virtual void startRecordMicData() = 0;
    virtual void stopRecordMicData() = 0;
    virtual void postRequest(const std::string pcm) = 0;
    virtual int onAudioInData(int16_t* buffer, int32_t size) = 0;
};

#endif
/* EOF */
