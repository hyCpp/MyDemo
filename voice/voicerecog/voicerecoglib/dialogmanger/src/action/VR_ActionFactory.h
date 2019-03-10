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
 * @file VR_ActionFactory.h
 * @brief Declaration file of class VR_ActionFactory.
 *
 * This file includes the declaration of class VR_ActionFactory.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONFACTORY_H
#define VR_ACTIONFACTORY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_ActionFactoryIF.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionBase);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_ActionEventSender);
// VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AvcLanSenderIF);

/**
 * @brief The VR_ActionFactory class
 *
 * class declaration
 */
class VR_ActionFactory : public VR_ActionFactoryIF
{
public:
    VR_ActionFactory(spVR_ActionEventSender sender);
    virtual ~VR_ActionFactory();
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrRecogResult& proto);
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrAdditionalInfoReq& proto);
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrTTSCommandReq& proto);
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrBeepCommandReq& proto);
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrActiveInfoReq& proto);
    virtual spVR_ActionBase CreateAction(const EV_EventContext &context, const navi::VoiceRecog::RequestVrVoiceTag& proto);
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrMessageAcquisitionReq& proto);
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrActionNavi& proto);
    virtual spVR_ActionBase CreateAction(int actionId, const navi::VoiceRecog::VrTtsListCommandReq& proto);

protected:
    spVR_ActionEventSender    m_sender;
//    spVR_AvcLanSenderIF       m_spAvcLanSender;
};

#endif /* VR_ACTIONFACTORY_H */
/* EOF */
