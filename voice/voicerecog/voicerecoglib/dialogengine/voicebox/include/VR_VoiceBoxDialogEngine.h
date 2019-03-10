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
 * @file VR_VoiceBoxDialogEngine.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_VOICEBOXDIALOGENGINE_H
#define VR_VOICEBOXDIALOGENGINE_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
/* Suntec Header */
#include "VR_DialogEngineIF.h"

/* Forward Declaration */
class VR_VoiceBoxEngineIF;
class VR_VoiceBoxMsgController;
class VR_VoiceBoxCatalogController;
class VR_VoiceBoxDialogEngineNull;

/**
 * @brief The VR_VoiceBoxDialogEngine class
 *
 * class declaration
 */
class VR_API VR_VoiceBoxDialogEngine : public VR_DialogEngineIF
{
public:
    /**
     * Constructor
     *
     */
    VR_VoiceBoxDialogEngine();

    /**
     * Destructor
     *
     */
    virtual ~VR_VoiceBoxDialogEngine();

    /**
     * Initialize
     *
     * Initialize the VoiceBox Dialog Engine
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
     * Start the VoiceBox Dialog Engine
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
     * Stop the VoiceBox Dialog Engine
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
     * Shutdown the VoiceBox Dialog Engine
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
     * Send XML format message to the VoiceBox Dialog Engine
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
    VR_VoiceBoxEngineIF*                m_pcVoiceBoxEngine;             ///< VoiceBox dialog engine
    VR_VoiceBoxMsgController*           m_pcMsgController;              ///< Message controller
    VR_VoiceBoxCatalogController*       m_pcCatalogController;          ///< Catalog Message controller
    VR_DialogEngineListener*            m_pcExternalCallback;           ///< External Callback
    VR_VoiceBoxDialogEngineNull*        m_pcVoiceBoxDialogEngineNull;   ///< VoiceBox Dialog Engine Null
    bool                                m_bIsNullEngine;

    bool InitializeVoiceBoxDialogEngine(VR_DialogEngineListener *pcExternalCallback, const VR_Settings &settings);

    bool StartVoiceBoxDialogEngine();

    void SetNullEngine(bool enginestatus)
    {
        m_bIsNullEngine = enginestatus;
    }


    /**
     * Load VBT SDK Version Information
     *
     * Load VBT SDK version information from the version file
     *
     * @param strVersionPath [IN]  : version file path
     * @param strVersion     [OUT] : version information
     * @return bool
     * @attention none
     * @see none
     */
    bool LoadVersionInfo(const std::string& strVersionPath, std::string& strVersion);

    /**
     * Load VDATA Version Information
     *
     * Load VDATA version information from the version.xml
     *
     * @param strVersionPath [IN]  : version.xml file path
     * @param strVersion     [OUT] : version information
     * @return bool
     * @attention none
     * @see none
     */
    bool LoadVDataVersionInfo(const std::string& strVersionPath, std::string& strVersion);

    /**
     * Chcek VData update info
     *
     * Check VData update from version.xml and VBT SDK data version file
     *
     * @param isUpdate [IN/OUT]  : isUpdate
     * @return bool
     * @retval true : check VData update info success
     * @retval false : load VData verison or VBT SDK date version file failed
     * @attention none
     * @see none
     */
    bool CheckVDataUpdate(bool &isUpdate);

    /**
     * Copy Constructor
     *
     */
    VR_VoiceBoxDialogEngine(const VR_VoiceBoxDialogEngine&);

    /**
     * Copy Assignment Operator
     *
     */
    VR_VoiceBoxDialogEngine &operator=(const VR_VoiceBoxDialogEngine&);
};

#endif // VR_VOICEBOXDIALOGENGINE_H
/* EOF */
