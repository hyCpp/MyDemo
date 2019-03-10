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
 * @file TTS_EngineCommDef.h                                          
 * @brief Declaration file of enum TTS_ERR_ENGINE_TYPE.                  
 *
 * This file includes the declaration of enum TTS_ERR_ENGINE_TYPE and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _TTS_ERR_ENGINE_TYPE_H_
#define _TTS_ERR_ENGINE_TYPE_H_

#pragma once

enum TTS_ERR_ENGINE_TYPE 
{
    TTS_ERR_NO_ERROR = 0,
    TTS_ERR_INV_PARAM = 0x90000001, 
    TTS_ERR_INV_OP, 
    TTS_ERR_NOMEM, 
    TTS_ERR_USERSTOP,
    TTS_ERR_ENGINE_OP_FAILED,
    TTS_ERR_UNSUPPORTED,
    TTS_ERR_UNACCESS,
    TTS_ERR_INVALID_OP,
    TTS_ERR_INVALID_PARAM,
    TTS_ERR_NEW_ERROR,
    TTS_ERR_INVALID_VALUE,
    TTS_ERR_UNKNOWN

};

#endif /* _TTS_ERR_ENGINE_TYPE_H_ */
/* EOF */
