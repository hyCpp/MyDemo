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
* @file  VR_StartServiceHandler.cpp
* @brief Declaration file of class VR_StartServiceHandler.
*
* This file includes the declaration of class VR_StartServiceHandler.
*
* @attention used for C++ only.
*/

#ifndef VR_STARTSERVICEHANDLER_H
#define VR_STARTSERVICEHANDLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif
#include "VR_StartServiceBase.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);


/**
 * @brief The VR_StartServiceHandler class
 *
 * class declaration
 */
class VR_StartServiceHandler : public VR_StartServiceBase
{
public:
    VR_StartServiceHandler(spVR_DialogEngineIF spDialogEngine, spVR_EventSenderIF spEventSender);
    virtual ~VR_StartServiceHandler();

    void Initialize();
    void NotifyTslState();
    void OnReplyCommonProperty();
    bool OnVoiceSettingAvailable();
    void OnNotifyVoiceSettingAvailable();
    void OnNotifyTslServiceAvailable();
    void OnNotifyVRServiceIdleStatus();
    void OnNotifyVoiceTagAvailable();
    void OnNotifyMMVRServiceAvailable();
    void OnNotifySDLVRServiceAvailable();
    void OnReplyCommonPropertyWithHints(const VR_ProtoMessageBase& msg);

    void OnMessage(const VR_ProtoMessageBase &msg);
    void NotifyUICrash(const VR_ProtoMessageBase& msg);
    void OnMessage(const navi::VoiceRecog::VrUnitStatusResult& statusResult);
    void OnMessage(const navi::VoiceRecog::VrStartStatusResult& statusResult);

    void RequestVrTslState(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnReqestVRStatusWithHints(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnReqestVRServiceIdleStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnReqestVoiceTagStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg);

/*********** Manual VR *************/
    void OnMMVRRequestVRServiceStatus(const EV_EventContext& context, const VR_ProtoMessageBase& msg);
    void OnReplyMMVRServiceStatus();

/*********** SDL VR ****************/
    void OnSDLVRRequestVRServiceStatus(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnReplySDLVRServiceStatus();

    // amazon
    bool CheckAwakenStatus();

protected:
    bool CheckVoiceTagStatus();
    void CheckLangDicData();
    void CheckStartStatus();
    bool CheckHigerInterrupt();
    virtual Start_Condition_Error CheckStartData();

private:
    spVR_DialogEngineIF      m_spDialogEngine;
    spVR_EventSenderIF       m_spEventSender;
    spEV_EventContext        m_spEventContext;
    bool                     m_bVoiceNotUsing;
};

#endif // VR_STARTSERVICEHANDLER_H
/* EOF */
