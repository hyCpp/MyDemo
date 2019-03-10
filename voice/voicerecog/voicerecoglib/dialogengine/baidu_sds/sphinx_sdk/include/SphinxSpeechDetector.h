/*
 * SphinxSpeechDetector.h
 *
 * Copyright (c) 2017 suntec.net, Inc. or its affiliates. All Rights Reserved.
 */

#ifndef SPHINX_SPEECH_DETECTOR_H_
#define SPHINX_SPEECH_DETECTOR_H_

#include <unordered_set>
#include <memory>
#include <vector>

#include "SpeechCommandObserverInterface.h"

namespace SphinxS {

class SphinxSpeechDetector {
public:
    /**
     * Create a SphinxSpeechDetector.
     *
     * @param pathToInputFolder The path to the inputs folder containing data files needed by this application.
     * @return A new @c SphinxSpeechDetector, or @c nullptr if the operation failed.
     */
    static std::unique_ptr<SphinxSpeechDetector> create(
        const std::string& pathToInputFolder,
        const std::vector<std::string> keyWords,
        std::unordered_set<
            std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::SpeechCommandObserverInterface>> speechCommandObservers);

    /// Destructor which manages the @c SphinxSpeechDetector.
    ~SphinxSpeechDetector();

    /// Feed data
    bool OnAudioInData(int16_t* buffer, int32_t size);

    // start storing record data
    void startStoringRecordData();

    // stop storing record data
    void stopStoringRecordData();

private:
    /**
     * Initialize a SphinxSpeechDetector.
     *
     * @param pathToInputFolder The path to the inputs folder containing data files needed by this application.
     * @return @c true if initialization succeeded, else @c false.
     */
    bool initialize(const std::string& pathToInputFolder,
        const std::vector<std::string> keyWords,
        std::unordered_set<
            std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::SpeechCommandObserverInterface>> speechCommandObservers);

};

}  // namespace

#endif  // SPHINX_SPEECH_DETECTOR_H_
