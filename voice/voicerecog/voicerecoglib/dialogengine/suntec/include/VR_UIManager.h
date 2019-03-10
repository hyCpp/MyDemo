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
#ifndef VR_UIMANAGER_H
#define VR_UIMANAGER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <memory>

#include <AVSCommon/SDKInterfaces/DialogUXStateObserverInterface.h>
#include <AVSCommon/SDKInterfaces/ConnectionStatusObserverInterface.h>
#include <AVSCommon/SDKInterfaces/AuthObserverInterface.h>
#include <AVSCommon/SDKInterfaces/SingleSettingObserverInterface.h>
#include <AVSCommon/SDKInterfaces/SpeakerInterface.h>
#include <AVSCommon/SDKInterfaces/SpeakerManagerObserverInterface.h>
#include <AVSCommon/SDKInterfaces/AudioPlayerObserverInterface.h>

using namespace alexaClientSDK;

class VR_AlexaObserver;

/**
 * This class manages the states that the user will see when interacting with the Sample Application. For now, it simply
 * prints states to the screen.
 */
class VR_UIManager
        : public avsCommon::sdkInterfaces::DialogUXStateObserverInterface
        , public avsCommon::sdkInterfaces::ConnectionStatusObserverInterface
        , public avsCommon::sdkInterfaces::SingleSettingObserverInterface
        , public avsCommon::sdkInterfaces::SpeakerManagerObserverInterface
        , public avsCommon::sdkInterfaces::AudioPlayerObserverInterface {
public:
    void onDialogUXStateChanged(DialogUXState state) override;

    void onConnectionStatusChanged(const Status status, const ChangedReason reason) override;

    void onSettingChanged(const std::string& key, const std::string& value) override;

    // @name SpeakerManagerObserverInterface Functions
    /// @{
    void onSpeakerSettingsChanged(
        const avsCommon::sdkInterfaces::SpeakerManagerObserverInterface::Source& source,
        const avsCommon::sdkInterfaces::SpeakerInterface::Type& type,
        const avsCommon::sdkInterfaces::SpeakerInterface::SpeakerSettings& settings) override;
    /// }

    void onPlayerActivityChanged(avsCommon::avs::PlayerActivity state, const Context& context);

    VR_UIManager(std::shared_ptr<VR_AlexaObserver> observer);

private:
    /// The current dialog UX state of the SDK
    DialogUXState m_dialogState;

    /// The current connection state of the SDK.
    avsCommon::sdkInterfaces::ConnectionStatusObserverInterface::Status m_connectionStatus;
    std::shared_ptr<VR_AlexaObserver>      m_alexaObserver;
};

#endif // VR_UIMANAGER_H
/* EOF */
