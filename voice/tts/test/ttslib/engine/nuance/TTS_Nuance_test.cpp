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

BYTE* TTS_EngineBuffers::GetWriteBuffer(int& nLen)
{
    if (m_writePos == 11 && m_readPos == 12) {
        nLen = 1;
        return NULL;
    }

    if (m_writePos == 13 && m_readPos == 14) {
        BYTE* buffer = new BYTE[10];
        nLen = 0;
        return buffer;
    }

    if (m_writePos == 15 && m_readPos == 16) {
        BYTE* buffer = new BYTE[10];
        nLen = 10;
        return buffer;
    }

    return NULL;
}

/* EOF */