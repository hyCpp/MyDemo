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
 * @file  VR_DialogEngine_mock.h
 * @brief Mock class for VR_DialogEngineIF.
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_DIALOGENGINE_MOCK_H
#define VR_DIALOGENGINE_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "VR_DialogEngineIF.h"
#include "EV_EventContext.h"

/**
*  VR_DialogEngine_mock
*
* The class is a mock class for VR_DialogEngine_mock.
*/
class VR_DialogEngine_mock : public  VR_DialogEngineIF
{
public:
    // Mock methods
    MOCK_METHOD2(Initialize, bool(VR_DialogEngineListener *listener, const VR_Settings &settings));
    MOCK_METHOD1(getHints, std::string(const std::string& hintsParams));
    MOCK_METHOD0(Start, bool());
    MOCK_METHOD0(Stop,  void());
    MOCK_METHOD2(SendMessage,  bool(const std::string& message, int actionSeqId));
    MOCK_METHOD0(UnInitialize, void());
};

#endif /* VR_DIALOGENGINE_MOCK_H */
/* EOF */
