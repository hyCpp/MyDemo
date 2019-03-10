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
 * @file VC_DecEventOgg.h                                            
 * @brief Summary for VC_DecEventOgg.
 *
 * This file includes the declaration of class VC_DecEventOgg and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_DECEVENTOGG_H
#define VC_DECEVENTOGG_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "VC_Decoder.h"
#include "VC_DecEvent.h"

/**
 * Summary for class VC_DecEventOgg
 * 
 * This is a detail description for class VC_DecEventOgg
 *
 */
class VC_DecEventOgg: public VC_DecEvent
{
public:
    VC_DecEventOgg(const BL_String& fileName, LONG lOffset, DWORD dwLen, VC_Decoder* pDecoder, VC_Decoder::Listener* pListener);
    VC_DecEventOgg(const VOID* pBuf, DWORD dwLen, VC_Decoder* pDecoder, VC_Decoder::Listener* pListener);
    virtual ~VC_DecEventOgg();
    virtual CL_ERROR DoAction();

    const VOID* m_pBuf;
    DWORD m_dwLen;
private:
    VC_DecEventOgg(const VC_DecEventOgg&);
    VC_DecEventOgg& operator = (const VC_DecEventOgg&);

    BL_String m_fileName;
    LONG m_lOffset;
    VC_Decoder* m_pDecoder;
    VC_Decoder::Listener* m_pListener;
};


#endif /* VC_DECEVENTOGG_H */
/* EOF */
