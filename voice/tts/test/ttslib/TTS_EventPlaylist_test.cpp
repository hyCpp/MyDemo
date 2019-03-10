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
#include "TTS_EventPlaylist.h"
#include "TTS_Controler.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_PlayThread.h"
#include "TTS_PlaylistInfoManager.h"
#include "TTS_ReceiverCb.h"

using namespace std;

TEST(TTS_EventPlaylist, OnEvent1)
{

    TTS_PlayThread thread;
    TTS_PlaylistInfo playlist("test", NULL);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;


    TTS_EventPlaylist ev(&thread, &playlist, &manager, &listener);
    ev.OnEvent();
}

TEST(TTS_EventPlaylist, OnEvent2)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("test", NULL);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    manager.SetCurrPlaylist(playlist);

    TTS_EventPlaylist ev(&thread, playlist, &manager, &listener);
    ev.OnEvent();
}

TEST(TTS_EventPlaylist, OnEvent3)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("test", NULL);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    TTS_POLICY policy;
    policy.queuePolicy = TTS_OP_QUEUE_ABORT_T;
    playlist->SetPolicy(policy); 

    manager.SetCurrPlaylist(playlist);

    TTS_EventPlaylist ev(&thread, playlist, &manager, &listener);
    ev.OnEvent();
} 



/* EOF */
