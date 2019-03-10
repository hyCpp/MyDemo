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
#include "VP_Event.h"
#include "CL_WaitObj.h"

using namespace std;
using namespace ::navi::VoicePlay;

shared_ptr<VP_Event> VP_PlayListEvent::DoAction()
{
    VP_LOGD_FUNC();
    shared_ptr<VP_VoicePlayControl> playControl = VP_Factory::Instance()->AcquirePlayControl();
    if (playControl->isMute()) {
        VP_LOGD("Current is mute state, directly return result!");
        playControl->postResult(sender, userdata, VP_PLAYRESULT_ABORT);
        RETURN(nullptr);
    }

    VoiceVector<shared_ptr<VP_DataAccessor>>::type pList;
    WrapDataAccessor(pList);

    m_channel = playControl->getChannel(sender, m_ssid);
    shared_ptr<VP_DataAccessorList> pAccessorList = make_shared<VP_DataAccessorList>(pList, sender, userdata);

    if (nullptr == m_channel || nullptr == pAccessorList) {
        VP_DEBUG_ERROR("Channel for [%s] doesn't exist or play list is NULL\n", sender.c_str());
        RETURN(nullptr);
    }

    pAccessorList->setChannelId(m_ssid);
    pAccessorList->setPlayEndNotify(m_bNotify);
    pAccessorList->setPlayControl(playControl);
    // Do not call setWaitPlayList of channel here or we will need a lock to ensure the exclusion of resource
    shared_ptr<VP_ChannelOpenEvent> event = make_shared<VP_ChannelOpenEvent>(m_channel, pAccessorList);
    if (event != nullptr) {
        event->DoAction();
    }

    RETURN(nullptr);
}

bool VP_PlayListEvent::WrapDataAccessor(VoiceVector<shared_ptr<VP_DataAccessor>>::type &pList) 
{
    CL_ERROR err = SetPhraseInfos();
    if (APL_ERR_NO_ERROR != err) {
        RETURN(false);
    }

    shared_ptr<VP_DataAccessor> pAccessor = nullptr;
    VP_LOGD("VP_SoundPlay_Request: size of m_phraseList is: [%d]\n", m_phraseList.size());
    VoiceList<shared_ptr<VP_PhraseImp>>::iterator itr = m_phraseList.begin();

    while (m_phraseList.end() != itr) {
        shared_ptr<VP_PhraseImp> phrase = (*itr);
        if (phrase->IsCancelled()) {
            ++itr;
            continue;
        }

        pAccessor = phrase->CreateDataAccessor();
        if (nullptr == pAccessor) {
            VP_DEBUG_ERROR("DataAccessor is nullptr, return...\n");
            pList.clear();
            RETURN(false);
        }
        pList.push_back(pAccessor);
        ++itr;
    }

    RETURN(true);
}

CL_ERROR VP_PlayListEvent::SetPhraseInfos() 
{
    CL_ERROR err = APL_ERR_NO_ERROR;
    shared_ptr<VP_DataManager> dataMgr = VP_Factory::Instance()->AcquireDataMgr();
    shared_ptr<VP_PhraseImp> pLastPhrase = nullptr;

    if (nullptr == dataMgr) {
        VP_DEBUG_ERROR("DataMgr can't be nullptr!");
        err = APL_ERR_VP_MEMORY_ERROR;
        RETURN(err);
    }
    
    VoiceList<shared_ptr<VP_PhraseImp>>::iterator itr = m_phraseList.begin();
    while (itr != m_phraseList.end()) {
        shared_ptr<VP_PhraseImp> phrase = (*itr);
        err = phrase->SetPhraseInfo(*dataMgr);
        
        if (APL_ERR_NO_ERROR == err) {
            pLastPhrase = phrase;
            ++itr;
        }
        else if (APL_ERR_VP_PHRASE_NOTEXIST == err) {
            if (itr == m_phraseList.begin() || !phrase->CanBeCancelled()) {
                RETURN(err);
            }
            phrase->SetCancelled();
            pLastPhrase->SetCancelled();
            ++itr;
        }
        else {
            RETURN(err);
        }
    }

    RETURN(err);
}

void VP_PlayListEvent::setPlayEndNotify(bool notify)
{
    m_bNotify = notify;
}

bool VP_PlayListEvent::getPlayEndNotify() const
{
    return m_bNotify;
}

shared_ptr<VP_Event> VP_ChannelOpenEvent::DoAction() 
{
    VP_LOGD_FUNC();
    VP_CHANNEL_STATUS status = m_channel->getChannelStatus();
    shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();
    shared_ptr<VP_DataAccessorList> origList = m_channel->getWaitPlayList();

    m_channel->setWaitPlayList(m_pList);
    VP_LOGD("Current status of channel [%s] is:[%d], status of player is:[%d]\n",
            m_channel->getChannelName().c_str(), status, player->getPlayerStatus());
    if (CHANNEL_STATUS_OPENED == status) {
        VP_PLAYER_STATUS player_status = player->getPlayerStatus();
        if (player_status == PLAYER_STATUS_PLAYING || player_status == PLAYER_STATUS_READY) {
            player->Stop();
        }
        else {
            // do nothing, because we can ensure player is playing or ready when channel is opened
        }
    }
    else if (CHANNEL_STATUS_IDLE == status) {
        VP_LOGI("Opening channel %s...\n", m_channel->getChannelName().c_str());
        m_channel->open();
    }
    else if (CHANNEL_STATUS_OPENING == status || CHANNEL_STATUS_WAITING == status) {
        player->postResult(origList, VP_PLAYRESULT_ABORT);
    }
    else {
        // the status of channel is closing or closed, we do not need do anything
    }

    RETURN(nullptr);
}

/* There are two ways to call open-channel event:
 * 1. A new play request, sequence is open channel -> open player -> play list
 * 2. Reuse the current channel and player, we should send back the play result
 * to the sender at this moment, the sequence is:
 * stop current playing list -> send back result -> re-open the player
 */
shared_ptr<VP_Event> VP_ChannelOpenedEvent::DoAction() 
{
    VP_LOGD_FUNC();
    VP_CHANNEL_STATUS channel_status = m_channel->getChannelStatus();

    if (CHANNEL_STATUS_OPENING == channel_status) {
        shared_ptr<VP_DataAccessorList> waitPlayList = m_channel->getWaitPlayList();
        shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();

        if (nullptr != waitPlayList) {
            m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);

            VP_LOGD("Ready to play list for module [%s]\n", waitPlayList->getSender().c_str());
            player->Play(waitPlayList);
            m_channel->setWaitPlayList(nullptr);
        }
        else {
            VP_LOGD("waitPlayList is empty so request to close the channel [%s]\n",
                    m_channel->getChannelName().c_str());
            m_channel->close();
        }
    }
    else if (CHANNEL_STATUS_IDLE == channel_status) {
        VP_LOGD("Nothing is need to play, close the channel!");
        m_channel->close();
    }
    else if (CHANNEL_STATUS_CLOSING == channel_status) {
        m_channel->close();
    }
    else if (CHANNEL_STATUS_WAITING == channel_status) {
        CL_WaitObj waitObj;
        waitObj.Wait(100);
        VP_LOGD("Wait 100ms to resume play!");
        resumePlay();
    }
    else {
        /* the status of channel is opened or closed,
         * 1. it means that we receive an 'on'command from avm when we are already opened,
         * do not need to handle this situation;
         * 2. it means that we receive an 'on' command from avm when we are already closed,
         * do not need to handle this situation because we didn't request it.
         */
    }

    RETURN(nullptr);
}

void VP_ChannelOpenedEvent::resumePlay()
{
    VP_LOGD("Channel [%s] is resumed by accepting an 'on' command\n", m_channel->getChannelName().c_str());
    shared_ptr<VP_DataAccessorList> resumedPlayList = m_channel->getWaitPlayList();
    shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();

    if (nullptr != resumedPlayList) {
        m_channel->setChannelStatus(CHANNEL_STATUS_OPENED);
        VP_LOGI("Abort? [%s]\n", resumedPlayList->isAbort() ? "true" : "false");
        resumedPlayList->setAbort(resumedPlayList->isAbort());

        int index = resumedPlayList->getPlayIndex();
        index = index > 0 ? index - 1 : index;
        resumedPlayList->setPlayIndex(index);

        player->Play(resumedPlayList);
        m_channel->setWaitPlayList(nullptr);
    }
    else {
        m_channel->close();
    }
}

/* There are two situation to do this action:
 * 1. just stop this player
 * 2. interrupt the current playing item and play the new request
 *    For the second situation, we do not need to close channel and re-open
 *    it, just set its status as CHANNEL_OPENED and play it.
 */
shared_ptr<VP_Event> VP_PlayerStopEvent::DoAction()
{
    VP_LOGD_FUNC();
    shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();
    VP_CHANNEL_STATUS channel_status = m_channel->getChannelStatus();

    VP_LOGD("Before stop play, the status of channel is: [%d]\n", channel_status);
    if (CHANNEL_STATUS_OPENED == channel_status) {
        // Do not need to judge the status of player, the job will be done in Stop
        player->Stop(); // stop the current playing list
    }
    else if (CHANNEL_STATUS_OPENING == channel_status || CHANNEL_STATUS_WAITING == channel_status) {
        VP_LOGD("Abort the waiting playlist!\n");
        shared_ptr<VP_DataAccessorList> waitPlayList = m_channel->getWaitPlayList();
        if (nullptr != waitPlayList) {
            waitPlayList->setAbort(true);
        }
    }
    else {
        /* the stratergy of 'stop' command from avm is:
         * 1. if channel is opened, close it;
         * 2. else do nothing
         */
    }

    RETURN(nullptr);
}

shared_ptr<VP_Event> VP_PlayerStoppedEvent::DoAction() 
{
    VP_LOGD_FUNC();
    VP_CHANNEL_STATUS channel_status = m_channel->getChannelStatus();
    shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();
    VP_LOGD("In stopped event, status of channel [%s] is:[%d]\n", m_channel->getChannelName().c_str(), channel_status);

    player->setPlayerStatus(PLAYER_STATUS_IDLE);
    if (CHANNEL_STATUS_OPENED == channel_status) {
        shared_ptr<VP_DataAccessorList> waitPlayList = m_channel->getWaitPlayList();
        if (nullptr != waitPlayList) {
            VP_LOGD("There is a waiting playlist need to play, call play method.\n");
            player->Play(waitPlayList);
            m_channel->setWaitPlayList(nullptr);
        }
        else {
            // no more playlist need to play, close the channel in case of opening channel failed next time
            VP_LOGI("Ready to request to close the channel %s\n", m_channel->getChannelName().c_str());
            m_channel->close();
        }
    }
    else if (CHANNEL_STATUS_CLOSED == channel_status) {
        m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    } 
    else if (CHANNEL_STATUS_WAITING == channel_status) {
        shared_ptr<VP_DataAccessorList> waitPlayList = m_channel->getWaitPlayList();
        if (nullptr != waitPlayList) {
            waitPlayList->setAbort(false);
        }
    }
    else {
        /* the status of channel can only be opend or closed:
         * for opened, when player finish its playing or we stop it, the status of channel will be opened
         * for closed, avm send us an 'off' command, channel has been closed, we just need to stop playing
         */
    }

    RETURN(nullptr);
}

shared_ptr<VP_Event> VP_ChannelClosedEvent::DoAction() 
{
    VP_LOGD_FUNC();
    VP_LOGI("Channel %s has been closed!\n", m_channel->getChannelName().c_str());

    VP_CHANNEL_STATUS status = m_channel->getChannelStatus();
    shared_ptr<VP_DataAccessorList> waitPlayList = m_channel->getWaitPlayList();
    shared_ptr<VP_Event> event = nullptr;
    shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();

    // we are requesting to open channel but receiving a 'off' command
    if (CHANNEL_STATUS_OPENING == status || CHANNEL_STATUS_WAITING == status) {
        player->postResult(waitPlayList, VP_PLAYRESULT_ABORT);   // send back the result to caller
        waitPlayList->setAbort(true);
        m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    }
    else if (CHANNEL_STATUS_CLOSING == status) {
        if (nullptr != waitPlayList) {
            // there is another playlist waiting for playing, post a channel-open-event
            event = make_shared<VP_ChannelOpenEvent>(m_channel, waitPlayList);
        }
        m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    }
    else if (CHANNEL_STATUS_OPENED == status) {
        VP_LOGD("Ready to stop the player!\n");

        m_channel->setChannelStatus(CHANNEL_STATUS_CLOSED); // CLOSED marks that we do not need to close channel
        player->Stop(); // do not need to check the status of player, Stop method will check it in its body

        VP_PLAYER_STATUS player_status = player->getPlayerStatus();
        if (PLAYER_STATUS_IDLE == player_status) {
            m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
        }
    }
    else {
        // the status of channel is closed or idle, do not need to do anything
    }

    RETURN(event);
}

shared_ptr<VP_Event> VP_ChannelWaitingEvent::DoAction()
{
    VP_LOGD_FUNC();
    VP_CHANNEL_STATUS status = m_channel->getChannelStatus();
    shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();
    VP_PLAYER_STATUS player_status = player->getPlayerStatus();

    VP_LOGD("Channel [%s] is waiting for 'on' command!\n", m_channel->getChannelName().c_str());
    if (CHANNEL_STATUS_OPENING == status) {
        m_channel->setChannelStatus(CHANNEL_STATUS_WAITING);
    }
    else if (CHANNEL_STATUS_OPENED == status) {
        if (PLAYER_STATUS_PLAYING == player_status
            || PLAYER_STATUS_READY == player_status) {   // player is in palying or ready-to-play state
            shared_ptr<VP_DataAccessorList> storedPlayList = player->getPlayList();
            m_channel->setWaitPlayList(storedPlayList);
            player->Stop();
        }
        m_channel->setChannelStatus(CHANNEL_STATUS_WAITING);
    }
    else if (CHANNEL_STATUS_CLOSING == status) {
        m_channel->close();
    }
    else {
        VP_LOGI("Channel %s does not accept this state [%d] in waiting\n",
               m_channel->getChannelName().c_str(), status);
    }

    RETURN(nullptr);
}

shared_ptr<VP_Event> VP_ChannelStopEvent::DoAction()
{
    VP_LOGD_FUNC();

    VP_CHANNEL_STATUS status = m_channel->getChannelStatus();
    shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();

    if (CHANNEL_STATUS_OPENED == status) {
        player->Stop();
    }
    else if (CHANNEL_STATUS_OPENING == status || CHANNEL_STATUS_WAITING == status) {
        player->postResult(m_channel->getWaitPlayList(), VP_PLAYRESULT_ABORT);
        m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    }
    else {
        m_channel->setWaitPlayList(nullptr);       // clear the wait play list
        m_channel->setChannelStatus(CHANNEL_STATUS_IDLE);
    }

    RETURN(nullptr);
}

shared_ptr<VP_Event> VP_SetMuteEvent::DoAction()
{
    VP_LOGD_FUNC();

    shared_ptr<VP_VoicePlayControl> playControl = VP_Factory::Instance()->AcquirePlayControl();
    playControl->StopAll();

    RETURN(nullptr);
}

shared_ptr<VP_Event> VP_ChannelConfirmedEvent::DoAction()
{
    VP_LOGD_FUNC();
    shared_ptr<VP_VoicePlayControl> playControl = VP_Factory::Instance()->AcquirePlayControl();
    shared_ptr<VP_ChannelManager> channelMgr = playControl->getChannelManager();

    channelMgr->confirmAudioStatus(m_sourceName, m_currentSource, m_reqAudio);
    RETURN(nullptr);
}

shared_ptr<VP_Event> VP_ChannelStatusQueryEvent::DoAction()
{
    VP_LOGD_FUNC();
    shared_ptr<VP_VoicePlayControl> playControl = VP_Factory::Instance()->AcquirePlayControl();
    shared_ptr<VP_ChannelManager> channelMgr = playControl->getChannelManager();

    channelMgr->queryAudioStatus(m_currentSource);
    RETURN(nullptr);
}

shared_ptr<VP_Event> VP_GetDurationEvent::DoAction()
{
    shared_ptr<VP_VoicePlayControl> playControl = VP_Factory::Instance()->AcquirePlayControl();
    int dwDuration = GetDuration();

    VpDuration *vp_duration = new(MEM_Voice)VpDuration();
    if (nullptr == vp_duration) {
        RETURN(nullptr);
    }

    vp_duration->set_sender(sender);
    vp_duration->set_userdata(userdata);
    vp_duration->set_duration(dwDuration);
    VP_LOGI("GetDuration: sender = [%s], total duration is [%u]\n", sender.c_str(), dwDuration);

    playControl->sendMessage(EV_EVENT_VOICEPLAY_DURATION, vp_duration);

    RETURN(nullptr);
}

int VP_GetDurationEvent::GetDuration() 
{
    CL_ERROR err;
    int dwDuration = VP_RESERVE_JIFFY; // prepared time for vp module initializing
    shared_ptr<VP_DataManager> dataMgr = VP_Factory::Instance()->AcquireDataMgr();

    if (nullptr == dataMgr) {
        RETURN(0);
    }

    auto itr = m_phraseList.begin();
    for (; itr != m_phraseList.end(); ++itr) {
        shared_ptr<VP_PhraseImp> phrase = (*itr);
        err = phrase->SetPhraseInfo(*dataMgr); 
        
        if (APL_ERR_NO_ERROR != err) {
            dwDuration = 0;
            VP_LOGD("!!GetDuration error!\n");
            break;
        }

        dwDuration += (phrase->GetDuration() + PHRASE_RESERVE_JIFFY);   // each phrase will consume about 50ms
    }

    RETURN(dwDuration);
}

shared_ptr<VP_Event> VP_LanguageChangeEvent::DoAction()
{
    VP_LOGD_FUNC();
    
    shared_ptr<VP_DataManager> dataMgr = VP_Factory::Instance()->AcquireDataMgr();
    if (nullptr == dataMgr) {
        RETURN(nullptr);
    }

    shared_ptr<VP_VoicePlayControl> playControl = VP_Factory::Instance()->AcquirePlayControl();
    playControl->StopAll();

    dataMgr->UnloadLanguage();

    dataMgr->LoadLanguage();

    RETURN(nullptr);
}

/* EOF */
