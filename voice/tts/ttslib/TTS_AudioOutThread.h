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
 * @file TTS_AudioOutThread.h
 * @brief Declaration file of class TTS_AudioOutThread.
 *
 * This file includes the declaration of class TTS_AudioOutThread, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef __TTS_AUDIOOUTTHREAD_H__
#define __TTS_AUDIOOUTTHREAD_H__

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_CL_LIST_H
#   include "CL_List.h"
#endif
#ifndef CXX_BL_THREAD_H
#    include "BL_Thread.h"
#endif
#include "VC_IAudioOut.h"
#include "VC_AudioOut.h"
#include "CL_WaitObj.h"

#include "TTS_IF.h"

class TTS_EngineBuffers;

/**
 * Summary for class TTS_AudioOutThread
 * 
 * This is a detail description for class TTS_AudioOutThread
 *
 */
class TTS_AudioOutThread : protected BL_Thread
{
public:
    TTS_AudioOutThread();
    virtual ~TTS_AudioOutThread();


    void JoinPlayer();
    virtual CL_BOOL Close();

    virtual CL_BOOL Pause()
    {
        return CL_FALSE;
    }

    virtual CL_BOOL Resume()
    {
        return CL_FALSE;
    }

    void StartAudioThread();
    virtual void StopOutThread();

    void Drain();

    void DrainNoSleep();

    void Flush();

    CL_BOOL Play(TTS_EngineBuffers* buffer, int sampleRate, int channel, TTS_STREAM_DEVICE_TYPE stream, int token, bool drainWait);

protected:
    virtual VOID Run();
    VOID DoTask();

private:

    TTS_AudioOutThread(const TTS_AudioOutThread&);
    TTS_AudioOutThread& operator = (const TTS_AudioOutThread&);

    enum State { ST_NONE, ST_IDLE, ST_RUNNING };

    enum StopType { STOP_NONE, STOP_NORMAL, STOP_FORCE };

    enum PlayType { PLAY_OPENDED, PLAY_WRITING, PLAY_PLAYOVER, PLAY_IDLE, PLAY_PAUSING };

    VC_IAudioOut* m_audioOut;
    TTS_EngineBuffers* m_buffer;

    bool        m_drainWait;
    CL_WaitObj  m_stopWait;
};

#endif /* VC_AUDIOOUTGENERAL_H */
/* EOF */

