/*
 * SpeechCommandObserverInterface.h
 *
 * Copyright 2017 suntec.com, Inc. or its affiliates. All Rights Reserved.
 *
 */

#ifndef ALEXA_CLIENT_SDK_AVSCOMMON_SDKINTERFACES_INCLUDE_AVSCOMMON_SDKINTERFACES_SPEECHCOMMANDOBSERVERINTERFACE_H_
#define ALEXA_CLIENT_SDK_AVSCOMMON_SDKINTERFACES_INCLUDE_AVSCOMMON_SDKINTERFACES_SPEECHCOMMANDOBSERVERINTERFACE_H_

#include <limits>

namespace alexaClientSDK {
namespace avsCommon {
namespace sdkInterfaces {

/**
 * A SpeechCommandObserverInterface is an interface class that clients can extend to listen for speech detections.
 */
class SpeechCommandObserverInterface {
public:
    /**
     * Destructor.
     */
    virtual ~SpeechCommandObserverInterface() = default;

    virtual void onBeginofSpeechDetected() = 0;

    // pcm path
    virtual void onEndofSpeechDetected(const std::string & cPath) = 0;

    // keyword
    virtual void onKeyWordSpeechDetected(const std::string & keyword) = 0;
};

}  // namespace sdkInterfaces
}  // namespace avsCommon
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_AVSCOMMON_SDKINTERFACES_INCLUDE_AVSCOMMON_SDKINTERFACES_SPEECHCOMMANDOBSERVERINTERFACE_H_