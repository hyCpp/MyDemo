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
 * @file VC_AudioSession.h                                            
 * @brief Summary for VC_AudioSession.
 *
 * This file includes the declaration of class VC_AudioSession and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_AUDIOSESSION_H
#define VC_AUDIOSESSION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include "CL_NewTypesDefine.h"
/**
 * Summary for class VC_AudioSession
 * 
 * This is a detail description for class VC_AudioSession
 *
 */
class VC_AudioSession 
{
public:

    /**
     * Summary for class VC_AudioSessionListener
     * 
     * This is a detail description for class VC_AudioSessionListener
     *
     */
    class VC_AudioSessionListener
    {
    public:
        virtual ~VC_AudioSessionListener() {}
        virtual void OnChannelCloseByOS() = 0;
    };

    virtual ~VC_AudioSession() {}
    enum AUDIO_TYPE { AUDIO_TYPE_SOUND, AUDIO_TYPE_BEEP, AUDIO_TYPE_TTS };

    virtual CL_BOOL ReqAudioOutOn(BYTE mediaType) = 0;
    virtual CL_BOOL ReqAudioOutOff(BYTE mediaType) = 0;
    
    virtual CL_BOOL TurnAudioIn(CL_BOOL on) 
    { 
        return CL_FALSE; 
    }

    virtual CL_BOOL SetSessionActive(CL_BOOL on) = 0;
    virtual void SetChannelCloseByOSListener(VC_AudioSession::VC_AudioSessionListener *listener) {}
    virtual void SendMsgForSourceActivity() {}
};


#endif /* VC_AUDIOSESSION_H */
/* EOF */
