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
 * @file VR_GoogleImpl.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_GOOGLEIMPL_H
#define VR_GOOGLEIMPL_H

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
class VR_GoogleObserver;
class VR_ControllerBase;
class VR_AudioFileSender;
class AbstractSDS;
class AudioInStreamer;
class VR_PortAudioMicWrapper;

/**
 * @brief The VR_GoogleImpl class
 *
 * dialog controller class
 */
class VR_GoogleImpl
{
public:
    VR_GoogleImpl();
    virtual ~VR_GoogleImpl();

    virtual bool Initialize(VR_ControllerBase* controller, AbstractSDS* pGoogleAbstractSDS, std::shared_ptr<VR_PortAudioMicWrapper> micWrapper);
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

    void SetDialogState(const std::string& strDialogState);

    void OnPlayAudioFile(const std::string& strPath);

    void StopAudioFile();

private:
    // The main interaction manager that interfaces with the SDK.
    std::shared_ptr<VR_InteractionManager>                               m_interactionManager;
    /// The Wakeword Detector which can wake up the client using audio input.
    std::unique_ptr<alexaClientSDK::kwd::AbstractKeywordDetector>        m_keywordDetector;
    VR_ControllerBase*                                                   m_controller;
    std::shared_ptr<VR_GoogleObserver>                                   m_googleObserver;
    std::shared_ptr<alexaClientSDK::mediaPlayer::MediaPlayer>            speakMediaPlayer;
    std::shared_ptr<alexaClientSDK::mediaPlayer::MediaPlayer>            audioMediaPlayer;
    std::shared_ptr<alexaClientSDK::mediaPlayer::MediaPlayer>            alertsMediaPlayer;
    std::shared_ptr<VR_AudioFileSender>                                  fileSenderWrapper;
    AbstractSDS*                                                         m_pGoogleAbstractSDS;
    AudioInStreamer*                                                     m_pAudioInStreamer;
    std::shared_ptr<VR_PortAudioMicWrapper>                              m_micWrapper;
};

#endif
/* EOF */
