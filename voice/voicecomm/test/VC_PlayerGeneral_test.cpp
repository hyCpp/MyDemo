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
#define protected public
#define private public

#include "voicecomm_test.h"

#ifndef CXX_BL_ERROR_H
#	include "BL_Error.h"
#endif

#include "AplThreadName.h"


#include "VC_PlayerGeneral.h"
#include "VC_CommDef.h"
#include "VC_AudioBufferGeneral.h"
#include "VC_AudioOutGeneral_mock.h"
#include "VC_CommonDef.h"
#include "BL_FullFile.h"
#include "pthread.h"
#include "stdio.h"
#include "VC_PlayerGeneral_Listener_test.h"
#include "VC_IAudioOutMedia_mock.h"
#include "VC_DecoderOgg.h"
#include "VC_DecEventOgg.h"

#include "VC_Factory_mock.h"
#include "VC_DecoderOgg_mock.h"

#ifndef CXX_CL_WAITOBJ_H
#	include "CL_WaitObj.h"
#endif

#include "CL_Thread.h"

using ::testing::Return;
using ::testing::_;

void* thread_test(void* ptr);

/**
 * Summary for class VC_PlayerGeneral_Test
 * 
 * This is a detail description for class VC_PlayerGeneral_Test
 *
 */
class VC_PlayerGeneral_Test : public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_PlayerGeneral = new(MEM_Voice) VC_PlayerGeneral(VC_AUDIO_SOUND);
        }

        virtual void TearDown()
        {
             if (NULL != m_pVC_PlayerGeneral) {
                  delete m_pVC_PlayerGeneral;
                  m_pVC_PlayerGeneral = NULL;
            }
        }

    public:
        VC_PlayerGeneral  *m_pVC_PlayerGeneral;
};

TEST_F(VC_PlayerGeneral_Test, Init_m_pDecoders_Initerr_startthread)
{
    Mock_VCFactory* mock_factory = new Mock_VCFactory();
    VC_Factory::Destroy();
    VC_Factory::s_pInstance = mock_factory;

    MockVC_DecoderOgg* mockogg = new MockVC_DecoderOgg();
    MockVC_DecoderOgg* mockogg2 = NULL;
    EXPECT_CALL(*mock_factory, CreateDecoder(_, VC_DECODER_OGG)).WillOnce(Return(mockogg));
    EXPECT_CALL(*mock_factory, CreateDecoder(_, VC_DECODER_TTS)).WillOnce(Return(mockogg2));
    EXPECT_CALL(*mockogg, Init(_)).WillOnce(Return(APL_ERR_VC_DECODER));

    VC_AudioOutGeneral* outgeneral = NULL;
    EXPECT_CALL(*mock_factory, CreateAudioOut(_)).WillOnce(Return(outgeneral));

    m_pVC_PlayerGeneral->Init();
    VC_Factory::Destroy();
}

TEST_F(VC_PlayerGeneral_Test, Init_m_pDecoders_Initerr_NotStartThread)
{
    Mock_VCFactory* mock_factory = new Mock_VCFactory();
    VC_Factory::Destroy();
    VC_Factory::s_pInstance = mock_factory;

    MockVC_DecoderOgg* mockogg = NULL;
    EXPECT_CALL(*mock_factory, CreateDecoder(_, _)).Times(2).WillRepeatedly(Return(mockogg));
    // EXPECT_CALL(*mock_factory, CreateDecoder(_, 1)).WillOnce(Return(mockogg));
    // EXPECT_CALL(*mockogg, Init(_)).WillOnce(Return(APL_ERR_VC_DECODER));

    VC_AudioOutGeneral* outgeneral = NULL;
    EXPECT_CALL(*mock_factory, CreateAudioOut(_)).WillOnce(Return(outgeneral));

    m_pVC_PlayerGeneral->Init();
    VC_Factory::Destroy();
}

TEST_F(VC_PlayerGeneral_Test, Play_InvalidFileName)
{
    BL_String fileName("filename");
    LONG loffset = 0;
    DWORD dwLen = 0;
    EXPECT_FALSE(m_pVC_PlayerGeneral->Play(fileName, loffset, dwLen, NULL));
}

TEST_F(VC_PlayerGeneral_Test, Play)
{
    m_pVC_PlayerGeneral->Init();
    BL_String fileName("SD/.kanavijp/NDATA/Sounds/IGOSENGCM.SND");
    LONG loffset = 7928;
    DWORD dwLen = 7846;
    EXPECT_TRUE(m_pVC_PlayerGeneral->Play(fileName, loffset, dwLen, NULL));
    // m_pVC_PlayerGeneral->DoPlay();
}

#if 0
TEST_F(VC_PlayerGeneral_Test, Play_Buf)
{
    m_pVC_PlayerGeneral->Init();
    CL_BOOL err = m_pVC_PlayerGeneral->Play(NULL, 100, NULL);
    EXPECT_EQ(err, CL_TRUE);
}

#endif

TEST_F(VC_PlayerGeneral_Test, Play_Buf_NullDecoder)
{
    EXPECT_FALSE(m_pVC_PlayerGeneral->Play(NULL, 100, NULL));
}

TEST_F(VC_PlayerGeneral_Test, Play_Tts_StateCheck)
{
    m_pVC_PlayerGeneral->m_state = VC_PlayerGeneral::ST_PENDING;
    BL_String ttsText("test");
    CL_BOOL err = m_pVC_PlayerGeneral->Play(ttsText, 3, NULL);
    EXPECT_EQ(err, CL_TRUE);
}

TEST_F(VC_PlayerGeneral_Test, IsPCM)
{
    // const char* pcmHeader = "FFIR";
    // EXPECT_TRUE(m_pVC_PlayerGeneral->IsPCM(pcmHeader));
    const char* pcmInvalidHeader = "PCM ";
    EXPECT_FALSE(m_pVC_PlayerGeneral->IsPCM(pcmInvalidHeader));
}

TEST_F(VC_PlayerGeneral_Test, InitPlayer)
{
    m_pVC_PlayerGeneral->InitPlayer();
}

TEST_F(VC_PlayerGeneral_Test, Stop_StateCheck)
{
    m_pVC_PlayerGeneral->m_state = VC_PlayerGeneral::ST_NONE;
    CL_BOOL err = m_pVC_PlayerGeneral->Stop();
    EXPECT_EQ(err, CL_TRUE);
}

TEST_F(VC_PlayerGeneral_Test, Stop_NullCheck)
{
    m_pVC_PlayerGeneral->m_state = VC_PlayerGeneral::ST_NONE;
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();
    m_pVC_PlayerGeneral->m_waitStop.Notify();
    CL_BOOL err = m_pVC_PlayerGeneral->Stop();
    EXPECT_EQ(err, CL_TRUE);
}

void* thread_test(void* ptr)
{
    printf("in ...\n");
    sleep(2);
    printf("in2...\n");
    CL_WaitObj *wait = (CL_WaitObj*)ptr;
    wait->Notify();
    printf("in2...\n");
    RETURN_NULL;
}

TEST_F(VC_PlayerGeneral_Test, Stop)
{
    pthread_t pid;
    int res = pthread_create(&pid, NULL, thread_test, &m_pVC_PlayerGeneral->m_waitStop);
    printf("res=%d", res);
    printf("out ...\n");
    m_pVC_PlayerGeneral->m_state = VC_PlayerGeneral::ST_PENDING;
    m_pVC_PlayerGeneral->Init();
    m_pVC_PlayerGeneral->m_pDecoder = m_pVC_PlayerGeneral->m_pDecoders[VC_DECODER_OGG];
    printf("stop in..\n");
    CL_BOOL err = m_pVC_PlayerGeneral->Stop();
    EXPECT_EQ(err, CL_TRUE);
}

#if 0
TEST_F(VC_PlayerGeneral_Test, GetDuration)
{
    m_pVC_PlayerGeneral->Init();
    EXPECT_TRUE(NULL != m_pVC_PlayerGeneral->m_pDecoders[VC_DECODER_TTS]);
    BL_String ttsText("test");
    DWORD dwDuration = m_pVC_PlayerGeneral->GetDuration(ttsText);
    // Currently, the duration always be 0
    EXPECT_EQ(DWORD(0), dwDuration);
}

#endif

TEST_F(VC_PlayerGeneral_Test, GetDuration_NullCheck)
{
    BL_String ttsText("test");
    DWORD dwDuration = m_pVC_PlayerGeneral->GetDuration(ttsText);
    EXPECT_EQ(DWORD(0), dwDuration);
}

TEST_F(VC_PlayerGeneral_Test, OnAudioOutComplete)
{
    CL_BOOL nbool = CL_FALSE;
    m_pVC_PlayerGeneral->OnAudioOutComplete(nbool);
}

TEST_F(VC_PlayerGeneral_Test, OnAudioOutDone_NullCheck)
{
    VC_AudioBuffer* pBuffer = NULL;
    m_pVC_PlayerGeneral->OnAudioOutDone(NULL);
}

TEST_F(VC_PlayerGeneral_Test, OnAudioOutDone)
{
    VC_AudioBuffer* pBuffer = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_PlayerGeneral->OnAudioOutDone(pBuffer);
}

TEST_F(VC_PlayerGeneral_Test, PlayFourParameters)
{
    m_pVC_PlayerGeneral->m_pDecoders[VC_DECODER_OGG] = new(MEM_Voice) VC_DecoderOgg();
    char buffer[] = "mengfanbing";
    EXPECT_TRUE(m_pVC_PlayerGeneral->Play((VOID*)buffer, 1, NULL, 1));
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeZeroIsPCM)
{
    m_pVC_PlayerGeneral->m_eventType = 0;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, NULL, NULL);
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();

    MockVC_AudioOutGeneral mockaudio;
    m_pVC_PlayerGeneral->m_pAudioOut = &mockaudio;
    EXPECT_CALL(mockaudio, Open(_, _, _, _)).WillRepeatedly(Return(false));
    EXPECT_CALL(mockaudio, Close()).WillRepeatedly(Return(true));
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeZeroNotPCMm_pAudioOutNULL)
{
    m_pVC_PlayerGeneral->m_eventType = 0;
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg("123456789123mengfanbingsuntec", 1, NULL, NULL);

    m_pVC_PlayerGeneral->m_pAudioOut = NULL;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZerom_pAudioOutNotNULLOpenSmallerThanZeroEXIT)
{
    m_pVC_PlayerGeneral->Init();
    m_pVC_PlayerGeneral->m_eventType = 1;
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg("FFIR56789123mengfanbingsuntec", 1, NULL, NULL);
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();

    MockVC_AudioOutGeneral mockaudio;
    m_pVC_PlayerGeneral->m_pAudioOut = &mockaudio;
    EXPECT_CALL(mockaudio, Open(_, _, _, _)).WillRepeatedly(Return(false));
    EXPECT_CALL(mockaudio, Close()).WillRepeatedly(Return(true));
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZerom_pAudioOutNotNULLOpenSmallerThanZeroEXITm_pListenerNULL)
{
    m_pVC_PlayerGeneral->Init();
    m_pVC_PlayerGeneral->m_eventType = 1;
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg("FFIR56789123mengfanbingsuntec", 1, NULL, NULL);
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();

    MockVC_AudioOutGeneral mockaudio;
    m_pVC_PlayerGeneral->m_pAudioOut = &mockaudio;
    EXPECT_CALL(mockaudio, Open(_, _, _, _)).WillRepeatedly(Return(false));
    EXPECT_CALL(mockaudio, Close()).WillRepeatedly(Return(true));
    m_pVC_PlayerGeneral->m_pListener = NULL;

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZerom_pAudioOutNotNULLOpenBiggerThanZerom_dwBufCountEqualZero)
{
    m_pVC_PlayerGeneral->Init();
    m_pVC_PlayerGeneral->m_eventType = 1;
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg("FFIR56789123mengfanbingsuntec", 1, NULL, NULL);
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();

    MockVC_AudioOutGeneral mockaudio;
    m_pVC_PlayerGeneral->m_pAudioOut = &mockaudio;
    EXPECT_CALL(mockaudio, Open(_, _, _, _)).WillRepeatedly(Return(false));
    EXPECT_CALL(mockaudio, Close()).WillRepeatedly(Return(true));
    m_pVC_PlayerGeneral->m_pListener = NULL;
    m_pVC_PlayerGeneral->m_dwBufCount = 0;

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

// new buffer

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeZeroIsPCMreturn)
{
    m_pVC_PlayerGeneral->m_eventType = 0;
    char buffer[] = { 'R', 'I', 'F', 'F', '5', '6', '7', '8', '9', '10', '11', '12', 'f', 'm', 't', ' ',
        1, 0, 0, 0, 's', 'f', 'a', 'c', 't', 1, 0, 0, 0, 's', 'm', 'e', 'n', 'g' };
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, NULL, NULL);
    // m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();

    MockVC_AudioOutGeneral mockaudio;
    m_pVC_PlayerGeneral->m_pAudioOut = &mockaudio;
    EXPECT_CALL(mockaudio, Open(_, _, _, _)).WillRepeatedly(Return(false));
    // EXPECT_CALL(mockaudio, Close()).WillRepeatedly(Return(true));
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

    // m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    // delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeZeroIsPCMParseDatareturn)
{
    m_pVC_PlayerGeneral->m_eventType = 0;
    char buffer[] = { 'R', 'I', 'F', 'F', '5', '6', '7', '8', '9', '10', '11', '12', 'f', 'm', 't', ' ',
        1, 0, 0, 0, 's', 'f', 'a', 'c', 't', 1, 0, 0, 0, 's', 'd', 'a', 't', 'a', 'm', 'e', 'n', 'g', 'c', 'h' };
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, NULL, NULL);
    // m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();

    MockVC_AudioOutGeneral mockaudio;
    m_pVC_PlayerGeneral->m_pAudioOut = &mockaudio;
    EXPECT_CALL(mockaudio, Open(_, _, _, _)).WillRepeatedly(Return(false));
    // EXPECT_CALL(mockaudio, Close()).WillRepeatedly(Return(true));
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

    // m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    // delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZeroOpenTrueEXIT)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, NULL, NULL);
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();

    MockVC_AudioOutGeneral mockaudio;
    m_pVC_PlayerGeneral->m_pAudioOut = &mockaudio;
    EXPECT_CALL(mockaudio, Open(_, _, _, _)).WillRepeatedly(Return(true));
    EXPECT_CALL(mockaudio, Close()).WillRepeatedly(Return(true));
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

void* thread_testm_bStopTrue(void* par)
{
    if (NULL == par) {
        RETURN_NULL;
    }

    // usleep(200000);

    VC_PlayerGeneral* p_VCPlayer = static_cast<VC_PlayerGeneral*> (par);
    Test_VC_AudioOutGeneral* p_TestVC = static_cast<Test_VC_AudioOutGeneral*> (p_VCPlayer->m_pAudioOut);
    p_VCPlayer->Wait();
    p_VCPlayer->m_bStop = true;

    RETURN_NULL;
}

// this
TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZeroOpenTrueiFragSizeBiggerthanZeroAllocBufferNotNULLm_bStopTrue)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, m_pVC_PlayerGeneral->m_pDecoder, NULL);

    Test_VC_AudioOutGeneral testaudio(m_pVC_PlayerGeneral);
    m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_bStop = true;

    // pthread_t pid;
    // int res = pthread_create(&pid, NULL, thread_testm_bStopTrue, m_pVC_PlayerGeneral);

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZeroOpenTrueiFragSizeBiggerthanZeroAllocBufferNotNULLm_bStopFalseStopFalse)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, m_pVC_PlayerGeneral->m_pDecoder, NULL);

    Test_VC_AudioOutGeneral testaudio(NULL);
    m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_bStop = false;

    // pthread_t pid;
    // int res = pthread_create(&pid, NULL, thread_testm_bStopTrue, m_pVC_PlayerGeneral);

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

void* thread_testm_bStopFalseStopTrue(void* par)
{
    if (NULL == par) {
        RETURN_NULL;
    }

    usleep(200000);
    VC_PlayerGeneral* p_VCPlayer = static_cast<VC_PlayerGeneral*> (par);
    p_VCPlayer->m_bStop = true;
    p_VCPlayer->Notify();

    RETURN_NULL;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZeroOpenTrueiFragSizeBiggerthanZeroAllocBufferNotNULLm_bStopFalseStopTrue)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, m_pVC_PlayerGeneral->m_pDecoder, NULL);

    Test_VC_AudioOutGeneral testaudio(NULL);
    testaudio.m_bstop = true;
    m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_bStop = true;

    pthread_t pid;
    int res = pthread_create(&pid, NULL, thread_testm_bStopFalseStopTrue, m_pVC_PlayerGeneral);

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

void* thread_testm_bStopFalseStopTruem_bStopNormalTrue(void* par)
{
    if (NULL == par) {
        RETURN_NULL;
    }

    usleep(200000);
    VC_PlayerGeneral* p_VCPlayer = static_cast<VC_PlayerGeneral*> (par);
    p_VCPlayer->m_bStopNormal = true;
    p_VCPlayer->Notify();

    RETURN_NULL;
}

TEST_F(VC_PlayerGeneral_Test, DoDecodePcm_AllocBufferNULL_m_dwBufCountZero)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    const void* p_vbuffer = buffer;

    Test_VC_AudioOutGeneral testaudio(NULL);
    testaudio.m_bstop = true;
    testaudio.m_balloc = false;
    m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;

    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_bStop = true;

    // pthread_t pid;
    // int res = pthread_create(&pid, NULL, thread_testm_bStopFalseStopTruem_bStopNormalTrue, m_pVC_PlayerGeneral);

    m_pVC_PlayerGeneral->DoDecodePcm(static_cast<const BYTE*> (p_vbuffer), 10, 0);
    delete m_pVC_PlayerGeneral->m_pListener;
}

// TEST_F(VC_PlayerGeneral_Test, DoDecodePcm_AllocBufferNULL_m_dwBufCountNotZero)
// {
//     m_pVC_PlayerGeneral->m_eventType = 1;
//     char buffer[] = "FFIR56789123mengfanbingsuntec";
//     const void* p_vbuffer = buffer;

//     Test_VC_AudioOutGeneral testaudio(NULL);
//     testaudio.m_bstop = true;
//     testaudio.m_balloc = false;
//     m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;

//     m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
//     m_pVC_PlayerGeneral->m_bStop = true;

//     // pthread_t pid;
//     // int res = pthread_create(&pid, NULL, thread_testm_bStopFalseStopTruem_bStopNormalTrue, m_pVC_PlayerGeneral);

//     m_pVC_PlayerGeneral->DoDecodePcm(static_cast<const BYTE*> (p_vbuffer), 10, 11);
//     delete m_pVC_PlayerGeneral->m_pListener;
// }

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZeroOpenTrueiFragSizeBiggerthanZeroAllocBufferNotNULLm_bStopFalseStopTruem_bStopNormaltrue)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, m_pVC_PlayerGeneral->m_pDecoder, NULL);

    Test_VC_AudioOutGeneral testaudio(NULL);
    testaudio.m_bstop = true;
    m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_bStop = true;

    pthread_t pid;
    int res = pthread_create(&pid, NULL, thread_testm_bStopFalseStopTruem_bStopNormalTrue, m_pVC_PlayerGeneral);

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlayAllocBufferNULLm_dwBufCountNotZero)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, m_pVC_PlayerGeneral->m_pDecoder, NULL);

    Test_VC_AudioOutGeneral testaudio(NULL);
    testaudio.m_balloc = false;
    m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_bStop = false;
    m_pVC_PlayerGeneral->m_dwBufCount = 1;

    // pthread_t pid;
    // int res = pthread_create(&pid, NULL, thread_testm_bStopTrue, m_pVC_PlayerGeneral);

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlayAllocBufferNULLm_dwBufCountEqualZero)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, m_pVC_PlayerGeneral->m_pDecoder, NULL);

    Test_VC_AudioOutGeneral testaudio(NULL);
    testaudio.m_balloc = false;
    m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_bStop = false;
    m_pVC_PlayerGeneral->m_dwBufCount = 0;

    // pthread_t pid;
    // int res = pthread_create(&pid, NULL, thread_testm_bStopTrue, m_pVC_PlayerGeneral);

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, DoPlaym_eventTypeNotZeroOpenTrueiFragSizeBiggerthanZeroAllocBufferNotNULLm_bStopTrueFlush)
{
    m_pVC_PlayerGeneral->m_eventType = 1;
    char buffer[] = "FFIR56789123mengfanbingsuntec";
    m_pVC_PlayerGeneral->m_pDecoder = new(MEM_Voice) VC_DecoderOgg();
    m_pVC_PlayerGeneral->m_pEvent = new(MEM_Voice) VC_DecEventOgg(buffer, 1, m_pVC_PlayerGeneral->m_pDecoder, NULL);

    Test_VC_AudioOutGeneral testaudio(m_pVC_PlayerGeneral);
    m_pVC_PlayerGeneral->m_pAudioOut = &testaudio;
    testaudio.m_dwIndexW = false;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();
    m_pVC_PlayerGeneral->m_bStop = true;

    // pthread_t pid;
    // int res = pthread_create(&pid, NULL, thread_testm_bStopTrue, m_pVC_PlayerGeneral);

    m_pVC_PlayerGeneral->DoPlay();
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pDecoder;
}

TEST_F(VC_PlayerGeneral_Test, Flush_TRUE)
{
    MockVC_AudioOutGeneral audioOutGeneral;
    EXPECT_CALL(audioOutGeneral, Write(_)).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioOutGeneral, Start(_)).WillOnce(Return(CL_TRUE));
    m_pVC_PlayerGeneral->m_pAudioOut = &audioOutGeneral;
    m_pVC_PlayerGeneral->m_dwIndexW = 1;
    EXPECT_TRUE(m_pVC_PlayerGeneral->Flush());
    m_pVC_PlayerGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_PlayerGeneral_Test, Flush_FALSE)
{
    MockVC_AudioOutGeneral audioOutGeneral;
    EXPECT_CALL(audioOutGeneral, Write(_)).WillOnce(Return(CL_FALSE));
    EXPECT_CALL(audioOutGeneral, Start(_)).WillOnce(Return(CL_TRUE));
    m_pVC_PlayerGeneral->m_pAudioOut = &audioOutGeneral;
    m_pVC_PlayerGeneral->m_dwIndexW = 1;
    EXPECT_FALSE(m_pVC_PlayerGeneral->Flush());
    m_pVC_PlayerGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_PlayerGeneral_Test, OnDecReady_StopCheck)
{
    MockVC_AudioOutGeneral* pAudioOutGeneral = new(MEM_Voice) MockVC_AudioOutGeneral();
    m_pVC_PlayerGeneral->m_pAudioOut = pAudioOutGeneral;
    m_pVC_PlayerGeneral->m_bStop = CL_TRUE;
    VC_AudioBuffer* pBuffer = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_PlayerGeneral->OnDecReady(pBuffer);

    delete pAudioOutGeneral;
    delete pBuffer;
}


TEST_F(VC_PlayerGeneral_Test, OnDecReady)
{
    MockVC_AudioOutGeneral audioOutGeneral;
    EXPECT_CALL(audioOutGeneral, Write(_)).WillOnce(Return(CL_TRUE));
    m_pVC_PlayerGeneral->m_pAudioOut = &audioOutGeneral;
    m_pVC_PlayerGeneral->m_bStop = CL_FALSE;
    m_pVC_PlayerGeneral->m_dwIndexW = 1;
    m_pVC_PlayerGeneral->m_iFragCount = 0;
    VC_AudioBuffer* pBuffer = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_PlayerGeneral->OnDecReady(pBuffer);
    m_pVC_PlayerGeneral->m_pAudioOut = NULL;
    delete pBuffer;
}

TEST_F(VC_PlayerGeneral_Test, OnDecReady_Flush)
{
    MockVC_AudioOutGeneral audioOutGeneral;
    EXPECT_CALL(audioOutGeneral, Write(_)).WillOnce(Return(CL_TRUE));
    EXPECT_CALL(audioOutGeneral, Start(_)).WillOnce(Return(CL_TRUE));
    m_pVC_PlayerGeneral->m_pAudioOut = &audioOutGeneral;
    m_pVC_PlayerGeneral->m_bStop = CL_FALSE;
    m_pVC_PlayerGeneral->m_dwIndexW = 0;
    m_pVC_PlayerGeneral->m_iFragCount = 1;
    VC_AudioBuffer* pBuffer = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_PlayerGeneral->OnDecReady(pBuffer);
    m_pVC_PlayerGeneral->m_pAudioOut = NULL;
    delete pBuffer;
}

TEST_F(VC_PlayerGeneral_Test, OnDecReady_m_dwIndexWSmallerThanm_iFragCount)
{
    m_pVC_PlayerGeneral->m_bStop = CL_FALSE;
    m_pVC_PlayerGeneral->m_dwIndexW = 0;
    m_pVC_PlayerGeneral->m_iFragCount = 10;
    VC_AudioBuffer* pBuffer = new(MEM_Voice) VC_AudioBufferGeneral(0);
    m_pVC_PlayerGeneral->OnDecReady(pBuffer);
    delete pBuffer;
}

TEST_F(VC_PlayerGeneral_Test, OnDecBuffer_NullCheck)
{
    m_pVC_PlayerGeneral->m_bStop = CL_TRUE;
    m_pVC_PlayerGeneral->OnDecBuffer();
}

void* thread_testOnDecBuffer(void* par)
{
    if (NULL == par) {
        RETURN_NULL;
    }

    VC_PlayerGeneral* p_VCPlayer = static_cast<VC_PlayerGeneral*> (par);
    usleep(2000);
    p_VCPlayer->m_dwIndexW = 0;
    p_VCPlayer->Notify();

    RETURN_NULL;
}

TEST_F(VC_PlayerGeneral_Test, OnDecBufferwaitm_dwBufCountZero)
{
   m_pVC_PlayerGeneral->m_bStop = CL_FALSE;
   m_pVC_PlayerGeneral->m_dwIndexR = 1;
   m_pVC_PlayerGeneral->m_dwIndexW = 3;
   m_pVC_PlayerGeneral->m_dwBufCount = 0;
   pthread_t pid;
   int res = pthread_create(&pid, NULL, thread_testOnDecBuffer, m_pVC_PlayerGeneral);
   m_pVC_PlayerGeneral->OnDecBuffer();
}

TEST_F(VC_PlayerGeneral_Test, OnDecBufferBiggerm_dwBufCountNotZero)
{
    m_pVC_PlayerGeneral->m_bStop = CL_FALSE;
    m_pVC_PlayerGeneral->m_dwIndexR = 5;
    m_pVC_PlayerGeneral->m_dwIndexW = 3;
    m_pVC_PlayerGeneral->m_dwBufCount = 3;

    VC_AudioOutGeneral* audioOutGeneral = new(MEM_Voice) VC_AudioOutGeneral(1);
    audioOutGeneral->m_state = VC_AudioOutGeneral::ST_IDLE;
    m_pVC_PlayerGeneral->m_pAudioOut = audioOutGeneral;
    m_pVC_PlayerGeneral->m_buffers[0] = m_pVC_PlayerGeneral->m_pAudioOut->AllocBuffer(10);
    // pthread_t pid;
    // int res = pthread_create(&pid, NULL, thread_testOnDecBuffer, m_pVC_PlayerGeneral);
    m_pVC_PlayerGeneral->OnDecBuffer();
    delete audioOutGeneral;
}

// TEST_F(VC_PlayerGeneral_Test, IsPCM)
// {
//    const VOID *buf = NULL;
//    m_pVC_PlayerGeneral->IsPCM(buf);
// }

TEST_F(VC_PlayerGeneral_Test, GetVolume)
{
    MockVC_AudioOutGeneral audioOutGeneral;
    m_pVC_PlayerGeneral->m_pAudioOut = &audioOutGeneral;
    EXPECT_CALL(audioOutGeneral, GetVolume()).WillOnce(Return(2));
    INT vol = m_pVC_PlayerGeneral->GetVolume();
    EXPECT_EQ(vol, 2);
    m_pVC_PlayerGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_PlayerGeneral_Test, SetVolume_NullCheck)
{
    m_pVC_PlayerGeneral->m_pAudioOut = NULL;
    CL_ERROR err = m_pVC_PlayerGeneral->SetVolume(2);
    EXPECT_EQ(err, CL_ERROR(APL_ERR_VC_INVALID_OP));
}

TEST_F(VC_PlayerGeneral_Test, SetVolume)
{
    MockVC_AudioOutGeneral audioOutGeneral;
    EXPECT_CALL(audioOutGeneral, SetVolume(_)).WillOnce(Return(CL_TRUE));
    m_pVC_PlayerGeneral->m_pAudioOut = &audioOutGeneral;
    CL_ERROR err = m_pVC_PlayerGeneral->SetVolume(2);
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
    m_pVC_PlayerGeneral->m_pAudioOut = NULL;
}

TEST_F(VC_PlayerGeneral_Test, DoDecodePcmOpenFailedParseFailed)
{
    printf("testXXXXXXXXXXXX\n");
    BYTE buffer[] = "123456789123mengfanbingsuntec";
    MockVC_AudioOutGeneral audioout;
    printf("testXXXXXXXXXXXX2\n");
    m_pVC_PlayerGeneral->m_pAudioOut = &audioout;
    printf("int....\n");
    EXPECT_CALL(audioout, Open(_, _, _, _)).WillRepeatedly(Return(false));

    printf("before ----------------------\n");
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

    printf("before DoDecodePcm\n");
    EXPECT_EQ(APL_ERR_NO_ERROR, m_pVC_PlayerGeneral->DoDecodePcm(buffer, 10, 0));
    printf("after DoDecodePcm\n");
    delete m_pVC_PlayerGeneral->m_pListener;
}

TEST_F(VC_PlayerGeneral_Test, DoDecodePcmOpenFailedParseFailed_FragSizeBiggerThanZero)
{
    printf("testXXXXXXXXXXXX\n");
    BYTE buffer[] = "123456789123mengfanbingsuntec";
    Test_VC_AudioOutGeneral audioout(NULL);
    printf("testXXXXXXXXXXXX2\n");
    m_pVC_PlayerGeneral->m_pAudioOut = &audioout;
    printf("int....\n");

    printf("before ----------------------\n");
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

    printf("before DoDecodePcm\n");
    EXPECT_EQ(APL_ERR_NO_ERROR, m_pVC_PlayerGeneral->DoDecodePcm(buffer, 10, 10));
    printf("after DoDecodePcm\n");
    delete m_pVC_PlayerGeneral->m_pListener;
}

/**
 * Summary for class VC_AudioInGeneral
 * 
 * This is a detail description for class VC_AudioInGeneral
 *
 */
struct PlayerGeneralAudioOut
{
    /**
     * Summary for class VC_AudioInGeneral
     * 
     * This is a detail description for class VC_AudioInGeneral
     *
     */
    VC_AudioOutGeneral*  audioout;

    /**
     * Summary for class VC_AudioInGeneral
     * 
     * This is a detail description for class VC_AudioInGeneral
     *
     */
    VC_PlayerGeneral*    player;
};

void* thread_testDoDecodePcmm_bStopTrue(void* par)
{
    if (NULL == par) {
        RETURN_NULL;
    }

    usleep(2000);
    PlayerGeneralAudioOut* p_VC = static_cast<PlayerGeneralAudioOut*> (par);
    VC_PlayerGeneral* p_VCPlayer = p_VC->player;
    p_VCPlayer->m_bStop = false;
    p_VCPlayer->m_bStopNormal = true;
    p_VCPlayer->Notify();

    usleep(20000);
    VC_AudioOutGeneral* p_AudioOut = p_VC->audioout;
    p_AudioOut->m_state = VC_AudioOutGeneral::ST_NONE;
    p_AudioOut->m_waitStop.Notify();

    RETURN_NULL;

    // VC_PlayerGeneral* p_VCPlayer = static_cast<VC_PlayerGeneral*> (par);
    // p_VCPlayer->m_bStop = false;
    // p_VCPlayer->m_bStopNormal = true;
    // p_VCPlayer->Notify();
    // RETURN_NULL;
}


TEST_F(VC_PlayerGeneral_Test, DoDecodePcmOpenTrueReturnm_dwBufCountZeropBufNotNULL)
{
    BYTE buffer[] = "123456789123mengfanbingsuntec";

    VC_AudioOutGeneral* audiooutgeneral = new(MEM_Voice) VC_AudioOutGeneral(10);
    m_pVC_PlayerGeneral->m_pAudioOut = audiooutgeneral;
    audiooutgeneral->m_bIsThreadStart = CL_TRUE;
    audiooutgeneral->m_stop = VC_AudioOutGeneral::STOP_FORCE;
    MockVC_IAudioOutMedia* mockmedia = new MockVC_IAudioOutMedia();
    audiooutgeneral->m_pAudioOut = mockmedia;
    EXPECT_CALL(*mockmedia, Open(_, _, _, _)).WillRepeatedly(Return(true));
    EXPECT_CALL(*mockmedia, Close()).WillRepeatedly(Return(true));
    m_pVC_PlayerGeneral->m_iFragCount = 0;

    audiooutgeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
    m_pVC_PlayerGeneral->m_dwBufCount = 0;
    m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

    PlayerGeneralAudioOut structplayer;
    structplayer.audioout = audiooutgeneral;
    structplayer.player = m_pVC_PlayerGeneral;
    pthread_t pid;
    pthread_create(&pid, NULL, thread_testDoDecodePcmm_bStopTrue, &structplayer);

    EXPECT_EQ(APL_ERR_NO_ERROR, m_pVC_PlayerGeneral->DoDecodePcm(buffer, 10, 0));
    delete m_pVC_PlayerGeneral->m_pListener;
    delete m_pVC_PlayerGeneral->m_pAudioOut;
}

// temp noted
// TEST_F(VC_PlayerGeneral_Test, DoDecodePcmOpenTruebreak)
// {
//     BYTE buffer[] = "123456789123datamengfanbing";
//     VC_AudioOutGeneral* audiooutgeneral = new(MEM_Voice) VC_AudioOutGeneral(10);
//     m_pVC_PlayerGeneral->m_pAudioOut = audiooutgeneral;
//     audiooutgeneral->m_bIsThreadStart = CL_TRUE;
//     m_pVC_PlayerGeneral->m_iFragCount = 0;

//     MockVC_IAudioOutMedia* mockmedia = new MockVC_IAudioOutMedia();
//     audiooutgeneral->m_pAudioOut = mockmedia;
//     EXPECT_CALL(*mockmedia, Open(_, _, _, _)).WillRepeatedly(Return(true));

//     m_pVC_PlayerGeneral->m_pListener = new VC_PlayerGeneral_Listener_test();

//     audiooutgeneral->m_state = VC_AudioOutGeneral::ST_RUNNING;
//     audiooutgeneral->m_stop = VC_AudioOutGeneral::STOP_FORCE;
//     m_pVC_PlayerGeneral->m_dwBufCount = 1;
//     m_pVC_PlayerGeneral->m_bStop = true;
//     m_pVC_PlayerGeneral->m_dwIndexW = 0;

//     EXPECT_EQ(APL_ERR_NO_ERROR, m_pVC_PlayerGeneral->DoDecodePcm(buffer, 10, 0));
//     delete m_pVC_PlayerGeneral->m_pListener;
//     delete m_pVC_PlayerGeneral->m_pAudioOut;

// }

TEST_F(VC_PlayerGeneral_Test, StopPlayer)
{
    VC_AudioOutGeneral* audiooutgeneral = new(MEM_Voice) VC_AudioOutGeneral(10);
    m_pVC_PlayerGeneral->m_pAudioOut = audiooutgeneral;
    
    m_pVC_PlayerGeneral->StopPlayer();

    delete m_pVC_PlayerGeneral->m_pAudioOut;
}

/* EOF */
