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
 * @file VR_ActionEventSender.h
 * @brief Declaration file of class VR_ActionEventSender.
 *
 * This file includes the declaration of class VR_ActionEventSender.
 *
 * @attention used for C++ only.
 */

#ifndef XX_VR_ACTIONEVENTSENDER_H
#define XX_VR_ACTIONEVENTSENDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
#include "gmock/gmock.h"
#include "action/VR_ActionEventSender.h"

/**
 * @brief The VR_ActionEventSender class
 *
 * class declaration
 */
class VR_ActionEventSender_mock : public VR_ActionEventSender
{
public:
    VR_ActionEventSender_mock(spVR_DialogEngineIF spEngine)
        : VR_ActionEventSender(spEngine)
    {

    }

    virtual ~VR_ActionEventSender_mock()
    {

    }

    MOCK_METHOD3(sendActionRequest, void(int id, int actionType,  const VR_ProtoMessageBase &msg));
    MOCK_METHOD2(sendInfoRequest, void(int id, const VR_ProtoMessageBase &msg));
    MOCK_METHOD2(sendSpecialInfoRequest, void(int id, const VR_ProtoMessageBase &msg));
    MOCK_METHOD2(sendMessageToFC, void(const char* protoTag, VR_ProtoMessageBase *msg));

    MOCK_METHOD2(sendEngineReply, void(int id,  const VR_ProtoMessageBase &msg));
    MOCK_METHOD2(sendActionReply, void(int id, const VR_ProtoMessageBase& msg));
    MOCK_METHOD2(sendEngineReply, void(int id,  const std::string &msg));

};

#endif /* XX_VR_ACTIONEVENTSENDER_H */
/* EOF */
