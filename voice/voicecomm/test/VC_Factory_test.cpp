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
#include "voicecomm_test.h"
#include "VC_Factory.h"

/**
 * Summary for class VC_Factory_Test
 * 
 * This is a detail description for class VC_Factory_Test
 *
 */
class VC_Factory_Test:public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_Factory = VC_Factory::Instance();
        }

        virtual void TearDown()
        {
             if (NULL != m_pVC_Factory) {
                  m_pVC_Factory->Destroy();
                  m_pVC_Factory = NULL;
            }
        }
        
    public:
        VC_Factory  *m_pVC_Factory;
};


// TEST_F(VC_Factory_Test, GetAudioSession)
// {
//    VC_AudioSession* pAudioSession = m_pVC_Factory->GetAudioSession();
//    EXPECT_FALSE(pAudioSession == NULL);
// }

TEST_F(VC_Factory_Test, CreateIAudioIn)
{
    m_pVC_Factory->CreateIAudioIn();
}

TEST_F(VC_Factory_Test, CreateIAudioOut)
{
    m_pVC_Factory->CreateIAudioOut(VC_AUDIO_BEEP);
}

TEST_F(VC_Factory_Test, CreateAudioIn)
{
    m_pVC_Factory->CreateIAudioIn();
}

TEST_F(VC_Factory_Test, CreateAudioOut)
{
    m_pVC_Factory->CreateAudioOut(VC_AUDIO_BEEP);
}

TEST_F(VC_Factory_Test, CreateDecoder_OGG)
{
    VC_Decoder* pDecoder = m_pVC_Factory->CreateDecoder(VC_AUDIO_BEEP, VC_DECODER_OGG);
    EXPECT_FALSE(pDecoder == NULL);
}

TEST_F(VC_Factory_Test, CreateDecoder_TTS)
{
    VC_Decoder* pDecoder = m_pVC_Factory->CreateDecoder(VC_AUDIO_BEEP, VC_DECODER_TTS);
    EXPECT_FALSE(pDecoder == NULL);
}

// TEST_F(VC_Factory_Test, CreateDecoder_Default)
// {
//    VC_Decoder* pDecoder = m_pVC_Factory->CreateDecoder(VC_AUDIO_BEEP, VC_DECODER_TTS + 5);
//    EXPECT_TRUE(pDecoder == NULL);
// }

TEST_F(VC_Factory_Test, CreatePlayer)
{
    VC_Player* pDecoder = m_pVC_Factory->CreatePlayer(VC_AUDIO_BEEP);
    EXPECT_FALSE(pDecoder == NULL);

}
/* EOF */
