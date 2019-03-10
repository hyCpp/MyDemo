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

#include "TTS_Engine.h"
#include "CL_WaitObj.h"

TTS_EngineBuffers::TTS_EngineBuffers() : m_readPos(0), m_writePos(0),
    m_stop(false), m_phraseFinish(false) 

{
    m_buffers.resize(m_size);
}

TTS_EngineBuffers::~TTS_EngineBuffers()
{
}

void TTS_EngineBuffers::Clear()
{
    m_readPos = 0;
    m_writePos = 0;
    m_stop = false;
    m_phraseFinish = false;

    m_writeObj.Reset();
    m_readObj.Reset();
}

void TTS_EngineBuffers::NewPlayer()
{
    m_readPos = 0;
    m_writePos = 0;
    m_phraseFinish = false;
}

BYTE* TTS_EngineBuffers::GetWriteBuffer(int& nLen)
{
    int freeSize = m_size - (m_writePos - m_readPos);
    while (freeSize <= 0) {
        if (m_stop || m_phraseFinish) {
            return NULL;
        }

        m_writeObj.Wait();
        freeSize = m_size - (m_writePos - m_readPos);
    }

    if (m_stop || m_phraseFinish) {
        return NULL;
    }

    int nPhysicWritePos = m_writePos % m_size;
    if ((nPhysicWritePos + freeSize) > m_size) {
        freeSize = m_size - nPhysicWritePos;
    }
    nLen = freeSize > m_fragSize ? m_fragSize : freeSize;

    return &(*(m_buffers.begin())) + nPhysicWritePos;
}

BYTE* TTS_EngineBuffers::GetReadBuffer(int& nSize)
{
    int len  = m_writePos - m_readPos;
    while (len < m_fragSize) {
        if (m_stop || m_phraseFinish) {
            break;
        }
        m_readObj.Wait();
        len = m_writePos - m_readPos;
    }

    int nPhysicReadPos = m_readPos % m_size;


    if (nPhysicReadPos + len > m_size) {
        len = m_size - nPhysicReadPos;
    }
    nSize = m_fragSize > len ? len : m_fragSize;

    return &(*(m_buffers.begin())) +  nPhysicReadPos;
}

void TTS_EngineBuffers::SetWriteLen(const int& len)
{
    m_writePos += len;
    m_readObj.Notify();
}

void TTS_EngineBuffers::SetReadLen(const int& len)
{
    m_readPos += len;
    m_writeObj.Notify();
}

void TTS_EngineBuffers::Stop()
{
    m_stop = true;

    m_writeObj.Notify();
    m_readObj.Notify();
}

void TTS_EngineBuffers::FinishPhrase()
{
    m_phraseFinish = true;
    m_writeObj.Notify();
    m_readObj.Notify();
}

bool TTS_EngineBuffers::IsStop()
{
    return m_stop;
}
/* EOF */
