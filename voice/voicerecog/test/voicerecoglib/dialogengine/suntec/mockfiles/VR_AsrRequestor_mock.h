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
 * @file VR_AsrRequestor_mock.h
 * @brief Declaration file of VR_AsrRequestor_mock.
 *
 * This file includes the declaration of VR_AsrRequestor_mock.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ASRREQUESTOR_MOCK_H
#define VR_ASRREQUESTOR_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_AsrRequestor.h"

/**
* VR_AsrRequestor_mock
*
* The class is a mock class for VR_AsrRequestor.
*/
class VR_AsrRequestor_mock : public VR_AsrRequestor
{
public:
    VR_AsrRequestor_mock()
    : VR_AsrRequestor()
    {
    }

    MOCK_METHOD1(updateGrammar, bool(N_Vr::N_Asr::C_Request_Context_List_Update&));
    MOCK_METHOD0(genVoiceTagPhoneme, bool());
    MOCK_METHOD3(setGrammarActive, void(const std::string &, bool, const VoiceList<std::string>::type &));
    MOCK_METHOD1(updateGrammarCategoryFinish, void(const std::string &category));
    MOCK_METHOD0(updateGrammarFinish, void());
};

#endif
/* EOF */
