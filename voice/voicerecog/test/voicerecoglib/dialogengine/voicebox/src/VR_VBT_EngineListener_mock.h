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
 * @file VR_VBT_EngineListener_mock.h
 * @brief Mock class for VR_DialogEngineListener.
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_VBT_ENGINELISTENER_MOCK_H
#define VR_VBT_ENGINELISTENER_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif
#include <string>
#include "gmock/gmock.h"
#include "VR_DialogEngineListener.h"

/**
* MockVR_VBT_EngineListener
*
* The class is a mock class for VR_DialogEngineListener.
*/
class MockVR_DialogEngineListener : public VR_DialogEngineListener
{
public:
    // mock methods
    MOCK_METHOD2(OnRequestAction, void(const std::string& message, int seqId));
    MOCK_METHOD0(OnStarted, void());
    MOCK_METHOD0(OnStopped, void());
};

#endif  // VR_VBT_ENGINELISTENER_MOCK_H
/* EOF */
