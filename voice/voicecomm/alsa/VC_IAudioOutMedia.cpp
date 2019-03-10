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
#include "VC_IAudioOutMedia.h"
#include "CL_WaitObj.h"

#ifdef  HAVE_NUTSHELL_OS
using namespace nutshell;
#endif

VC_IAudioOutMedia::VC_IAudioOutMedia()
:m_stream(new(MEM_Voice) AUDIO_STREAM())
, m_config(new(MEM_Voice) AUDIO_STREAM_CONFIG())
, m_state(STATE_NONE)
, m_nWriteCount(0)
{
}

VC_IAudioOutMedia::~VC_IAudioOutMedia() 
{
    if (NULL != m_stream) {
        delete m_stream;
    }

    if (NULL != m_config) {
        delete m_config;
    }
}

bool VC_IAudioOutMedia::Open(int* sampleRate, int channels, int tokenid) 
{
    VC_LOGD("AudioOut Open ...");
    return Open(sampleRate, channels, EAUDIO_STREAM_DEVICE_OUTPUT_INTERRUPT, tokenid);
}

bool VC_IAudioOutMedia::Open(int* sampleRate, int channels, int deviceid, int tokenid)
{
    if (NULL == sampleRate) {
        VC_LOGE("Invalid sample rate parameter\n");
        return CL_FALSE;
    }

    if (!m_stream || !m_config) {
        return CL_FALSE;
    }

    VC_LOGP("=== AudioOut Open sampleRate=%d, channels=%d, device=%d", *sampleRate, channels, deviceid);
    // strcpy(m_config->name, "tts");        // fix
    m_config->rate = *sampleRate;
    m_config->channels = channels;        // fix
    m_config->format = EAUDIO_STREAM_FORMAT_TYPE_S16LE;
    m_config->direction = EAUDIO_STREAM_DIRECTION_PLAYBACK;
    m_config->latency = ALSA_LATENCY;
    m_config->prebuf = ALSA_PREBUF;
    // m_config->device = bargein ? EAUDIO_STREAM_DEVICE_VR_OUT : EAUDIO_STREAM_DEVICE_OUTPUT_INTERRUPT;
    m_config->device = static_cast<EAUDIO_STREAM_DEVICE_TYPE>(deviceid);
    m_config->token = tokenid;
    int ret = m_stream->create(*m_config);
    VC_LOGP("=== AudioOut Open End = %d", ret);
    if (ret < 0) {
        VC_LOGE("Open AudioStream failed!");
        return CL_FALSE;
    }

    m_state = STATE_OPENED;
    m_nWriteCount = 0;
    return CL_TRUE;
}

int VC_IAudioOutMedia::Write(const void* buf, int len) 
{
    if (0 == m_nWriteCount) {
        VC_LOGP("Write PCM data, len=%d", len);
    }

    int ret = m_stream->write(buf, len);
    if (len != ret) {
        VC_LOGD("Write PCM data failed, err=%d, len=%d", ret, len);
        m_state = STATE_STOPPED;
        return -1;
    }

    if (0 == m_nWriteCount) {
        VC_LOGP("Write PCM data done");
    }

    ++m_nWriteCount;

    return ret;
}

bool VC_IAudioOutMedia::Stop(bool immediately, bool last_phrase)
{
    VC_LOGD("AudioOut stop ...");
    if (immediately) {
        m_stream->flush();
    } 
    else {
        if (last_phrase) {
            m_stream->drain();
        }
        else {
            VC_LOGD("Set latency to zero!\n");
            m_stream->drain(0);
        }
    }
    m_state = STATE_STOPPED;
    VC_LOGD("AudioOut stop done");
    return CL_TRUE;
}

bool VC_IAudioOutMedia::Close()
{
    VC_LOGP("AudioOut close ...");
    m_stream->destroy();
    VC_LOGP("AudioOut close done");
    m_state = STATE_NONE;
    return CL_TRUE;
}

void VC_IAudioOutMedia::Drain()
{
    VC_LOGI("Drain begin...");
    m_stream->drain();
    VC_LOGI("Drain stop...");
}

// for audiostream need sleep some time ON MEU
// when the request is last phrase, we need call Drain to sleep 
void VC_IAudioOutMedia::DrainNoSleep()
{
    VC_LOGI("Drain begin...");
    m_stream->drain(0);
    VC_LOGI("Drain stop...");
}

void VC_IAudioOutMedia::Flush()
{
    m_stream->flush();
}

#ifdef      HAVE_NUTSHELL_OS
bool VC_IAudioOutMedia::Pause()
{
    VC_LOGD("VC_IAudioOutMedia::Pause()\n");
    return m_stream->pause() < 0 ? CL_FALSE : CL_TRUE;
}

bool VC_IAudioOutMedia::Resume()
{
    VC_LOGD("VC_IAudioOutMedia::Resume()\n");
    return m_stream->resume() < 0 ? CL_FALSE : CL_TRUE;
}

#else
bool VC_IAudioOutMedia::Pause()
{
    return CL_TRUE;
}

bool VC_IAudioOutMedia::Resume()
{
    return CL_TRUE;
}

#endif

/* EOF */
