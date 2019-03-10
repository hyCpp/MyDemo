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
 * @file NC_SubAudioManager_mock.h                                            
 * @brief Summary for MockNC_SubAudioManager.
 *
 * This file includes the declaration of class MockVC_Player and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef MOCK_NC_SUBAUDIO_MANAGER_H
#define MOCK_NC_SUBAUDIO_MANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "NCSubAudioListener.h"
#include "NCSubAudioManager.h"
#include "NCError.h"
#include "NCString.h"
#include "NCTypesDefine.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

typedef ::nutshell::NCSubAudioManager AUDIO_MANAGER;
using ::nutshell::NC_ERROR;
using ::nutshell::NCString;
using ::nutshell::NC_BOOL;

/**
 * This is a mock class for AUDIO_MANAGER
 *
 * This class is used to mock a object for class 
 * AUDIO_MANAGER
 */
class MockNC_SubAudioManager : public AUDIO_MANAGER
{
public:
    // doesn't interest in the function taking a list as a parameter
    using AUDIO_MANAGER::registerAvm;
    using AUDIO_MANAGER::unregister;

    MOCK_METHOD2(registerAvm, NC_ERROR(const NCString& audioName, ::nutshell::NCSubAudioListener* listener));
    MOCK_METHOD1(unregister, NC_ERROR(const NCString& audioName));
    MOCK_METHOD2(reqAudioSource, NC_ERROR(const NCString& audioName, NC_BOOL on));
    MOCK_METHOD2(cfmAudioStatus, NC_ERROR(const NCString& audioName, const NCString& status));
};

#endif
/* EOF */

