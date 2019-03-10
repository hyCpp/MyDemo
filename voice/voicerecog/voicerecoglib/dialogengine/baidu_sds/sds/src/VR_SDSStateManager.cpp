/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "VR_SDSStateManager.h"
#include "VR_SDSRequestServer.h"
#include "VR_SDSStateConcrete.h"
#include "VR_Log.h"

using namespace std;

#define     PROMPT_NETWORK_DISCONNECTED     "使用语音功能，请先打开网络。"
#define     PROMPT_NETWORK_PROBLEM          "网络问题，请稍候再试。"
#define     PROMPT_INVALID_SEARCH_SERVER    "当前搜索服务不可用，请稍后再试。"
#define     PROMPT_SERVER_BUSY              "系统忙，请稍后再试。"
#define     PROMPT_SERVER_SYSTEM_ERROR      "系统故障，请稍后再试。"
#define     VR_REMOTE_DIALOG_SERVER         "http://nlu.suntec.net/dialog-dev"

VR_SDSStateManager::VR_SDSStateManager(std::shared_ptr<VR_SDSStateMachine> pStateMachine,
                                       std::shared_ptr<VR_Platform> pPlatform)
    :m_pcStateMachine(pStateMachine)
    ,m_pPlatform(pPlatform)
{
}

VR_SDSStateManager::~VR_SDSStateManager()
{
    std::map<VR_SDSSERVER_STATE_TYPE, VR_SDSAbstractState*>::iterator it;

    for (it = m_stateMap.begin(); it != m_stateMap.end(); ++it) {
        delete (*it).second;
    }

    m_stateMap.clear();
}

CL_BOOL VR_SDSStateManager::Initialize()
{
    if (!BuildState()) {
        return CL_FALSE;
    }

    return CL_TRUE;
}

CL_BOOL VR_SDSStateManager::BuildState()
{
    m_stateMap[VR_SDSSERVER_STATE_TYPE_IDLE] = new VR_SDSStateServerIdle("IDLE");
    m_stateMap[VR_SDSSERVER_STATE_TYPE_PRESTART] = new VR_SDSStateServerPreStart("PRE START");
    m_stateMap[VR_SDSSERVER_STATE_TYPE_START] = new VR_SDSStateServerStart("START");
    m_stateMap[VR_SDSSERVER_STATE_TYPE_DIALOG] = new VR_SDSStateServerDialog("DIALOG");
    m_stateMap[VR_SDSSERVER_STATE_TYPE_ERROR] = new VR_SDSStateServerError("ERROR");

    VR_SDSRequestServerBase::SetHost(VR_REMOTE_DIALOG_SERVER);

    for (unsigned int i = 0; i < m_stateMap.size(); ++i) {
        if (NULL == m_stateMap[static_cast<VR_SDSSERVER_STATE_TYPE>(i)]) {
            VR_LOGE("|VR_SDSStateManager| |BuildState| [NEW Fails(%d)]\n", i);
            return CL_FALSE;
        } else {
            m_stateMap[static_cast<VR_SDSSERVER_STATE_TYPE>(i)]->SetPlatformIF(m_pPlatform);
            m_stateMap[static_cast<VR_SDSSERVER_STATE_TYPE>(i)]->SetStateMachine(GetStateMachine());
        }
    }

    // Set Host Info
    VR_SDSRequestServerBase::SetHost(VR_REMOTE_DIALOG_SERVER);

    VR_SDSStateServerError* pcState = static_cast<VR_SDSStateServerError*>(m_stateMap[VR_SDSSERVER_STATE_TYPE_ERROR]);

    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_NETWORK_DISCONNECT, PROMPT_NETWORK_DISCONNECTED);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_NETWORK_PROBLEM, PROMPT_NETWORK_PROBLEM);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_SC_SEARCH, PROMPT_INVALID_SEARCH_SERVER);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_SC_INSUFFICIENT_SID, PROMPT_SERVER_BUSY);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_SC_INVALID_UID, PROMPT_SERVER_SYSTEM_ERROR);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_SC_INVALID_SID, PROMPT_SERVER_SYSTEM_ERROR);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_SC_INVALID_MSG, PROMPT_SERVER_SYSTEM_ERROR);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_SC_INVALID_RSP, PROMPT_SERVER_SYSTEM_ERROR);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_SC_INVALID_INDEX, PROMPT_SERVER_SYSTEM_ERROR);
    pcState->BuildErrorMap(VR_SDSSERVER_ERROR_SC_UNKNOWN, PROMPT_SERVER_SYSTEM_ERROR);

    return (VR_SDSSERVER_STATE_TYPE_NUM == m_stateMap.size())? CL_TRUE : CL_FALSE;
}

std::shared_ptr<VR_SDSStateMachine> VR_SDSStateManager::GetStateMachine()
{
    return m_pcStateMachine;
}

/* EOF */
