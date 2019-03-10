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
 * @file VC_AudioIn.h                                            
 * @brief Summary for VC_AudioIn.
 *
 * This file includes the declaration of class VC_AudioIn and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_AUDIOIN_H
#define VC_AUDIOIN_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

/**
 * Summary for class VC_AudioIn
 *
 * There should be a detail description for
 * class VC_AudioIn
 */
class VC_API VC_AudioIn
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
        /**
         * callback of recorded PCM data
         * it will be called repeatly until stopped.
         */
        virtual VOID OnAudioInData(VOID* data, INT len) = 0;

        /**
         * currently type only support barge-in
         * it will be called once on Open.
         * 
         */
        virtual VOID OnAudioInCustom(int type, VOID* data, INT len) {} 
        virtual VOID OnAudioInStarted() {}
        virtual VOID OnAudioInStopped() {}
    };

    virtual ~VC_AudioIn() {}

    /** 
     * Open audio recorder device
     * @param sampleRate audio in device sample rate, such as 8000, 16000 HZ
     */

    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount)
    {
        RETURN(false);
    }

    /** 
     * Open audio recorder device
     * @param sampleRate audio in device sample rate, such as 8000, 16000 HZ
     * @param mode audio mode, such as NORMAL, BARGEIN, BARGEIN_WITH_DEUBG
     */
    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount, INT mode) = 0;

    /**
     * @deprecated  it will be replaced by Open(sampleRate, ..., bargin, debug)
     */
    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount, bool bargeIn)
    {
        RETURN(false);
    }

    /** 
     * Open audio recorder device
     * @param sampleRate audio in device sample rate, such as 8000, 16000 HZ
     * @param bargeIn it wlll interrupt TTS or beep propmpt when it's true
     * @param debug it will run barge-in online debug mode to tune SSE parameters when it's true
     */
    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount, bool bargeIn, bool debug)
    {
        RETURN(false);
    }
    
    /** 
     * Start audio recorder recording
     */
    virtual CL_BOOL Start(Listener* pListener) = 0;
    
    /** 
     * Stop audio recorder recording
     */
    virtual CL_BOOL Stop(bool async = false) = 0;
    
    /** 
     * Close audio recorder 
     */
    virtual CL_BOOL Close() = 0;
};


#endif /* VC_AUDIOIN_H */
/* EOF */
