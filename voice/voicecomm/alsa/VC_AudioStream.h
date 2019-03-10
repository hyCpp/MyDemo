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
 * @file VC_AudioStream.h
 * @brief Declaration file of class VC_AudioStream.
 *
 * This file includes the declaration of class VC_AudioStream, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */
#ifndef VC_AUDIOSTREAM_H
#define VC_AUDIOSTREAM_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <mutex>
#include <pulse/context.h>

#include "pulsecore/config.h"
#include "pulsecore/atomic.h"
#include "pulsecore/log.h"
#include "pulsecore/proplist-util.h"
#include "pulsecore/sample-util.h"
#include "pulsecore/core-util.h"
#include "pulsecore/core-rtclock.h"
#include "VC_AudioDef.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct pa_simple;
    struct pa_stream;

#ifdef __cplusplus
}
#endif

/**
 * VC_AudioStream
 *
 * The class is just for audio streamming.
 */
class VC_AudioStream
{
public:
    VC_AudioStream();
    virtual ~VC_AudioStream();

    /**
    * @brief user config for audio stream parameters
    */
    struct VC_AudioStreamConfig
    {
        VC_AudioStreamConfig();

        EAUDIO_STREAM_FORMAT_TYPE       format;         ///< sample data format
        EAUDIO_STREAM_DIRECTION         direction;      ///< direction of audio stream
        EAUDIO_STREAM_DEVICE_TYPE       device;         ///< device to operate
        uint32_t                        rate;           ///< sampling rate
        uint32_t                        channels;       ///< audio channel type
        int32_t                         token;          ///< token id of client
        int32_t                         fade_in_time;   ///< fade-in time in ms
        int32_t                         fade_out_time;  ///< fade-out time in ms

        /**< Playback only: pre-buffering. The server does not start with
         * playback before at least prebuf bytes are available in the
         * buffer. It is recommended to set this to (uint32_t) -1, which
         * will initialize this to the same value as tlength, whatever
         * that may be. Initialize to 0 to enable manual start/stop
         * control of the stream. This means that playback will not stop
         * on underrun and playback will not start automatically. Instead
         * pa_stream_cork() needs to be called explicitly. If you set
         * this value to 0 you should also set PA_STREAM_START_CORKED.
         *
         * summary : music stream just set -1, if you are playing with a
         * tts or ringtone, you want a smaller latency, you can pass a smaller value, 0 is acceptable
         *
         * Forget about the long story, did you get that :
         * MUSIC JUST SET TO GOD DAMMED -1, TTS OR RINGTONE PLZ SET IT TO A SMALLER VALUE (CAN BE ZERO) !!
         *
         * */

        int32_t                         prebuf;         ///< pre-buffer time in ms (ASK ME IF YOU WANT TO CHANGE THIS)


        /**< Playback only: target length of the buffer. The server tries
         * to assure that at least tlength bytes are always available in
         * the per-stream server-side playback buffer. It is recommended
         * to set this to (uint32_t) -1, which will initialize this to a
         * value that is deemed sensible by the server. However, this
         * value will default to something like 2s, i.e. for applications
         * that have specific latency requirements this value should be
         * set to the maximum latency that the application can deal
         * with. When PA_STREAM_ADJUST_LATENCY is not set this value will
         * influence only the per-stream playback buffer size. When
         * PA_STREAM_ADJUST_LATENCY is set the overall latency of the sink
         * plus the playback buffer size is configured to this value. Set
         * PA_STREAM_ADJUST_LATENCY if you are interested in adjusting the
         * overall latency. Don't set it if you are interested in
         * configuring the server-side per-stream playback buffer
         * size. */

        int32_t                         latency;        ///< latency time in ms (ASK ME IF YOU WANT TO CHANGE THIS)
    };

    struct VC_AutoRefCount
    {
        explicit VC_AutoRefCount(pa_atomic_t *t);
        ~VC_AutoRefCount();

    private:
        pa_atomic_t* m_ref;
    };

    /**
     * create a new connection to the pulseaudio server
     *
     * @param  [IN] config : The config to create audio stream
     *
     * @return audiostream creation error code
     * @retval >= 0 : succeed to create a new connection
     * @retval < 0 : create connection failure, error code is stored
     * virtual is for voice lib
     */
    virtual int32_t create(const VC_AudioStreamConfig& config);

    /**
     * Read some data from the pulseaudio server.
     * This function blocks until bytes amount of data has been received from the server,
     * or until an error occurs. Returns a negative value on failure.
     *
     * @param  [OUT] data : A pointer to a buffer.
     * @param  [IN] bytes : The number of bytes to read.
     *
     * @return A postive value for recorded data size or A negative value on failure
     * @retval >= 0 : recorded data size in bytes
     * @retval < 0 : read operation failure, error code is stored
     */
    virtual int32_t read(void* data, uint32_t bytes);

    /**
     * Write some data to the pulseaudio server.
     * This function blocks until bytes amount of data has been sended to the server,
     * or until an error occurs. Returns a negative value on failure.
     *
     * @param  [IN] data : A pointer to a buffer.
     * @param  [IN] bytes : The number of bytes to write.
     *
     * @return A postive value for playbacked data size or A negative value on failure
     * @retval >= 0 : playbacked data size in bytes
     * @retval < 0 : write operation failure, error code is stored
     */
    virtual int32_t write(const void* data, uint32_t bytes);

    /**
     * Wait until all data already written is played by the pulseaudio.
     *
     * @param  [IN] sleep_ms: sleep ms for TTS, only effect MEU!!!
     *
     * @return drain operation error code
     * @retval >= 0 : drain operation successful
     * @retval < 0 : drain operation failure, error code is stored
     */
    virtual int32_t drain(int sleep_ms = 250);

    /**
     * Flush the playback or record buffer.
     * This discards any audio in the buffer.
     *
     *
     * @return flush operation error code
     * @retval >= 0 : flush operation successful
     * @retval < 0 : flush operation failure, error code is stored
     */
    virtual int32_t flush(int sleep_ms = 250);

    /**
     * Flush the playback or record buffer.
     *
     *
     * @return flush operation error code
     * @retval >= 0 : flush operation successful
     * @retval < 0 : flush operation failure, error code is stored
     */
    virtual int32_t flush_nointerrupt(int sleep_ms = 250);

    /**
     * interrupt the playback or record buffer.
     *
     *
     * @return interrupt operation error code
     * @retval >= 0 : flush operation successful
     * @retval < 0 : flush operation failure, error code is stored
     */
    virtual int32_t interrupt();

    /**
     * reset the playback or record buffer.
     *
     *
     * @return reset operation error code
     * @retval >= 0 : flush operation successful
     * @retval < 0 : flush operation failure, error code is stored
     */
    virtual int32_t reset();

    /**
     * Lose and free the connection to the server.
     *
     * The connection object becomes invalid when this is called.
     *
     */
    virtual int32_t destroy(void);

    /**
     * Get the latency of current stream
     *
     * @param  [IN] update_time_info : sync time info in this call, if update_time_info=false, will use a 10ms dealy value
     *
     * @return the latency in usec
     * @retval >= 0 : real time in usec.
     * @retval < 0  : error code
     */
    // virtual int64_t getLatency(bool update_time_info = true);

    /**
     * Get the buffer latency size of current stream
     *
     * @param  [IN] update_time_info : sync time info in this call, if update_time_info=false, will use a 10ms dealy value
     * @param  [IN] add_sink_buff : the return size will add the alsa buffer (ask the author if you do not know what dose it mean)
     *
     * @return the latency in bytes
     * @retval >= 0 : real size in bytes.
     * @retval < 0  : error code
     */
    // virtual int64_t getBufferLatencySize(bool update_time_info = true, bool add_sink_buff = true);

     /**
     * transform bytes to usec by stream config last set.
     *
     * @return the usec from byte size
     * @retval >= 0 : real time in usec
     * @retval < 0  : error code
     */
    virtual int64_t bytesToUsec(int64_t bytes) const;

     /**
     * transform usec to bytes by stream config last set.
     *
     * @return the usec from byte size
     * @retval >= 0 : real size in bytes
     * @retval < 0  : error code
     */
    virtual int64_t usecToBytes(int64_t usec) const;

     //=====================================================================
     //  Below is all the extended interface, currently only used by VR
     //=====================================================================

     /**
     *
     *   Extended Interface
     *
     * pre init current stream (not open audio device yet)
     *
     * @param  [IN] extType : type information in enum NMAUDIO_EXT_TYPE
     * @param  [IN] data : for vr it`s the barge-in plugin debug mode
     *
     * @retval : error code
     */
     virtual int32_t preInit(int extType, int data);

     /**
     *
     *   Extended Interface
     *
     *  Return data information
     *
     * @param  [IN] extType : type information in enum NMAUDIO_EXT_TYPE
     * @param  [INOUT] pSize : for vr it`s the barge-in data size
     * @param  [INOUT] pData : for vr it`s the barge-in data
     *
     * @retval : error code
     */
     virtual int32_t getData(int extType, unsigned *pSize, void *pData);

    /**
     *
     *   Extended Interface
     *
     * pre un-init current stream (BargeInPlugInRelease)
     *
     * @param  [IN] extType : type information in enum NMAUDIO_EXT_TYPE
     *
     * @retval : error code
     */
     virtual int32_t preUnInit(int extType);

private:
    int processErrorCode(const char* what, int error);
    int innerDestroy();
    int32_t innerFlush(int sleep_ms);
    pa_simple* pa_simple_new_extend2(uint32_t device, int dir,
                                     const char* stream_name,
                                     const pa_sample_spec *ss,
                                     const pa_channel_map *map,
                                     const pa_buffer_attr *attr,
                                     pa_proplist* plist);

    // void pa_simple_free_ext(pa_simple* s);
    int32_t get_drain_latency();

    VC_AudioStream(const VC_AudioStream&);
    VC_AudioStream& operator= (const VC_AudioStream&);

private:
    /// audio stream config
    VC_AudioStreamConfig     m_config;

    /// The connection object for each user
    pa_simple               *m_psimple;

    /// the flag of current work
    pa_atomic_t             *m_workFlag;

    /// last operation result
    int                     m_lastOpRst;

    /// lock
    std::mutex              m_mutex;
};

#endif /* NMAUDIOSTREAM_H */
/* EOF */
