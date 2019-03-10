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
 * @file VR_DialogEngineIF.h
 * @brief interface for DialogEngineIF.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_DIALOGENGINEIF_MOCK_H
#define VR_DIALOGENGINEIF_MOCK_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "gmock/gmock.h"
#include "VR_DialogEngineIF.h"

/**
 * @brief The VR_DialogEngineIF_mock class
 *
 * class declaration
 */
class VR_DialogEngineIF_mock : public VR_DialogEngineIF
{
public:
    virtual ~VR_DialogEngineIF_mock() {}

    // sync
    MOCK_METHOD2(Initialize, bool(VR_DialogEngineListener *listener, const VR_Settings &settings));

    MOCK_METHOD1(getHints, std::string(const std::string& hintsParams));

    virtual bool Start()
    {

    }

    // async
    virtual void Stop()
    {

    }

    // async
    MOCK_METHOD2(SendMessage, bool(const std::string& message, int actionSeqId));

    // sync
    MOCK_METHOD0(UnInitialize,  void());
};

#endif // VR_DIALOGENGINEIF_MOCK_H
/* EOF */
