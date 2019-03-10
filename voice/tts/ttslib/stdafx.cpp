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

// stdafx.cpp : source file that includes just the standard includes
// Tts.pch will be the pre - compiled header
// stdafx.obj will contain the pre - compiled type information

#include "stdafx.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>





#ifdef TTS_TEST_PERFORMANCE
#include "CL_Time.h"

//  The struct is just for TTS_TIME.
struct TTS_TIME
{
    std::string m_tag;           // TTS_TIME tag.
    std::string m_timeStr;       // TTS_TIME timeStr.
    unsigned int m_tickCount;    // TTS_TIME tickCount.
    unsigned int m_threadTime;   // TTS_TIME threadTime.
};

VoiceVector<TTS_TIME>::type g_times;

void _MakeTimeTag(char* str)
{
    timeval tv;
    char timeStr[60] = {};
    gettimeofday(&tv, NULL);
    tm *localTime = localtime(&tv.tv_sec);
    strftime(timeStr, sizeof(timeStr), "%F:%H:%M:%S:", localTime);
    TTS_TIME time1;
    time1.m_tag = std::string("[TTS]") + str;
    time1.m_timeStr = timeStr;
    // sprintf(timeStr, "%ld", tv.tv_usec/1000);
    snprintf(timeStr, 60, "%ld", tv.tv_usec/1000);
    time1.m_timeStr += timeStr;
    time1.m_tickCount = GetTickCount();
    time1.m_threadTime = GetThreadTime();
    g_times.push_back(time1);
}

void _LogoutTimeTags()
{
    VoiceVector<TTS_TIME>::iterator it = g_times.begin();
    for (; it != g_times.end(); ++it) {
        printf("time=%s, tickCount=%u, threadTime=%u, tag=%s\n", (*it).m_timeStr.c_str(), (*it).m_tickCount, (*it).m_threadTime, (*it).m_tag.c_str());
    }
    printf("==========\n");
    fflush(stdout);
}

void _ClearTags()
{
    g_times.clear();
}

#endif
/* EOF */
