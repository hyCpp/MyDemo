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

#include "VP_Channel.h"
using namespace std;

VP_Channel::VP_Channel(const string& channelName, 
        shared_ptr<AUDIO_MANAGER> audioMgr)
    : m_selfName(channelName)
    , m_audioMgr(nullptr)
    , m_selfStatus(CHANNEL_STATUS_IDLE)
{
    m_audioMgr = audioMgr;
}

void VP_Channel::reqAudioSource(bool on) 
{
    if (nullptr != m_audioMgr) {
        m_audioMgr->reqAudioSource(m_selfName.c_str(), on);
    }
} 

/*
 * reqAudioSource according to the status of
 * channel, do not set the status of channel directly.
 */
void VP_Channel::open()
{
    reqAudioSource(true);
    m_selfStatus = CHANNEL_STATUS_OPENING;
}

/*
 * reqAudioSource according to the status of
 * channel, do not set the status of channel
 * directly, such as, if (CHANNEL_STATUS_IDLE == m_selfStatus)
 * do nothing;
 */
void VP_Channel::close()
{
    reqAudioSource(false);
    m_selfStatus = CHANNEL_STATUS_CLOSING;
}

void VP_Channel::setChannelStatus(VP_CHANNEL_STATUS status) 
{
    m_selfStatus = status;
}

VP_CHANNEL_STATUS VP_Channel::getChannelStatus() const 
{
    RETURN(m_selfStatus);
}

string VP_Channel::getChannelName() const 
{
    RETURN(m_selfName);
}

void VP_Channel::setWaitPlayList(std::shared_ptr<VP_DataAccessorList> waitPlayList)
{
    m_waitPlayList = waitPlayList;
}

shared_ptr<VP_DataAccessorList> VP_Channel::getWaitPlayList() const
{
    RETURN(m_waitPlayList);
}

void VP_Channel::queryAudioStatus()
{
    if (CHANNEL_STATUS_OPENED == m_selfStatus) {
        VP_LOGI("Channel [%s] is still opened, response continue", m_selfName.c_str());
        m_audioMgr->cfmAudioStatus(m_selfName.c_str(), "continue");
    }
    else if (CHANNEL_STATUS_OPENING == m_selfStatus || CHANNEL_STATUS_WAITING == m_selfStatus) {
        VP_LOGI("Channel [%s] request to open or wait to open, but it was already opened!", m_selfName.c_str());
        m_audioMgr->cfmAudioStatus(m_selfName.c_str(), "continue");
        m_selfStatus = CHANNEL_STATUS_OPENED;   // there should be a msg that notify play-control to play list when status is opened
    }
    else if (CHANNEL_STATUS_CLOSING == m_selfStatus) {
        VP_LOGI("Channel [%s] will close in a moment, reply continue and then request to close!", m_selfName.c_str());
        m_audioMgr->cfmAudioStatus(m_selfName.c_str(), "continue");
        m_audioMgr->reqAudioSource(m_selfName.c_str(), false);
    }
    else {
        VP_LOGI("Long time no response, request to turn off the channel [%s]\n", m_selfName.c_str());
        m_audioMgr->reqAudioSource(m_selfName.c_str(), false);
    }
}

void VP_Channel::resetChannelStatus()
{
    switch (m_selfStatus) {
    case CHANNEL_STATUS_OPENING:
    case CHANNEL_STATUS_OPENED:
        VP_LOGD("Current channel is not me, but I'm in opening or opened status, try to request to open!");
        open();
        break;
    case CHANNEL_STATUS_CLOSING:
        VP_LOGD("Incorrect status of closing channel [%s]!", m_selfName.c_str());
        m_selfStatus = CHANNEL_STATUS_IDLE;
        break;
    default:
        break;
    }
}

void VP_Channel::confirmAudioStatus(const string& currentSource, bool reqAudio)
{
    VP_LOGD("My name is [%s] and status is [%d], current source name is [%s], reqAudio is [%d]\n",
        m_selfName.c_str(), m_selfStatus, currentSource.c_str(), reqAudio);
    if (!reqAudio || m_selfName != currentSource) {
        resetChannelStatus();
        return;
    }

    shared_ptr<VP_SoundPlayer> player = VP_Factory::Instance()->AcquireSoundPlayer();
    switch (m_selfStatus) {
    case CHANNEL_STATUS_IDLE:
    case CHANNEL_STATUS_CLOSING:
        VP_LOGD("Current channel is me, but I have already request to close, request again!");
        close();
        break;
    case CHANNEL_STATUS_WAITING:
    case CHANNEL_STATUS_OPENING:
        VP_LOGD("I'm already opened, play list directly!");
        m_selfStatus = CHANNEL_STATUS_OPENED;
        player->Play(m_waitPlayList);   // should send back a play message to nofity playControl to play list
        break;
    default:
        break;
    }

}

/* EOF */
