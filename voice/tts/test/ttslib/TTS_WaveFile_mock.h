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
 * @file TTS_WaveFile_mock.h 
 * @brief Declaration file of class MockTTS_WaveFile.
 *
 * This file includes the declaration of test class MockTTS_WaveFile and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _MOCK_TTS_WAVE_FILE_H_
#define _MOCK_TTS_WAVE_FILE_H_

#include "TTS_WaveFile.h"
#include "gmock/gmock.h"
#include "BL_String.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* MockTTS_WaveFile
*
* The class is just for TTS_WaveFile test.
*/

class MockTTS_WaveFile : public TTS_WaveFile
{
public:
    MOCK_METHOD2(GoodWaveFile, CL_BOOL(const BL_String &fileName, int *sampleRate));	

};

#endif /* _MOCK_TTS_WAVE_FILE_H_ */
/* EOF */
