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
 * @file TTS_PlayElementSilence.h
 * @brief Declaration file of class TTS_PlayElementSilence.                  
 *
 * This file includes the declaration of class TTS_PlayElementSilence and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef CXX_TTS_PLAYELEMENTSILENCE_H
#define CXX_TTS_PLAYELEMENTSILENCE_H

#include "TTS_PlayElementBase.h"
#include "CL_WaitObj.h"
#include <stdio.h>

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* TTS_PlayElementSilence
*
* The class is just for TTS_PlayElementSilence.
*/
class TTS_PlayElementSilence : public TTS_PlayElementBase
{
public:
    TTS_PlayElementSilence();
    virtual ~TTS_PlayElementSilence();

    void SetSilenceTime(int millisecond)
    {
        m_millisecond = millisecond;
    }

    int GetSilenceTime() const
    {
        return m_millisecond;
    }

    virtual CL_BOOL DoPlay(TTS_EngineBuffers* buffer);
    virtual void Stop();

private:
    TTS_PlayElementSilence(const TTS_PlayElementSilence&);
    TTS_PlayElementSilence & operator = (const TTS_PlayElementSilence&);

private:
    int							m_millisecond;
    pthread_mutex_t 			mutex;
    pthread_cond_t 				cond;
    pthread_condattr_t 			condattr;
};


#endif
/* EOF */
