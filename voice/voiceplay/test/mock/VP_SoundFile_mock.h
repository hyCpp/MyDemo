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
 * @file VP_SoundFile_mock.h                                            
 * @brief Summary for MockVP_SoundFile.
 *
 * This file includes the declaration of class MockVP_SoundPlayer and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_SOUNDFILE_MOCK_H
#define VP_SOUNDFILE_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "VP_SoundFile.h"

/**
 * This is a mock class for VP_SoundFile
 *
 * This class is used to mock a object for class 
 * VP_SoundFile
 */
class MockVP_SoundFile : public VP_SoundFile
{
public:
    MOCK_METHOD1(Initialize, CL_ERROR(const BL_String& soundFileName));
    MOCK_CONST_METHOD1(GetPhraseInfo, const VP_Phrase*(DWORD phraseId));
};

#endif
/* EOF */
