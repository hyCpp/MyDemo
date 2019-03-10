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
 * @file TTS_DurationCalculater.h
 * @brief Declaration file of class TTS_DurationCalculater.                  
 *
 * This file includes the declaration of class TTS_DurationCalculater and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef _CXX_DURATION_CALCULATER_H_
#define _CXX_DURATION_CALCULATER_H_

#include "TTS_Engine.h"
#include <string>
#include <fstream>
#include "MEM_vector.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_Engine;

/**
* Mock_NMAudioStream
*
* The class is just for AudioMedia test.
*/
class TTS_DurationCalculater : public TTS_Engine::Listener
{
public:
    TTS_DurationCalculater();
    virtual ~TTS_DurationCalculater();

    int CaculateDuration(int lang, const char *fileName);
    
    inline void SetPitch(int pitch)
    {
        m_pitch = pitch;
    }

    inline void SetSpeed(int speed)
    {
        m_speed = speed;
    }

    inline void SetVolume(int volume)
    {
        m_volume = volume;
    }

#if 0
    // Inherited from TTS_Engine::Listener
    virtual bool SpeechBuffer(BYTE** pBuffer, DWORD* dwSize);
    virtual void SpeechReady(BYTE* pData, DWORD dwLen);
#endif    

private:
    // bool GetTTSLangType(int lang, std::string &isoLangType);
    void DoCalculate(TTS_Engine *engine, std::ifstream &file);

    TTS_DurationCalculater(const TTS_DurationCalculater &);
    const TTS_DurationCalculater & operator = (const TTS_DurationCalculater &);

private:
    static const int TTS_BUFFER_LEN = 4096;
    unsigned char 		*m_buffer;
    int 				m_dataLen;
    int					m_textLen;
    int					m_pitch;
    int 				m_speed;
    int 				m_volume;
    VoiceVector<double>::type m_durs;
    // std::map<int, int>	m_textLenAndDataLen;	// key: text length, value:data length of engine created.
};

#endif /* _CXX_DURATION_CALCULATER_H_ */
/* EOF */
