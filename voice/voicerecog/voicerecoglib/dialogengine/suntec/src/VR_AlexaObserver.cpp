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

/* Suntec Header */
#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_CONTROLLERBASE_H
#    include "VR_ControllerBase.h"
#endif

#ifndef VR_ALEXAOBSERVER_H
#    include "VR_AlexaObserver.h"
#endif

VR_AlexaObserver::VR_AlexaObserver(VR_ControllerBase* controller)
: m_controller(controller)
{

}

VR_AlexaObserver::~VR_AlexaObserver()
{

}

void VR_AlexaObserver::renderTemplateCard(const std::string& jsonPayload)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(jsonPayload, -1);
    }
}

void VR_AlexaObserver::renderPlayerInfoCard(const std::string& jsonPayload)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(jsonPayload, -1);
    }
}

void VR_AlexaObserver::onKeyWordDetected()
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage("<event name=\"keyWordDetected\"><engine value=\"Alexa\"/></event>", -1);
    }
}

void VR_AlexaObserver::onDialogUXStateChanged(const std::string& state)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(state, -1);
    }
}

void VR_AlexaObserver::onConnectionStatusChanged(const std::string& status, const std::string& reason)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(status, -1);
    }
}

void VR_AlexaObserver::onPlayerActivityChanged(const std::string& status)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(status, -1);
    }
}

void VR_AlexaObserver::NotifyMessageToEngine(const std::string& message)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(message, -1);
    }
}

/* EOF */
