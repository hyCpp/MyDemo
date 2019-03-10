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
 * @file VR_VoiceBoxAgentApps.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_VOICEBOXTSLPARAMMANAGER_H
#define VR_VOICEBOXTSLPARAMMANAGER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>

/**
 * @brief The VR_VoiceBoxTslParamManager class
 *
 * class declaration
 */
class VR_VoiceBoxTslParamManager
{
public:
    /**
     * Constructor
     *
     */
    VR_VoiceBoxTslParamManager()
        : m_bBackToRecoState(false)
        , m_bCancel(true)
        , m_bRendering(false)
    {
    }

    /**
     * Destructor
     *
     */
    ~VR_VoiceBoxTslParamManager() {}

    void SetRecoStateMessage(const std::string& strRecoStateMsg)
    {
        m_strCurrentRecoStateMsg = strRecoStateMsg;
    }

    std::string GetRecoStateMessage()
    {
        return m_strCurrentRecoStateMsg;
    }

    void SetStartMessage(const std::string& strStartMsg)
    {
        m_strCurrentStartMsg = strStartMsg;
    }

    std::string GetStartMessage()
    {
        return m_strCurrentStartMsg;
    }

    void SetRecoStateTransaction(const std::string& strRecoStateTrans)
    {
        m_strRecoStateTrans = strRecoStateTrans;
    }

    std::string GetRecoStateTransaction()
    {
        return m_strRecoStateTrans;
    }

    void SetBackToRecoStateFlag(bool bBackToRecoState)
    {
        m_bBackToRecoState = bBackToRecoState;
    }

    bool GetBackToRecoStateFlag()
    {
        return m_bBackToRecoState;
    }

    void SetCancelFlag(bool bCancel)
    {
        m_bCancel = bCancel;
    }

    bool GetCancelFlag()
    {
        return m_bCancel;
    }

    void SetRenderDoneSyncFlag(bool bRenderDoneSync)
    {
        m_bRenderDoneSync = bRenderDoneSync;
    }

    bool GetRenderDoneSyncFlag()
    {
        return m_bRenderDoneSync;
    }

    void SetRenderingFlag(bool bRendering)
    {
        m_bRendering = bRendering;
    }

    bool GetRenderingFlag()
    {
        return m_bRendering;
    }

    void SetPlayTransAfterRender(IVECITransaction* trans, std::string playMsg)
    {
        m_cPlayTransAfterRender.first = trans;
        m_cPlayTransAfterRender.second = playMsg;
    }

    std::pair<IVECITransaction*, std::string> GetPlayTransAfterRender()
    {
        return m_cPlayTransAfterRender;
    }

private:
    std::string m_strCurrentRecoStateMsg;
    std::string m_strCurrentStartMsg;
    std::string m_strRecoStateTrans;
    bool        m_bBackToRecoState;
    bool        m_bCancel;
    bool        m_bRenderDoneSync;
    bool        m_bRendering;
    std::pair<IVECITransaction*, std::string> m_cPlayTransAfterRender;
};

#endif // VR_VOICEBOXTSLPARAMMANAGER_H
/* EOF */
