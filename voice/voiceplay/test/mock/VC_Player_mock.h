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
 * @file VC_Player_mock.h                                            
 * @brief Summary for MockVC_Player.
 *
 * This file includes the declaration of class MockVC_Player and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_PLAYER_MOCK_H
#define VC_PLAYER_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VC_Player.h"


/**
 * Summary for class MockVC_Player
 * 
 * This is a detail description for class MockVC_Player
 *
 */
class MockVC_Player : public VC_Player 
{
public:

/**
 * Summary for class MockListener
 * 
 * This is a detail description for class MockListener
 *
 */
    class MockListener : public Listener{
        MOCK_METHOD1(OnPlayComplete, VOID(CL_BOOL success));
    };

    MOCK_METHOD5(Play, CL_BOOL(const BL_String& ttsText, DWORD dwLanguage, 
                VC_Player::Listener* pListener, int tokenid, bool last_phrase));
    MOCK_METHOD5(Play, CL_BOOL(const VOID* pBuf, LONG lLen, VC_Player::Listener* pListener,
                int tokenid, bool last_phrase));
    MOCK_METHOD6(Play, CL_BOOL(const BL_String& fileName, LONG lOffset, LONG lLen, 
                VC_Player::Listener* pListener, int tokenid, bool last_phrase));
    MOCK_METHOD0(Init, CL_ERROR());
    MOCK_METHOD0(Stop, CL_BOOL());
    MOCK_METHOD1(SetVolume, CL_ERROR(INT vol));
    MOCK_CONST_METHOD0(GetVolume, INT());
    MOCK_METHOD1(GetDuration, DWORD(const BL_String& text));
    MOCK_METHOD0(StopPlayer, void());
};

#endif /* VC_PLAYER_MOCK_H */
/* EOF */
