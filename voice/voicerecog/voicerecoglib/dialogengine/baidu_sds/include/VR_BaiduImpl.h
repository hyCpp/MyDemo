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
 * @file VR_BaiduImpl.h
 * @brief dialog engine class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef VR_BaiduIMPL_H
#define VR_BaiduIMPL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/* Standard Header */
#include <string>
#include <memory>

#include "observerInterface/SpeechCommandObserverInterface.h"
#include "SpeechCommandObserver.h"
#include "SphinxSpeechDetector.h"

#ifndef __BAIDUSDK_SPEECH_H__
    #include "speech.h"
#endif

#ifndef __VR_ASRENGINEIF_H
    #include "VR_AsrEngineIF.h"
#endif

#include "BL_Timer.h"


/**
 * @brief The VR_RecogTimer class
 *
 * class declaration
 */
class  VR_RecogTimer : public BL_Timer
{
public:
    VR_RecogTimer(VR_AsrEngineIF* handler);
    virtual ~VR_RecogTimer();
    virtual VOID OnTimer();
private:
    VR_AsrEngineIF* m_handler;
};

using namespace SphinxS;

/**
 * @brief The VR_BaiduImpl class
 *
 * dialog controller class
 */
class VR_BaiduImpl:public VR_AsrEngineIF
{
public:
    VR_BaiduImpl();
    virtual ~VR_BaiduImpl();

    virtual bool Initialize(VR_ControllerBase* controller);
    virtual bool Start();
    virtual void Stop();
    virtual void UnInitialize();

    inline bool PostMessage(const std::string& message, int actionId = -1);

    /**
     * ProcessMessage
     *
     * Process Voice Recognition related messages
     *
     * @param none
     * @return none
     * @attention none
     * @see none
     */
    virtual void ProcessMessage(const std::string& message);

    virtual int onAudioInData(int16_t* buffer, int32_t size);

    void startRecordMicData();

    void stopRecordMicData();

    void postRequest(const std::string pcm);

private:
    // The main interaction manager that interfaces with the SDK.
    baiduSDK::Speech m_client;
    VR_ControllerBase*                     m_controller;
    std::shared_ptr<alexaClientSDK::avsCommon::sdkInterfaces::SpeechCommandObserverInterface> m_speechcommandobserver;
    std::unique_ptr<SphinxSpeechDetector> m_speechdetector;

    std::shared_ptr<VR_RecogTimer> m_timer;
};

#endif
/* EOF */
