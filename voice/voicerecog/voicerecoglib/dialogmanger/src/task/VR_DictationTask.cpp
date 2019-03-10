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

#include <memory>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"

#include "VR_DictationTask.h"
#include "VR_DialogEngineIF.h"
#include "VR_DailogEngineMessageBuilder.h"
#include "navi/Voice/VoiceRecog/RespVrCancelRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/RespVrStartRecognitionResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"

using namespace navi::VoiceRecog;


void VR_DictationTask::Start()
{
    VR_LOGD_FUNC();
}

void VR_DictationTask::Cancel(const EV_EventContext &context)
{
    VR_LOGD_FUNC();
}

void VR_DictationTask::OnMessage(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();
    const VrEngineControlMessage& message = (const VrEngineControlMessage&)msg;

    switch (message.type()) {
    case DMTaskMessageType_StartEnd :
        break;
//    case TaskState_OnNotifyComplete: {
//        this->OnNotifyDictationResult();
//        break;
//    }
    default:
        break;
    }
}

void VR_DictationTask::OnNotifyDictationResult()
{
    VR_LOGD_FUNC();
}

void VR_DictationTask::OnNotifyState(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

/* EOF */
