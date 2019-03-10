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
 * @brief Summary for stdafx.h
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

#define VR_SMART_AUTO
#define ENABLE_VOICE_DEBUG
#include "VC_Log.h"

// #define VP_TEST_PERFORMANCE
#ifdef VP_TEST_PERFORMANCE

#include "MEM_vector.h"

// #include <vector>
#include <time.h>
#include <string>
struct VP_TIME;
extern VoiceVector<VP_TIME>::type g_times;

extern void VC_MakeTimeTag(char* str);
extern void VC_LogoutTimeTags();
extern void VC_ClearTags();

#define MakeTimeTag(str) VC_MakeTimeTag(str)
#define LogoutTimeTags() VC_LogoutTimeTags()
#define ClearTags() VC_ClearTags()

#else
#define MakeTimeTag(str)
#define LogoutTimeTags()
#define ClearTags()

#endif
/* EOF */
