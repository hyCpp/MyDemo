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
 * @file VC_IAudioInMedia_Test                                         
 * @brief Declaration file of class VC_IAudioInMedia_Test.                  
 *
 * This file includes the declaration of test class VC_IAudioInMedia_Test and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#include "voicecomm_test.h"
#include "stdafx.h"
#include "VC_IAudioInMedia.h"
#include "VC_AudioSession.h"
#include "VC_Factory.h"
#ifndef CXX_CL_WAITOBJ_H
#   include "CL_WaitObj.h"
#endif
#include "VC_AudioBufferGeneral.h"
#include "NMAudioStream_mock.h"
#include <stdio.h>

using ::testing::Return;
using ::testing::_;

/**
* VC_IAudioInMedia_Test
*
* The class is just for IAudioInMedia test.
*/

/**
 * Summary for class VC_IAudioInMedia_Test
 * 
 * This is a detail description for class VC_IAudioInMedia_Test
 *
 */
class VC_IAudioInMedia_Test : public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_IAudioInMedia = new(MEM_Voice) VC_IAudioInMedia();
            delete m_pVC_IAudioInMedia->m_pStream;
            testMock = new(MEM_Voice) Mock_NMAudioStream();
            m_pVC_IAudioInMedia->m_pStream = testMock;
        }

        virtual void TearDown()
        {
             if (NULL != m_pVC_IAudioInMedia) {
                  delete m_pVC_IAudioInMedia;
                  m_pVC_IAudioInMedia = NULL;
            }
        }

    public:
        VC_IAudioInMedia  *m_pVC_IAudioInMedia;
        Mock_NMAudioStream *testMock;
};

TEST_F(VC_IAudioInMedia_Test, Open_false)
{
    int sampleRate = 16000;
    EXPECT_CALL(*testMock, create(_)).WillRepeatedly(Return(-1));
    CL_BOOL err = m_pVC_IAudioInMedia->Open(sampleRate, VC_AUDIO_MODE_BARGEIN);
    EXPECT_EQ(err, CL_FALSE);
}

TEST_F(VC_IAudioInMedia_Test, Open)
{
    INT sampleRate = 16000;
    EXPECT_CALL(*testMock, create(_)).WillRepeatedly(Return(1));
    CL_BOOL err = m_pVC_IAudioInMedia->Open(sampleRate, VC_AUDIO_MODE_BARGEIN);
    EXPECT_EQ(err, CL_TRUE);             
}

TEST_F(VC_IAudioInMedia_Test, OpenNormalMode)
{
    INT sampleRate = 16000;
    EXPECT_CALL(*testMock, create(_)).WillRepeatedly(Return(1));
    CL_BOOL err = m_pVC_IAudioInMedia->Open(sampleRate, VC_AUDIO_MODE_NORMAL);
    EXPECT_EQ(err, CL_TRUE);             
}

TEST_F(VC_IAudioInMedia_Test, OpenStreamNull)
{

    m_pVC_IAudioInMedia->m_pStream = NULL;
    EXPECT_FALSE(m_pVC_IAudioInMedia->Open(1, VC_AUDIO_MODE_NORMAL));
}

TEST_F(VC_IAudioInMedia_Test, OpenConfigNULL)
{
    m_pVC_IAudioInMedia->m_pConfig = NULL;
    EXPECT_FALSE(m_pVC_IAudioInMedia->Open(1, VC_AUDIO_MODE_NORMAL));
}


TEST_F(VC_IAudioInMedia_Test, Read_StateCheck)
{
    m_pVC_IAudioInMedia->m_state = VC_IAudioInMedia::ST_NONE;
    char buf[] = "abcde"; 
    int len = m_pVC_IAudioInMedia->Read(&buf, sizeof(buf));
    EXPECT_EQ(len, -1);              
}

TEST_F(VC_IAudioInMedia_Test, Read)
{
    m_pVC_IAudioInMedia->m_state = VC_IAudioInMedia::ST_OPENED;
    EXPECT_CALL(*testMock, read(_, _)).WillRepeatedly(Return(6));
    char buf[] = "abcde"; 
    int len = m_pVC_IAudioInMedia->Read(&buf, sizeof(buf));
    EXPECT_EQ(len, 6);      
}

TEST_F(VC_IAudioInMedia_Test, Stop)
{
    CL_BOOL err = m_pVC_IAudioInMedia->Stop();
    EXPECT_EQ(err, CL_TRUE);
}

TEST_F(VC_IAudioInMedia_Test, CloseNULLBargeInData)
{
    EXPECT_CALL(*testMock, destroy()).WillRepeatedly(Return(1));
    CL_BOOL err = m_pVC_IAudioInMedia->Close();
    EXPECT_EQ(err, CL_TRUE);
}

TEST_F(VC_IAudioInMedia_Test, CloseNULLStream)
{
    m_pVC_IAudioInMedia->m_pStream = NULL;
    CL_BOOL err = m_pVC_IAudioInMedia->Close();
    EXPECT_EQ(err, CL_TRUE);
}


TEST_F(VC_IAudioInMedia_Test, Close)
{
    EXPECT_CALL(*testMock, destroy()).WillRepeatedly(Return(1));
    m_pVC_IAudioInMedia->m_pBargeInData = new(MEM_Voice) char[10];
    CL_BOOL err = m_pVC_IAudioInMedia->Close();
    EXPECT_EQ(err, CL_TRUE);
}

TEST_F(VC_IAudioInMedia_Test,  GetParam)
{
    int i = 0;
    int size = 10;

    int* psize = &size;
    // char* buffer= new char[10];
    // memset(buffer, 0, strlen(buffer));
    // strcpy(buffer, "mfb");

    m_pVC_IAudioInMedia->m_bargeInDataSize = 11;

    EXPECT_TRUE(m_pVC_IAudioInMedia->GetParam(i, (void**)&psize, &size));

}

TEST_F(VC_IAudioInMedia_Test,  GetParamFalse)
{
    int i = 0;
    int size = 10;
    // char* buffer= new char[10];
    // memset(buffer, 0, strlen(buffer));
    // strcpy(buffer, "mfb");

    int* psize = &size;

    m_pVC_IAudioInMedia->m_bargeInDataSize = 0;

    EXPECT_FALSE(m_pVC_IAudioInMedia->GetParam(i, (void**)&psize, &size));
    
}


/* EOF */
