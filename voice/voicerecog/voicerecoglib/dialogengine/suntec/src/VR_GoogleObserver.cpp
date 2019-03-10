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

#ifndef VR_GOOGLEOBSERVER_H
#    include "VR_GoogleObserver.h"
#endif

VR_GoogleObserver::VR_GoogleObserver(VR_ControllerBase* controller)
: m_controller(controller)
{

}

VR_GoogleObserver::~VR_GoogleObserver()
{

}

void VR_GoogleObserver::onDialogStateChanged(DialogState state)
{
    VR_LOGD_FUNC();

    VR_LOGI("Receive Google DialogState: %d", state);

    switch (state) {
    case DialogState::IDLE:
        {
            m_controller->PostMessage("IDLE", -1);
        }
        break;
    case DialogState::LISTENING:
        {
            m_controller->PostMessage("LISTENING", -1);
        }
        break;
    case DialogState::THINKING:
        {
            m_controller->PostMessage("THINKING", -1);
        }
        break;
    case DialogState::SPEAKING:
        {
            m_controller->PostMessage("SPEAKING", -1);
        }
        break;
    case DialogState::ERROR:
        {
            m_controller->PostMessage("ERROR", -1);
        }
        break;
    default:
        break;
    }
}

void VR_GoogleObserver::onKeyWordDetected(const std::string& strKeyWordType)
{
    VR_LOGD_FUNC();

    if (("google" == strKeyWordType) && m_controller) {
        m_controller->PostMessage("<event name=\"keyWordDetected\"><engine value=\"Google\"/></event>", -1);
    }
    if (("baidu" == strKeyWordType) && m_controller) {
        m_controller->PostMessage("<event name=\"keyWordDetected\"><engine value=\"Baidu\"/></event>", -1);
    }
}

void VR_GoogleObserver::renderTemplateCard(const std::string& jsonPayload)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(jsonPayload, -1);
    }
}

void VR_GoogleObserver::renderPlayerInfoCard(const std::string& jsonPayload)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(jsonPayload, -1);
    }
}

void VR_GoogleObserver::onDialogUXStateChanged(const std::string& state)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(state, -1);
    }
}

void VR_GoogleObserver::onConnectionStatusChanged(const std::string& status, const std::string& reason)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(status, -1);
    }
}


/* EOF */
