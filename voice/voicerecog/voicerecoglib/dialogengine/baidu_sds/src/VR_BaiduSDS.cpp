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

#include "VR_BaiduSDS_IF.h"

#include "VR_ControllerBase.h"
#include "VR_MsgController.h"
#include "VR_SDSController.h"
#include "VR_BaiduAudioInStreamer.h"

VR_BaiduSDS* VR_BaiduSDS::pBaiduSDSInstance = nullptr;

VR_BaiduSDS* VR_BaiduSDS::CreateBaiduSDS(VR_BaiduSDSListener* listener)
{
    VR_LOGD_FUNC();
    if (!pBaiduSDSInstance) {
        pBaiduSDSInstance = new VR_BaiduSDS();
        pBaiduSDSInstance->initialize(listener);
        pBaiduSDSInstance->start();
    }

    return pBaiduSDSInstance;
}

void VR_BaiduSDS::Destroy()
{
    VR_LOGD_FUNC();
    pBaiduSDSInstance->stop();
    pBaiduSDSInstance->unInitialize();
    delete pBaiduSDSInstance;
    pBaiduSDSInstance = nullptr;
}

VR_AudioInStreamer* VR_BaiduSDS::startDialog()
{
    VR_LOGD_FUNC();
    m_pController->StartDialogIF();
    return m_audioStreamer.get();
}

int VR_BaiduSDS::stopDialog()
{
    VR_LOGD_FUNC();
    m_pController->StopDialog();
    return 0;
}

bool VR_BaiduSDS::onMessage(const std::string& message, int actionSeqId)
{
    VR_LOGD_FUNC();
    m_pController->ProcessMessage(message, actionSeqId);
    return true;
}

VR_BaiduSDS::VR_BaiduSDS()
    :m_pListener(nullptr)
    ,m_pController(nullptr)
    ,m_msgController(nullptr)
    ,m_audioStreamer(nullptr)
{

}

void VR_BaiduSDS::initialize(VR_BaiduSDSListener* listener)
{
    VR_LOGD_FUNC();
    m_pListener = listener;

    m_msgController = std::make_shared<VR_MsgController>();
    m_pController = std::make_shared<VR_SDSController>();
    m_audioStreamer = std::make_shared<VR_BaiduAudioInStreamer>(m_pController);

    if (!m_pController->Initialize(m_pListener, m_msgController)) {
        return;
    }

    if (!m_msgController->Initialize(m_pController.get())) {
        return;
    }
    VR_LOGD("VR_MsgController Initialize successfully");
}

void VR_BaiduSDS::unInitialize()
{
    VR_LOGD_FUNC();
    if (nullptr != m_pController) {
        m_pController->UnInitialize();
    }
}

void VR_BaiduSDS::start()
{
    VR_LOGD_FUNC();
    if (!m_pController || !m_msgController || !m_pListener) {
        VR_LOGE("Some member pointers are nullptr!");
        return;
    }

    VR_LOGD("Start SDSController...");
    if (!m_pController->Start()) {
        VR_LOGE("SDSController started failed!");
        return;
    }

    VR_LOGD("Start message controller...");
    if (m_msgController) {
        m_msgController->Start();
    }

    m_pListener->OnStarted();
}

void VR_BaiduSDS::stop()
{
    VR_LOGD_FUNC();

    if (m_pController) {
        m_pController->Stop();
    }

    if (m_msgController) {
        m_msgController->Stop();
    }

    if (m_pListener) {
        m_pListener->OnStopped();
    }
}

/* EOF */