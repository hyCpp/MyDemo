/*
 * SpeechCommandObserver.h
 *
 * Copyright (c) 2017 suntec.net, Inc. or its affiliates. All Rights Reserved.
 *
 */

#ifndef SPEECH_COMMAND_OBSERVER_H
#define SPEECH_COMMAND_OBSERVER_H

#include <memory>
#include <string>

#include "SpeechCommandObserverInterface.h"

namespace SphinxS {

/**
 * Observes callbacks from keyword detections and notifies the DefaultClient that a speech command has occurred.
 */
class SpeechCommandObserver : public alexaClientSDK::avsCommon::sdkInterfaces::SpeechCommandObserverInterface {

public:
    /**
     * Constructor.
     */
    SpeechCommandObserver();

    void onBeginofSpeechDetected() override;

    void onEndofSpeechDetected(const std::string & cPath) override;

    void onKeyWordSpeechDetected(const std::string & keyword) override;

private:

};

}  // namespace SphinxS

#endif  // SPEECH_COMMAND_OBSERVER_H
