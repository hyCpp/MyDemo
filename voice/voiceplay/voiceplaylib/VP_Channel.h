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
 * @file VP_Channel.h                                            
 * @brief Summary for VP_Channel.
 *
 * This file includes the declaration of class VP_Channel and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_CHANNEL_H
#define VP_CHANNEL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#pragma once

#include <time.h>
#include <memory>
#include <string>
#include "VP_DataAccessorList.h"

#ifdef HAVE_NUTSHELL_OS
#include "NCSubAudioManager.h"
typedef     nutshell::NCSubAudioManager AUDIO_MANAGER;
#else
#include "VP_NativeAudioManager.h"
class VP_NativeAudioManager;

typedef     VP_NativeAudioManager       AUDIO_MANAGER;
#endif

#include "VP_Define.h"
#include "VP_SoundPlayer.h"

class VP_SoundPlayer;

/**
 * class used to keep the status of a simulated channel 
 *
 * This class is a class providing a way
 * to keep the status of a channel 
 */
class VP_Channel 
{
public:
    VP_Channel(const std::string& channelName, 
            std::shared_ptr<AUDIO_MANAGER> audioMgr);

    void open();
    void close();
    void setChannelStatus(VP_CHANNEL_STATUS status);
    VP_CHANNEL_STATUS getChannelStatus() const;
    std::string getChannelName() const;
    void setWaitPlayList(std::shared_ptr<VP_DataAccessorList> waitPlayList);
    std::shared_ptr<VP_DataAccessorList> getWaitPlayList() const;
    void queryAudioStatus();
    void confirmAudioStatus(const std::string &currentSource, bool reqAudio);
    void incTimeout();
    void resetTimeout();

private:
    void reqAudioSource(bool on);
    void resetChannelStatus();

private:
    std::string         m_selfName;
    std::shared_ptr<AUDIO_MANAGER>   m_audioMgr;
    VP_CHANNEL_STATUS               m_selfStatus;
    std::shared_ptr<VP_DataAccessorList> m_waitPlayList;
};

#endif
/* EOF */
