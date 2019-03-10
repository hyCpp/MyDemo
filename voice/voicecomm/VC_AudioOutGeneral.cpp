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
#include "VC_AudioOutGeneral.h"
#include "VC_AudioBufferGeneral.h"
#include "AplThreadName.h"

#ifdef HAVE_NUTSHELL_OS
#include "NMAudioStream.h"
#else
#include "VC_AudioStream.h"
#endif

#include "VC_Factory.h"

const INT VC_RETRY_MAX = 100;
const INT VC_WAIT_TIME = 10; // wait for 20 milisecond one time

VC_AudioOutGeneral::VC_AudioOutGeneral(INT audioType)
    :m_pListener(NULL)
    , m_state(ST_NONE)
    , m_stop(STOP_NONE)
    , m_play(PLAY_IDLE)
    , m_pAudioOut(NULL)
    , m_dwIndexW(0)
    , m_dwIndexR(0)
    , m_dwBufCount(0)
    , m_bIsThreadStart(CL_FALSE)
    , m_audioType(audioType)
    , m_bLastPhrase(false)
{
    m_pAudioOut = VC_Factory::Instance()->CreateIAudioOut(static_cast<VC_AUDIO_TYPE>(m_audioType));
    for (DWORD i = 0; i < VC_AUDIO_BUF_COUNT; ++i) {
        m_buffers[i] = NULL;
    }
}

VC_AudioOutGeneral::~VC_AudioOutGeneral()
{
    ClearBuffers();
    delete m_pAudioOut;
}

CL_BOOL VC_AudioOutGeneral::Open(INT* sampleRate, INT* fragSize, INT* fragCount, int tokenid)
{
    return Open(sampleRate, fragSize, fragCount, EAUDIO_STREAM_DEVICE_OUTPUT_INTERRUPT, tokenid);
}

CL_BOOL VC_AudioOutGeneral::Open(INT* sampleRate, INT* fragSize, INT* fragCount, int deviceid, int tokenid)
{
    if (CL_FALSE == m_bIsThreadStart) {
        if (NULL == m_pAudioOut) {
            return CL_FALSE;
        }

        RegisterName(VC_AUDIOOUT_THREAD);
        StartRegistThread();
        m_bIsThreadStart = CL_TRUE;
    }

    *fragSize = 8192;
    *fragCount = 4;
    CL_BOOL ok = m_pAudioOut->Open(sampleRate, 1, deviceid, tokenid);
    if (ok) {
        m_dwIndexW = 0;
        m_dwIndexR = 0;
        m_stop = STOP_NONE;
        m_state = ST_IDLE;
        m_play = PLAY_OPENDED;
    }

    return ok;
}

VC_AudioBuffer* VC_AudioOutGeneral::AllocBuffer(INT nSize)
{
    if (ST_IDLE != m_state || VC_AUDIO_BUF_COUNT <= m_dwBufCount) {
        VC_LOGE("Invalid operation, state=%d\n", m_state);
        RETURN_NULL;
    }

    VC_AudioBufferGeneral* pBuf = VC_AudioBufferGeneral::Alloc(nSize);
    if (NULL == pBuf) {
        VC_LOGE("Alloc audio buffer failed, size=%d\n", nSize);
        RETURN_NULL;
    }

    m_buffers[m_dwBufCount++] = pBuf;
    VC_LOGD("buf size=%d", pBuf->size());

    return pBuf;
}

CL_BOOL VC_AudioOutGeneral::Write(VC_AudioBuffer* pBuf)
{
    if (NULL == pBuf) {
        VC_LOGE("Invalid param, buffer is null\n");
        return CL_FALSE;
    }

    if (STOP_NONE != m_stop) {
        return CL_FALSE;
    }

    VC_LOGD("m_dwIndexR=%d, m_dwIndexW=%d, m_dwBufCount=%d", m_dwIndexR, m_dwIndexW, m_dwBufCount);
    if (m_dwIndexR + m_dwBufCount <= m_dwIndexW) {
        VC_LOGE("Not sequence write buffer\n");
        return CL_FALSE;
    }

    ++m_dwIndexW;
    if (ST_RUNNING == m_state) {
        Notify();
    }

    return CL_TRUE;
}

CL_BOOL VC_AudioOutGeneral::Start(Listener* pListener)
{
    VC_LOGD_FUNC();
    if (ST_IDLE != m_state) {
         VC_LOGE("Invalid operation, state=%d\n", m_state);
         return CL_FALSE;
    }

    m_pListener = pListener;
    m_state = ST_RUNNING;
    Notify();
    return CL_TRUE;
}

CL_BOOL VC_AudioOutGeneral::Stop(CL_BOOL immediately, bool last_phrase)
{
    VC_LOGI("immediately=%d", immediately);
    if (ST_RUNNING != m_state) {
         VC_LOGI("Invalid operation, state=%d\n", m_state);
         return CL_FALSE;
    }

    if (STOP_FORCE == m_stop) {
        VC_LOGD("Force stop has been invoked!");
        return CL_FALSE;
    }

    m_bLastPhrase = last_phrase;
    if (immediately) {
        m_stop = STOP_FORCE;
        if (PLAY_PAUSING == m_play) {
            m_waitPause.Notify();
        }
        Notify();
        while (1) {
            if (ST_RUNNING != m_state) {
                break;
            }

            VC_LOGD("state is running need wait again");
            m_waitStop.Wait();
        }
        VC_LOGD("state is running need wait again end");
    }
    else {
        m_stop = STOP_NORMAL;
        if (PLAY_PAUSING == m_play) {
            m_waitPause.Notify();
        }
        Notify();
    }

    return CL_TRUE;
}

CL_BOOL VC_AudioOutGeneral::Close()
{
    VC_LOGD_FUNC();
    if (ST_NONE == m_state) {
        return CL_FALSE;
    }

    if (ST_RUNNING == m_state) {
        Stop(true);
    }

    CL_BOOL ok = m_pAudioOut->Close();
    if (ok) {
        m_state = ST_NONE;
        m_play = PLAY_PLAYOVER;
    }

    ClearBuffers();

    return ok;
}

VOID VC_AudioOutGeneral::Run()
{
    while (1) {
        VC_LOGI("VC_AudioOutGeneral thread waiting ...");
        Wait();
        VC_LOGI("VC_AudioOutGeneral thread wake up");
        if (TRUE == CheckQuit()) {
            VC_LOGI("VC_AudioOutGeneral: quit thread!");
            break;
        }
        DoTask();
    }
}

VOID VC_AudioOutGeneral::DoTask()
{
    if (ST_RUNNING != m_state) {
        VC_LOGI("Invalid state, state=%d", m_state);
        return;
    }

    CL_BOOL rc = CL_FALSE;
    do {

        while (STOP_NONE == m_stop && m_dwIndexR == m_dwIndexW) {
            VC_LOGI("Wait data buffer, idx(r/w)=%d/%d\n", m_dwIndexR, m_dwIndexW);
            Wait();
        }

        if (PLAY_PAUSING == m_play) {
            m_waitPause.Wait();
        }

        if (STOP_FORCE == m_stop) {
            VC_LOGD("Force stop");
            m_pAudioOut->Stop(CL_TRUE);
            break;
        }

        if (STOP_NORMAL == m_stop && m_dwIndexR == m_dwIndexW) {
            VC_LOGD("Normal stop");
            m_pAudioOut->Stop(CL_FALSE, m_bLastPhrase);
            rc = CL_TRUE;
            break;
        }

        if (0 == m_dwBufCount) {
            VC_LOGE("VC_AudioOutGeneral::DoTask, m_dwBufCount is 0");
            ++m_dwIndexR;
            if (NULL != m_pListener) {
                m_pListener->OnAudioOutDone(NULL);
            }
            continue;
        }
        VC_AudioBufferGeneral* pBuf = m_buffers[m_dwIndexR % m_dwBufCount];
        if (ST_RUNNING == m_state) {
            if (0 != pBuf->getLen()) {
                m_pAudioOut->Write(pBuf->addr(), pBuf->getLen());
            }
        }

        ++m_dwIndexR;
        if (NULL != m_pListener) {
            m_pListener->OnAudioOutDone(pBuf);
        }
    } while (1);


    VC_LOGD("Audio out complete, indexW=%d, indexR=%d\n", m_dwIndexW, m_dwIndexR);
    CL_BOOL normalEnd = (STOP_NORMAL == m_stop);
    Listener* pListener = m_pListener;
    m_state = ST_IDLE;
    m_waitStop.Notify();
    if (normalEnd && NULL != pListener) {
        pListener->OnAudioOutComplete(rc);
    }
}

CL_ERROR VC_AudioOutGeneral::Pause()
{
    VC_LOGD_FUNC();
    if (PLAY_IDLE == m_play || PLAY_PLAYOVER == m_play) {
        return VC_ERROR_FAILED;
    }

    if (PLAY_WRITING == m_play || PLAY_OPENDED == m_play) {
        m_play = PLAY_PAUSING;
        return m_pAudioOut->Pause() ? VC_ERROR_NO_ERROR : VC_ERROR_ERROR;
    }

    return VC_ERROR_ERROR;
}

CL_ERROR VC_AudioOutGeneral::Resume()
{
    VC_LOGD_FUNC();

    if (PLAY_PAUSING == m_play) {
        m_play = PLAY_WRITING;
        if (!m_pAudioOut->Resume()) {    // 进行返回值的判断
            return VC_ERROR_ERROR;
        }
        m_waitPause.Notify();
        return VC_ERROR_NO_ERROR;
    }
    return VC_ERROR_FAILED;
}

CL_BOOL VC_AudioOutGeneral::SetVolume(INT volume)
{
    RETURN(false);
}

INT VC_AudioOutGeneral::GetVolume() const
{
    return 0;
}

VOID VC_AudioOutGeneral::ClearBuffers()
{
    for (DWORD i = 0; i < m_dwBufCount; ++i) {
        delete m_buffers[i];
        m_buffers[i] = NULL;
    }
    m_dwBufCount = 0;
}

void VC_AudioOutGeneral::StopOutThread()
{
    VC_LOGI("StopOutThread");
    StopThread();
}

/* EOF */
