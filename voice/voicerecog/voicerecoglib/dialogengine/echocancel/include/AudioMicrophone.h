/*
 * AudioMicrophone.h
 *
 * Copyright (c) 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef ALEXA_AUDIO_MICROPHONE_H_
#define ALEXA_AUDIO_MICROPHONE_H_

#include <memory>

enum AECMODE
{
    AECOFF = 0,
    AECON
};

namespace alexaClientSDK {
namespace sampleApp {

class AudioDataWriter {
public:
    /**
     * This function adds new data to the stream by copying it from the provided buffer.
     *
     * @param buf A buffer to copy the data from.
     * @param nWords The maximum number of @c wordSize words to copy.
     * @return The number of @c wordSize words copied, or zero if the stream has closed, or a
     *     negative @c Error code if the stream is still open, but no data could be written.
     */
    virtual ssize_t write(const void* buf, size_t nWords) = 0;
};

/// This acts as a wrapper around PortAudio, a cross-platform open-source audio I/O library.
class AudioMicrophone {
public:
    /**
     * set the audio data writer
     *
     * @param writer The shared data stream writer.
     * @return void
     */
    virtual void setAudioDataWriter(std::shared_ptr<AudioDataWriter> writer) = 0;

    static AudioMicrophone *createAEC(AECMODE mode = AECON);

    virtual void initializeAEC() = 0;

    virtual bool setMode(AECMODE mode) = 0;

    /**
     * Stops streaming from the microphone.
     *
     * @return Whether the stop was successful.
     */
    virtual bool stopStreamingMicrophoneData() = 0;

    /**
     * Starts streaming from the microphone.
     *
     * @return Whether the start was successful.
     */
    virtual bool startStreamingMicrophoneData() = 0;
};

}  // namespace sampleApp
}  // namespace alexaClientSDK

#endif  // ALEXA_AUDIO_MICROPHONE_H_
