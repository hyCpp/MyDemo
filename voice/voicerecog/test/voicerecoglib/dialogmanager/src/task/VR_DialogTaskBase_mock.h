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
 * @file VR_VBT_EngineCallback_mock.h
 * @brief Mock class for IVECIEngineClient.
 *
 *
 * @attention used for C++ only.
 */

#ifndef CXX_VR_DIALOGTASKBASE_MOCK_H
#define CXX_VR_DIALOGTASKBASE_MOCK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "VR_DialogTaskBase.h"
#include "EV_EventContext.h"

/**
* VR_DialogTaskBase_Mock
*
* The class is a mock class for VR_DialogTaskBase_Mock.
*/
class VR_DialogTaskBase_Mock: public  VR_DialogTaskBase
{
public:
    // Mock methods
    MOCK_CONST_METHOD0(GetTaskID, int());
    MOCK_CONST_METHOD0(IsComplete, bool());
    MOCK_METHOD0(Start, void());
    MOCK_METHOD1(Cancel, void(const EV_EventContext &context));
    MOCK_METHOD1(OnMessage, void(const VR_ProtoMessageBase &msg));
};

#endif /* CXX_VR_DIALOGTASKBASE_MOCK_H */
/* EOF */