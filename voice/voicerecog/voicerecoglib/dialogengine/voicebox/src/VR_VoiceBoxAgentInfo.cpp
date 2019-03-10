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

/* Standard Header */
#include "VR_Log.h"

/* VBT Header */
#ifndef VR_VOICEBOXSOURCE_H
#    include "VR_VoiceBoxSource.h"
#endif

/* Suntec Header */
#ifndef VR_VOICEBOXAGENTINFO_H
#    include "VR_VoiceBoxAgentInfo.h"
#endif

#ifndef VR_VOICEBOXLOG_H
#    include "VR_VoiceBoxLog.h"
#endif

#ifndef VR_VOICEBOXENGINECALLBACK_H
#    include "VR_VoiceBoxEngineCallback.h"
#endif

// Constructor
VR_VoiceBoxAgentInfo::VR_VoiceBoxAgentInfo(
    IVECIEngineClient& client,
    IVECIEngineCommand& engineCommand,
    VR_VoiceBoxEngineCallback& engineCallback
)
: VR_VoiceBoxEventSink(client, engineCommand, engineCallback)
{
}

// Destructor
VR_VoiceBoxAgentInfo::~VR_VoiceBoxAgentInfo()
{
}

bool
VR_VoiceBoxAgentInfo::Initialize()
{
    return true;
}

void
VR_VoiceBoxAgentInfo::UnInitialize()
{
}

bool
VR_VoiceBoxAgentInfo::ReplyQueryInfo(
    VR_VoiceBoxXmlParser& parser
)
{
    return true;
}

// QueryAgentSupport
HRESULT
VR_VoiceBoxAgentInfo::QueryAgentSupport(
    VBT_CSTR szAgent,
    VBT_BOOL *pbSupported
)
{
    if (NULL == pbSupported) {
        VR_LOGE("QueryAgentSupport: NULL Parameter");
        return E_FAIL;
    }

    CVECICStr strAgent(&m_client, szAgent);
    // Indicate we want to receive events for Information agent messages
    if (strAgent.IsEqual(VBT_AGENT_INFORMATION)) {
        VR_LOGI("QueryAgentSupport: %s", szAgent);
        *pbSupported = VBT_TRUE;
    }
    else {
        *pbSupported = VBT_FALSE;
    }

    return S_OK;
}

// OnAgentResultMessage
HRESULT
VR_VoiceBoxAgentInfo::OnAgentResultMessage(
    IVECITransaction* pTrans,
    IVECIParsedMessage* pResultMsg
)
{
    if ((NULL == pTrans) || (NULL == pResultMsg)) {
        VR_LOGE("OnAgentResultMessage: NULL Parameter");
        return E_FAIL;
    }

    if (!ProcessAgentMessage(pResultMsg)) {
        return E_FAIL;
    }

    return S_OK;
}

// OnAgentDriverMessage
HRESULT
VR_VoiceBoxAgentInfo::OnAgentDriverMessage(
    IVECIParsedMessage* pDriverMsg
)
{
    if (NULL == pDriverMsg) {
        return E_FAIL;
    }

    if (!ProcessAgentMessage(pDriverMsg)) {
        return E_FAIL;
    }

    return S_OK;
}

bool
VR_VoiceBoxAgentInfo::ProcessAgentMessage(IVECIParsedMessage *pcMsg)
{
    if (NULL == pcMsg) {
        return false;
    }

    if (!SendActionMessage(pcMsg)) {
        return false;
    }

    std::string strAgentResult = "";
    do {
        CVECIOutStr strActionType;
        pcMsg->GetActionType(&strActionType);
        if (NULL != strActionType.Get()) {
            strAgentResult = strActionType.Get();
            break;
        }

        CVECIOutStr strListType;
        pcMsg->GetListType(&strListType);
        if (NULL != strListType.Get()) {
            strAgentResult = strListType.Get();
            break;
        }

        return false;

    } while (false);

    VR_LOGI("ProcessAgentMessage: %s", strAgentResult.c_str());

    // Dispatch the messages
    VoiceMap<std::string, InfoMsgHandler>::const_iterator iterMap = m_infoMsgHandler.find(strAgentResult);
    if (m_infoMsgHandler.cend() != iterMap) {
        if (NULL != iterMap->second) {
            (this->*(iterMap->second))(pcMsg);
            return true;
        }
    }

    return false;
}

/* EOF */