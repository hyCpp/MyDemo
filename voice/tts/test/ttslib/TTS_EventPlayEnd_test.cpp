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
#include "TTS_EventPlayEnd.h"
#include "TTS_Controler.h"
#include "TTS_PlaylistInfo.h"
#include "TTS_PlayThread.h"
#include "TTS_PlaylistInfoManager.h"
#include "TTS_ReceiverCb.h"

using namespace std;

TEST(TTS_EventPlayEnd, OnEvent1)
{

    TTS_PlayThread thread;
    TTS_PlaylistInfo playlist("test", NULL);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    TTS_EventPlayEnd ev(&thread, &playlist, &manager, &listener, TTS_PLAYER_START);
    ev.OnEvent();
}

TEST(TTS_EventPlayEnd, OnEvent2)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(1);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    manager.SetCurrPlaylist(playlist);

    TTS_EventPlayEnd ev(&thread, playlist, &manager, &listener, TTS_PLAYER_PAUSE);
    ev.OnEvent();

}

TEST(TTS_EventPlayEnd, OnEvent3)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(1);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    manager.SetCurrPlaylist(playlist);

    TTS_EventPlayEnd ev(&thread, playlist, &manager, &listener, TTS_PLAYER_START);
    ev.OnEvent();
}

TEST(TTS_EventPlayEnd, OnEvent4)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(1);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    manager.SetCurrPlaylist(playlist);

    TTS_PlaylistInfo *playlist2 = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    manager.AddPlayList(playlist2);

    TTS_EventPlayEnd ev(&thread, playlist, &manager, &listener, TTS_PLAYER_START);
    ev.OnEvent();
} 

TEST(TTS_EventPlayEnd, OnEvent5)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfo *playlist = new(MEM_Voice) TTS_PlaylistInfo("sender", NULL);
    playlist->SetRequestID(1);
    TTS_PlaylistInfoManager manager;
    TTS_PlayListener listener;

    manager.SetCurrPlaylist(playlist);


    TTS_EventPlayEnd ev(&thread, NULL, &manager, &listener, TTS_PLAYER_START);
    ev.OnEvent();
} 

/* EOF */

