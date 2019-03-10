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
 * @file VR_EventSender.cpp
 * @brief Declaration file of class VR_EventSender.
 *
 * This file includes the declaration of class VR_EventSender.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include "VR_EventSender.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"

VR_EventSender::VR_EventSender()
{
    VR_LOGD_FUNC();
}

VR_EventSender::~VR_EventSender()
{
    VR_LOGD_FUNC();
}

void VR_EventSender::SendEvent(const char *msgName, VR_ProtoMessageBase *msg)
{
    VR_LOGI("msgName=[%s]", msgName);
    EV_EventSender().SendEvent(msgName, msg);
}

bool VR_EventSender::Reply(const EV_EventContext &context, VR_ProtoMessageBase*msg)
{
    VR_LOGD_FUNC();
    return EV_EventSender().Reply(context, msg);
}

/* EOF */
