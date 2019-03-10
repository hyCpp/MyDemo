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
 * @file VR_SpeechCommandObserver.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <memory>
#include <string>

#ifndef __VR_ASRENGINEIF_H
    #include "VR_AsrEngineIF.h"
#endif

#include "observerInterface/SpeechCommandObserverInterface.h"

namespace SphinxS {

/**
 * Observes callbacks from keyword detections and notifies the DefaultClient that a speech command has occurred.
 */
class VR_SpeechCommandObserver : public alexaClientSDK::avsCommon::sdkInterfaces::SpeechCommandObserverInterface {

public:
    /**
     * Constructor.
     */
    VR_SpeechCommandObserver(VR_AsrEngineIF* listener);

    void onBeginofSpeechDetected() override;

    void onEndofSpeechDetected(const std::string & cPath) override;

    void onKeyWordSpeechDetected(const std::string & keyword) override;

private:

    VR_AsrEngineIF*  m_pListener;

};

}  // namespace SphinxS
