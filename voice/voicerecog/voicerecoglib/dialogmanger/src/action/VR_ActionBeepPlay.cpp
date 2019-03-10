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
 * @file VR_ActionBeepPlay.cpp
 * @brief Declaration file of class VR_ActionBeepPlay.
 *
 * This file includes the declaration of class VR_ActionBeepPlay.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"

#include <memory>
#include "VR_EventSenderIF.h"
#include "VR_ActionEventSender.h"

#include "VR_ActionBeepPlay.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"

#include "navi/Voice/TTS/TtsPlayFile.pb.h"

using namespace navi::VoiceRecog;
using namespace navi::TTS;
int VR_ActionBeepPlay::m_lastActionId = 0;
int VR_ActionBeepPlay::m_EngineBeepPlayId = 0;

VR_ActionBeepPlay::VR_ActionBeepPlay(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
    , m_FilePath(((VrBeepCommandReq&)proto).beep_file_path())
    , m_isBargein(((VrBeepCommandReq&)proto).isbargein())
{
    m_EngineBeepPlayId = m_EngineReqId;
    VR_LOGI("EngineReqId=[%d],action type=[%d],beep_file_path=[%s]", m_EngineReqId, m_ActionType, m_FilePath.c_str());
}

VR_ActionBeepPlay::~VR_ActionBeepPlay()
{
    VR_LOGD_FUNC();
}

int VR_ActionBeepPlay::getLastActionId()
{
    VR_LOGI("m_lastActionId=[%d]", m_lastActionId);
    return m_lastActionId;
}

int VR_ActionBeepPlay::getLastEngineId()
{
    VR_LOGI("m_EngineBeepPlayId=[%d]", m_EngineBeepPlayId);
    return m_EngineBeepPlayId;
}

void VR_ActionBeepPlay::Run()
{
    VR_LOGD_FUNC();
    std::unique_ptr<TtsPlayFile> msg(TtsPlayFile::default_instance().New());
    msg->mutable_play();
    msg->mutable_play()->mutable_base()->set_sender(EV_MODULE_VOICERECOG);
    msg->mutable_play()->mutable_base()->set_onresponse(true);
    msg->mutable_play()->set_reqid(this->GetActionId());
    VR_LOGI("GetActionId()=[%d]", GetActionId());
    msg->mutable_play()->set_priority(TtsBasePlay_OP_PRIORITY_OP_PRI_VR);
    msg->mutable_play()->set_tokenid(27);
    msg->mutable_play()->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);
    msg->set_filepath(m_FilePath);

    VR_EventSenderIF::GetSender()->SendEvent(EV_EVENT_TTS_PLAYFILE, msg.release());
    VR_LOGP("PTT:pressDown Send BeepActionReq to TTS");
    m_lastActionId = this->GetActionId();
}

void VR_ActionBeepPlay::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrActionDM2DE> replyMsg(VrActionDM2DE::default_instance().New());
    replyMsg->set_id(this->GetActionId());
    replyMsg->set_vr_dm_action_type(ActionType_BEEP_COMMAND_RESP);

    VrTTSCommandResult pOldMsg = (VrTTSCommandResult&)proto;
    VrTTSProcessType ttsState = pOldMsg.func_code();
    switch (ttsState) {
    case TTSProcessType_Stop:
        replyMsg->mutable_tts_result()->set_func_code(TTSProcessType_Stop);
        break;
    case TTSProcessType_Pause:
        replyMsg->mutable_tts_result()->set_func_code(TTSProcessType_Pause);
        break;
    default:
        return;
    }

    replyMsg->mutable_tts_result()->set_result(true);
    m_sender->sendEngineReply(m_EngineReqId, *replyMsg);
    this->SetActionComplete();
}

/* EOF */
