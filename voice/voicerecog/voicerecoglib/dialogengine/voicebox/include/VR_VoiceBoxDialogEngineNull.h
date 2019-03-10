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
 * @file VR_VoiceBoxDialogEngineNull.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_VOICEBOXDIALOGENGINENULL_H
#define VR_VOICEBOXDIALOGENGINENULL_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
/* Suntec Header */
#include "VR_DialogEngineIF.h"

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif


/**
 * @brief The VR_VoiceBoxDialogEngineNull class
 *
 * class declaration
 */
class VR_VoiceBoxDialogEngineNull : public VR_DialogEngineIF
{
public:
    /**
     * Constructor
     *
     */
    VR_VoiceBoxDialogEngineNull();

    /**
     * Destructor
     *
     */
    virtual ~VR_VoiceBoxDialogEngineNull();

    /**
     * Initialize
     *
     * Initialize the VoiceBox Dialog Engine Null
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
     * Start the VoiceBox Dialog Engine Null
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
     * Stop the VoiceBox Dialog Engine Null
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
     * Shutdown the VoiceBox Dialog Engine Null
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
     * Send XML format message to the VoiceBox Dialog Engine Null
     *
     * @param message [IN] : XML format message
     * @return bool
     * @attention none
     * @see none
     */
    virtual bool SendMessage(const std::string& message, int actionSeqId = -1);

    virtual std::string getHints(const std::string& hintsParams);

private:
    VR_DialogEngineListener*           m_pcExternalCallback; ///< External service's callback
    VR_REGION_TYPE                     m_country;

    void SendDMMessage(const std::string& strmsg, int actionSeqId);
    void AddResultToNode(std::string& strDestxml, const std::string& strSourxml, bool isAppendErrCode);
    void BuildGrammarResultXML(const std::string& strGrammarxml, std::string& strGrammarResultxml);

    /**
     * Copy Constructor
     *
     */
    VR_VoiceBoxDialogEngineNull(const VR_VoiceBoxDialogEngineNull&);

    /**
     * Copy Assignment Operator
     *
     */
    VR_VoiceBoxDialogEngineNull &operator=(const VR_VoiceBoxDialogEngineNull&);
};

#endif // VR_VOICEBOXDIALOGENGINENULL_H
/* EOF */
