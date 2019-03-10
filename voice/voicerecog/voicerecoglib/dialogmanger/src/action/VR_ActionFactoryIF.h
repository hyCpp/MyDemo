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
 * @file VR_ActionFactoryIF.h
 * @brief Declaration file of class VR_ActionFactoryIF.
 *
 * This file includes the declaration of class VR_ActionFactoryIF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONFACTORYIF_H
#define VR_ACTIONFACTORYIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

class EV_EventContext;

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionBase);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionFactoryIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionEventSender);
// VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AvcLanSenderIF);

namespace navi
{
namespace VoiceRecog
{
    class VrRecogResult;
    class VrAdditionalInfoReq;
    class VrActiveInfoReq;
    class VrTTSCommandReq;
    class VrBeepCommandReq;
    class RequestVrVoiceTag;
    class VrMessageAcquisitionReq;
    class VrActionNavi;
    class VrTtsListCommandReq;
}
}

/**
 * @brief The VR_ActionFactoryIF class
 *
 * class declaration
 */
class VR_ActionFactoryIF
{
public:
    virtual ~VR_ActionFactoryIF() {}
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrRecogResult& proto) = 0;
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrAdditionalInfoReq& proto) = 0;
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrTTSCommandReq& proto) = 0;
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrBeepCommandReq& proto) = 0;
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrActiveInfoReq& proto) = 0;
    virtual spVR_ActionBase CreateAction(const EV_EventContext &context, const navi::VoiceRecog::RequestVrVoiceTag& proto) = 0;
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrMessageAcquisitionReq& proto) = 0;
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrActionNavi& proto) = 0;
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrTtsListCommandReq& proto) = 0;

    virtual   void  UpdateNaviPathPointList(const  VR_ProtoMessageBase& msg) {}

public:
    static  spVR_ActionFactoryIF getActionFactory(spVR_ActionEventSender sender);
};

#endif /* VR_ACTIONFACTORYIF_H */
/* EOF */
