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
 * @file VR_VBT_Controller_mock.h
 * @brief Mock class for VR_VoiceBoxController.
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_VBT_CONTROLLER_MOCK_H
#define VR_VBT_CONTROLLER_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif
#include <string>
#include "gmock/gmock.h"
#include "VR_VoiceBoxController.h"

/**
* MockVR_VBT_Controller
*
* The class is a mock class for VR_VoiceBoxController.
*/
class MockVR_VBT_Controller : public VR_VoiceBoxController
{
public:
    // mock methods
    MOCK_METHOD2(PostMessage, bool(const std::string& message, int actionSeqId));
    MOCK_METHOD2(PushFrontMessage, bool(const std::string& message, int actionSeqId));
    MOCK_METHOD0(ClearMessage, void());
    MOCK_METHOD2(PostStatusMsg, bool(const std::string& message, int actionSeqId));
    MOCK_METHOD0(GetCurrentMessage, std::string());
};

#endif // VR_VBT_CONTROLLER_MOCK_H
/* EOF */
