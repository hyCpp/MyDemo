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
 * @file TTS_IF.h
 * @brief Declaration file of class TTS_IF.
 *
 * This file includes the declaration of class TTS_IF and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef _CXX_TTS_IF_H_
#define _CXX_TTS_IF_H_

#include "BL_String.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TtsCapability;
class TtsPlaylist;

enum TTS_ErrorCode
{
    TTS_ERROR_NO_ERROR,
    TTS_ERROR_PARAM,
    TTS_ERROR_NO_RESOURCE,
    TTS_ERROR_NO_MEMORY,
    TTS_ERROR_NULL_POINTER,
    TTS_ERROR_ENGINEKEDA,
    TTS_ERROR_FILE_ERROR,
    TTS_ERROR_LANG_ERROR
};

enum TTS_PARAM_TYPE
{
    TTS_PARAM_ERROR = -1,
    TTS_PARAM_ROLE,
    TTS_PARAM_VOLUME,
    TTS_PARAM_PITCH,
    TTS_PARAM_SPEED,
    TTS_PARAM_AUDIO_STREAM
};

/*
enum TTS_AUDIO_STREAM_TYPE
{
    TTS_AUDIO_STREAM_NORMAL = 0x01,
    TTS_AUDIO_STREAM_BARGIN = 0x02,
    TTS_AUDIO_STREAM_FILE = 0x03
};
*/

enum TTS_STREAM_DEVICE_TYPE
{
    // outputs
    TTS_STREAM_DEVICE_OUTPUT_SPEAKER = 0,            ///< Device Type for output speaker
    TTS_STREAM_DEVICE_OUTPUT_REAR = 1,               ///< Device Type for output rear
    TTS_STREAM_DEVICE_OUTPUT_INTERRUPT = 2,          ///< Device Type for output interrupt
    TTS_STREAM_DEVICE_OUTPUT_HANDSFREE = 3,          ///< Device Type for output handsfree
    TTS_STREAM_DEVICE_OUTPUT_BEEP = 4,               ///< Device Type for Beep (L2 is MOST)
    TTS_STREAM_DEVICE_OUTPUT_REAR_MOST = 5,          ///< Device Type for DIRANA3 rear output (L2 only)
    TTS_STREAM_DEVICE_OUTPUT_MIRROR = 6,             ///< Device Type for mirror case

    // inputs
    TTS_STREAM_DEVICE_INPUT_MIC = 7,                 ///< Device Type for input mic
    TTS_STREAM_DEVICE_INPUT_HANDSFREE = 8,           ///< Device Type for input handsfree
    TTS_STREAM_DEVICE_INPUT_VR = 9,                  ///< Device Type for input vr
    TTS_STREAM_DEVICE_INPUT_USB1 = 10,               ///< Device Type for usb audio port 1
    TTS_STREAM_DEVICE_INPUT_USB2 = 11,               ///< Device Type for usb audio prot 2
    TTS_STREAM_DEVICE_INPUT_FRONT_MECHA = 12,        ///< Device Type for front mecha
    TTS_STREAM_DEVICE_INPUT_REAR_MECHA = 13,         ///< Device Type for rear mecha
    TTS_STREAM_DEVICE_INPUT_REAR_FADE = 14,          ///< Device Type for DIRANA3 rear input (L2 only)
    TTS_STREAM_DEVICE_INPUT_INTERRUPT = 15,          ///< Device Type for DiRaNA3 interrupt input (L2 only)
    TTS_STREAM_DEVICE_INPUT_CACHERADIO = 16,         ///< Device Type for cache radio

    // virtual device
    TTS_STREAM_DEVICE_VIRTUALINPUT_A2DP = 17,        ///< a2dp virtual device input
    TTS_STREAM_DEVICE_VIRTUALINPUT_FILE = 18,        ///< file virtual input

    TTS_STREAM_DEVICE_VR_IN = 19,                    ///< for vr barge in
    TTS_STREAM_DEVICE_VR_OUT = 20,                   ///< for vr barge in tts
    TTS_STREAM_DEVICE_VIRTUALOUPUT_NULL = 21,        ///< for null output

    TTS_STREAM_DEVICE_VIRTUALOUPUT_FILE = 22,        ///< for file output
    TTS_STREAM_DEVICE_VIRTUALOUPUT_REMOTE = 23,      ///< for remote output

    TTS_STREAM_DEVICE_TYPE_TOTAL_NUM

};

enum TTS_PRIORITY
{
    TTS_OP_PRI_VR_T,
    TTS_OP_PRI_NAVI_T,
    TTS_OP_PRI_GENERAL_T
};

enum TTS_QUEUE_POLICY
{
    TTS_OP_QUEUE_ADD_T = 0,
    TTS_OP_QUEUE_ABORT_T
};

enum TTS_RESUME_POLICY
{
    TTS_OP_RESUME_CURRENT_T = 0,
    TTS_OP_RESUME_NEXT_T,
    TTS_OP_RESUME_RESTART_T,
    TTS_OP_RESUME_ABORT_T
};

struct TTS_TuningData
{
    struct TTS_APDBTuningData
    {
        BL_String apdbFileFullPath;
        BL_String rulesetFileFullPath;
    };

    BL_String sender;
    TTS_APDBTuningData apdbTuningData;
    BL_String replaceFileFullPath;
};

/**
* TTS_POLICY
*
* The class is just for TTS_POLICY.
*/
class TTS_POLICY
{
public:
    TTS_POLICY()
    : priority(TTS_OP_PRI_GENERAL_T),
      queuePolicy(TTS_OP_QUEUE_ADD_T),
      resumePolicy(TTS_OP_RESUME_CURRENT_T)
    {
    }

    TTS_POLICY(TTS_PRIORITY pri, TTS_QUEUE_POLICY queue, TTS_RESUME_POLICY resume)
    : priority(pri),
      queuePolicy(queue),
      resumePolicy(resume)
    {
    }

    TTS_POLICY(const TTS_POLICY &policy)
    : priority(policy.priority),
      queuePolicy(policy.queuePolicy),
      resumePolicy(policy.resumePolicy)
    {
    }

    ~TTS_POLICY() 
    {
    }

    TTS_POLICY& operator = (const TTS_POLICY& policy)
    {
        priority = policy.priority;
        queuePolicy = policy.queuePolicy;
        resumePolicy = policy.resumePolicy;
        return *this;
    }

    bool operator == (const TTS_POLICY &policy2) const
    {
        return this->priority == policy2.priority && this->queuePolicy == policy2.queuePolicy && this->resumePolicy == policy2.resumePolicy;
    }

    bool operator != (const TTS_POLICY &policy2)
    {
        return !(this->priority == policy2.priority && this->queuePolicy == policy2.queuePolicy && this->resumePolicy == policy2.resumePolicy);
    }

    TTS_PRIORITY 			priority;
    TTS_QUEUE_POLICY 		queuePolicy;
    TTS_RESUME_POLICY		resumePolicy;

};

enum TTS_PLAY_RESULT
{
    TTS_PLAY_OK,
    TTS_PLAY_ABORT,
    TTS_PLAY_ERROR,
    TTS_PLAY_VRMODE,
    TTS_PLAY_INTERRUPT
};


enum TTS_REGISTER_RESULT
{
    TTS_REGISTER_RESULT_OK,
    TTS_REGISTER_RESULT_OPENFILEFAILED,
    TTS_REGISTER_RESULT_FORMATERROR,
    TTS_REGISTER_RESULT_ERROR
};

enum TTS_PLAYER_STATE
{
    TTS_PLAYER_WAIT,
    TTS_PLAYER_START,
    TTS_PLAYER_PAUSE,
    TTS_PLAYER_RESUME,
    TTS_PLAYER_PHRASE_FINISH,
    TTS_PLAYER_STOPED,
    TTS_PLAYER_ABORT,
    TTS_PLAYER_ERROR_ID,
    TTS_PLAYER_ERROR_LANG,
    TTS_PLAYER_ERROR,
};

#endif /* _CXX_TTS_IF_H_ */
/* EOF */
