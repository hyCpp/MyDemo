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
 * @file VC_AudioOutGeneral_mock.h                                            
 * @brief Summary for MockVC_AudioOutGeneral.
 *
 * This file includes the declaration of class MockVC_AudioOutGeneral and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_AUDIOOUTGENERAL_MOCK_H
#define VC_AUDIOOUTGENERAL_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#define private public
#define protected public

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VC_AudioOutGeneral.h"
#include "VC_PlayerGeneral.h"
#include "VC_AudioBufferGeneral.h"


/**
 * Summary for class MockVC_AudioOutGeneral
 * 
 * This is a detail description for class MockVC_AudioOutGeneral
 *
 */
class MockVC_AudioOutGeneral : public VC_AudioOutGeneral 
{
public:
    MockVC_AudioOutGeneral() : VC_AudioOutGeneral(0)  
    {
    }

    MOCK_METHOD1(Write, CL_BOOL(VC_AudioBuffer* pBuf));
    MOCK_METHOD1(Start, CL_BOOL(Listener* pListener));
    MOCK_METHOD1(SetVolume, CL_BOOL(INT volume));
    MOCK_CONST_METHOD0(GetVolume, INT());
    MOCK_METHOD4(Open, CL_BOOL(INT* sampleRate, INT* fragSize, INT* fragCount, int tokenid));
    MOCK_METHOD0(Close, CL_BOOL());
    MOCK_METHOD1(AllocBuffer, VC_AudioBuffer*(INT size));
};


/**
 * Summary for class MockVC_AudioOutGeneral
 * 
 * This is a detail description for class MockVC_AudioOutGeneral
 *
 */
class Test_VC_AudioOutGeneral : public VC_AudioOutGeneral
{
public:
    Test_VC_AudioOutGeneral(VC_PlayerGeneral* player)
    : VC_AudioOutGeneral(0)
    , m_bstop(false)
    , m_balloc(true)
    , m_dwIndexW(true)
    , p_VCPlayer(player)
    {
    }

    virtual CL_BOOL Start(Listener* pListener)
    {
        return true;
    }

    virtual CL_BOOL Write(VC_AudioBuffer* pBuf)
    {
        return true;
    }

    virtual CL_BOOL Open(INT* sampleRate, INT* fragSize, INT* fragCount, int tokenid = 0)
    {
        *fragSize = 10;
        *fragCount = 0;

        if (tokenid == 10) {
            *fragSize = 10;
            *fragCount = 0;
            return false;
        }

        if (tokenid == 11) {
            // m_waitStop.Wait();
            p_VCPlayer->m_dwBufCount = 1;
            p_VCPlayer->m_bStop = CL_TRUE;
        }
        return true;
    }

    virtual VC_AudioBuffer* AllocBuffer(INT nSize)
    {
        if (!m_balloc) {
            return NULL;
        }
        VC_AudioBufferGeneral* pBuf = VC_AudioBufferGeneral::Alloc(nSize);
        if (NULL != p_VCPlayer) {
            p_VCPlayer->m_bStop = true;
            if (!m_dwIndexW) {
                p_VCPlayer->m_dwIndexW = -1;
            }
        }
        return pBuf;
    }

    virtual CL_BOOL Close()
    {
        return true;
    }

    virtual CL_BOOL Stop(CL_BOOL immediate)
    {
        return m_bstop;
    }

private:
    bool m_bstop;
    bool m_balloc;
    bool m_dwIndexW;
    VC_PlayerGeneral* p_VCPlayer;
    CL_WaitObj m_waitStop;
};


#endif /* VC_AUDIOOUTGENERAL_MOCK_H */
/* EOF */
