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
* @file  VR_TtsStatusMoniter.cpp
* @brief Declaration file of class VR_TtsStatusMoniter.
*
* This file includes the declaration of class VR_TtsStatusMoniter.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "boost/format.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_RunableThread.h"
#include "VR_TtsStatusMoniter.h"
#include "servicemanager/NCServiceManagerDefine.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/TTS/TtsServiceStatus.pb.h"

const nutshell::NCString TTS_SPACE     =  nutshell::NCString("nutshell.servicemanager.status");
const nutshell::NCString TTS_IDENTITY  =  nutshell::NCString("nutshell.voice.tts");

using namespace navi::VoiceRecog;
using namespace navi::TTS;

VR_TtsEventHandler::VR_TtsEventHandler(VR_TtsStatusMoniter *pTtsHandle, const nutshell::NCRunnableLooper &looper)
    : nutshell::NCEventHandler(looper)
    , m_pTtsStatusMoniter(pTtsHandle)
    , m_bTtsCrash(false)

{
    VR_LOGD_FUNC();
}

VR_TtsEventHandler::~VR_TtsEventHandler()
{
    VR_LOGD_FUNC();
}

void VR_TtsEventHandler::handleEvent(const nutshell::NCEvent &event)
{
    VR_LOGD_FUNC();
    nutshell::NCString  evtSpace = event.space();
    if (-1 == evtSpace.find(TTS_SPACE.getString())) {
        VR_LOGI("Event space error : [%s]", evtSpace.getString());
        return;
    }

    nutshell::NCString  ncIdentity("");
    event.identity(&ncIdentity);
    VR_INT32 nStatus = event.arg1();
    VR_LOGI("space identity: [%s],nStatus=[%d],m_bTtsCrash=[%d]", ncIdentity.getString(), nStatus, m_bTtsCrash);
    if (TTS_IDENTITY == ncIdentity) {
        if (EVIP_SERVICE_STATUS_READY != nStatus) {
            if (m_bTtsCrash) {
                return;
            }
            m_bTtsCrash = true;
            std::unique_ptr<VrLoopBack>    loopMessage(VrLoopBack::default_instance().New());
            loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
            VrEngineControlMessage*  controlMessage = loopMessage->mutable_m_controlmessage();
            controlMessage->set_type(DMTaskMessageType_StartStatusResult);
            controlMessage->mutable_startstatusresult()->set_starttype(Tts_Crash_Status);
            controlMessage->mutable_startstatusresult()->set_startreuslt(true);
            EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
        }
        else {
            m_bTtsCrash = false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VR_TtsStatusMoniter::VR_TtsStatusMoniter(spVR_RunableThread  spRunnableThread)
    : m_spRunnableThread(spRunnableThread)
    , m_pcEventHandler(NULL)
{
    VR_LOGD_FUNC();
    m_filter.clear();
}

VR_TtsStatusMoniter::~VR_TtsStatusMoniter()
{
    VR_LOGD_FUNC();
}

void VR_TtsStatusMoniter::Start()
{
    VR_LOGD_FUNC();
    m_filter.push_back(m_context.buildEventFullName(TTS_SPACE, TTS_IDENTITY));
    m_pcEventHandler = VR_new VR_TtsEventHandler(this, m_spRunnableThread->getRunableLooper());
    m_context.registerHandler(m_pcEventHandler, m_filter);

    std::unique_ptr<TtsServiceStatus> protoMsg(TtsServiceStatus::default_instance().New());
    protoMsg->set_sender("TTS_MAIN");
    EV_EventSender().SendEvent(EV_EVENT_TTS_SERVICESTATUS, protoMsg.release());
}

void VR_TtsStatusMoniter::Stop()
{
    VR_LOGD_FUNC();
    m_context.unregisterHandler(m_pcEventHandler, m_filter);
    m_filter.clear();
}

/* EOF */
