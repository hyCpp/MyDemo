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

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "stdafx.h"
#include "VC_CommDef.h"
#include "TTS_PlayThread.h"
#include "TTS_EngineManager.h"
#include "TTS_PlayElementText.h"
#include <time.h>
#include "BL_FullFile.h"
#include "VC_AudioOut_mock.h"
#include "VC_AudioBufferGeneral.h"
#include "TTS_PlayThread_test.h"
#include "TTS_Engine_mock.h"

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgPointee;


/**
* TTS_PlayElementText_Test
*
* The class is just for TTS_PlayElementText test.
*/
class TTS_PlayElementText_Test:public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TTS_PlayElementText_Test, DoPlay)
{
    MockTTS_Engine *engine = new(MEM_Voice) MockTTS_Engine();
    TTS_EngineManager manager;
    manager.m_defaultEngine = engine; 
    TTS_PlayElementText elemText(&manager, "test", "test", "test", "test");

    EXPECT_CALL(*engine, SynthesizeText(_, _, _, _)).Times(1);

    TTS_EngineBuffers buffer;
    elemText.DoPlay(&buffer);
}

TEST_F(TTS_PlayElementText_Test, DoPlay_buff_null)
{
    MockTTS_Engine *engine = new(MEM_Voice) MockTTS_Engine();
    TTS_EngineManager manager;
    manager.m_defaultEngine = engine; 
    TTS_PlayElementText elemText(&manager, "test", "test", "test", "test");

    elemText.DoPlay(NULL);
}

TEST_F(TTS_PlayElementText_Test, DoPlay_engine_null)
{
    TTS_EngineManager manager;
    manager.m_defaultEngine = NULL; 
    TTS_PlayElementText elemText(&manager, "test", "test", "test", "test");

    TTS_EngineBuffers buffer;
    elemText.DoPlay(&buffer);
}

TEST_F(TTS_PlayElementText_Test, GetSampleRate)
{
    MockTTS_Engine *engine = new(MEM_Voice) MockTTS_Engine(); 
    TTS_EngineManager manager;
    manager.m_defaultEngine = engine; 
    TTS_PlayElementText elemText(&manager, "test", "test", "test", "test");

    EXPECT_CALL(*engine, GetSampleRate()).Times(1);

    elemText.GetSampleRate();
}

/* EOF */
