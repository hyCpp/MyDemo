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
 * @file VC_DecoderOgg.h                                            
 * @brief Summary for VC_DecoderOgg.
 *
 * This file includes the declaration of class VC_DecoderOgg and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_DECODEROGG_H
#define VC_DECODEROGG_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#ifndef CXX_VC_DECODEROGG_H
#define CXX_VC_DECODEROGG_H

#include "VC_Decoder.h"
#include "ivorbisfile.h"
#include "BL_FullFile.h"
#include "BL_NDataFile.h"

/**
* The VC_DecoderOgg is implementation of VC_Decoder for OGG
*/
struct VC_OggState
{
    OggVorbis_File vf;
    bool        isFile;
    VOID*       dataHandle;   // Pointer to the memory data of file
    long        dataSize;     // Size of the data
    long        dataRead;     // Bytes read so far
};

/**
 * Summary for class VC_DecoderOgg
 * 
 * This is a detail description for class VC_DecoderOgg
 *
 */
class VC_DecoderOgg: public VC_Decoder
{
public:
    VC_DecoderOgg();
    virtual ~VC_DecoderOgg();
    virtual CL_ERROR Init(const VOID* param);
    virtual CL_ERROR Uninit();
    virtual CL_ERROR Decode(const VOID* pData, DWORD dwLen, Listener* pListener);
    virtual CL_ERROR Decode(const BL_String& fileName, LONG lOffset, DWORD dwLen, Listener* pListener);
    virtual CL_ERROR Stop();
    virtual CL_ERROR GetParam(DEC_PARAM_TYPE type, VOID* param) const;
    virtual CL_ERROR SetParam(DEC_PARAM_TYPE type, const VOID* param);
    virtual VOID SetSampleRate(const BL_String& fileName, LONG lOffset);
    virtual INT GetSampleRate() const;
    
private:
    CL_ERROR DoDecode(Listener* pListener);

    VC_OggState m_oggState;
    CL_BOOL     m_bStop;
    // Platform dependent
    BL_NDataFile m_dataFile;
    INT         m_sampleRate;
};

#endif /* >>CXX_VC_DECODEROGG_H<< */
/* EOF */


#endif /* VC_DECODEROGG_H */
/* EOF */
