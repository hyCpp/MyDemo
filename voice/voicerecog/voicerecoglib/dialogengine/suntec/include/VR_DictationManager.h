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
 * @file VR_DictationManager.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_DICTATIONMANAGER_H
#define VR_DICTATIONMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */

#ifndef VC_COMMONIF_H
#    include "VC_CommonIF.h"
#endif

#include "VR_CurlHttpTool.h"
#include "VC_WavFileWriter.h"

/* Forward Declaration */
class VR_ControllerBase;

/**
 * @brief The VR_DictationManager class
 *
 * dialog controller class
 */
class VR_DictationManager
{
public:
    VR_DictationManager(VR_ControllerBase* controller);
    virtual ~VR_DictationManager();

    void Initialize();

    void UnInitialize();

    void DicTationData(const void* inputBuffer, unsigned long numSamples);

    void SaveMicToFile(const void* inputBuffer, unsigned long numSamples);

    void SetDictationValue(bool value, bool bSend = true);

    void SendDictationFileToWeb();

    std::string DictationRecognizeResult(const std::string &strSuccess, const std::string &strResult);

private:
    bool ParseJsonFile(const std::string& path);

private:
    // The main interaction manager that interfaces with the SDK.
    VR_ControllerBase*                                    m_controller;
    void*                                                 m_pVad;
    int                                                   m_vadCount; // 100ms cout  if speaker speak end more than 100ms, stop record
    int                                                   m_timeout;  // 10s cout  if speaker says more than 10s, stop record
    std::shared_ptr<VC_WavFileWriter>                     m_spWavWriter;
    bool                                                  m_bOpenDictation;
    std::string                                           m_sRequestUrl;
    CurlMap                                               m_requestHeader;
    CurlMap                                               m_requestUrlParam;
    bool                                                  m_beginRec;
    short*                                                m_buffer;
    int                                                   m_cursor;
};

#endif // VR_DICTATIONMANAGER_H
/* EOF */
