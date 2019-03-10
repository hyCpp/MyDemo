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
 * @file VC_IAudioOut.h                                            
 * @brief Summary for VC_IAudioOut.
 *
 * This file includes the declaration of class VC_IAudioOut and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_IAUDIOOUT_H
#define VC_IAUDIOOUT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

class VC_IAudioBuffer;

/**
 * Summary for class VC_IAudioOut
 *
 * There should be a detail description for
 * class VC_IAudioOut
 */
class  VC_IAudioOut
{
public:
    /**
     * Read decoded data streamer
     *
     * There should be a detail description for
     * class Streamer
     */
    class Streamer {
    public:
        virtual ~Streamer() {}
        virtual int OnStreamRead(void* pBuf, int size) = 0;
    };

    virtual ~VC_IAudioOut() {}
    virtual bool Open(int* sampleRate, int channels = 1, int tokenid = 0) = 0;
    virtual bool Open(int* sampleRate, int channels, int deviceid, int tokenid) = 0;
    virtual int Write(const void* pBuf, int nLen) = 0;
    virtual bool Stop(bool immediately, bool last_phrase = false) = 0;
    virtual bool Close() = 0;
    virtual bool Pause() = 0;
    virtual bool Resume() = 0;

    virtual void Drain()
    {
    }

    virtual void DrainNoSleep()
    {
    } 

    virtual void Flush()
    {
    }

};


#endif /* VC_IAUDIOOUT_H */
/* EOF */
