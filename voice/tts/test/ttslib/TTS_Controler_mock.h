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
 * @file TTS_Controler_mock.h
 * @brief Declaration file of class MockTTS_Controler.                  
 *
 * This file includes the declaration of test class MockTTS_Controler and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef CXX_TTS_CONTROLER_MOCK_H_
#define CXX_TTS_CONTROLER_MOCK_H_

#include "TTS_Controler.h"
#include "gmock/gmock.h"
#include "BL_String.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* MockTTS_Controler
*
* The class is just for TTS_Controler test.
*/
class MockTTS_Controler : public TTS_Controler
{
public:
    // MockTTS_Controler():TTS_Controler(NULL, NULL) {}

    MOCK_METHOD2(OnPlayEnd, void(TTS_PlaylistInfo *playlist, TTS_PlayResultBase *result));
};

#endif /* CXX_TTS_CONTROLER_MOCK_H_ */
/* EOF */
