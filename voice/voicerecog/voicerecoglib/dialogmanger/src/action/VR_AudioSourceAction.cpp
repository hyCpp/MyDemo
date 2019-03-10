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

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include <memory>
#include <sstream>
#include <boost/format.hpp>
#include "stdafx.h"
#include "EV_Define.h"
#include "VR_AudioSourceAction.h"
#include "VR_ActionEventSender.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "VR_AudioSourceListener.h"

using namespace navi::VoiceRecog;
using namespace nutshell::media;
using namespace navi::dataprovider;
std::string strSwitchSource = "<action agent=\"media\" op=\"changeSource\">"
                                 "<sourceid>%1%</sourceid>"
                              "</action>";

#ifndef VR_ACTION_CASE
#define VR_ACTION_CASE(MSG_ID, MSG_TYPE, P_MSG_BASE) \
    case (MSG_ID): \
    { \
        const MSG_TYPE* msg = static_cast<const MSG_TYPE *>(P_MSG_BASE); \
        this->RunAction(*msg); \
    } \
    break
#endif

VR_AudioSourceAction::VR_AudioSourceAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_AudioSourceAction::~VR_AudioSourceAction()
{
    VR_LOGD_FUNC();
}


template<typename MsgType>
void VR_AudioSourceAction::RunAction(const MsgType& msg)
{
    VR_LOGD_FUNC();
    MsgType vrGeneralAction;
    vrGeneralAction.CopyFrom(msg);
    m_sender->sendActionRequest(GetActionId(), m_ActionType, vrGeneralAction);
}

template<>
void VR_AudioSourceAction::RunAction(const VrSwitchSource& msg)
{
    // VR_LOGP("AudioSource: Recive Action From DE Case:212-4");
    const VR_INT32 INVALID_SOURCE = 0;
    const VR_INT32 nFirstAppSourceId = 26;
    std::string resultXml;
    std::string sourceName = msg.source_name();
    VR_LOGI("source name %s", sourceName.c_str());
    int sourceId = VR_AudioSourceListener::FindFcSourceId(sourceName.c_str());
    if (INVALID_SOURCE == sourceId) {
        struct MediaInfo_EntuneList entuneList;
        memset(&entuneList, 0, sizeof(entuneList));
        bool bFailed = NMMediaInfoHelper::load(MediaInfoKey_EntuneList, &entuneList, sizeof(entuneList));
        if (bFailed) {
            int count = sizeof(entuneList.appName) / sizeof(entuneList.appName[0]);
            for (int i = 0; i < count; ++i) {
                if (!sourceName.compare((const char*)entuneList.appName[i])) {
                    sourceId = i + nFirstAppSourceId;
                    break;
                }
            }
        }
    }
    VR_LOGI("source id %d", sourceId);
    //  source come from xusonglin
    std::stringstream stream;
    std::string strSourceId;
    stream.clear();
    stream.str("");
    stream << sourceId;
    stream >> strSourceId;
    resultXml = (boost::format(strSwitchSource) % strSourceId).str();
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(m_EngineReqId);
    notifyAction->set_actionid(m_ActionType);

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());
    // VR_LOGP("AudioSource: Send ActionReq TO FC Case:212-4");
}

void VR_AudioSourceAction::Run()
{
    VR_LOGI("action type=[%d]", m_ActionType);
    switch (m_ActionType) {
        VR_ACTION_CASE(AudioOFF,         VrAudioOFF,          (m_ReqMsg.get()));
        VR_ACTION_CASE(AudioON,          VrAudioON,           (m_ReqMsg.get()));
        VR_ACTION_CASE(SwitchSource,     VrSwitchSource,      (m_ReqMsg.get()));
    default :
        break;
    }
    this->SetActionComplete();
}

void VR_AudioSourceAction::OnReceive(const VR_ProtoMessageBase& proto)
{
    VR_LOGD_FUNC();
    VrRecogResultConfirm resultConfirm;
    resultConfirm.set_action_id(m_ActionType);
    resultConfirm.set_processing_result(true);
    m_sender->sendEngineReply(m_EngineReqId, resultConfirm);
}
/* EOF */

