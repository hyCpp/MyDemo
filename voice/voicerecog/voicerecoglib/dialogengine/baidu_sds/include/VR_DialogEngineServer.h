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
 * @file VR_DialogEngineServer.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_DIALOGENGINESERVER_H
#define VR_DIALOGENGINESERVER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
/* Suntec Header */
#include "VR_DialogEngineIF.h"
#include "VR_MsgController.h"
#include "VR_SDSController.h"

class VR_DialogEngineListener;
class VR_Settings;
class VR_ControllerBase;


/**
 * @brief The VR_VoiceBoxDialogEngine class
 *
 * class declaration
 */
class VR_API VR_DialogEngineServer: public VR_DialogEngineIF
{
public:
    /**
     * Constructor
     *
     */
    VR_DialogEngineServer();

    /**
     * Destructor
     *
     */
    virtual ~VR_DialogEngineServer();

    /**
     * Initialize
     *
     * Initialize the VoiceBox Server Engine
     *
     * @param pcExternalCallback [IN] : External service's callback
     * @return bool
     * @attention none
     * @see none
     */
    virtual bool Initialize(VR_DialogEngineListener *listener, const VR_Settings &settings);

    /**
     * Start
     *
     * Start the Server Dialog Engine
     *
     * @param none
     * @return bool
     * @attention none
     * @see none
     */
    virtual bool Start();

    /**
     * Stop
     *
     * Stop the Server Dialog Engine
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    virtual void Stop();

    /**
     * UnInitialize
     *
     * Shutdown the Server Dialog Engine
     *
     * @param none
     * @return noe
     * @attention none
     * @see none
     */
    virtual void UnInitialize();

    /**
     * Send Message
     *
     * Send XML format message to the Server Dialog Engine
     *
     * @param message [IN] : XML format message
     * @return bool
     * @attention none
     * @see none
     */
    virtual bool SendMessage(const std::string& message, int actionSeqId = -1);

    virtual std::string getHints(const std::string& hintsParams);

    virtual bool IsNullEngine()
    {
        return m_bIsNullEngine;
    }

private:
    /**
     * Copy Constructor
     *
     */
    VR_DialogEngineServer(const VR_DialogEngineServer&);

    /**
     * Copy Assignment Operator
     *
     */
    VR_DialogEngineServer &operator=(const VR_DialogEngineServer&);

private:
    bool                                        m_bIsNullEngine;
    std::shared_ptr<VR_DialogEngineListener>    m_pListener;
    std::shared_ptr<VR_SDSController>           m_pController;
    std::shared_ptr<VR_MsgController>           m_msgController;
};

#endif // VR_VOICEBOXDIALOGENGINE_H
/* EOF */
