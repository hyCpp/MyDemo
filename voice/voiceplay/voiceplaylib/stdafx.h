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
 * @brief Summary for stdafx.h.
 *
 * include file for standard system include files,
 * or project specific include files that are used frequently, but
 * are changed infrequently
 *
 */

#pragma once

#if defined(_WIN32) || defined(_WIN32_WCE)

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
# define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif
# include "windows.h"
#else
# include "BL_Base.h"
#endif

#ifndef _BL_LOG_H
#define _BL_LOG_H
#include "BL_Log.h"
#endif


#define FOR_GTEST
// #define FOR_PRINT

#define ENABLE_VOICE_DEBUG
#include "VC_Log.h"

// #define VC_ERROR(...)   VOICE_LOG_ERROR("VP", errCode, optData)
#define VP_ERROR(errCode, optData, format, ...) \
                        _VOICE_LOG_ERROR_AND_MESSAGE(VOICE_Module_VP, errCode, optData, format, ##__VA_ARGS__)
#define VP_LOGE(format, ...)   _VOICE_LOGE(VOICE_Module_VP, format, ##__VA_ARGS__)
#define VP_LOGI(format, ...)     _VOICE_LOGI(VOICE_Module_VP, format, ##__VA_ARGS__)
#define VP_LOGD(format, ...)    _VOICE_LOGD(VOICE_Module_VP, format, ##__VA_ARGS__)
#define VP_LOGD_FUNC()  _VOICE_LOG_DEBUG_FUNC_INOUT(VOICE_Module_VP)
#define VP_LOGD_LN()	_VOICE_LOG_DEBUG_LINENUMBER(VOICE_Module_VP)

#define VP_DEBUG_ERROR(format, ...)     BL_Log::Output(BL_LOG_LEVEL_DEBUG, BL_LOG_KIND_ERROR, format, ##__VA_ARGS__)

/*
#define VP_LOGE(...)                       printf(__VA_ARGS__);\
                                           printf("\n")
  // #define VP_ERROR(errCode, optData, ...) VOICE_LOG_ERROR_MESSAGE("VC", BL_LOG_KIND_VOICEPLAY, errCode, optData, __VA_ARGS__)
#define VP_LOGI(...)                         printf(__VA_ARGS__);\
                                           printf("\n")
#define VP_LOGD(...)                        printf(__VA_ARGS__);\
                                           printf("\n")
*/

#include <time.h>
                                           
// #define VP_UNIT_TEST
#ifdef VP_UNIT_TEST

#include "MEM_map.h"
    // #include <map>
#include <string>
    extern VoiceMap<std::string, int>::type vp_flags;
    extern void insert(std::string key, int value);
#define VP_TEST(key, value) insert(key, value)
#else
#define VP_TEST(key, value)
#endif


// #define VP_TEST_PERFORMANCE
#ifdef VP_TEST_PERFORMANCE

#include "MEM_vector.h"

// #include <vector>
#include <string>
struct VP_TIME;
extern VoiceVector<VP_TIME>::type g_times;

extern void _MakeTimeTag(char* str);
extern void _LogoutTimeTags();
extern void _ClearTags();

#define MakeTimeTag(str) _MakeTimeTag(str)
#define LogoutTimeTags() _LogoutTimeTags()
#define ClearTags() _ClearTags()

#else
#define MakeTimeTag(str)
#define LogoutTimeTags()
#define ClearTags()

#endif
/* EOF */
