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
 * @file VC_AudioOut.h                                            
 * @brief Summary for VC_AudioOut.
 *
 * This file includes the declaration of class VC_AudioOut and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_AUDIOOUT_H
#define VC_AUDIOOUT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#pragma once
// Forward declaration
class VC_AudioBuffer;
#include "BL_String.h"

enum VC_ErrorCode
{
    VC_ERROR_NO_ERROR,
    VC_ERROR_FAILED,
    VC_ERROR_ERROR,
};

/**
 * Summary for class VC_AudioOut
 *
 * There should be a detail description for
 * class VC_AudioOut
 */
class VC_API VC_AudioOut
{
public:
/**
 * Summary for class Listener
 * 
 * This is a detail description for class Listener
 *
 */
    class Listener {
    public:
        virtual ~Listener() {}
        virtual VOID OnAudioOutDone(VC_AudioBuffer* pBuf) = 0;
        virtual VOID OnAudioOutComplete(CL_BOOL success) = 0;
    };

    virtual ~VC_AudioOut() {}

    /** 
     * Open audio out device
     * @param sampleRate audio in device sample rate, such as 16000, 22050 HZ
     */
    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount, int tokenid = 0)= 0;
    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount, int deviceid, int tokenid)= 0;
    
    /** 
     * Allocate audio loop buffer 
     */
    virtual VC_AudioBuffer* AllocBuffer(INT size) = 0;
     
    /** 
     * Write audio buffer 
     */
    virtual CL_BOOL Write(VC_AudioBuffer* pBuf) = 0;

    /** 
     * Start audio out playback
     */
    virtual CL_BOOL Start(Listener* pListener) = 0;
    
    /**
     * Play
     */
    // virtual CL_BOOL Play() = 0;

    /** 
     * Stop audio out playback
     */
    virtual CL_BOOL Stop(CL_BOOL immediate, bool last_phrase = false) = 0;
    
    /** 
     * Close audio out 
     */
    virtual CL_BOOL Close() = 0;

    /**
    * Pause the currunt play
    */
    virtual CL_ERROR Pause() = 0;

    /**
    * Resume the play
    */
    virtual CL_ERROR Resume() = 0;

    /** 
     * Set volume 
     */
    virtual CL_BOOL SetVolume(INT volume) = 0;

    /** 
     * Get current volume 
     */
    virtual INT  GetVolume() const = 0;
    
    /**
     * Set session active (only for iphone)
     */
    virtual void  SetSessionProperty(CL_BOOL on) = 0;

    virtual void StopOutThread() = 0;
};


#endif /* VC_AUDIOOUT_H */
/* EOF */
