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
 * @file VC_IAudioInMedia.h                                            
 * @brief Summary for VC_IAudioInMedia.
 *
 * This file includes the declaration of class VC_IAudioInMedia and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_IAUDIOINMEDIA_H
#define VC_IAUDIOINMEDIA_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "VC_IAudioIn.h"

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
 * Summary for class VC_IAudioInMedia
 * 
 * This is a detail description for class VC_IAudioInMedia
 *
 */
class  VC_IAudioInMedia: public VC_IAudioIn
{
public:
    VC_IAudioInMedia();
    virtual ~VC_IAudioInMedia();

    virtual bool Open(int sampleRate, int mode);
    virtual bool GetParam(int type, void **buf, int* size);
    virtual int Read(VOID* buf, int size);
    virtual bool Stop();
    virtual bool Close();

private:
    // Disable copy and assign constructors
    VC_IAudioInMedia(const VC_IAudioInMedia & o);
    VC_IAudioInMedia& operator= (const VC_IAudioInMedia & o);

    enum State { ST_NONE, ST_OPENED, ST_RUNNING, ST_STOPPED };
    
    AUDIO_STREAM*           m_pStream;
    AUDIO_STREAM_CONFIG*    m_pConfig;

    State m_state;
    
    char* m_pBargeInData;
    unsigned int m_bargeInDataSize;
};

#endif /* VC_IAUDIOINMEDIA_H */
/* EOF */
