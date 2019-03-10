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
 * @file VP_Event.h                                            
 * @brief Summary for VP_Event.
 *
 * This file includes the declaration of class VP_Event and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_EVENT_H
#define VP_EVENT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include <string>
#include <memory>

#include "MEM_list.h"
#include "MEM_set.h"
#include "VP_Define.h"
#include "VP_DataAccessorList.h"
#include "VP_SoundPlayer.h"
#include "VP_Channel.h"
#include "VP_PhraseImp.h"
#include "VP_Request.h"
#include "navi/Voice/VoicePlay/VpDuration.pb.h"

class VP_SoundPlayer;
class VP_Channel;

/**
 * This is an interface for all events 
 *
 * This class defines the interface function and shared variables
 *
 */
class VP_Event
{
public:
    VP_Event() : m_channel(nullptr)
    {
    }

    explicit VP_Event(const std::shared_ptr<VP_Channel>& channel) : m_channel(channel)
    {
    }

    virtual ~VP_Event() 
    {
    }

    virtual std::shared_ptr<VP_Event> DoAction() = 0;

protected:
    std::shared_ptr<VP_Channel> m_channel;
};

/**
 * class used to play the playlist 
 *
 * This class is a class providing a way
 * to play playlist
 */
class VP_PlayListEvent : public VP_Event 
{
public:
    VP_PlayListEvent();
    
    explicit VP_PlayListEvent(VoiceList<std::shared_ptr<VP_PhraseImp>>::type &pList,
            VP_SMALL_SIGNAL_ID ssid, std::string sdr, int usrdata)
        : VP_Event(nullptr)
        , m_ssid(ssid)
        , sender(sdr)
        , userdata(usrdata)
        , m_bNotify(true)
        , m_phraseList(pList)
    {
    }

    std::shared_ptr<VP_Event> DoAction();

    void setPlayEndNotify(bool notifyPlayEnd);

    bool getPlayEndNotify() const;

private:
    bool WrapDataAccessor(VoiceVector<std::shared_ptr<VP_DataAccessor>>::type &pList);
    CL_ERROR SetPhraseInfos();

private:
    VP_SMALL_SIGNAL_ID m_ssid;
    std::string sender;
    int userdata;
    bool m_bNotify;
    VoiceList<std::shared_ptr<VP_PhraseImp>>::type m_phraseList;
};

/**
 * class used to get duration of a phrase list
 *
 * This class is a class providing a way
 * to get duration of a phrase list
 */
class VP_GetDurationEvent : public VP_Event 
{
public:
    VP_GetDurationEvent();
    
    explicit VP_GetDurationEvent(VoiceList<std::shared_ptr<VP_PhraseImp>>::type &pList, 
        std::string sdr, int ud)
        : VP_Event(nullptr)
        , m_phraseList(pList)
        , sender(sdr)
        , userdata(ud)
    {
    }

    std::shared_ptr<VP_Event> DoAction();

private:
    int GetDuration(); 

private:
    const static int PHRASE_RESERVE_JIFFY = 50;
    const static int VP_RESERVE_JIFFY = 550;

    VoiceList<std::shared_ptr<VP_PhraseImp>>::type m_phraseList;
    std::string sender;
    int userdata;
};

/**
 * class used to confirm whether player has stopped the playlist 
 *
 * This class is a class providing a way
 * to confirm whether current playlist has been stopped 
 */
class VP_PlayerStoppedEvent : public VP_Event 
{
public:
    VP_PlayerStoppedEvent();
    
    explicit VP_PlayerStoppedEvent(const std::shared_ptr<VP_Channel>& channel) : VP_Event(channel) 
    {
    }

    std::shared_ptr<VP_Event> DoAction();
};

/**
 * class used to notify player to stop the current playlist
 *
 * This class is a class providing a way
 * to stop the current playlist 
 */
class VP_PlayerStopEvent : public VP_Event 
{
public:
    VP_PlayerStopEvent();

    explicit VP_PlayerStopEvent(const std::shared_ptr<VP_Channel>& channel) : VP_Event(channel) 
    {
    }

    std::shared_ptr<VP_Event> DoAction();
};

/**
 * class used to send a rquest to open channel 
 *
 * This class is a class providing a way
 * to open channel 
 */
class VP_ChannelOpenEvent : public VP_Event 
{
public:
    explicit VP_ChannelOpenEvent(const std::shared_ptr<VP_Channel>& channel,
                    const std::shared_ptr<VP_DataAccessorList>& pAccessorList)
        : VP_Event(channel)
        , m_pList(pAccessorList)
    {
    }

    std::shared_ptr<VP_Event> DoAction();

private:
    std::shared_ptr<VP_DataAccessorList> m_pList;
};

/**
 * class used to confirm whether channel has been opened
 *
 * This class is a class providing a way
 * to check whether applied channel has benn opened 
 */
class VP_ChannelOpenedEvent : public VP_Event 
{
public:
    explicit VP_ChannelOpenedEvent(const std::shared_ptr<VP_Channel>& channel)
        : VP_Event(channel)
    {
    }

    std::shared_ptr<VP_Event> DoAction();

private:
    void resumePlay();
};

/**
 * class used to confirm whether current channel has been closed 
 *
 * This class is a class providing a way
 * to check whether current channel has been closed 
 */
class VP_ChannelClosedEvent : public VP_Event 
{
public:
    explicit VP_ChannelClosedEvent(const std::shared_ptr<VP_Channel>& channel) : VP_Event(channel) 
    {
    }

    std::shared_ptr<VP_Event> DoAction();
};

/**
 * class used to stop all threads when quiting the module 
 *
 * This class is a class providing a way
 * clean all variables when quiting the module
 */
class VP_ChannelStopEvent : public VP_Event
{
public:
    explicit VP_ChannelStopEvent(const std::shared_ptr<VP_Channel>& channel) : VP_Event(channel)
    {
    }

    std::shared_ptr<VP_Event> DoAction();
};

/**
 * class used to stop all threads when quiting the module 
 *
 * This class is a class providing a way
 * clean all variables when quiting the module
 */
class VP_ChannelWaitingEvent : public VP_Event
{
public:
    explicit VP_ChannelWaitingEvent(const std::shared_ptr<VP_Channel>& channel) : VP_Event(channel)
    {
    }

    std::shared_ptr<VP_Event> DoAction();
};

/**
 * class used to stop all sounds playing and set all channels to mute state 
 *
 * This class is a class providing a way
 * to stop all sounds playing 
 */
class VP_SetMuteEvent : public VP_Event
{
public:
    explicit VP_SetMuteEvent() : VP_Event(nullptr)
    {
    }

    std::shared_ptr<VP_Event> DoAction();
};

/**
 * class used to adjust the incorrect status of channel
 *
 * This class is a class providing a way
 * to adjust the status of channel
 */
class VP_ChannelConfirmedEvent : public VP_Event
{
public:
    explicit VP_ChannelConfirmedEvent(const std::string& sourceName,
        const std::string& currentSource, bool reqAudio)
        : VP_Event(nullptr)
        , m_sourceName(sourceName)
        , m_currentSource(currentSource)
        , m_reqAudio(reqAudio)
    {
    }

    std::shared_ptr<VP_Event> DoAction();

private:
    std::string m_sourceName;
    std::string m_currentSource;
    bool        m_reqAudio;
};

/**
 * class used to query the status of channel
 *
 * This class is a class providing a way
 * to query the status of channel
 */
class VP_ChannelStatusQueryEvent : public VP_Event
{
public:
    explicit VP_ChannelStatusQueryEvent(const std::string& currentSource)
        : VP_Event(nullptr)
        , m_currentSource(currentSource)
    {
    }

    std::shared_ptr<VP_Event> DoAction();

private:
    std::string m_currentSource;
};

/**
 * class used to stop all threads when quiting the module 
 *
 * This class is a class providing a way
 * clean all variables when quiting the module
 */
class VP_QuitEvent : public VP_Event 
{
public:
    std::shared_ptr<VP_Event> DoAction();
};

/**
 * class used to reload sound file
 *
 * This class is a class providing a way
 * to reload sound file
 */
class VP_LanguageChangeEvent : public VP_Event 
{
public:
    std::shared_ptr<VP_Event> DoAction();
};

#endif /* VP_EVENT_H */
/* EOF */
