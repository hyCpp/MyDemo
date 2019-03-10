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
 * @file VR_DialogEngineListener_mock.h
 * @brief inteface for interpeter or asr engine to perfer command
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_DIALOGENGINELISTENER_MOCK_H
#define VR_DIALOGENGINELISTENER_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_DialogEngineListener.h"

/**
* VR_DialogEngineListener_mock
*
* The class is a mock class for VR_DialogEngineListener.
*/
class VR_DialogEngineListener_mock : public VR_DialogEngineListener
{
public:
    virtual ~VR_DialogEngineListener_mock() {}
    VR_DialogEngineListener_mock() : VR_DialogEngineListener()
    {
    }

    MOCK_METHOD2(OnRequestAction, void(const std::string& message, int seqId));
    MOCK_METHOD0(OnStarted, void());
    MOCK_METHOD0(OnStopped, void());
};

#endif
/* EOF */
