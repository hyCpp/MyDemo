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
#ifndef VC_FACTORY_MOCK_H
#define VC_FACTORY_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VC_Factory.h"
#include "gmock/gmock.h"

/**
 * Summary for class MockVC_AudioOutGeneral
 * 
 * This is a detail description for class MockVC_AudioOutGeneral
 *
 */
class Mock_VCFactory : public VC_Factory
{
public:
    Mock_VCFactory() : VC_Factory()
    {
    }

    MOCK_METHOD1(CreateIAudioIn, VC_IAudioIn*(bool mock));
    MOCK_METHOD1(CreateAudioOut, VC_AudioOut*(VC_AUDIO_TYPE audioType));
    MOCK_METHOD2(CreateDecoder, VC_Decoder*(VC_AUDIO_TYPE audioType, VC_DECODER_TYPE decType));
};

#endif
/* EOF */