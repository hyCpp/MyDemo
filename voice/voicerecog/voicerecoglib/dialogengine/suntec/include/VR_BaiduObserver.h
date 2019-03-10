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
 * @file VR_BaiduObserver.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_BAIDUOBSERVER_H
#define VR_BAIDUOBSERVER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>

#ifndef VR_BAIDUSDS_IF_H
#    include "VR_BaiduSDS_IF.h"
#endif

class VR_ControllerBase;
class VR_DialogEngineListener;

/**
 * @brief The VR_BaiduObserver class
 *
 * class declaration
 */
class VR_BaiduObserver : public VR_BaiduSDSListener
{
public:
    VR_BaiduObserver(VR_ControllerBase* controller, VR_DialogEngineListener* listener);

    /**
     * Destructor
     *
     */
    virtual ~VR_BaiduObserver();

    virtual void onDialogStateChanged(VR_DialogStateBaidu state);

    virtual void OnRequestAction(const std::string& message, int seqId);

    virtual void OnStarted();

    virtual void OnStopped();

    /// @name KeyWordObserverInterface Functions
    /// @{
    void onKeyWordDetected(const std::string& strKeyWordType);
    /// @}

    /// @name TemplateRuntimeObserverInterface Functions
    /// @{
    void renderTemplateCard(const std::string& jsonPayload);
    void renderPlayerInfoCard(const std::string& jsonPayload);
    /// @}

    // @name DialogUXStateObserverInterface Functions
    /// @{
    void onDialogUXStateChanged(const std::string& state);
    /// @}

    // @name ConnectionStatusObserverInterface Functions
    /// @{
    void onConnectionStatusChanged(const std::string& status, const std::string& reason);
    /// @}

private:
    VR_ControllerBase*           m_controller;
    VR_DialogEngineListener*     m_listener;
};

#endif // VR_BAIDUOBSERVER_H
/* EOF */