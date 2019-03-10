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
 * @file VP_VoicePlayControl.h                                            
 * @brief Summary for VP_VoicePlayControl.
 *
 * This file includes the declaration of class VP_VoicePlayControl and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_VOICEPLAYCONTROL_H
#define VP_VOICEPLAYCONTROL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include <atomic>

#ifndef CXX_CL_LIST_H
#	include "CL_List.h"
#endif

#ifndef CXX_BL_THREAD_H
#	include "BL_Thread.h"
#endif

#include "VP_Define.h"
#include "VP_DataManager.h"
#include "VP_SoundPlayer.h"
#include "VP_Event.h"
#include "VP_BlockingQueue.hpp"
#include "VP_Channel.h"
#include "VP_DurationManager.h"

#include "VP_ChannelManager.h"

#include "navi/Voice/VoicePlay/VpOnPlayEnd.pb.h"

class VP_Event;
class VP_DataManager;
class VP_SoundPlayer;
class VP_DurationManager;
class VP_ChannelManager;

/**
 * Summary for class VP_VoicePlayControl
 * 
 * This is a detail description for class VP_VoicePlayControl
 *
 */
class VP_VoicePlayControl : protected BL_Thread
{
public:
    VP_VoicePlayControl(std::shared_ptr<VP_DataManager> pDataMgr, std::shared_ptr<VP_SoundPlayer> pSoundPlayer);
    virtual ~VP_VoicePlayControl();

    virtual CL_ERROR Initialize(std::shared_ptr<VP_ChannelManager> channelMgr);

    virtual CL_ERROR PostEvent(std::shared_ptr<VP_Event> event);

    virtual CL_ERROR Quit();

    void StopAll();

    std::shared_ptr<VP_Channel> getChannel(const string& moduleName, VP_SMALL_SIGNAL_ID ssid) const;

    std::shared_ptr<VP_Channel> getChannel(const std::string& moduleName) const;

    std::shared_ptr<VP_Channel> getChannel(VP_SMALL_SIGNAL_ID ssid) const;

    std::shared_ptr<VP_ChannelManager> getChannelManager() const;

    std::shared_ptr<VP_SoundPlayer> getSoundPlayer() const;

    void setMute(CL_BOOL isMute);

    bool isMute() const;

    void sendMessage(EV_EVENT_ID msgId, EV_MSG_PTR msg);

    void postResult(std::string sender, int userdata, VP_PLAY_RESULT playResult);

    void setTtsServiceStatus(CL_BOOL status);

    CL_BOOL getTtsServiceStatus() const;

protected:
    virtual void Run();

protected:
    std::shared_ptr<VP_DataManager>	                    m_pDataMgr;
    std::shared_ptr<VP_SoundPlayer>		                m_pSoundPlayer;
    VP_BlockingQueue<std::shared_ptr<VP_Event>>         m_eventQueue;
    std::mutex          m_mutex;
    
private:
    CL_BOOL                         m_quited;
    CL_BOOL                         m_mute;
    std::atomic<CL_BOOL>            m_bTtsStatus;
    std::shared_ptr<VP_ChannelManager>   m_channelMgr;
};

#endif /* VP_VOICEPLAYCONTROL_H */
/* EOF */
