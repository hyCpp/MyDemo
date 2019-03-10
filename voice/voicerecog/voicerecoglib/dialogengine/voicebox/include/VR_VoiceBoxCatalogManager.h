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
 * @file VR_VoiceBoxCatalogManager.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_VOICEBOXCATALOGMANAGER_H
#define VR_VOICEBOXCATALOGMANAGER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>
#include <utility>
#include <atomic>

#ifndef VR_VOICEBOXENGINECALLBACK_H
#    include "VR_VoiceBoxEngineCallback.h"
#endif

#ifndef MEM_VECTOR_H
#    include "MEM_vector.h"
#endif

#ifndef MEM_MAP_H
#    include "MEM_map.h"
#endif

/* Forward Declaration */
class IVECIEngineClient;
class IVECIEngineCommand;
class VR_VoiceBoxCatalogApps;
class VR_VoiceBoxCatalogAudio;
class VR_VoiceBoxCatalogPhone;
class VR_VoiceBoxCatalogPoi;

/**
 * @brief The VR_VoiceBoxCatalogManager class
 *
 * class declaration
 */
class VR_VoiceBoxCatalogManager
{
public:
    /**
     * Constructor
     *
     */
    VR_VoiceBoxCatalogManager(
        IVECIEngineClient& client,
        IVECIEngineCommand& engineCommand,
        VR_VoiceBoxEngineCallback& engineCallback
        );

    /**
     * Destructor
     *
     */
    ~VR_VoiceBoxCatalogManager();

    /**
     * Initialize
     *
     * Initialize
     *
     * @param none
     * @return bool
     * @attention none
     * @see none
     */
    bool Initialize();

    /**
     * ProcessMessage
     *
     * Process Catalog related messages
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    void ProcessMessage(const std::string& message);
    VR_VoiceBoxCatalogPhone* GetCatalogPhone();

    void PauseGrammarUpdate(const std::string& strAgent);

    void ResumeGrammarUpdate(const std::string& strAgent);

    void SendPhoneGrammarResult(const std::string& strPhoneGrammar);

    VR_VoiceBoxCatalogAudio* GetCatalogAudio();

    std::pair<std::string, std::string> GetCurrentTransaction();

    void CancelGrammarUpdate(const std::string& strAgent, bool bSync);

    void InitialPersonData();

    void SetNeedPauseGrammarFlg(bool bNeedPauseGrammarFlg);

    bool GetNeedPauseGrammarFlg();

    void ClearCurrentTransaction();

    bool GetVBTErrorFlg();

    void HandleGrammarAfterVDATAUpdate(bool isVDataUpdate, bool &isNeedInstallApp);
    void RequestGrammarExceptApp(bool isVDataUpdate);
    bool UpdateGrammarFinishedStatus(const std::string& strGrammarName, const std::string& strGrammarID);

private:
    IVECIEngineClient&         m_client;       ///< EngineClient
    IVECIEngineCommand&        m_engineCommand;
    VR_VoiceBoxEngineCallback& m_engineCallback; ///< Callback for the dialog engine
    VR_VoiceBoxCatalogApps*    m_catalogApps; ///< Catalog for Agent Apps
    VR_VoiceBoxCatalogAudio*   m_catalogAudio; ///< Catalog for Agent Audio
    VR_VoiceBoxCatalogPhone*   m_catalogPhone; ///< Catalog for Agent Phone
    VR_VoiceBoxCatalogPoi*     m_catalogPoi; ///< Catalog for Agent POI
    bool                       m_isUpdateGrammar;
    std::atomic<bool>          m_bNeedPauseGrammar;
    bool                       m_bVBTError;
    VoiceMultiMap<std::string, std::string>::type m_MapUndoGrammar;

    bool GetGrammarUndoStatus(VoiceMultiMap<std::string, std::string>::type& mapUndoGrammar);
    void SendRequestGrammar(const std::string& strAgentName, const std::string& strGrammarID);


    /**
     * Copy Constructor
     *
     */
    VR_VoiceBoxCatalogManager(const VR_VoiceBoxCatalogManager&);

    /**
     * Copy Assignment Operator
     *
     */
    VR_VoiceBoxCatalogManager &operator=(const VR_VoiceBoxCatalogManager&);
};

#endif // VR_VOICEBOXCATALOGMANAGER_H
/* EOF */
