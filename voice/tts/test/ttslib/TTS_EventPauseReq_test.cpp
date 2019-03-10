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
#include "ttslib_test.h"
#include "TTS_TextToSpeechImpl.h"
#include "TTS_EventPauseReq.h"
#include "TTS_Controler.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_PlayThread.h"
#include "TTS_PlaylistInfoManager.h"
#include "TTS_ReceiverCb.h"

using namespace std;

TEST(TTS_EventPauseReq, OnEvent1)
{

    TTS_PlayThread thread;
    TTS_PlaylistInfo playlist("test", NULL);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;


    TTS_EventPauseReq ev(&thread, &listener, &manager, 1, "sender");
    ev.OnEvent();
}

TEST(TTS_EventPauseReq, OnEvent2)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(1);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    manager.SetCurrPlaylist(playlist);

    TTS_EventPauseReq ev(&thread, &listener, &manager, 1, "sender");
    ev.OnEvent();
}

TEST(TTS_EventPauseReq, OnEvent3)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(1);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;
    
    manager.SetCurrPlaylist(playlist);

    TTS_POLICY policy;
    policy.queuePolicy = TTS_OP_QUEUE_ABORT_T;
    playlist->SetPolicy(policy); 
    
    TTS_PlaylistInfo *playlist2 = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(2);

    manager.AddPlayList(playlist2);

    TTS_EventPauseReq ev(&thread, &listener, &manager, 1, "sender");
    ev.OnEvent();
}

TEST(TTS_EventPauseReq, OnEvent4)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(1);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    manager.SetCurrPlaylist(playlist);

    TTS_POLICY policy;
    policy.queuePolicy = TTS_OP_QUEUE_ABORT_T;
    playlist->SetPolicy(policy); 

    manager.AddPlayList(playlist);

    TTS_EventPauseReq *ev = new(MEM_Voice) TTS_EventPauseReq(&thread, &listener, &manager, 1, "sender1");
    ev->OnEvent();
    delete ev;
} 

TEST(TTS_EventPauseReq, OnEvent5)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(1);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;
    
    manager.SetCurrPlaylist(playlist);

    TTS_POLICY policy;
    policy.queuePolicy = TTS_OP_QUEUE_ABORT_T;
    playlist->SetPolicy(policy); 

    manager.AddPlayList(playlist);

    TTS_EventPauseReq *ev = new(MEM_Voice) TTS_EventPauseReq(&thread, &listener, &manager, 2, "sender1");
    ev->OnEvent();
    delete ev;
} 


/* EOF */
