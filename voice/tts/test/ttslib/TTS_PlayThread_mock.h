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
 * @file TTS_PlayThread_mock.h 
 * @brief Declaration file of class MockTTS_PlayThread.                  
 *
 * This file includes the declaration of test class MockTTS_PlayThread and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _TTS_PLAY_THREAD_MOCK_H_
#define _TTS_PLAY_THREAD_MOCK_H_

#include "TTS_PlayThread.h"
#include "gmock/gmock.h"
#include "BL_String.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* MockTTS_PlayThread
*
* The class is just for TTS_PlayThread test.
*/
class MockTTS_PlayThread : public TTS_PlayThread
{
public:
    // MockTTS_Controler():TTS_Controler(NULL, NULL) {}

    MOCK_METHOD1(StartPlay, void(TTS_PlaylistInfo *playlist));
};

#endif /* _TTS_PLAY_THREAD_MOCK_H_ */
/* EOF */
