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
 * @file VR_ErrorCode.h
 * @brief Declaration file of log macro.
 *
 * This file includes the declaration of log macro.
 *
 * @attention used on *.cpp only.
 */

#ifndef VR_ERRORCODEDEF_H
#define VR_ERRORCODEDEF_H

#ifndef NCERROR_H
#include <ncore/NCError.h>
#endif

#ifndef NCTYPESDEFINE_H
#include "ncore/NCTypesDefine.h"
#endif

#ifndef FWERRORCODEDEF_H
#include "system/FwErrorCodeDef.h"
#endif

/*
 * VR用的ErrorLog的定义范围为：0x20400000~0x204FFFFF
 * 系统级Error:  0x20400000~0x2040000A
 * Debug级Error:0x20400100~0x20400112
 * 以下Error的定义随意追加!!!!!!禁止!!!!!!
 * 如确实需要，必须申请
*/

namespace nutshell
{
    enum VRCommonErrorCode
    {
        /* 系统级Error */
        VR_ERR_UNSUPPORTED = NCERRORCODEF(NC_ERRORCODE_FW_SUB_MODULE_VR, 0), /* 0x20400000:Not supported (such as language) */
        VR_ERR_SERVICE_TIMEOUT, /* 0x20400001:Service timeout (such as TTS, Search services) */
        VR_ERR_SERVICE_UNAVAILABLE, /* 0x20400002:Service not available */
        VR_ERR_AUDIO_SESSION, /* 0x20400003:Open audio session failed */
        VR_ERR_AUDIOIN, /* 0x20400004	Open audio-in device failed */
        VR_ERR_FILE_NOT_EXIST, /* 0x20400005	File not exist */
        VR_ERR_DATA_VERSION,  /* 0x20400006	Data version not match with app */
        VR_ERR_DATA_CONTENT, /* 0x20400007	Data content incorrect */
        VR_ERR_NETWORK,  /* 0x20400008	Network error */
        VR_ERR_MEDIA_DATA,  /* 0x20400009	Media data error */
        VR_ERR_ENGINE_VBT,  /* 0x2040000A	VBT engine error */

        /* Debug级Error */
        VR_ERR_NOMEM = NCERRORCODEF(NC_ERRORCODE_FW_SUB_MODULE_VR, 256),  /* 0x20400100	No memory */
        VR_ERR_INVALID_PARAM, /* 0x20400101	Invalid parameter */
        VR_ERR_INVALID_OP, /* 0x20400102	Invalid operation */
        VR_ERR_NULL_PARAM, /* 0x20400103	Null paramter */
        VR_ERR_XML_NODE_VALUE, /* 0x20400104	XML node value is empty or wrong */
        VR_ERR_OUT_RANGE, /* 0x20400105	Index is out of range */
        VR_ERR_INVALID_ASR_RESULT, /* 0x20400106	ASR result format is wrong */
        VR_ERR_SCRIPT, /* 0x20400107	Interpreter's script incorrect */
        VR_ERR_GRAMMAR_LIMIT, /* 0x20400108	ASR grammar count over limit */
        VR_ERR_INVALID_DEVICE_INDEX, /* 0x20400109	Invald device index */
        VR_ERR_EXCEPTION, /* 0x2040010A	Interpreter's exception */
        VR_ERR_OPEN_DB, /* 0x2040010B	Open DB failed */
        VR_ERR_SQL, /* 0x2040010C	SQL error */
        VR_ERR_INUSED, /* 0x2040010D	file be in used */
        VR_ERR_BLOCKED, /* 0x2040010E	WaitForCompletion failed: 0x%lx */
        VR_ERR_PHONE_NOT_CONNTECED, /* 0x2040010F	Phone not be connected */
        VR_ERR_INVALID_PROTO, /* 0x20400110	Invalid proto */
        VR_ERR_NO_ACTIVE_SOURCETYPE, /* 0x20400111	not find active source type */
        VR_ERR_HTTPFILE_FAIL, /* 0x20400112	http transfer file fail */
    };

}

/**
* @brief Record recoverable errors macro
*
* It can automatically append the file info and line number.
* <b>You should use this macro instead using AL_AplError::recordError function directly.</b>
* #define NCErrorLog(error, option)    NCError::recordError(__FILE__, __LINE__, error, option)
*/
#define VRErrorLog NCErrorLog


/**
* @brief Record Debug errors macro
*
* It can automatically append the file info and line number.
* <b>You should use this macro instead using AL_AplError::recordError function directly.</b>
* #define NCDebugError(error, option)    NCError::recordDebug(__FILE__, __LINE__, error, option)
*/

#define VRDebugLog NCDebugError

/**
* @brief Record Fatal errors macro
*
* It can automatically append the file info and line number.
* <b>You should use this macro instead using AL_AplError::recordError function directly.</b>
* #define NCFatalError(error, option)    NCError::recordFatal(__FILE__, __LINE__, error, option)
*/

#define VRFatalLog NCFatalError

/* 如何使用
 * bool VR_AvcUnit::xxx()
 * {
 *   .......
 *   res = httpFile.getLastError();
 *   if (NC_HTTP_ERROR_SOCKET_NEEDCONECT == res) {
 *      httpFile.close();
 *      httpclient.disconnectClient();
 *      VRErrorLog(VR_ERR_HTTPFILE_FAIL, 0);
 *  }
 *  .....
 * }
*/

#endif /* VR_ERRORCODEDEF_H */
/* EOF */
