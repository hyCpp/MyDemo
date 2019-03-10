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
 * @file VC_AudioOutGeneral.h
 * @brief Declaration file of class VC_AudioOutGeneral.
 *
 * This file includes the declaration of class VC_AudioOutGeneral, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef VC_AUDIOOUTGENERAL_H
#define VC_AUDIOOUTGENERAL_H

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

// Forward declaration
class VC_AudioOutGeneral;
class VC_AudioBufferGeneral;

/**
 * VC_AudioOutGeneralx play
 *
 * VC_AudioOutGeneral for playing PCM
 * 
 */

/**
 * Summary for class VC_AudioOutGeneral
 * 
 * This is a detail description for class VC_AudioOutGeneral
 *
 */
class VC_AudioOutGeneral: public VC_AudioOut, protected BL_Thread
{
public:
    explicit VC_AudioOutGeneral(INT audioType);
    virtual ~VC_AudioOutGeneral();

    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount, int tokenid = 0);
    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount, int deviceid, int tokenid);
    
    virtual VC_AudioBuffer* AllocBuffer(INT nSize);
    virtual CL_BOOL Write(VC_AudioBuffer* pBuf);

    virtual CL_BOOL Start(Listener* pListener);
    virtual CL_BOOL Stop(CL_BOOL immediately, bool last_phrase = false);
    virtual CL_BOOL Close();
    virtual CL_ERROR Pause();
    virtual CL_ERROR Resume();
    virtual CL_BOOL SetVolume(INT volume);
    virtual INT     GetVolume() const;

    virtual void  SetSessionProperty(CL_BOOL on)
    {
    }

    virtual void StopOutThread();

protected:
    virtual VOID Run();
    VOID DoTask();

private:
    VOID ClearBuffers();

private:

    VC_AudioOutGeneral(const VC_AudioOutGeneral&);
    VC_AudioOutGeneral& operator = (const VC_AudioOutGeneral&);

    static const DWORD VC_AUDIO_BUF_COUNT = 16;

    enum State { ST_NONE, ST_IDLE, ST_RUNNING };

    enum StopType { STOP_NONE, STOP_NORMAL, STOP_FORCE };

    enum PlayType { PLAY_OPENDED, PLAY_WRITING, PLAY_PLAYOVER, PLAY_IDLE, PLAY_PAUSING };

    Listener* m_pListener; // Not owned
    volatile State m_state;
    volatile StopType m_stop;
    volatile PlayType m_play;
    CL_WaitObj m_waitStop;
    CL_WaitObj m_waitPause;
    VC_IAudioOut* m_pAudioOut;
    DWORD m_dwIndexW;
    DWORD m_dwIndexR;
    DWORD m_dwBufCount;
    CL_BOOL m_bIsThreadStart;
    // CL_List<VC_AudioBufferGeneral> m_buffers;
    VC_AudioBufferGeneral *m_buffers[VC_AUDIO_BUF_COUNT];
    INT m_audioType;
    INT m_tokenId;
    bool m_bLastPhrase;
};

#endif /* VC_AUDIOOUTGENERAL_H */
/* EOF */
