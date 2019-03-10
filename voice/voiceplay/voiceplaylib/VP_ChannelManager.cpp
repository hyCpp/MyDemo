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

#include "VP_ChannelManager.h"
using namespace std;

#ifdef      HAVE_NUTSHELL_OS
using nutshell::NCString;
using nutshell::NC_BOOL;
using nutshell::NCSubAudioManager;

typedef     nutshell::NCSubAudioManager AUDIO_MANAGER;

#else
#include "VP_NativeAudioManager.h"
typedef     VP_NativeAudioManager       AUDIO_MANAGER;

#endif

VP_ChannelManager::VP_ChannelManager(shared_ptr<AUDIO_MANAGER> audioMgr)
{
    if (nullptr == audioMgr) {
        m_audioMgr = make_shared<AUDIO_MANAGER>();
    }
    else {
        m_audioMgr = audioMgr;
    }
    m_pSoundPlayer = VP_Factory::Instance()->AcquireSoundPlayer();
    m_listener = new(MEM_Voice) VP_AudioManagerListener(m_audioMgr);
}

VP_ChannelManager::~VP_ChannelManager()
{
    unregisterChannels();
    delete m_listener;
    m_listener = nullptr;
}

bool VP_ChannelManager::registerChannel(const string& channelName)
{
    auto itr = m_channels_table.find(channelName);
    if (itr != m_channels_table.end()) {
        VP_LOGD("Channel %s has been registered!\n", channelName.c_str());
        RETURN(false);
    }

    m_audioMgr->registerAvm(channelName.c_str(), m_listener);

    shared_ptr<VP_Channel> channel = make_shared<VP_Channel>(channelName, m_audioMgr);
    m_channels_table.insert(make_pair(channelName, channel));
    m_channels_list.push_back(channel);

    RETURN(true);
}

bool VP_ChannelManager::unregisterChannel(const string& channelName)
{
    auto itr = m_channels_table.find(channelName);
    if (itr == m_channels_table.end()) {
        VP_LOGD("nonexistent channel name!\n");
        RETURN(false);
    }

    m_audioMgr->unregister(channelName.c_str());
    RETURN(true);
}

void VP_ChannelManager::unregisterChannels() 
{
    auto itr = m_channels_table.begin();
    for (; itr != m_channels_table.end(); ++itr) {
        m_audioMgr->unregister(itr->first.c_str());
    }
}

shared_ptr<VP_Channel> VP_ChannelManager::getChannel(const string& channelName) const
{
    auto itr = m_channels_table.find(channelName);
    if (itr == m_channels_table.end()) {
        RETURN(nullptr);
    }

    RETURN(itr->second);
}

const VoiceList<shared_ptr<VP_Channel>>::type & VP_ChannelManager::getChannels() const
{
    RETURN(m_channels_list);
}

void VP_ChannelManager::queryAudioStatus(const string& currentSource)
{
    shared_ptr<VP_Channel> channel = getChannel(currentSource);
    if (nullptr != channel) {
        channel->queryAudioStatus();
    }
}

void VP_ChannelManager::confirmAudioStatus(const string& sourceName, const string& currentSource, bool reqAudio)
{
    shared_ptr<VP_Channel> channel = getChannel(sourceName);
    if (nullptr != channel) {
        channel->confirmAudioStatus(currentSource, reqAudio);
    }
}

#ifdef HAVE_NUTSHELL_OS
//////////////////////////DEFINITION FOR VP_AudioManagerListener////////////////////////

VP_AudioManagerListener::VP_AudioManagerListener(shared_ptr<NCSubAudioManager> audioMgr)
{
    m_audioMgr = audioMgr;
    m_pControl = VP_Factory::Instance()->AcquirePlayControl();
}

VOID VP_AudioManagerListener::currentAudioSource(const nutshell::NCString &recvAudio,
    const nutshell::NCString &audioName, NC_BOOL reqAudio)
{
    VP_LOGI("currentAudioSource: audioName is [%s]", audioName.getString());
    shared_ptr<VP_Event> event = make_shared<VP_ChannelConfirmedEvent>(
        recvAudio.getString(), audioName.getString(), reqAudio);
    m_pControl->PostEvent(event);
}
    
// callback after reqAudioSource
VOID VP_AudioManagerListener::notifyStatusChanged(const NCString &audioName, 
        const NCString &status) 
{
    VP_LOGI("audioName=%s,status=%s", audioName.getString(), status.getString());

    shared_ptr<VP_ChannelManager> channelMgr = m_pControl->getChannelManager();
    if (nullptr == channelMgr) {
        return;
    }

    shared_ptr<VP_Channel> channel = channelMgr->getChannel(audioName.getString());
    if (nullptr == channel) {
        VP_DEBUG_ERROR("Channel %s havn't been registered !", audioName.getString());
        return;
    }

    m_audioMgr->cfmAudioStatus(audioName, status);

    shared_ptr<VP_Event> event = nullptr;
    if (NCString("on") == status) {
        event = make_shared<VP_ChannelOpenedEvent>(channel);
    }
    else if (NCString("off") == status) {
        event = make_shared<VP_ChannelClosedEvent>(channel);
    }
    else if (NCString("wait") == status) {
        event = make_shared<VP_ChannelWaitingEvent>(channel);
    }
    else if (NCString("stop") == status) {
        event = make_shared<VP_ChannelStopEvent>(channel);
    }
    else {
        VP_DEBUG_ERROR("Unsupported status for avm!");
    }

    m_pControl->PostEvent(event);
}

// ask by AvManager
VOID VP_AudioManagerListener::askAudioStatus(const NCString &audioName)
{
    VP_LOGI("askAudioStatus for source : [%s]", audioName.getString());
    shared_ptr<VP_Event> event = make_shared<VP_ChannelStatusQueryEvent>(audioName.getString());
    m_pControl->PostEvent(event);
}

VOID VP_AudioManagerListener::notifyAMPStsChanged(const NCString& audioName, NC_BOOL connect)
{
    shared_ptr<VP_ChannelManager> channelMgr = m_pControl->getChannelManager();
    if (nullptr == channelMgr) {
        return;
    }

    shared_ptr<VP_Channel> channel = channelMgr->getChannel(audioName.getString());
    if (nullptr == channel) {
        VP_DEBUG_ERROR("Channel %s hasn't been registered!", audioName.getString());
        return;
    }

    if (connect) {
        VP_LOGD("AMP comes back, audio name is: [%s]\n", audioName.getString());
        return;
    }
    else {
        VP_LOGD("AMP disconnected with us, audio name is: [%s]\n", audioName.getString());
        shared_ptr<VP_Event> event = make_shared<VP_ChannelClosedEvent>(channel);
        m_pControl->PostEvent(event);
    }
}

VOID VP_AudioManagerListener::onConnected() 
{
}

#else

VP_AudioManagerListener::VP_AudioManagerListener(shared_ptr<AUDIO_MANAGER> audioMgr)
{
    m_audioMgr = audioMgr;
    m_pControl = VP_Factory::Instance()->AcquirePlayControl();
}

void VP_AudioManagerListener::notifyStatusChanged(const string &audioName, const string &status)
{
    VP_LOGI("audioName=%s,status=%s", audioName.c_str(), status.c_str());

    shared_ptr<VP_ChannelManager> channelMgr = m_pControl->getChannelManager();
    if (nullptr == channelMgr) {
        return;
    }

    shared_ptr<VP_Channel> channel = channelMgr->getChannel(audioName.c_str());
    if (nullptr == channel) {
        VP_DEBUG_ERROR("Channel %s havn't been registered !", audioName.c_str());
        return;
    }

    shared_ptr<VP_Event> event = nullptr;

    if (status == "on") {
        event = make_shared<VP_ChannelOpenedEvent>(channel);
    }
    else if (status == "off") {
        event = make_shared<VP_ChannelClosedEvent>(channel);
    }

    m_pControl->PostEvent(event);
}

#endif      // end of HAVE_NUTSHELL_OS
/* EOF */
