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

/* Standard Header */
#include <sstream>

/* Alexa Header */
#include <AVSCommon/SDKInterfaces/DialogUXStateObserverInterface.h>

/* Suntec Header */
#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_ALEXAOBSERVER_H
#    include "VR_AlexaObserver.h"
#endif

#ifndef VR_UIMANAGER_H
#    include "VR_UIManager.h"
#endif

using namespace avsCommon::sdkInterfaces;

// VR_AlexaEngine
VR_UIManager::VR_UIManager(std::shared_ptr<VR_AlexaObserver> observer)
: m_alexaObserver{observer}
{

}

void VR_UIManager::onDialogUXStateChanged(DialogUXState state)
{
    VR_LOGD_FUNC();

    std::string strState = DialogUXStateObserverInterface::stateToString(state);
    VR_LOGI("onDialogUXStateChanged: %s", strState.c_str());

    if (state == m_dialogState) {
        return;
    }
    m_dialogState = state;

    if (m_alexaObserver) {
        m_alexaObserver->onDialogUXStateChanged(strState);
    }
}

void VR_UIManager::onConnectionStatusChanged(const Status status, const ChangedReason reason)
{
    VR_LOGD_FUNC();

    // Convert status to a description string
    std::ostringstream ossStatus;
    ossStatus << status;
    // Convert reason to a description string
    std::ostringstream ossReason;
    ossReason << reason;

    VR_LOGI("onConnectionStatusChanged: %s, %s\n", ossStatus.str().c_str(), ossReason.str().c_str());

    if (m_connectionStatus == status) {
        return;
    }
    m_connectionStatus = status;

    if (m_alexaObserver) {
        m_alexaObserver->onConnectionStatusChanged(ossStatus.str(), ossReason.str());
    }
}

void VR_UIManager::onSettingChanged(const std::string& key, const std::string& value)
{
    VR_LOGD_FUNC();
}

void VR_UIManager::onSpeakerSettingsChanged(
    const SpeakerManagerObserverInterface::Source& source,
    const SpeakerInterface::Type& type,
    const SpeakerInterface::SpeakerSettings& settings)
{
    VR_LOGD_FUNC();

    // std::ostringstream oss;
    // oss << "SOURCE:" << source << " TYPE:" << type << " VOLUME:" << static_cast<int>(settings.volume)
    //     << " MUTE:" << settings.mute;
}

void VR_UIManager::onPlayerActivityChanged(avsCommon::avs::PlayerActivity state, const Context& context)
{
    VR_LOGD_FUNC();
    std::string  strActivity = playerActivityToString(state);
    VR_LOGI("strActivity: %s", strActivity.c_str());
    if (m_alexaObserver) {
        if ("PLAYING" == strActivity) {
            m_alexaObserver->onPlayerActivityChanged("PLAYING");
        }
        else if ("IDLE" == strActivity) {
            // do nothing
        }
        else if ("PAUSED" == strActivity) {
            m_alexaObserver->onPlayerActivityChanged("PAUSED");
        }
        else {
            m_alexaObserver->onPlayerActivityChanged("STOPPED");
        }
    }
}

/* EOF */
