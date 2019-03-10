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
 * @file VR_ActionBeepStop.cpp
 * @brief Declaration file of class VR_ActionBeepStop.
 *
 * This file includes the declaration of class VR_ActionBeepStop.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"
#include <memory>
#include "VR_EventSenderIF.h"
#include "VR_ActionEventSender.h"

#include "VR_ActionBeepStop.h"
#include "VR_ActionBeepPlay.h"
#include "VR_DailogEngineMessageBuilder.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"

#include "navi/Voice/TTS/TtsStop.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::TTS;


VR_ActionBeepStop::VR_ActionBeepStop(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_Id.SetID(VR_ActionBeepPlay::getLastActionId());
    m_EngineBeepPlayReqId = VR_ActionBeepPlay::getLastEngineId();
    VR_LOGI("VR_ActionBeepPlay::getLastActionId()=[%d]", VR_ActionBeepPlay::getLastActionId());
}

VR_ActionBeepStop::~VR_ActionBeepStop()
{
    VR_LOGD_FUNC();
}

void VR_ActionBeepStop::Run()
{
    VR_LOGD_FUNC();
    std::unique_ptr<TtsStop> msg(TtsStop::default_instance().New());
    msg->set_reqid(this->GetActionId());
    // msg->set_stopall(true); // stop all current
    msg->mutable_base()->set_sender(EV_MODULE_VOICERECOG);
    msg->mutable_base()->set_onresponse(true);
    VR_EventSenderIF::GetSender()->SendEvent(EV_EVENT_TTS_STOP, msg.release());
}

void VR_ActionBeepStop::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrActionDM2DE> replyMsg(VrActionDM2DE::default_instance().New());
    replyMsg->set_id(this->GetActionId());
    replyMsg->set_vr_dm_action_type(ActionType_BEEP_COMMAND_RESP);
    int nEngineReqId = 0;

    VrTTSCommandResult pOldMsg = (VrTTSCommandResult&)proto;
    VrTTSProcessType ttsState = pOldMsg.func_code();
    switch (ttsState) {
    case TTSProcessType_Stop:
        {
            replyMsg->mutable_tts_result()->set_func_code(TTSProcessType_Stop);
            nEngineReqId = m_EngineBeepPlayReqId;
        }
        break;
    case TTSProcessType_Pause:
        {
            replyMsg->mutable_tts_result()->set_func_code(TTSProcessType_Pause);
            nEngineReqId = m_EngineReqId;
        }
        break;
    default:
        return;
    }

    replyMsg->mutable_tts_result()->set_result(true);
    m_sender->sendEngineReply(nEngineReqId, *replyMsg);
    this->SetActionComplete();
}
/* EOF */
