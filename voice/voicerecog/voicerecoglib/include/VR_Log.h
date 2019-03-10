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
 * @file VR_Log.h
 * @brief Declaration file of log macro.
 *
 * This file includes the declaration of log macro.
 *
 * @attention used on *.cpp only.
 */

#ifndef VR_LOG_H
#define VR_LOG_H

#if defined(_FOR_IPHONE_) || defined(_LINUX)
#   define _VOICE_IAUTO_TEST_
#endif

#ifdef _ENABLE_VR_PERFORMANCE_TEST
# undef _ENABLE_VR_DEBUG_LOG
#endif

#define _ENABLE_VR_DEBUG_LOG

#include "VC_Log.h"
#define VR_UNUSED_VAR(p)    ((void)p)

#define VR_ERROR(errCode, optData, format, ...) _VOICE_LOG_ERROR_AND_MESSAGE(VOICE_Module_VR, (errCode), (optData), format, ##__VA_ARGS__)
#define VR_LOGE(format, ...)                         _VOICE_LOGE(VOICE_Module_VR, format, ##__VA_ARGS__)
#define VR_LOGP(format, ...)                         _VOICE_LOGP(VOICE_Module_VR, format, ##__VA_ARGS__)
#define VR_LOGI(format, ...)                         _VOICE_LOGI(VOICE_Module_VR, format, ##__VA_ARGS__)

#ifdef _ENABLE_VR_DEBUG_LOG
# define VR_LOGD(format, ...)                        _VOICE_LOGD(VOICE_Module_VR, format, ##__VA_ARGS__)
# define VR_LOGD_FUNC()                              _VOICE_LOG_DEBUG_FUNC_INOUT(VOICE_Module_VR)
# define VR_LOG_ASR(format, ...)                     _VOICE_LOG_ASR(VOICE_Module_VR, format, ##__VA_ARGS__)
#else
# define VR_LOGD(format, ...)
# define VR_LOGD_FUNC()
# define VR_LOG_ASR(format, ...)
#endif

// for de error log optdata
#define VR_DE_OPTIONDATA_NONE      0

#endif /* VR_LOG_H */
/* EOF */
