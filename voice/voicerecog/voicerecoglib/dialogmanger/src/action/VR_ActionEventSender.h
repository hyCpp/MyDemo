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
 * @file VR_ActionEventSender.h
 * @brief Declaration file of class VR_ActionEventSender.
 *
 * This file includes the declaration of class VR_ActionEventSender.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONEVENTSENDER_H
#define VR_ACTIONEVENTSENDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

namespace navi
{
namespace VoiceRecog
{
    class VrRecogResult;
    class VrAdditionalInfoReq;
}
}

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionEventSender);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);


/**
 * @brief The VR_ActionEventSender class
 *
 * class declaration
 */
class VR_ActionEventSender
{
public:
    VR_ActionEventSender(spVR_DialogEngineIF spEngine);
    ~VR_ActionEventSender();
    void sendActionRequest(int id, int actionType,  const VR_ProtoMessageBase &msg);
    void sendInfoRequest(int id, const VR_ProtoMessageBase &msg);
    void sendSpecialInfoRequest(int id, const VR_ProtoMessageBase &msg);
    void sendMessageToFC(const char* protoTag, VR_ProtoMessageBase *msg);

    void sendEngineReply(int id,  const VR_ProtoMessageBase &msg);
    void sendActionReply(int id,  const VR_ProtoMessageBase& msg);
    void sendEngineReply(int id,  const VR_String &msg);

    spVR_DialogEngineIF getDialogEngine();

protected:
    spVR_DialogEngineIF m_spEngineIF;
    spVR_EventSenderIF m_spEventSenderIF;

};

#endif /* VR_ACTIONEVENTSENDER_H */
/* EOF */
