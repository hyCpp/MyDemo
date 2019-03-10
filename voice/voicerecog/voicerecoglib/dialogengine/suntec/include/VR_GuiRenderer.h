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
#ifndef VR_GUIRENDERER_H
#define VR_GUIRENDERER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <AVSCommon/SDKInterfaces/TemplateRuntimeObserverInterface.h>

class VR_AlexaObserver;

using namespace alexaClientSDK;

/**
 * A simple class that implments the TemplateRuntimeObserverInterface.  Instead of rendering the
 * display cards, this class will print out some useful information (e.g. JSON payload)
 * when the renderTemplateCard or renderPlayerInfoCard callbacks are called.
 *
 * @note Due to the payload in RenderTemplate may contain sensitive information, the
 * payload will only be printed if @c ACSDK_EMIT_SENSITIVE_LOGS is ON.
 */
class VR_GuiRenderer : public avsCommon::sdkInterfaces::TemplateRuntimeObserverInterface {
public:
    /// @name TemplateRuntimeObserverInterface Functions
    /// @{
    void renderTemplateCard(const std::string& jsonPayload) override;
    void renderPlayerInfoCard(const std::string& jsonPayload, TemplateRuntimeObserverInterface::AudioPlayerInfo info)
        override;
    /// @}

    VR_GuiRenderer(std::shared_ptr<VR_AlexaObserver> observer);

private:
	std::shared_ptr<VR_AlexaObserver>      m_alexaObserver;
};


#endif // VR_GUIRENDERER_H
/* EOF */
