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

#include "EV_Define.h"
#include <boost/format.hpp>
#include "VR_ActionEventSender.h"
#include "VR_HFDGetMsgBodyAction.h"
#include "VR_ActionInnerParameterDef.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrMessageTrans.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"

using namespace navi::VoiceRecog;

VR_HFDGetMsgBodyAction::VR_HFDGetMsgBodyAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto)
    : VR_ActionBase(sender)
    , m_EngineReqId(id)
    , m_ActionType(type)
    , m_ReqMsg(proto.New())
{
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_HFDGetMsgBodyAction::~VR_HFDGetMsgBodyAction()
{
}

void VR_HFDGetMsgBodyAction::OnReceive(const VR_ProtoMessageBase& proto)
{
    VR_LOGD_FUNC();

    VR_String strActionXml = "<action-result agent=\"phone\" op=\"getMessageDetail\" errcode=\"0\">"\
                                 "<messagebody>%1%</messagebody>"\
                             "</action-result>";
    VR_String resultXml;
    VrMessageAcquisitionResp resultResponse = (VrMessageAcquisitionResp&)proto;

    if (proto.GetTypeName() == resultResponse.GetTypeName()) {
        VR_String msgBody = resultResponse.body();
        resultXml = (boost::format(strActionXml) % msgBody).str();
        m_sender->sendEngineReply(m_EngineReqId, resultXml);
    }
    this->SetActionComplete();
}

template<>
void VR_HFDGetMsgBodyAction::RunAction(const VrMessageAcquisitionReq& msg)
{
    VR_LOGD_FUNC();
    VR_String strActionXml = "<action agent=\"phone\" op=\"getMessageDetail\">"\
                                 "<instanceId>%1%</instanceId>"\
                                 "<messageId>%2%</messageId>"\
                             "</action>";

    VR_String resultXml;
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(m_EngineReqId);
    notifyAction->set_actionid(m_ActionType);

    resultXml = (boost::format(strActionXml) % msg.service_id() % msg.message_id()).str();

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());
}

void VR_HFDGetMsgBodyAction::Run()
{
    VR_LOGD_FUNC();

    this->RunAction(*((const VrMessageAcquisitionReq *)(m_ReqMsg.get())));
}
/* EOF */
