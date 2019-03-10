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
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "stdafx.h"
#include "TTS_Controler.h"
#include "TTS_TextToSpeechImpl.h"
#include "TTS_PlayThread.h"
#include "TTS_ReceiverCb.h"
#include "TTS_EventStopReq.h"

/**
* TTS_EventStopReq_Test
*
* The class is just for TTS_EventStopReq test.
*/
class TTS_EventStopReq_Test:public testing::Test
{
};

TEST_F(TTS_EventStopReq_Test, OnEvent_stop_1)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = false;


    TTS_EventStopReq req(&thread, &manager, &listener, reqID, sender, stopAll);

    req.OnEvent();
}

TEST_F(TTS_EventStopReq_Test, OnEvent_stop_2)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = false;

    TTS_PlaylistInfo listInfo(sender, NULL);
    listInfo.SetRequestID(reqID);

    manager.SetCurrPlaylist(&listInfo);


    TTS_EventStopReq req(&thread, &manager, &listener, reqID, sender, stopAll);

    req.OnEvent();
}

TEST_F(TTS_EventStopReq_Test, OnEvent_stop_3)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = false;

    TTS_PlaylistInfo* listInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    listInfo->SetRequestID(reqID);

    manager.AddPauseList(listInfo);


    TTS_EventStopReq req(&thread, &manager, &listener, reqID, sender, stopAll);

    req.OnEvent();
}

TEST_F(TTS_EventStopReq_Test, OnEvent_stop_diff_sender)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = false;

    TTS_PlaylistInfo* listInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    listInfo->SetRequestID(reqID);

    manager.SetCurrPlaylist(listInfo);
    // manager.AddPauseList(listInfo);


    TTS_EventStopReq req(&thread, &manager, &listener, reqID, "sender", stopAll);

    req.OnEvent();
}

TEST_F(TTS_EventStopReq_Test, OnEvent_stop_diff_id)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = false;

    TTS_PlaylistInfo* listInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    listInfo->SetRequestID(reqID);

    // manager.AddPauseList(listInfo);
    manager.SetCurrPlaylist(listInfo);


    TTS_EventStopReq req(&thread, &manager, &listener, 10, sender, stopAll);

    req.OnEvent();
}




TEST_F(TTS_EventStopReq_Test, OnEvent_StopAll)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = true;


    TTS_EventStopReq req(&thread, &manager, &listener, reqID, sender, stopAll);

    req.OnEvent();
}

TEST_F(TTS_EventStopReq_Test, OnEvent_StopAll_2)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = true;

    TTS_PlaylistInfo listInfo(sender, NULL);
    listInfo.SetRequestID(reqID);

    manager.SetCurrPlaylist(&listInfo);


    TTS_EventStopReq req(&thread, &manager, &listener, reqID, sender, stopAll);

    req.OnEvent();
}

TEST_F(TTS_EventStopReq_Test, OnEvent_StopAll_3)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = true;

    TTS_PlaylistInfo* listInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    listInfo->SetRequestID(reqID);

    manager.AddPauseList(listInfo);


    TTS_EventStopReq req(&thread, &manager, &listener, reqID, sender, stopAll);

    req.OnEvent();
}

TEST_F(TTS_EventStopReq_Test, OnEvent_StopAll_diff_sender)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = true;

    TTS_PlaylistInfo* listInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    listInfo->SetRequestID(reqID);

    // manager.AddPauseList(listInfo);
    manager.SetCurrPlaylist(listInfo);


    TTS_EventStopReq req(&thread, &manager, &listener, reqID, "sender", stopAll);

    req.OnEvent();
}

TEST_F(TTS_EventStopReq_Test, OnEvent_StopAll_diff_id)
{
    TTS_PlayThread thread;
    TTS_PlaylistInfoManager manager; 
    TTS_PlayListener listener; 
    int reqID = 1;
    BL_String sender = "test"; 
    CL_BOOL stopAll = true;

    TTS_PlaylistInfo* listInfo = new(MEM_Voice) TTS_PlaylistInfo(sender, NULL);
    listInfo->SetRequestID(reqID);

    manager.SetCurrPlaylist(listInfo);
    // manager.AddPauseList(listInfo);


    TTS_EventStopReq req(&thread, &manager, &listener, 10, sender, stopAll);

    req.OnEvent();
}

/* EOF */
