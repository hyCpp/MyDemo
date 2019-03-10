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
 * @file VR_USBMoniter.h
 * @brief Declaration file of class VR_USBMoniter.
 *
 * This file includes the declaration of class VR_USBMoniter.
 *
 * @attention used for C++ only.
 */
#ifndef VR_USBMONITER_H
#define VR_USBMONITER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_EventSenderIF.h"
#include "VR_NCMonitor.h"
#include "VR_ConfigureIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_DialogEngineListener.h"
#include "VR_DebugRecorderEventHandler.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineListener);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DebugRecorderEventHandler);

/**
 * @brief The VR_USBMoniter class
 *
 * class declaration
 */
class VR_USBMoniter
{
public:
    VR_USBMoniter(spVR_EventSenderIF   spEventSender,  spVR_DialogEngineIF spDialogEngine, spVR_DialogEngineListener spEngineListener,
                  spVR_DebugRecorderEventHandler spDebugEventHandler);
    virtual  ~VR_USBMoniter();

    virtual  void  OnReqestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    virtual  void  OnMessage(const navi::VoiceRecog::VrVBTSwitchTypeResult& msg);
    virtual  void  OnMessage(const navi::VoiceRecog::VrEngineControlMessage_VrCancelEnd& msg);
    virtual  void  OnMessage(const navi::VoiceRecog::VrDialogEventResult&  msg);
    virtual  void  OnStart();
    virtual  void  OnStop();
    virtual  void  OnCallBack(VR_VBTLogSwithType emFlag);

protected:
    bool           DiagCopyStart();
    bool           DiagDeleteStart();
    bool           DiagDelete();
    void           DiagCopyFinish();
    void           DiagDeleteFinsh();

protected:
    spVR_EventSenderIF                                    m_spEventSender;
    spNCUsbManager                                        m_usbManager;    // usb manager
    spVRUsbMoniter                                        m_usbMonitor;    // usb monitor
    VR_VBTLogSwithType                                    m_emVBTLogSwitch;    // vbt log switch flag
    VR_ConfigureIF::VR_EngineType                         m_eEngType;      // eng type
    spVR_DialogEngineIF                                   m_spDialogEngine;
    spVR_DialogEngineListener                             m_spDialogEngineListener;
    spVR_DebugRecorderEventHandler                        m_spDebugEventHandler;
    bool                                                  m_bStopDialog;
};

#endif // VR_USBMONITER_H
/* EOF */
