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
 * @file TTS_Engine_mock.h
 * @brief Declaration file of class MockTTS_Engine.                
 *
 * This file includes the declaration of test class MockTTS_Engine and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _TTS_ENGINE_MOCK_H_
#define _TTS_ENGINE_MOCK_H_

#include "TTS_Engine.h"
#include "gmock/gmock.h"
#include "BL_String.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* MockTTS_Engine
*
* The class is just for TTS_Engine test.
*/
class MockTTS_Engine : public TTS_Engine
{
public:
    MOCK_METHOD4(SynthesizeText, bool(const XCHAR* pszText, const std::string& module, const std::string& lang, TTS_EngineBuffers* buffer));
    MOCK_METHOD2(Init, bool(const XCHAR* conf, const TTS_EngineInfo& params)); 
    MOCK_METHOD0(Uninit, CL_ERROR());
    MOCK_METHOD1(Stop, CL_ERROR(const std::string& lang));
    // MOCK_METHOD4(GetParamRange, void(TTS_PARAM_INTERNAL_TYPE type, int &min, int &max, int &def));
    MOCK_CONST_METHOD1(GetIntonation, CL_ERROR(TtsIntonation* pIntonation));
    MOCK_METHOD1(SetIntonation, CL_ERROR(TtsIntonation intonation));
    MOCK_METHOD1(ResetEngineParam, void(const TTS_EngineLanguage& param));
    MOCK_METHOD1(LoadEngine, bool(const TTS_EngineInfo& engineInfo));
    MOCK_METHOD1(ReLoadEngine, bool(const TTS_EngineInfo& engineInfo));
    MOCK_CONST_METHOD0(GetSampleRate, INT());
};

/**
* MockTTS_EngineBuffers
*
* The class is just for TTS_Engine test.
*/
class TTS_EngineBuffers_mock : public TTS_EngineBuffers
{
public:
    MOCK_METHOD0(Clear, void());
    MOCK_METHOD0(NewPlayer, void());
    MOCK_METHOD1(GetWriteBuffer, BYTE*(int& nLen));
    MOCK_METHOD1(GetReadBuffer, BYTE*(int& nSize));
    MOCK_METHOD1(SetWriteLen, void(const int& nSize));
    MOCK_METHOD1(SetReadLen, void(const int& nSize));
    MOCK_METHOD0(Stop, void());
    MOCK_METHOD0(FinishPhrase, void());
    MOCK_METHOD0(IsStop, bool());
};


#endif /* _TTS_ENGINE_MOCK_H_ */
/* EOF */
