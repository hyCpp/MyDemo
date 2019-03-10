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
 * @file VC_IAudioIn.h                                            
 * @brief Summary for VC_IAudioIn.
 *
 * This file includes the declaration of class VC_IAudioIn and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_IAUDIOIN_H
#define VC_IAUDIOIN_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once
#include "CL_NewTypesDefine.h" 

/**
 * Summary for class VC_IAudioIn
 *
 * Platform Audio In Interface 
 * 
 */
class  VC_IAudioIn
{
public:
/**
 * Summary for class VC_IAudioIn
 * 
 * This is a detail description for class VC_IAudioIn
 *
 */
    class Streamer {
    public:
        virtual ~Streamer() {}
        virtual void OnStreamWrite(void* data, int len) = 0;
    };

    virtual ~VC_IAudioIn() {}

    /**
     * Open audio recorder device
     * @param sampleRate audio in device sample rate, such as 8000, 16000 HZ
     * @param mode audio mode, such as NORMAL, BARGEIN, BARGEIN_WITH_DEUBG
     */
    virtual bool Open(int sampleRate, int mode) = 0;

    virtual bool GetParam(int type, void **buf, int* size) = 0;

    /**
     * Read audio in recording data
     * @param buf recording data buffer 
     * @param size request recording data size
     * @return recorded data length if success, else return - 1 
     */
    virtual INT Read(void* buf, int size) = 0;
    
    /**
     * Stop audio recorder recording
     */
    virtual bool Stop() = 0;
    
    /**
     * Close audio recorder 
     */
    virtual bool Close() = 0;
};


#endif /* VC_IAUDIOIN_H */
/* EOF */
