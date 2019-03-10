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
#include "TTS_PlayThread.h"
#include "TTS_PlayElementSilence.h"

TTS_PlayElementSilence::TTS_PlayElementSilence()
    : m_millisecond(0)
{
    pthread_mutex_init(&mutex, NULL);
    int ret = pthread_condattr_init(&condattr);
       if (ret != 0) {
           return;
    }
    pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
    pthread_cond_init(&cond, &condattr);
}

TTS_PlayElementSilence::~TTS_PlayElementSilence()
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

CL_BOOL TTS_PlayElementSilence::DoPlay(TTS_EngineBuffers* buffer)
{
    // TTS_LOGD_FUNC();
    pthread_mutex_lock(&mutex);
    struct timespec nptime;
    clock_gettime(CLOCK_MONOTONIC, &nptime);
    nptime.tv_sec += m_millisecond/1000;
    nptime.tv_nsec += (m_millisecond % 1000)*1000000;
    pthread_cond_timedwait(&cond, &mutex, &nptime);
    pthread_mutex_unlock(&mutex);

    return CL_TRUE;
}

void TTS_PlayElementSilence::Stop()
{
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

/* EOF */
