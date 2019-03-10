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
 * @file VC_IAudioOutAlsa_mock.h                                            
 * @brief Summary for MockVC_IAudioOutMedia.
 *
 * This file includes the declaration of class MockVC_IAudioOutMedia and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_IAUDIOOUTALSA_MOCK_H
#define VC_IAUDIOOUTALSA_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VC_IAudioOutMedia.h"


/**
 * Summary for class MockVC_IAudioOutMedia
 * 
 * This is a detail description for class MockVC_IAudioOutMedia
 *
 */
class MockVC_IAudioOutMedia : public VC_IAudioOutMedia 
{
public:
    MockVC_IAudioOutMedia() : VC_IAudioOutAlsa(0)  
    {
    }
    
    MOCK_METHOD3(Open, CL_BOOL(INT* sampleRate, INT* fragSize, INT* fragCount));
    MOCK_METHOD0(Close, CL_BOOL());
};


#endif /* VC_IAUDIOOUTALSA_MOCK_H */
/* EOF */
