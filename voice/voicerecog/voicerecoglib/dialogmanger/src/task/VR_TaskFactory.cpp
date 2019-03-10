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
#include "VR_Log.h"
#include "VR_TaskFactory.h"
#include "VR_RecogTask.h"
#include "VR_VoiceTslApp.h"
#include "VR_VoiceTagTTS.h"
#include "VR_StartServiceHandler.h"
#include "VR_StartGBookHandler.h"

VR_TaskFactory::VR_TaskFactory()
{
    VR_LOGD("VR_TaskFactory");
}

VR_TaskFactory::~VR_TaskFactory()
{

}


spVR_GViewerProxyBase  VR_TaskFactory::CreateGViewProxy(spVR_DialogEngineIF spDialogEngine)
{
    spVR_GViewerProxyBase  spGviewerProxy(VR_new  VR_GViewerProxyBase());
    return  spGviewerProxy;
}

spVR_WeatherDataServiceBase VR_TaskFactory::CreateWeatherDataService(spVR_DialogEngineIF spDialogEngine)
{
    spVR_WeatherDataServiceBase  spWeatherDataService(VR_new  VR_WeatherDataServiceBase());
    return  spWeatherDataService;
}

spVR_StartGBookBase   VR_TaskFactory::CreateStartGBook(spVR_RunableThread  spRunableThread, spVR_EventSenderIF  spEventSender, spVR_DialogEngineIF spDialogEngine)
{
    // spVR_StartGBookBase   spStartGBookHandler(VR_new  VR_StartGBookHandler(spEventSender, spDialogEngine));
    // return   spStartGBookHandler;
    return NULL;
}

spVR_DialogTaskBase VR_TaskFactory::CreateVoiceTagTTS(spVR_EventSenderIF pEventSender, spVR_DialogEngineIF pDialogSender, const spVR_SessionManger pVR_SessionManger)
{
#ifdef HAVE_NUTSHELL_OS
    spVR_DialogTaskBase spDialogTask(VR_new VR_VoiceTagTTS(pEventSender, pDialogSender, pVR_SessionManger));
    return spDialogTask;
#else
    return NULL;
#endif
}

spVR_NaviStatusMoniter VR_TaskFactory::CreateNaviStatusMoniter(spVR_RunableThread spRunnableThread, spVR_DialogEngineIF spDialogEngine)
{
    // spVR_NaviStatusMoniter spNaviStatusMoniter(VR_new VR_NaviStatusMoniter(spDialogEngine));
    // return spNaviStatusMoniter;
    return NULL;
}

spVR_DebugRecorderEventHandler  VR_TaskFactory::CreateDebugEventHandler(spVR_RunableThread spRunableThread, spVR_DialogEngineIF spDialogEngine)
{
    // spVR_DebugRecorderEventHandler  spDebugEventHandler(VR_new VR_DebugRecorderEventHandler(spRunableThread, spDialogEngine));
    // return  spDebugEventHandler;
    return NULL;
}

spVR_TtsStatusMoniter VR_TaskFactory::CreateTtsStatusMoniter(spVR_RunableThread spRunnableThread)
{
    // spVR_TtsStatusMoniter spTtsStatusMoniter(VR_new VR_TtsStatusMoniter(spRunnableThread));
    // return spTtsStatusMoniter;
    return NULL;
}

#if 0
spVR_DialogTaskBase
VR_TaskFactory::CreateDictationTask(spVR_AvcLanSenderIF spAvcLan, spVR_EventSenderIF spSender)
{
    spVR_DialogTaskBase spDialogTask(VR_new VR_DialogTaskNull());
    return spDialogTask;
}

spVR_DialogTaskBase
VR_TaskFactory::CreateRecogTask(const int type, const VR_String &session, const EV_EventContext &context, spVR_DialogEngineIF engine)
{
    spVR_DialogTaskBase spDialogTask(VR_new VR_RecogTask(type, session, context, engine));
    return spDialogTask;
}

spVR_DialogTaskBase VR_TaskFactory::CreateTslApp(spVR_AvcLanSenderIF spAvcLan, spVR_EventSenderIF pEventSender, spVR_DialogEngineIF pDialogSender, const spVR_SessionManger pVR_SessionManger)
{
    spVR_DialogTaskBase spDialogTask(VR_new VR_VoiceTslApp(pEventSender, pDialogSender, pVR_SessionManger));
    return spDialogTask;
}

spVR_StartServiceBase  VR_TaskFactory::CreateStartService(spVR_AvcLanSenderIF spAvcLan, spVR_EventSenderIF spEventSender, spVR_DialogEngineIF spDialogEngine)
{
    spVR_StartServiceBase   spStartServiceHandler(VR_new  VR_StartServiceHandler(spDialogEngine, spEventSender));
    return spStartServiceHandler;
}

spVR_VoiceRecogStatus  VR_TaskFactory::CreateVoiceStatus(spVR_AvcLanSenderIF spAvcLanSender, spVR_DialogEngineIF spDialogEngine)
{
    spVR_VoiceRecogStatus   spVoiceRecogStatus(VR_new  VR_VoiceRecogStatus(spDialogEngine));
    return   spVoiceRecogStatus;
}

spVR_USBMoniter VR_TaskFactory::CreateUsbMoniter(spVR_EventSenderIF spEventSender, spVR_DialogEngineIF spDialogEngine, spVR_DialogEngineListener spEngineListener,
                                                 spVR_DebugRecorderEventHandler spDebugEventHandler, spVR_RunableThread  spRunnableThread)
{
    spVR_USBMoniter   spUsbMoniter(VR_new  VR_USBMoniter(spEventSender, spDialogEngine, spEngineListener, spDebugEventHandler));
    return   spUsbMoniter;
}

#endif
/* EOF */
