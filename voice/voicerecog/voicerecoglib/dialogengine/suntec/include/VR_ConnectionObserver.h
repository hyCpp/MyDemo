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
#ifndef VR_CONNECTIONOBSERVER_H
#define VR_CONNECTIONOBSERVER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <chrono>
#include <condition_variable>
#include <mutex>

#include <AVSCommon/SDKInterfaces/AuthObserverInterface.h>
#include <AVSCommon/SDKInterfaces/ConnectionStatusObserverInterface.h>

using namespace alexaClientSDK;

/**
 * A class that observes the status of authorization and connection to AVS.
 */
class VR_ConnectionObserver
        : public avsCommon::sdkInterfaces::AuthObserverInterface
        , public avsCommon::sdkInterfaces::ConnectionStatusObserverInterface {
public:
    /**
     * Constructor.
     */
    VR_ConnectionObserver();

    void onAuthStateChange(
        avsCommon::sdkInterfaces::AuthObserverInterface::State newState,
        avsCommon::sdkInterfaces::AuthObserverInterface::Error error) override;

    void onConnectionStatusChanged(
        const avsCommon::sdkInterfaces::ConnectionStatusObserverInterface::Status status,
        const avsCommon::sdkInterfaces::ConnectionStatusObserverInterface::ChangedReason reason) override;

    /**
     * Waits for the specified authorization state.
     *
     * @param authState The auth state to wait for.
     * @param duration The optional duration to wait for. This defaults to 20 seconds.
     * @return Whether the state was successfully reached.
     */
    bool waitFor(
        const avsCommon::sdkInterfaces::AuthObserverInterface::State authState,
        const std::chrono::seconds duration = std::chrono::seconds(20));

    /**
     * Waits for the specified connection state.
     *
     * @param authState The connection state to wait for.
     * @param duration The optional duration to wait for. This defaults to 20 seconds.
     * @return Whether the state was successfully reached.
     */
    bool waitFor(
        const avsCommon::sdkInterfaces::ConnectionStatusObserverInterface::Status connectionStatus,
        const std::chrono::seconds duration = std::chrono::seconds(20));

private:
    /// Internal mutex to serialize access to m_connectionStatus and m_authState states.
    std::mutex m_mutex;

    /// A condition variable used to wait for state changes.
    std::condition_variable m_trigger;

    /// The current authorization state.
    avsCommon::sdkInterfaces::AuthObserverInterface::State m_authState;

    /// The current connection state.
    avsCommon::sdkInterfaces::ConnectionStatusObserverInterface::Status m_connectionStatus;
};

#endif // VR_CONNECTIONOBSERVER_H
/* EOF */
