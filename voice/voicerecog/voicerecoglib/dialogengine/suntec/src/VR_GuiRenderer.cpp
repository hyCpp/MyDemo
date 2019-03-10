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

// #include <rapidjson/document.h>
// #include <rapidjson/error/en.h>

#include <AVSCommon/Utils/JSON/JSONUtils.h>
#ifndef VR_ALEXAOBSERVER_H
#    include "VR_AlexaObserver.h"
#endif

#include "VR_GuiRenderer.h"
#include "VR_Log.h"

void VR_GuiRenderer::renderTemplateCard(const std::string& jsonPayload) {
    VR_LOGD_FUNC();

    VR_LOGI("%s\n", jsonPayload.c_str());

    if (m_alexaObserver) {
        m_alexaObserver->renderTemplateCard(jsonPayload);
    }
}

void VR_GuiRenderer::renderPlayerInfoCard(
    const std::string& jsonPayload,
    TemplateRuntimeObserverInterface::AudioPlayerInfo info) {
    VR_LOGD_FUNC();

    VR_LOGI("%s\n", jsonPayload.c_str());
    std::string playerState = playerActivityToString(info.audioPlayerState);
    std::string length = std::to_string(info.offset.count());
    VR_LOGI("playerActivityToString(info.audioPlayerState) : %s\n", playerState.c_str());
    VR_LOGI("length : %s\n", length.c_str());

    if (m_alexaObserver) {
        m_alexaObserver->renderPlayerInfoCard(jsonPayload);
    }
}

VR_GuiRenderer::VR_GuiRenderer(std::shared_ptr<VR_AlexaObserver> observer)
: m_alexaObserver{observer}
{

}

/* EOF */
