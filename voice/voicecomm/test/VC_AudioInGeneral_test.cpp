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
#define private public
#define protected public

#include "voicecomm_test.h"

#include "VC_IAudioInMedia_mock.h"
#include "VC_CommonDef.h"
#include "VC_AudioInGeneral.h"
#include "VC_Factory_mock.h"

#include "CL_Thread.h"

using ::testing::Return;
using ::testing::_;

/**
 * Summary for class VC_AudioInGeneral_Test
 * 
 * This is a detail description for class VC_AudioInGeneral_Test
 *
 */
class VC_AudioInGeneral_Test : public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_AudioInGeneral = new(MEM_Voice) VC_AudioInGeneral();
        }

        virtual void TearDown()
        {
             if (NULL != m_pVC_AudioInGeneral) {
                  delete m_pVC_AudioInGeneral;
                  m_pVC_AudioInGeneral = NULL;
            }
        }

    public:
        VC_AudioInGeneral  *m_pVC_AudioInGeneral;
};

/**
 * Summary for class VC_AudioInGeneral_Test
 * 
 * This is a detail description for class VC_AudioInGeneral_Test
 *
 */
class Lis_test: public VC_AudioIn::Listener
{
private:
    Lis_test() {}
    virtual VOID OnAudioInData(VOID* data, INT len) {}
};

TEST_F(VC_AudioInGeneral_Test, Start_NotIdle)
{
    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();    
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_NONE;
    CL_BOOL nbool = m_pVC_AudioInGeneral->Start(pListener);
    EXPECT_EQ(CL_FALSE, nbool);
    delete pListener;
}

TEST_F(VC_AudioInGeneral_Test, Start_NullCheck)
{
    VC_AudioIn::Listener* pListener = NULL;
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_NONE;
    CL_BOOL nbool = m_pVC_AudioInGeneral->Start(pListener);
    EXPECT_EQ(CL_FALSE, nbool);
}

TEST_F(VC_AudioInGeneral_Test, StartNotify)
{
    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_IDLE;
    CL_BOOL nbool = m_pVC_AudioInGeneral->Start(pListener);
    EXPECT_EQ(CL_TRUE, nbool);
    delete pListener;
}


/*
TEST_F(VC_AudioInGeneral_Test, Start_Async)
{
    MockVC_IAudioInMedia audioMedia;
    EXPECT_CALL(audioMedia, IsAsync()).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioMedia, StartAsync(_)).WillOnce(Return(CL_TRUE));
    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_IDLE;
    EXPECT_EQ(VC_AudioInGeneral::ST_IDLE, m_pVC_AudioInGeneral->m_state);
    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();
    EXPECT_TRUE(NULL != pListener);
    EXPECT_TRUE(m_pVC_AudioInGeneral->Start(pListener));
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;
    m_pVC_AudioInGeneral->m_pListener = NULL;
    delete pListener;
}

TEST_F(VC_AudioInGeneral_Test, Start_Sync)
{
    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_IDLE;
    MockVC_IAudioInMedia audioMedia;
    EXPECT_CALL(audioMedia, IsAsync()).WillOnce(Return(CL_FALSE));
    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    EXPECT_TRUE(m_pVC_AudioInGeneral->Start(pListener));
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;
    m_pVC_AudioInGeneral->m_pListener = NULL;
    delete pListener;
}
*/

TEST_F(VC_AudioInGeneral_Test, Open_Sample)
{
    CL_BOOL err = m_pVC_AudioInGeneral->Open(NULL, NULL, NULL, 0);
    EXPECT_EQ(CL_FALSE, err);
}

TEST_F(VC_AudioInGeneral_Test, Open_State)
{
    INT sampleRate = 0;
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_RUNNING;
    CL_BOOL err = m_pVC_AudioInGeneral->Open(&sampleRate, NULL, NULL, 0);
    EXPECT_EQ(CL_FALSE, err);
}

 TEST_F(VC_AudioInGeneral_Test, Open_NullCheck)
 {
     INT sampleRate = 0;
     INT fragSize = 0;
     INT fragCount = 0;
     m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_NONE;
     m_pVC_AudioInGeneral->m_pAudioIn = NULL;
     CL_BOOL err = m_pVC_AudioInGeneral->Open(&sampleRate, &fragSize, &fragCount, 0);
     EXPECT_EQ(CL_FALSE, err);
 }

TEST_F(VC_AudioInGeneral_Test, Openm_pAudioInNotNULL)
{
    MockVC_IAudioInMedia audioMedia;
    EXPECT_CALL(audioMedia, Open(_, _)).WillOnce(Return(CL_TRUE));
    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_NONE;
    INT sampleRate = 0;
    INT fragSize = 0;
    INT fragCount = 0;
    EXPECT_TRUE(m_pVC_AudioInGeneral->Open(&sampleRate, &fragSize, &fragCount, 0));
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;
}

TEST_F(VC_AudioInGeneral_Test, Openm_pAudioInNULL)
{
    MockVC_IAudioInMedia audioMedia;
    // EXPECT_CALL(audioMedia, Open(_, _)).WillOnce(Return(CL_TRUE));
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_NONE;
    INT sampleRate = 0;
    INT fragSize = 0;
    INT fragCount = 0;
    EXPECT_FALSE(m_pVC_AudioInGeneral->Open(&sampleRate, &fragSize, &fragCount, 0));
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;
}

TEST_F(VC_AudioInGeneral_Test, Openm_pAudioInCreateNULL)
{
    MockVC_IAudioInMedia audioMedia;
    // EXPECT_CALL(audioMedia, Open(_, _)).WillOnce(Return(CL_TRUE));
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_NONE;
    Mock_VCFactory* mockfactory = new Mock_VCFactory();
    VC_Factory::s_pInstance = mockfactory;

    VC_IAudioInMedia* iaudioinmedia = NULL;
    EXPECT_CALL(*mockfactory, CreateIAudioIn(_)).WillRepeatedly(Return(iaudioinmedia));

    std::cout << "in null______________________" << std::endl;
    INT sampleRate = 0;
    INT fragSize = 0;
    INT fragCount = 0;
    EXPECT_FALSE(m_pVC_AudioInGeneral->Open(&sampleRate, &fragSize, &fragCount, 0));

    delete mockfactory;
    mockfactory = NULL;
    VC_Factory::s_pInstance = NULL;
}


TEST_F(VC_AudioInGeneral_Test, Stop_StateCheck)
{
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_NONE;
    CL_BOOL nbool = m_pVC_AudioInGeneral->Stop(true);
    EXPECT_EQ(CL_FALSE, nbool);
}

TEST_F(VC_AudioInGeneral_Test, StopTrue)
{
    m_pVC_AudioInGeneral->m_waitStop.Notify();
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_RUNNING;
    CL_BOOL nbool = m_pVC_AudioInGeneral->Stop(true);
    EXPECT_EQ(CL_TRUE, nbool);
}

TEST_F(VC_AudioInGeneral_Test, Stop)
{
    m_pVC_AudioInGeneral->m_waitStop.Notify();
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_RUNNING;
    CL_BOOL nbool = m_pVC_AudioInGeneral->Stop(false);
    EXPECT_EQ(CL_TRUE, nbool);
}

TEST_F(VC_AudioInGeneral_Test, Close_NullCheck)
{
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;
    CL_BOOL nbool = m_pVC_AudioInGeneral->Close();
    EXPECT_EQ(CL_TRUE, nbool);
}

TEST_F(VC_AudioInGeneral_Test, Close)
{
    MockVC_IAudioInMedia audioMedia;
    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    EXPECT_CALL(audioMedia, Close()).WillOnce(Return(CL_TRUE));
    EXPECT_TRUE(m_pVC_AudioInGeneral->Close());
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;
}

TEST_F(VC_AudioInGeneral_Test, CloseFalse)
{
    MockVC_IAudioInMedia audioMedia;
    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    EXPECT_CALL(audioMedia, Close()).WillOnce(Return(CL_FALSE));
    EXPECT_FALSE(m_pVC_AudioInGeneral->Close());
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;
}

TEST_F(VC_AudioInGeneral_Test, Closem_bAsyncTrue)
{
    MockVC_IAudioInMedia audioMedia;
    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    m_pVC_AudioInGeneral->m_bAsync = true;
    EXPECT_TRUE(m_pVC_AudioInGeneral->Close());
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;
}

TEST_F(VC_AudioInGeneral_Test, DoTask)
{
    MockVC_IAudioInMedia audioMedia;
    // EXPECT_CALL(audioMedia, Start()).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioMedia, Read(_, _)).WillOnce(Return(-1));
    EXPECT_CALL(audioMedia, Stop()).WillOnce(Return(CL_TRUE));

    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();
    m_pVC_AudioInGeneral->m_pListener = pListener;

    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    m_pVC_AudioInGeneral->m_bStop = CL_FALSE;
    m_pVC_AudioInGeneral->m_nAudioMode = VC_AUDIO_MODE_NORMAL;
    m_pVC_AudioInGeneral->DoTask();
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;

}

TEST_F(VC_AudioInGeneral_Test, DoTaskm_nAudioModeNotNormal)
{
    m_pVC_AudioInGeneral->m_nAudioMode = VC_AUDIO_MODE_BARGEIN;

    MockVC_IAudioInMedia audioMedia;
    // EXPECT_CALL(audioMedia, Start()).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioMedia, Read(_, _)).WillOnce(Return(-1));
    EXPECT_CALL(audioMedia, Stop()).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioMedia, GetParam(_, _, _)).WillOnce(Return(true));

    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();
    m_pVC_AudioInGeneral->m_pListener = pListener;

    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    m_pVC_AudioInGeneral->m_bStop = CL_FALSE;
    m_pVC_AudioInGeneral->DoTask();
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;

}

TEST_F(VC_AudioInGeneral_Test, DoTaskm_bStopisTrue)
{
    m_pVC_AudioInGeneral->m_nAudioMode = VC_AUDIO_MODE_NORMAL;

    MockVC_IAudioInMedia audioMedia;
    EXPECT_CALL(audioMedia, Stop()).WillOnce(Return(CL_TRUE));

    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();
    m_pVC_AudioInGeneral->m_pListener = pListener;

    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    m_pVC_AudioInGeneral->m_bStop = CL_TRUE;
    m_pVC_AudioInGeneral->DoTask();
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;

}

TEST_F(VC_AudioInGeneral_Test, DoTaskm_bAsyncTrue)
{
    m_pVC_AudioInGeneral->m_nAudioMode = VC_AUDIO_MODE_NORMAL;

    MockVC_IAudioInMedia audioMedia;
    EXPECT_CALL(audioMedia, Stop()).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioMedia, Close()).WillOnce(Return(CL_TRUE));

    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();
    m_pVC_AudioInGeneral->m_pListener = pListener;

    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    m_pVC_AudioInGeneral->m_bStop = CL_TRUE;
    m_pVC_AudioInGeneral->m_bAsync = CL_TRUE;
    m_pVC_AudioInGeneral->DoTask();
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;

}

TEST_F(VC_AudioInGeneral_Test, DoTaskm_nAudioModeNotNormalLenFirstBiggerthanZeroThenSmallerThanZero)
{
    m_pVC_AudioInGeneral->m_nAudioMode = VC_AUDIO_MODE_BARGEIN;

    MockVC_IAudioInMedia audioMedia;
    // EXPECT_CALL(audioMedia, Start()).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioMedia, Read(_, _)).Times(2).WillOnce(Return(1)).WillOnce(Return(-1));
    EXPECT_CALL(audioMedia, Stop()).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioMedia, GetParam(_, _, _)).WillOnce(Return(true));

    VC_AudioIn::Listener* pListener = new(MEM_Voice) Lis_test();
    m_pVC_AudioInGeneral->m_pListener = pListener;

    m_pVC_AudioInGeneral->m_pAudioIn = &audioMedia;
    m_pVC_AudioInGeneral->m_bStop = CL_FALSE;
    m_pVC_AudioInGeneral->DoTask();
    m_pVC_AudioInGeneral->m_pAudioIn = NULL;

}

TEST_F(VC_AudioInGeneral_Test, SyncWaitexit)
{
    m_pVC_AudioInGeneral->m_state = VC_AudioInGeneral::ST_IDLE;

    m_pVC_AudioInGeneral->SyncWait();
}

/* EOF */
