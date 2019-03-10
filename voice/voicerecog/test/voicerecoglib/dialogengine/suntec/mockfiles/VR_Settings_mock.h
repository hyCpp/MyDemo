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

/**
 * @file VR_Settings_mock.h
 * @brief inteface for interpeter or asr engine to perfer command
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_SETTINGS_MOCK_H
#define VR_SETTINGS_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_DialogEngineIF.h"

/**
* VR_Settings_mock
*
* The class is a mock class for VR_Settings.
*/
class VR_Settings_mock : public VR_Settings
{
public:
    virtual ~VR_Settings_mock() {}
    VR_Settings_mock() : VR_Settings_mock()
    {
        language = "";
        country = "";
        naviExist = true;
    }

};

#endif
/* EOF */
