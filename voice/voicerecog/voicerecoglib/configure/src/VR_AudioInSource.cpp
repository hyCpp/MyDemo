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
#include <sstream>
#include <time.h>
#include "stdafx.h"
#include "CL_AutoSync.h"
#include "VR_AudioInSource.h"
#include "VR_ConfigureIF.h"
#include "BL_Dir.h"
#include "BL_SystemTime.h"

using namespace N_Vr::N_Asr;

VR_AudioInSource::~VR_AudioInSource()
{
    VR_LOGD_FUNC();
    UnPrepare();
    if (NULL != m_pAecData) {
        delete m_pAecData;
        m_pAecData = NULL;
    }
    if (NULL != m_pAudioIn) {
        delete m_pAudioIn;
        m_pAudioIn = NULL;
    }
}

VR_AudioInSource::VR_AudioInSource()
    : m_nAudioMode(VC_AUDIO_MODE_NORMAL)
    , m_Offset(10)
    , m_bStarted(false)
    , m_hasMoreData(false)
    , m_buffers()
    , m_pAudioIn(NULL)
    , m_waitObj()
    , m_syncObj()

    , m_nFileNum(0)
    , m_bOutFolderCreated(false)
    , m_bInputWavOption(false)
    , m_pAecData(NULL)
    , m_nAecDataSize(0)
    , m_bFirstFrame(false)
    , m_pListener(NULL)
    , m_iIgnoreLength(0)
{
    VR_LOGD_FUNC();
}

int VR_AudioInSource::CreateOutFolder()
{
    char buffer[100] = { 0 };

    BL_SystemTime cTime;
    BL_TimeSystem currentTime;
    memset(&currentTime, 0x00, sizeof(currentTime));
    if (BL_NOERROR == cTime.GetSystemTime(&currentTime)) {
        snprintf(buffer, sizeof(buffer), "%02d%02d%02d%02d%02d",
            currentTime.Time.wMonth, currentTime.Time.wDay,
            currentTime.Time.wHour, currentTime.Time.wMinute,
            currentTime.Time.wSecond);
    }
    else {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        snprintf(buffer, sizeof(buffer), "%04d%02d%03d",
            (int)(ts.tv_sec/60), (int)(ts.tv_sec%60), (int)(ts.tv_nsec/1000000));
    }

    std::string s_time(buffer);
    VR_LOGI("now time is : %s", s_time.c_str());

    BL_Dir recordDir(BL_FILE_PREFIX_RW);
    recordDir.MakeDir((std::string("VR/SUNTEC/") + std::string("Records/")).c_str());
    recordDir.MakeDir((std::string("VR/SUNTEC/Records/") + s_time).c_str());
    m_recordFilePath = VR_ConfigureIF::Instance()->getUsrPath() + "Records/" + s_time + "/";
    return 0;
}

void VR_AudioInSource::M_Fn_Get_Data_Type_Audio_SSE(void ** ppvoid_Data_Type_Audio_SSE, size_t * pi_Size_Data_Type_Audio_SSE)
{
    VR_LOGD_FUNC();

    if ((m_nAecDataSize <= 0) || (NULL == m_pAecData)) {
        VR_LOGD("(m_nAecDataSize <= 0) || (NULL == m_pAecData)");
        if (pi_Size_Data_Type_Audio_SSE) {
            *pi_Size_Data_Type_Audio_SSE = 0;
        }
        return;
    }

    if (pi_Size_Data_Type_Audio_SSE) {
        *pi_Size_Data_Type_Audio_SSE = m_nAecDataSize;
    }

    if (ppvoid_Data_Type_Audio_SSE) {
        *ppvoid_Data_Type_Audio_SSE = m_pAecData;
    }
}

// Dialog engine thread will call
int VR_AudioInSource::Prepare()
{
    VR_LOGD_FUNC();
    Clear();
    m_waitObj.Reset();

    bool outputWavOption = VR_ConfigureIF::Instance()->getOutputWavOption();
    if (outputWavOption) {
        // Create record wav folder
        if (!m_bOutFolderCreated) {
            m_bOutFolderCreated = true;
            CreateOutFolder();
        }

        m_spWavWriter.reset(new(MEM_Voice) VC_WavFileWriter);
        m_spWavFetchWriter.reset(new(MEM_Voice) VC_WavFileWriter);

        std::ostringstream oss;
        oss << m_recordFilePath << ++m_nFileNum << ".wav";
        m_spWavWriter->Open(oss.str());
        oss.str("");
        oss << m_recordFilePath << m_nFileNum << ".fetch.wav";
        m_spWavFetchWriter->Open(oss.str());
    }

    INT sampleRate = RATE;
    INT fragSize = BUF_SIZE;
    INT fragCount = BUF_COUNT;

    bool inputWavOption = VR_ConfigureIF::Instance()->getInputWavOption();
    if (NULL != m_pAudioIn && m_bInputWavOption != inputWavOption) {
        delete m_pAudioIn;
        m_pAudioIn = NULL;
    }

    if (NULL == m_pAudioIn) {
        m_pAudioIn = VC_CommonIF::Instance()->CreateAudioIn(inputWavOption);
        m_bInputWavOption = inputWavOption;
    }


    if (!m_pAudioIn->Open(&sampleRate, &fragSize, &fragCount, m_nAudioMode)) {
        VR_LOGE("AudioIn open failed, mode=%d", m_nAudioMode);
        return 1;
    }
    VR_LOGP("AudioIn open");

    m_bFirstFrame = true;

    if (!m_pAudioIn->Start(this)) {
        VR_LOGE("AudioIn start failed!");
        return 1;
    }
    VR_LOGP("AudioIn start");

    m_bStarted = true;
    m_hasMoreData = true;
    m_iIgnoreLength = 0;

    VR_LOGP("AudioIn start OK,mode=%d", m_nAudioMode);
    return 0;
}

int VR_AudioInSource::M_Fn_Start()
{
    VR_LOGD_FUNC();
    if (!m_bStarted) {
        VR_LOGE("AudioIn start failed!");
    }
    return 0;
}

int VR_AudioInSource::UnPrepare()
{
    VR_LOGD_FUNC();
    if (m_bStarted) {
        VR_LOGP("AudioIn stop ...");
        m_pAudioIn->Stop(false);
        VR_LOGP("AudioIn close ...");
        m_pAudioIn->Close();
        VR_LOGP("AudioIn close done");
        m_bStarted = false;
        m_hasMoreData = false;
    }
    // clear the m_buffer
    Clear();
    return 0;
}

int VR_AudioInSource::M_Fn_Stop()
{
    VR_LOGD_FUNC();
    UnPrepare();
    return 0;
}

int VR_AudioInSource::M_Fn_Data_Available_Check(bool * pb_Data_Available)
{
    CL_AutoSync OAutoSync(m_syncObj);
    *pb_Data_Available = !m_buffers.empty();
    return 0;
}

int VR_AudioInSource::M_Fn_Data_Fetch(void ** ppvoid_Buffer, size_t * pi_Size)
{
    VR_LOGD_FUNC();
    m_waitObj.Reset();
    if (NULL == ppvoid_Buffer || NULL == pi_Size) {
        VR_LOGE("Invalid fetch parameters");
        return -1;
    }

    do {
        while (m_buffers.empty() && m_hasMoreData) {
            m_waitObj.Wait(20);
        }
        if (!m_hasMoreData) {
            VR_LOGI("Fetch Audio, but no more data");
            *ppvoid_Buffer = NULL;
            *pi_Size = 0;
            return 0;
        }
        {
            VR_LOGD("CL_AutoSync ...");
            CL_AutoSync sync(m_syncObj);
            if (m_buffers.empty()) {
                continue;
            }

            *ppvoid_Buffer = m_buffers.front().first;
            *pi_Size = m_buffers.front().second;
            VR_LOGD("FetchData Audio Size: %d, address[%p]", *pi_Size, *ppvoid_Buffer);
            m_buffers.pop_front();
        }

        if (m_spWavFetchWriter) {
                BYTE* pby = static_cast<BYTE*>(*ppvoid_Buffer);
                INT len = static_cast<INT>(*pi_Size);
                m_spWavFetchWriter->Write(pby, len);
        }
        break;
     } while (true);
    return 0;
}

int VR_AudioInSource::M_Fn_Data_Return(void * const & rpvoid_Buffer)
{
    VR_LOGD_FUNC();
    BYTE * pby = const_cast<BYTE * >(static_cast<BYTE * const>(rpvoid_Buffer));
    delete[] pby;
    return 0;
}

// audioIn thread callback
VOID VR_AudioInSource::OnAudioInData(VOID * pData, INT len)
{
    VR_LOGD_FUNC();
    if (m_bFirstFrame) {
        m_bFirstFrame = false;
        VR_LOGP("AudioIn callback, first frame data");
    }

    if (NULL != pData && len > 0) {
        // Dump PCM to wav file
        if (m_spWavWriter) {
            m_spWavWriter->Write(pData, len);
        }
        {
            CL_AutoSync sync(m_syncObj);
            if (m_iIgnoreLength > 0) {
                VR_LOGD("Ignore this AudioIn length[%d], total ignore length[%d]", len, m_iIgnoreLength);
                m_iIgnoreLength -= len;
                return;
            }

            BYTE * pby = new(MEM_Voice) BYTE[len];
            if (pby) {
                memcpy(pby, pData, len);
                m_buffers.push_back(std::pair<BYTE *, size_t>(pby, static_cast<size_t>(len)));
            }
            else {
                VR_LOGE("AudioInData callback no memory!");
                m_hasMoreData = false;
            }
        }
    }
    else { // -1 == len
        VR_LOGE("AudioInData error: DataLen < 0!");
        m_hasMoreData = false;
    }
    m_waitObj.Notify();
}

VOID VR_AudioInSource::OnAudioInStarted()
{
    VR_LOGD_FUNC();

    if (m_pListener) {
        VR_LOGI("OnAudioInStartedAction.");
        m_pListener->OnAudioInStartedAction();
    }
}

VOID VR_AudioInSource::OnAudioInStopped()
{
    if (m_spWavWriter) {
        m_spWavWriter->Close(); // here maybe lost some data if audio stop no sync.
        m_spWavWriter.reset();
    }
    if (m_spWavFetchWriter) {
        m_spWavFetchWriter->Close();
        m_spWavFetchWriter.reset();
    }
    m_hasMoreData = false;
}

VOID VR_AudioInSource::OnAudioInCustom(int type, VOID* data, INT len)
{
    VR_LOGD_FUNC();

    if ((len <= 0) || (NULL == data)) {
        m_nAecDataSize = 0;
        return;
    }

    char* pData = new(MEM_Voice) char[len];
    if (NULL == pData) {
        m_nAecDataSize = 0;
        return;
    }

    if (NULL != m_pAecData) {
        delete m_pAecData;
        m_pAecData = NULL;
    }

    m_pAecData = pData;
    memcpy(m_pAecData, (char*)data, len);
    m_nAecDataSize = len;
}

void VR_AudioInSource::setAudioMode(int mode, Listener* pListener)
{
    m_nAudioMode = mode;

    m_pListener = pListener;
}

void VR_AudioInSource::Clear()
{
    VR_LOGD_FUNC();
    int length = 0;
    {
        CL_AutoSync OAutoSync(m_syncObj);
        while (!m_buffers.empty()) {
            delete[] m_buffers.front().first;
            length += m_buffers.front().second;
            m_buffers.pop_front();
        }
    }

    if (m_iIgnoreLength > 0) {
        m_iIgnoreLength -= length;
    }
}

// Dialog engine thread will call
void VR_AudioInSource::IgnoreAudioInByTime(int milliseconds)
{
    VR_LOGD_FUNC();
    CL_AutoSync OAutoSync(m_syncObj);
    m_iIgnoreLength = 2 * RATE * milliseconds / 1000;
    Clear();
}

/* EOF */
