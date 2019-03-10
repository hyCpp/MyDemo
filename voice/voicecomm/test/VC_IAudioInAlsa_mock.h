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
 * @file VC_IAudioInAlsa_mock.h                                            
 * @brief Summary for MockVC_IAudioInAlsa.
 *
 * This file includes the declaration of class MockVC_IAudioInAlsa and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_IAUDIOINALSA_MOCK_H
#define VC_IAUDIOINALSA_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VC_IAudioInAlsa.h"


/**
 * Summary for class MockVC_IAudioInAlsa
 * 
 * This is a detail description for class MockVC_IAudioInAlsa
 *
 */
class MockVC_IAudioInAlsa : public VC_IAudioInAlsa 
{
public:
    MockVC_IAudioInAlsa() : VC_IAudioInAlsa()  
    {
    }
    
    MOCK_METHOD3(Open, CL_BOOL(INT* sampleRate, INT* fragSize, INT* fragCount));
   // MOCK_METHOD1(StartAsync, CL_BOOL(Streamer* pStreamer));
    MOCK_METHOD0(Start, CL_BOOL());
    MOCK_METHOD0(Close, CL_BOOL());
    MOCK_METHOD2(Read, INT(VOID* buf, INT size));
//    MOCK_METHOD1(Stop, CL_BOOL(CL_BOOL immediately));
//    MOCK_METHOD2(Write, INT(const VOID* pBuf, INT nLen));
//    MOCK_METHOD1(SetVolume, CL_BOOL(INT volume));
//    MOCK_CONST_METHOD0(GetVolume, INT());
};


#endif /* VC_IAUDIOINALSA_MOCK_H */
/* EOF */
