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
#ifndef BL_DATASTORAGE_H
#    include "BL_DataStorage.h"
#endif

#ifndef BL_DIR_H
#    include "BL_Dir.h"
#endif

#include "BL_File.h"

#ifndef VR_VOICEBOXDATASTORAGE_H
#    include "VR_VoiceBoxDataStorage.h"
#endif

#include "VR_Log.h"

// Constructor
VR_VoiceBoxDataStorage::VR_VoiceBoxDataStorage()
{

}

// Destructor
VR_VoiceBoxDataStorage::~VR_VoiceBoxDataStorage()
{
}

void
VR_VoiceBoxDataStorage::GetValue(std::string& deviceVoiceTagValueStr)
{
    BL_DataStorage storage;
    BL_DataValue dataValue;
    VR_LOGI("Save VoiceTag Begin");
    if (storage.GetValue(BL_DATA_KEY_VR_SMARTAGENT, dataValue)) {
        VR_LOGI("GetValue OK");
        const void * dataPoint = dataValue.GetData();
        if (nullptr != dataPoint) {
            deviceVoiceTagValueStr.assign((char*)dataPoint);
        }
    }
    else {
        VR_LOGI("GetValue Failed");
    }

    VR_LOGI("VoiceTag Value: %s", deviceVoiceTagValueStr.c_str());
}

void
VR_VoiceBoxDataStorage::PutValue(std::string& deviceVoiceTagValueStr)
{
    BL_DataStorage storage;
    if (storage.PutValue(BL_DATA_KEY_VR_SMARTAGENT, (void*)(deviceVoiceTagValueStr.c_str()), deviceVoiceTagValueStr.size() + 1)) {
        VR_LOGI("PutValue OK");
    }
    else {
        VR_LOGI("PutValue Failed");
    }
}

void
VR_VoiceBoxDataStorage::GetValue(int key, std::string& deviceVoiceTagValueStr)
{
    BL_DataStorage storage;
    BL_DataValue dataValue;
    VR_LOGI("Save VoiceTag Begin");
    if (storage.GetValue((BL_DATA_KEY_NAME)key, dataValue)) {
        VR_LOGI("GetValue OK");
        const void * dataPoint = dataValue.GetData();
        if (nullptr != dataPoint) {
            deviceVoiceTagValueStr.assign((char*)dataPoint);
        }
    }
    else {
        VR_LOGI("GetValue Failed");
    }

    VR_LOGI("VoiceTag Value: %s", deviceVoiceTagValueStr.c_str());
}

void
VR_VoiceBoxDataStorage::PutValue(int key, std::string& deviceVoiceTagValueStr)
{
    BL_DataStorage storage;
    if (storage.PutValue((BL_DATA_KEY_NAME)key, (void*)(deviceVoiceTagValueStr.c_str()), deviceVoiceTagValueStr.size() + 1)) {
        VR_LOGI("PutValue OK");
    }
    else {
        VR_LOGI("PutValue Failed");
    }
}

void
VR_VoiceBoxDataStorage::PutValue(int key, void* pData, int size)
{
    BL_DataStorage storage;
    storage.PutValue((BL_DATA_KEY_NAME)key, pData, size);
}

void
VR_VoiceBoxDataStorage::GetValue(int key, void* pData, int& size)
{
    BL_DataStorage storage;
    storage.GetValue((BL_DATA_KEY_NAME)key, pData, size);
}

void
VR_VoiceBoxDataStorage::DeleteValue()
{
    BL_DataStorage storage;
    storage.DeleteValue(BL_DATA_KEY_VR_SMARTAGENT);
}

void
VR_VoiceBoxDataStorage::DeleteValue(int key)
{
    BL_DataStorage storage;
    storage.DeleteValue((BL_DATA_KEY_NAME)key);
}

void
VR_VoiceBoxDataStorage::RemoveDir(const std::string &cDirName)
{
    BL_Dir dataDir(BL_FILE_PREFIX_RW);
    dataDir.RemoveDir(cDirName.c_str());
}

void
VR_VoiceBoxDataStorage::RemoveFile(const std::string &cDirName)
{
    VR_LOGI("RemoveFile");
    BL_File file(BL_FILE_PREFIX_RW);
    // file.RemoveFile(BL_String(cDirName.c_str()));
    bool isRemoved = file.RemoveFile(BL_String(cDirName.c_str()));
    VR_LOGI("Remove File: %s, %s", cDirName.c_str(), isRemoved ? "true" : "false");
    /* if (file.IsFileExist(cDirName.c_str())) {
        bool isRemoved = file.RemoveFile(BL_String(cDirName.c_str()));
        VR_LOGI("Remove File: %s, %s", cDirName.c_str(), isRemoved ? "true" : "false");
    } */
}

void
VR_VoiceBoxDataStorage::GetGrammarStatusValue(unsigned int &statusvalue)
{
    BL_DataStorage storage;
    BL_DataValue dataValue;
    if (storage.GetValue(BL_DATA_KEY_VBT_UPDATE_STATE, dataValue)) {
        VR_LOGI("GetValue OK");
        const void * dataPoint = dataValue.GetData();
        if (nullptr != dataPoint) {
            statusvalue = *((unsigned int*)dataPoint);
        }
    }
    else {
        VR_LOGI("GetValue Failed");
    }
    VR_LOGI("Grammar status is : %d", statusvalue);
}

void
VR_VoiceBoxDataStorage::PutGrammarStatusValue(unsigned int statusvalue)
{
    BL_DataStorage storage;
    if (storage.PutValue(BL_DATA_KEY_VBT_UPDATE_STATE, (void*)(&statusvalue), sizeof(statusvalue))) {
        VR_LOGI("PutValue OK, Now Grammar status is : %d", statusvalue);
    }
    else {
        VR_LOGI("PutValue Failed");
    }
}

void
VR_VoiceBoxDataStorage::PutInstallTslAgent(const std::string& strInstall)
{
    BL_DataStorage storage;
    if (storage.PutValue(BL_DATA_KEY_VBT_TSL_INSTALL, (void*)(strInstall.c_str()), strInstall.size() + 1)) {
        VR_LOGI("PutValue OK");
    }
    else {
        VR_LOGI("PutValue Failed");
    }
}

void
VR_VoiceBoxDataStorage::GetInstallTslAgent(std::string& strInstall)
{
    BL_DataStorage storage;
    BL_DataValue dataValue;
    VR_LOGI("Save install Begin");
    if (storage.GetValue(BL_DATA_KEY_VBT_TSL_INSTALL, dataValue)) {
        VR_LOGI("GetValue OK");
        const void * dataPoint = dataValue.GetData();
        if (nullptr != dataPoint) {
            strInstall.assign((char*)dataPoint);
        }
    }
    else {
        VR_LOGI("GetValue Failed");
    }
    VR_LOGI("install Value: %s", strInstall.c_str());
}

/* EOF */
