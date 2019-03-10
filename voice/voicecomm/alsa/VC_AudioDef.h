/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
 * @file NMAudio.h
 * @brief definitions of NMAudio.
 *
 * This file includes the the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VCAUDIODEF_H
#define VCAUDIODEF_H

#include <pulse/stream.h>

#define AUDIO_STREAM_NAME_MAX_LEN           (256)
#define NMAUDIO_RATE_32000                  (32000)
#define NMAUDIO_RATE_44100                  (44100)
#define NMAUDIO_RATE_48000                  (48000)
#define INVALID_TOKEN_ID_32                 (-1)

#define PA_EVENT_STREAM_FADE_IN                   "stream.fade-in"
#define PA_EVENT_STREAM_FADE_OUT                  "stream.fade-out"

#define PA_PROP_STREAM_TOKEN_ID                   "stream.tokenid"
#define PA_A2DP_DECODE_NAME                       "a2dp-decode"
#define PA_PROP_FADE_TIME                         "stream.fadetime"
#define PA_PROP_FADE_IN_TIME                      "stream.fade-in-time"
#define PA_PROP_FADE_OUT_TIME                     "stream.fade-out-time"
#define PA_PROP_LATENCY_HW                        "stream.latency-hw"
#define PA_PROP_ARGS                              "ext-args"
#define PA_PROP_DTCP_ID_DEVICE                    "dtcp.device"

#ifdef __cplusplus
extern "C"
{
#endif

struct pa_simple {
    pa_context *context;
    pa_stream *stream;
    pa_stream_direction_t direction;

    const void *read_data;
    size_t read_index, read_length;

    int operation_success;
};

enum EAUDIO_ERR_TYPE
{
    EAUDIO_NOERROR = 0,                       ///< no error
    EAUDIO_ERROR_INVALID_PARAMETER = -1,      ///< error of invalid parameters
    EAUDIO_ERROR_NO_CREATE = -2,              ///< operate a not created stream
    EAUDIO_ERROR_PULSEAUDIO = -3,             ///< error when call pulseaudio api
    EAUDIO_ERROR_CONTEXTNOTREADY  = -4,       ///< error when context is not ready
    EAUDIO_ERROR_LOST_CONNECTION = -5,        ///< error when server died or lost connection
    EAUDIO_ERROR_ALREADY_CREATED = -6,        ///< stream already created
    EAUDIO_ERROR_SEND_COMMAND = -7,           ///< stream send command result
    EAUDIO_ERROR_TOKEN_INVALID = -8,          ///< stream token no valid
};

// the value must be same as pa_sample_format definitions in sample.h
enum EAUDIO_STREAM_FORMAT_TYPE
{
    EAUDIO_STREAM_FORMAT_TYPE_U8,
    EAUDIO_STREAM_FORMAT_TYPE_ALAW,
    EAUDIO_STREAM_FORMAT_TYPE_ULAW,
    EAUDIO_STREAM_FORMAT_TYPE_S16LE,
    EAUDIO_STREAM_FORMAT_TYPE_S16BE,
    EAUDIO_STREAM_FORMAT_TYPE_FLOAT32LE,
    EAUDIO_STREAM_FORMAT_TYPE_FLOAT32BE,
    EAUDIO_STREAM_FORMAT_TYPE_S32LE,
    EAUDIO_STREAM_FORMAT_TYPE_S32BE,
    EAUDIO_STREAM_FORMAT_TYPE_S24LE,
    EAUDIO_STREAM_FORMAT_TYPE_S24BE,
    EAUDIO_STREAM_FORMAT_TYPE_S24_32LE,
    EAUDIO_STREAM_FORMAT_TYPE_S24_32BE,
    EAUDIO_STREAM_FORMAT_TYPE_MAX,
    EAUDIO_STREAM_FORMAT_TYPE_AUTO,
    EAUDIO_STREAM_FORMAT_TYPE_INVALID = -1
};

enum EAUDIO_STREAM_DIRECTION
{
    EAUDIO_STREAM_DIRECTION_PLAYBACK,                   ///< playback direction
    EAUDIO_STREAM_DIRECTION_RECORD,                     ///< capture direction
    EAUDIO_STREAM_DIRECTION_UPLOAD,                     ///< Sample upload stream
    EAUDIO_STREAM_DIRECTION_INVALID = -1
};

enum EAUDIO_STREAM_DEVICE_TYPE
{
    // outputs
    EAUDIO_STREAM_DEVICE_OUTPUT_SPEAKER = 0,            ///< Device Type for output speaker
    EAUDIO_STREAM_DEVICE_OUTPUT_REAR = 1,               ///< Device Type for output rear
    EAUDIO_STREAM_DEVICE_OUTPUT_INTERRUPT = 2,          ///< Device Type for output interrupt
    EAUDIO_STREAM_DEVICE_OUTPUT_HANDSFREE = 3,          ///< Device Type for output handsfree
    EAUDIO_STREAM_DEVICE_OUTPUT_BEEP = 4,               ///< Device Type for Beep (L2 is MOST)
    EAUDIO_STREAM_DEVICE_OUTPUT_REAR_MOST = 5,          ///< Device Type for DIRANA3 rear output (L2 only)
    EAUDIO_STREAM_DEVICE_OUTPUT_MIRROR = 6,             ///< Device Type for mirror case

    // inputs
    EAUDIO_STREAM_DEVICE_INPUT_MIC = 7,                 ///< Device Type for input mic
    EAUDIO_STREAM_DEVICE_INPUT_HANDSFREE = 8,           ///< Device Type for input handsfree
    EAUDIO_STREAM_DEVICE_INPUT_VR = 9,                  ///< Device Type for input vr
    EAUDIO_STREAM_DEVICE_INPUT_USB1 = 10,               ///< Device Type for usb audio port 1
    EAUDIO_STREAM_DEVICE_INPUT_USB2 = 11,               ///< Device Type for usb audio prot 2
    EAUDIO_STREAM_DEVICE_INPUT_FRONT_MECHA = 12,        ///< Device Type for front mecha
    EAUDIO_STREAM_DEVICE_INPUT_REAR_MECHA = 13,         ///< Device Type for rear mecha
    EAUDIO_STREAM_DEVICE_INPUT_REAR_FADE = 14,          ///< Device Type for DIRANA3 rear input (L2 only)
    EAUDIO_STREAM_DEVICE_INPUT_INTERRUPT = 15,          ///< Device Type for DiRaNA3 interrupt input (L2 only)
    EAUDIO_STREAM_DEVICE_INPUT_CACHERADIO = 16,         ///< Device Type for cache radio

    // virtual device
    EAUDIO_STREAM_DEVICE_VIRTUALINPUT_A2DP = 17,        ///< a2dp virtual device input
    EAUDIO_STREAM_DEVICE_VIRTUALINPUT_FILE = 18,        ///< file virtual input

    EAUDIO_STREAM_DEVICE_VR_IN = 19,                    ///< for vr barge in
    EAUDIO_STREAM_DEVICE_VR_OUT = 20,                   ///< for vr barge in tts
    EAUDIO_STREAM_DEVICE_VIRTUALOUPUT_NULL = 21,        ///< for null output

    EAUDIO_STREAM_DEVICE_VIRTUALOUPUT_FILE = 22,        ///< for file output
    EAUDIO_STREAM_DEVICE_VIRTUALOUPUT_REMOTE = 23,      ///< for remote output

    EAUDIO_STREAM_DEVICE_TYPE_TOTAL_NUM

};

enum EAUDIO_STREAM_CHANNLE_TYPE
{
    EAUDIO_STREAM_CHANNEL_MONO = 1,                          ///< mono channel
    EAUDIO_STREAM_CHANNEL_STEREO = 2,                        ///< stereo channel
    EAUDIO_STREAM_CHANNEL_FIVE_ONE = 6,                      ///< 5.1 channel
    EAUDIO_STREAM_CHANNEL_TYPE_NUM
};

enum NMAUDIO_EXT_TYPE
{
    NMAUDIO_EXT_VR = 0,


    NMAUDIO_EXT_TYPE_NUM
};

enum NMAudioMuteStatus
{
    NMAUDIO_MUTE_OFF = 0,
    NMAUDIO_MUTE_ON,
};

enum NMAudioFunctionStatus
{
    NMAUDIO_FUNCTION_OFF = 0,
    NMAUDIO_FUNCTION_ON,
};

enum NMAudioFadeMode
{
    NMAUDIO_FADE_OUT = 0,
    NMAUDIO_FADE_IN,
};

enum NMAudioZeroDataMode
{
    NMAUDIO_ZERO_DATA_OUTPUT_OFF = 0,
    NMAUDIO_ZERO_DATA_OUTPUT_ON,
};

#ifdef __cplusplus
}
#endif

#endif /* NMAUDIODEF_H */
/* EOF */
