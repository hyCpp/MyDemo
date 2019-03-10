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
 * @file VR_ActionTtsPlay.cpp
 * @brief Declaration file of class VR_ActionTtsPlay.
 *
 * This file includes the declaration of class VR_ActionTtsPlay.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"
#include <memory>
#include "VR_EventSenderIF.h"
#include "VR_ActionEventSender.h"

#include "VR_ActionTtsPlay.h"
#include "VR_DailogEngineMessageBuilder.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include <boost/assign/list_of.hpp>
#include "navi/Voice/TTS/TtsPlayList.pb.h"
#include "navi/Voice/TTS/TtsPlayText.pb.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "VR_Configure.h"

using namespace navi::VoiceRecog;
using namespace navi::TTS;
int VR_ActionTtsPlay::m_lastActionId = 0;
int VR_ActionTtsPlay::m_EngineTtsPlayId = 0;

VR_ActionTtsPlay::VR_ActionTtsPlay(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
    , m_Language(((VrTTSCommandReq&)proto).tts_language())
    , m_isBargein(((VrTTSCommandReq&)proto).isbargein())
    , m_ReqMsg(proto.New())
{
    m_EngineTtsPlayId = m_EngineReqId;
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
}

VR_ActionTtsPlay::~VR_ActionTtsPlay()
{
    VR_LOGD_FUNC();
}

int VR_ActionTtsPlay::getLastActionId()
{
    VR_LOGI("m_lastActionId=[%d]", m_lastActionId);
    return m_lastActionId;
}

int VR_ActionTtsPlay::getLastEngineId()
{
    VR_LOGI("m_EngineTtsPlayId=[%d]", m_EngineTtsPlayId);
    return m_EngineTtsPlayId;
}

void VR_ActionTtsPlay::Run()
{
    VR_LOGD_FUNC();
    const VrTTSCommandReq* ttsCommandReq = static_cast<const VrTTSCommandReq*>(m_ReqMsg.get());
    std::unique_ptr<TtsPlayList> msg(TtsPlayList::default_instance().New());

    VR_INT32 ttsCount = ttsCommandReq->tts_texts_size();
    VR_LOGI("ttsCount=[%d]", ttsCount);

    for (VR_INT32 i = 0; i < ttsCount; i++) {
        TtsPlayList_Phrase* phrase = msg->add_phrases();
        phrase->set_type(TtsPlayList_Phrase_PHRASE_TYPE_PHRASE_TEXT);
        phrase->set_text(ttsCommandReq->tts_texts(i));
        VR_LOGI("tts_texts[%d]=[%s]", i, ttsCommandReq->tts_texts(i).c_str());
    }
    msg->mutable_base();
    msg->mutable_base()->mutable_base()->set_sender(EV_MODULE_VOICERECOG);
    msg->mutable_base()->mutable_base()->set_onresponse(true);
    msg->mutable_base()->set_reqid(this->GetActionId());
    msg->mutable_base()->set_lang(TtsBasePlay_TtsLanguage_TTS_LANG_UK_ENGLISH);
    msg->mutable_base()->set_priority(TtsBasePlay_OP_PRIORITY_OP_PRI_VR);
    msg->mutable_base()->set_tokenid(27);
    msg->mutable_base()->set_streamtype(TtsBasePlay_TTS_STREAM_TYPE_TTS_STREAM_NORMAL);

    VR_String cTTSLang;
    VR_String cSex;
    VR_ConfigureIF::Instance()->getTTSLanguage(m_Language, cTTSLang, cSex);
    msg->mutable_base()->set_language(cTTSLang);
    msg->mutable_base()->set_speaker_sex(cSex);
    msg->set_resptype(TtsPlayList_TtsResponseType_TtsResponse_Need);

    VR_EventSenderIF::GetSender()->SendEvent(EV_EVENT_TTS_PLAYLIST, msg.release());

    m_lastActionId = this->GetActionId();
}

void VR_ActionTtsPlay::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    std::unique_ptr<VrActionDM2DE> replyMsg(VrActionDM2DE::default_instance().New());
    replyMsg->set_id(this->GetActionId());
    replyMsg->set_vr_dm_action_type(ActionType_TTS_COMMAND_RESULT);

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
