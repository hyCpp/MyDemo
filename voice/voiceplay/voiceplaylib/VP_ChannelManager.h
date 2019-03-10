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
 * @file VP_ChannelManager.h                                            
 * @brief Summary for VP_ChannelManager.
 *
 * This file includes the declaration of class VP_ChannelManager and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_CHANNEL_MANAGER_H
#define VP_CHANNEL_MANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#pragma once

#include <memory>
#include <string>

#include "MEM_list.h"
#include "MEM_map.h"
#include "MEM_set.h"

#ifdef HAVE_NUTSHELL_OS

#include "NCSubAudioListener.h"
#include "NCSubAudioManager.h"
#include "NCString.h"
#include "NCTypesDefine.h"

#endif

#include "VP_SoundPlayer.h"
#include "VP_Channel.h"
#include "VP_Factory.h"
#include "VP_VoicePlayControl.h"

class VP_VoicePlayControl;

#ifdef HAVE_NUTSHELL_OS

/**
 * This class is used to listen the status of channel
 *
 * When sound player request an audio source, AvManager will
 * notify sound player by calling this listener
 */
class VP_AudioManagerListener : public nutshell::NCSubAudioListener 
{
public:
    explicit VP_AudioManagerListener(std::shared_ptr<nutshell::NCSubAudioManager> audioMgr);

    ~VP_AudioManagerListener() {}
    
    virtual VOID onConnected();
    // notify current audio source
    virtual VOID currentAudioSource(const nutshell::NCString &recvAudio,
        const nutshell::NCString &audioName, nutshell::NC_BOOL reqAudio);
    // callback after reqAudioSource
    virtual VOID notifyStatusChanged(const nutshell::NCString &audioName, const nutshell::NCString &status);
    // ask by AvManager
    virtual VOID askAudioStatus(const nutshell::NCString &audioName);
    // message sent to me when AMP was disconnected
    virtual VOID notifyAMPStsChanged(const nutshell::NCString& audioName, nutshell::NC_BOOL connect);

private:
    void closeChannel(std::shared_ptr<VP_Channel> channel);
    void openChannel(std::shared_ptr<VP_Channel> channel);
    void blockChannel(std::shared_ptr<VP_Channel> channel);
    void resumeChannel(std::shared_ptr<VP_Channel> channel); 
    void stopChannel(std::shared_ptr<VP_Channel> channel);

private:
    std::shared_ptr<AUDIO_MANAGER>       m_audioMgr; 
    std::shared_ptr<VP_VoicePlayControl> m_pControl;
};

#else

/**
 * This class is used to listen the status of channel
 *
 * When sound player request an audio source, AvManager will
 * notify sound player by calling this listener
 */
class VP_AudioManagerListener
{
public:
    VP_AudioManagerListener(std::shared_ptr<AUDIO_MANAGER> audioMgr);

    ~VP_AudioManagerListener() {}

    void notifyStatusChanged(const std::string& audioName, const std::string& status);

private:
    std::shared_ptr<AUDIO_MANAGER>      m_audioMgr;
    std::shared_ptr<VP_VoicePlayControl> m_pControl;
};


#endif

/**
 * This class is used to manager the channel using by player
 *
 * This manager is used to register/unregister channel applying to 
 * avm
 */
class VP_ChannelManager
{
public:
    explicit VP_ChannelManager(std::shared_ptr<AUDIO_MANAGER> audioMgr = nullptr);
    virtual ~VP_ChannelManager();

    virtual bool registerChannel(const std::string& channelName);
    virtual bool unregisterChannel(const std::string& channelName);
    void unregisterChannels();
    virtual std::shared_ptr<VP_Channel> getChannel(const std::string& channelName) const;
    const VoiceList<std::shared_ptr<VP_Channel>>::type & getChannels() const;
    virtual void queryAudioStatus(const std::string& currentSource);
    virtual void confirmAudioStatus(const std::string &sourceName, const std::string &currentSource, bool reqAudio);

private:
    VP_ChannelManager(const VP_ChannelManager&);
    VP_ChannelManager& operator=(const VP_ChannelManager&);

private:
    VP_AudioManagerListener*             m_listener;
    VoiceMap<std::string, std::shared_ptr<VP_Channel>>::type m_channels_table;
    VoiceList<std::shared_ptr<VP_Channel>>::type             m_channels_list;
    std::shared_ptr<AUDIO_MANAGER>       m_audioMgr;
    std::shared_ptr<VP_SoundPlayer>      m_pSoundPlayer;
};

#endif
/* EOF */
