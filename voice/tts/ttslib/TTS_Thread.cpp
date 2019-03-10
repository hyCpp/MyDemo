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
#include "TTS_Event.h"
#include "TTS_Thread.h"

TTS_Thread::~TTS_Thread()
{
    while (true) {
        TTS_Event *ev = lQueue.Pop();
        if (NULL != ev) {
            delete ev;
            ev = NULL;
        }
        else {
            break;
        }
    }
}

CL_BOOL TTS_Thread::OnEvent(TTS_Event *ev)
{
    CL_BOOL result = CL_FALSE;
    if (!ev) {
        return result;
    }

    CL_SyncObj& lock = GetSyncObj();
    lock.SyncStart();
    if (!CheckQuit()) {
        lQueue.Append(ev);
        result = CL_TRUE;
    }
    lock.SyncEnd();

    Notify();
    return result;
}

void TTS_Thread::Run()
{
    while (!CheckQuit()) {
        CL_SyncObj& lock = GetSyncObj();
        lock.SyncStart();
        TTS_Event *ev = lQueue.Pop();
        lock.SyncEnd();

        if (!ev) {
            Wait();
            continue;
        }

        ev->OnEvent();
        delete ev;
    }

}
/* EOF */
