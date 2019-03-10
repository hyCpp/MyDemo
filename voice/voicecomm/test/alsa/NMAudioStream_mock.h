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
 * @file Mock_NMAudioStream.h                                          
 * @brief Declaration file of class Mock_NMAudioStream.                  
 *
 * This file includes the declaration of test class Mock_NMAudioStream and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _MOCK_NMAUDIOSTREAM_
#define _MOCK_NMAUDIOSTREAM_

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "NMAudioStream.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* Mock_NMAudioStream
*
* The class is just for AudioMedia test.
*/

/**
 * Summary for class Mock_NMAudioStream
 * 
 * This is a detail description for class Mock_NMAudioStream
 *
 */
class Mock_NMAudioStream : public nutshell::NMAudioStream
{
    public:
        Mock_NMAudioStream() : NMAudioStream()
        {
        }

        MOCK_METHOD0(drain, int32_t());
        MOCK_METHOD0(flush, int32_t());
        MOCK_METHOD0(destroy, int32_t());
        MOCK_METHOD2(write, int32_t(const void* data, uint32_t bytes));
        MOCK_METHOD2(read, int32_t(void* data, uint32_t bytes));
        MOCK_METHOD1(create, int32_t(const NMAudioStream::NMAudioStreamConfig& config));
};

#endif /* _MOCK_NMAUDIOSTREAM_ */
/* EOF */
