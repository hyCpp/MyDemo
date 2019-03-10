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
#include "VR_AppsStartAction.h"
#include "VR_TransformProtoToXml.h"
#include "VR_ActionEventSender.h"
#include "VR_ActionInnerParameterDef.h"
#include "EV_Define.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"

#include <boost/format.hpp>
#include <sstream>

using namespace navi::VoiceRecog;

#ifndef VR_ACTION_CASE
#define VR_ACTION_CASE(MSG_ID, MSG_TYPE, P_MSG_BASE) \
    case (MSG_ID): \
    { \
        const MSG_TYPE* msg = static_cast<const MSG_TYPE *>(P_MSG_BASE); \
        this->RunAction(*msg); \
    } \
    break
#endif

VR_AppsStartAction::VR_AppsStartAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_EngineReqId(id)
    , m_ActionType(type)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_AppsStartAction::~VR_AppsStartAction()
{
    VR_LOGD_FUNC();
}

VR_String VR_AppsStartAction::GetStartAppsXml()
{
    VR_String resultXml = "";
    VR_String dataContent = "";

    VR_String strFormat = "<action agent=\"apps\" op=\"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                              "<%6%>%7%</%6%>"
                          "</action>";

    const VrStartApps* startApps = static_cast<const VrStartApps*>(m_ReqMsg.get());
    VR_INT32 applicationId = startApps->application_id();
    VR_INT32 dataSize = startApps->data_size();
    VR_String sDataSize = std::to_string(dataSize);
    if ((dataSize != 0) && startApps->has_data_content()) {
        dataContent = startApps->data_content();
    }

    resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_APPS_LAUNCH_APP
                 % VR_VOICE_ACTION_TAG_KEY_APP_ID % applicationId
                 % VR_VOICE_ACTION_TAG_KEY_DATA_SIZE % sDataSize
                 % VR_VOICE_ACTION_TAG_KEY_DATA % dataContent).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

void VR_AppsStartAction::RunAction()
{
    VR_String resultXml = "";
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(m_EngineReqId);
    notifyAction->set_actionid(m_ActionType);

    VR_LOGI("action type=[%d]", m_ActionType);
    switch (m_ActionType) {
    case StartApps :
        {
            VR_LOGI("case StartApps");
            resultXml = GetStartAppsXml();
        }
        break;
    default:
        {
            VR_LOGI("default");
        }
        break;

    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());
}

void VR_AppsStartAction::Run()
{
    VR_LOGD_FUNC();
    RunAction();
    this->SetActionComplete();
}

void VR_AppsStartAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}
/* EOF */
