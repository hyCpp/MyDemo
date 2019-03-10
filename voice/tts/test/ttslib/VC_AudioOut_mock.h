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
 * @file VC_AudioOut_mock.h 
 * @brief Declaration file of class VC_AudioOut_mock.
 *
 * This file includes the declaration of test class VC_AudioOut_mock and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _CXX_TTS_AUDIOOUT_H__
#define _CXX_TTS_AUDIOOUT_H__
#include "VC_IAudioOut.h"
#include "gmock/gmock.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* VC_AudioOut_mock
*
* The class is just for VC_AudioOut test.
*/
class VC_AudioOut_mock : public VC_IAudioOut
{
public:
    MOCK_METHOD3(Open, bool(int* sampleRate, int channels, int tokenid));
    MOCK_METHOD4(Open, bool(int* sampleRate, int channels, int devicedid, int tokenid));
    MOCK_METHOD2(Write, int(const void* pBuf, int nLen));
    MOCK_METHOD2(Stop, bool(bool immediately, bool last_phrase));
    MOCK_METHOD0(Close, bool());
    MOCK_METHOD0(Pause, bool());
    MOCK_METHOD0(Resume, bool());
    MOCK_METHOD0(Drain, void());
    MOCK_METHOD0(Flush, void());
};

#endif /* _CXX_TTS_AUDIOOUT_H__ */
/* EOF */
