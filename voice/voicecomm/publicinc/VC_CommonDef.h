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
 * @file VC_CommonDef.h                                            
 * @brief Summary for VC_CommonDef.h
 *
 * This file includes the declaration of class and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef _VC_COMMONDEF_H
#define _VC_COMMONDEF_H

#pragma once

#ifdef __GNUC__
# define VC_API __attribute__((visibility("default")))
#else
# define VC_API
#endif

#define VC_NAVI_LONEPROMPT  0x01
#define VC_NAVI_SHORTPROMPT 0x02
#define VC_NAVI_ALARTTONE   0x03
#define VC_VR_SPEECHREC     0x04
#define VC_TTS_AUDIO        0X05
enum VC_ERROR_TYPE 
{
    APL_ERR_VC_NO_ERROR = 0,
    APL_ERR_VC_INV_PARAM = 0x80000001,
    APL_ERR_VC_INV_OP,
    APL_ERR_VC_NOMEM,
    APL_ERR_VC_USERSTOP,
    APL_ERR_VC_TTSENGINE,
    APL_ERR_VC_AUDIOOUT,
    APL_ERR_VC_AUDIIN,
    APL_ERR_VC_DECODER,
    APL_ERR_VC_UNSUPPORTED,
    APL_ERR_VC_INVALID_OP,
    APL_ERR_VC_INVALID_PARAM,
    APL_ERR_VC_NEW_ERROR,
    APL_ERR_VC_TTS,
    APL_ERR_VC_INVALID_VALUE,
    APL_ERR_VC_UNKNOWN
};

enum VC_AUDIO_MODE_TYPE
{
    VC_AUDIO_MODE_NORMAL = 0,
    VC_AUDIO_MODE_BARGEIN,
    VC_AUDIO_MODE_BARGEIN_WITH_DEBUG
};

enum VC_MODULE_TYPE { VC_MODULE_NAVI = 0, VC_MODULE_TTS, VC_MODULE_VR };

enum VC_AUDIO_TYPE { VC_AUDIO_BEEP = 0, VC_AUDIO_SOUND, VC_AUDIO_NUM };

enum VC_DECODER_TYPE { VC_DECODER_OGG = 0, VC_DECODER_TTS, VC_DECODER_NUM };

#endif
/* EOF */
