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

/**
 * @file VR_VoiceBoxAudioInListener.h
 * @brief interface for VR_VoiceBoxAudioInListener.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_VOICEBOXAUDIOINLISTENER_H
#define VR_VOICEBOXAUDIOINLISTENER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Suntec Header */
#ifndef VC_COMMONIF_H
#    include "VC_CommonIF.h"
#endif

#ifndef VC_AUDIOIN_H
#    include "VC_AudioIn.h"
#endif

#include "VC_WavFileWriter.h"

/* Forward Declaration */
class VR_AudioBufferManager;

/**
 * @brief The VR_VoiceBoxAudioInListener class
 *
 * class declaration
 */
class VR_VoiceBoxAudioInListener : public VC_AudioIn::Listener
{
public:
    VR_VoiceBoxAudioInListener(
        VR_AudioBufferManager& bufferManager
    );
    virtual ~VR_VoiceBoxAudioInListener();
    virtual VOID OnAudioInData(VOID* data, INT len);
    virtual VOID OnAudioInCustom(int type, VOID* data, INT len);
    virtual VOID OnAudioInStarted();
    virtual VOID OnAudioInStopped();

    // Get AEC audio type data
    unsigned int GetAecAudioTypeDataSize();
    bool GetAecAudioTypeData(unsigned int& size, void* data);

    void SetVoiceTagFlag(bool bVoiceTag);

    void SetVoiceTagTimer(const std::string& strTimer);

private:
    int  CreateOutFolder();
    bool IsVoiceTag() const { return m_bVoiceTag; }
    FILE* Open(const std::string& tempvoiceTagPath);
    void Write(void *pData, int iLen, FILE* pFile);
    void WriteVoiceTagFile(FILE* pFile);

    VR_AudioBufferManager& m_bufferManager;
    char*                  m_pAecData;
    unsigned int           m_nAecDataSize;
    boost::shared_ptr<VC_WavFileWriter> m_spWavWriter;

    int                    m_nFileNum;
    std::string            m_recordFilePath;
    bool                   m_bVoiceTag;
    bool                   m_bOutFolderCreated;
    FILE                   *m_pFile;
    long                   m_lVoiceTagTimer;
};

#endif // VR_VOICEBOXAUDIOINLISTENER_H
/* EOF */
