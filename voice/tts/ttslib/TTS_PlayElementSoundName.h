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
 * @file TTS_PlayElementSoundName.h
 * @brief Declaration file of class TTS_PlayElementSoundName. 
 *
 * This file includes the declaration of  class TTS_PlayElementSoundName and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef CXX_TTS_PLAYELEMENTSOUNDNAME_H
#define CXX_TTS_PLAYELEMENTSOUNDNAME_H

#include "TTS_PlayElementBase.h"
#include "BL_String.h"
#include "CL_SyncObj.h"
#include "BL_FullFile.h"
#include "TTS_WaveFile.h"
#include <stdio.h>

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_EngineBuffer;

/**
* TTS_PlayElementSoundName
*
* The class is just for TTS_PlayElementSoundName.
*/
class TTS_PlayElementSoundName : public TTS_PlayElementBase
{
public:
    TTS_PlayElementSoundName();
    virtual ~TTS_PlayElementSoundName();

    void SetSoundName(const BL_String &soundName)
    {
        m_cSoundName = soundName;
        m_bValidfile = m_waveFile.ReadFile(soundName.GetString());
    }

    const BL_String & GetSoundName() const
    {
        return m_cSoundName;
    }

    int GetSampleRate() const
    {
        return m_waveFile.GetSampleRate();
    }

    int GetChannel() const 
    {
        return m_waveFile.GetChannel();
    }

    virtual CL_BOOL DoPlay(TTS_EngineBuffers* buffer);

private:
    TTS_PlayElementSoundName(const TTS_PlayElementSoundName&);
    TTS_PlayElementSoundName & operator = (const TTS_PlayElementSoundName&);

private:
    BL_String			m_cSoundName;
    TTS_WaveFile		m_waveFile;
    bool                m_bValidfile;
};

#endif
/* EOF */
