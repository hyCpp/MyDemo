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
 * @file TTS_PlayThread.h
 * @brief Declaration file of class TTS_PlayThread.                  
 *
 * This file includes the declaration of class TTS_PlayThread and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef CXX_TTS_PLAYTHREAD_H
#define CXX_TTS_PLAYTHREAD_H

#include "CL_SyncObj.h"
#include "CL_WaitObj.h"
#include "TTS_Thread.h"
#include "VC_AudioOut.h"
#include "TTS_IF.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_PlayThread.h"
#include "TTS_AudioOutThread.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_EngineManager;
class TTS_PlayElementBase;
class TTS_Engine;
class TTS_TextToSpeechProperty;
class TTS_PlayResultBase;
class VC_AudioSession;

enum TTS_STOP_TYPE
{
    TTS_STOP_NO_STOP,
    TTS_STOP_PAUSE,
    TTS_STOP_ABORT,
    TTS_STOP_VRMODE
};

/**
* TTS_PlayThread
*
* The class is just for TTS_PlayThread.
*/

class TTS_PlayThread : public BL_Thread
{
public:
    TTS_PlayThread();
    virtual ~TTS_PlayThread();
    
    virtual void StopPlay(TTS_PLAYER_STATE stopType);

    void Start();
    void Stop();
    void StopPlayThread();
    void PlayReq(TTS_PlaylistInfo* playlist);
    void Pause(TTS_PlaylistInfo *playlist);
    void Resume(TTS_PlaylistInfo *playlist);

protected:
    virtual void Run();

private:
    void SendPlayResult(TTS_PlaylistInfo *playlist, bool state);
    void SendPlayResult(TTS_PlaylistInfo *playlist, TTS_PLAYER_STATE state);
    void SendPlayState(TTS_PlaylistInfo *playlist, TTS_PLAYER_STATE state, int playNum = 0);
    
    void Play();

private:

    TTS_PlayThread(const TTS_PlayThread&);
    TTS_PlayThread& operator = (const TTS_PlayThread&);

    enum TTS_STATE { ST_IDLE, ST_PLAYING, ST_STOPPING };

    CL_SyncObj				    m_lock;
    CL_WaitObj					m_stopWait;
    bool                        m_stop;
    TTS_PLAYER_STATE            m_stopType;

    TTS_PlaylistInfo            *m_playlist;
    TTS_EngineBuffers            m_buffer;
    TTS_AudioOutThread          m_audioThread;
};


#endif
/* EOF */
