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
 * @file VR_AudioDataWriter.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_AUDIODATAWRITER_H
#define VR_AUDIODATAWRITER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <mutex>
#include <thread>

#include <AVSCommon/AVS/AudioInputStream.h>
#include <AVSCommon/SDKInterfaces/DialogUXStateObserverInterface.h>
#include <AVSCommon/SDKInterfaces/SpeakerInterface.h>
#include <AVSCommon/Utils/RequiresShutdown.h>

#include <portaudio.h>

#ifndef ALEXA_AUDIO_MICROPHONE_H_
#    include "AudioMicrophone.h"
#endif

using namespace alexaClientSDK::sampleApp;

class VR_PortAudioMicWrapper;

class VR_AudioDataWriter : public AudioDataWriter
{
public:
    VR_AudioDataWriter(std::shared_ptr<VR_PortAudioMicWrapper> micWrapper);
    ~VR_AudioDataWriter();
    virtual ssize_t write(const void* buf, size_t nWords);

private:
    std::shared_ptr<VR_PortAudioMicWrapper>          m_micWrapper;
};


#endif // VR_AUDIODATAWRITER_H
/* EOF */
