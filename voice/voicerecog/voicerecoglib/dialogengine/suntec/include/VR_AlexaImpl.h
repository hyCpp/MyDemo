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
 * @file VR_AlexaImpl.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_ALEXAIMPL_H
#define VR_ALEXAIMPL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>
#include <memory>

#include <KWD/AbstractKeywordDetector.h>
#include <MediaPlayer/MediaPlayer.h>

/* Forward Declaration*/
class VR_InteractionManager;
class VR_AlexaObserver;
class VR_GoogleObserver;
class VR_ControllerBase;
class VR_AudioFileSender;
class AbstractSDS;
class VR_PortAudioMicWrapper;
class VR_AudioDataWriter;

#ifndef ALEXA_AUDIO_MICROPHONE_H_
#    include "AudioMicrophone.h"
#endif

/**
 * @brief The VR_AlexaImpl class
 *
 * dialog controller class
 */
class VR_AlexaImpl
{
public:
    VR_AlexaImpl();
    virtual ~VR_AlexaImpl();

    virtual bool Initialize(VR_ControllerBase* controller, std::shared_ptr<VR_GoogleObserver> googleObserver, alexaClientSDK::sampleApp::AudioMicrophone* audioMicrophone);
    virtual bool Start();
    virtual void Stop();
    virtual void UnInitialize();

    /**
     * ProcessMessage
     *
     * Process Voice Recognition related messages
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    virtual void ProcessMessage(const std::string& message);

    void OnPlayAudioFile(const std::string& strPath);

    void StopAudioFile();

    std::shared_ptr<VR_PortAudioMicWrapper> GetAudioWrapper();

private:
    // The main interaction manager that interfaces with the SDK.
    std::shared_ptr<VR_InteractionManager> m_interactionManager;
    /// The Wakeword Detector which can wake up the client using audio input.
    std::unique_ptr<alexaClientSDK::kwd::AbstractKeywordDetector> m_keywordDetector;
    VR_ControllerBase*                     m_controller;
    std::shared_ptr<VR_AlexaObserver>      m_alexaObserver;
    std::shared_ptr<VR_GoogleObserver>     m_googleObserver;
    std::shared_ptr<alexaClientSDK::mediaPlayer::MediaPlayer> speakMediaPlayer;
    std::shared_ptr<alexaClientSDK::mediaPlayer::MediaPlayer> audioMediaPlayer;
    std::shared_ptr<alexaClientSDK::mediaPlayer::MediaPlayer> alertsMediaPlayer;
    std::shared_ptr<VR_AudioFileSender> fileSenderWrapper;
    std::shared_ptr<VR_PortAudioMicWrapper> m_micWrapper;
    alexaClientSDK::sampleApp::AudioMicrophone*            m_audioMicrophone;
    std::shared_ptr<VR_AudioDataWriter>                    m_audioDataWriter;
};

#endif
/* EOF */
