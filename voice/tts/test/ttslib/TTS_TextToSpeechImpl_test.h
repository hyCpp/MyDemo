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
 * @file TTS_TextToSpeechImp_test.h 
 * @brief Declaration file of class MockTTS_EngineManager
 *
 * This file includes the declaration of class MockTTS_EngineManager and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _MOCK_TTS_ENGINE_MANGER_
#define _MOCK_TTS_ENGINE_MANGER_

#include "TTS_EngineManager.h"
#include "ttslib_test.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* MockTTS_EngineManager
*
* The class is just for TTS_TextToSpeechImp test.
*/

class MockTTS_EngineManager : public TTS_EngineManager
{
public:
    MockTTS_EngineManager()
    {

    }

    MOCK_METHOD0(GetEngineMgr, void());
    MOCK_METHOD3(SetParameter, CL_ERROR(const BL_String &sender, TTS_PARAM_TYPE type, int value));

};

#endif /* _MOCK_TTS_ENGINE_MANGER_ */
/* EOF */
