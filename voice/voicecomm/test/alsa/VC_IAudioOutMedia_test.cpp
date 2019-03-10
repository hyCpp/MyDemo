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
 * @file VC_IAudioOutMedia_Test                                         
 * @brief Declaration file of class VC_IAudioOutMedia_Test.                  
 *
 * This file includes the declaration of test googletest and googlemock.
 *
 * @attention used for C++ only.
 */

#include "voicecomm_test.h"

#include "stdafx.h"
#include "VC_IAudioOutMedia.h"
#include "VC_AudioBufferGeneral.h"
#include "NMAudioStream_mock.h"
#include "CL_WaitObj.h"

using ::testing::Return;
using ::testing::_;

/**
* VC_IAudioOutMedia_Test
*
* The class is just for IAudioOutMedia test.
*/

/**
 * Summary for class VC_IAudioOutMedia_Test
 * 
 * This is a detail description for class VC_IAudioOutMedia_Test
 *
 */
class VC_IAudioOutMedia_Test:public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_IAudioOutMedia = new(MEM_Voice) VC_IAudioOutMedia();
            delete m_pVC_IAudioOutMedia->m_stream;
            testMock = new(MEM_Voice) Mock_NMAudioStream();
            m_pVC_IAudioOutMedia->m_stream = testMock;
        }

        virtual void TearDown()
        {
             if (NULL != m_pVC_IAudioOutMedia) {
                 delete m_pVC_IAudioOutMedia;
                 m_pVC_IAudioOutMedia = NULL;
            }
        }

    public:
        VC_IAudioOutMedia  *m_pVC_IAudioOutMedia;
        Mock_NMAudioStream *testMock;
};

TEST_F(VC_IAudioOutMedia_Test, Open_NullCheck)
{
    int channels = 1;
    CL_BOOL err = m_pVC_IAudioOutMedia->Open(NULL, channels);
    EXPECT_EQ(err, CL_FALSE);
}

TEST_F(VC_IAudioOutMedia_Test, OpenFalse)
{
    int sampleRate = 16000;
    int channels = 1;
    EXPECT_CALL(*testMock, create(_)).WillRepeatedly(Return(-6));
    CL_BOOL err = m_pVC_IAudioOutMedia->Open(&sampleRate, channels);
    EXPECT_EQ(err, CL_FALSE);
}

TEST_F(VC_IAudioOutMedia_Test, Open)
{
    int sampleRate = 16000;
    int channels = 1;
    EXPECT_CALL(*testMock, create(_)).WillRepeatedly(Return(1));
    CL_BOOL err = m_pVC_IAudioOutMedia->Open(&sampleRate, channels);
    EXPECT_EQ(err, CL_TRUE);
}

TEST_F(VC_IAudioOutMedia_Test, WriteFalse)
{
    EXPECT_CALL(*testMock, write(_, _)).WillRepeatedly(Return(2));
    char buf[] = "abcde";
    int ret = m_pVC_IAudioOutMedia->Write(&buf, sizeof(buf));
    EXPECT_EQ(ret, -1);
}

TEST_F(VC_IAudioOutMedia_Test, Write)
{
    EXPECT_CALL(*testMock, write(_, _)).WillRepeatedly(Return(6));
    char buf[] = "abcde";
    int len = m_pVC_IAudioOutMedia->Write(&buf, sizeof(buf));
    EXPECT_EQ(len, 6);
}

TEST_F(VC_IAudioOutMedia_Test, Write_m_nWriteCountNotZero)
{
    EXPECT_CALL(*testMock, write(_, _)).WillRepeatedly(Return(6));
    char buf[] = "abcde";
    m_pVC_IAudioOutMedia->m_nWriteCount = 10;
    int len = m_pVC_IAudioOutMedia->Write(&buf, sizeof(buf));
    EXPECT_EQ(len, 6);
}

TEST_F(VC_IAudioOutMedia_Test, Stop_true)
{
    EXPECT_CALL(*testMock, flush()).WillRepeatedly(Return(1));
    CL_BOOL err = m_pVC_IAudioOutMedia->Stop(CL_TRUE);
    EXPECT_EQ(err, CL_TRUE);
}

TEST_F(VC_IAudioOutMedia_Test, Stop_false)
{
    EXPECT_CALL(*testMock, drain()).WillRepeatedly(Return(1));
    CL_BOOL err = m_pVC_IAudioOutMedia->Stop(CL_FALSE);
    EXPECT_EQ(err, CL_TRUE);
}

TEST_F(VC_IAudioOutMedia_Test, Close)
{
    EXPECT_CALL(*testMock, destroy()).WillRepeatedly(Return(1));
    CL_BOOL err = m_pVC_IAudioOutMedia->Close();
    EXPECT_EQ(err, CL_TRUE);
}


TEST_F(VC_IAudioOutMedia_Test, Drain)
{
    EXPECT_CALL(*testMock, drain()).WillRepeatedly(Return(1));
    m_pVC_IAudioOutMedia->Drain();
}

TEST_F(VC_IAudioOutMedia_Test, Flush)
{
    EXPECT_CALL(*testMock, flush()).WillRepeatedly(Return(1));
    m_pVC_IAudioOutMedia->Flush();
}

TEST_F(VC_IAudioOutMedia_Test, Resume)
{
    m_pVC_IAudioOutMedia->Resume();
}

TEST_F(VC_IAudioOutMedia_Test, destructNullMStream)
{
    m_pVC_IAudioOutMedia->m_stream = NULL;
}

TEST_F(VC_IAudioOutMedia_Test, destructNullMConfig)
{
    m_pVC_IAudioOutMedia->m_config = NULL;
}

TEST_F(VC_IAudioOutMedia_Test, OpenNullMStream)
{
    m_pVC_IAudioOutMedia->m_stream = NULL;

    int sampleRate = 16000;
    int channels = 1;

    CL_BOOL err = m_pVC_IAudioOutMedia->Open(&sampleRate, channels);
    EXPECT_EQ(err, CL_FALSE);
}

TEST_F(VC_IAudioOutMedia_Test, OpenNullMConfig)
{
    m_pVC_IAudioOutMedia->m_config = NULL;

    int sampleRate = 16000;
    int channels = 1;

    CL_BOOL err = m_pVC_IAudioOutMedia->Open(&sampleRate, channels);
    EXPECT_EQ(err, CL_FALSE);
}

TEST_F(VC_IAudioOutMedia_Test, Pause)
{
    m_pVC_IAudioOutMedia->Pause();
}

/* EOF */