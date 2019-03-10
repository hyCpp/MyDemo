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

#include "stdafx.h"
#include "VC_AudioStream.h"
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "pulse/simple.h"
#include "pulse/error.h"
#include "pulse/xmalloc.h"
#include "pulse/context.h"
#include "pulse/introspect.h"
#include "pulse/stream.h"
#include "pulse/thread-mainloop.h"
#include "pulse/sample.h"
#include "pulse/timeval.h"
#include "pulse/rtclock.h"
#include "pulse/util.h"

#ifdef __cplusplus
}
#endif

#define DEFAULT_MUSIC_LATENCY       (200)
#define MIN_LATENCY_MS              (25)

#define API_LOCK                    std::lock_guard<std::mutex> lock(m_mutex);
#define CHECK_POINTER_RET(ptr, ret) \
    if (!(ptr)) {\
        VC_LOGE(#ptr"=NULL %s, %d", __PRETTY_FUNCTION__, __LINE__); \
        return (ret); \
    }

#define DEV_NAME_PREFIX     "audioio."
#define GET_PLUGIN_NAME(name, device) snprintf((name), sizeof(name), DEV_NAME_PREFIX"%d", (device));

static pa_stream_direction_t get_pa_direction(EAUDIO_STREAM_DIRECTION dir)
{
    switch (dir) {
    case EAUDIO_STREAM_DIRECTION_PLAYBACK:
        return PA_STREAM_PLAYBACK;
    case EAUDIO_STREAM_DIRECTION_RECORD:
        return PA_STREAM_RECORD;
    case EAUDIO_STREAM_DIRECTION_UPLOAD:
        return PA_STREAM_UPLOAD;
    case EAUDIO_STREAM_DIRECTION_INVALID:
        return PA_STREAM_NODIRECTION;
    default:
        return PA_STREAM_NODIRECTION;
        break;
    }
}

static int audio_fmt_to_pa_fmt(int format)
{
    int audio_fromats[] =
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
    };

    int pulse_formats[] =
    {
        PA_SAMPLE_U8,
        PA_SAMPLE_ALAW,
        PA_SAMPLE_ULAW,
        PA_SAMPLE_S16LE,
        PA_SAMPLE_S16BE,
        PA_SAMPLE_FLOAT32LE,
        PA_SAMPLE_FLOAT32BE,
        PA_SAMPLE_S32LE,
        PA_SAMPLE_S32BE,
        PA_SAMPLE_S24LE,
        PA_SAMPLE_S24BE,
        PA_SAMPLE_S24_32LE,
        PA_SAMPLE_S24_32BE,
        PA_SAMPLE_MAX,
    };

    size_t num = sizeof(audio_fromats) / sizeof(audio_fromats[0]);
    for (size_t i = 0; i < num; ++i) {
        if (format == audio_fromats[i]) {
            return pulse_formats[i];
        }
    }
    return PA_SAMPLE_INVALID;
}

static void transfer_sample_spec(pa_sample_spec& ss, const VC_AudioStream::VC_AudioStreamConfig& config)
{
    ss.rate = config.rate;
    ss.format = static_cast<pa_sample_format>(audio_fmt_to_pa_fmt(config.format));
    ss.channels = config.channels;
}

static void dump_buffer_attr(const char* msg, const pa_buffer_attr* attr, const pa_sample_spec* ss)
{
    VC_LOGD("%s pa_buffer_attr latency=%.2fms, prebuf=%.2fms, minreq=%.2f",
            msg,
            static_cast<double>(pa_bytes_to_usec(attr->tlength, ss) / PA_USEC_PER_MSEC),
            static_cast<double>(pa_bytes_to_usec(attr->prebuf, ss) / PA_USEC_PER_MSEC),
            static_cast<double>(pa_bytes_to_usec(attr->minreq, ss) / PA_USEC_PER_MSEC));
}

VC_AudioStream::VC_AudioStreamConfig::VC_AudioStreamConfig()
: format(EAUDIO_STREAM_FORMAT_TYPE_S16LE)
, direction(EAUDIO_STREAM_DIRECTION_PLAYBACK)
, device(EAUDIO_STREAM_DEVICE_OUTPUT_SPEAKER)
, rate(48000)
, channels(EAUDIO_STREAM_CHANNEL_STEREO)
, token(0)
, fade_in_time(0)
, fade_out_time(0)
, prebuf(-1)
, latency(-1)
{

}

VC_AudioStream::VC_AutoRefCount::VC_AutoRefCount(pa_atomic_t *t)
    : m_ref(t)
{
    if (m_ref) {
        pa_atomic_inc(m_ref);
    }
}

VC_AudioStream::VC_AutoRefCount::~VC_AutoRefCount()
{
    if (m_ref) {
        pa_atomic_dec(m_ref);
    }
}

VC_AudioStream::VC_AudioStream()
: m_config()
, m_psimple(NULL)
, m_workFlag(new pa_atomic_t())
, m_lastOpRst(0)
{
    pa_atomic_store(m_workFlag, 0);
}

VC_AudioStream::~VC_AudioStream()
{
    innerDestroy();

    delete m_workFlag;
    m_workFlag = NULL;
}

//================================================================================================
//         BEGIN OF PUBLIC API
//================================================================================================

int32_t VC_AudioStream::destroy(void)
{
    return innerDestroy();
}

int32_t VC_AudioStream::create(const VC_AudioStreamConfig& config)
{
    API_LOCK;

    if (m_psimple) {
        VC_LOGE("stream already created, destory it first!.");
        return EAUDIO_ERROR_ALREADY_CREATED;
    }

    VC_LOGD("audio-stream create fmt=%d, dir=%d, dev=%d, r=%d, c=%d, t=%d, f=(%d,%d)",
            config.format, config.direction, config.device,
            config.rate, config.channels, config.token,
            config.fade_in_time, config.fade_out_time);

    m_config = config;

    /*********************************************************
        start echocancel if we are recording
    **********************************************************/
    /* bool cond_1 = config.echocancel != NMAUDIO_ChannelId_Num;
    bool cond_2 = config.direction == EAUDIO_STREAM_DIRECTION_RECORD;
    bool cond_3 = (config.device == EAUDIO_STREAM_DEVICE_INPUT_MIC) || (config.device == EAUDIO_STREAM_DEVICE_VR_IN);
    if (cond_1 && cond_2 && cond_3) {
        VC_LOGD("start confg.echocancel = %d", config.echocancel);
        NMEchoCancelConfig ec_cfg;
        memset(&ec_cfg, 0x00, sizeof(ec_cfg));
        ec_cfg.format = config.format;
        ec_cfg.rate = config.rate;
        ec_cfg.channels = config.channels;
        ec_cfg.token = config.token;
        ec_cfg.input = EAUDIO_STREAM_DEVICE_TYPE_TOTAL_NUM;
        ec_cfg.output = EAUDIO_STREAM_DEVICE_TYPE_TOTAL_NUM;
        NMAudioSetting::startEchoCancel_Common(config.echocancel, ec_cfg, this);
    }*/

    pa_sample_spec ss;
    transfer_sample_spec(ss, config);
    VC_LOGD("query finished finshed");

    char appName[AUDIO_STREAM_NAME_MAX_LEN] = { 0 };
    snprintf(appName, sizeof(appName), "#%d", getpid());

    static int stream_index = 0;
    char streamName[AUDIO_STREAM_NAME_MAX_LEN*2] = { 0 };
    snprintf(streamName, sizeof(streamName), "%s_%d", appName, stream_index++);

    VC_LOGD("app=%s, stream=%s", appName, streamName);

    int error = 0;
    /*
    pa_proplist *proplist = pa_proplist_new();
    pa_proplist_set(proplist, PA_PROP_STREAM_TOKEN_ID, &config.token, sizeof(config.token));
    pa_proplist_set(proplist, PA_PROP_FADE_IN_TIME, &config.fade_in_time, sizeof(config.fade_in_time));
    pa_proplist_set(proplist, PA_PROP_FADE_OUT_TIME, &config.fade_out_time, sizeof(config.fade_out_time));

    pa_init_proplist(proplist);
    */

    uint32_t init_value = static_cast<uint32_t>(-1);
    pa_buffer_attr attr = { init_value, init_value, init_value, init_value, init_value };

    int prebuf = config.prebuf;
     if (config.device == EAUDIO_STREAM_DEVICE_VR_OUT) {
        // bargein sink is controled by bargein-lib
        // prebuf will make silence popded, that`s not accpetable
        prebuf = 0;
    }
    if (prebuf >= 0) {
        VC_LOGD("target prebuffer time %d MS", prebuf);
        attr.prebuf = pa_usec_to_bytes(prebuf * PA_USEC_PER_MSEC, &ss);
    }

    int latency = config.latency;
    if (latency > 0 && latency < MIN_LATENCY_MS) {
        latency = MIN_LATENCY_MS;
    }

    if (latency > 0) {
        VC_LOGD("target length time %d MS", latency);
        attr.tlength = pa_usec_to_bytes(latency * PA_USEC_PER_MSEC, &ss);
        attr.fragsize = attr.tlength;
    }
    else {
        latency = DEFAULT_MUSIC_LATENCY;
        VC_LOGD("using default %d MS", latency);
        attr.tlength = pa_usec_to_bytes(latency * PA_USEC_PER_MSEC, &ss); /* 250ms of buffering */
    }
    attr.minreq = pa_usec_to_bytes(10 * PA_USEC_PER_MSEC, &ss);
    dump_buffer_attr("create", &attr, &ss);

    {
        char dev[64];
        GET_PLUGIN_NAME(dev, config.device);
        m_psimple = pa_simple_new(NULL, "VC_AudioStream", get_pa_direction(config.direction),
                                  NULL, streamName, &ss, NULL, &attr, &error);
        /* m_psimple = pa_simple_new_extend2(
                config.device,
                get_pa_direction(config.direction),
                streamName,
                &ss,
                NULL,
                &attr,
                proplist,
                &error); */
    }

    // pa_proplist_free(proplist);

    if (error || !m_psimple) {
        VC_LOGE("pa_simple_new_extend error %s", pa_strerror(error));
        if (m_psimple) {
            pa_simple_free(m_psimple);
        }
        return processErrorCode("pa_simple_new_extend", error);
    }

    return EAUDIO_NOERROR;
}

/*
pa_simple* VC_AudioStream::pa_simple_new_extend2(uint32_t device, int dir,
                                                const char* stream_name,
                                                const pa_sample_spec *ss,
                                                const pa_channel_map *map,
                                                const pa_buffer_attr *attr,
                                                pa_proplist* plist)
{
    pa_simple* p;
    int error = PA_ERR_INTERNAL, r;
    char dev[128] = {0};

    snprintf(dev, sizeof(dev), "audioio.%d", device);
    p = pa_xnew0(pa_simple, 1);
    p->direction = dir;
    p->stream = pa_stream_new_with_proplist(p->context, stream_name, ss, map, plist);

    if (!p->stream) {
        pa_simple_free_ext(p);
        return NULL;
    }

    int flags = PA_STREAM_INTERPOLATE_TIMING | PA_STREAM_ADJUST_LATENCY | PA_STREAM_AUTO_TIMING_UPDATE;
    if (dir == PA_STREAM_PLAYBACK) {
        r = pa_stream_connect_playback(p->stream, dev, attr, flags, NULL, NULL);
    }
    else {
        r = pa_stream_connect_record(p->stream, dev, attr, flags);
    }

    while(true) {
        pa_stream_state_t state = pa_stream_get_state(p->stream);

        if (state == PA_STREAM_READY) {
            break;
        }

        if (!PA_STREAM_IS_GOOD(state)) {
            VC_LOGE("State of pulse audio is not good!");
            pa_simple_free_ext(p);
            return NULL;
        }

        usleep(100);
    }

    return p;
}
*/

/*
void VC_AudioStream::pa_simple_free_ext(pa_simple* s)
{
    if (!s || !s->stream) {
        return;
    }

    pa_stream_state_t state = pa_stream_get_state(s->stream);
    if (state == PA_STREAM_READY) {
        pa_stream_disconnect(s->stream);
    }

    pa_stream_unref(s->stream);
    s->stream = NULL;

    pa_xfree(s);
}
*/

int32_t VC_AudioStream::read(void* data, uint32_t bytes)
{
    // this is how it works no lock in all performance critical functions
    VC_AutoRefCount _r(m_workFlag);

    pa_sample_spec ss;
    transfer_sample_spec(ss, m_config);
    size_t frame_size = pa_frame_size(&ss);
    if (bytes < frame_size) {
        return 0;
    }

    // bytes = pa_frame_align(bytes, &ss);

    int error = 0;
    CHECK_POINTER_RET(m_psimple, EAUDIO_ERROR_NO_CREATE);
    int nRet = pa_simple_read(m_psimple, data, bytes, &error);
    if (nRet < 0 || error != 0) {
        return processErrorCode("pa_simple_read", error);
    }

    return bytes;
}

int32_t VC_AudioStream::write(const void* data, uint32_t bytes)
{
    VC_AutoRefCount _r(m_workFlag);

    pa_sample_spec ss;
    transfer_sample_spec(ss, m_config);
    size_t frame_size = pa_frame_size(&ss);
    if (bytes < frame_size) {
        return 0;
    }

    // bytes = pa_frame_align(bytes, &ss);
    int error = 0;
    CHECK_POINTER_RET(m_psimple, EAUDIO_ERROR_NO_CREATE);

    int nRet = pa_simple_write(m_psimple, data, bytes, &error);
    if (nRet < 0 || error != 0) {
        return processErrorCode("pa_simple_write", error);
    }

    return bytes;
}

int32_t VC_AudioStream::get_drain_latency()
{
    /* this will need more sophiscated porting job
     * just comment it temporarily
    int64_t r = getBufferLatencySize();
    pa_sample_spec ss;
    transfer_sample_spec(ss, m_config);
    pa_usec_t t = pa_bytes_to_usec(r, &ss);
    VC_LOGD("stream drain left_to_play = %.2f ms", (double) t / PA_USEC_PER_MSEC);
    int32_t ms = t / PA_USEC_PER_MSEC;

#ifndef EMULATOR
    if (ms > 2000) {
        ms = 2000;
    }
    else if (ms < 0) {
        ms = 500;
    }
#endif
    */

    return 800;
}

int32_t VC_AudioStream::drain(int sleep_ms)
{
    API_LOCK;

    VC_AutoRefCount _r(m_workFlag);

    int error = 0;
    int nRet = pa_simple_drain(m_psimple, &error);
    if (nRet < 0 || error != 0) {
        return processErrorCode("pa_simple_drain", error);
    }

    int32_t ms = get_drain_latency();
    pa_msleep(ms);

#ifdef MEU_L2
    // thanks to TTS
    if (m_config.device == EAUDIO_STREAM_DEVICE_OUTPUT_INTERRUPT) {
        pa_msleep(sleep_ms);
    }
#endif

    VC_LOGD("finished drain block");

    return EAUDIO_NOERROR;
}

int32_t VC_AudioStream::innerFlush(int sleep_ms)
{
    int error = 0;
    int nRet = pa_simple_flush(m_psimple, &error);
    if (nRet < 0 || error != 0) {
        return processErrorCode("pa_simple_flush", error);
    }

    int32_t ms = get_drain_latency();
    pa_msleep(ms);

#ifdef MEU_L2
    // thanks to TTS
    if (m_config.device == EAUDIO_STREAM_DEVICE_OUTPUT_INTERRUPT) {
        pa_msleep(sleep_ms);
    }
#endif

    return EAUDIO_NOERROR;
}

int32_t VC_AudioStream::flush_nointerrupt(int sleep_ms)
{
    API_LOCK;

    VC_AutoRefCount _r(m_workFlag);

    int32_t nRet = innerFlush(sleep_ms);
    return nRet;
}

int32_t VC_AudioStream::flush(int sleep_ms)
{
    API_LOCK;

    VC_AutoRefCount _r(m_workFlag);

    // pa_simple_interrupt(m_psimple);
    int32_t nRet = innerFlush(sleep_ms);
    // pa_simple_reset(m_psimple);
    return nRet;
}

int32_t VC_AudioStream::interrupt()
{
    VC_AutoRefCount _r(m_workFlag);

    CHECK_POINTER_RET(m_psimple, EAUDIO_ERROR_NO_CREATE);
    // pa_simple_interrupt(m_psimple);
    return EAUDIO_NOERROR;
}

int32_t VC_AudioStream::reset()
{
    VC_AutoRefCount _r(m_workFlag);

    CHECK_POINTER_RET(m_psimple, EAUDIO_ERROR_NO_CREATE);
    // pa_simple_reset(m_psimple);
    return EAUDIO_NOERROR;
}

/*
int64_t VC_AudioStream::getLatency(bool update_time_info)
 {
     API_LOCK;

     VC_AutoRefCount _r(m_workFlag);

     pa_stream* s = pa_simple_get_stream(m_psimple);
     CHECK_POINTER_RET(s, EAUDIO_ERROR_NO_CREATE);

     if (update_time_info) {
         pa_stream_update_timing_info(s, NULL, NULL);
     }

     int error = 0;
     pa_usec_t latency_time = pa_simple_get_latency(m_psimple, &error);
     if (error != 0) {
         return processErrorCode("pa_simple_get_latency", error);
     }
     return static_cast<int64_t>(latency_time);
 }

 int64_t VC_AudioStream::getBufferLatencySize(bool update_time_info, bool add_sink_buff)
 {
     API_LOCK;
     VC_AutoRefCount _r(m_workFlag);

     pa_stream* s = pa_simple_get_stream(m_psimple);
     CHECK_POINTER_RET(s, EAUDIO_ERROR_NO_CREATE);

     if (update_time_info) {
         pa_stream_update_timing_info(s, NULL, NULL);
     }

     const pa_timing_info* t = pa_stream_get_timing_info(s);
     if (!t) {
         VC_LOGE("pa_stream_get_timing_info return NULL");
         return EAUDIO_ERROR_PULSEAUDIO;
     }

     if (t->write_index_corrupt || t->read_index_corrupt) {
         VC_LOGE("write_index_corrupt|read_index_corrupt");
         return EAUDIO_ERROR_PULSEAUDIO;
     }

     int64_t ret = t->write_index - t->read_index;
     VC_LOGD("write_index=%lld, read_index=%lld sink-usec=%.2fms\n",
         t->write_index,
         t->read_index,
         (double)t->sink_usec/PA_USEC_PER_MSEC);

     if (add_sink_buff) {
         char name[64] = {0};
         GET_PLUGIN_NAME(name, m_config.device);
         pa_sink *s = pa_namereg_get(c, name, PA_NAMEREG_SINK);
         pa_usec_t left_to_play = pa_sink_get_left_to_play(s);

         VC_LOGD("alsa-buffer-left=%d ms", left_to_play);
         ret += usecToBytes(left_to_play * 1000);
     }

     return PA_MAX(0, ret);
 }

 int64_t VC_AudioStream::getTime(bool update_time_info) const
 {
     API_LOCK;
     VC_AutoRefCount _r(m_workFlag);

     pa_stream* s = pa_simple_get_stream(m_psimple);
     CHECK_POINTER_RET(s, EAUDIO_ERROR_NO_CREATE);

     if (update_time_info) {
         pa_stream_update_timing_info(s, NULL, NULL);
     }

     pa_usec_t t = 0;
     int r = pa_stream_get_time(s, &t);
     if (r < 0) {
         VC_LOGE("pa_stream_get_time error = s", pa_strerror(r));
         return EAUDIO_ERROR_PULSEAUDIO;
     }

     return static_cast<int64_t>(t);
}
*/

//================================================================================================
//         END OF PUBLIC API
//================================================================================================
int VC_AudioStream::processErrorCode(const char* what, int error)
{
    VC_LOGE("processErrorCode %s %s", what, pa_strerror(error));
    if (error == PA_ERR_CONNECTIONTERMINATED) {
        return EAUDIO_ERROR_LOST_CONNECTION;
    }
    else if (error == PA_ERR_AUTHKEY) {
        return EAUDIO_ERROR_TOKEN_INVALID;
    }
    else if (error == PA_OK) {
        return EAUDIO_NOERROR;
    }
    else {
        return EAUDIO_ERROR_PULSEAUDIO;
    }
}

int64_t VC_AudioStream::bytesToUsec(int64_t bytes) const
{
    CHECK_POINTER_RET(m_psimple, EAUDIO_ERROR_NO_CREATE);
    pa_sample_spec ss;
    transfer_sample_spec(ss, m_config);
    return pa_bytes_to_usec(bytes, &ss);
}

int64_t VC_AudioStream::usecToBytes(int64_t usec) const
{
    CHECK_POINTER_RET(m_psimple, EAUDIO_ERROR_NO_CREATE);
    pa_sample_spec ss;
    transfer_sample_spec(ss, m_config);
    return pa_usec_to_bytes(usec, &ss);
}

int VC_AudioStream::innerDestroy()
{
    API_LOCK;

    if (m_psimple) {
        // pa_simple_interrupt(m_psimple);

        int wait_num = 0;
        const int max_wait_num = 1000;
        VC_LOG("stream destroy work flag = %d", pa_atomic_load(m_workFlag));

        // hell no shitty locks, the worst case is waiting for max_wait_num * 1 = 1000 ms
        while (pa_atomic_load(m_workFlag)) {
            pa_msleep(1);
            if (wait_num++ > max_wait_num) {
                VC_LOGE("destroy wait overflow > %d !!!", max_wait_num);
                break;
            }
        }

        pa_simple_free(m_psimple);
        m_psimple = NULL;
    }

    /* bool cond_1 = m_config.echocancel != NMAUDIO_ChannelId_Num;
    bool cond_2 = m_config.direction == EAUDIO_STREAM_DIRECTION_RECORD;
    bool cond_3 = (m_config.device == EAUDIO_STREAM_DEVICE_INPUT_MIC) || (m_config.device == EAUDIO_STREAM_DEVICE_VR_IN);

    // stop echo cancel id if last time it`s started
    if (cond_1 && cond_2 && cond_3) {
        VC_LOGD("stop echocancel = %d", m_config.echocancel);
        NMAudioSetting::stopEchoCancel_Common(m_config.echocancel, this);
    } */

    return EAUDIO_NOERROR;
}

int32_t VC_AudioStream::preInit(int extType, int data)
{
    /* CAutoTimeLog _l("VC_AudioStream::preInit(%d,%d)", extType, data);

    char str_data[32] = { 0 };
    snprintf(str_data, sizeof(str_data), "%d", data);

    KeyValuePair attributes[] =
    {
       { PROPLIST_KEY_NAME, PROPLIST_VALUE_EXT },
       { PROPLIST_KEY_ATTRIBUTE, PROPLIST_VALUE_VR },
       { PROPLIST_KEY_ACTION, PROPLIST_VALUE_PRE_INIT },
       { PROPLIST_KEY_DATA, str_data },
    };

    return sendSimpleCommand(attributes, ARRAY_COUNTOF(attributes)); */
    return -1;
}

int32_t VC_AudioStream::getData(int extType, unsigned *pSize, void *pData)
{
    /* CAutoTimeLog _l("VC_AudioStream::getData(%p,%p)", pSize, pData);

    pa_proplist* p = pa_proplist_new();
    pa_proplist_sets(p, PROPLIST_KEY_NAME, PROPLIST_VALUE_EXT);
    pa_proplist_sets(p, PROPLIST_KEY_ATTRIBUTE, PROPLIST_VALUE_VR);
    pa_proplist_sets(p, PROPLIST_KEY_ACTION, PROPLIST_VALUE_GET_DATA);
    int size = pSize ? *pSize : 0;
    char str_size[32] = { 0 };
    snprintf(str_size, sizeof(str_size), "%d", size);
    pa_proplist_sets(p, PROPLIST_KEY_SIZE, str_size);
    if (pData) {
        pa_proplist_set(p, PROPLIST_KEY_DATA, pData, size);
    }

    pa_proplist* r = sendCommand(p);
    pa_proplist_free(p);

    if (!r) {
        return EAUDIO_ERROR_PULSEAUDIO;
    }

    const char* str_result = pa_proplist_gets(r, PROPLIST_KEY_COMMAND_RESULT);
    if (!str_result) {
        pa_proplist_free(r);
        return EAUDIO_ERROR_PULSEAUDIO;
    }

    int32_t ret = atoi(str_result);
    const char* ret_str_size = pa_proplist_gets(r, PROPLIST_KEY_SIZE);
    if (!ret_str_size) {
        pa_proplist_free(r);
        return EAUDIO_ERROR_PULSEAUDIO;
    }

    int ret_size = atoi(ret_str_size);
    if (pSize) {
        *pSize = ret_size;
    }
    if (pData) {
        if (size < ret_size) {
            pa_proplist_free(r);
            NCLOGE("request buffer size = %d, get buffer size = %d", size, ret_size);
            return EAUDIO_ERROR_INVALID_PARAMETER;
        }
    }

    NCLOGD("get-data ret size = %d", ret_size);

    const void* ret_data = NULL;
    size_t n_bytes = 0;
    pa_proplist_get(r, PROPLIST_KEY_DATA, &ret_data, &n_bytes);

    if (pData && ret_data) {
        memcpy(pData, ret_data, ret_size);
    }

    pa_proplist_free(r);
    return ret; */
    return -1;
}

int32_t VC_AudioStream::preUnInit(int extType)
{
    /* CAutoTimeLog _l("VC_AudioStream::preUnInit(%d)", extType);

    KeyValuePair attributes[] =
    {
       { PROPLIST_KEY_NAME, PROPLIST_VALUE_EXT },
       { PROPLIST_KEY_ATTRIBUTE, PROPLIST_VALUE_VR },
       { PROPLIST_KEY_ACTION, PROPLIST_VALUE_PRE_UNINIT },
    };

    return sendSimpleCommand(attributes, ARRAY_COUNTOF(attributes)); */
    return -1;
}
/* EOF */
