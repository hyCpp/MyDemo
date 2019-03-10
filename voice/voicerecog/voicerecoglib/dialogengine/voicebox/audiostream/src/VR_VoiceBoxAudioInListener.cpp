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
#ifndef VR_VOICEBOXAUDIOINLISTENER_H
#    include "VR_VoiceBoxAudioInListener.h"
#endif

#ifndef VR_AUDIOBUFFERMANAGER_H
#    include "VR_AudioBufferManager.h"
#endif

#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#include <sstream>
#include "VR_ConfigureIF.h"
#include "BL_Dir.h"
#include "VC_WavFileWriter.h"
#include "BL_SystemTime.h"

static const int VR_VOICETAG_BUFFER_LEN = 1280;
static const int VR_VOICETAG_SAMPLE_RATE = 16000;
static const int VR_VOICETAG_PER_SAMPLE = 16;
static const int VR_VOICETAG_CHANNELS = 1;
static const int VR_VOICETAG_MSEC_TRANSFER_SEC = 1000;
static const int VR_VOICETAG_BYTE_LEN = 8;

#define VR_VOICETAG_FILE    "voiceTag.wav"
#define VR_TEMP_VOICETAG_FILE    "tempVoiceTag.wav"

// Constructor
VR_VoiceBoxAudioInListener::VR_VoiceBoxAudioInListener(
    VR_AudioBufferManager& bufferManager
)
: m_bufferManager(bufferManager)
, m_pAecData(NULL)
, m_nAecDataSize(0)
, m_nFileNum(0)
, m_bVoiceTag(false)
, m_bOutFolderCreated(false)
, m_pFile(NULL)
, m_lVoiceTagTimer(0)
{
}

// Destructor
VR_VoiceBoxAudioInListener::~VR_VoiceBoxAudioInListener()
{
    delete m_pAecData;
    m_pAecData = NULL;
}

int VR_VoiceBoxAudioInListener::CreateOutFolder()
{
    char buffer[100] = { 0 };

    BL_SystemTime cTime;
    BL_TimeSystem currentTime = { { 0, 0, 0, 0, 0, 0, 0, 0 }, 0 };
    if (BL_NOERROR == cTime.GetSystemTime(&currentTime)) {
        snprintf(buffer, sizeof(buffer), "%02d%02d%02d%02d%02d",
            currentTime.Time.wMonth, currentTime.Time.wDay,
            currentTime.Time.wHour, currentTime.Time.wMinute,
            currentTime.Time.wSecond);
    }
    else {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        snprintf(buffer, sizeof(buffer), "%02d%02d%09d",
            (int)(ts.tv_sec/60), (int)(ts.tv_sec%60), (int)(ts.tv_nsec));
    }

    std::string s_time(buffer);
    VR_LOGI("System time is : %s", s_time.c_str());

    BL_Dir recordDir(BL_FILE_PREFIX_RW);
    recordDir.MakeDir((std::string("VR/VBT/") + std::string("Records/")).c_str());
    recordDir.MakeDir((std::string("VR/VBT/Records/") + s_time).c_str());
    m_recordFilePath = VR_ConfigureIF::Instance()->getUsrPath() + "Records/" + s_time + "/";
    return 0;
}

VOID VR_VoiceBoxAudioInListener::OnAudioInData(VOID* data, INT len)
{
    VR_LOGD_FUNC();

    if (IsVoiceTag() && (NULL != m_pFile)) {
        VR_LOGI("writing voicetag file");
        Write(data, len, m_pFile);
    }

    if (m_spWavWriter) {
        VR_LOGI("write not voicetag file");
        m_spWavWriter->Write(data, len);
    }

    m_bufferManager.OnAudioInData(data, len);
}

VOID VR_VoiceBoxAudioInListener::OnAudioInCustom(int type, VOID* data, INT len)
{
    VR_LOGD_FUNC();

    if ((len <= 0) || (NULL == data)) {
        m_nAecDataSize = 0;
        return;
    }

    char* pData = VR_new char[len];
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

VOID VR_VoiceBoxAudioInListener::OnAudioInStarted()
{
    VR_LOGD_FUNC();

    if (IsVoiceTag()) {
        VR_LOGI("create voicetag file");
        if (NULL != m_pFile) {
            VR_LOGI("previous m_pFile is not close");
            fclose(m_pFile);
            m_pFile = NULL;
        }
        m_pFile = Open(std::string(VR_TEMP_VOICETAG_FILE));
    }

    bool outputWav = VR_ConfigureIF::Instance()->getOutputWavOption();
    if (outputWav) {
        if (!m_bOutFolderCreated) {
            CreateOutFolder();
            m_bOutFolderCreated = true;
        }

        std::stringstream ss;
        ss << m_recordFilePath << ++m_nFileNum << ".wav";

        VR_LOGD("wav file path is :%s", ss.str().c_str());
        m_spWavWriter.reset(VR_new VC_WavFileWriter());
        m_spWavWriter->Open(ss.str());
    }

}

VOID VR_VoiceBoxAudioInListener::OnAudioInStopped()
{
    VR_LOGD_FUNC();

    if (IsVoiceTag() && (NULL != m_pFile)) {
        VR_LOGI("voicetag");
        WriteVoiceTagFile(m_pFile);
        fclose(m_pFile);
        m_pFile = NULL;
    }

    if (m_spWavWriter) {
        m_spWavWriter->Close(); // here maybe lost some data if audio stop no sync.
        m_spWavWriter.reset();
    }

    if (VR_ConfigureIF::Instance()->isFileExist(VR_TEMP_VOICETAG_FILE)) {
        bool removeOK = VR_ConfigureIF::Instance()->removeFile(VR_TEMP_VOICETAG_FILE);
        VR_LOGI("removeOK : %d", removeOK);
    }
    m_lVoiceTagTimer = 0;
    VR_LOGI("OnAudioInStopped");
}

unsigned int
VR_VoiceBoxAudioInListener::GetAecAudioTypeDataSize()
{
    return m_nAecDataSize;
}

bool
VR_VoiceBoxAudioInListener::GetAecAudioTypeData(unsigned int& size, void* data)
{
    VR_LOGD_FUNC();

    if ((m_nAecDataSize <= 0) || (NULL == m_pAecData)) {
        return false;
    }

    size = m_nAecDataSize;
    if (NULL == data) {
        return true;
    }

    memcpy((char*) data, m_pAecData, size);
    return true;
}

void
VR_VoiceBoxAudioInListener::SetVoiceTagFlag(bool bVoiceTag)
{
    VR_LOGD_FUNC();

    m_bVoiceTag = bVoiceTag;
}

void
VR_VoiceBoxAudioInListener::SetVoiceTagTimer(const std::string& strTimer)
{
    VR_LOGD_FUNC();

    VR_LOGI("strTimer : %s", strTimer.c_str());
    int iTimer = std::atoi(strTimer.c_str());
    m_lVoiceTagTimer = (VR_VOICETAG_SAMPLE_RATE * VR_VOICETAG_PER_SAMPLE * VR_VOICETAG_CHANNELS * iTimer)
                       / (VR_VOICETAG_MSEC_TRANSFER_SEC * VR_VOICETAG_BYTE_LEN);
    VR_LOGI("m_lVoiceTagTimer : %ld", m_lVoiceTagTimer);
}

FILE*
VR_VoiceBoxAudioInListener::Open(const std::string& tempvoiceTagPath)
{
    VR_LOGD_FUNC();

    if (VR_ConfigureIF::Instance()->isFileExist(tempvoiceTagPath.c_str())) {
        bool removeOK = VR_ConfigureIF::Instance()->removeFile(tempvoiceTagPath.c_str());
        VR_LOGI("removeOK : %d", removeOK);
    }

    std::string strVoiceTagPath = VR_ConfigureIF::Instance()->getUsrPath() + tempvoiceTagPath;
    FILE* pFile = fopen(strVoiceTagPath.c_str(), "wb+");
    if (NULL == pFile) {
        VC_LOGE("open file[%s] failed!", strVoiceTagPath.c_str());
        return NULL;
    }

    return pFile;
}

void
VR_VoiceBoxAudioInListener::Write(void *pData, int iLen, FILE* pFile)
{
    if ((NULL == pFile) || (NULL == pData) || (iLen < 1)) {
        VC_LOGI("the len is lt 0 or buffer is not existe!");
        return;
    }

    int write_len = fwrite(pData, 1, iLen, pFile);
    int count = 0;
    while ((write_len < iLen) && (count < 3)) {
        VR_LOGI("write error and contine write");
        iLen -= write_len;
        write_len = fwrite(pData, 1, iLen, pFile);
        ++count;
    }
}

void
VR_VoiceBoxAudioInListener::WriteVoiceTagFile(FILE* pFile)
{
    VR_LOGD_FUNC();
    if (!IsVoiceTag() || (0 == m_lVoiceTagTimer) || (NULL == pFile)) {
        VR_LOGI("not need to write voicetag file");
        return;
    }
        // set file point to out locatio
    if (0 != fseek(pFile, m_lVoiceTagTimer, SEEK_SET)) {
        VC_LOGI("the offset set failed");
        return;
    }

    VC_WavFileWriter* spWavWriter = VR_new VC_WavFileWriter();
    if (NULL == spWavWriter) {
        VR_LOGI("create spWavWriter failed");
        return;
    }

    char buf[VR_VOICETAG_BUFFER_LEN] = { 0 };
    std::string newVoiceTagPath = VR_ConfigureIF::Instance()->getUsrPath() + VR_VOICETAG_FILE;
    spWavWriter->Open(newVoiceTagPath);

    size_t readLen = fread(buf, 1, VR_VOICETAG_BUFFER_LEN, pFile);
    while (readLen > 0) {
        spWavWriter->Write(buf, readLen);
        readLen = fread(buf, 1, VR_VOICETAG_BUFFER_LEN, pFile);
    }

    spWavWriter->Close();
    delete spWavWriter;
    spWavWriter = NULL;
}
/* EOF */
