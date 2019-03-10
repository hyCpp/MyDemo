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
#include "VC_CommonIF.h"
#include "VC_AudioSessionAlsa.h"
#include "VC_AudioOut.h"

/**
 * Summary for class VC_CommonIF_Test
 * 
 * This is a detail description for class VC_CommonIF_Test
 *
 */
class VC_CommonIF_Test:public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_CommonIF = VC_CommonIF::Instance();
        }

        virtual void TearDown()
        {
             if (NULL != m_pVC_CommonIF) {
                  m_pVC_CommonIF->Destroy();
                  m_pVC_CommonIF = NULL;
            }
        }
        
    public:
        VC_CommonIF  *m_pVC_CommonIF;
};



// TEST_F(VC_CommonIF_Test, GetAudioSession)
// {
//    BYTE fBlockID = NM_VMOST_FBLOCK_ID_NAVI;
//    m_pVC_CommonIF->GetAudioSession(fBlockID);
// }

TEST_F(VC_CommonIF_Test, CreateAudioIn)
{
    m_pVC_CommonIF->CreateAudioIn();

}

TEST_F(VC_CommonIF_Test, InstanceNotNULL)
{
    VC_CommonIF* commif = m_pVC_CommonIF->Instance();
    VC_CommonIF* commif2 = m_pVC_CommonIF->Instance();
}

TEST_F(VC_CommonIF_Test, DestroyNULL)
{
    m_pVC_CommonIF->Destroy();
}

TEST_F(VC_CommonIF_Test, GetAudioSessionNULL)
{
    VC_AudioSession* audiosession = m_pVC_CommonIF->GetAudioSession(VC_MODULE_NAVI);

    EXPECT_TRUE(NULL == audiosession);
}

TEST_F(VC_CommonIF_Test, GetAudioSessionm_pAudioSessionNotNULL)
{
    m_pVC_CommonIF->m_pAudioSession = new VC_AudioSessionAlsa(1);
    VC_AudioSession* audiosession = m_pVC_CommonIF->GetAudioSession(VC_MODULE_NAVI);

    EXPECT_TRUE(NULL != audiosession);
}

// TEST_F(VC_CommonIF_Test, GetAudioSessionm_pAudioSessionNotNULLfactoryNULL)
// {
//     m_pVC_CommonIF->m_pAudioSession = new VC_AudioSession();

//     EXPECT_TRUE(NULL != audiosession);
// }

TEST_F(VC_CommonIF_Test, CreateAudioOut)
{
    VC_AudioOut* audioout = m_pVC_CommonIF->CreateAudioOut(VC_AUDIO_BEEP);

    EXPECT_FALSE(NULL == audioout);
    delete audioout;
    audioout = NULL;
}

TEST_F(VC_CommonIF_Test, CreatePlayer)
{
    VC_Player* vcplayer = m_pVC_CommonIF->CreatePlayer(VC_AUDIO_BEEP);

    EXPECT_FALSE(NULL == vcplayer);
    vcplayer = NULL;
}

TEST_F(VC_CommonIF_Test, GetAudioOutNotExist)
{
    VC_AudioOut* audioout = m_pVC_CommonIF->GetAudioOut(VC_AUDIO_BEEP);

    EXPECT_FALSE(NULL == audioout);
    audioout = NULL;
}

TEST_F(VC_CommonIF_Test, GetAudioOutExist)
{
    VC_AudioOut* audioout = m_pVC_CommonIF->GetAudioOut(VC_AUDIO_BEEP);
    VC_AudioOut* audioout2 = m_pVC_CommonIF->GetAudioOut(VC_AUDIO_BEEP);

    EXPECT_FALSE(NULL == audioout);
    EXPECT_FALSE(NULL == audioout2);
    audioout = NULL;
    audioout2 = NULL;
}

TEST_F(VC_CommonIF_Test, Destruct)
{
    m_pVC_CommonIF->m_pAudioSession = new(MEM_Voice) VC_AudioSessionAlsa(10);
}

/* EOF */
