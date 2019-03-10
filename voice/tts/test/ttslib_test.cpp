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

#include "ttslib_test.h"
#include "BL_AplSystem.h"
#include "CL_WaitObj.h"
#include "TTS_IF.h"
#include "TTS_Controler.h"
#include "TTS_ReceiverCb.h"
#include <dlfcn.h>
#include <unistd.h>
#include "BL_Thread.h"
#include "gmock/gmock.h"
// #include "TTS_PlayThread.h"
#include "TTS_PlaylistInfo.h"
// #include "TTS_PlayThread_test.h"
#include "EV_EventSystem.h"
#include "BL_SystemLanguage_DEF.h"
#include "TTS_TextToSpeechImpl.h"
// #include ""
// #include "VP_Log.h"

// using namespace testing;
// using namespace testing::internal;
/* using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;
using ::testing::ByRef;
using ::testing::Eq;
using ::testing::Matcher;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
// using  ::testing::AtLeast;
using  ::testing::internal::UntypedFunctionMockerBase; */

#define T_LOG(...)	printf(__VA_ARGS__);\
                    printf("\n");


/* static void TestCase1()
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF1";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "线程1, 在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾";
    int reqId = 100;
    int soundId1 = -1;
    int soundId2 = -1;
    tts->RegisterSound(sender, soundName1, &soundId1);
    tts->RegisterSound(sender, soundName2, &soundId2);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundId2);
    playlist->AddSilence(5000);
    playlist->AddSound(soundId1);
    T_LOG("operation=%d....................", (TTS_OP_MASK)(TTS_OP_PRI_NORMAL | TTS_OP_RESUME_RESTART));
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ABORT_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->Play(playlist, policy , &listener, &reqId);
    sleep(5);
    str = "线程1, 我是高优先级的";
    reqId = 101;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);

    reqId = 102;
    str = "线程1, 按照以上命令执行，在执行的时候，会出现以下错误";
    tts->Play(sender, str, (TTS_OP_MASK)(TTS_OP_PRI_HIGH | TTS_OP_QUEUE_ADD ) , &listener, &reqId);
    sleep(70);
} */

/* static void TestCase2()
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF2";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "线程2, 在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾";
    int reqId = 100;
    int soundId1 = -1;
    int soundId2 = -1;
    tts->RegisterSound(sender, soundName1, &soundId1);
    tts->RegisterSound(sender, soundName2, &soundId2);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundId2);
    playlist->AddSound(soundId1);
    tts->Play(playlist, (TTS_OP_MASK)(TTS_OP_PRI_NORMAL | TTS_OP_RESUME_CONTINUE) , &listener, &reqId);
    sleep(5);
    str = "线程2, 我是高优先级的";
    reqId = 101;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);

    reqId = 102;
    str = "线程2, 按照以上命令执行，在执行的时候，会出现以下错误";
    tts->Play(sender, str, (TTS_OP_MASK)(TTS_OP_PRI_HIGH | TTS_OP_QUEUE_ABORT ) , &listener, &reqId);

    reqId = 103;
    str = "线程2, 安装时，请关闭，使用管理员权限运行";
    tts->Play(sender, str, (TTS_OP_MASK)(TTS_OP_PRI_HIGH | TTS_OP_QUEUE_ADD ) , &listener, &reqId);

    reqId = 103;
    str = "线程2, 在这条语句中，i是左值，0是临时值";
    tts->Play(sender, str, (TTS_OP_MASK)(TTS_OP_PRI_HIGH | TTS_OP_QUEUE_ADD ) , &listener, &reqId);

    sleep(70);
} */

/* static void TestCase3()
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF3";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "线程3, 在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾";
    int reqId = 300;
    int soundId1 = -1;
    int soundId2 = -1;
    tts->RegisterSound(sender, soundName1, &soundId1);
    tts->RegisterSound(sender, soundName2, &soundId2);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundId2);
    playlist->AddSound(soundId1);
    tts->Play(playlist, (TTS_OP_MASK)(TTS_OP_PRI_NORMAL | TTS_OP_RESUME_ABORT) , &listener, &reqId);
    sleep(5);
    str = "线程3, 我是高优先级的";
    reqId = 301;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);

    reqId = 302;
    str = "线程3, 按照以上命令执行，在执行的时候，会出现以下错误";
    tts->Play(sender, str, (TTS_OP_MASK)(TTS_OP_PRI_HIGH | TTS_OP_QUEUE_ADD ) , &listener, &reqId);
    sleep(5);
    tts->StopAll(sender);
    sleep(10);
} */

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    // void *handler = dlopen("/usr/lib/libtts - navi.so", RTLD_LAZY);
    BL_AplSystem system;
    system.Initialize("TTS");
    TTS_ReceiverCb *rb = new(MEM_Voice) TTS_ReceiverCb();
    rb->Initialize();
    rb->Start();

    EV_EventConfigure config;
    EV_EventSystem::Instance()->Initialize("TTS", config);

    EV_EventSystem::Instance()->Start();
    
    return RUN_ALL_TESTS();
}

#if 0
TEST(test_tts, tts_play_stop_navi)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String str = "如果大家想往这两个分区写数据";
    int reqId = 100;
    // BL_String str = "Kamu sudah menemukan?";
    BL_String sender = "testIF";
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_NAVI_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    /* tts->SetLanguage(sender, TTS_LANG_INDONESIAN);
    // tts->Play(sender, str, policy , &listener, &reqId);

    // sleep(1000000000);

    str = "Eu amo - te, Eu amo - te, Eu amo - te, Eu amo - te";
    reqId = 101;
    tts->SetLanguage(sender, TTS_LANG_PORTUGUESE_BRAZIL);
    tts->Play(sender, str, policy , &listener, &reqId);

    sleep(10); */

    /* str = "استمر على الطريق الحالي ثم وجهتك على اليسار.";
    reqId = 102;
    tts->SetLanguage(sender, TTS_LANG_ARABIC);
    // tts->Play(sender, str, policy , &listener, &reqId);
    BL_String path = "/data/test.wav";
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);
    tts->Play(sender, str, policy , &listener, &reqId); */

    tts->SetLanguage(sender, TTS_LANG_UK_ENGLISH);
    BL_String path = "/data/test1.wav";
    str = "Continue on the current road, and then your destination is on the left.";
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test2.wav";
    str = "In ninety kilometers, make a U - turn if possible.";
    reqId = 101;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test3.wav";
    str = "In eight hundred metres, continue straight on.";
    reqId = 103;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test4.wav";
    str = "In one tenth of a mile, bear right, onto the motorway entrance.";
    reqId = 104;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test5.wav";
    str = "In five hundred metres, you will arrive at your destination. ";
    reqId = 105;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test6.wav";
    str = "Overdid it , and think it might happen again? ";
    reqId = 106;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test7.wav";
    str = "Before you start feeling guilty, know that taking a too - strict approach after a binge will most certainly backfire.";
    reqId = 107;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test8.wav";
    str = "Here, the habits guilt - trippers get trapped in, and better ways to get on track.";
    reqId = 108;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test9.wav";
    str = "You know that your weight can fluctuate a lot from day to day";
    reqId = 109;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    path = "/data/test10.wav";
    str = "Ample research shows that people who eat breakfast weigh less and eat healthier than people who skip it. ";
    reqId = 110;
    tts->SynthesizeToFile(sender, str, path, policy, &listener, &reqId);

    // sleep(10);

    wait.Wait();
    wait.Wait();
    wait.Wait();
}

#endif

#if 0
TEST(test_tts, tts_synthesizeToFile)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener *listener = new(MEM_Voice) Listener(wait);
    BL_String str = "<TTS_Phoneme>\"dZa|lan \"ra|ja \"pOn|dO? g@|*\"de</TTS_Phoneme>";
    BL_String path = "/udisk/geda.wav";
    int reqId = 100;
    BL_String sender = "testIF";
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_NAVI_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->SetLanguage(sender, TTS_LANG_INDONESIAN);
    tts->SynthesizeToFile(sender, str, path, policy, listener, &reqId);

    reqId = 102;
    str = "<TTS_Phoneme>\"dZa|lan \"ka \"ha \"mas *\"man|SUr</TTS_Phoneme>";
    path = "/udisk/mas.wav";
    Listener *listener2 = new(MEM_Voice) Listener(wait);
    tts->SynthesizeToFile(sender, str, path, policy, listener2, &reqId);
    wait.Wait();
    wait.Wait();
    wait.Wait();
}

#endif

/*
TEST(test_tts, tts_play)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String str = "大概在 1992 年";
    int reqId = 100;
    BL_String sender = "testIF";
    tts->SetParameter(sender, TTS_PARAM_ROLE, 3);
    tts->SetParameter(sender, TTS_PARAM_PITCH, 50);
    tts->SetParameter(sender, TTS_PARAM_VOLUME, 100);
    tts->SetParameter(sender, TTS_PARAM_SPEED, 100);
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ABORT_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->Play(sender, str, policy , &listener, &reqId);
    wait.Wait();
    wait.Wait();
    wait.Wait();
    wait.Wait();
    wait.Wait();
    sleep(10000000);

}
*/
/* TEST(test_tts, tts_play)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String str = "大概在 1992 年";
    int reqId = 100;
    BL_String sender = "testIF";
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ABORT_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->Play(sender, str, policy , &listener, &reqId);

    str = "可能会有一些不正常的通知!";
    reqId = 101;
    tts->Play(sender, str, policy , &listener, &reqId);

    sleep(1);
    sender = "VoiceRecog";
    str = "账号权限已经对应中，下周可以对应完成!";
    reqId = 102;
    tts->Play(sender, str, policy , &listener, &reqId);
    wait.Wait();
    wait.Wait();
    wait.Wait();
} */

/* TEST(test_tts, tts_play_stop)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String str = "大概在 1992 年常见的测试命令如下表所示";
    int reqId = 100;
    BL_String sender = "testIF";
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_GENERAL_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->Play(sender, str, policy , &listener, &reqId);

    sleep(1);
    str = "音认的请求1";
    reqId = 101;
    tts->SetVrMode(true);
    BL_String sender2 = "VoiceRecog";
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_ABORT_T;
    tts->Play(sender2, str, policy , &listener, &reqId);
    str = "请求2";
    reqId = 102;
    tts->Play(sender2, str, policy , &listener, &reqId);
    str = "请求3";
    reqId = 103;
    tts->Play(sender, str, policy , &listener, &reqId);
    tts->SetVrMode(false);
    str = "请求4";
    reqId = 104;
    tts->Play(sender, str, policy , &listener, &reqId);
    sleep(10000000);
} */
/*
TEST(test_test, tts_play_soundName)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    AL_String soundName = "/var/Washington_Song_Hospital.wav";
    int reqId = 100;
    AL_String sender = "testIF";
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_GENERAL_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    // tts->Play(sender, str, policy , &listener, &reqId);

    tts->PlaySound("testIF", soundName, policy, &listener, &reqId);
    wait.Wait();
    // tts->Stop("testIF", reqId);
    wait.Wait();
}
*/
#if 0
TEST(test_tts, tts_play_stop_navi)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String str = "大概在 1992 年常见的测试命令如下表所示1";
    int reqId = 100;
    BL_String sender = "testIF";
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_GENERAL_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->Play(sender, str, policy, &listener, &reqId);

    str = "大概在 1992 年常见的测试命令如下表所示2";
    reqId = 101;
    tts->Play(sender, str, policy, &listener, &reqId);

    sleep(1);
    str = "Navi的请求1";
    reqId = 201;
    BL_String sender2 = "VoicePlay";
    policy.priority = TTS_OP_PRI_NAVI_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_ABORT_T;
    tts->Play(sender2, str, policy, &listener, &reqId);
    str = "请求2";
    reqId = 202;
    tts->Play(sender2, str, policy, &listener, &reqId);
    str = "请求3";
    reqId = 203;
    tts->Play(sender, str, policy, &listener, &reqId);
    str = "请求4";
    reqId = 204;
    tts->Play(sender, str, policy, &listener, &reqId);
    sleep(10000000);
}

#endif

/* TEST(test_tts, tts_synthesizeFile)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String str = "大概在 1992 年常见的测试命令如下表所示";
    int reqId = 100;
    BL_String sender = "testIF";
    BL_String file = "/udisk/test.wav";
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->SynthesizeToFile(sender, str, file, policy , &listener, &reqId);
    usleep(900);
    tts->Stop(sender, reqId);
    sleep(10000000);
} */

#if 0
TEST(test_tts, tts_setparam)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String str = "大概在 1992 年";
    int reqId = 100;
    BL_String sender = "testIF";
    /* tts->SetParameter(sender, TTS_PARAM_ROLE, 2);
    tts->SetParameter(sender, TTS_PARAM_PITCH, 51);
    tts->SetParameter(sender, TTS_PARAM_VOLUME, 99);
    tts->SetParameter(sender, TTS_PARAM_SPEED, 99); */
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    // tts->Play(sender, str, policy, &listener, &reqId);
    tts->GetParameter(sender, TTS_PARAM_ROLE);
    tts->GetParameter(sender, TTS_PARAM_PITCH);
    tts->GetParameter(sender, TTS_PARAM_VOLUME);
    tts->GetParameter(sender, TTS_PARAM_SPEED);
    wait.Wait();
    wait.Wait();
    sleep(1000000);
}

#endif

#if 0
TEST(test_tts, tts_setparam)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String str = "大概在 1992 年";
    int reqId = 100;
    BL_String sender = "testIF";
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->SetLanguage(sender, TTS_LANG_UK_ENGLISH);
    wait.Wait();
    wait.Wait();
    sleep(1000000);
}

#endif

/* TEST(test_test, tts_play_soundName)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String soundName = "01.wav";
    int reqId = 100;
    tts->PlaySound("testIF", soundName, TTS_OP_PRI_HIGH, &listener, &reqId);
    wait.Wait(3000);
    tts->Stop("testIF", reqId);
    wait.Wait();
} */

/* TEST(test_test, tts_play_text_soundName)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String soundName = "212.wav";
    BL_String str = "大概在1992年, 一家称作Nombas的公司开发了一种叫做C减减的嵌入式脚本语言";
    int reqId = 100;
    BL_String sender = "test";
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    // playlist->AddText(str);
    playlist->AddSound(soundName);
    TTS_POLICY policy;
    policy.priority = TTS_OP_PRI_VR_T;
    policy.queuePolicy = TTS_OP_QUEUE_ADD_T;
    policy.resumePolicy = TTS_OP_RESUME_RESTART_T;
    tts->Play(playlist, policy, &listener, &reqId);
    // wait.Wait(3000);
    // tts->Stop("testIF", reqId);
    // wait.Wait();
    sleep(200000);
} */

/* TEST(test_test, tts_play_soundId)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    BL_String soundName = "01.wav";
    int reqId = 100;
    int soundId = -1;
    tts->RegisterSound("testIF", soundName, &soundId);
    T_LOG("soundId=%d", soundId);
    tts->Play("testIF", soundId, TTS_OP_PRI_HIGH, &listener, &reqId);
    // wait.Wait(3000);
    // tts->Stop("testIF", reqId);
    wait.Wait();
} */

/* TEST(test_test, tts_test_listparamrange)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    int reqId = 100;
    int count = 0;
    wait.Wait(1000);
    tts->ListParamRange(NULL, &count);
    T_LOG("...................count = %d", count);
    TTS_ParamRange *params[count];
    for (int i = 0; i < count; ++i) {
        params[i] = new(MEM_Voice) TTS_ParamRange();
    }
    tts->ListParamRange(params, &count);
    for (int i = 0; i < count; ++i) {
        T_LOG("engine=%s, type=%d, min=%d, max=%d, def=%d", params[i]->engineName.GetString(), params[i]->type, params[i]->min, params[i]->max, params[i]->def);	
    }
    // wait.Wait(3000);
    // tts->Stop("testIF", reqId);
    // wait.Wait();
} */

/* TEST(test_test, tts_test_getdefaultrole)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    int reqId = 100;
    int count = 0;

    TTS_RoleInfo *role = new(MEM_Voice) TTS_RoleInfo();
    tts->GetDefaultRole(role);
    T_LOG("name=%s, soundType=%d, id=%d", role->m_name.GetString(), role->m_soundType, role->m_id);
} */

/* TEST(test_test, tts_test_listrole)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    int reqId = 100;
    int count = 0;
    tts->ListRole(NULL, &count);
    TTS_RoleInfo *roles[count];
    for (int i = 0; i < count; ++i) {
        TTS_RoleInfo *role = new(MEM_Voice) TTS_RoleInfo();
        roles[i] = role;
    }
    tts->ListRole(roles, &count);
    for (int i = 0; i < count; ++i) {
        T_LOG("name=%s, soundType=%d, id=%d", roles[i]->m_name.GetString(), roles[i]->m_soundType, roles[i]->m_id);
    }

    // wait.Wait(3000);
    // tts->Stop("testIF", reqId);
    // wait.Wait();
} */

/* TEST(test_test, tts_test_playlist)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    BL_String soundName = "01.wav";
    BL_String str = "大概在1992年, 一家称作Nombas的公司开发了一种叫做C减减的嵌入式脚本语言";
    int reqId = 100;
    int soundId = -1;
    T_LOG("1");
    tts->RegisterSound(sender, soundName, &soundId);
    T_LOG("2");
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundName);
    playlist->AddSilence(3000);
    playlist->AddSound(soundId);
    T_LOG("3");
    tts->Play(playlist, TTS_OP_PRI_HIGH, &listener, &reqId);
    T_LOG("4");
    wait.Wait();
} */

/* TEST(test_test, tts_test_playlist)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾，所返回的一个信号值！";
    int reqId = 100;
    int soundId = -1;
    tts->RegisterSound(sender, soundName1, &soundId);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundName2);
    playlist->AddSilence(5000);
    playlist->AddSound(soundId);
    tts->Play(playlist, TTS_OP_PRI_NORMAL, &listener, &reqId);

    // wait.Wait(20000);
    sleep(5);
    str = "我是高优先级的";
    reqId = 101;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);
    
    wait.Wait();
    T_LOG("end1............................................");
    wait.Wait();
    T_LOG("end2............................................");
} */

/* TEST(test_test, tts_test_playsilence)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    int reqId = 100;
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddSilence(10000);
    tts->Play(playlist, TTS_OP_PRI_NORMAL, &listener, &reqId);

    // wait.Wait(20000);
    wait.Wait();
} */


/* TEST(test_test, tts_test_playlist)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾，所返回的一个信号值！";
    int reqId = 100;
    int soundId = -1;
    tts->RegisterSound(sender, soundName1, &soundId);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundName2);
    playlist->AddSilence(5000);
    playlist->AddSound(soundId);
    tts->Play(playlist, TTS_OP_PRI_NORMAL, &listener, &reqId);

    sleep(15);
    str = "我是高优先级的";
    reqId = 101;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);

    reqId = 102;
    str = "按照以上命令执行，在执行./configure的时候，会出现以下错误";
    tts->Play(sender, str, TTS_OP_PRI_NORMAL, &listener, &reqId);
    
    wait.Wait();
    T_LOG("end1............................................");
    wait.Wait();
    T_LOG("end2............................................");
    wait.Wait();
} */


/* TEST(test_test, tts_test_playlist_play)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾，所返回的一个信号值！";
    int reqId = 100;
    int soundId1 = -1;
    int soundId2 = -1;
    tts->RegisterSound(sender, soundName1, &soundId1);
    tts->RegisterSound(sender, soundName2, &soundId2);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundId2);
    playlist->AddSilence(5000);
    playlist->AddSound(soundId1);
    tts->Play(playlist, TTS_OP_PRI_NORMAL, &listener, &reqId);

    sleep(15);
    str = "我是高优先级的";
    reqId = 101;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);

    reqId = 102;
    str = "按照以上命令执行，在执行./configure的时候，会出现以下错误";
    tts->Play(sender, str, TTS_OP_PRI_NORMAL, &listener, &reqId);
    
    wait.Wait();
    wait.Wait();
    wait.Wait();
} */

/* TEST(test_test, tts_test_playlist_stopall)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾，所返回的一个信号值！";
    int reqId = 100;
    int soundId1 = -1;
    int soundId2 = -1;
    tts->RegisterSound(sender, soundName1, &soundId1);
    tts->RegisterSound(sender, soundName2, &soundId2);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundId2);
    playlist->AddSilence(5000);
    playlist->AddSound(soundId1);
    tts->Play(playlist, TTS_OP_PRI_NORMAL, &listener, &reqId);

    sleep(15);
    str = "我是高优先级的";
    reqId = 101;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);

    reqId = 102;
    str = "按照以上命令执行，在执行./configure的时候，会出现以下错误";
    tts->Play(sender, str, TTS_OP_PRI_NORMAL, &listener, &reqId);

    sleep(3);
    tts->StopAll(sender);
    sleep(15);
} */

/* TEST(test_test, tts_test_play_policy)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾";
    int reqId = 100;
    int soundId1 = -1;
    int soundId2 = -1;
    tts->RegisterSound(sender, soundName1, &soundId1);
    tts->RegisterSound(sender, soundName2, &soundId2);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundId2);
    playlist->AddSilence(5000);
    playlist->AddSound(soundId1);
    T_LOG("operation=%d....................", (TTS_OP_MASK)(TTS_OP_PRI_NORMAL | TTS_OP_RESUME_RESTART));
    tts->Play(playlist, (TTS_OP_MASK)(TTS_OP_PRI_NORMAL | TTS_OP_RESUME_RESTART) , &listener, &reqId);

    sleep(5);
    str = "我是高优先级的";
    reqId = 101;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);

    reqId = 102;
    str = "按照以上命令执行，在执行./configure的时候，会出现以下错误";
    tts->Play(sender, str, (TTS_OP_MASK)(TTS_OP_PRI_HIGH | TTS_OP_QUEUE_ABORT) , &listener, &reqId);

    sleep(15);
} */

/* TEST(test_test, tts_test_play_policy2)
{
    TextToSpeech* tts = TextToSpeech::CreateInstance();
    CL_WaitObj wait;
    Listener listener(wait);
    const BL_String sender = "testIF";
    BL_String soundName1 = "212.wav";
    BL_String soundName2 = "213.wav";
    BL_String str = "在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾";
    int reqId = 100;
    int soundId1 = -1;
    int soundId2 = -1;
    tts->RegisterSound(sender, soundName1, &soundId1);
    tts->RegisterSound(sender, soundName2, &soundId2);
    TtsPlaylist *playlist = tts->CreatePlayList(sender);
    playlist->AddText(str);
    playlist->AddSound(soundId2);
    playlist->AddSilence(5000);
    playlist->AddSound(soundId1);
    T_LOG("operation=%d....................", (TTS_OP_MASK)(TTS_OP_PRI_NORMAL | TTS_OP_RESUME_RESTART));
    tts->Play(playlist, (TTS_OP_MASK)(TTS_OP_PRI_NORMAL | TTS_OP_RESUME_RESTART) , &listener, &reqId);

    sleep(5);
    str = "我是高优先级的";
    reqId = 101;
    tts->Play(sender, str, TTS_OP_PRI_HIGH, &listener, &reqId);

    reqId = 102;
    str = "按照以上命令执行，在执行./configure的时候，会出现以下错误";
    tts->Play(sender, str, (TTS_OP_MASK)(TTS_OP_PRI_HIGH | TTS_OP_QUEUE_ADD ) , &listener, &reqId);

    sleep(25);
} */

/* TEST(test_test, tts_test_multithread_play_policy)
{
    TestThread testThread1, testThread2, testThread3;
    testThread1.StartRegistThread();
    testThread2.StartRegistThread();
    testThread3.StartRegistThread();
    testThread1.DoTest(&TestCase1);
    testThread2.DoTest(&TestCase2);
    testThread3.DoTest(&TestCase3);

    sleep(80);
} */

/* class MockTTS_PlayThread : public TTS_PlayThread
{
public:
    MockTTS_PlayThread()
    : TTS_PlayThread()
    {

    }

    using TTS_PlayThread::StartPlay;
    MOCK_METHOD1(StartPlay, void ( TTS_PlaylistInfo *playlist ));
    // MOCK_METHOD1(StopPlay, void (TTS_STOP_TYPE stopType));
};

TEST(TTS_EventPlayReq, OnEvent_param_playThread)
{
    BL_String sender = "test";
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo(sender);
    TTS_EventPlayReq *req = new(MEM_Voice) TTS_EventPlayReq(playlist);
    MockTTS_PlayThread *playThread = new(MEM_Voice) MockTTS_PlayThread();
    EXPECT_CALL(*playThread, StartPlay(playlist)).Times(1);
    req->OnEvent(playThread);
}

TEST(TTS_EventPlayReq, OnEvent_param_NULL)
{
    BL_String sender = "test";
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo(sender);
    TTS_EventPlayReq *req = new(MEM_Voice) TTS_EventPlayReq(playlist);
    MockTTS_PlayThread *playThread = new(MEM_Voice) MockTTS_PlayThread();
    EXPECT_CALL(*playThread, StartPlay(playlist)).Times(0);
    req->OnEvent(NULL);

} */
/* EOF */
