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
 * @file TTS_Engine.h 
 * @brief Declaration file of class TTS_Engine.
 *
 * This file includes the declaration of class TTS_Engine and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef _CXX_TTS_ENGINE_H_
#define _CXX_TTS_ENGINE_H_

#include <string>
#include "BL_String.h"
#include "MEM_vector.h"
#include "CL_WaitObj.h"
#include "TTS_EngineInfo.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

// Define TTS parameter type
typedef LONG TtsPitch;
typedef LONG TtsSpeed;
typedef LONG TtsVolume;
typedef LONG TtsRole;
typedef LONG TtsIntonation;

struct TtsCtx;
class BL_String;

/**
* TTS_EngineBuffers
*
* The class is for share data between TTS engine with Audio out.
*/
class TTS_EngineBuffers
{
    public:
        TTS_EngineBuffers();
        virtual ~TTS_EngineBuffers(); 

        virtual void Clear();

        virtual void NewPlayer();

        virtual BYTE* GetWriteBuffer(int& nLen);

        virtual BYTE* GetReadBuffer(int& nSize);

        virtual void SetWriteLen(const int& len);

        virtual void SetReadLen(const int& len);

        virtual void Stop();

        virtual void FinishPhrase();

        virtual bool IsStop();

    private:
#ifdef HAVE_NUTSHELL_OS
        static const int m_fragSize = 4096;
#else
        static const int m_fragSize = 4096 * 2;
#endif
        static const int m_fragCount = 4;
        static const int m_size = m_fragSize * m_fragCount;
        int m_readPos;
        int m_writePos;
        bool m_stop;
        bool m_phraseFinish;

        VoiceVector<BYTE>::type m_buffers;

        CL_WaitObj m_writeObj;
        CL_WaitObj m_readObj;
};    



/**
* TTS_Engine
*
* The class is just for TTS_Engine.
*/
class TTS_Engine
{
public:

    virtual ~TTS_Engine() {}

    /**
     * Open TTS engine adapter, which will load necessary resource
     * @param conf [IN] TTS configuration
     *
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual bool Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo) = 0;

    virtual bool LoadEngine(const TTS_EngineInfo& engineInfo) = 0;
    
    virtual bool ReLoadEngine(const TTS_EngineInfo& engineInfo) = 0;

    /**
     * Close TTS engine adapter
     *
     * @return APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Uninit() = 0;


    /**
     * Convert text to TTS sound wave data
     *
     * @param pszText [IN] The text to convert
     * @param pListener [IN] The text conversion listener
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual bool SynthesizeText(const XCHAR* pszText, const std::string& module, const std::string& lang, const std::string& sex, TTS_EngineBuffers* buffer) = 0;

    /**
     * Stop TTS engine's synthesize
     *
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Stop(const std::string& lang) = 0;

    // virtual void GetParamRange(TTS_PARAM_INTERNAL_TYPE type, int &min, int &max, int &def) = 0;

    /**
     * Get TTS intonation setting value
     *
     * @param pIntonation [OUT] Current TTS intonation setting value
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR GetIntonation(TtsIntonation* pIntonation) const = 0;

    /**
     * Set TTS intonation setting value
     *
     * @param intonation [IN] new intonation value
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR SetIntonation(TtsIntonation intonation) = 0;

    /**
     * Get TTS synthesis PCM sample rate
     *
     * @return	synthesis PCM sample rate.
     */
    virtual INT GetSampleRate() const = 0;

    virtual void ResetEngineParam(const TTS_EngineLanguage& param) = 0;
};

typedef TTS_Engine* (*GetInstance_t)();

#endif /* _CXX_TTS_ENGINE_H_ */
/* EOF */
