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
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "TTS_TextToSpeechProperty.h"

/**
* TTS_TextToSpeechProperty_Test
*
* The class is just for TTS_TextToSpeechProperty test.
*/
class TTS_TextToSpeechProperty_Test:public testing::Test
{
    public:
        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }

    public:
        TTS_TextToSpeechProperty  m_pTTS_TextToSpeechProperty;
};

TEST_F(TTS_TextToSpeechProperty_Test, SetParam_ROLE)
{
    m_pTTS_TextToSpeechProperty.SetParam(TTS_PARAM_ROLE, 0);
}

TEST_F(TTS_TextToSpeechProperty_Test, SetParam_AUDIO_STREAM)
{
    m_pTTS_TextToSpeechProperty.SetParam(TTS_PARAM_AUDIO_STREAM, 0);
}

TEST_F(TTS_TextToSpeechProperty_Test, SetParam_PITCH)
{
    m_pTTS_TextToSpeechProperty.SetParam(TTS_PARAM_PITCH, 0);
}

TEST_F(TTS_TextToSpeechProperty_Test, SetParam_SPEED)
{
    m_pTTS_TextToSpeechProperty.SetParam(TTS_PARAM_SPEED, 0);
}

TEST_F(TTS_TextToSpeechProperty_Test, SetParam_VOLUME)
{
    m_pTTS_TextToSpeechProperty.SetParam(TTS_PARAM_VOLUME, 0);
}

TEST_F(TTS_TextToSpeechProperty_Test, GetParam_ROLE)
{
    m_pTTS_TextToSpeechProperty.GetParam(TTS_PARAM_ROLE);
}

TEST_F(TTS_TextToSpeechProperty_Test, GetParam_AUDIO_STREAM)
{
    m_pTTS_TextToSpeechProperty.GetParam(TTS_PARAM_AUDIO_STREAM);
}

TEST_F(TTS_TextToSpeechProperty_Test, GetParam_PITCH)
{
    m_pTTS_TextToSpeechProperty.GetParam(TTS_PARAM_PITCH);
}

TEST_F(TTS_TextToSpeechProperty_Test, GetParam_SPEED)
{
    m_pTTS_TextToSpeechProperty.GetParam(TTS_PARAM_SPEED);
}

TEST_F(TTS_TextToSpeechProperty_Test, GetParam_VOLUME)
{
    m_pTTS_TextToSpeechProperty.GetParam(TTS_PARAM_VOLUME);
}

TEST_F(TTS_TextToSpeechProperty_Test, opetator)
{
    TTS_TextToSpeechProperty other(m_pTTS_TextToSpeechProperty);

    other = other;
}


/* EOF */
