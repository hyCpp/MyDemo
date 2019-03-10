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
* @file  VR_NaviStatusMoniter.cpp
* @brief Declaration file of class VR_NaviStatusMoniter.
*
* This file includes the declaration of class VR_NaviStatusMoniter.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "VR_NaviStatusMoniter.h"

VR_NaviStatusMoniter::VR_NaviStatusMoniter(spVR_DialogEngineIF dialogEngine)
    : m_spDialogEngine(dialogEngine)
{
    VR_LOGD_FUNC();
}

VR_NaviStatusMoniter::~VR_NaviStatusMoniter()
{
    VR_LOGD_FUNC();
}

void VR_NaviStatusMoniter::Start()
{
    VR_LOGD_FUNC();
}

void VR_NaviStatusMoniter::Stop()
{
    VR_LOGD_FUNC();
}

void VR_NaviStatusMoniter::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

/* EOF */
