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

#ifndef VR_BAIDUOBSERVER_H
#    include "VR_BaiduObserver.h"
#endif

#ifndef VR_DIALOGENGINEIF_H
#    include "VR_DialogEngineIF.h"
#endif

VR_BaiduObserver::VR_BaiduObserver(VR_ControllerBase* controller, VR_DialogEngineListener* listener)
: m_controller(controller)
, m_listener(listener)
{

}

VR_BaiduObserver::~VR_BaiduObserver()
{

}

void VR_BaiduObserver::onDialogStateChanged(VR_DialogStateBaidu state)
{
    VR_LOGD_FUNC();

    VR_LOGI("Receive Baidu VR_DialogStateBaidu: %d", state);

    switch (state) {
    case VR_DialogStateBaidu::IDLE:
        {
            m_controller->PostMessage("IDLE", -1);
        }
        break;
    case VR_DialogStateBaidu::LISTENING:
        {
            m_controller->PostMessage("LISTENING", -1);
        }
        break;
    case VR_DialogStateBaidu::THINKING:
        {
            m_controller->PostMessage("THINKING", -1);
        }
        break;
    case VR_DialogStateBaidu::SPEAKING:
        {
            m_controller->PostMessage("SPEAKING", -1);
        }
        break;
    case VR_DialogStateBaidu::ERROR:
        {
            m_controller->PostMessage("ERROR", -1);
        }
        break;
    default:
        break;
    }
}

void VR_BaiduObserver::OnRequestAction(const std::string& message, int seqId)
{
    VR_LOGD_FUNC();
    VR_LOGI("Baidu OnRequestAction message is: %s, seqId is: %d", message.c_str(), seqId);
    if (m_listener) {
        m_listener->OnRequestAction(message, seqId);
        VR_LOGI("OnRequestAction message for Baidu");
    }
}

void VR_BaiduObserver::OnStarted()
{
    VR_LOGD_FUNC();

}

void VR_BaiduObserver::OnStopped()
{
    VR_LOGD_FUNC();

}

void VR_BaiduObserver::onKeyWordDetected(const std::string& strKeyWordType)
{
    VR_LOGD_FUNC();

}

void VR_BaiduObserver::renderTemplateCard(const std::string& jsonPayload)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(jsonPayload, -1);
    }
}

void VR_BaiduObserver::renderPlayerInfoCard(const std::string& jsonPayload)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(jsonPayload, -1);
    }
}

void VR_BaiduObserver::onDialogUXStateChanged(const std::string& state)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(state, -1);
    }
}

void VR_BaiduObserver::onConnectionStatusChanged(const std::string& status, const std::string& reason)
{
    VR_LOGD_FUNC();
    if (m_controller) {
        m_controller->PostMessage(status, -1);
    }
}


/* EOF */
