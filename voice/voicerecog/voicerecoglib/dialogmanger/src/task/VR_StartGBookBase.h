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
 * @file VR_StartGBookBase.h
 * @brief Declaration file of class VR_StartGBookBase.
 *
 * This file includes the declaration of class VR_StartGBookBase.
 *
 * @attention used for C++ only.
 */
#ifndef VR_STARTGBOOKBASE_H
#define VR_STARTGBOOKBASE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef  VR_DEF_H
#    include  "VR_Def.h"
#endif

#ifndef  VR_MACRO_H
#    include  "VR_Macro.h"
#endif

#include  "VR_DialogEngineIF.h"

class  EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

/**
 *   @brief   The class of VR_StartGBookBase
 *
 *   class Declaration
 */
class VR_StartGBookBase
{
public:
    VR_StartGBookBase(spVR_EventSenderIF   spEventSender, spVR_DialogEngineIF spDialogEngine);
    virtual  ~VR_StartGBookBase();

    virtual  void  OnReqestCommonProperty(const EV_EventContext& context, const VR_ProtoMessageBase& msg) = 0;
    virtual  void  OnMessage(const VR_ProtoMessageBase& msg) = 0;
    virtual  void  OnStart() = 0;
    virtual  void  OnStop() = 0;
    virtual std::string getServerVRUrl() = 0;

protected:
    spVR_EventSenderIF        m_spEventSender;
    spVR_DialogEngineIF       m_spDialogEngine;
};

#endif // VR_STARTGBOOKBASE_H
/* EOF */
