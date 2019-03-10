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
#include "TTS_PlayThread.h"
#include "TTS_Controler.h"
#include "TTS_Common.h"
#include "AplThreadName.h"
#include "TTS_EngineManager.h"
#include "TTS_PlaylistInfo.h"
#include "VC_CommonIF.h"
#include "TTS_EventStopReq.h" 
// #include "TTS_TuningManager.h"
#include "TTS_PlaylistInfoManager.h"

TTS_PlaylistInfoManager::~TTS_PlaylistInfoManager()
{
    ClearList(m_playList);
    ClearList(m_pauseList);
}

void TTS_PlaylistInfoManager::AddPlayList(TTS_PlaylistInfo* playlist)
{
    m_playList.push_back(playlist);
}


void TTS_PlaylistInfoManager::AddPauseList(TTS_PlaylistInfo* playlist)
{
    m_pauseList.push_back(playlist);
}

bool TTS_PlaylistInfoManager::CanPlayNow() const
{
    return NULL == m_currPlayList;
}

TTS_PlaylistInfo* TTS_PlaylistInfoManager::GetCurrPlaylist()
{
    return m_currPlayList;
}

void TTS_PlaylistInfoManager::SetCurrPlaylist(TTS_PlaylistInfo* playlist)
{
    m_currPlayList = playlist;
}

TTS_PlaylistInfo* TTS_PlaylistInfoManager::GetFirstPlayList()
{
    if (m_playList.empty()) {
        return NULL;
    }
            
    TTS_PlaylistInfo* pFirst = m_playList.front();
    m_playList.pop_front();
    return pFirst;
}

TTS_PlaylistInfo* TTS_PlaylistInfoManager::TakePlayList(const BL_String& sender, const int reqId)
{
    return Take(m_playList, sender, reqId);
}

TTS_PlaylistInfo* TTS_PlaylistInfoManager::TakePauseList(const BL_String& sender, const int reqId)
{
    return Take(m_pauseList, sender, reqId);
}

TTS_PlaylistInfo* TTS_PlaylistInfoManager::TakePlayOrPauseList(const BL_String& sender, const int reqId)
{
    TTS_PlaylistInfo* playlist = TakePlayList(sender, reqId);
    if (NULL == playlist) {
        return TakePauseList(sender, reqId);
    }

    return playlist;
}

bool TTS_PlaylistInfoManager::RemovePlayAndPause(const BL_String& sender, const int reqId)
{
    return (RemovePlayList(sender, reqId) || RemovePauseList(sender, reqId)); 
}

bool TTS_PlaylistInfoManager::RemovePlayAndPause(const BL_String& sender)
{
    return (RemovePlayList(sender) || RemovePauseList(sender));
}


bool TTS_PlaylistInfoManager::RemovePlayList(const BL_String& sender, const int reqId)
{
    return  Remove(m_playList, sender, reqId);
}

bool TTS_PlaylistInfoManager::RemovePlayList(const BL_String& sender)
{
    return Remove(m_playList, sender);
}

bool TTS_PlaylistInfoManager::RemovePauseList(const BL_String& sender, const int reqId)
{
    return  Remove(m_pauseList, sender, reqId);
}

bool TTS_PlaylistInfoManager::RemovePauseList(const BL_String& sender)
{
    return Remove(m_pauseList, sender);
}

void TTS_PlaylistInfoManager::ClearList(VoiceList<TTS_PlaylistInfo*>::type& playList)
{
    VoiceList<TTS_PlaylistInfo*>::iterator it = playList.begin();
    for (; it != playList.end(); ++it) {
        if (*it != NULL) {
            delete *it;
        }
    }

    playList.clear();
}

/*
 * class CompareWiteName  
*/

class CompareWithName
{
    public:

        explicit CompareWithName(const BL_String& sender) : m_sender(sender)
        {
        }

        bool operator()(const TTS_PlaylistInfo* playlist) const 
        {
            return playlist->GetSender() == m_sender;
        }

    private:
        BL_String m_sender;
};

/*
 * class CompareWithId
*/
class CompareWithId
{
    public:

        CompareWithId(const BL_String& sender, const int reqId) : m_sender(sender), m_reqId(reqId)
        {
        }

        bool operator()(const TTS_PlaylistInfo* playlist) const 
        {
            TTS_LOGI("%s , %d, %lld", m_sender.GetString(), m_reqId, playlist);
            return ((playlist->GetSender() == m_sender) && (playlist->GetRequestId() == m_reqId));
        }

    private:
        BL_String m_sender;
        int m_reqId;
};


bool TTS_PlaylistInfoManager::Remove(VoiceList<TTS_PlaylistInfo*>::type& playlist, const BL_String& sender, const int reqId)
{
    VoiceList<TTS_PlaylistInfo*>::type::size_type size = playlist.size();
    playlist.remove_if(CompareWithId(sender, reqId));
    return size != playlist.size();
}

bool TTS_PlaylistInfoManager::Remove(VoiceList<TTS_PlaylistInfo*>::type& playlist, const BL_String& sender)
{
    VoiceList<TTS_PlaylistInfo*>::type::size_type size = playlist.size();
    playlist.remove_if(CompareWithName(sender));
    return size != playlist.size();
}

TTS_PlaylistInfo* TTS_PlaylistInfoManager::Take(VoiceList<TTS_PlaylistInfo*>::type& playlist, const BL_String& sender, const int reqId)
{
    TTS_LOGI("Take send=%s,id=%d", sender.GetString(), reqId);
    VoiceList<TTS_PlaylistInfo*>::iterator it = std::find_if(playlist.begin(), playlist.end(), CompareWithId(sender, reqId));

    if (it == playlist.end()) {
        return NULL;
    }
    else {
        TTS_PlaylistInfo* p = *it;
        playlist.erase(it);
        return p;
    }
}

#if 0
TTS_PlaylistInfo* Take(VoiceList<TTS_PlaylistInfo*>::type& playlist, const BL_String& sender)
{
    VoiceList<TTS_PlaylistInfo*>::iterator it = std::find_if(playlist.begin(), playlist.end(), CompareWithName(sender));

    if (it == playlist.end()) {
        return NULL;
    }
    else {
        TTS_PlaylistInfo* p = *it;
        playlist.erase(it);
        return p;
    }
}

#endif

/* EOF */

