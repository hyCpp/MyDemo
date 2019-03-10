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
 * @file VC_DecoderOgg_mock.h                                            
 * @brief Summary for MockVC_DecoderOgg.
 *
 * This file includes the declaration of class MockVC_DecoderOgg and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_DECODEROGG_MOCK_H
#define VC_DECODEROGG_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VC_DecoderOgg.h"


/**
 * Summary for class MockVC_DecoderOgg
 * 
 * This is a detail description for class MockVC_DecoderOgg
 *
 */
class MockVC_DecoderOgg : public VC_DecoderOgg 
{
public:
    MockVC_DecoderOgg() : VC_DecoderOgg()  
    {
    }
    
    MOCK_METHOD1(Init, CL_ERROR(const VOID* param));
    MOCK_METHOD3(Decode, CL_ERROR(const VOID* pData, DWORD dwLen, Listener* pListener));
    MOCK_METHOD4(Decode, CL_ERROR(const BL_String& fileName, LONG lOffset, DWORD dwLen, Listener* pListener));
};


#endif /* VC_DECODEROGG_MOCK_H */
/* EOF */
