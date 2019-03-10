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

#include "CL_AutoSync.h"

#ifndef CXX_BL_ERROR_H
#    include "BL_Error.h"
#endif

#ifndef CXX_APLTHREADNAME_H
#    include "AplThreadName.h"
#endif

#include "VP_VoicePlayControl.h"
#include "VP_DataManager.h"
#include "CL_SyncObj.h"

using namespace std;
using namespace ::navi::VoicePlay;
const string MODULE_UI = "UI";
const string MODULE_SETTINGUI = "SettingUI";
const string MODULE_DEBUG = "Debug";

VP_VoicePlayControl::VP_VoicePlayControl(shared_ptr<VP_DataManager> pDataMgr,
                                         shared_ptr<VP_SoundPlayer> pSoundPlayer)
    : m_pDataMgr(pDataMgr)
    , m_pSoundPlayer(pSoundPlayer)
    , m_quited(CL_FALSE)
    , m_mute(CL_FALSE)
    , m_bTtsStatus(false)
{
}

VP_VoicePlayControl::~VP_VoicePlayControl()
{
    VP_LOGI("~VP_VoicePlayControl");
}

CL_ERROR VP_VoicePlayControl::Initialize(shared_ptr<VP_ChannelManager> channelMgr)
{
    // Check dependend objects
    if (nullptr == m_pDataMgr || nullptr == m_pSoundPlayer) {
        VP_DEBUG_ERROR("Initialize failed!");
        return APL_ERR_VP_INVALID_OP;
    }

    m_pSoundPlayer->Init();

    if (nullptr != channelMgr) {
        m_channelMgr = channelMgr;
#ifdef  HAVE_NUTSHELL_OS
        m_channelMgr->registerChannel(nutshell::SUBAUDIO_ROUTEGUIDANCE.getString());
        m_channelMgr->registerChannel(nutshell::SUBAUDIO_NAVNOTUFICATION.getString());
        m_channelMgr->registerChannel(nutshell::SUBAUDIO_TRAFFICINFORMATIONGUIDANCEMODE.getString());
        m_channelMgr->registerChannel(nutshell::SUBAUDIO_TRAFFICGUIDANCEMODE.getString());
#else
#define USELESS_CHANNEL     "Useless Channel"
        m_channelMgr->registerChannel(USELESS_CHANNEL);
#endif
    }

#ifndef VP_UNIT_TEST
    RegisterName(const_cast<TCHAR*>(VP_VOICEPLAYMAIN_THREAD));
    StartRegistThread();
#endif

    return APL_ERR_NO_ERROR;
}

// used for event generated in event loop
CL_ERROR VP_VoicePlayControl::PostEvent(shared_ptr<VP_Event> event)
{
    if (nullptr == event) {
        return APL_ERR_VP_INVALID_PARAM;
    }

    if (!m_quited) {
        m_eventQueue.EnQueue(event);
    }

    return APL_ERR_NO_ERROR;
}

CL_ERROR VP_VoicePlayControl::Quit()
{
    m_quited = CL_TRUE;     // does not accept events from outside
    StopAll();

    m_eventQueue.EnQueue(nullptr);      // notify queue

    if (nullptr != m_pSoundPlayer) {
        m_pSoundPlayer->StopPlayerThread();
    }
    VP_LOGI("VP_VoicePlayControl Terminate\n");

    return APL_ERR_NO_ERROR;
}

void VP_VoicePlayControl::StopAll()
{
    if (nullptr == m_channelMgr) {
        VP_LOGD("Because we initialized failed, so just return from StopAll!");
        return;
    }

    VoiceList<shared_ptr<VP_Channel>>::type channel_list = m_channelMgr->getChannels();

    auto itr = channel_list.begin();
    for (; itr != channel_list.end(); ++itr) {
        VP_CHANNEL_STATUS channel_status = (*itr)->getChannelStatus();
        (*itr)->setWaitPlayList(nullptr);
        if (CHANNEL_STATUS_OPENED == channel_status || CHANNEL_STATUS_OPENING == channel_status 
            || CHANNEL_STATUS_WAITING == channel_status) {
            (*itr)->close();
        }
    }

    VP_PLAYER_STATUS player_status = m_pSoundPlayer->getPlayerStatus();
    if (PLAYER_STATUS_PLAYING == player_status) {
        m_pSoundPlayer->Stop(); // stop the current playing list
    }
}

void VP_VoicePlayControl::Run()
{
    VP_LOGI("VP_VoicePlayControl Run enter\n");

    shared_ptr<VP_Event> event = nullptr;
#ifndef VP_UNIT_TEST
    do {
        if (CL_TRUE == m_quited) {
            break;
        }

        event = m_eventQueue.DeQueue();
        if (nullptr == event) {
            continue;
        }

        VP_LOGI("VP_VoicePlayControl thread dispatches events...\n");
        shared_ptr<VP_Event> next_event = event->DoAction();

        if (nullptr != next_event) {
            m_eventQueue.EnQueue(next_event);
        }
    } while (true);
#endif

    VP_LOGI("VP_VoicePlayControl Run exit\n");
}

shared_ptr<VP_Channel> VP_VoicePlayControl::getChannel(const string& moduleName, VP_SMALL_SIGNAL_ID ssid) const
{
    if (VP_SS_DEFAULT_ID == ssid) {
        return getChannel(moduleName);
    }
    else {
        return getChannel(ssid);
    }
}

// deprecated
shared_ptr<VP_Channel> VP_VoicePlayControl::getChannel(const string& moduleName) const
{
    if (m_channelMgr == nullptr) {
        return nullptr;
    }

    if (moduleName.empty()) {
        VP_DEBUG_ERROR("empty name of sender, can't fetch channel!");
        return nullptr;
    }
#ifdef HAVE_NUTSHELL_OS
    else if (moduleName == EV_MODULE_POINT) {
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_NAVNOTUFICATION.getString());
    }
    else if (moduleName == EV_MODULE_EFFECT) {
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_NAVNOTUFICATION.getString());
    }
    else if (moduleName == EV_MODULE_GUIDE || moduleName == EV_MODULE_PATH || moduleName == MODULE_SETTINGUI) {
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_ROUTEGUIDANCE.getString());
    }
    else if (moduleName == EV_MODULE_TRAFFIC) {
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_TRAFFICINFORMATIONGUIDANCEMODE.getString());
    }
    else if (moduleName == MODULE_UI) {
        // used for FC to supply an interface for UI
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_ROUTEGUIDANCE.getString());
    }
    else if (moduleName == MODULE_DEBUG) {
        // used for Debug
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_ROUTEGUIDANCE.getString());
    }
    else {
        return nullptr; 
    }
#else
    else {
        return m_channelMgr->getChannel(USELESS_CHANNEL);
    }
#endif
}

shared_ptr<VP_Channel> VP_VoicePlayControl::getChannel(VP_SMALL_SIGNAL_ID ssid) const
{
    if (VP_SS_DEFAULT_ID == ssid) {
        return nullptr;
    }
#ifdef  HAVE_NUTSHELL_OS
    else if (VP_SS_ROUTE_GUIDANCE == ssid || VP_SS_SETTING_UI == ssid) {
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_ROUTEGUIDANCE.getString());
    }
    else if (VP_SS_TRAFFIC_INFORMATION_GUIDANCE == ssid) {
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_TRAFFICINFORMATIONGUIDANCEMODE.getString());
    }
    else if (VP_SS_TRAFFIC_GUIDANCE == ssid) {
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_TRAFFICGUIDANCEMODE.getString());
    }
    else if (VP_SS_NAVI_NOTIFICATION == ssid) {
        return m_channelMgr->getChannel(nutshell::SUBAUDIO_NAVNOTUFICATION.getString());
    }
    else {
        return nullptr;
    }
#else
    else {
        return m_channelMgr->getChannel(USELESS_CHANNEL);
    }
#endif
}

shared_ptr<VP_ChannelManager> VP_VoicePlayControl::getChannelManager() const
{
    return m_channelMgr;
}

shared_ptr<VP_SoundPlayer> VP_VoicePlayControl::getSoundPlayer() const
{
    return m_pSoundPlayer;
}

void VP_VoicePlayControl::setMute(CL_BOOL isMute)
{
    m_mute = isMute;
}

bool VP_VoicePlayControl::isMute() const
{
    return m_mute;
}

void VP_VoicePlayControl::setTtsServiceStatus(CL_BOOL status)
{
    m_bTtsStatus = status;
}

CL_BOOL VP_VoicePlayControl::getTtsServiceStatus() const
{
    return m_bTtsStatus;
}

void VP_VoicePlayControl::sendMessage(EV_EVENT_ID msgId, EV_MSG_PTR msg)
{
    if (nullptr != msg) {
        EV_EventSender cSender;
        cSender.SendEvent(msgId, msg);
    }
    else {
        VP_DEBUG_ERROR("msg is null\n");
    }
}

void VP_VoicePlayControl::postResult(string sender, int userdata, VP_PLAY_RESULT playResult)
{
    VpOnPlayEnd* m = new(MEM_Voice) VpOnPlayEnd();

    VP_LOGD("Play Result is: [%d], Sender is [%s]\n", playResult, sender.c_str());
    m->set_sender(sender);
    m->set_userdata(userdata);
    m->set_result(static_cast<VpOnPlayEnd_VP_PLAY_RESULT>(playResult));

    sendMessage(EV_EVENT_VOICEPLAY_ONPLAYEND, m);
}

/* EOF */
