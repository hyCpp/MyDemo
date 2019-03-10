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

/**
 * @file stdafx.h
 * @brief Declaration file of class vc_trace_func_class.                  
 *
 * This file includes the declaration of class vc_trace_func_class and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if defined(_WIN32) || defined(_WIN32_WCE)

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
# define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif
# include "windows.h"
#else
# include "BL_Base.h"
#endif

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

// #define FOR_GTEST
// #define FOR_PRINT

#ifndef FOR_GTEST
#define ENABLE_VOICE_DEBUG
#include "VC_Log.h"

#define TTS_ERROR_MSG(errCode, optData, format, ...) _VOICE_LOG_ERROR_AND_MESSAGE(VOICE_Module_TTS, errCode, optData, format, ##__VA_ARGS__)
#define TTS_LOGE(format, ...)                        _VOICE_LOGE(VOICE_Module_TTS, format, ##__VA_ARGS__)
#define TTS_LOGI(format, ...)                        _VOICE_LOGI(VOICE_Module_TTS, format, ##__VA_ARGS__)
#define TTS_LOGD(format, ...)                        _VOICE_LOGD(VOICE_Module_TTS, format, ##__VA_ARGS__)
#define TTS_LOGD_FUNC()                              _VOICE_LOG_DEBUG_FUNC_INOUT(VOICE_Module_TTS)
#define TTS_LOGD_LN()                                _VOICE_LOG_DEBUG_LINENUMBER(VOICE_Module_TTS)

#elif defined(FOR_PRINT)

#define TTS_LOGE(...)                       printf(__VA_ARGS__);\
                                            printf("\n")
#define TTS_ERROR(errCode, optData, ...) VOICE_LOG_ERROR_MESSAGE("TTS", BL_LOG_KIND_VOICEPLAY, errCode, optData, __VA_ARGS__)
#define TTS_LOGI(...)                         printf(__VA_ARGS__);\
                                            printf("\n")
#define TTS_LOGD(...)                        printf(__VA_ARGS__);\
                                            printf("\n")
#define TTS_LOGD_FUNC()                      
#define TTS_LOGD_LN()	                    VOICE_LOG_DEBUG_LINENUMBER("TTS")

#else

#include "ncore/NCLog.h"
#   define VOICE_LOG(tag, module, format, ...)  nutshell::NCLog::output(nutshell::NC_LOG_INFO, (tag), "%s :" format,  __PRETTY_FUNCTION__, ##__VA_ARGS__)
#   define VOICE_LOG_DEBUG(tag, format, ...)    nutshell::NCLog::output(nutshell::NC_LOG_DEBUG, (tag), "%s :" format,  __PRETTY_FUNCTION__, ##__VA_ARGS__)
#   define VOICE_LOGW(tag, module, format, ...) nutshell::NCLog::output(nutshell::NC_LOG_WARNING, (tag), "%s :" format,  __PRETTY_FUNCTION__, ##__VA_ARGS__)

#   define VOICE_LOG_ERROR(tag, errCode, optData) nutshell::NCLog::output(nutshell::NC_LOG_ERROR, (tag), "ERROR!! err=%x opt=%x % s", (errCode), (optData), __PRETTY_FUNCTION__)
#   define VOICE_LOG_ERROR_MESSAGE(tag, module, errCode, optData, format, ...) nutshell::NCLog::output(nutshell::NC_LOG_ERROR, (tag), "ERROR!! err=%x opt=%x % s :" format, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#   define VOICE_LOG_DEBUG_FUNC_INOUT(tag)  vc_trace_func_class vc_trace_func_obj((tag), (__PRETTY_FUNCTION__))
#   define VOICE_LOG_DEBUG_LINENUMBER(tag)  nutshell::NCLog::output(nutshell::NC_LOG_DEBUG, (tag), "%s :line=%d", __PRETTY_FUNCTION__, __LINE__)

/**
* vc_trace_func_class
*
* The class is just for vc_trace_func_class.
*/
class vc_trace_func_class
{
    public:
        vc_trace_func_class(const char* tag, const char* func) : m_tag(tag), m_func(func)
        {
            nutshell::NCLog::output(nutshell::NC_LOG_DEBUG, m_tag, "->>>%s", m_func);
        }

        ~vc_trace_func_class()
        {
            nutshell::NCLog::output(nutshell::NC_LOG_DEBUG, m_tag, "<<<-%s", m_func);
        }

    private:
        const char* m_tag;
        const char* m_func;
};


#define TTS_LOGE(...)                       VOICE_LOG("TTS", BL_LOG_KIND_TTS, __VA_ARGS__)
#define TTS_ERROR(errCode, optData, ...) VOICE_LOG_ERROR_MESSAGE("TTS", BL_LOG_KIND_VOICEPLAY, errCode, optData, __VA_ARGS__)
#define TTS_LOGI(...)                         VOICE_LOG("TTS", BL_LOG_KIND_TTS, __VA_ARGS__)
#define TTS_LOGD(...)                        VOICE_LOG_DEBUG("TTS", __VA_ARGS__)
#define TTS_LOGD_FUNC()                      VOICE_LOG_DEBUG_FUNC_INOUT("TTS")
#define TTS_LOGD_LN()	                    VOICE_LOG_DEBUG_LINENUMBER("TTS")

#endif

// #define TTS_UNIT_TEST
#ifdef TTS_UNIT_TEST
#include "MEM_map.h"
#include <string>
    extern VoiceMap<std::string, int>::type g_flags;
    extern void insert(std::string key, int value);
#define TTS_TEST(key, value) insert(key, value)
#else
#define TTS_TEST(key, value)

#endif
/* EOF */
