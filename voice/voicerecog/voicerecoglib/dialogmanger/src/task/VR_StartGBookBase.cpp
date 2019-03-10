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
 * @file  VR_StartGBookBase.h
 * @brief Declaration file of class VR_StartGBookBase.
 *
 * This file includes the declaration of class VR_StartGBookBase.
 *
 * @attention used for C++ only.
 */

#include  "stdafx.h"
#include  "VR_StartGBookBase.h"

VR_StartGBookBase::VR_StartGBookBase(spVR_EventSenderIF spEventSender, spVR_DialogEngineIF spDialogEngine)
    : m_spEventSender(spEventSender)
    , m_spDialogEngine(spDialogEngine)
{
    VR_LOGD_FUNC();
}

VR_StartGBookBase::~VR_StartGBookBase()
{
    VR_LOGD_FUNC();
}

/* EOF */
