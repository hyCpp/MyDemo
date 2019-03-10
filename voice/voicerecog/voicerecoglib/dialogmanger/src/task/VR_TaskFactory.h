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
 * @file VR_TaskFactory.h
 * @brief Declaration file of class VR_TaskFactory.
 *
 * This file includes the declaration of class VR_TaskFactory.
 *
 * @attention used for C++ only.
 */

#ifndef VR_TASKFACTORY_H
#define VR_TASKFACTORY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_TaskFactoryIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_DialogTaskBase.h"
#include "VR_SessionManger.h"
#include "VR_EventSenderIF.h"
#include "VR_StartServiceBase.h"
#include "VR_VoiceRecogStatus.h"
#include "VR_GViewerProxyBase.h"
// #include "VR_USBMoniter.h"
#include "VR_DebugRecorderEventHandler.h"

class EV_EventContext;

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogTaskBase);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_TaskFactoryIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SessionManger);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_StartServiceBase);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_VoiceRecogStatus);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_GViewerProxyBase);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DebugRecorderEventHandler);
// VR_DECLARE_CLASS_WITH_SMART_PTR(VR_USBMoniter);

/**
 * @brief The VR_TaskFactoryIF class
 *
 * class declaration
 */
class VR_TaskFactory : public VR_TaskFactoryIF
{
public:
    VR_TaskFactory();
    virtual ~VR_TaskFactory();
    virtual spVR_GViewerProxyBase  CreateGViewProxy(spVR_DialogEngineIF spDialogEngine);
    virtual spVR_WeatherDataServiceBase  CreateWeatherDataService(spVR_DialogEngineIF spDialogEngine);
    virtual spVR_StartGBookBase    CreateStartGBook(spVR_RunableThread  spRunableThread, spVR_EventSenderIF  spEventSender,  spVR_DialogEngineIF spDialogEngine);
    virtual spVR_DialogTaskBase CreateVoiceTagTTS(spVR_EventSenderIF pEventSender, spVR_DialogEngineIF pDialogSender, const spVR_SessionManger pVR_SessionManger);
    virtual spVR_NaviStatusMoniter CreateNaviStatusMoniter(spVR_RunableThread spRunnableThread, spVR_DialogEngineIF spDialogEngine);
    virtual spVR_DebugRecorderEventHandler  CreateDebugEventHandler(spVR_RunableThread spRunableThread, spVR_DialogEngineIF spDialogEngine);
    virtual spVR_TtsStatusMoniter CreateTtsStatusMoniter(spVR_RunableThread spRunnableThread);
#if 0
    virtual spVR_DialogTaskBase CreateDictationTask(spVR_AvcLanSenderIF spAvcLan, spVR_EventSenderIF spSender);
    virtual spVR_DialogTaskBase CreateRecogTask(const int type, const VR_String& session, const EV_EventContext &context, spVR_DialogEngineIF engine);
    virtual spVR_DialogTaskBase CreateTslApp(spVR_AvcLanSenderIF spAvcLan, spVR_EventSenderIF pEventSender, spVR_DialogEngineIF pDialogSender, const spVR_SessionManger pVR_SessionManger);
    virtual spVR_StartServiceBase  CreateStartService(spVR_AvcLanSenderIF spAvcLan, spVR_EventSenderIF spEventSender, spVR_DialogEngineIF spDialogEngine);
    virtual spVR_VoiceRecogStatus  CreateVoiceStatus(spVR_AvcLanSenderIF spAvcLanSender, spVR_DialogEngineIF spDialogEngine);
    virtual spVR_USBMoniter    CreateUsbMoniter(spVR_EventSenderIF  spEventSender, spVR_DialogEngineIF spDialogEngine, spVR_DialogEngineListener spEngineListener,
                                                spVR_DebugRecorderEventHandler spDebugEventHandler, spVR_RunableThread spRunnableThread);
#endif
};

#endif /* VR_TASKFACTORYIF_H */
/* EOF */
