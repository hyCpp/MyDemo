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
 * @file VC_TtsEngine_mock.h                                            
 * @brief Summary for MockVC_TtsEngine.
 *
 * This file includes the declaration of class MockVC_TtsEngine and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef CXX_VC_TTSENGINE_MOCK_H_
#define CXX_VC_TTSENGINE_MOCK_H_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#ifndef CXX_VP_TTSENGINE_H
#    include "VC_TtsEngine.h"
#endif

/**
 * Summary for class MockVC_TtsEngine
 * 
 * This is a detail description for class MockVC_TtsEngine
 *
 */
class MockVC_TtsEngine : public VC_TtsEngine 
{
public:
    MOCK_METHOD1(SetLanguage, CL_ERROR(LONG lang));
    MOCK_METHOD2(SynthesizeText, CL_ERROR(const XCHAR* pszText, Listener* pListener));
};

#endif // CXX_VC_TTSENGINE_MOCK_H_
/* EOF */