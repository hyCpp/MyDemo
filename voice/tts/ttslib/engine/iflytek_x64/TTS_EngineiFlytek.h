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
 * @file TTS_EngineiFlytek
 * @brief Declaration file of class TTS_EngineiFlytek.
 *
 * This file includes the declaration of test class TTS_EngineiFlytek and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef _TTS_ENGINE_IFLYTEK_H
#define _TTS_ENGINE_IFLYTEK_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "TTS_Engine.h"

/**
* TTS_EngineiFlytek
*
* The class is just for TTS_EngineiFlytek.
*/

class TTS_EngineiFlytek : public TTS_Engine
{
public:
    TTS_EngineiFlytek();
    virtual ~TTS_EngineiFlytek();

    /**
     * Open TTS engine adapter, which will load necessary resource
     * @param conf [IN] TTS configuration
     *
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual bool Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo);

    /**
     * Close TTS engine adapter
     *
     * @return APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Uninit();

    virtual bool LoadEngine(const TTS_EngineInfo& engineInfo);

    virtual bool ReLoadEngine(const TTS_EngineInfo& engineInfo);

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
    TTS_EngineiFlytek(const TTS_EngineiFlytek&);
    TTS_EngineiFlytek& operator = (const TTS_EngineiFlytek&);

    bool ttsInit();
    void ttsUninit();
    bool OutputTTSData(int& res);

private:
    TTS_EngineBuffers* m_buffer;
    const char*        m_sessionID;

};

#endif /* _TTS_ENGINE_NUANCE_H */
/* EOF */
