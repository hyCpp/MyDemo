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
 * @file TTS_EventPlaylist.h                                       
 * @brief Declaration file of class TTS_EventPlaylist.
 *
 * This file includes the declaration of class TTS_EventPlaylist and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef _TTS_EVENT_PLAY_LIST_H_
#define _TTS_EVENT_PLAY_LIST_H_

#include "TTS_Event.h"
#include "TTS_TextToSpeechImpl.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_PlaylistInfo;
class TTS_PlayThread;
class TTS_PlaylistInfoManager;


/**
* TTS_EventPlaylist
*
* The class is just for TTS_EventPlaylist.
*/

class TTS_EventPlaylist : public TTS_Event
{
public:
    TTS_EventPlaylist(TTS_PlayThread *thread, TTS_PlaylistInfo *playlist,
            TTS_PlaylistInfoManager* manager, TTS_TextToSpeechImpl::PlaybackListener *listener);
    virtual ~TTS_EventPlaylist();

    virtual void OnEvent();

protected:
    void Play();    

private:

    TTS_EventPlaylist(const TTS_EventPlaylist&);
    TTS_EventPlaylist& operator = (const TTS_EventPlaylist&);

    TTS_PlayThread	*m_thread;
    TTS_PlaylistInfo *m_playlist;
    TTS_PlaylistInfoManager *m_manager;
    TTS_TextToSpeechImpl::PlaybackListener *m_listener;
};

#endif /* _TTS_EVENT_PLAY_LIST_H_ */
/* EOF */
