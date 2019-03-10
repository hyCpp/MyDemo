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
#include "TTS_TextToSpeechImpl_test.h"
#include "TTS_TextToSpeechImpl.h"
#include "TTS_Controler.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_ReceiverCb.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_IF.h"

using namespace std;
using ::testing::_;
using ::testing::Return;

/**
* MockTTS_Controler
*
* The class is just for TTS_TextToSpeechImpl test.
*/

class MockTTS_Controler : public TTS_Controler
{
public:
    MockTTS_Controler() {}
    virtual ~MockTTS_Controler() {}

    MOCK_METHOD1(OnEvent, CL_BOOL(TTS_Event* event));
    MOCK_METHOD2(GetSoundName, BL_String(const BL_String &sender, int soundID));

};

/**
* TestTextToSpeechImpl
*
* The class is just for TTS_TextToSpeechImpl test.
*/

class TestTextToSpeechImpl : public testing::Test
{
public:
    TestTextToSpeechImpl() {}
    virtual ~TestTextToSpeechImpl() {}

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

public:

};

TEST(TestTextToSpeechImpl, PreInitialize)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.PreInitialize();
}

TEST(TestTextToSpeechImpl, PreInitialize_null)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_control = NULL;
    impl.m_playThread = NULL;
    impl.m_engineManager = NULL;
    impl.PreInitialize();
}

TEST(TestTextToSpeechImpl, Initialize)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.Initialize();
}

TEST(TestTextToSpeechImpl, Initialize_null)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_control = NULL;
    impl.m_playThread = NULL;
    impl.m_engineManager = NULL;
    impl.Initialize();
    impl.Start();
}

TEST(TestTextToSpeechImpl, Initializei_frist_app)
{
    TTS_TextToSpeechImpl impl(NULL);
    // impl.m_bFirstAppShow = true;
    impl.Initialize();
}

TEST(TestTextToSpeechImpl, LoadEngine_null)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_engineManager = NULL;
    impl.LoadEngine();
}

TEST(TestTextToSpeechImpl, LoadEngine_has_load)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_engineManager->m_loadedEngine = true;
    impl.LoadEngine();
}

TEST(TestTextToSpeechImpl, LoadEngine)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_engineManager->m_loadedEngine = false;
    impl.LoadEngine();
}

TEST(TestTextToSpeechImpl, Stop)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.Stop();
}

TEST(TestTextToSpeechImpl, Stop_null)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_control = NULL;
    impl.m_playThread = NULL;
    impl.m_engineManager = NULL;
    impl.Stop();
}

TEST(TestTextToSpeechImpl, Stop_false)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(false));

    impl.Stop("sender", 1);
}

TEST(TestTextToSpeechImpl, stop_true)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));

    impl.Stop("sender", 1);
}



TEST(TestTextToSpeechImpl, StopAll_false)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(false));

    impl.StopAll("sender");
}

TEST(TestTextToSpeechImpl, StopAll_true)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));

    impl.StopAll("sender");
}

TEST(TTS_TextToSpeech, Destory)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.Destory();
}

TEST(TTS_TextToSpeech, Destory_NULL)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_control = NULL;
    impl.Destory();
}

TEST(TestTextToSpeechImpl, ListRole)
{
    TTS_TextToSpeechImpl impl(NULL);
    TTS_EngineManager* manager = new(MEM_Voice) TTS_EngineManager();
    impl.m_engineManager = manager;
    TtsEngineRoles roles;
    impl.ListRole(roles);
}


TEST(TestTextToSpeechImpl, GetParameter)
{
    TTS_TextToSpeechImpl impl(NULL);
    TTS_EngineManager* manager = new(MEM_Voice) TTS_EngineManager();
    impl.m_engineManager = manager;

    TTS_EngineLanguage param;
    impl.GetParameter("sender", "ENG", param);
    // EXPECT_NE(-1, value);
}

TEST(TestTextToSpeechImpl, SetParameter)
{
    TTS_TextToSpeechImpl impl(NULL);
    TTS_EngineManager *manager = new(MEM_Voice) TTS_EngineManager();
    impl.m_engineManager = manager;
    TTS_EngineLanguage param;
    impl.SetParameter("test", "eng", param);
}

TEST(TestTextToSpeechImpl, RegisterSound)
{
    TTS_TextToSpeechImpl impl(NULL);
    BL_String sender = "test";
    BL_String pathName = "aa";
    int soundId;
    impl.RegisterSound(sender, pathName, &soundId);
}

TEST(TestTextToSpeechImpl, RegisterSound_NULL)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_control = NULL;
    BL_String sender = "test";
    BL_String pathName = "aa";
    int soundId;
    impl.RegisterSound(sender, pathName, &soundId);
}

TEST(TestTextToSpeechImpl, UnRegisterSound)
{
    TTS_TextToSpeechImpl impl(NULL);
    BL_String sender = "test";
    int soundId = 1;
    impl.UnRegisterSound(sender, soundId);
}

TEST(TestTextToSpeechImpl, UnRegisterSound_NULL)
{
    TTS_TextToSpeechImpl impl(NULL);
    impl.m_control = NULL;
    BL_String sender = "test";
    int soundId = 1;
    impl.UnRegisterSound(sender, soundId);
}

TEST(TestTextToSpeechImpl, CreatePlayList)
{
    TTS_TextToSpeechImpl impl(NULL);
    BL_String sender = "test";
    EXPECT_TRUE(NULL != impl.CreatePlayList(sender));
}

TEST(TestTextToSpeechImpl, PlayList2)
{
    TTS_TextToSpeechImpl impl(NULL);
    BL_String sender = "test";
    int reqId;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ABORT_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    EXPECT_EQ(TTS_ERROR_NO_ERROR, impl.Play(playlist, policy, &reqId));
}


TEST(TestTextToSpeechImpl, OnPlayEnd1)
{
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    BL_String sender = "test";
    int reqId = 1;
    TTS_ReceiverCb cb;
    impl.OnPlayEnd(TTS_PLAY_OK, sender, reqId);
}

TEST(TestTextToSpeechImpl, OnPlayState)
{
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    BL_String sender = "test";
    int reqId = 1;
    TTS_ReceiverCb cb;
    impl.OnPlayState(reqId, sender, TTS_PLAYER_START, 1);
}

TEST(TestTextToSpeechImpl, OnPlayEnd_playlist_null)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));

    impl.OnPlayEnd(NULL, TTS_PLAYER_START);
}

TEST(TestTextToSpeechImpl, OnPlayEnd_playlist)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    TTS_PlaylistInfo *playlist = new TTS_PlaylistInfo("test", NULL);

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(false));

    impl.OnPlayEnd(playlist, TTS_PLAYER_START);
}

TEST(TestTextToSpeechImpl, OnPlayEnd_Pause_true)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));

    impl.Pause(1, "sender");
}

TEST(TestTextToSpeechImpl, OnPlayEnd_Pause_false)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;


    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(false));

    impl.Pause(1, "sender");
}

TEST(TestTextToSpeechImpl, OnPlayEnd_Resume_true)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));

    impl.Resume(1, "sender");
}

TEST(TestTextToSpeechImpl, OnPlayEnd_Resume_false)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(false));

    impl.Resume(1, "sender");
}

#if 0
TEST(TestTextToSpeechImpl, RegisterTuningDatatrue)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));
    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));
    impl.RegisterTuningData("sender", NULL);
}

TEST(TestTextToSpeechImpl, RegisterTuningData_false)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;


    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(false));

    impl.RegisterTuningData("sender", NULL);
}

#endif

TEST(TestTextToSpeechImpl, SynthesizeToFile)
{
    TTS_TextToSpeechImpl impl(NULL);
    BL_String text     = "abcdefg";
    BL_String fileName = "test_file";
    BL_String sender   = "test";
    int reqId = 0;
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_GENERAL_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_CURRENT_T;

    CL_ERROR err = impl.SynthesizeToFile(sender, text, fileName, policy, &reqId);
    EXPECT_EQ(TTS_ERROR_NO_ERROR, err);
}

TEST(TestTextToSpeechImpl, SynthesizeToFile_Failed)
{
    TTS_TextToSpeechImpl impl(NULL);


    BL_String text     = "abcdefg";
    BL_String fileName = "test_file";
    BL_String sender   = "test";
    int reqId = 0;
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_GENERAL_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_CURRENT_T;

    CL_ERROR err = impl.SynthesizeToFile(sender, text, fileName, policy, &reqId);
    EXPECT_EQ(TTS_ERROR_NO_ERROR, err);
}

TEST(TestTextToSpeechImpl, Play_true)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));

    TTS_PlaylistInfo playlist("test", NULL);
    TTS_POLICY policy;
    int req;

    impl.Play(&playlist, policy, &req);
}

TEST(TestTextToSpeechImpl, Play_load_engine)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;
    impl.m_engineManager->m_loadedEngine = true;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));

    TTS_PlaylistInfo playlist("test", NULL);
    TTS_POLICY policy;
    int req;

    impl.Play(&playlist, policy, &req);
}

#if 0
TEST(TestTextToSpeechImpl, Play_false)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(false));

    TTS_PlaylistInfo playlist("test", NULL);
    TTS_POLICY policy;
    int req;

    impl.Play(&playlist, policy, &req);
}

#endif

TEST(TestTextToSpeechImpl, PlayImpl_true)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));

    TTS_PlaylistInfo playlist("test", NULL);
    int req;

    impl.PlayImpl("sender", &playlist, &req);
}

TEST(TestTextToSpeechImpl, PlayImpl_req_null)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;

    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(true));
    TTS_PlaylistInfo playlist("test", NULL);

    impl.PlayImpl("sender", &playlist, NULL);
}

TEST(TestTextToSpeechImpl, PlayImpl_false)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;


    EXPECT_CALL(*control, OnEvent(_)).WillOnce(Return(false));

    TTS_PlaylistInfo* playlist = new(MEM_Voice) TTS_PlaylistInfo("test", NULL);
    int req = 1;

    impl.PlayImpl("sender", playlist, &req);
}


TEST(TestTextToSpeechImpl, GetSoundName)
{
    MockTTS_Controler* control = new(MEM_Voice) MockTTS_Controler();
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);
    impl.m_control = control;


    EXPECT_CALL(*control, GetSoundName(_, _)).WillOnce(Return(""));

    impl.GetSoundName("sender", 1);
}

TEST(TestTextToSpeechImpl, GetDefaultLanguage)
{
    TTS_PlayListener listener;
    TTS_TextToSpeechImpl impl(&listener);

    impl.GetDefaultLanguage();
}

/* EOF */
