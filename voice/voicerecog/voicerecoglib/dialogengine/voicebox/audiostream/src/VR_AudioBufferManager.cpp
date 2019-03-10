/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

/* Suntec Header */
#ifndef VR_AUDIOBUFFERMANAGER_H
#    include "VR_AudioBufferManager.h"
#endif

#ifndef VR_AUDIOBUFFER_H
#    include "VR_AudioBuffer.h"
#endif

#ifndef CXX_CL_WAITOBJ_H
#    include "CL_WaitObj.h"
#endif

#ifndef CXX_CL_SYNCOBJ_H
#    include "CL_SyncObj.h"
#endif

#ifndef CXX_CL_AUTOSYNC_H
#    include "CL_AutoSync.h"
#endif

#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

// Constructor
VR_AudioBufferManager::VR_AudioBufferManager(CL_WaitObj& waitObj, CL_SyncObj& syncObj)
: m_receiver(NULL)
, m_bBosDetecting(false)
, m_bBosDetected(false)
, m_bRecording(false)
, m_bEmpty(true)
, m_bEnd(false)
, m_waitObj(waitObj)
, m_syncObj(syncObj)
{
}

// Destructor
VR_AudioBufferManager::~VR_AudioBufferManager()
{
    m_receiver = NULL;
}

void
VR_AudioBufferManager::OnAudioInData(void* data, int len)
{
    VR_LOGD_FUNC();

    if ((len <= 0) || (NULL == data)) {
        m_bEnd = true;
        return;
    }

    if (m_bBosDetecting) {
        VR_LOGI("BOS detecting");
        char* buffer = VR_new char[len];
        if (NULL == buffer) {
            VR_LOGE("create buffer failed");
            return;
        }

        memcpy(buffer, data, len);
        PushRecordData(buffer, len);

        m_bEmpty = m_bufferList.empty();
        m_waitObj.Notify();
    }
    else if (m_bAudioCaching) {
        char* buffer = VR_new char[len];
        if (NULL == buffer) {
            VR_LOGE("create buffer failed");
            return;
        }

        memcpy(buffer, data, len);
        PushRecordData(buffer, len);
    }
    else {
        VR_LOGI("Recording for recognize");
        if (NULL == m_receiver) {
            VR_LOGE("Null Receiver");
            return;
        }

        if (m_bBosDetected) {
            VR_LOGI("Writing the buffers to VBT engine");
            while (!m_parsedBufferList.empty()) {
                if (!m_bRecording) {
                    break;
                }
                std::pair<char*, size_t> bufferIno = PopParsedData();
                if (NULL == bufferIno.first) {
                    break;
                }
                m_receiver->OnAudioInData(bufferIno.first, bufferIno.second);
                delete[] bufferIno.first;
            }
        }
        else {
            if (!m_bEmpty) {
                Clear();
            }
        }

        while (!m_bufferList.empty()) {
            if (!m_bRecording) {
                break;
            }
            std::pair<char*, size_t> bufferIno = PopRecordData();
            if (NULL == bufferIno.first) {
                break;
            }
            m_receiver->OnAudioInData(bufferIno.first, bufferIno.second);
            delete[] bufferIno.first;
        }

        if (m_bRecording) {
            VR_LOGI("Writing the audio data to VBT engine");
            m_receiver->OnAudioInData(data, len);
        }
    }
}

void
VR_AudioBufferManager::SetBosDetecting(bool detecting)
{
    VR_LOGD_FUNC();

    m_bBosDetecting = detecting;
    if (m_bBosDetecting) {
        m_bEnd = false;
    }
    else {
        m_bEnd = true;
        m_waitObj.Notify();
    }
}

void
VR_AudioBufferManager::SetBosDetected(bool detected)
{
    VR_LOGD_FUNC();

    m_bBosDetected = detected;
}

bool
VR_AudioBufferManager::IsAvailable()
{
    VR_LOGD_FUNC();

    CL_AutoSync autoSync(m_syncObj);
    bool available = !m_bufferList.empty();

    return available;
}

int
VR_AudioBufferManager::FetchData(void** ppBuffer, size_t* size)
{
    VR_LOGD_FUNC();

    m_waitObj.Reset();
    if (m_bEmpty) {
        VR_LOGI("Empty flag: %d, End flag: %d", m_bEmpty, m_bEnd);
        while (m_bEmpty && !m_bEnd) {
            const int FETCHDATA_WAIT_TIME = 20;
            m_waitObj.Wait(FETCHDATA_WAIT_TIME);
        }

        if (m_bEmpty) {
            *ppBuffer = NULL;
            *size = 0;
            return 0;
        }
    }

    if (m_bBosDetected) {
        *ppBuffer = NULL;
        *size = 0;
        return 0;
    }

    std::pair<char*, size_t> bufferIno = PopRecordData();
    m_bEmpty = m_bufferList.empty();
    *ppBuffer = bufferIno.first;
    *size = bufferIno.second;
    if (NULL != bufferIno.first) {
        PushParsedData(bufferIno.first, bufferIno.second);
    }

    VR_LOGI("Get buffer size: %d", *size);

    return 0;
}

int
VR_AudioBufferManager::ReturnData(void* const& rpBuffer)
{
    VR_LOGD_FUNC();

    const int BUFFER_MAX_LEN = 10;
    while (m_parsedBufferList.size() > BUFFER_MAX_LEN) {
        std::pair<char*, size_t> bufferIno = PopParsedData();
        delete[] bufferIno.first;
    }

    return 0;
}

void
VR_AudioBufferManager::Clear()
{
    VR_LOGD_FUNC();

    while (!m_bufferList.empty()) {
        std::pair<char*, size_t> bufferIno = PopRecordData();
        delete[] bufferIno.first;
    }

    while (!m_parsedBufferList.empty()) {
        std::pair<char*, size_t> bufferIno = PopParsedData();
        delete[] bufferIno.first;
    }

    m_bEmpty = true;
}

void
VR_AudioBufferManager::SetReceiver(VR_AudioBuffer* receiver)
{
    VR_LOGD_FUNC();

    m_receiver = receiver;
}

void
VR_AudioBufferManager::SetRecordingFlag(bool bRecording)
{
    VR_LOGD_FUNC();

    m_bRecording = bRecording;
}

void
VR_AudioBufferManager::SetAudioCachingFlag(bool bCaching)
{
    m_bAudioCaching = bCaching;
}

bool
VR_AudioBufferManager::PushRecordData(char* buffer, size_t len)
{
    CL_AutoSync autoSync(m_syncObj);
    m_bufferList.push_back(
        std::pair<char*, size_t>(buffer, len)
    );

    return true;
}

std::pair<char*, size_t>
VR_AudioBufferManager::PopRecordData()
{
    CL_AutoSync autoSync(m_syncObj);
    char* pBuffer = NULL;
    size_t len = 0;
    if (!m_bufferList.empty()) {
        pBuffer = m_bufferList.front().first;
        len = m_bufferList.front().second;
        m_bufferList.pop_front();
    }

    return std::pair<char*, size_t>(pBuffer, len);
}

bool
VR_AudioBufferManager::PushParsedData(char* buffer, size_t len)
{
    CL_AutoSync autoSync(m_syncObj);
    m_parsedBufferList.push_back(
        std::pair<char*, size_t>(buffer, len)
    );

    return true;
}

std::pair<char*, size_t>
VR_AudioBufferManager::PopParsedData()
{
    CL_AutoSync autoSync(m_syncObj);
    char* pBuffer = NULL;
    size_t len = 0;
    if (!m_parsedBufferList.empty()) {
        pBuffer = m_parsedBufferList.front().first;
        len = m_parsedBufferList.front().second;
        m_parsedBufferList.pop_front();
    }

    return std::pair<char*, size_t>(pBuffer, len);
}

/* EOF */