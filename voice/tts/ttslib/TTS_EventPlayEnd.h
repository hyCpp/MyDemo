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
 * @file TTS_EventPlayEnd.h                                       
 * @brief Declaration file of class TTS_EventPlayiEND.
 *
 * This file includes the declaration of class TTS_EventPlayEnd and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef _TTS_EVENT_PLAY_END_H_
#define _TTS_EVENT_PLAY_END_H_

#include "TTS_Event.h"

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

class TTS_EventPlayEnd : public TTS_Event
{
public:
    TTS_EventPlayEnd(TTS_PlayThread *thread, TTS_PlaylistInfo *playlist,
            TTS_PlaylistInfoManager* manager, TTS_TextToSpeechImpl::PlaybackListener *listener, TTS_PLAYER_STATE status);
    virtual ~TTS_EventPlayEnd();

    virtual void OnEvent();

protected:
    void PlayEnd();    

private:

    TTS_EventPlayEnd(const TTS_EventPlayEnd&);
    TTS_EventPlayEnd& operator = (const TTS_EventPlayEnd&);

    TTS_PlayThread	*m_thread;
    TTS_PlaylistInfo *m_playlist;
    TTS_PlaylistInfoManager *m_manager;
    TTS_TextToSpeechImpl::PlaybackListener *m_listener;
    TTS_PLAYER_STATE    m_status;
};

#endif /* _TTS_EVENT_PLAY_LIST_H_ */
/* EOF */

