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
 * @file VC_Log.h
 * @brief Declaration file of log macro.
 *
 * This file includes the declaration of log macro.
 *
 * @attention used on *.cpp only.
 */

#ifndef VC_LOG_H
#define VC_LOG_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifdef      HAVE_NUTSHELL_OS
#ifndef NCERROR_H
#include <ncore/NCError.h>
#endif

#ifndef FWERRORCODEDEF_H
#include "system/FwErrorCodeDef.h"
#endif  // end of FWERRORCODEDEF_H

#endif  // end of HAVE_NUTSHELL_OS

#include "VC_CommonDef.h"

enum VOICE_LogLevel
{
    VOICE_ERROR,
    VOICE_INFO,
    VOICE_DEBUG,
    VOICE_DEBUG_FUNC_INOUT,
    VOICE_PERFORMANCE
};

enum VOICE_Module
{
    VOICE_Module_VR,
    VOICE_Module_VP,
    VOICE_Module_VC,
    VOICE_Module_VRSTUB,
    VOICE_Module_TTS
};

enum VOICE_ErrorCode
{
#ifdef HAVE_NUTSHELL_OS
    VOICE_TTS_ERR_DATA_PATH = NCERRORCODEF((nutshell::NC_ERRORCODE_FW_SUB_MODULE_TTS), 0), // can not get the right data path
#else
    VOICE_TTS_ERR_DATA_PATH = 0x20500000,
#endif
    VOICE_TTS_ERR_CONFIG_FILE,                                    // config file error
    VOICE_TTS_ERR_UNSUPPORTED_ENGINE,                             // can not get the 3rd engine lib
    VOICE_TTS_ERR_ENGINE_RESOURE,                                 // resoure error that the engine need
    VOICE_TTS_ERR_NOMEM = VOICE_TTS_ERR_DATA_PATH + 0x0100,       // No memory
    VOICE_TTS_ERR_INVALID_PARAM,                                  // Invalid parameter
    VOICE_TTS_ERR_UNSUPPORTED_LANG,                               // unsupported language
    VOICE_TTS_ERR_NULL_PARAM,                                     // Null paramter
    VOICE_TTS_ERR_WAVE_FILE,                                      // invalid wave file
#ifdef  HAVE_NUTSHELL_OS
    VOICE_VR_ERR_UNSUPPORTED = NCERRORCODEF((nutshell::NC_ERRORCODE_FW_SUB_MODULE_VR), 0), // Not supported (such as language)
#else
    VOICE_VR_ERR_UNSUPPORTED = 0x20400000,
#endif
    VOICE_VR_ERR_SERVICE_TIMEOUT,                                 // Service timeout (such as TTS, Search services)
    VOICE_VR_ERR_SERVICE_UNAVAILABLE,                             // Service not available
    VOICE_VR_ERR_AUDIO_SESSION,                                   // Open audio session failed
    VOICE_VR_ERR_AUDIOIN,                                         // Open audio-in device failed
    VOICE_VR_ERR_FILE_NOT_EXIST,                                  // File not exist
    VOICE_VR_ERR_DATA_VERSION,                                    // Data version not match with app
    VOICE_VR_ERR_DATA_CONTENT,                                    // Data content incorrect
    VOICE_VR_ERR_NETWORK,                                         // Network error
    VOICE_VR_ERR_MEDIA_DATA,                                      // Media data error
    VOICE_VR_ERR_ENGINE_VBT,                                      // VBT engine error
    VOICE_VR_ERR_NOMEM = VOICE_VR_ERR_UNSUPPORTED + 0x0100,       // No memory
    VOICE_VR_ERR_INVALID_PARAM,                                   // Invalid parameter
    VOICE_VR_ERR_INVALID_OP,                                      // Invalid operation
    VOICE_VR_ERR_NULL_PARAM,                                      // Null paramter
    VOICE_VR_ERR_XML_NODE_VALUE,                                  // XML node value is empty or wrong
    VOICE_VR_ERR_OUT_RANGE,                                       // Index is out of range
    VOICE_VR_ERR_INVALID_ASR_RESULT,                              // ASR result format is wrong
    VOICE_VR_ERR_SCRIPT,                                          // Interpreter's script incorrect
    VOICE_VR_ERR_GRAMMAR_LIMIT,                                   // ASR grammar count over limit
    VOICE_VR_ERR_INVALID_DEVICE_INDEX,                            // Invald device index
    VOICE_VR_ERR_EXCEPTION,                                       // Interpreter's exception
    VOICE_VR_ERR_OPEN_DB,                                         // Open DB failed
    VOICE_VR_ERR_SQL,                                             // SQL error
    VOICE_VR_ERR_INUSED,                                          // file be in used
    VOICE_VR_ERR_BLOCKED,                                         // WaitForCompletion failed: 0x%lx
    VOICE_VR_ERR_PHONE_NOT_CONNTECED,                             // Phone not be connected
    VOICE_VR_ERR_INVALID_PROTO,                                   // Invalid proto
    VOICE_VR_ERR_NO_ACTIVE_SOURCETYPE,                            // not find active source type
};


/**
     * Summary for class VC_TraceFunc
     *
     * There should be a detail description for
     * class VC_TraceFunc
     */
class VC_API VC_TraceFunc
{
    public:
        VC_TraceFunc(const char* const func, int module);
        ~VC_TraceFunc();

    private:
        VC_TraceFunc(const VC_TraceFunc&);
        VC_TraceFunc &operator=(const VC_TraceFunc&);

        const char* const m_func;
        const int m_module;
};

VC_API void VC_LOG_TRACE(VOICE_LogLevel level, VOICE_Module module, const char* format, ...);

#define _VOICE_LOGE(module, format, ...)  VC_LOG_TRACE(VOICE_ERROR, (module), "[E]%s: " format, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define _VOICE_LOGI(module, format, ...)  VC_LOG_TRACE(VOICE_INFO, (module), "[I]%s: " format, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define _VOICE_LOGD(module, format, ...)  VC_LOG_TRACE(VOICE_DEBUG, (module), "[D]%s: " format, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define _VOICE_LOGP(module, format, ...)  VC_LOG_TRACE(VOICE_PERFORMANCE, (module), "[P] " format, ##__VA_ARGS__)

#ifdef HAVE_NUTSHELL_OS
#define _VOICE_LOG_ERROR_AND_MESSAGE(module, errCode, optData, format, ...)  \
        nutshell::NCErrorLog(errCode, optData); \
        VC_LOG_TRACE(VOICE_ERROR, (module), "[E]errCode=%x, optData=%x %s: " format, errCode, optData, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#else
#define _VOICE_LOG_ERROR_AND_MESSAGE(module, errCode, optData, format, ...)  \
        VC_LOG_TRACE(VOICE_ERROR, (module), "[E]errCode=%x, optData=%x %s: " format, errCode, optData, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#endif

#define _VOICE_LOG_DEBUG_FUNC_INOUT(module)   VC_TraceFunc vc_trace_fun((__PRETTY_FUNCTION__), (module))

// beg: will be delete
#define _VOICE_LOG_ERROR_MESSAGE(module, errCode, optData, format, ...)  VC_LOG_TRACE(VOICE_ERROR, (module), "[E]errCode=%x, optData=%x %s: " format, errCode, optData, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define _VOICE_LOG(module, format, ...)  VC_LOG_TRACE(VOICE_INFO, (module), "%s: " format, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define VC_LOG(format, ...)                        _VOICE_LOGI(VOICE_Module_VC, format, ##__VA_ARGS__)
// end: will be delete

#define _VOICE_LOG_ASR(module, format, ...)  VC_LOG_TRACE(VOICE_INFO, (module), "[I]" format, ##__VA_ARGS__)

#define VC_LOGE(format, ...)                        _VOICE_LOGE(VOICE_Module_VC, format, ##__VA_ARGS__)
#define VC_LOGI(format, ...)                        _VOICE_LOGI(VOICE_Module_VC, format, ##__VA_ARGS__)
#define VC_LOGD(format, ...)                        _VOICE_LOGD(VOICE_Module_VC, format, ##__VA_ARGS__)
#define VC_LOGD_FUNC()                              _VOICE_LOG_DEBUG_FUNC_INOUT(VOICE_Module_VC)
#define VC_LOGP(format, ...)                        _VOICE_LOGP(VOICE_Module_VC, format, ##__VA_ARGS__)

#define VC_ERROR(errCode, optData, format, ...)     _VOICE_LOG_ERROR_AND_MESSAGE(VOICE_Module_VC, (errCode), (optData), format, ##__VA_ARGS__)

#ifdef HAVE_NUTSHELL_OS
#include "ncore/NCLog.h"
#endif

#endif
/* EOF */

