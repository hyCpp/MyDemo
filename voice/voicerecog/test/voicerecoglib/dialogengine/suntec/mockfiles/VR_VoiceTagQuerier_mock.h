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
 * @file VR_VoiceTagQuerier_mock.h
 * @brief Declaration file of VR_VoiceTagQuerier_mock.
 *
 * This file includes the declaration of VR_VoiceTagQuerier_mock.
 *
 * @attention used for C++ only.
 */

#ifndef VR_VOICETAGQUERIER_MOCK_H
#define VR_VOICETAGQUERIER_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_VoiceTagManager.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


/**
* VR_VoiceTagQuerier_mock
*
* The class is a mock class for VR_VoiceTagQuerier.
*/
class VR_VoiceTagQuerier_mock : public VR_VoiceTagQuerier
{
public:
    MOCK_METHOD1(getContactInfo, std::string(const std::string &));
    MOCK_METHOD0(getVoiceTagCount, int());
    MOCK_METHOD1(getContactIDByVoiceTagID, std::string(const std::string &voiceTagID));
    MOCK_METHOD1(getVoiceTagIDByContactID, std::string(const std::string &contactID));
};

#endif
/* EOF */