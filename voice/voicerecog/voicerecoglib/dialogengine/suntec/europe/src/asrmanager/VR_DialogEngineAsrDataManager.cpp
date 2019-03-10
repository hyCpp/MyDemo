/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
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

#include "VR_DialogEngineAsrDataManager.h"
#include "VR_Log.h"
#include "VR_ConfigureIF.h"

VR_DialogEngineAsrDataManager::VR_DialogEngineAsrDataManager()
     :m_bVMPDataExists(false),
      m_loadStatus(VMPLoadState::NONE)
{
    VR_LOGD_FUNC();

    m_lastLoadedMagicString.clear();
    m_currentLoadMagicString.clear();
}

VR_DialogEngineAsrDataManager::~VR_DialogEngineAsrDataManager()
{
    VR_LOGD_FUNC();
}

bool VR_DialogEngineAsrDataManager::checkAsrClcExists(const std::string& clcPath)
{
    VR_LOGD_FUNC();

    if (clcPath.empty()) {
        return false;
    }
    return PathAccess(clcPath);
}


const std::string VR_DialogEngineAsrDataManager::getAsrVDATAPath()
{
    VR_LOGD_FUNC();
    return VR_ConfigureIF::Instance()->getAsrDataPath();
}


bool VR_DialogEngineAsrDataManager::PathAccess(const std::string& Path)
{
    return (::access(Path.c_str(), 0) == 0);
}


void VR_DialogEngineAsrDataManager::resetAsrVMPStatus(const std::string &asrLanguage)
{
    VR_LOGD_FUNC();

    m_bVMPDataExists = false;
    m_loadStatus = VMPLoadState::NONE;

    m_lastLoadedMagicString.clear();
    m_currentLoadMagicString.clear();

    if (asrLanguage.empty()) {
        VR_LOGE("VR ASR language is NULL");
        return;
    }

    // check asr gracenote data status
    std::string vmpClcPath = getAsrVDATAPath() + "Model/Language_" + asrLanguage + '/' + VR_DE_ASR_VMPCLC_NAME;
    m_bVMPDataExists = checkAsrClcExists(vmpClcPath);

}

// gracenote clc data exist status
bool VR_DialogEngineAsrDataManager::isVmpDataExists()
{
    return m_bVMPDataExists;
}

void VR_DialogEngineAsrDataManager::onCallBackSetVMPState(const std::string& magicString, bool loadStatus)
{
    VR_LOGD_FUNC();
    m_loadStatus = VMPLoadState::LOADFAIL;
    m_lastLoadedMagicString = magicString;

    // if asr load successful save the magic string
    if (loadStatus) {
        m_loadStatus = VMPLoadState::LOADSUCCESS;
    }
}

bool VR_DialogEngineAsrDataManager::getAsrVMPStatus()
{
    return m_loadStatus == VMPLoadState::LOADSUCCESS;
}

VR_DialogEngineAsrDataManager::MagicStringValueState VR_DialogEngineAsrDataManager::checkMagicStringAvalible(const std::string& newMagicString)
{
    if (newMagicString.empty()) {
        return MagicStringValueState::NULL_VALUE;
    }

    if (VMPLoadState::NONE == m_loadStatus) {
        return MagicStringValueState::VALID_VALUE;
    }
    else if (VMPLoadState::LOADING == m_loadStatus) {
        // check the next magic string is same with first
        if (newMagicString == m_currentLoadMagicString) {
            return MagicStringValueState::INVALID_VALUE;
        }
    }
    else if (VMPLoadState::LOADFAIL == m_loadStatus
        || VMPLoadState::LOADSUCCESS == m_loadStatus) {
        // check the next magic string is same with loaded magicString
        if (newMagicString == m_lastLoadedMagicString) {
            return MagicStringValueState::INVALID_VALUE;
        }
    }
    else {

    }
    return MagicStringValueState::VALID_VALUE;
}


void VR_DialogEngineAsrDataManager::setVMPLoadState(const VMPLoadState& state, const std::string& loadingMagicString)
{
    m_loadStatus = state;
    m_currentLoadMagicString = loadingMagicString;

    if (VMPLoadState::LOADFAIL == state) {
        m_lastLoadedMagicString = loadingMagicString;
    }
}

VR_DialogEngineAsrDataManager::VMPLoadState VR_DialogEngineAsrDataManager::getVMPLoadState()
{
    return m_loadStatus;
}


const std::string& VR_DialogEngineAsrDataManager::getCurrentLoadingMagicString()
{
    return m_currentLoadMagicString;
}

/* EOF */
