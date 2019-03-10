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

#ifndef CXX_BL_ERROR_H
#	include "BL_Error.h"
#endif

#include "VP_DataAccessorSound.h"
#include "VP_SoundFile.h"
#include "VC_Player.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VC_AudioSession.h"
#include "VP_Factory.h"
#include "VC_CommonDef.h"
#include "navi/Voice/TTS/TtsBasePlay.pb.h"
#include "navi/Voice/TTS/TtsBase.pb.h"
#include "VP_Define.h"
#include "BL_SystemLanguage.h"

using namespace std;
using namespace ::navi::TTS;

CL_ERROR VP_DataAccessorID::Play(VC_Player* pPlayer, VC_Player::Listener *pListener,
        CL_BOOL& isTtsPlay, int reqId, int tokenid)
{
    VP_LOGI("VP_DataAccessorID::Play4 enter");
    return pPlayer->Play(m_strFileName, m_dwOffset, m_dwSize, 
            pListener, tokenid, m_bLastPhrase) ? APL_ERR_NO_ERROR : APL_ERR_VP_AUDIO_PLAY;
}

CL_ERROR VP_DataAccessorBuf::Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
        CL_BOOL& isTtsPlay, int reqId, int tokenid)
{
    VP_LOGI("VP_DataAccessorBuf::play item read from memory");
    if (nullptr == m_pBuf || 0 == m_dwSize) {
        VP_DEBUG_ERROR("buf is nullptr or size is 0!");
        return CL_FALSE;
    }
    // Direct static memory buffer, such as beep file
    return pPlayer->Play(m_pBuf, m_dwSize, pListener, 
            tokenid, m_bLastPhrase) ? APL_ERR_NO_ERROR : APL_ERR_VP_AUDIO_PLAY;
}

void VP_DataAccessorBuf::playFinished()
{
    if (nullptr != m_pBuf) {
        delete m_pBuf;
        m_pBuf = nullptr;
    }
}

CL_ERROR VP_DataAccessorWaveFile::Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
        CL_BOOL& isTtsPlay, int reqId, int tokenid)
{
    VP_LOGI("VP_DataAccessorWaveFile::play item read from wave file");
    return CL_FALSE;
}

CL_ERROR VP_DataAccessorTts::Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
        CL_BOOL& isTtsPlay, int reqId, int tokenid)
{
    if (!m_pControl->getTtsServiceStatus()) {
        return APL_ERR_VP_AUDIO_PLAY;
    }

    VP_LOGI("VP_DataAccessorTts::Sending play text message to TTS module...\n");
    isTtsPlay = CL_TRUE;

    TtsPlayText* playTtsMsg = CreateMessage(reqId, tokenid);
    if (nullptr != playTtsMsg) {
        SendMessage(EV_EVENT_TTS_PLAYTEXT, playTtsMsg);
        MakeTimeTag("message send to tts module time");
        return APL_ERR_NO_ERROR;
    }

    return APL_ERR_VP_NEW_ERROR;
}

TtsPlayText* VP_DataAccessorTts::CreateMessage(int reqId, int tokenid)
{
    TtsPlayText* playTextMsg = new(MEM_Voice) TtsPlayText();
    if (nullptr == playTextMsg) {
        VP_LOGI("VP_DataAccessorTts, playTextMsg is null..");
        return nullptr;
    }

    TtsBasePlay* basePlay = playTextMsg->mutable_play();
    if (nullptr == basePlay) {
        VP_LOGI("VP_DataAccessorTts, basePlay is null..");
        delete playTextMsg;
        return nullptr;
    }
    basePlay->set_reqid(reqId);
    basePlay->set_priority(TtsBasePlay::OP_PRI_Navi);
    basePlay->set_tokenid(tokenid);
    basePlay->set_drain_wait(m_bLastPhrase);
    shared_ptr<VP_DataManager> dataManager = VP_Factory::Instance()->AcquireDataMgr();

    if (nullptr != dataManager) {
        basePlay->set_language(m_ttsLang);
        basePlay->set_speaker_sex(dataManager->GetGender());
        VP_LOGD("Parameters for TTS module -- language: [%s], gender: [%s]\n",
                m_ttsLang.c_str(), dataManager->GetGender().c_str());
    }

    TtsBase* base = basePlay->mutable_base();
    if (nullptr == base) {
        VP_LOGI("VP_DataAccessorTts, base is null..");
        delete playTextMsg;
        return nullptr;
    }
    base->set_sender(EV_MODULE_VOICEPLAY);
    base->set_onresponse(true);
    
    playTextMsg->set_text(m_strTtsText);

    return playTextMsg;
}

void VP_DataAccessorTts::SendMessage(EV_EVENT_ID msgId, EV_MSG_PTR msg) 
{
    if (nullptr == msg) {
        VP_DEBUG_ERROR("msg is null\n");
        return;
    }
    EV_EventSender cSender;
    cSender.SendEvent(msgId, msg);
    VP_LOGI("send play message to tts module");
}

/* EOF */
