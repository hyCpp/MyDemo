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
 * @file VP_SoundPlayer_mock.h                                            
 * @brief Summary for MockVP_SoundPlayer.
 *
 * This file includes the declaration of class MockVP_SoundPlayer and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_SOUNDPLAYER_MOCK_H
#define VP_SOUNDPLAYER_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VP_SoundPlayer.h"
#include "VP_DataAccessorList.h"

/**
 * Summary for class MockVP_SoundPlayer
 * 
 * This is a detail description for class MockVP_SoundPlayer
 *
 */
class MockVP_SoundPlayer : public VP_SoundPlayer 
{
public:

    MockVP_SoundPlayer()
    :VP_SoundPlayer(NULL) 
    {
    }

    MOCK_METHOD0(Init, VOID());
    MOCK_METHOD0(StopPlayerThread, void());
    MOCK_METHOD0(Stop, void());
    MOCK_METHOD1(Play, void(std::shared_ptr<VP_DataAccessorList>));
};


#endif /* VP_SOUNDPLAYER_MOCK_H */
/* EOF */
