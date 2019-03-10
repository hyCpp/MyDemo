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

#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sstream>

/* Suntec Header */
#ifndef VR_DIALOGENGINESERVER_H
#    include "VR_DialogEngineServer.h"
#endif

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif

#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef CXX_CL_DIR_ABS_H
#   include "CL_Dir_Abs.h"
#endif

#ifndef CXX_BL_STRING_H
#   include "BL_String.h"
#endif

#include "BL_File.h"

#ifndef HEADER_PUGIXML_HPP
#    include "pugixml.hpp"
#endif/* Standard Header */

#ifndef VR_XMLPARSER_H
#    include "VR_XmlParser.h"
#endif

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif

#ifndef CXX_BL_AUTOSYNC_H
#   include "BL_AutoSync.h"
#endif

#ifndef VR_DIALOGENGINEIF_H
#    include "VR_DialogEngineIF.h"
#endif

#ifndef VR_CONTROLLERBASE_H
#    include "VR_ControllerBase.h"
#endif

using namespace std;

extern "C" {
// Factory Method
// Create a dialog engine instance
VR_API VR_DialogEngineIF* VR_CreateDialogEngine()
{
    // The external service should delete created instance
    return VR_new VR_DialogEngineServer;
}
}

// Constructor
VR_DialogEngineServer::VR_DialogEngineServer()
    :m_bIsNullEngine(false)
    ,m_pListener(nullptr)
    ,m_pController(nullptr)
    ,m_msgController(nullptr)
{
}

VR_DialogEngineServer::~VR_DialogEngineServer()
{
}

bool
VR_DialogEngineServer::Initialize(VR_DialogEngineListener* listener, const VR_Settings& settings)
{
    VR_LOGD_FUNC();

    m_pListener = shared_ptr<VR_DialogEngineListener>(listener);

    m_msgController = make_shared<VR_MsgController>();
    m_pController = make_shared<VR_SDSController>();

    if (!m_pController->Initialize(m_pListener, m_msgController)) {
        return false;
    }

    if (!m_msgController->Initialize(m_pController.get())) {
        return false;
    }

    return true;
}

void VR_DialogEngineServer::UnInitialize()
{
    VR_LOGD_FUNC();

    if (nullptr != m_pController) {
        m_pController->UnInitialize();
    }
}

std::string VR_DialogEngineServer::getHints(const std::string& hintsParams)
{
    VR_LOGD_FUNC();
    return "Dialog Server donesn't support hints.";
}

bool VR_DialogEngineServer::Start()
{
    VR_LOGD_FUNC();
    if (!m_pController || !m_msgController || !m_pListener) {
        VR_LOGE("Some member pointers are nullptr!");
        return false;
    }

    VR_LOGD("Start SDSController...");
    if (!m_pController->Start()) {
        VR_LOGE("SDSController started failed!");
        return false;
    }

    VR_LOGD("Start message controller...");
    if (m_msgController) {
        m_msgController->Start();
    }

    m_pListener->OnStarted();

    return true;
}

void VR_DialogEngineServer::Stop()
{
    VR_LOGD_FUNC();

    if (m_pController) {
        m_pController->Stop();
    }

    if (m_msgController) {
        m_msgController->Stop();
    }

    if (m_pListener) {
        m_pListener->OnStopped();
    }
}

bool VR_DialogEngineServer::SendMessage(const string& message, int actionSeqId)
{
    VR_LOGD_FUNC();

    if (nullptr != m_msgController) {
        return m_msgController->PostMessage(message, actionSeqId);
    }

    return false;
}

/* EOF */
