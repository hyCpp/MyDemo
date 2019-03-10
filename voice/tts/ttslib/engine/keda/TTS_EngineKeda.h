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
 * @file TTS_EngineKeda.h
 * @brief Declaration file of class TTS_EngineKeda.
 *
 * This file includes the declaration of class TTS_EngineKeda and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */



#ifndef __TTS_ENGINE_KEDA_H__
#define __TTS_ENGINE_KEDA_H__

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "TTS_Engine.h"
#include "iss_tts.h"

#include "CL_WaitObj.h"


/**
* TTS_EngineKeda
*
* The class is just for TTS_EngineKeda.
*/

class TTS_EngineKeda : public TTS_Engine
{
public:
    TTS_EngineKeda();
    virtual ~TTS_EngineKeda();

    /**
     * Open TTS engine adapter,which will load necessary resource
     * @param conf [IN] TTS configuration
     *
     * @return    APL_ERR_TTS_NOERROR if the method succeeds,else return error code.
     */
    virtual bool Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo);

    virtual bool LoadEngine(const TTS_EngineInfo& params)
    {
        return true;
    }
    
    virtual bool ReLoadEngine(const TTS_EngineInfo& engineInfo)
    {
        return true;
    }

    /**
     * Close TTS engine adapter
     *
     * @return APL_ERR_TTS_NOERROR if the method succeeds,else return error code.
     */
    virtual CL_ERROR Uninit();

    /**
     * Set language to TTS engine adapter
     *
     * @param lang [IN] TTS language type
     * @param pListener [IN] Cancel set language listener
     * @return APL_ERR_TTS_NOERROR if the method succeeds,else return error code.
     */
    virtual CL_ERROR SetLanguage(const std::string& lang);

    /**
     * Get TTS engine adapter language settings
     *
     * @param lang [OUT] TTS language type
     * @return APL_ERR_TTS_NOERROR if the method succeeds,else return error code.
     */
    virtual CL_ERROR GetLanguage(std::string& lang);

    /**
     * Convert text to TTS sound wave data
     *
     * @param pszText [IN] The text to convert
     * @param pListener [IN] The text conversion listener
     * @return    APL_ERR_TTS_NOERROR if the method succeeds,else return error code.
     */
    virtual bool SynthesizeText(const XCHAR* pszText, const std::string& module, const std::string& lang, TTS_EngineBuffers* buffer);

    /**
     * Stop TTS engine's synthesize
     *
     * @return    APL_ERR_TTS_NOERROR if the method succeeds,else return error code.
     */
    virtual CL_ERROR Stop(const std::string& lang);

    /**
     * Get TTS intonation setting value
     *
     * @param pIntonation [OUT] Current TTS intonation setting value
     * @return    APL_ERR_TTS_NOERROR if the method succeeds,else return error code.
     */
    virtual CL_ERROR GetIntonation(TtsIntonation* pIntonation) const;

    /**
     * Set TTS intonation setting value
     *
     * @param intonation [IN] new intonation value
     * @return    APL_ERR_TTS_NOERROR if the method succeeds,else return error code.
     */
    virtual CL_ERROR SetIntonation(TtsIntonation intonation);

    /**
     * Get TTS synthesis PCM sample rate
     *
     * @return    synthesis PCM sample rate.
     */
    virtual INT GetSampleRate() const;

    virtual void ResetEngineParam(const TTS_EngineLanguage& param); 

    static void OnTTSDataReady(void* pUsrArg);

    bool OutputTTSData();

private:
    bool SetParams(const TTS_EngineLanguage& param);   
    bool SetParams(const std::string& lang); 

    TTS_EngineKeda(const TTS_EngineKeda&);
    TTS_EngineKeda& operator = (const TTS_EngineKeda&);

private:
    TTS_EngineBuffers*       m_buffer;
    std::string             m_lang;

    HISSTTS                 m_hTTS;
    HISSTTSRES              m_hRes;

    TtsEngineLanguages m_param;
    
    CL_WaitObj m_waitStop;
    int TtsInit(const XCHAR *filename);
    void TtsUninit();
};

#ifdef __cplusplus
extern "C" { TTS_Engine* GetInstance(); }
#endif

#endif
/* EOF */

