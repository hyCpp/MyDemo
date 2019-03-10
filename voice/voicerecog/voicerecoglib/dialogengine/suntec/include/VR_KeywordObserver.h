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
#ifndef VR_KEYWORDOBSERVER_H
#define VR_KEYWORDOBSERVER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <memory>
#include <string>

#include <AVSCommon/AVS/AudioInputStream.h>
#include <AVSCommon/SDKInterfaces/KeyWordObserverInterface.h>
#include <DefaultClient/DefaultClient.h>

using namespace alexaClientSDK;
class VR_AlexaObserver;
class VR_GoogleObserver;

/**
 * Observes callbacks from keyword detections and notifies the DefaultClient that a wake word has occurred.
 */
class VR_KeywordObserver : public avsCommon::sdkInterfaces::KeyWordObserverInterface {
public:
    /**
     * Constructor.
     *
     * @param client The default SDK client.
     * @param audioProvider The audio provider from which to stream audio data from.
     */
    VR_KeywordObserver(
    std::shared_ptr<defaultClient::DefaultClient> client,
    capabilityAgents::aip::AudioProvider audioProvider,
    std::shared_ptr<VR_AlexaObserver> alexaObserver,
    std::shared_ptr<VR_GoogleObserver> googleObserver);

    void onKeyWordDetected(
        std::shared_ptr<avsCommon::avs::AudioInputStream> stream,
        std::string keyword,
        avsCommon::avs::AudioInputStream::Index beginIndex,
        avsCommon::avs::AudioInputStream::Index endIndex) override;

private:
    /// The default SDK client.
    std::shared_ptr<defaultClient::DefaultClient> m_client;

    /// The audio provider.
    capabilityAgents::aip::AudioProvider m_audioProvider;
    std::shared_ptr<VR_AlexaObserver>    m_alexaObserver;
    std::shared_ptr<VR_GoogleObserver>   m_googleObserver;
};

#endif // VR_KEYWORDOBSERVER_H
/* EOF */
