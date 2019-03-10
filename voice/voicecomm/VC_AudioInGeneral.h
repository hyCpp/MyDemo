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
 * @file VC_AudioInGeneral.h
 * @brief Declaration file of class VC_AudioInGeneral.
 *
 * This file includes the declaration of class VC_AudioInGeneral, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef VC_AUDIOINGENERAL
#define VC_AUDIOINGENERAL

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_BL_THREAD_H
#    include "BL_Thread.h"
#endif

#include "VC_AudioIn.h"
#include "VC_IAudioIn.h"
#include "CL_WaitObj.h"

/**
 *
 * VC_AudioInGeneral for recording
 * 
 */

/**
 * Summary for class VC_AudioInGeneral
 * 
 * This is a detail description for class VC_AudioInGeneral
 *
 */
class  VC_AudioInGeneral: protected BL_Thread, public VC_AudioIn
{
public:
    explicit VC_AudioInGeneral(bool mock = false);
    virtual ~VC_AudioInGeneral();
    virtual bool Open(INT* sampleRate, INT* fragSize, INT* fragCount, int mode);
    
    virtual bool Start(Listener* pListener);
    virtual bool Stop(bool async);
    virtual bool Close();

protected:
    virtual void Run();
    void DoTask();

private:
    enum State { ST_NONE, ST_IDLE, ST_RUNNING };

    VC_AudioInGeneral(const VC_AudioInGeneral&);
    VC_AudioInGeneral& operator = (const VC_AudioInGeneral&);
    void SyncWait();
    
    bool m_bMock;
    Listener* m_pListener; // Not owned
    VC_IAudioIn* m_pAudioIn;
    State   m_state;
    volatile bool m_bStop;
    BYTE*   m_pBuffer;
    CL_WaitObj m_waitStop;
    int m_nAudioMode;
    bool m_bAsync;
};

#endif /* VC_AUDIOINGENERAL */
/* EOF */
