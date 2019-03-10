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
#include "stdafx.h"
#include "VR_DialogTaskBase.h"

VR_DialogTaskBase::VR_DialogTaskBase()
    : m_bIsComplete(false)
{
    VR_LOGD_FUNC();
}

VR_DialogTaskBase::~VR_DialogTaskBase()
{
    VR_LOGD_FUNC();
}

int VR_DialogTaskBase::GetTaskID() const
{
    VR_LOGD_FUNC();
    return m_taskId.GetID();
}

bool VR_DialogTaskBase::IsComplete() const
{
    VR_LOGD_FUNC();
    return m_bIsComplete;
}

void VR_DialogTaskBase::Start(const EV_EventContext &, const VR_ProtoMessageBase &)
{
    VR_LOGD_FUNC();
}

void VR_DialogTaskBase::UserOperation(const EV_EventContext &context, const VR_String &type)
{
    VR_LOGI("type=[%s]", type.c_str());
    VR_UNUSED_VAR(context);
    VR_UNUSED_VAR(type);
}

void VR_DialogTaskBase::OnRequestVrTsl(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

bool VR_DialogTaskBase::SetComplete()
{
    VR_LOGD_FUNC();
    return (m_bIsComplete = true);
}

// DialogTask null's define
VR_DialogTaskNull::VR_DialogTaskNull()
{
    VR_LOGD_FUNC();
}

VR_DialogTaskNull::~VR_DialogTaskNull()
{
    VR_LOGD_FUNC();
}

void VR_DialogTaskNull::Start()
{
    VR_LOGD_FUNC();
}

void VR_DialogTaskNull::Cancel(const EV_EventContext &context)
{
    VR_LOGD_FUNC();
}

void VR_DialogTaskNull::UserOperation(const EV_EventContext &context, const VR_String &type)
{
    VR_LOGD_FUNC();
}

void VR_DialogTaskNull::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_DialogTaskNull::OnRequestVrTsl(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

/* EOF */
