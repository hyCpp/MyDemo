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
 * @file VR_ActionTtsListStop.cpp
 * @brief Declaration file of class VR_ActionTtsListStop.
 *
 * This file includes the declaration of class VR_ActionTtsListStop.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"
#include <memory>
#include "VR_EventSenderIF.h"
#include "VR_ActionEventSender.h"

#include "VR_ActionTtsListStop.h"
#include "VR_ActionTtsListPlay.h"
#include "VR_DailogEngineMessageBuilder.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"

#include "navi/Voice/TTS/TtsStop.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::TTS;


VR_ActionTtsListStop::VR_ActionTtsListStop(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_Id.SetID(VR_ActionTtsListPlay::getLastActionId());
    m_EngineTtsPlayReqId = VR_ActionTtsListPlay::getLastEngineId();
}

VR_ActionTtsListStop::~VR_ActionTtsListStop()
{
    VR_LOGD_FUNC();
}

void VR_ActionTtsListStop::Run()
{
    VR_LOGD_FUNC();
    std::unique_ptr<TtsStop> msg(TtsStop::default_instance().New());
    msg->set_reqid(this->GetActionId());
    msg->mutable_base()->set_sender(EV_MODULE_VOICERECOG);
    msg->mutable_base()->set_onresponse(true);
    VR_EventSenderIF::GetSender()->SendEvent(EV_EVENT_TTS_STOP, msg.release());
}

void VR_ActionTtsListStop::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    int nEngineReqId = 0;
    VR_String strXml = "";

    VrTTSCommandResult pOldMsg = (VrTTSCommandResult&)proto;
    VrTTSProcessType ttsState = pOldMsg.func_code();
    switch (ttsState) {
    case TTSProcessType_Stop:
        {
            strXml = "<action-result agent=\"prompt\"  op=\"playTtsList\" errcode=\"0\">"
                     "</action-result>";
            nEngineReqId = m_EngineTtsPlayReqId;
        }
        break;
    case TTSProcessType_Pause:
        {
            strXml = "<action-result agent=\"prompt\"  op=\"stopTtsList\" errcode=\"0\">"
                     "</action-result>";
            nEngineReqId = m_EngineReqId;
        }
        break;
    default: 
        return;
    }

    m_sender->sendEngineReply(nEngineReqId, strXml);
    this->SetActionComplete();
}
/* EOF */
