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

/* Suntec Header */
#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_ALEXAENGINE_H
#    include "VR_AlexaEngine.h"
#endif

#ifndef VR_MSGCONTROLLER_H
#    include "VR_MsgController.h"
#endif

#ifndef VR_DIALOGENGINE_H
#    include "VR_DialogEngine.h"
#endif

// Create the instance of dialog engine
extern "C" VR_API VR_DialogEngineIF* VR_CreateDialogEngine()
{
    return VR_new VR_DialogEngine();
}

// VR_DialogEngine
VR_DialogEngine::VR_DialogEngine()
: m_engine(nullptr)
, m_msgController(nullptr)
, m_listener(nullptr)
{

}


bool
VR_DialogEngine::Initialize(VR_DialogEngineListener* listerner, const VR_Settings &settings)
{
    VR_LOGD_FUNC();
    if (!listerner) {
        return false;
    }

    m_listener = listerner;

    m_engine = VR_new VR_AlexaEngine();
    if (!m_engine) {
        return false;
    }

    m_msgController = VR_new VR_MsgController();
    if (!m_msgController) {
        return false;
    }

    if (!m_engine->Initialize(listerner, m_msgController)) {
        return false;
    }

    if (!m_msgController->Initialize(m_engine)) {
        return false;
    }

    return true;
}

void VR_DialogEngine::UnInitialize()
{
    if (m_engine) {
        m_engine->UnInitialize();
        delete m_engine;
        m_engine = nullptr;
    }

    if (m_msgController) {
        delete m_msgController;
        m_msgController = nullptr;
    }
}

std::string VR_DialogEngine::getHints(const std::string& hintsParams)
{
    VR_LOGD_FUNC();
    return "Alexa Not Support Hints";
}

bool VR_DialogEngine::Start()
{
    VR_LOGD_FUNC();
    if (!m_engine || !m_msgController || !m_listener) {
        return false;
    }

    if (!m_engine->Start()) {
        return false;
    }

    if (m_msgController) {
        m_msgController->Start();
    }

    m_listener->OnStarted();

    return true;
}

void VR_DialogEngine::Stop()
{
    VR_LOGD_FUNC();

    if (m_engine) {
        m_engine->Stop();        
    }

    if (m_msgController) {
        m_msgController->Stop();        
    }

    if (m_listener) {
        m_listener->OnStopped();        
    }
}

VR_DialogEngine::~VR_DialogEngine()
{

}

bool
VR_DialogEngine::SendMessage(const std::string& event, int actionSeqId)
{
    VR_LOGD_FUNC();

    if (m_msgController) {
        return m_msgController->PostMessage(event, actionSeqId);
    }

    return false;
}
/* EOF */
