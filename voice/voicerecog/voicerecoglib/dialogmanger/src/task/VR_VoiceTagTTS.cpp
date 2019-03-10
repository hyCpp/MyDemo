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
 * @file VR_VoiceTagTTS.cpp
 * @brief Declaration file of class VR_VoiceTagTTS.
 *
 * This file includes the declaration of class VR_VoiceTagTTS.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include "VR_Configure.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_EventSenderIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_VoiceTagTTS.h"
#include <boost/format.hpp>
#include "VR_ActionEventSender.h"

#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"

using namespace navi::VoiceRecog;

VR_VoiceTagTTS::VR_VoiceTagTTS(spVR_EventSenderIF pEventSender, spVR_DialogEngineIF pDialogSender, const spVR_SessionManger pVR_SessionManger)
    : m_spEngineIF(pDialogSender)
    , m_spEventSenderIF(pEventSender)
    , m_spVR_SessionManger(pVR_SessionManger)
    , m_sender(VR_new VR_ActionEventSender(pDialogSender))
{
    VR_LOGD_FUNC();
}

VR_VoiceTagTTS::~VR_VoiceTagTTS()
{
    VR_LOGD_FUNC();
}

void VR_VoiceTagTTS::Start()
{
    VR_LOGD_FUNC();
}

void VR_VoiceTagTTS::Cancel(const EV_EventContext &context)
{
    VR_LOGD_FUNC();
}

void VR_VoiceTagTTS::Start(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    m_spContext = VR_EventSenderIF::CopyEventContext(&context);
    VR_String session = "";
    const VR_String xml = "<event name=\"longPressed\">"
                                          "</event>";
    m_spVR_SessionManger->CreateSession(session);
    m_spVR_SessionManger->SetCheckVoiceTagSession(m_spContext, session, xml, m_sender, -1, -1);
    m_spVR_SessionManger->OpenSession(session);
}

void VR_VoiceTagTTS::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}
/* EOF */
