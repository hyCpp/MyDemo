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
 * @file VR_Configure_mock.h
 * @brief inteface for interpeter or asr engine to perfer command
 *
 *
 * @attention used for C++ only.
 */
#ifndef VRSERVICEREQUESTOR_MOCK_H
#define VRSERVICEREQUESTOR_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "uscxml/plugins/invoker/vr/VRServiceRequestor.h"

/**
* VR_Configure_mock
*
* The class is a mock class for VRServiceRequestor.
*/

class VRServiceRequestor_mock : public VRServiceRequestor
{
public:
    MOCK_METHOD1(requestService, void(const uscxml::Event& interEvent));
};

#endif
/* EOF */
