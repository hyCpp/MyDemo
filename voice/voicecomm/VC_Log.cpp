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
#include <stdio.h>
#include "BL_Log.h"
#include "VC_Log.h"

#ifdef HAVE_LINUX_NATIVEX86
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#endif

#ifdef HAVE_NUTSHELL_OS
#include <ncore/NCPerformanceLog.h>
#endif

#define VOICE_LOG_BUF_SIZE 2048

const DWORD TAG_KINDS[] = {
    BL_LOG_KIND_VOICERECOG,
    BL_LOG_KIND_VOICEPLAY,
    BL_LOG_KIND_VOICECOMM,
    BL_LOG_KIND_VOICESTUB,
    BL_LOG_KIND_TTS,
};

const DWORD TAG_KINDS_P[] = {
    BL_LOG_KIND_VOICERECOG_P,
    BL_LOG_KIND_VOICEPLAY_P,
    BL_LOG_KIND_VOICECOMM_P,
    BL_LOG_KIND_VOICESTUB_P,
    BL_LOG_KIND_TTS_P,
};

/**
 * Summary for class VC_TraceFunc
 *
 * Platform for Voice log
 *
 */
VC_TraceFunc::VC_TraceFunc(const char* const func, int module) : m_func(func), m_module(module)
{
#ifdef HAVE_NUTSHELL_OS
    BL_Log::Output(BL_LOG_LEVEL_DEBUG, TAG_KINDS[m_module], "=>[%s]", m_func);
#elif HAVE_LINUX_NATIVEX86
    printf("[Voice]=>[%s]\r\n", m_func);
#endif
}

VC_TraceFunc::~VC_TraceFunc()
{
#ifdef HAVE_NUTSHELL_OS
    BL_Log::Output(BL_LOG_LEVEL_DEBUG, TAG_KINDS[m_module], "<=[%s]", m_func);
#elif HAVE_LINUX_NATIVEX86
    printf("[Voice]<=[%s]\r\n", m_func);
#endif
}

void VC_LOG_TRACE(VOICE_LogLevel level, VOICE_Module module, const char* format, ...)
{
#ifdef HAVE_NUTSHELL_OS
    const DWORD dwModule = (VOICE_PERFORMANCE == level) ? TAG_KINDS_P[static_cast<int>(module)] : TAG_KINDS[static_cast<int>(module)];
    int status = BL_Log::GetSettingKindStatus(dwModule);
    if (0 == status) {
        return;
    }

    va_list list;
    va_start(list, format);

    switch (level) {
    case VOICE_PERFORMANCE:
    {
        char buf[VOICE_LOG_BUF_SIZE] = { 0 };
        vsnprintf(buf, VOICE_LOG_BUF_SIZE, format, list);
        nutshell::NCPERFORMANCE_OUT("voicerecog", "VRService", buf);
        BL_Log::OutputV(BL_LOG_LEVEL_INFO, dwModule, format, list);
        break;
    }
    case VOICE_ERROR:
        BL_Log::OutputV(BL_LOG_LEVEL_ERROR, dwModule, format, list);
        break;
    case VOICE_INFO:
        BL_Log::OutputV(BL_LOG_LEVEL_INFO, dwModule, format, list);
        break;
    case VOICE_DEBUG:
        BL_Log::OutputV(BL_LOG_LEVEL_DEBUG, dwModule, format, list);
        break;
    case VOICE_DEBUG_FUNC_INOUT:
    {
        char buf[VOICE_LOG_BUF_SIZE] = { 0 };
        vsnprintf(buf, VOICE_LOG_BUF_SIZE, format, list);
        VC_TraceFunc traceFunc(buf, static_cast<int>(module));
        break;
    }
    default :
        break;
    }

    va_end(list);
#elif HAVE_LINUX_NATIVEX86
    static char buff[256] = { 0 };
    memset(buff, 0, 256);

    va_list list;
    va_start(list, format);
    // max len 255, avoid memory crossing the border
    _vsnprintf(buff, 255, format, list);
    va_end(list);

    printf("[Voice]: %s\r\n", buff);
#endif
}
/* EOF */
