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
 * @file VR_ActionBase.h
 * @brief Declaration file of class VR_ActionBase.
 *
 * This file includes the declaration of class VR_ActionBase.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONBASE_MOCK_H
#define VR_ACTIONBASE_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "action/VR_ActionBase.h"
#include "VR_SessionManger.h"
#include "action/VR_ActionEventSender.h"

/**
 * @brief The VR_ActionBase_mock class
 *
 * class declaration
 */
class VR_ActionBase_mock : public VR_ActionBase
{
public:
    VR_ActionBase_mock(spVR_ActionEventSender sender)
        : VR_ActionBase(sender)
    {
    }

    ~VR_ActionBase_mock()
    {
    }

    MOCK_CONST_METHOD0(GetActionId, int());
    MOCK_CONST_METHOD0(IsActionCompleted, bool());
    MOCK_METHOD0(Run, void());
    MOCK_METHOD1(OnReceive, void(const VR_ProtoMessageBase &proto));
    MOCK_METHOD1(SetSessionManager, void(spVR_SessionManger spSessionManager));
    MOCK_METHOD0(SetActionComplete, void());

};

#endif /* VR_ACTIONBASE_MOCK_H */
/* EOF */
