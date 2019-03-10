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
#include "TTS_AudioOutThread.h"
#include "TTS_Engine.h"
#include "VC_AudioBufferGeneral.h"
#include "AplThreadName.h"
#include "VC_Factory.h"
#include <fstream>
 
TTS_AudioOutThread::TTS_AudioOutThread() : m_buffer(NULL), m_drainWait(false)
{
    m_audioOut = VC_Factory::Instance()->CreateIAudioOut(VC_AUDIO_SOUND);
}

TTS_AudioOutThread::~TTS_AudioOutThread()
{
    if (NULL != m_audioOut) {
        delete m_audioOut;
        m_audioOut = NULL;
    }
}

CL_BOOL TTS_AudioOutThread::Play(TTS_EngineBuffers* buffer, int sampleRate, int channel, TTS_STREAM_DEVICE_TYPE stream, int token, bool drainWait)
{

    if (NULL == m_audioOut || NULL == buffer || sampleRate == -1) {
        return false;
    }
    
    TTS_LOGI("begin to play...");

    if (!m_audioOut->Open(&sampleRate, channel, stream, token)) {
        return CL_FALSE;
    }

    m_drainWait = drainWait;

    m_buffer = buffer;
    m_stopWait.Reset();
    
    TTS_LOGI("begin play,Rate=%d", sampleRate);
    Notify(); 
    return CL_TRUE;   
}

void TTS_AudioOutThread::JoinPlayer()
{
    TTS_LOGI("OutThread::Join");
    m_stopWait.Wait(); 
    TTS_LOGI("OutThread::Join end");
}

CL_BOOL TTS_AudioOutThread::Close()
{
    return m_audioOut->Close();
}

VOID TTS_AudioOutThread::Run()
{
    while (1) {
        TTS_LOGI("OutThread waiting ...");
        Wait();
        TTS_LOGI("OutThread wake up");
        if (TRUE == CheckQuit()) {
            TTS_LOGI("OutThread: quit thread!");
            break;
        }
        DoTask();
    }
}

VOID TTS_AudioOutThread::DoTask() 
{

#if 0 
    std::ofstream file("/data/test.pcm", std::ios::binary);
#endif
    do {
        int size = 0;
        BYTE* byte = m_buffer->GetReadBuffer(size);

        if (m_buffer->IsStop() || NULL == byte || size <= 0) {
            TTS_LOGI("OutThread::DoTask end");
            break;
        }

#if 0 
        file.write((const char*)byte, size);
#endif

        int len = m_audioOut->Write(byte, size);
        if (len <= 0) {
            TTS_LOGI("Err Write,Len=%d", len);
            m_buffer->Stop();
            break;
        }

        m_buffer->SetReadLen(len);

    } while (1);

    if (m_drainWait || m_buffer->IsStop()) {
        Drain();
    }
    else {
        DrainNoSleep();
    }

    Close();

    m_stopWait.Notify();

}

void TTS_AudioOutThread::Drain()
{
    m_audioOut->Drain();
}

void TTS_AudioOutThread::DrainNoSleep()
{
    m_audioOut->DrainNoSleep();
}

void TTS_AudioOutThread::Flush()
{
    m_audioOut->Flush();
}

void TTS_AudioOutThread::StartAudioThread()
{
    RegisterName(VC_AUDIOOUT_THREAD);
    StartRegistThread();
}

void TTS_AudioOutThread::StopOutThread()
{
    TTS_LOGI("StopOutThread");
    StopThread();
}
/* EOF */

