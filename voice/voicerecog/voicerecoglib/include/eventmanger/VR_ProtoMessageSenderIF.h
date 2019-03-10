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
 * @file VR_ProtoMessageSenderIF.h
 * @brief Declaration file of class VR_ProtoMessageSenderIF.
 *
 * This file includes the declaration of class VR_ProtoMessageSenderIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_PROTOMESSAGESENDERIF_H
#define VR_PROTOMESSAGESENDERIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Macro.h"
#include "VR_Def.h"

#include <boost/function.hpp>

class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ProtoMessageSenderIF);

/**
 * @brief The VR_ProtoMessageSenderIF class
 *
 * class declaration
 */
class VR_ProtoMessageSenderIF
{
public:
    virtual ~VR_ProtoMessageSenderIF();
    virtual void SendEvent(VR_ProtoMessageBase* pMsg, const EV_EventContext* pContext = NULL) = 0;
    virtual void SendReplyEvent(VR_ProtoMessageBase* pMsg, const EV_EventContext* pContext = NULL) = 0;
};

#endif /* VR_PROTOMESSAGESENDERIF_H */
/* EOF */
