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

// #include "SphinxSpeechDetector.h"
// // #include "KeyWordMultiObserver.h"
// #include "SpeechCommandObserver.h"

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_LOG_H
#    include "VR_Log.h"
#endif

#ifndef VR_BAIDUIMPL_H
#    include "VR_BaiduImpl.h"
#endif

#ifndef VR_SPEECHCOMMANDOBSERVER_H
#    include "VR_SpeechCommandObserver.h"
#endif

#include "VR_ControllerBase.h"

using namespace std;

static const std::string strAppid = "10602062";
static const std::string strApikey = "IcUVEVh2qDbmQAP8SHsRBGva";
static const std::string strSecretkey = "3924da26995a3565cdc6966d168e3fc9";



VR_RecogTimer::VR_RecogTimer(VR_AsrEngineIF* handler) : BL_Timer(15000, CL_FALSE)
{
    VR_LOGD_FUNC();
    m_handler = handler;
}

VR_RecogTimer::~VR_RecogTimer()
{
    VR_LOGD_FUNC();
}

VOID VR_RecogTimer::OnTimer()
{
    VR_LOGI("Enter VR_RecogTimer");
    string strResponse = "<action op =\"timeout\" value = \"timeout\"></action>";

    if (m_handler) {
        m_handler->PostMessage(strResponse);
    }
    this->Stop();
}

VR_BaiduImpl::VR_BaiduImpl()
    : m_client(strAppid, strApikey, strSecretkey)
    , m_controller(nullptr)
{
}


bool
VR_BaiduImpl::Initialize(VR_ControllerBase* controller)
{
    VR_LOGD_FUNC();

    m_timer = std::make_shared<VR_RecogTimer>(this);
    if (m_timer) {
        m_timer->Start();
        m_timer->Stop();
    }

    const std::string InputFolder = "/data/avs_data/inputs";
    m_controller = controller;

    VR_LOGD("before create speechDetector, data path: %s", InputFolder.c_str());

    m_speechcommandobserver = std::make_shared<SphinxS::VR_SpeechCommandObserver>(this);
    m_speechdetector = SphinxS::SphinxSpeechDetector::create(InputFolder, {"BIDUBIDU"}, {m_speechcommandobserver});

    VR_LOGD("after create speechDetector");

    if (!m_speechdetector) {
        VR_LOGE("create speechDetector Failed\n");
        return false;
    }

    VR_LOGI("BaiduImpl initialize sucessfully\n");
    return true;
}

void VR_BaiduImpl::UnInitialize()
{
    VR_LOGD_FUNC();
    if (m_timer) {
        m_timer->Stop();
    }
}

bool VR_BaiduImpl::Start()
{
    // VR_LOGD_FUNC();

    // if (!m_speechdetector->startStreamingMicrophoneData()) {
    //     VR_LOGE("start mic in Error\n");
    //     return false;
    // }
    return true;
}

void VR_BaiduImpl::Stop()
{
    // VR_LOGD_FUNC();

    // if (!m_speechdetector->stopStreamingMicrophoneData()) {
    //     VR_LOGE("stop mic in Error\n");
    // }
}

VR_BaiduImpl::~VR_BaiduImpl()
{

}

void VR_BaiduImpl::ProcessMessage(const std::string& message)
{
    VR_LOGD_FUNC();
    // if (!m_interactionManager) {
    //     VR_LOGI("AlexaImpl interaction manager is NULL!!!\n");
    //     return;
    // }

    // if ("tap" == message) {
    //     // m_interactionManager->startFileAudio("/pdata/GetGasStationReq.wav");  // GetGasStationReq.wav   GetParkReq.wav
    //     m_interactionManager->tap();
    //     // m_interactionManager->microphoneToggle();
    // }

    // if ("talkEnd" == message) {
    //     m_interactionManager->talkEnd();
    // }
    return;

}

int VR_BaiduImpl::onAudioInData(int16_t* buffer, int32_t size)
{
    if (m_speechdetector) {
        return m_speechdetector->OnAudioInData(buffer, size);
    }
    else {
        return -1;
    }
}

void VR_BaiduImpl::startRecordMicData()
{
    VR_LOGD_FUNC();
    if (m_speechdetector) {
        m_speechdetector->startStoringRecordData();
    }

    if (m_timer) {
        m_timer->Restart();
    }

    return;

}

void VR_BaiduImpl::stopRecordMicData()
{
    VR_LOGD_FUNC();
    if (m_speechdetector) {
        m_speechdetector->stopStoringRecordData();
    }

    if (m_timer) {
        m_timer->Stop();
    }

    return;

}

bool VR_BaiduImpl::PostMessage(const string &message, int actionId)
{
    VR_LOGD("message:%s", message.c_str());
    return m_controller->PostMessage(message, actionId);
}

void VR_BaiduImpl::postRequest(const std::string pcm)
{
    VR_LOGD_FUNC();
    VR_LOGD("postRequest content: pcm :%s", pcm.c_str());
    PostMessage("THINKING");
    VR_LOGD("start thinking");

    std::string file_content;
    baiduSDK::get_file_content(pcm.c_str(), &file_content);
    Json::Value result = m_client.recognize(file_content, "pcm", 16000, baiduSDK::null);

    string strResult = "";
    strResult = result["result"][0].asString().c_str();
    string strResponse = "<action op =\"recognizeResult\" value = \"" + strResult + "\"></action>";

    PostMessage(strResponse);

    return;
}
