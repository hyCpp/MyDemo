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
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef VP_TEST_PERFORMANCE
#include "CL_Time.h"

// definition for VP_TIME
struct VP_TIME
{
    std::string m_tag;          // member description for m_tag
    std::string m_timeStr;      // member description for m_timeStr
    unsigned int m_tickCount;   // member description for m_tickCount
    unsigned int m_threadTime;  // member description for m_threadTime
};

VoiceVector<VP_TIME>::type g_times;

void VC_MakeTimeTag(char* str)
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

void VC_LogoutTimeTags()
{
    VoiceVector<VP_TIME>::iterator it = g_times.begin();
    for (; it != g_times.end(); ++it) {
        printf("time=%s, tickCount=%u, threadTime=%u, tag=%s\n", (*it).m_timeStr.c_str(), (*it).m_tickCount, (*it).m_threadTime, (*it).m_tag.c_str());
    }
    printf("==========================================\n");
    fflush(stdout);
}

void VC_ClearTags()
{
    g_times.clear();
}

#endif
/* EOF */
