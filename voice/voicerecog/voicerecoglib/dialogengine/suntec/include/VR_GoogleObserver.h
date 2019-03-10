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
 * @file VR_GoogleObserver.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_GOOGLEOBSERVER_H
#define VR_GOOGLEOBSERVER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>

#ifndef CXX_GOOASSPUBLIC_H
#   include "GooAssPublic.h"
#endif

class VR_ControllerBase;

/**
 * @brief The VR_GoogleObserver class
 *
 * class declaration
 */
class VR_GoogleObserver : public DialogStateObserver
{
public:
    VR_GoogleObserver(VR_ControllerBase* controller);

    /**
     * Destructor
     *
     */
    virtual ~VR_GoogleObserver();

    virtual void onDialogStateChanged(DialogState state);

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
    VR_ControllerBase*  m_controller;
};

#endif // VR_GOOGLEOBSERVER_H
/* EOF */