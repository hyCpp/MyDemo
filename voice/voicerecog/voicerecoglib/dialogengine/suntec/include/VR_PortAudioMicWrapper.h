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
 * @file VR_MsgController.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_PORTAUDIOMICWRAPPER_H
#define VR_PORTAUDIOMICWRAPPER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <mutex>
#include <thread>
#include <fstream>
#include <AVSCommon/AVS/AudioInputStream.h>
#include <AVSCommon/SDKInterfaces/DialogUXStateObserverInterface.h>
#include <AVSCommon/SDKInterfaces/SpeakerInterface.h>
#include <AVSCommon/Utils/RequiresShutdown.h>

#include <portaudio.h>

using namespace alexaClientSDK;

#ifndef ALEXA_AUDIO_MICROPHONE_H_
#include "AudioMicrophone.h"
#endif


class VR_InteractionManager;
class AudioInStreamer;
class VR_AudioInStreamer;
class VR_DictationManager;

/// This acts as a wrapper around PortAudio, a cross-platform open-source audio I/O library.
class VR_PortAudioMicWrapper {
public:
    /**
     * Creates a @c VR_PortAudioMicWrapper.
     *
     * @param stream The shared data stream to write to.
     * @return A unique_ptr to a @c VR_PortAudioMicWrapper if creation was successful and @c nullptr otherwise.
     */
    static std::unique_ptr<VR_PortAudioMicWrapper> create(std::shared_ptr<avsCommon::avs::AudioInputStream> stream);

    /**
     * Stops streaming from the microphone.
     *
     * @return Whether the stop was successful.
     */
    bool stopStreamingMicrophoneData();

    /**
     * Starts streaming from the microphone.
     *
     * @return Whether the start was successful.
     */
    bool startStreamingMicrophoneData();

    /**
     * Stops streaming from the microphone.
     *
     * @return Whether the stop was successful.
     */
    bool stopStreamingData();

    /**
     * Starts streaming from the microphone.
     *
     * @return Whether the start was successful.
     */

    bool startStreamingData(const std::string& audioFile);

    void threadFunc();

    void SetInteractionObj(std::shared_ptr<VR_InteractionManager> interactionManager);

    void SetMicrophoneObj(alexaClientSDK::sampleApp::AudioMicrophone* audioMicrophone);

    void SetGoogleAudioIn(AudioInStreamer* stream);

    void SetBaiduAudioIn(VR_AudioInStreamer* stream);

    void SetEngineValue(const std::string& strEngineValue);

    void SetMicState(const std::string& strMicState);

    int WriteAudioDataToSDK(const void* inputBuffer, unsigned long numSamples);

    void SaveMicToBuffer(const short* inputBuffer, unsigned long numSamples);

    short *SaveMicToBuffer(const short *inputBuffer, unsigned long numSamples, short *buffer, short *ptop);

    void SaveMicToFile(std::string strMicState);

    void SetDictationManager(std::shared_ptr<VR_DictationManager> dictationManager);

    /**
     * Destructor.
     */
    ~VR_PortAudioMicWrapper();

private:
    /**
     * Constructor.
     *
     * @param stream The shared data stream to write to.
     */
    VR_PortAudioMicWrapper(std::shared_ptr<avsCommon::avs::AudioInputStream> stream);

    /**
     * The callback that PortAudio will issue when audio is avaiable to read.
     *
     * @param inputBuffer The temporary buffer that microphone audio data will be available in.
     * @param outputBuffer Not used here.
     * @param numSamples The number of samples available to consume.
     * @param timeInfo Time stamps indicated when the first sample in the buffer was captured. Not used here.
     * @param statusFlags Flags that tell us when underflow or overflow conditions occur. Not used here.
     * @param userData A user supplied pointer.
     * @return A PortAudio code that will indicate how PortAudio should continue.
     */
    static int PortAudioCallback(
        const void* inputBuffer,
        void* outputBuffer,
        unsigned long numSamples,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData);

    /// Initializes PortAudio
    bool initialize();

    /// The stream of audio data.
    const std::shared_ptr<avsCommon::avs::AudioInputStream> m_audioInputStream;

    /// The writer that will be used to writer audio data into the sds.
    std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> m_writer;

    /// The PortAudio stream
    PaStream* m_paStream;

    /**
     * A lock to seralize access to startStreamingMicrophoneData() and stopStreamingMicrophoneData() between different
     * threads.
     */
    std::mutex m_mutex;

    std::mutex m_mutexbuffer;

    std::thread m_thread;

    bool m_stopReadWareFile;

    std::string m_audioSenderFile;

    std::shared_ptr<VR_InteractionManager>                m_interactionManager;
    std::shared_ptr<VR_DictationManager>                  m_dictationManager;
    static AudioInStreamer*                               m_pGoogleStreamer;
    static VR_AudioInStreamer*                            m_pBaiduStreamer;
    static std::string                                    m_strEngineValue;
    static std::string                                    m_strMicState;
    // static VC_WavFileWriter*                              m_spWavWriter;
    // static boost::shared_ptr<VC_WavFileWriter>                   m_spWavWriter;
    alexaClientSDK::sampleApp::AudioMicrophone*           m_audioMicrophone;
    short*                                                m_buffer;
    int                                                   m_numfile;
    std::string                                           m_srcName;
    bool                                                  m_writeOverLoad;
    short*                                                m_topbuffer;

    // avsCommon::utils::threading::Executor m_executor;
};


#endif // VR_PORTAUDIOMICWRAPPER_H
/* EOF */
