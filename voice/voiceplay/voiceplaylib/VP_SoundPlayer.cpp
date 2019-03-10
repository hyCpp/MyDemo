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
#    include "BL_Error.h"
#endif

#ifndef CXX_APLTHREADNAME_H
#    include "AplThreadName.h"
#endif

#include "VC_CommonDef.h"
#include "VC_AudioSession.h"
#include "VP_SoundPlayer.h"
#include "VC_Player.h"
#include "VP_DataAccessorList.h"
#include "VP_Factory.h"
#include "navi/Voice/TTS/TtsStop.pb.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VP_DurationManager.h"

using namespace ::navi::TTS;
using namespace ::navi::VoicePlay; 
using namespace std;

unsigned int VP_SoundPlayer::m_reqId = 0;

VP_SoundPlayer::VP_SoundPlayer(VC_Player* pPlayer)
    : m_state(PLAYER_STATUS_IDLE)
    , m_bIsTtsPlay(CL_FALSE)
    , m_bResultState(CL_TRUE)
    , m_currentReqId(VP_INVALID_REQID)
    , m_pPlayer(pPlayer)
{
}

VP_SoundPlayer::~VP_SoundPlayer()
{
}

CL_ERROR VP_SoundPlayer::Initialize()
{
#ifndef VP_UNIT_TEST
    RegisterName(const_cast<TCHAR*>(VP_PLAYSND_THREAD));
    StartRegistThread();
#endif
    return APL_ERR_NO_ERROR;
}

bool VP_SoundPlayer::CreateStopMsg()
{
    TtsStop* msg = new(MEM_Voice) TtsStop();     // proto
    if (NULL == msg) {
        VP_DEBUG_ERROR("msg is null\n");
        return false;
    }
    msg->set_reqid(m_currentReqId);
    TtsBase *base = msg->mutable_base();
    if (NULL == base) {
        VP_LOGI("*base is null");
        delete msg;
        return false;
    }

    base->set_sender(EV_MODULE_VOICEPLAY);
    EV_EventSender cSender;
    cSender.SendEvent(EV_EVENT_TTS_STOP, msg);
    VP_LOGI("Stop, stopmessage send to ttsmodule");
    return true;
}

void VP_SoundPlayer::Stop()
{
    VP_LOGI("VP_SoundPlayer:Stop() enter\n");

    if (nullptr == m_playList || m_playList->isAbort()) {
        return;
    }

    if (PLAYER_STATUS_PLAYING == m_state) {
        m_playList->setAbort(true);
        m_state = PLAYER_STATUS_STOPPING;

        if (m_bIsTtsPlay && !CreateStopMsg()) {        // iauto TTS play
            OnPlayComplete(false, m_currentReqId);
            return;
        }
        else {                // iauto WaveId play
            m_pPlayer->Stop();
        }
    }
    else if (PLAYER_STATUS_READY == m_state) {
        m_playList->setAbort(true);
        m_state = PLAYER_STATUS_STOPPING;
    }
    else {
        // if player is not playing or ready to play, we do not need to stop it
    }

    VP_LOGI("VP_SoundPlayer:Stop() exit\n");
    return;
}

void VP_SoundPlayer::Run()
{
#ifndef VP_UNIT_TEST
    while (true) {
        VP_LOGI("VP_SoundPlayer thread waiting...\n");
        Wait();
        VP_LOGI("Channel is opened and player is ready for playing, VP_SoundPlayer thread wake up\n");

        if (TRUE == CheckQuit()) {
            VP_LOGI("VP_SoundPlayer thread quit");
            break;
        }

        if (nullptr == m_playList) {
            VP_LOGD("Current playlist is nullptr, continue waiting...\n");
            continue;
        }

        Play();
        VP_LOGI("VP_SoundPlayer::Run, Play() end\n");
    }
#endif
}

void VP_SoundPlayer::Play(shared_ptr<VP_DataAccessorList> pList)
{
    m_playList = pList;
    m_state = PLAYER_STATUS_READY;
    Notify();
}

void VP_SoundPlayer::Play()
{
    VP_LOGD_FUNC();
    CL_ERROR err = APL_ERR_NO_ERROR;
    VP_PLAY_RESULT playRes = VP_PLAYRESULT_OK;
    shared_ptr<VP_DataAccessorList> playList = m_playList;  // ensure the consistency

    while (playList && !playList->isAbort() && playList->HasNext()) {
        m_state = PLAYER_STATUS_PLAYING;
        m_currentReqId = ReqIdCreater();
        m_bIsTtsPlay = CL_FALSE;

        err = playList->Play(m_pPlayer, this, m_bIsTtsPlay, 
                m_currentReqId, VP_ROUTE_GUIDANCE_TOKEN_ID);

        if (APL_ERR_NO_ERROR != err) {
            playRes = VP_PLAYRESULT_ERROR;
            VP_LOGI("play accessorList error, err=%d", err);
            continue;
        }

#ifndef VP_UNIT_TEST
        m_syncPlayEnd.Wait();   // wait for callback
#endif

        m_bIsTtsPlay = CL_FALSE;

        if (APL_ERR_NO_ERROR != err || CL_FALSE == m_bResultState) {
            VP_LOGI("VP_SoundPlayer::Play() failed, err=%ld, m_bResultState=%d\n", err, m_bResultState);
            playRes = VP_PLAYRESULT_ERROR;
        }
    }

    if (playList) {
        m_state = PLAYER_STATUS_STOPPING;   // mark the player has finished its job
        playRes = playList->isAbort() ? VP_PLAYRESULT_ABORT : playRes;
        onVpPlayEnd(playList, playRes);    
    }
}

void VP_SoundPlayer::onVpPlayEnd(shared_ptr<VP_DataAccessorList>& pList, VP_PLAY_RESULT playResult)
{
    pList->playFinished();

    postResult(pList, playResult);

    notifyStoppedState(pList);
}

void VP_SoundPlayer::notifyStoppedState(shared_ptr<VP_DataAccessorList> pList) 
{
    shared_ptr<VP_VoicePlayControl> playControl = VP_Factory::Instance()->AcquirePlayControl();
    shared_ptr<VP_Channel> channel = playControl->getChannel(pList->getSender(), pList->getChannelId());
    if (nullptr != channel) {
        shared_ptr<VP_PlayerStoppedEvent> event = make_shared<VP_PlayerStoppedEvent>(channel);
        playControl->PostEvent(event);
    }
}

VOID VP_SoundPlayer::OnPlayComplete(CL_BOOL bSuccess)
{
    OnPlayComplete(bSuccess, VP_INVALID_REQID);
}

VOID VP_SoundPlayer::OnPlayComplete(CL_BOOL bSuccess, int reqId)    // for play TTS
{
    VP_LOGI("bSuccess = %d, reqId = %d, m_currentReqId = %d", bSuccess, reqId, m_currentReqId);
    m_bResultState = bSuccess;
    if (reqId == VP_INVALID_REQID || m_currentReqId == reqId) {
        m_currentReqId = reqId == VP_INVALID_REQID ? m_currentReqId : VP_INVALID_REQID; // reqId = -1 means playing wav 
        m_syncPlayEnd.Notify();
    }
}

VOID VP_SoundPlayer::Init()
{
    if (m_pPlayer) {
        m_pPlayer->Init();
    }
}

int VP_SoundPlayer::ReqIdCreater()
{
    return ((m_reqId++) % VP_MAX_REQID);
}

void VP_SoundPlayer::StopPlayerThread()
{
    m_pPlayer->StopPlayer();
    StopThread();
}

VP_PLAYER_STATUS VP_SoundPlayer::getPlayerStatus() const 
{
    return m_state;
}

void VP_SoundPlayer::setPlayerStatus(VP_PLAYER_STATUS status)
{
    m_state = status;
}

shared_ptr<VP_DataAccessorList> VP_SoundPlayer::getPlayList() const
{
    return m_playList;
}

void VP_SoundPlayer::postResult(shared_ptr<VP_DataAccessorList> pList, VP_PLAY_RESULT playResult)
{
    if (nullptr == pList || pList->getSender().empty()) {
        return;
    }

    if (!pList->getPlayEndNotify()) {
        return;
    }

    shared_ptr<VP_VoicePlayControl> playControl = VP_Factory::Instance()->AcquirePlayControl();
    playControl->postResult(pList->getSender(), pList->getUserData(), playResult);
}

void VP_SoundPlayer::onTtsServiceDown()
{
    if (m_currentReqId >= 0 && m_bIsTtsPlay) {
        OnPlayComplete(false, m_currentReqId);
    }
}

/* EOF */
