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

/* Suntec Header */
#ifndef VR_VOICEBOXDIALOGENGINE_H
#    include "VR_VoiceBoxDialogEngine.h"
#endif

#ifndef VR_VOICEBOXENGINEIF_H
#    include "VR_VoiceBoxEngineIF.h"
#endif

#ifndef VR_VOICEBOXMSGCONTROLLER_H
#    include "VR_VoiceBoxMsgController.h"
#endif

#ifndef VR_VOICEBOXCATALOGCONTROLLER_H
#    include "VR_VoiceBoxCatalogController.h"
#endif

#ifndef VR_VOICEBOXDIALOGENGINENULL_H
#    include "VR_VoiceBoxDialogEngineNull.h"
#endif

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif

#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
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
#endif

extern "C" {
// Factory Method
// Create a dialog engine instance
VR_API VR_DialogEngineIF* VR_CreateDialogEngine()
{
    // The external service should delete created instance
    return VR_new VR_VoiceBoxDialogEngine;
}
}

// Constructor
VR_VoiceBoxDialogEngine::VR_VoiceBoxDialogEngine()
    : m_pcVoiceBoxEngine(NULL)
    , m_pcMsgController(NULL)
    , m_pcCatalogController(NULL)
    , m_pcExternalCallback(NULL)
    , m_pcVoiceBoxDialogEngineNull(NULL)
    , m_bIsNullEngine(false)
{
}

// Destructor
VR_VoiceBoxDialogEngine::~VR_VoiceBoxDialogEngine()
{
    if (NULL != m_pcVoiceBoxEngine) {
        delete m_pcVoiceBoxEngine;
        m_pcVoiceBoxEngine = NULL;
    }
    if (NULL != m_pcMsgController) {
        delete m_pcMsgController;
       m_pcMsgController = NULL;
    }
    if (NULL != m_pcCatalogController) {
        delete m_pcCatalogController;
        m_pcCatalogController = NULL;
    }
    if (NULL != m_pcVoiceBoxDialogEngineNull) {
        delete m_pcVoiceBoxDialogEngineNull;
        m_pcVoiceBoxDialogEngineNull = NULL;
    }
    m_pcExternalCallback = NULL;
}


bool VR_VoiceBoxDialogEngine::InitializeVoiceBoxDialogEngine(VR_DialogEngineListener *pcExternalCallback, const VR_Settings &settings)
{
    CL_Dir_Abs cDir;
    CL_AbstractFile::FileSize size;

    bool isUpdate = false;
    if (!CheckVDataUpdate(isUpdate)) {
        VR_LOGI("cannnot load VData version info, maybe there is no DATA !!! Now we will start NullEngine");
        return false;
    }

    enVDataStatus enVdataVBTStatus = settings.enVDStatus;
    // if VDATA updated or vbt data version is changed, update vbt data
    // DE_VBT will check version too, and set update flag if/data/vr/data/version file not exist
    if (enVDataStatus_VersionChange == settings.enVDStatus || isUpdate) {
        VR_LOGI("Update VDATA version, first remove the used data directory");
        cDir.RemoveDir(XTEXT("/data/vr"));
        enVdataVBTStatus = enVDataStatus_VersionChange;
    }

    if (!cDir.GetFileInfo(XTEXT("/data/vr/data/version.xml"), &size)) {
        if (!cDir.MakeDirTree(XTEXT("/data/vr/data"))) {
            VR_LOGI("Make direcotry tree failed\n");
            return false;
        }

        if (!cDir.MakeDirTree(XTEXT("/data/vr/config"))) {
            VR_LOGI("Make direcotry tree failed\n");
            return false;
        }

        if (!cDir.MakeDirTree(XTEXT("/data/vr/vbtlog"))) {
            VR_LOGI("Make direcotry tree failed\n");
            return false;
        }

        const int DATA_PATH_LEN = 256;
        XCHAR chVBTDataPath[DATA_PATH_LEN] = { 0 };

        std::string strVBTDataPath = VR_ConfigureIF::Instance()->getDataPath();
        strVBTDataPath.append("data");
        int len = CharToXCHAR(strVBTDataPath.c_str(), strVBTDataPath.length(), chVBTDataPath, DATA_PATH_LEN);
        if (len != static_cast<int>(strVBTDataPath.length())) {
            VR_LOGI("Convert char to xchar failed\n");
            return false;
        }

        if (!cDir.CopyDir(chVBTDataPath, XTEXT("/data/vr/data"), CL_TRUE)) {
            VR_LOGI("Copy data failed\n");
            return false;
        }

        strVBTDataPath = VR_ConfigureIF::Instance()->getDataPath();
        strVBTDataPath.append("/config");
        len = CharToXCHAR(strVBTDataPath.c_str(), strVBTDataPath.length(), chVBTDataPath, DATA_PATH_LEN);
        if (len != static_cast<int>(strVBTDataPath.length())) {
            VR_LOGI("Convert char to xchar failed\n");
            return false;
        }

        if (!cDir.CopyDir(chVBTDataPath, XTEXT("/data/vr/config"), CL_TRUE)) {
            VR_LOGI("Copy config failed\n");
            return false;
        }

        // copy VDATA/version.xml to /data/vr/data/version.xml
        BL_File pathFile(BL_FILE_PREFIX_VDATA);
        BL_String versionFullPath;
        pathFile.GetLogicalPath("version.xml", versionFullPath);
        std::string strVBTDataVersionPath(versionFullPath.GetString());

        len = CharToXCHAR(strVBTDataVersionPath.c_str(), strVBTDataVersionPath.length(), chVBTDataPath, DATA_PATH_LEN);
        if (len != static_cast<int>(strVBTDataVersionPath.length())) {
            VR_LOGI("Convert char to xchar failed\n");
            return false;
        }

        if (!cDir.CopyFile(chVBTDataPath, XTEXT("/data/vr/data/version.xml"), CL_TRUE)) {
            VR_LOGI("Copy version.xml file failed\n");
            return false;
        }
    }

    // Create the VoiceBox dialog engine instance
    // Used for voice recognition
    m_pcVoiceBoxEngine = VR_VoiceBoxEngineIF::CreateInstance();
    if (NULL == m_pcVoiceBoxEngine) {
        VR_ERROR(VOICE_VR_ERR_ENGINE_VBT, 0, "VR_VoiceBoxDialogEngine Create Failed\n");
        return false;
    }

    m_pcVoiceBoxEngine->setVDATAUpdateStatus(enVdataVBTStatus);

    // Create the message contoller
    // Used for dispatch the XML message the receive from external service
    // or self.
    m_pcMsgController = VR_new VR_VoiceBoxMsgController();
    if (NULL == m_pcMsgController) {
        return false;
    }

    // Create the catalog message contoller
    // Used for dispatch the XML message the receive from external service
    // or self.
    m_pcCatalogController = VR_new VR_VoiceBoxCatalogController();
    if (NULL == m_pcCatalogController) {
        return false;
    }


    // Initialize the VoiceBox dialog engine
    bool bInitRet = m_pcVoiceBoxEngine->Initialize(
                    pcExternalCallback, // Used for notify external service
                    m_pcMsgController,  // Used for receive self's message
                    m_pcCatalogController
                    );
    if (!bInitRet) {
        VR_LOGE("VR_VoiceBoxDialogEngine Initialize failed\n");
        return false;
    }

    // Initialize the message controller
    bInitRet = m_pcMsgController->Initialize(
               m_pcVoiceBoxEngine  // Used for process VR related requests
               );
    if (!bInitRet) {
        return false;
    }

    bInitRet = m_pcCatalogController->Initialize(
               m_pcVoiceBoxEngine  // Used for process VR related requests
               );
    if (!bInitRet) {
        return false;
    }

    return true;
}

bool VR_VoiceBoxDialogEngine::StartVoiceBoxDialogEngine()
{
    if (NULL == m_pcVoiceBoxEngine) {
        return false;
    }

    // Start the message controller thread
    if (NULL != m_pcMsgController) {
        m_pcMsgController->Start();
    }

    // Start the catalog controller thread
    if (NULL != m_pcCatalogController) {
        m_pcCatalogController->Start();
    }

    VR_ConfigureIF * pcConfig = VR_ConfigureIF::Instance();
    if (NULL == pcConfig) {
        return false;
    }

    std::string cultureName = "en-US";
    std::string lang = pcConfig->getVRLanguage();
    if (VR_LANGUAGE_FR_CA == lang) {
        cultureName = "fr-CA";
    }
    else if (VR_LANGUAGE_ES_MX == lang) {
        cultureName = "es-MX";
    }
    else if (VR_LANGUAGE_EN_AU == lang) {
        cultureName = "en-AU";
    }
    else {
        // do nothing
    }

    // If start dialog engine failed, we could not used the VR function.
    if (!m_pcVoiceBoxEngine->Start(cultureName)) {
        VR_LOGE("VR_VoiceBoxDialogEngine Start failed\n");
        return false;
    }
    int promptLevel = pcConfig->getVRPromptLevel();
    m_pcVoiceBoxEngine->setPromptLevel(promptLevel);

    return true;
}

// Create the dialog engine related instances and initialize them.
bool VR_VoiceBoxDialogEngine::Initialize(
    VR_DialogEngineListener *pcExternalCallback,
    const VR_Settings &settings
    )
{
    VR_LOGI("VR_VoiceBoxDialogEngine Initializing\n");
    m_pcExternalCallback = pcExternalCallback;
    SetNullEngine(false);
    // when update VDATA error, we should remove vbt data and create NullEngine
    if (enVDataStatus_Error == settings.enVDStatus) {
        VR_LOGI("VData update Error !!!");

        CL_Dir_Abs cDir;
        cDir.RemoveDir(XTEXT("/data/vr"));

        m_pcVoiceBoxDialogEngineNull = VR_new VR_VoiceBoxDialogEngineNull();
        SetNullEngine(true);
        if (NULL == m_pcVoiceBoxDialogEngineNull) {
            VR_LOGE("new m_pcVoiceBoxDialogEngineNull failed !!");
            return false;
        }
        m_pcVoiceBoxDialogEngineNull->Initialize(m_pcExternalCallback, settings);
        return false;
    }

    if (!InitializeVoiceBoxDialogEngine(m_pcExternalCallback, settings)) {
        VR_LOGE("InitializeVoiceBoxDialogEngine failed, will start NULLEngine !!");
        UnInitialize();
        m_pcVoiceBoxDialogEngineNull = VR_new VR_VoiceBoxDialogEngineNull();
        SetNullEngine(true);
        if (NULL == m_pcVoiceBoxDialogEngineNull) {
            VR_LOGE("new m_pcVoiceBoxDialogEngineNull failed !!");
            return false;
        }
        m_pcVoiceBoxDialogEngineNull->Initialize(m_pcExternalCallback, settings);
        return false;
    }

    VR_LOGI("VR_VoiceBoxDialogEngine Initialize successfully\n");
    return true;
}

// Start the dialog engine
bool VR_VoiceBoxDialogEngine::Start()
{
    VR_LOGI("VR_VoiceBoxDialogEngine Starting\n");
    if (m_bIsNullEngine) {
        VR_LOGI("Now the VoiceBox DialogEngine is NullEngine !!");
        if (NULL != m_pcVoiceBoxDialogEngineNull) {
            m_pcVoiceBoxDialogEngineNull->Start();
        }
        return false;
    }

    if (!StartVoiceBoxDialogEngine()) {
        VR_LOGE("StartVoiceBoxDialogEngine failed, will start NULLEngine !!");

        Stop();
        UnInitialize();
        m_pcVoiceBoxDialogEngineNull = VR_new VR_VoiceBoxDialogEngineNull();
        SetNullEngine(true);
        if (NULL == m_pcVoiceBoxDialogEngineNull) {
            VR_LOGE("new m_pcVoiceBoxDialogEngineNull failed !!");
            return false;
        }
        VR_Settings settings;
        m_pcVoiceBoxDialogEngineNull->Initialize(m_pcExternalCallback, settings);
        m_pcVoiceBoxDialogEngineNull->Start();
        return false;
    }

    VR_LOGI("VR_VoiceBoxDialogEngine Start successfully\n");
    return true;
}

// Stop the dialog engine
void VR_VoiceBoxDialogEngine::Stop()
{
    if (m_bIsNullEngine) {
        VR_LOGI("Now the VoiceBox DialogEngine is NullEngine !!");
        if (NULL != m_pcVoiceBoxDialogEngineNull) {
            m_pcVoiceBoxDialogEngineNull->Stop();
        }
        return;
    }

    if (NULL != m_pcVoiceBoxEngine) {
        // Stop the dialog engine
        m_pcVoiceBoxEngine->Stop();
    }

    if (NULL != m_pcMsgController) {
        // Stop the message controller thread
        m_pcMsgController->Stop();
    }

    if (NULL != m_pcCatalogController) {
        // Stop the catalog controller thread
        m_pcCatalogController->Stop();
    }
}

// Uninitialize the dialog engine
void VR_VoiceBoxDialogEngine::UnInitialize()
{
    if (NULL != m_pcVoiceBoxEngine) {
        // Uninitialize and shutdown the dialog engine
        m_pcVoiceBoxEngine->UnInitialize();
        // Delete the dialog engine and message controller instances
        delete m_pcVoiceBoxEngine;
        m_pcVoiceBoxEngine = NULL;
    }

    if (NULL != m_pcMsgController) {
        delete m_pcMsgController;
       m_pcMsgController = NULL;
    }
    if (NULL != m_pcCatalogController) {
        delete m_pcCatalogController;
        m_pcCatalogController = NULL;
    }
    if (NULL != m_pcVoiceBoxDialogEngineNull) {
        delete m_pcVoiceBoxDialogEngineNull;
        m_pcVoiceBoxDialogEngineNull = NULL;
    }
}

// Process the messages that received from external service or self
bool VR_VoiceBoxDialogEngine::SendMessage(const std::string& message, int actionSeqId)
{
    VR_LOGI("VR_VoiceBoxDialogEngine SendMessage: %s\n", message.c_str());
    if (m_bIsNullEngine) {
        VR_LOGI("Now the VoiceBox DialogEngine is NullEngine , and NullEngine will handle Message..");
        if (NULL != m_pcVoiceBoxDialogEngineNull) {
            return m_pcVoiceBoxDialogEngineNull->SendMessage(message, actionSeqId);
        }
        return false;
    }

    if ((NULL == m_pcVoiceBoxEngine) || (NULL == m_pcMsgController)) {
        // If the dialog engine or message controller thread does not exist,
        // We could not process the messages, so we just return.
        return false;
    }

    // Post message to the message controller,
    // the controller will dispatch the XML message.
    if (!m_pcMsgController->PostMessage(message, actionSeqId)) {
        return false;
    }

    VR_LOGI("VR_VoiceBoxDialogEngine SendMessage successfully\n");
    return true;
}


bool
VR_VoiceBoxDialogEngine::LoadVersionInfo(const std::string& strVersionPath, std::string& strVersion)
{
    const size_t VERSION_MAX_LEN = 64;
    char version[VERSION_MAX_LEN] = { 0 };
    FILE *pFile = fopen(strVersionPath.c_str(), "r");
    if (NULL == pFile) {
        VR_LOGI("Open the version file failed");
        return false;
    }

    size_t result = fread(version, 1, VERSION_MAX_LEN, pFile);
    fclose(pFile);
    if (0 == result) {
        VR_LOGI("Read the version file failed");
        return false;
    }

    strVersion = version;
    return true;
}

bool VR_VoiceBoxDialogEngine::LoadVDataVersionInfo(const std::string& strVersionPath, std::string& strVersion)
{
    std::ifstream fin(strVersionPath.c_str());
    if (!fin) {
        VR_LOGI("cannnot open version file !!");
        return false;
    }
    std::stringstream ss;
    ss << fin.rdbuf();


    pugi::xml_document doc;
    if (!doc.load(ss.str().c_str())) {
        VR_LOGI("cannnot load version file !!");
        return false;
    }

    std::string strFullVersion = doc.select_node("//version").node().child_value();
    // get version which VBT SDK version
    const size_t posBegin = 2;
    const size_t sublen = 3;
    const size_t length = 8;
    if (length != strFullVersion.size()) {
        VR_LOGI(" version.xml file is not correct!!");
        return false;
    }
    strVersion = strFullVersion.substr(posBegin, sublen);

    return true;
}

bool VR_VoiceBoxDialogEngine::CheckVDataUpdate(bool &isUpdate)
{
    VR_LOGD_FUNC();
    // firstly, check VDATA version.xml info
    std::string strBackupVersion;
    std::string strCurrentVersion;

    BL_File pathFile(BL_FILE_PREFIX_VDATA);
    BL_String versionFullPath;
    pathFile.GetLogicalPath("version.xml", versionFullPath);

    std::string strVBTDataVersionPath(versionFullPath.GetString());
    if (!LoadVDataVersionInfo(strVBTDataVersionPath, strBackupVersion)) {
        // If the backup data is invalid, we should not copy it.
        VR_LOGI("Load backup data version.xml failed");
        return false;
    }

    // if load current version success, we need to compare it with backup version
    LoadVDataVersionInfo("/data/vr/data/version.xml", strCurrentVersion);
    VR_LOGI("strBackupVersion = %s, strCurrentVersion = %s", strBackupVersion.c_str(), strCurrentVersion.c_str());
    if (strBackupVersion != strCurrentVersion) {
        isUpdate = true;
        return true;
    }

    // secondly, check /data/vr/data/version of VBT SDK
    strBackupVersion.clear();
    std::string strVBTDataPath = VR_ConfigureIF::Instance()->getDataPath();
    strVBTDataPath.append("data/version");
    if (!LoadVersionInfo(strVBTDataPath, strBackupVersion)) {
        // If the backup data is invalid, we should not copy it.
        VR_LOGI("Load backup VBT SDK data version failed");
        return false;
    }

    strCurrentVersion.clear();
    LoadVersionInfo("/data/vr/data/version", strCurrentVersion);
    VR_LOGI("strBackupVersion = %s, strCurrentVersion = %s", strBackupVersion.c_str(), strCurrentVersion.c_str());
    if (strBackupVersion != strCurrentVersion) {
        isUpdate = true;
        return true;
    }

    return true;
}

std::string
VR_VoiceBoxDialogEngine::getHints(const std::string& hintsParams)
{

    if (m_bIsNullEngine || NULL == m_pcVoiceBoxEngine) {
        return "";
    }
    else {
        return m_pcVoiceBoxEngine->getHints(hintsParams);
    }
}

/* EOF */
