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
 * @file VR_VoiceTagHandler.h
 * @brief Declaration file of class VR_VoiceTagHandler.
 *
 * This file includes the declaration of class VR_VoiceTagHandler.
 *
 * @attention used for C++ only.
 */

#ifndef VR_VOICETAGHANDLER_H
#define VR_VOICETAGHANDLER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "nceventsys/NCEventHandler.h"
#include "nceventsys/NCRunnableLooper.h"
#include "nceventsys/NCRunnableThread.h"
#include "nceventsys/NCEvSysContext.h"
#include "VR_DialogEngineIF.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RunableThread);

class VR_VoiceTagHandler;

/**
 * @brief The VR_VoiceTagEventHandler class
 *
 * class declaration
 */
class VR_VoiceTagEventHandler : public nutshell::NCEventHandler
{
public:
    VR_VoiceTagEventHandler(VR_VoiceTagHandler* pVoiceTagHandle,  const nutshell::NCRunnableLooper& looper);
    virtual ~VR_VoiceTagEventHandler();
    void  handleEvent(const nutshell::NCEvent &event);

private:
    VR_VoiceTagHandler*     m_pVoiceTag;

private:
    VR_VoiceTagEventHandler(const VR_VoiceTagEventHandler&);
    VR_VoiceTagEventHandler& operator=(const VR_VoiceTagEventHandler&);
};

/**
 * @brief The VR_VoiceTagHandler class
 *
 * class declaration
 */
class VR_VoiceTagHandler
{
public:
    VR_VoiceTagHandler(spVR_RunableThread spRunnableThread, spVR_DialogEngineIF dialogEngine);
    virtual ~VR_VoiceTagHandler();

public:
    void OnMessage(const VR_ProtoMessageBase& msg);
    spVR_DialogEngineIF GetDialogEngine();
    void Start();
    void Stop();

protected:
    void SendVoiceTagInfo();
    VR_INT32 GetDeviceId();
    void SendMsg2DE(VR_String& strXml);
    VR_INT32 TransformPhoneType(VR_INT32 nPhoneType, VR_INT32 nPhoneIndex);

private:
    spVR_RunableThread                                   m_spRunnableThread;
    spVR_DialogEngineIF                                  m_spDialogEngine;
    nutshell::ncsp<VR_VoiceTagEventHandler>::sp          m_pcEventHandler;
    nutshell::NCEvSysContext                             m_context;
    VoiceList<nutshell::NCString>::type                  m_filter;
};

#endif // VR_VOICETAGHANDLER_H
/* EOF */
