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
 * @file VR_VoiceTagTTS.h
 * @brief Declaration file of class VR_VoiceTagTTS.
 *
 * This file includes the declaration of class VR_VoiceTagTTS.
 *
 * @attention used for C++ only.
 */

#ifndef VR_VOICETAGTTS_H
#define VR_VOICETAGTTS_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_SESSIONMANGER_H
#    include "VR_SessionManger.h"
#endif

#include "VR_DialogTaskBase.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SessionManger);

class EV_EventContext;

/**
 * @brief The VR_VoiceTagTTS class
 *
 * class declaration
 */
class VR_VoiceTagTTS : public VR_DialogTaskBase
{
public:
    VR_VoiceTagTTS(spVR_EventSenderIF pEventSender, spVR_DialogEngineIF pDialogSender, const spVR_SessionManger pVR_SessionManger);
    virtual ~VR_VoiceTagTTS();
    virtual void Start ();
    virtual void Cancel (const EV_EventContext &context);

    void Start(const EV_EventContext &context, const VR_ProtoMessageBase &msg);
    void OnMessage(const VR_ProtoMessageBase& msg);

private:
    spVR_DialogEngineIF         m_spEngineIF;
    spVR_EventSenderIF          m_spEventSenderIF;
    spVR_SessionManger         m_spVR_SessionManger;
    spEV_EventContext            m_spContext;
    spVR_ActionEventSender  m_sender;
};

#endif /* VR_VOICETAGTTS_H */
/* EOF */
