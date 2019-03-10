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

#include "CL_AutoSync.h"
#include "VR_SDSStateMachine.h"
#include "VR_SDSStateManager.h"
#include "VR_SDSStateBase.h"
#include "AplThreadName.h"
#include "VR_Log.h"

using namespace std;

VR_SDSStateMachine::VR_SDSStateMachine(std::shared_ptr<VR_ControllerBase> pController)
    :m_pController(pController)
{
}

VR_SDSStateMachine::~VR_SDSStateMachine()
{
    ClearRequest();
}

bool VR_SDSStateMachine::Init()
{
    m_pPlatform = make_shared<VR_Platform>();
    m_pStateManager = make_shared<VR_SDSStateManager>(
                shared_ptr<VR_SDSStateMachine>(this), m_pPlatform);
    m_pStateManager->Initialize();

    m_pcState = m_pStateManager->GetRootState();
    VR_LOGD("Get idle state in state machine");

    m_pcState->Enter();

    return CL_TRUE;
}


bool VR_SDSStateMachine::Start() {
    if (NULL == m_pcState) {
        return false;
    }

    RegisterName(VR_VBT_ENGINE_THREAD_DEF);
    StartRegistThread();

    return true;
}

void VR_SDSStateMachine::Stop() {
    if (NULL != m_pcState) {
        m_pcState->Abort();
    }

    StopThread();
    ClearRequest();

    InnerStop();
}

bool VR_SDSStateMachine::IsDone()
{
    return false;
}

VOID VR_SDSStateMachine::ChangeStateTo(VR_SDSAbstractState* pDestState)
{
    m_pLastState = static_cast<VR_SDSAbstractState*>(m_pcState);
    if (NULL != m_pcState) {
        m_pcState->Exit();
    }

    m_pcState = pDestState;

    if (NULL != m_pcState) {
        m_pcState->Enter();
    }
}

VR_SDSAbstractState* VR_SDSStateMachine::GetState(VR_SDSSERVER_STATE_TYPE eType)
{

    if (nullptr == m_pStateManager) {
        return nullptr;
    }

    return m_pStateManager->GetState(eType);
}

VR_SDSAbstractState* VR_SDSStateMachine::GetRootState()
{

    if (nullptr == m_pStateManager) {
        return nullptr;
    }

    return m_pStateManager->GetRootState();
}

void VR_SDSStateMachine::Run()
{
    VR_SDSEvent* pcEvent = nullptr;

    while (true) {
        Wait();

        if (CheckQuit()) {
            break;
        }

        while (nullptr != (pcEvent = GetRequest())) {
            if (nullptr != m_pcState) {
                VR_LOGD("Ready to execute event");
                VR_LOGD("Current state: [%s]", m_pcState->GetStateName().c_str());
                m_pcState->OnEvent(*pcEvent);
            }

            delete pcEvent;
        }
    }
}


void VR_SDSStateMachine::PostRequest(VR_SDSEvent* pcEvent)
{
    CL_AutoSync cAuto(m_cSyncObj);

    if (NULL != pcEvent) {
        VR_LOGD("Post request in state machine!");
        m_cEventList.push_back(pcEvent);

        Notify();
    }
}

VR_SDSEvent* VR_SDSStateMachine::GetRequest()
{
    VR_SDSEvent* pcEvent = NULL;

    CL_AutoSync cAuto(m_cSyncObj);

    if (!m_cEventList.empty()) {
        auto itr = m_cEventList.begin();
        pcEvent = (*itr);

        m_cEventList.erase(m_cEventList.begin());
    }

    return pcEvent;
}

VOID VR_SDSStateMachine::ClearRequest()
{
    CL_AutoSync cAuto(m_cSyncObj);

    for (auto it= m_cEventList.begin(); it != m_cEventList.end(); ++it) {
        delete *it;
    }

    m_cEventList.clear();
}

void VR_SDSStateMachine::SendMessage(const std::string& message, int actionSeqId)
{
    VR_LOGD("Post message to SDSController, message digest: %s",
            message.substr(0, 12).c_str());
    m_pController->PostMessage(message, actionSeqId);
}
