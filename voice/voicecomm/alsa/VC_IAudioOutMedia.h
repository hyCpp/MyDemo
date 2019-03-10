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
 * @file VC_IAudioOutMedia.h                                            
 * @brief Summary for VC_IAudioOutMedia.
 *
 * This file includes the declaration of class VC_IAudioOutMedia and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_IAUDIOOUTMEDIA_H
#define VC_IAUDIOOUTMEDIA_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include "VC_IAudioOut.h"

#ifdef HAVE_NUTSHELL_OS
#include    "NMAudioStream.h"
#define     AUDIO_STREAM            nutshell::NMAudioStream
#define     AUDIO_STREAM_CONFIG     nutshell::NMAudioStream::NMAudioStreamConfig

#else
#include    "VC_AudioStream.h"
#define     AUDIO_STREAM            VC_AudioStream
#define     AUDIO_STREAM_CONFIG     VC_AudioStream::VC_AudioStreamConfig

#endif

/**
 * Summary for class VC_IAudioOutMedia
 * 
 * This is a detail description for class VC_IAudioOutMedia
 *
 */
class  VC_IAudioOutMedia : public VC_IAudioOut 
{
public:
    enum State { STATE_NONE, STATE_OPENED, STATE_PREPARED, STATE_RUNNING, STATE_STOPPED, STATE_SUSPENDED };

    VC_IAudioOutMedia();
    virtual ~VC_IAudioOutMedia();
    virtual bool Open(int* sampleRate, int channels = 1, int tokenid = 0);
    virtual bool Open(int* sampleRate, int channels, int deviceid, int tokenid);
    virtual int Write(const void* buf, int len);
    virtual bool Stop(bool immediately, bool last_phrase = false);
    virtual bool Close();
    virtual bool Pause();
    virtual bool Resume();

    virtual void Drain();
    virtual void DrainNoSleep();
    virtual void Flush();

private:
    VC_IAudioOutMedia(const VC_IAudioOutMedia&);
    VC_IAudioOutMedia& operator = (const VC_IAudioOutMedia&);

    AUDIO_STREAM*           m_stream;
    AUDIO_STREAM_CONFIG*    m_config;
    State                   m_state;

    // Only for performance statistic
    int m_nWriteCount;

    const static int ALSA_LATENCY = 400;
    const static int ALSA_PREBUF = 300;
};


#endif /* VC_IAUDIOOUTMEDIA_H */
/* EOF */
