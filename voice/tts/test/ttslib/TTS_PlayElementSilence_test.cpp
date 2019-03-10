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

#include "stdafx.h"
#include "TTS_PlayElementSilence.h"
#include "gtest/gtest.h"

/**
* TestTTS_PlayElementSilence
*
* The class is just for TTS_PlayElementSilence test.
*/

class TestTTS_PlayElementSilence : public testing::Test
{
public:
    TestTTS_PlayElementSilence() {}
    virtual ~TestTTS_PlayElementSilence() {}

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

public:
};

TEST_F(TestTTS_PlayElementSilence, DoPlay)
{
    TTS_PlayElementSilence elemSilence;
    EXPECT_TRUE(elemSilence.DoPlay(NULL));
}



TEST_F(TestTTS_PlayElementSilence, Stop)
{
    TTS_PlayElementSilence elemSilence;
    elemSilence.Stop();
}
 
/* EOF */
