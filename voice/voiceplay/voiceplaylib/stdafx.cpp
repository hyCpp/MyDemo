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
// VoicePlaySimple.pch will be the pre - compiled header
// stdafx.obj will contain the pre - compiled type information

#include "stdafx.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef VP_UNIT_TEST
    using namespace std;
    VoiceMap<std::string, int>::type vp_flags;
    void insert(std::string key, int value)
    {
        VoiceMap<string, int>::iterator it = vp_flags.find(key);
        if (vp_flags.end() == it) {
            it->second = value;
        } 
        else {
            vp_flags.insert(make_pair(key, value));
        }
    }

#endif

#ifdef VP_TEST_PERFORMANCE
#include "CL_Time.h"

/// definition of VP_TIME
struct VP_TIME
{
    std::string m_tag;          // there should be a description of tag
    std::string m_timeStr;      // there should be a description of m_timeStr
    unsigned int m_tickCount;   // there should be a description of m_tickCount
    unsigned int m_threadTime;  // there should be a description of m_threadTime
};

VoiceVector<VP_TIME>::type g_times;

void _MakeTimeTag(char* str)
{
    timeval tv;
    char timeStr[60] = { 0 };
    gettimeofday(&tv, NULL);
    tm *localTime = localtime(&tv.tv_sec);
    strftime(timeStr, sizeof(timeStr), "%F:%H:%M:%S:", localTime);
    VP_TIME time1;
    time1.m_tag = str;
    time1.m_timeStr = timeStr;
    snprintf(timeStr, 60, "%ld", tv.tv_usec/1000);
    time1.m_timeStr += timeStr;
    time1.m_tickCount = GetTickCount();
    time1.m_threadTime = GetThreadTime();
    g_times.push_back(time1);
}

void _LogoutTimeTags()
{
    VoiceVector<VP_TIME>::iterator it = g_times.begin();
    for (; it != g_times.end(); ++it) {
        printf("time=%s, tickCount=%u, threadTime=%u, tag=%s\n", (*it).m_timeStr.c_str(), (*it).m_tickCount, (*it).m_threadTime, (*it).m_tag.c_str());
    }
    printf("==========================================\n");
    fflush(stdout);
}

void _ClearTags()
{
    g_times.clear();
}

#endif
/* EOF */
