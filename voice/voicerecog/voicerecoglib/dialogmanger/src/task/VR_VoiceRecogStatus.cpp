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
 * @file RecogTask_State.cpp
 * @brief Declaration file of class VR_RecogTaskStateBase and Derived classes.
 *
 * This file includes the declaration of class VR_RecogTaskStateBase and Derived classes.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"
#include "VR_VoiceRecogStatus.h"

VR_VoiceRecogStatus::VR_VoiceRecogStatus(spVR_DialogEngineIF spDialogEngine)
    : m_bStatus(false)
    , m_spDialogEngine(spDialogEngine)
{
    VR_LOGD_FUNC();
}


VR_VoiceRecogStatus::~VR_VoiceRecogStatus()
{
    VR_LOGD_FUNC();
}

bool   VR_VoiceRecogStatus::GetStatus()  const
{
    VR_LOGD_FUNC();
    return  m_bStatus;
}

void  VR_VoiceRecogStatus::SetStatus(bool bStatus)
{
    VR_LOGD_FUNC();
    m_bStatus = bStatus;
}

void  VR_VoiceRecogStatus::OnRecevieMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    VR_String xmlResult = "<event name=\"startedNotify\"></event>";
    VR_LOGI("SendMessage: [%s]", xmlResult.c_str());
    m_spDialogEngine->SendMessage(xmlResult);
}

void  VR_VoiceRecogStatus::Start()
{
    VR_LOGD_FUNC();
}

void  VR_VoiceRecogStatus::Stop()
{
    VR_LOGD_FUNC();
}
/* EOF */
