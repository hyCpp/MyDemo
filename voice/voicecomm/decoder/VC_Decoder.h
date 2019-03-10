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
 * @file VC_Decoder.h                                            
 * @brief Summary for VC_Decoder.
 *
 * This file includes the declaration of class VC_Decoder and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_DECODER_H
#define VC_DECODER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#ifndef CXX_VC_DECODER_H
#define CXX_VC_DECODER_H

#include "BL_String.h"

class VC_AudioBuffer;

/* ------------------------------------------------------------------------- */
// constants for engine result code
enum DEC_ENG_RESULT 
{
    DEC_ENG_OK,
    DEC_ENG_ABORT,
    DEC_ENG_ERROR
};

/* ------------------------------------------------------------------------- */
enum DEC_PARAM_TYPE 
{
    DEC_PARAM_LANG,
    DEC_PARAM_BUF,
    DEC_PARAM_BUF_SIZE
};

/**
* Summary for class VC_Decoder
*
* The VC_Decoder class is a general abstract interface, it provides all general methods
* to adapt different decoder.
*/
class VC_Decoder
{
public:
    /**
     * Summary for class Listener
     *
     * Handles the callback for decoder.
     * A client must implement this interface to set sound data buffer to decoder
     * and receive notification about sound data is ready by decoder.
     */
    class Listener {
    public:
        virtual ~Listener() {}
        /**
         * Request client to set speech buffer and its length
         *
         * @return buffer if can get a free speech buffer,
         * else return null if user request to stop
         */
        virtual VC_AudioBuffer* OnDecBuffer() = 0;

        /**
         * Notify client that decoded data is ready
         *
         * @param pBuf [IN] Decoder engine has generated sound data
         * @return None
         */
        virtual VOID OnDecReady(VC_AudioBuffer* pBuf) = 0;
    };

    virtual ~VC_Decoder() {}

    /**
     * Open TTS engine adapter, which will load necessary resource
     * @param conf [IN] TTS configuration
     *
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Init(const VOID* param) = 0;

    /**
     * Close TTS engine adapter
     *
     * @return APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Uninit() = 0;

    /**
     * Decode buffer data to sound wave data
     *
     * @param pData [IN] source data
     * @param dwLen [IN] source data length
     * @param pListener [IN] The decoder listener
     * @return	APL_ERR_DEC_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Decode(const VOID* pData, DWORD dwLen, Listener* pListener) = 0;

    /**
     * Decode file to sound wave data
     *
     * @param fileName [IN] sound data file name
    * @param lOffset [IN] sound data file offset
     * @param dwLen [IN] sound data file length
     * @param pListener [IN] The decoder listener
     * @return	APL_ERR_DEC_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Decode(const BL_String& fileName, LONG lOffset, DWORD dwLen, Listener* pListener) =0;

    /**
     * Stop decoder process
     *
     * @return	APL_ERR_DEC_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Stop() = 0;

    /**
     * Get decoder parameter
     *
     * @param type [IN]  param type
     * @param param [OUT] Current param setting value
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR GetParam(DEC_PARAM_TYPE type, VOID* param) const = 0;

    /**
     * Set decoder parameter
     *
     * @param type [IN] param type
     * @param param [OUT] Current param setting value
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR SetParam(DEC_PARAM_TYPE type, const VOID* param) = 0;

    /**
     * Set sample rate
     *
     * @param fileName where to read information about sample rate
     * @param the information's offset from file head
     */
    virtual VOID SetSampleRate(const BL_String& fileName, LONG lOffset) = 0;

    /**
     * Query sample rate
     *
     * @return decoder supported sample rate.
     */
    virtual INT GetSampleRate() const = 0;
    
    /**
     * Get TTS text duraton
     */
    virtual INT GetDuration(const BL_String& ttsText)  
    {
        return 0; 
    }
};

#endif /* >>CXX_VC_DECODER_H<< */
/* EOF */


#endif /* VC_DECODER_H */
/* EOF */
