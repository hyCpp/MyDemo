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

#ifdef  HAVE_NUTSHELL_OS
#   include "NCString.h"
#   include "servicemanager/NCServiceManagerDefine.h"
#   include "nceventsys/NCEvSysContext.h"
#endif

#include "navi/Voice/TTS/TtsServiceStatus.pb.h"

#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif

#include "EV_EventSender.h"
#include "VP_NCEventReceiver.h"
#include "VP_Request.h"
#include "MEM_list.h"

#ifdef  HAVE_NUTSHELL_OS
using ::nutshell::NCEvSysContext;
using ::nutshell::NCEvent;
using ::nutshell::NCRunnableLooper;
#endif
using namespace ::navi::TTS;
using namespace std;

const string VP_NCEventReceiver::TTS_SERVICE = "nutshell.voice.tts";
const string VP_NCEventReceiver::TTS_LISTENER_THREAD = "TTS_LISTENER_THRD";

VP_NCEventReceiver::~VP_NCEventReceiver()
{
#ifdef  HAVE_NUTSHELL_OS
    delete m_thread;
    m_thread = NULL;
#endif
}

void VP_NCEventReceiver::initialize()
{
#ifdef  HAVE_NUTSHELL_OS
    // listen to the status of tts module
    NCEvSysContext evSysContext;
    m_thread = new(MEM_Voice) nutshell::NCRunnableThread();
    m_thread->startThread(TTS_LISTENER_THREAD.c_str());
    // NCRunnableLooper looper = nutshell::NCRunnableThread::getCurrentLooper();
    m_pcEventHandler = new(MEM_Voice) NCTtsStatusEventHandler(m_thread->looper());
    VoiceList<nutshell::NCString>::type filter;

    filter.push_back(evSysContext.buildEventFullName(EVIP_SVRMGR_SERVICE_STATUS_SPACE, TTS_SERVICE.c_str()));
    evSysContext.registerHandler(m_pcEventHandler, filter);
#endif

    queryTtsStatus();
}

void VP_NCEventReceiver::stopRecv()
{
#ifdef  HAVE_NUTSHELL_OS
    m_thread->stopThread();
#endif
}

void VP_NCEventReceiver::queryTtsStatus()
{
    TtsServiceStatus *msg = new(MEM_Voice)TtsServiceStatus();
    msg->set_sender(EV_MODULE_VOICEPLAY);
    EV_EventSender cSender;
    cSender.SendEvent(EV_EVENT_TTS_SERVICESTATUS, msg);
}

#ifdef  HAVE_NUTSHELL_OS
void NCTtsStatusEventHandler::handleEvent(const nutshell::NCEvent& event)
{
    int serviceStatus = event.arg1();

    VP_LOGD("STATUS OF TTS IS %d\n", serviceStatus);
    if (serviceStatus == EVIP_SERVICE_STATUS_UNKNOWN) {
        shared_ptr<VP_TtsServiceDown_Request> request = make_shared<VP_TtsServiceDown_Request>();
        request->commit();
    }
    else {
        return;
    }
}

#endif

/* EOF */
