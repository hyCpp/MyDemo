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

#include "stdafx.h"
#include "VC_IAudioInMedia.h"
#include "VC_AudioSession.h"
#include "VC_Factory.h"
#include "CL_WaitObj.h"

#ifdef      HAVE_NUTSHELL_OS
using namespace nutshell;
#endif


static const int SAMPLE_RATE = 16000; // sample rate 16K
static const int VC_RETRY_COUNT = 10;
const int VC_WAIT_DATA_TIME = 100; // ms

VC_IAudioInMedia::VC_IAudioInMedia()
: m_pStream(new(MEM_Voice) AUDIO_STREAM())
, m_pConfig(new(MEM_Voice) AUDIO_STREAM_CONFIG())
, m_state(ST_NONE), m_pBargeInData(NULL), m_bargeInDataSize(0)
{
}

VC_IAudioInMedia::~VC_IAudioInMedia()
{
    if (NULL != m_pStream) {
        delete m_pStream;
        m_pStream = NULL;
    }

    if (NULL != m_pConfig) {
        delete m_pConfig;
        m_pConfig = NULL;
    }

    if (NULL != m_pBargeInData) {
        delete[] m_pBargeInData;
        m_pBargeInData = NULL;
    }
}

bool VC_IAudioInMedia::Open(int sampleRate, int mode)
{
    VC_LOGD_FUNC();

    if (!m_pStream || !m_pConfig) {
        VC_LOGE("stream or config is null!, stream=%p, config=%p", m_pStream, m_pConfig);
        RETURN(false);
    }

    if (VC_AUDIO_MODE_NORMAL != mode) {
        int err = m_pStream->preInit(NMAUDIO_EXT_VR, (VC_AUDIO_MODE_BARGEIN_WITH_DEBUG == mode) ? 1 : 0);
        if (0 != err) {
            VC_LOGE("preInit failed, err:%d", err);
            RETURN(false);
        }

        m_bargeInDataSize = 0;
        err = m_pStream->getData(NMAUDIO_EXT_VR, &m_bargeInDataSize, NULL);
        if (0 != err) {
            VC_LOGE("getData failed, err:%d", err);
            RETURN(false);
        }

        VC_LOGI("bargeIn data size= %d", m_bargeInDataSize);
        m_pBargeInData = new(MEM_Voice) char[m_bargeInDataSize];
        err = m_pStream->getData(NMAUDIO_EXT_VR, &m_bargeInDataSize, m_pBargeInData);
        if (0 != err) {
            VC_LOGE("getData failed, err:%d", err);
            RETURN(false);
        }
    }


    // strncpy(m_pConfig->name, "tts", sizeof("tts"));        // name not be used
    m_pConfig->rate = sampleRate;
    m_pConfig->channels = 1;        // only support mono channel
    m_pConfig->format = EAUDIO_STREAM_FORMAT_TYPE_S16LE;
    m_pConfig->direction = EAUDIO_STREAM_DIRECTION_RECORD;
    m_pConfig->device = (VC_AUDIO_MODE_NORMAL != mode) ? EAUDIO_STREAM_DEVICE_VR_IN : EAUDIO_STREAM_DEVICE_INPUT_MIC;
#ifdef  HAVE_NUTSHELL_OS
    m_pConfig->echocancel = NMAUDIO_ChannelId_VoiceRecognition;
#endif
    // m_pConfig->tokenId = 0;    // tokenId not be used
    VC_LOGD("Open sample rate:%d", sampleRate);
    int err = m_pStream->create(*m_pConfig);
    if (err < 0) {
        VC_LOGE("NMAudioStream create failed, err:%d, sample rate:%d, device:%d", err, sampleRate, m_pConfig->device);
        RETURN(false);
    }
    m_state = ST_OPENED;
    VC_LOGD("AudioInMedia::Open ok");
    return true;
}


bool VC_IAudioInMedia::GetParam(int type, void **buf, int* size)
{
    *buf = m_pBargeInData;
    *size = m_bargeInDataSize;
    return (m_bargeInDataSize > 0);
}


int VC_IAudioInMedia::Read(void *buf, int size)
{
    if (ST_OPENED != m_state) {
        VC_LOGE("AudioIn read invalid m_state=%d", m_state);
        return -1;
    }

    unsigned char *buffer = static_cast<unsigned char*>(buf);
    int ret = m_pStream->read(buffer, size);
    return ret;
}

bool VC_IAudioInMedia::Stop()
{
    return true;
}

bool VC_IAudioInMedia::Close()
{
    VC_LOGD_FUNC();
    if (m_pStream) {
        m_pStream->destroy();
        if (NULL != m_pBargeInData) {  // barge-in enable
            m_pStream->preUnInit(NMAUDIO_EXT_VR);

            delete[] m_pBargeInData;
            m_pBargeInData = NULL;
            m_bargeInDataSize = 0;
        }
    }

    m_state = ST_NONE;
    return true;
}
/* EOF */
