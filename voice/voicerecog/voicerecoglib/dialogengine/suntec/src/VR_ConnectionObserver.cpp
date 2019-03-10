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

#include "VR_ConnectionObserver.h"
#include "VR_Log.h"

using namespace avsCommon::sdkInterfaces;

VR_ConnectionObserver::VR_ConnectionObserver() :
        m_authState(AuthObserverInterface::State::UNINITIALIZED),
        m_connectionStatus(ConnectionStatusObserverInterface::Status::DISCONNECTED) {
}

void VR_ConnectionObserver::onAuthStateChange(AuthObserverInterface::State newState, AuthObserverInterface::Error error) {
    VR_LOGD_FUNC();
    std::lock_guard<std::mutex> lock{m_mutex};
    m_authState = newState;
    m_trigger.notify_all();
}

void VR_ConnectionObserver::onConnectionStatusChanged(
    const ConnectionStatusObserverInterface::Status status,
    const ConnectionStatusObserverInterface::ChangedReason reason) {
    VR_LOGD_FUNC();
    std::lock_guard<std::mutex> lock{m_mutex};
    m_connectionStatus = status;
    m_trigger.notify_all();
}

bool VR_ConnectionObserver::waitFor(const AuthObserverInterface::State authState, const std::chrono::seconds duration) {
    VR_LOGD_FUNC();
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_trigger.wait_for(lock, duration, [this, authState]() { return authState == m_authState; });
}

bool VR_ConnectionObserver::waitFor(
    const ConnectionStatusObserverInterface::Status connectionStatus,
    const std::chrono::seconds duration) {
    VR_LOGD_FUNC();
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_trigger.wait_for(
        lock, duration, [this, connectionStatus]() { return connectionStatus == m_connectionStatus; });
}


/* EOF */
