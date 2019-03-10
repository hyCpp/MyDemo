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
 * @file TTS_EngineFlite
 * @brief Declaration file of class TTS_EngineFlite.
 *
 * This file includes the declaration of test class TTS_EngineFlite and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef _TTS_ENGINE_FLITE_H
#define _TTS_ENGINE_FLITE_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#pragma once

#include "TTS_Engine.h"
#include <string>
#include "MEM_vector.h"
#include "MEM_list.h"
#include "MEM_map.h"

#include <flite.h>

#ifdef __cplusplus
extern "C" {
#endif

void usenglish_init(cst_voice* v);

cst_lexicon *cmulex_init(void);

cst_voice* register_cmu_us_rms(const char* voxdir);

#ifdef __cplusplus
}
#endif


/**
* TTS_EngineFlite
*
* The class is just for TTS_EngineFlite.
*/

class TTS_EngineFlite : public TTS_Engine
{
public:
    TTS_EngineFlite();
    virtual ~TTS_EngineFlite();

    /**
     * Open TTS engine adapter, which will load necessary resource
     * @param conf [IN] TTS configuration
     *
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual bool Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo);

    virtual bool LoadEngine(const TTS_EngineInfo& engineInfo);

    virtual bool ReLoadEngine(const TTS_EngineInfo& engineInfo);

    /**
     * Close TTS engine adapter
     *
     * @return APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Uninit();

    /**
     * Convert text to TTS sound wave data
     *
     * @param pszText [IN] The text to convert
     * @param pListener [IN] The text conversion listener
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual bool SynthesizeText(const XCHAR* pszText, const std::string& module,
                                const std::string& lang, const std::string& sex, TTS_EngineBuffers* buffer);

    /**
     * Stop TTS engine's synthesize
     *
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Stop(const std::string& lang);


    /**
     * Get TTS intonation setting value
     *
     * @param pIntonation [OUT] Current TTS intonation setting value
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR GetIntonation(TtsIntonation* pIntonation) const;

    /**
     * Set TTS intonation setting value
     *
     * @param intonation [IN] new intonation value
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR SetIntonation(TtsIntonation intonation);

    /**
     * Get TTS synthesis PCM sample rate
     *
     * @return	synthesis PCM sample rate.
     */
    virtual INT GetSampleRate() const;

    virtual void ResetEngineParam(const TTS_EngineLanguage& param);


private:

    TTS_EngineFlite(const TTS_EngineFlite&);
    TTS_EngineFlite& operator = (const TTS_EngineFlite&);

    enum ST_STATE { ST_NONE, ST_INIT, ST_RUNING };

    int                         m_sampleRate;
    TTS_EngineBuffers*          m_buffer;
    volatile ST_STATE			m_state;
    std::string					m_userDataDir;

    cst_voice*                  m_pVoice;

    const static std::string    TTS_FLITE_DATA_DIR;
    const static std::string    TTS_FLITE_AEW_VOX;
};

#endif /* _TTS_ENGINE_NUANCE_H */
/* EOF */
