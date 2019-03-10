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
 * @file TTS_PlaylistInfo.h
 * @brief Declaration file of class TTS_PlaylistInfo.
 *
 * This file includes the declaration of class TTS_PlaylistInfo and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef __CPP_TTS_TTSPLAYLISTINFOMANAGER_H__
#define __CPP_TTS_TTSPLAYLISTINFOMANAGER_H__


#include "MEM_list.h"
class TTS_PlaylistInfo;

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* TTS_PlaylistInfoManager
*
* The class is to manager the play list. 
*/
class TTS_PlaylistInfoManager
{
    public:
        TTS_PlaylistInfoManager() : m_currPlayList(NULL)
        {
        }

        ~TTS_PlaylistInfoManager();

        void AddPlayList(TTS_PlaylistInfo* playlist);

        void AddPauseList(TTS_PlaylistInfo* playlist);

        bool CanPlayNow() const; 

        TTS_PlaylistInfo* GetCurrPlaylist();

        void SetCurrPlaylist(TTS_PlaylistInfo* playlist);

        TTS_PlaylistInfo* GetFirstPlayList();

        TTS_PlaylistInfo* TakePlayList(const BL_String& sender, const int reqId);

        TTS_PlaylistInfo* TakePauseList(const BL_String& sender, const int reqId);

        TTS_PlaylistInfo* TakePlayOrPauseList(const BL_String& sender, const int reqId);

        bool RemovePlayAndPause(const BL_String& sender, const int reqId);

        bool RemovePlayAndPause(const BL_String& sender);

        bool RemovePlayList(const BL_String& sender, const int reqId);

        bool RemovePlayList(const BL_String& sender);

        bool RemovePauseList(const BL_String& sender, const int reqId);

        bool RemovePauseList(const BL_String& sender);

        void ClearList(VoiceList<TTS_PlaylistInfo*>::type& playList);

    private:

        bool Remove(VoiceList<TTS_PlaylistInfo*>::type& playlist, const BL_String& sender, const int reqId);

        bool Remove(VoiceList<TTS_PlaylistInfo*>::type& playlist, const BL_String& sender);

        TTS_PlaylistInfo* Take(VoiceList<TTS_PlaylistInfo*>::type& playlist, const BL_String& sender, const int reqId);

        TTS_PlaylistInfo* Take(VoiceList<TTS_PlaylistInfo*>::type& playlist, const BL_String& sender);

        TTS_PlaylistInfoManager(const TTS_PlaylistInfoManager&);
        TTS_PlaylistInfoManager& operator = (const TTS_PlaylistInfoManager&);
    
    private:
        VoiceList<TTS_PlaylistInfo*>::type m_playList;
        VoiceList<TTS_PlaylistInfo*>::type m_pauseList;
        TTS_PlaylistInfo*                  m_currPlayList;
};

#endif
/* EOF */

