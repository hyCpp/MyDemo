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

#include "BL_FullFile.h"
#include "TTS_AudioOutThread.h"
#include "VC_AudioOut_mock.h"
#include "TTS_Engine_mock.h"


using ::testing::_;
using ::testing::Return;
using ::testing::ReturnPointee;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::InSequence;
using ::testing::AnyNumber;

/**
* TTS_AudioOutThread
*
* The class is just for TTS_AudioOuThread_Test.
*/
class TTS_AudioOutThread_Test : public testing::Test
{
};

TEST_F(TTS_AudioOutThread_Test, play_buffer_null)
{
    TTS_AudioOutThread audioOut;

    EXPECT_FALSE(audioOut.Play(NULL, 1, 1, TTS_STREAM_DEVICE_OUTPUT_SPEAKER, 1, true));
}

TEST_F(TTS_AudioOutThread_Test, play_audio_null)
{
    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = NULL;

    EXPECT_FALSE(audioOut.Play(NULL, 1, 1, TTS_STREAM_DEVICE_OUTPUT_SPEAKER, 1, true));
}

TEST_F(TTS_AudioOutThread_Test, play_open_audio_false)
{
    VC_AudioOut_mock *mock = new(MEM_Voice) VC_AudioOut_mock(); 
    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = mock;

    EXPECT_CALL(*mock, Open(_, _, _, _)).WillOnce(Return(false));

    TTS_EngineBuffers buffer;
    EXPECT_FALSE(audioOut.Play(&buffer, 1, 1, TTS_STREAM_DEVICE_OUTPUT_SPEAKER, 1, true));
}

TEST_F(TTS_AudioOutThread_Test, play_open_audio_true)
{
    VC_AudioOut_mock *mock = new(MEM_Voice) VC_AudioOut_mock(); 
    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = mock;

    EXPECT_CALL(*mock, Open(_, _, _, _)).WillOnce(Return(true));

    TTS_EngineBuffers buffer;
    EXPECT_TRUE(audioOut.Play(&buffer, 1, 1, TTS_STREAM_DEVICE_OUTPUT_SPEAKER, 1, true));
}

TEST_F(TTS_AudioOutThread_Test, Close)
{
    VC_AudioOut_mock *mock = new(MEM_Voice) VC_AudioOut_mock(); 
    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = mock;

    EXPECT_CALL(*mock, Close()).WillOnce(Return(false));

    EXPECT_FALSE(audioOut.Close());
}

TEST_F(TTS_AudioOutThread_Test, Drain)
{
    VC_AudioOut_mock *mock = new(MEM_Voice) VC_AudioOut_mock(); 
    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = mock;

    EXPECT_CALL(*mock, Drain()).Times(1);

    audioOut.Drain();
}

TEST_F(TTS_AudioOutThread_Test, Flush)
{
    VC_AudioOut_mock *mock = new(MEM_Voice) VC_AudioOut_mock(); 
    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = mock;

    EXPECT_CALL(*mock, Flush()).Times(1);

    audioOut.Flush();
}

TEST_F(TTS_AudioOutThread_Test, StopOutThread)
{
    TTS_AudioOutThread audioOut;
    audioOut.StopOutThread();
}

TEST_F(TTS_AudioOutThread_Test, DoTask_buffer_stop)
{
    TTS_EngineBuffers_mock buffers;
    VC_AudioOut_mock *audio = new VC_AudioOut_mock();

    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = audio;
    audioOut.m_buffer = &buffers;

    unsigned char* p = NULL;
    EXPECT_CALL(buffers, GetReadBuffer(_)).WillOnce(Return(p));
    EXPECT_CALL(buffers, IsStop()).WillOnce(Return(false));
    EXPECT_CALL(*audio, Drain()).Times(AnyNumber());
    EXPECT_CALL(*audio, Close()).Times(1);

    audioOut.DoTask();
}

TEST_F(TTS_AudioOutThread_Test, DoTask_buffer_NULL)
{
    TTS_EngineBuffers_mock buffers;
    VC_AudioOut_mock *audio = new VC_AudioOut_mock();

    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = audio;
    audioOut.m_buffer = &buffers;

    unsigned char* p = NULL;
    EXPECT_CALL(buffers, GetReadBuffer(_)).WillOnce(Return(p));
    EXPECT_CALL(buffers, IsStop()).WillOnce(Return(false));
    EXPECT_CALL(*audio, Drain()).Times(AnyNumber());
    EXPECT_CALL(*audio, Close()).Times(1);

    audioOut.DoTask();
}

TEST_F(TTS_AudioOutThread_Test, DoTask_buffer_size_0)
{
    TTS_EngineBuffers_mock buffers;
    VC_AudioOut_mock *audio = new VC_AudioOut_mock();

    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = audio;
    audioOut.m_buffer = &buffers;

    unsigned char byte;
    int nsize = 0;
    EXPECT_CALL(buffers, GetReadBuffer(_)).WillOnce(DoAll(SetArgReferee<0>(nsize), Return(&byte)));
    EXPECT_CALL(buffers, IsStop()).WillOnce(Return(false));
    EXPECT_CALL(*audio, Drain()).Times(AnyNumber());
    EXPECT_CALL(*audio, Close()).Times(1);

    audioOut.DoTask();
}

TEST_F(TTS_AudioOutThread_Test, DoTask_Write_fail)
{
    TTS_EngineBuffers_mock buffers;
    VC_AudioOut_mock *audio = new VC_AudioOut_mock();

    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = audio;
    audioOut.m_buffer = &buffers;

    unsigned char byte;
    int nsize = 10;
    EXPECT_CALL(buffers, GetReadBuffer(_)).WillOnce(DoAll(SetArgReferee<0>(nsize), Return(&byte)));
    EXPECT_CALL(buffers, IsStop()).WillOnce(Return(false));
    EXPECT_CALL(*audio, Write(_, _)).WillOnce(Return(0));
    EXPECT_CALL(*audio, Drain()).Times(AnyNumber());
    EXPECT_CALL(*audio, Close()).Times(1);

    audioOut.DoTask();
}

#if 0
TEST_F(TTS_AudioOutThread_Test, DoTask_Write_sucess)
{
    TTS_EngineBuffers_mock buffers;
    VC_AudioOut_mock *audio = new VC_AudioOut_mock();

    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = audio;
    audioOut.m_buffer = &buffers;

    unsigned char byte;
    int nsize = 10;
    EXPECT_CALL(buffers, GetReadBuffer(_)).WillOnce(DoAll(SetArgReferee<0>(nsize), Return(&byte)));
    EXPECT_CALL(buffers, IsStop()).WillOnce(Return(false));
    EXPECT_CALL(buffers, SetReadLen(_)).Times(1);
    EXPECT_CALL(*audio, Write(_, _)).WillOnce(Return(10));
    EXPECT_CALL(*audio, Drain()).Times(1);
    EXPECT_CALL(*audio, Close()).Times(1);

    audioOut.DoTask();
}

#endif

TEST_F(TTS_AudioOutThread_Test, DoTask_While_2)
{
    TTS_EngineBuffers_mock buffers;
    VC_AudioOut_mock *audio = new VC_AudioOut_mock();

    TTS_AudioOutThread audioOut;
    audioOut.m_audioOut = audio;
    audioOut.m_buffer = &buffers;

    {
        InSequence s;
        unsigned char byte;
        int nsize = 10;
        EXPECT_CALL(buffers, GetReadBuffer(_)).WillOnce(DoAll(SetArgReferee<0>(nsize), Return(&byte)));
        EXPECT_CALL(buffers, IsStop()).WillOnce(Return(false));
        EXPECT_CALL(*audio, Write(_, _)).WillOnce(Return(10));
        EXPECT_CALL(buffers, SetReadLen(_)).Times(1);
        
        unsigned char* p = NULL;
        EXPECT_CALL(buffers, GetReadBuffer(_)).WillOnce(Return(p));
        EXPECT_CALL(buffers, IsStop()).WillOnce(Return(true));
    }
    EXPECT_CALL(*audio, Drain()).Times(AnyNumber());
    EXPECT_CALL(*audio, Close()).Times(1);

    audioOut.DoTask();
}

/* EOF */
