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

#include "BL_Thread.h"
#include "CL_WaitObj.h"
#include "AplThreadName.h"

#include "VC_CommonDef.h"
#include "VC_IAudioOutMedia_mock.h"
#include "VC_AudioBufferGeneral.h"
#include "VC_Player.h"
#include "VC_AudioOutGeneral.h"


using ::testing::Return;
using ::testing::_;

/**
 * Summary for class VC_AudioOutGeneral_Test
 * 
 * This is a detail description for class VC_AudioOutGeneral_Test
 *
 */
class VC_AudioOutGeneral_Test:public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_AudioOutGeneral = new(MEM_Voice) VC_AudioOutGeneral(VC_AUDIO_BEEP);
        }

        virtual void TearDown()
        {
            if (NULL != m_pVC_AudioOutGeneral) {
                delete m_pVC_AudioOutGeneral;
                m_pVC_AudioOutGeneral = NULL;
            }
        }

    public:
        VC_AudioOutGeneral  *m_pVC_AudioOutGeneral;
};

/**
 * Summary for class Listener_test
 * 
 * This is a detail description for class Listener_test
 *
 */
class Listener_test: public VC_AudioOut::Listener
{
private:
    Listener_test() {}
    virtual VOID OnAudioOutDone(VC_AudioBuffer* pBuf) {}
    virtual VOID OnAudioOutComplete(CL_BOOL success) {}
};

TEST_F(VC_AudioOutGeneral_Test, SetSessionProperty)
{
    m_pVC_AudioOutGeneral->SetSessionProperty(CL_TRUE);
}

TEST_F(VC_AudioOutGeneral_Test, Openm_bIsThreadStartFalsem_pAudioOutNULL)
{
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
    m_pVC_AudioOutGeneral->m_bIsThreadStart = CL_FALSE;
    INT sampleRate = 0;
    INT fragSize = 0;
    INT fragCount = 0;
    EXPECT_FALSE(m_pVC_AudioOutGeneral->Open(&sampleRate, &fragSize, &fragCount));
}

TEST_F(VC_AudioOutGeneral_Test, Openm_bIsThreadStartFalsem_pAudioOutopentrue)
{
    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Open(_, _, _, _)).WillOnce(Return(CL_TRUE));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
    m_pVC_AudioOutGeneral->m_bIsThreadStart = CL_FALSE;
    INT sampleRate = 0;
    INT fragSize = 0;
    INT fragCount = 0;
    EXPECT_TRUE(m_pVC_AudioOutGeneral->Open(&sampleRate, &fragSize, &fragCount));
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}


TEST_F(VC_AudioOutGeneral_Test, Openm_bIsThreadStartTruem_pAudioOutOpenFalse)
{
    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Open(_, _, _, _)).WillOnce(Return(CL_FALSE));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
    m_pVC_AudioOutGeneral->m_bIsThreadStart = CL_TRUE;
    INT sampleRate = 0;
    INT fragSize = 0;
    INT fragCount = 0;
    EXPECT_FALSE(m_pVC_AudioOutGeneral->Open(&sampleRate, &fragSize, &fragCount));

    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_AudioOutGeneral_Test, AllocBuffer_Idle)
{
    INT nSize = 0;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_NONE;
    VC_AudioBuffer* pAudioBuffer = m_pVC_AudioOutGeneral->AllocBuffer(nSize);
    EXPECT_TRUE(NULL == pAudioBuffer);
}

TEST_F(VC_AudioOutGeneral_Test, AllocBufferm_dwBufCountBiggerthansixteen)
{
    INT nSize = 2;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_IDLE;
    m_pVC_AudioOutGeneral->m_dwBufCount = 16;
    VC_AudioBuffer* pAudioBuffer = m_pVC_AudioOutGeneral->AllocBuffer(nSize);
    EXPECT_TRUE(NULL == pAudioBuffer);
}

TEST_F(VC_AudioOutGeneral_Test, AllocBufferNULL)
{
    INT nSize = 0;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_IDLE;
    m_pVC_AudioOutGeneral->m_dwBufCount = 2;
    VC_AudioBuffer* pAudioBuffer = m_pVC_AudioOutGeneral->AllocBuffer(nSize);
    EXPECT_TRUE(NULL == pAudioBuffer);
}

TEST_F(VC_AudioOutGeneral_Test, AllocBuffer)
{
    INT nSize = 2;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_IDLE;
    m_pVC_AudioOutGeneral->m_dwBufCount = 2;
    VC_AudioBuffer* pAudioBuffer = m_pVC_AudioOutGeneral->AllocBuffer(nSize);
    EXPECT_FALSE(NULL == pAudioBuffer);
}

TEST_F(VC_AudioOutGeneral_Test, Write_NullCheck)
{
    VC_AudioBuffer* pBuf = NULL;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Write(pBuf);
    EXPECT_EQ(CL_FALSE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Write_StopNone)
{
    VC_AudioBuffer* pBuf = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NORMAL;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Write(pBuf);
    EXPECT_EQ(CL_FALSE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Write_False)
{
    VC_AudioBuffer* pBuf = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_AudioOutGeneral->m_dwIndexR = 0;
    m_pVC_AudioOutGeneral->m_dwBufCount = 0;
    m_pVC_AudioOutGeneral->m_dwIndexW = 1;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NONE;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Write(pBuf);
    EXPECT_EQ(CL_FALSE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Write)
{
    VC_AudioBuffer* pBuf = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_AudioOutGeneral->m_dwIndexR = 0;
    m_pVC_AudioOutGeneral->m_dwBufCount = 6;
    m_pVC_AudioOutGeneral->m_dwIndexW = 1;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NONE;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Write(pBuf);
    EXPECT_EQ(CL_TRUE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Writem_stateRun)
{
    VC_AudioBuffer* pBuf = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_AudioOutGeneral->m_dwIndexR = 0;
    m_pVC_AudioOutGeneral->m_dwBufCount = 6;
    m_pVC_AudioOutGeneral->m_dwIndexW = 1;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NONE;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Write(pBuf);
    EXPECT_EQ(CL_TRUE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Start_StNone)
{
    VC_AudioOut::Listener* pListener = NULL;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_NONE;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Start(pListener);
    EXPECT_EQ(CL_FALSE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Start)
{
    VC_AudioOut::Listener* pListener = NULL;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_IDLE;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Start(pListener);
    EXPECT_EQ(CL_TRUE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Stop_StateCheck)
{
    CL_BOOL immediately = CL_FALSE;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_IDLE;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Stop(immediately);
    EXPECT_EQ(CL_FALSE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Stop_StopCheck)
{
    CL_BOOL immediately = CL_FALSE;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_FORCE;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Stop(immediately);
    EXPECT_EQ(CL_FALSE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Stop_FalseCheck)
{
    CL_BOOL immediately = CL_FALSE;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NORMAL;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Stop(immediately);
    EXPECT_EQ(CL_TRUE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Stop_immediatelyFalsem_playPLAY_PAUSING)
{
    CL_BOOL immediately = CL_FALSE;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NORMAL;
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;
    CL_BOOL nbool = m_pVC_AudioOutGeneral->Stop(immediately);
    EXPECT_EQ(CL_TRUE, nbool);
}

void* thread_Stopm_waitStopNotify(void* par)
{
    printf("in thread_Stopm_waitStopNotify\n");
    if (NULL == par) {
        RETURN_NULL;
    }
    VC_AudioOutGeneral* p_VCAudio = static_cast<VC_AudioOutGeneral*> (par);
    usleep(300000);

    printf("begin Notify\n");
    p_VCAudio->m_state = VC_AudioOutGeneral::ST_NONE;
    p_VCAudio->m_waitStop.Notify();
    printf("m_waitStop Notify end\n");

    RETURN_NULL;
}

TEST_F(VC_AudioOutGeneral_Test, Stopimmediatelytruem_playPLAY_PAUSING)
{
    CL_BOOL immediately = CL_TRUE;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NORMAL;
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;
    // m_pVC_AudioOutGeneral->m_waitStop.Notify();

    pthread_t pid;
    int res = pthread_create(&pid, NULL, thread_Stopm_waitStopNotify, m_pVC_AudioOutGeneral);

    CL_BOOL nbool = m_pVC_AudioOutGeneral->Stop(immediately);
    EXPECT_EQ(CL_TRUE, nbool);
}

TEST_F(VC_AudioOutGeneral_Test, Stopimmediatelytruem_playPLAY_IDLE)
{
    CL_BOOL immediately = CL_TRUE;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NORMAL;
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_IDLE;
    m_pVC_AudioOutGeneral->m_waitStop.Notify();

    pthread_t pid;
    int res = pthread_create(&pid, NULL, thread_Stopm_waitStopNotify, m_pVC_AudioOutGeneral);

    CL_BOOL nbool = m_pVC_AudioOutGeneral->Stop(immediately);
    EXPECT_EQ(CL_TRUE, nbool);
}


TEST_F(VC_AudioOutGeneral_Test, Close_StateCheck)
{
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_NONE;
    CL_BOOL err = m_pVC_AudioOutGeneral->Close();
    EXPECT_EQ(err, CL_FALSE);
}

TEST_F(VC_AudioOutGeneral_Test, Close)
{
    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Close()).WillOnce(Return(CL_TRUE));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_FORCE;
    CL_BOOL err = m_pVC_AudioOutGeneral->Close();
    EXPECT_EQ(err, CL_TRUE);
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_AudioOutGeneral_Test, Closem_stateST_IDLE)
{
    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Close()).WillOnce(Return(CL_TRUE));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_IDLE;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_FORCE;
    CL_BOOL err = m_pVC_AudioOutGeneral->Close();
    EXPECT_EQ(err, CL_TRUE);
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_AudioOutGeneral_Test, CloseFalse)
{
    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Close()).WillOnce(Return(CL_FALSE));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_FORCE;
    CL_BOOL err = m_pVC_AudioOutGeneral->Close();
    EXPECT_EQ(err, CL_FALSE);
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_AudioOutGeneral_Test, DoTask)
{
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_NONE;

    m_pVC_AudioOutGeneral->DoTask();
}


TEST_F(VC_AudioOutGeneral_Test, DoTaskm_stopNoneRNotEqualWm_stopForce)
{
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_FORCE;
    m_pVC_AudioOutGeneral->m_dwIndexR = 1;
    m_pVC_AudioOutGeneral->m_dwIndexW = 2;
    MockVC_IAudioOutMedia* audioMedia = new(MEM_Voice) MockVC_IAudioOutMedia();
    m_pVC_AudioOutGeneral->m_pAudioOut = audioMedia;
    m_pVC_AudioOutGeneral->DoTask();
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;

    delete audioMedia;
}

void* thread_dotaskfirst(void* par)
{
    if (NULL == par) {
        return NULL;
    }

    VC_AudioOutGeneral* p_VCAudio = static_cast<VC_AudioOutGeneral*> (par);
    p_VCAudio->m_state = VC_AudioOutGeneral::ST_IDLE;
    Listener_test listener;
    usleep(20000);
    p_VCAudio->m_dwIndexW = 10;
    p_VCAudio->Start(&listener);
    usleep(20000);
    p_VCAudio->m_waitPause.Notify();

    usleep(50000);
    p_VCAudio->m_dwBufCount = 2;
    p_VCAudio->m_waitPause.Notify();

    usleep(50000);
    p_VCAudio->m_play = VC_AudioOutGeneral::PLAY_PLAYOVER;
    p_VCAudio->m_stop = VC_AudioOutGeneral::STOP_FORCE;
    // notify dotask break;
    p_VCAudio->m_waitPause.Notify();

    return NULL;
}

TEST_F(VC_AudioOutGeneral_Test, DoTaskm_stopSTOP_NONEm_dwIndexRWEqualm_playForce)
{
    // m_dwBufCount first 0, second 2
    // m_pListener != NULL
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NONE;
    m_pVC_AudioOutGeneral->m_dwIndexW = 1;
    m_pVC_AudioOutGeneral->m_dwIndexR = 1;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;
    m_pVC_AudioOutGeneral->m_dwBufCount = 0;

    // listener
    Listener_test listener;
    m_pVC_AudioOutGeneral->m_pListener = &listener;

    // allocBuffer
    VC_AudioBufferGeneral* pBuf = VC_AudioBufferGeneral::Alloc(10);
    if (NULL == pBuf) {
        std::cout << "Alloc audio buffer failed, size" << std::endl;
        return;
    }
    pBuf->setLen(5);

    m_pVC_AudioOutGeneral->m_buffers[0] = pBuf;
    std::cout << "******************buf size= " <<  pBuf->size() << std::endl;

    MockVC_IAudioOutMedia* audioMedia = new(MEM_Voice) MockVC_IAudioOutMedia();
    EXPECT_CALL(*audioMedia, Write(_, _)).WillOnce(Return(1));
    m_pVC_AudioOutGeneral->m_pAudioOut = audioMedia;

    pthread_t pid;
    pthread_create(&pid, NULL, thread_dotaskfirst, m_pVC_AudioOutGeneral);
    m_pVC_AudioOutGeneral->DoTask();
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;

    delete audioMedia;
}

TEST_F(VC_AudioOutGeneral_Test, DoTaskm_stopSTOP_NONEm_dwIndexRWEqualm_playForce_getLenZero_ListenerNULL)
{
    // m_dwBufCount first 0, second 2
    // m_pListener != NULL
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NONE;
    m_pVC_AudioOutGeneral->m_dwIndexW = 1;
    m_pVC_AudioOutGeneral->m_dwIndexR = 1;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;
    m_pVC_AudioOutGeneral->m_dwBufCount = 0;

    // // listener
    // Listener_test listener;
    // m_pVC_AudioOutGeneral->m_pListener = &listener;

    // allocBuffer
    VC_AudioBufferGeneral* pBuf = VC_AudioBufferGeneral::Alloc(10);
    if (NULL == pBuf) {
        std::cout << "Alloc audio buffer failed, size" << std::endl;
        return;
    }
    pBuf->setLen(0);

    m_pVC_AudioOutGeneral->m_buffers[0] = pBuf;
    std::cout << "******************buf size= " <<  pBuf->size() << std::endl;

    MockVC_IAudioOutMedia* audioMedia = new(MEM_Voice) MockVC_IAudioOutMedia();
    // EXPECT_CALL(audioMedia, Write(_, _)).WillOnce(Return(1));
    m_pVC_AudioOutGeneral->m_pAudioOut = audioMedia;

    pthread_t pid;
    pthread_create(&pid, NULL, thread_dotaskfirst, m_pVC_AudioOutGeneral);
    m_pVC_AudioOutGeneral->DoTask();
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;

    delete audioMedia;
}

void* thread_dotasksecond(void* par)
{
    if (NULL == par) {
        return NULL;
    }

    VC_AudioOutGeneral* p_VCAudio = static_cast<VC_AudioOutGeneral*> (par);
    usleep(20000);
    p_VCAudio->m_dwIndexW = 10;
    p_VCAudio->m_state = VC_AudioOutGeneral::ST_IDLE;
    p_VCAudio->m_waitPause.Notify();

    usleep(50000);
    p_VCAudio->m_stop = VC_AudioOutGeneral::STOP_FORCE;
    p_VCAudio->m_waitPause.Notify();

    return NULL;
}

TEST_F(VC_AudioOutGeneral_Test, DoTaskm_stopSTOP_NONEm_dwIndexRWEqualm_playForce_getLenZero_ListenerNULL_m_statechanged)
{
    // m_dwBufCount first 0, second 1
    // m_pListener != NULL
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NONE;
    m_pVC_AudioOutGeneral->m_dwIndexW = 1;
    m_pVC_AudioOutGeneral->m_dwIndexR = 2;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;
    m_pVC_AudioOutGeneral->m_dwBufCount = 2;

    // // listener
    // Listener_test listener;
    // m_pVC_AudioOutGeneral->m_pListener = &listener;

    // allocBuffer
    VC_AudioBufferGeneral* pBuf = VC_AudioBufferGeneral::Alloc(10);
    if (NULL == pBuf) {
        std::cout << "Alloc audio buffer failed, size" << std::endl;
        return;
    }
    pBuf->setLen(0);

    m_pVC_AudioOutGeneral->m_buffers[0] = pBuf;
    std::cout << "******************buf size= " <<  pBuf->size() << std::endl;

    // m_pAudioOut
    MockVC_IAudioOutMedia* audioMedia = new(MEM_Voice) MockVC_IAudioOutMedia();
    // EXPECT_CALL(audioMedia, Write(_, _)).WillOnce(Return(1));
    m_pVC_AudioOutGeneral->m_pAudioOut = audioMedia;

    pthread_t pid;
    pthread_create(&pid, NULL, thread_dotasksecond, m_pVC_AudioOutGeneral);
    m_pVC_AudioOutGeneral->DoTask();
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;

    delete audioMedia;
}

TEST_F(VC_AudioOutGeneral_Test, DoTask_NormalStop)
{
    m_pVC_AudioOutGeneral->m_dwIndexR = 1;
    m_pVC_AudioOutGeneral->m_dwIndexW = 1;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NORMAL;
    MockVC_IAudioOutMedia* audioMedia = new(MEM_Voice) MockVC_IAudioOutMedia();
    m_pVC_AudioOutGeneral->m_pAudioOut = audioMedia;
    m_pVC_AudioOutGeneral->DoTask();
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;

    delete audioMedia;
}

TEST_F(VC_AudioOutGeneral_Test, DoTaskm_pListenerNotNULL)
{
    m_pVC_AudioOutGeneral->m_dwIndexR = 1;
    m_pVC_AudioOutGeneral->m_dwIndexW = 1;
    m_pVC_AudioOutGeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NORMAL;
    MockVC_IAudioOutMedia* audioMedia = new(MEM_Voice) MockVC_IAudioOutMedia();
    m_pVC_AudioOutGeneral->m_pAudioOut = audioMedia;

    Listener_test listener;
    m_pVC_AudioOutGeneral->m_pListener = &listener;
    m_pVC_AudioOutGeneral->DoTask();
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;

    delete audioMedia;
}

TEST_F(VC_AudioOutGeneral_Test, ClearBuffers)
{
    m_pVC_AudioOutGeneral->m_dwBufCount = 2;
    m_pVC_AudioOutGeneral->m_buffers[0] = NULL;
    m_pVC_AudioOutGeneral->m_buffers[1] = NULL;
    m_pVC_AudioOutGeneral->ClearBuffers();
}

TEST_F(VC_AudioOutGeneral_Test, SetVolume_NullCheck)
{
    CL_BOOL err = m_pVC_AudioOutGeneral->SetVolume(3);
    EXPECT_EQ(err, CL_FALSE);
}

TEST_F(VC_AudioOutGeneral_Test, GetVolume_NullCheck)
{
    int err = m_pVC_AudioOutGeneral->GetVolume();
    EXPECT_EQ(err, 0);
}

// TEST_F(VC_AudioOutGeneral_Test, SetVolume)
// {
//     MockVC_IAudioOutMedia audioMedia;
//     m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
//     EXPECT_CALL(audioMedia, SetVolume(_)).WillOnce(Return(CL_TRUE));
//     EXPECT_TRUE(m_pVC_AudioOutGeneral->SetVolume(3));
//     m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
// }

// TEST_F(VC_AudioOutGeneral_Test, GetVolume)
// {
//     MockVC_IAudioOutMedia audioMedia;
//     m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
//     EXPECT_CALL(audioMedia, GetVolume()).WillOnce(Return(4));
//     INT vol = m_pVC_AudioOutGeneral->GetVolume();
//     EXPECT_EQ(4, vol);
//     m_pVC_AudioOutGeneral->m_pAudioOut = NULL;

// }

TEST_F(VC_AudioOutGeneral_Test, StopOutThread)
{
    m_pVC_AudioOutGeneral->StopOutThread();
}

TEST_F(VC_AudioOutGeneral_Test, Resumem_playNotPause)
{
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_OPENDED;
    CL_ERROR err = m_pVC_AudioOutGeneral->Resume();

    EXPECT_EQ(VC_ERROR_FAILED, err);
}

TEST_F(VC_AudioOutGeneral_Test, ResumeTrue)
{
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;

    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Resume()).WillOnce(Return(CL_TRUE));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
    CL_ERROR err = m_pVC_AudioOutGeneral->Resume();

    EXPECT_EQ(VC_ERROR_NO_ERROR, err);

    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_AudioOutGeneral_Test, ResumeFalse)
{
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;

    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Resume()).WillOnce(Return(false));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
    CL_ERROR err = m_pVC_AudioOutGeneral->Resume();

    EXPECT_EQ(VC_ERROR_ERROR, err);

    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_AudioOutGeneral_Test, Pausem_playPLAY_IDLE)
{
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_IDLE;
    CL_ERROR err = m_pVC_AudioOutGeneral->Pause();

    EXPECT_EQ(err, 1);
}

TEST_F(VC_AudioOutGeneral_Test, Pausem_playPLAY_PLAYOVER)
{
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PLAYOVER;
    CL_ERROR err = m_pVC_AudioOutGeneral->Pause();

    EXPECT_EQ(err, 1);
}

TEST_F(VC_AudioOutGeneral_Test, Pausem_playPLAY_WRITING)
{
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_WRITING;

    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Pause()).WillOnce(Return(true));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;

    CL_ERROR err = m_pVC_AudioOutGeneral->Pause();

    EXPECT_EQ(err, 0);

    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_AudioOutGeneral_Test, Pausem_playPLAY_OPENDED)
{
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_OPENDED;

    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Pause()).WillOnce(Return(false));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;

    CL_ERROR err = m_pVC_AudioOutGeneral->Pause();

    EXPECT_EQ(err, 2);

    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_AudioOutGeneral_Test, Pausem_playPLAY_PAUSING)
{
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;
    m_pVC_AudioOutGeneral->Pause();
}

TEST_F(VC_AudioOutGeneral_Test, Openm_bIsThreadStartFalsem_pAudioOutopentrueRun)
{
    MockVC_IAudioOutMedia audioMedia;
    EXPECT_CALL(audioMedia, Open(_, _, _, _)).WillOnce(Return(CL_FALSE));
    m_pVC_AudioOutGeneral->m_pAudioOut = &audioMedia;
    m_pVC_AudioOutGeneral->m_bIsThreadStart = CL_FALSE;
    m_pVC_AudioOutGeneral->m_play = VC_AudioOutGeneral::PLAY_PAUSING;
    INT sampleRate = 0;
    INT fragSize = 0;
    INT fragCount = 0;
    EXPECT_FALSE(m_pVC_AudioOutGeneral->Open(&sampleRate, &fragSize, &fragCount, 1, 1));
    m_pVC_AudioOutGeneral->Start(NULL);
    m_pVC_AudioOutGeneral->m_pAudioOut = NULL;
    sleep(1);

    m_pVC_AudioOutGeneral->m_stop = VC_AudioOutGeneral::STOP_NORMAL;
    // for DoTask Wait
    m_pVC_AudioOutGeneral->Start(NULL);
    m_pVC_AudioOutGeneral->m_waitPause.Notify();

}

/* EOF */
