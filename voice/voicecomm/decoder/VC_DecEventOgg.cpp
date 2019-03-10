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

#include "stdafx.h"
#include "VC_DecEventOgg.h"

VC_DecEventOgg::VC_DecEventOgg(const BL_String& fileName, LONG lOffset, DWORD dwLen, VC_Decoder* pDecoder, VC_Decoder::Listener* pListener)
    :m_pBuf(NULL)
    , m_dwLen(dwLen)
    , m_fileName(fileName)
    , m_lOffset(lOffset)
    , m_pDecoder(pDecoder)
    , m_pListener(pListener) 
{
}

VC_DecEventOgg::VC_DecEventOgg(const VOID* pBuf, DWORD dwLen, VC_Decoder* pDecoder, VC_Decoder::Listener* pListener)
    :m_pBuf(pBuf)
    , m_dwLen(dwLen)
    , m_fileName("")
    , m_lOffset(0)
    , m_pDecoder(pDecoder)
    , m_pListener(pListener) 
{
}

VC_DecEventOgg::~VC_DecEventOgg() 
{
}

CL_ERROR VC_DecEventOgg::DoAction() 
{
    VC_LOGI("VC_DecEventOgg DoAction\n");
    CL_ERROR err = 0;
    if (m_fileName.GetLength() > 0) {
        err = m_pDecoder->Decode(m_fileName, m_lOffset, m_dwLen, m_pListener);
    } 
    else {
           err = m_pDecoder->Decode(m_pBuf, m_dwLen, m_pListener);
    }
    return err;

}
/* EOF */
