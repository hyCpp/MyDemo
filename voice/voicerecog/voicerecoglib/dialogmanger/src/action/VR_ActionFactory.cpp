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
#include "VR_ActionFactory.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActionNavi.pb.h"
#include "VR_ActionBase.h"

using namespace navi::VoiceRecog;


VR_ActionFactory::VR_ActionFactory(spVR_ActionEventSender sender)
    : m_sender(sender)
{
    VR_LOGD_FUNC();
}

VR_ActionFactory::~VR_ActionFactory()
{
    VR_LOGD_FUNC();
}

spVR_ActionBase VR_ActionFactory::CreateAction(int actionId, const VrMessageAcquisitionReq &proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_ActionFactory::CreateAction(int actionId, const VrRecogResult& proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_ActionFactory::CreateAction(int actionId, const VrAdditionalInfoReq& proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_ActionFactory::CreateAction(int actionId, const VrTTSCommandReq& proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_ActionFactory::CreateAction(int actionId, const VrBeepCommandReq& proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_ActionFactory::CreateAction(const EV_EventContext &context, const RequestVrVoiceTag& proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_ActionFactory::CreateAction(int actionId, const VrActiveInfoReq &proto)
{
    VR_LOGI("actionId=[%d],action type=[%d]", actionId, proto.request_id());
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_ActionFactory::CreateAction(int actionId, const VrActionNavi &proto)
{
    VR_LOGI("actionId=[%d],action type=[%d]", actionId, proto.actiontype());
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

spVR_ActionBase VR_ActionFactory::CreateAction(int actionId, const VrTtsListCommandReq &proto)
{
    VR_LOGD_FUNC();
    spVR_ActionBase spAction(VR_new VR_ActionBase(m_sender));
    return spAction;
}

/* EOF */
