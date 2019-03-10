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
 * @file VP_NativeAudioManager.h
 * @brief Summary for VP_NativeAudioManager
 *
 * This file includes the declaration of class VP_NativeAudioManager and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_NATIVE_AUDIOMGR_H
#define VP_NATIVE_AUDIOMGR_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#pragma once

#include <string>
#include <memory>

class VP_AudioManagerListener;


/**
 * class used to mock audio manager of media module
 *
 * This class is a class used to mock audio manager
 */
class VP_NativeAudioManager
{
public:
    VP_NativeAudioManager();

    bool registerAvm(const std::string& audioName, VP_AudioManagerListener* listener);

    bool unregister(const std::string& audioName);

    bool reqAudioSource(const std::string& audioName, bool on);

    bool cfmAudioStatus(const std::string& audioName, const std::string& status);

private:
    VP_NativeAudioManager(const VP_NativeAudioManager& rhs);
    VP_NativeAudioManager& operator=(const VP_NativeAudioManager& rhs);
    VP_AudioManagerListener* m_pListener;
};

#endif      // end of VP_NATIVE_AUDIOMGR_H
/* EOF */
