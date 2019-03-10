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


#include "CL_NewTypesDefine.h"
#include "VC_Factory.h"
#include "stdafx.h"

#include "VC_AudioSessionAlsa.h"
#include "VC_IAudioOutMedia.h"
#include "VC_DecoderOgg.h"
#include "VC_AudioInGeneral.h"
#include "VC_AudioOutGeneral.h"
#include "VC_PlayerGeneral.h"

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

TEST_F(VC_Factory_Test, CreateAudioSession_Navi)
{
    VC_AudioSession* pAudioSession = m_pVC_Factory->CreateAudioSession(VC_MODULE_NAVI);
    EXPECT_TRUE(pAudioSession == NULL);
}

TEST_F(VC_Factory_Test, CreateAudioSession_Vr)
{
    VC_AudioSession* pAudioSession = m_pVC_Factory->CreateAudioSession(VC_MODULE_VR);
    EXPECT_TRUE(pAudioSession == NULL);
}

TEST_F(VC_Factory_Test, CreateAudioSession_Default)
{
    VC_AudioSession* pAudioSession = m_pVC_Factory->CreateAudioSession(VC_MODULE_TYPE(-1));
    EXPECT_TRUE(pAudioSession == NULL);
}

TEST_F(VC_Factory_Test, CreateAudioSession_TTS)
{
    VC_AudioSession* pAudioSession = m_pVC_Factory->CreateAudioSession(VC_MODULE_TTS);
}

TEST_F(VC_Factory_Test, CreateIAudioInMedia)
{
    VC_IAudioIn* pIAudioIn = m_pVC_Factory->CreateIAudioIn();
    EXPECT_TRUE(NULL != pIAudioIn);
    delete pIAudioIn;
}

TEST_F(VC_Factory_Test, CreateIAudioInFile)
{
    VC_IAudioIn* pIAudioIn = m_pVC_Factory->CreateIAudioIn(true);
    EXPECT_TRUE(NULL != pIAudioIn);
    delete pIAudioIn;
}

TEST_F(VC_Factory_Test, CreateIAudioOut)
{
    VC_IAudioOut* pIAudioOut = m_pVC_Factory->CreateIAudioOut(VC_AUDIO_BEEP);
    EXPECT_TRUE(NULL != pIAudioOut);
    delete pIAudioOut;
}

TEST_F(VC_Factory_Test, CreateAudioIn)
{
    VC_AudioIn* pAudioIn = m_pVC_Factory->CreateAudioIn();
    EXPECT_TRUE(NULL != pAudioIn);
    delete pAudioIn;
}

TEST_F(VC_Factory_Test, CreateAudioOut)
{
    VC_AudioOut* pAudioOut = m_pVC_Factory->CreateAudioOut(VC_AUDIO_BEEP);
    EXPECT_TRUE(NULL != pAudioOut);
    delete pAudioOut;
}

TEST_F(VC_Factory_Test, CreateDecoder_OGG)
{
    VC_Decoder* pDecoder = m_pVC_Factory->CreateDecoder(VC_AUDIO_BEEP, VC_DECODER_OGG);
    EXPECT_FALSE(pDecoder == NULL);
    delete pDecoder;
}

TEST_F(VC_Factory_Test, CreateDecoder_TTS)
{
    VC_Decoder* pDecoder = m_pVC_Factory->CreateDecoder(VC_AUDIO_BEEP, VC_DECODER_OGG);
    EXPECT_FALSE(pDecoder == NULL);
}


TEST_F(VC_Factory_Test, CreateDecoder_Default)
{
    VC_Decoder* pDecoder = m_pVC_Factory->CreateDecoder(VC_AUDIO_BEEP, VC_DECODER_NUM);
    EXPECT_TRUE(pDecoder == NULL);
}

TEST_F(VC_Factory_Test, CreatePlayer)
{
    VC_Player* pPlayer = m_pVC_Factory->CreatePlayer(VC_AUDIO_BEEP);
    EXPECT_FALSE(pPlayer == NULL);
    delete pPlayer;

}

TEST_F(VC_Factory_Test, DestroyNullInstance)
{
    m_pVC_Factory->Destroy();
}

/* EOF */
