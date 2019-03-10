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

/**
 * @file VR_DialogEngineAsrDataManager.h
 * @brief Declaration file of VR_DialogEngineAsrDataManager.
 *
 * This file includes the declaration of VR_DialogEngineAsrDataManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DIALOGENGINEASRDATAMANAGER_H
#define VR_DIALOGENGINEASRDATAMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_MsgDefine.h"

/**
 * @brief The VR_DialogEngineAsrDataManager class
 *
 * provide interface for asr data contrl
 */

class VR_DialogEngineAsrDataManager
{
    public:
        enum class VMPLoadState
        {
            NONE,
            LOADING,
            LOADFAIL,
            LOADSUCCESS,
        };

        enum class MagicStringValueState
        {
            INVALID_VALUE,
            VALID_VALUE,
            NULL_VALUE,
        };

        VR_DialogEngineAsrDataManager();

        ~VR_DialogEngineAsrDataManager();

        // reset VMP status
        void resetAsrVMPStatus(const std::string& asrLanguage);

        // return if vmp data exists
        bool isVmpDataExists();

        bool getAsrVMPStatus();
        void onCallBackSetVMPState(const std::string& magicString, bool loadStatus);
        VR_DialogEngineAsrDataManager::VMPLoadState getVMPLoadState();
        const std::string& getCurrentLoadingMagicString();

        VR_DialogEngineAsrDataManager::MagicStringValueState checkMagicStringAvalible(const std::string& newMagicString);
        void setVMPLoadState(const VMPLoadState& state, const std::string& loadingMagicString);

    protected:

    private:
        // check gracenote asr clc is exists in data
        bool checkAsrClcExists(const std::string& clcPath);

        // get asr clc data path
        const std::string getAsrVDATAPath();

        // path exist check
        bool PathAccess(const std::string& Path);

    private:
        bool m_bVMPDataExists; // is asr vmp data exist
        VMPLoadState m_loadStatus;
        std::string m_currentLoadMagicString;
        std::string m_lastLoadedMagicString;
};

#endif // VR_DIALOGENGINEASRDATAMANAGER_H
/* EOF */
