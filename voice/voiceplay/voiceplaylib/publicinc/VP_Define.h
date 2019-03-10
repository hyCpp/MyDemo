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
 * @file VP_Define.h
 * @brief definition for useful const
 *
 * This file contains all useful const variables and enumeration type
 *
 */

#ifndef _VP_DEFINE_H
#define _VP_DEFINE_H

#pragma once

#include "CL_NewTypesDefine.h"
#include "BL_Base.h"

// -------------------------------------------
//	ERROR CODE DEFINITION
// -------------------------------------------
// ERROR CODE RANGE of VOICE: 0xF0110000 --- 0xF011FFFF

const CL_ERROR APL_ERR_VP_BASE            = 0xF0110000;
const CL_ERROR APL_ERR_VP_INVALID_PARAM   = 0xF0110010; // (APL_ERR_VP_BASE + 1)
const CL_ERROR APL_ERR_VP_INVALID_VALUE   = 0xF0110011;
const CL_ERROR APL_ERR_VP_UNKNOWN         = 0xF0110012;
const CL_ERROR APL_ERR_VP_UNIMPLEMENT     = 0xF0110013;
const CL_ERROR APL_ERR_VP_UNSUPPORTED     = 0xF0110014;
const CL_ERROR APL_ERR_VP_INVALID_OP      = 0xF0110015;
const CL_ERROR APL_ERR_VP_WAIT_TIMEOUT    = 0xF0110016;
const CL_ERROR APL_ERR_VP_NEW_ERROR       = 0xF0110017;
const CL_ERROR APL_ERR_VP_MEMORY_ERROR    = 0xF0110018;
const CL_ERROR APL_ERR_VP_BUFFER_SMALL    = 0xF0110019;
const CL_ERROR APL_ERR_VP_USER_STOP       = 0xF011001A;
const CL_ERROR APL_ERR_VP_FILE_NOTEXIST   = 0xF0110020;
const CL_ERROR APL_ERR_VP_FILE_OPEN       = 0xF0110021;
const CL_ERROR APL_ERR_VP_FILE_READ       = 0xF0110022;
const CL_ERROR APL_ERR_VP_PHRASE_NOTEXIST = 0xF0110023;
const CL_ERROR APL_ERR_VP_PHRASE_TOOMANY = 0xF0110024;
const CL_ERROR APL_ERR_VP_TTSENGINE = 0xF0110031;
const CL_ERROR APL_ERR_VP_AUDIO_PLAY = 0xF0110032;
const CL_ERROR APL_ERR_VP_JNI_ERR = 0xF0110033;

// -------------------------------------------
//	REQUEST PHRASE COUNT
// -------------------------------------------
const DWORD VP_MAX_PHRASE = 20;

// -------------------------------------------
//	PLAY RESULT
// -------------------------------------------
enum VP_PLAY_RESULT 
{
    VP_PLAYRESULT_OK = 0,
    VP_PLAYRESULT_ABORT,
    VP_PLAYRESULT_ERROR
};

// -------------------------------------------
//  SPECIAL WAVE ID DEFINATION
// -------------------------------------------
// ALL MODEL COMMON
const DWORD VP_WAVE_ID_SPEEDCAMERA          = 0x1C101101;
const DWORD VP_WAVE_ID_CAMERA_IN_FRONT_200M = 0x1C101102;
const DWORD VP_WAVE_ID_CAMERA_IN_FRONT_500M = 0x1C101103;

// CHINA MODEL
const DWORD VP_WAVE_ID_CANCEL_1 = 0x82109105;
const DWORD VP_WAVE_ID_CANCEL_2 = 0x82109106;
const DWORD VP_WAVE_ID_CANCEL_3 = 0x82106130;

const DWORD VP_WAVE_ID_VOLUME_CHANGE_JP = 0x4d;
const DWORD VP_WAVE_ID_10097_JP = 0x010097;

enum VP_CHANNEL_STATUS 
{
    CHANNEL_STATUS_UNKNOWN = -1,
    CHANNEL_STATUS_IDLE,
    CHANNEL_STATUS_OPENING,
    CHANNEL_STATUS_OPENED,
    CHANNEL_STATUS_CLOSING,
    CHANNEL_STATUS_CLOSED,
    CHANNEL_STATUS_WAITING,
};

enum VP_PLAYER_STATUS
{
    PLAYER_STATUS_UNKNOWN = -1,
    PLAYER_STATUS_IDLE = 0,
    PLAYER_STATUS_READY,        // indicate that player has been ready to play the current play list, next expected status is PLAYING
    PLAYER_STATUS_PLAYING,
    PLAYER_STATUS_STOPPING,
    PLAYER_STATUS_STOPPED   // an ephemeral state to indicate player has been stopped, the state is usually transferred to IDLE or READY state immediately
};

enum VP_SMALL_SIGNAL_ID
{
    VP_SS_DEFAULT_ID = -1,  
    VP_SS_ROUTE_GUIDANCE,       // for Guide and Path
    VP_SS_TRAFFIC_INFORMATION_GUIDANCE, // for Traffic triggered by Navi
    VP_SS_TRAFFIC_GUIDANCE,     // for Traffic when RTIC button pressed
    VP_SS_NAVI_NOTIFICATION,    // for Point, Effect and UI
    VP_SS_SETTING_UI           // for SettingUI
};

// request id to tts
const DWORD TTS_REQUST_ID = 1;

// max TTS request ID
const int VP_MAX_REQID = 0xFFFF;
// invalid TTS request ID

const int VP_INVALID_REQID = 0xFFFFFFFF;	// -1

const int VP_ROUTE_GUIDANCE_TOKEN_ID = 18;

#endif
/* EOF */
