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
 * @file VC_AudioBufferGeneral.h                                            
 * @brief Summary for VC_AudioBufferGeneral.
 *
 * This file includes the declaration of class VC_AudioBufferGeneral and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_AUDIOBUFFERGENERAL_H
#define VC_AUDIOBUFFERGENERAL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once
#include "VC_CommDef.h"

/**
 * Summary for class VC_AudioBufferGeneral
 * 
 * This is a detail description for class VC_AudioBufferGeneral
 *
 */
class  VC_AudioBufferGeneral : public VC_AudioBuffer 
{
public:
    static VC_AudioBufferGeneral* Alloc(INT buff_size) 
    {
        VC_AudioBufferGeneral* p = new(MEM_Voice) VC_AudioBufferGeneral(buff_size);
        if (NULL != p && NULL == p->addr()) {
            delete p;
            p = NULL;
        }
        return p;
    }

    explicit VC_AudioBufferGeneral(INT buff_size) : m_nLen(0) 
    {
        m_pBuf = (buff_size > 0) ? new(MEM_Voice) BYTE[buff_size] : NULL;
        m_nSize = (buff_size > 0) ? buff_size : 0;
    }

    virtual  ~VC_AudioBufferGeneral() 
    { 
        delete[] m_pBuf; 
    }

    virtual VOID* addr() 
    { 
        return m_pBuf; 
    }

    virtual INT   size() 
    { 
        return m_nSize; 
    }

    virtual VOID  setLen(INT len) 
    { 
        m_nLen = len; 
    }

    INT getLen() 
    { 
        return m_nLen; 
    }

private:
    VC_AudioBufferGeneral(const VC_AudioBufferGeneral&);
    VC_AudioBufferGeneral& operator = (const VC_AudioBufferGeneral&);

    INT m_nSize;
    INT m_nLen;
    BYTE* m_pBuf;
};


#endif /* VC_AUDIOBUFFERGENERAL_H */
/* EOF */
