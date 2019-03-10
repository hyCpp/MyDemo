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
 * @file VR_VBT_EngineIF_mock.h
 * @brief Mock class for VR_VoiceBoxEngineIF
 *
 *
 * @attention used for C++ only.
 */

#ifndef CXX_VR_VBT_ENGINEIF_MOCK_H
#define CXX_VR_VBT_ENGINEIF_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include <string>
#include "gmock/gmock.h"
#include "VR_VoiceBoxEngineIF.h"

/**
* MockVR_VBT_EngineIF
*
* The class is a mock class for VR_VoiceBoxEngineIF.
*/

class MockVR_VBT_EngineIF : public VR_VoiceBoxEngineIF
{
public:
    // Mock methods
    MOCK_METHOD3(Initialize, bool(VR_DialogEngineListener* pcExternalCallback,
                VR_VoiceBoxController*  pcMsgController,
                VR_VoiceBoxController*  pcCatalogController));
    MOCK_METHOD1(Start, bool(const std::string &strCultureName));
    MOCK_METHOD0(Stop, void());
    MOCK_METHOD0(UnInitialize, void());
    MOCK_METHOD2(ProcessMessage, void(const std::string& message, int actionSeqId));
    MOCK_METHOD0(GetCatalogManager, VR_VoiceBoxCatalogManager*());
    MOCK_METHOD1(setPromptLevel, void(int promptLevel));
    MOCK_METHOD1(getHints, std::string(const std::string& hintsParams));
    MOCK_METHOD1(setVDATAUpdateStatus, void(const enVDataStatus& vdataStatus));
    MOCK_METHOD2(SendDEStatus, void(const std::string& strType, const std::string& strStatus));
};

#endif  // CXX_VR_VBT_ENGINEIF_MOCK_H
/* EOF */
