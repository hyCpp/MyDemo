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
#include "VC_Log.h"
#include "VC_CommonDef.h"
#include "VC_AudioInGeneral.h"
#include "AplThreadName.h"
#include "VC_Factory.h"

#define VC_AUDIOIN_BARGEIN_PARAM 0

static const int VC_AUDIOIN_BUF_LEN = 1280; // about 40ms

VC_AudioInGeneral::VC_AudioInGeneral(bool mock) : m_bMock(mock), m_pListener(NULL), m_pAudioIn(NULL),
    m_state(ST_NONE), m_bStop(false), m_pBuffer(NULL), m_nAudioMode(VC_AUDIO_MODE_NORMAL), m_bAsync(false)
{
    VC_LOGD_FUNC();
}

VC_AudioInGeneral::~VC_AudioInGeneral()
{
    VC_LOGD_FUNC();

    delete m_pAudioIn;
    m_pAudioIn = NULL;

    delete[] m_pBuffer;
    m_pBuffer = NULL;

}

bool VC_AudioInGeneral::Open(INT* sampleRate, INT* fragSize, INT* fragCount, INT mode)
{
    VC_LOGD_FUNC();
    if (NULL == sampleRate) {
        VC_LOGE("VC_AudioInGeneral::invalid sample rate param\n");
        RETURN(false);
    }

    if (ST_NONE != m_state && ST_IDLE != m_state) {
        VC_LOGE("VC_AudioInGeneral::invalid operation, state=%d\n", m_state);
        RETURN(false);
    }

    if (NULL == m_pAudioIn) {
        m_pAudioIn = VC_Factory::Instance()->CreateIAudioIn(m_bMock);
        if (NULL == m_pAudioIn) {
            RETURN(false);
        }

        m_pBuffer = new(MEM_Voice) BYTE[VC_AUDIOIN_BUF_LEN];
        RegisterName(VC_AUDIOIN_THREAD);
        StartRegistThread();
    }

    *fragSize = 4096;
    *fragCount = 4;
    bool ok = m_pAudioIn->Open(*sampleRate, mode);

    if (ok) {
        VC_LOGD("VC_AudioInGeneral:open sucess!\n");
        m_nAudioMode = mode;
        m_state = ST_IDLE;
    }
    return ok;
}

bool VC_AudioInGeneral::Start(Listener* pListener)
{
    VC_LOGD_FUNC();
    if (NULL == pListener || ST_IDLE != m_state) {
         RETURN(false);
    }

    m_pListener = pListener;
    m_bStop = false;
    m_bAsync = false;
    m_state = ST_RUNNING;
    Notify();
    return true;
}

void VC_AudioInGeneral::SyncWait()
{
    int loop = 100;
    while (ST_RUNNING == m_state) {
        // Fix issue: OnAudioInData() and Stop() in a deadlock
        m_waitStop.Wait(20);
        if (--loop == 0) {
            VC_LOGE("AudioIn stop timeout, wait max time 2s");
            break;
        }
        VC_LOGD("AudioIn is running need to wait, loop=%d", loop);
    }
}

bool VC_AudioInGeneral::Stop(bool async)
{
    VC_LOGD_FUNC();

    if (ST_RUNNING != m_state) {
        RETURN(false);
    }

    m_bAsync = async;
    m_bStop = true;
    VC_LOGI("Stop, async=%d", m_bAsync);

    if (!async) {
        SyncWait();
    }
    return true;
}

bool VC_AudioInGeneral::Close()
{
    VC_LOGD_FUNC();
    bool res = true;
    if (NULL != m_pAudioIn) {
        if (!m_bAsync) {
            res = m_pAudioIn->Close();
            if (res) {
                m_state = ST_NONE;
            }
        }
    }
    return res;
}

void VC_AudioInGeneral::Run()
{
    VC_LOGD_FUNC();
    while (true) {
        VC_LOGI("AudioIn thread waiting ...\n");
        Wait();
        VC_LOGI("AudioIn thread wake up\n");
        if (TRUE == CheckQuit()) {
            VC_LOGD("AudioIn thread quit!\n");
            break;
        }
        DoTask();
    }
}

void VC_AudioInGeneral::DoTask()
{
    VC_LOGD_FUNC();
    if (VC_AUDIO_MODE_NORMAL != m_nAudioMode) {
        void* buf  = NULL;
        int size = 0;
        m_pAudioIn->GetParam(VC_AUDIOIN_BARGEIN_PARAM, &buf, &size);
        m_pListener->OnAudioInCustom(VC_AUDIOIN_BARGEIN_PARAM, buf, size);
    }

    m_pListener->OnAudioInStarted();

    while (!m_bStop) {	// user stop when m_bStop == true
        int len = m_pAudioIn->Read(m_pBuffer, VC_AUDIOIN_BUF_LEN);
        m_pListener->OnAudioInData(m_pBuffer, len);
        if (len < 0) {	// we finished recording wave if len < 0
            break;
        }
    }

    m_pAudioIn->Stop();
    if (m_bAsync) {
        m_pAudioIn->Close();
        m_state = ST_NONE;
    }
    else {
        m_state = ST_IDLE;
        m_waitStop.Notify();
    }
    m_pListener->OnAudioInStopped();
}
/* EOF */
