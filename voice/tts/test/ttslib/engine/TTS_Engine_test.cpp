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

#include "TTS_Engine.h"

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgPointee;

/**
* TTS_PlayElementText_Test
*
* The class is just for TTS_PlayElementText test.
*/
class TTS_EngineBuffers_Test:public testing::Test
{
};

TEST_F(TTS_EngineBuffers_Test, NewPlayer)
{
    TTS_EngineBuffers buffer;
    buffer.NewPlayer();
}

TEST_F(TTS_EngineBuffers_Test, GetWriteBuffer_right)
{
    TTS_EngineBuffers buffer;
    int len;
    buffer.GetWriteBuffer(len);

    EXPECT_EQ(len, 4096);
}

TEST_F(TTS_EngineBuffers_Test, GetWriteBuffer_buff_limit)
{
    TTS_EngineBuffers buffer;
    buffer.m_writePos = 4096 * 3 + 1000;
    int len;
    buffer.GetWriteBuffer(len);

    EXPECT_EQ(len, 3096);
}

TEST_F(TTS_EngineBuffers_Test, GetWriteBuffer_stop)
{
    TTS_EngineBuffers buffer;
    int len;
    buffer.m_stop = true;
    buffer.GetWriteBuffer(len);

}

TEST_F(TTS_EngineBuffers_Test, GetWriteBuffer_finish)
{
    TTS_EngineBuffers buffer;
    int len;
    buffer.m_phraseFinish = true;
    buffer.GetWriteBuffer(len);

}

TEST_F(TTS_EngineBuffers_Test, GetWriteBuffer_no_buff_stop)
{
    TTS_EngineBuffers buffer;
    buffer.m_writePos = 4096 * 4;
    buffer.m_stop = true;
    int len;
    buffer.GetWriteBuffer(len);
}

TEST_F(TTS_EngineBuffers_Test, GetWriteBuffer_no_buff_finish)
{
    TTS_EngineBuffers buffer;
    buffer.m_writePos = 4096 * 4;
    buffer.m_phraseFinish = true;
    int len;
    buffer.GetWriteBuffer(len);
}

TEST_F(TTS_EngineBuffers_Test, GetReadBuffer_right)
{
    TTS_EngineBuffers buffer;
    buffer.m_writePos = 4096;
    int len;
    buffer.GetReadBuffer(len);

    EXPECT_EQ(len, 4096);
}

TEST_F(TTS_EngineBuffers_Test, GetReadBuffer_right_long)
{
    TTS_EngineBuffers buffer;
    buffer.m_writePos = 4096 * 2;
    int len;
    buffer.GetReadBuffer(len);

    EXPECT_EQ(len, 4096);
}

TEST_F(TTS_EngineBuffers_Test, GetReadBuffer_stop)
{
    TTS_EngineBuffers buffer;
    int len;
    buffer.m_stop = true;
    buffer.GetReadBuffer(len);
}

TEST_F(TTS_EngineBuffers_Test, GetReadBufferf_finish)
{
    TTS_EngineBuffers buffer;
    int len;
    buffer.m_phraseFinish = true;
    buffer.GetReadBuffer(len);
}

TEST_F(TTS_EngineBuffers_Test, SetWriteLen)
{
    TTS_EngineBuffers buffer;
    buffer.SetWriteLen(1);
}

TEST_F(TTS_EngineBuffers_Test, SetReadLen)
{
    TTS_EngineBuffers buffer;
    buffer.SetReadLen(1);
}

TEST_F(TTS_EngineBuffers_Test, Stop)
{
    TTS_EngineBuffers buffer;
    buffer.Stop();
}

TEST_F(TTS_EngineBuffers_Test, FinishPhrase)
{
    TTS_EngineBuffers buffer;
    buffer.FinishPhrase();
}

TEST_F(TTS_EngineBuffers_Test, IsStop)
{
    TTS_EngineBuffers buffer;
    buffer.IsStop();
}

/* EOF */
