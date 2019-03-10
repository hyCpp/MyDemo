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
 * @file VR_ActionTtsListPlay.cpp
 * @brief Declaration file of class VR_ActionTtsListPlay.
 *
 * This file includes the declaration of class VR_ActionTtsListPlay.
 *
 * @attention used for C++ only.
 */
#include "stdafx.h"
#include <memory>
#include "VR_EventSenderIF.h"
#include "VR_ActionEventSender.h"

#include "VR_ActionTtsListPlay.h"
#include "VR_DailogEngineMessageBuilder.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include <boost/assign/list_of.hpp>
#include "navi/Voice/TTS/TtsPlayList.pb.h"
#include "navi/Voice/TTS/TtsPlayText.pb.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "VR_Configure.h"

using namespace navi::VoiceRecog;
using namespace navi::TTS;
int VR_ActionTtsListPlay::m_lastActionId = 0;
int VR_ActionTtsListPlay::m_EngineTtsPlayId = 0;

VR_ActionTtsListPlay::VR_ActionTtsListPlay(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
    , m_Language(((VrTtsListCommandReq&)proto).language())
    , m_ReqMsg(proto.New())
{
    m_EngineTtsPlayId = m_EngineReqId;
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
}

VR_ActionTtsListPlay::~VR_ActionTtsListPlay()
{
    VR_LOGD_FUNC();
}

int VR_ActionTtsListPlay::getLastActionId()
{
    VR_LOGI("m_lastActionId=[%d]", m_lastActionId);
    return m_lastActionId;
}

int VR_ActionTtsListPlay::getLastEngineId()
{
    VR_LOGI("m_EngineTtsPlayId=[%d]", m_EngineTtsPlayId);
    return m_EngineTtsPlayId;
}

void VR_ActionTtsListPlay::Run()
{
    VR_LOGD_FUNC();
    const VrTtsListCommandReq* ttsListCommandReq = static_cast<const VrTtsListCommandReq*>(m_ReqMsg.get());
    std::unique_ptr<TtsPlayList> msg(TtsPlayList::default_instance().New());

    VR_INT32 ttsCount = ttsListCommandReq->tts_phrase_size();
    VR_LOGI("ttsCount=[%d]", ttsCount);

    for (VR_INT32 i = 0; i < ttsCount; i++) {
        TtsPlayList_Phrase* phrase = msg->add_phrases();
        if (VrTtsListPhrase_VrPhraseType_text == ttsListCommandReq->tts_phrase(i).type()) {
            phrase->set_type(TtsPlayList_Phrase_PHRASE_TYPE_PHRASE_TEXT);
            phrase->set_text(ttsListCommandReq->tts_phrase(i).tts_text());
            VR_LOGI("tts_text[%d]=[%s]", i, ttsListCommandReq->tts_phrase(i).tts_text().c_str());
        }
        else if (VrTtsListPhrase_VrPhraseType_soundName == ttsListCommandReq->tts_phrase(i).type()) {
            phrase->set_type(TtsPlayList_Phrase_PHRASE_TYPE_PHRASE_SOUNDNAME);
            phrase->set_filename(ttsListCommandReq->tts_phrase(i).beep_path());
            VR_LOGI("beep_path[%d]=[%s]", i, ttsListCommandReq->tts_phrase(i).beep_path().c_str());
        }
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

void VR_ActionTtsListPlay::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    VR_String strXml = "";

    VrTTSCommandResult pOldMsg = (VrTTSCommandResult&)proto;
    VrTTSProcessType ttsState = pOldMsg.func_code();
    switch (ttsState) {
    case TTSProcessType_Stop:
        strXml = "<action-result agent=\"prompt\"  op=\"playTtsList\" errcode=\"0\">"
                 "</action-result>";
        break;
    case TTSProcessType_Pause:
        strXml = "<action-result agent=\"prompt\"  op=\"stopTtsList\" errcode=\"0\">"
                 "</action-result>";
        break;
    default:
        return;
    }

    m_sender->sendEngineReply(m_EngineReqId, strXml);
    this->SetActionComplete();
}
/* EOF */
