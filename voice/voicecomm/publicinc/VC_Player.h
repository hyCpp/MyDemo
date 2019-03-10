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
 * @file VC_Player.h                                            
 * @brief Summary for VC_Player.
 *
 * This file includes the declaration of class VC_Player and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_PLAYER_H
#define VC_PLAYER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once
#include "VC_CommonDef.h"

class BL_String;

/**
 * Summary for class VC_Player
 * 
 * This is a detail description for class VC_Player
 *
 */
class VC_API VC_Player
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
         * Playback event listener
         * @param success playback reuslt
         * @return None
         */
        virtual VOID OnPlayComplete(CL_BOOL success) = 0;
    };
 
    virtual ~VC_Player() {}
    
    virtual CL_ERROR Init() = 0;

    /** 
     * Play TTS text 
     * @param sampleRate audio in device sample rate, such as 16000, 22050 HZ
     */
    virtual CL_BOOL Play(const BL_String& ttsText, DWORD dwLanguage, 
            VC_Player::Listener* pListener, int tokenid = 0, bool last_phrase = false) = 0;
    
    /** 
     * Play Buffer sound data 
     */
    virtual CL_BOOL Play(const VOID* pBuf, LONG lLen, VC_Player::Listener* pListener,
            int tokenid = 0, bool last_phrase = false) = 0;
    
    /** 
     * Play sound file by means of OS media framework
     */
    virtual CL_BOOL Play(const BL_String& fileName, LONG lOffset, LONG lLen, 
            VC_Player::Listener* pListener, int tokenid = 0, bool last_phrase = false) 
    { 
        return CL_FALSE; 
    }
    
    /** 
     * Stop player playback 
     */
    virtual CL_BOOL Stop() = 0;

    /** 
     * Set player volume 
     * @param vol player volume, range is between 0 and 100
     */
    virtual CL_ERROR SetVolume(INT vol) = 0;
    
    /** 
     * Get player volume 
     * @return player volume, -1 represent error
     */
    virtual INT GetVolume() const = 0;

    virtual DWORD GetDuration(const BL_String& text) = 0;

    virtual void StopPlayer() = 0;
};


#endif /* VC_PLAYER_H */
/* EOF */
