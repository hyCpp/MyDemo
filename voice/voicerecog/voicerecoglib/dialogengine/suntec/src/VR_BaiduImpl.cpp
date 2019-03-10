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
#include "VR_BaiduImpl.h"
#include "VR_Log.h"
#include "VR_UIManager.h"
#include "VR_ConnectionObserver.h"
#include "VR_PortAudioMicWrapper.h"
#include "VR_KeywordObserver.h"
#include "VR_InteractionManager.h"
#include "VR_ConsolePrinter.h"
//#include "VR_DummyMediaPlayer.h"
#include "VR_AlexaObserver.h"
#include "VR_GoogleObserver.h"
#include "VR_ControllerBase.h"

#ifndef VR_BAIDUSDS_IF_H
#    include "VR_BaiduSDS_IF.h"
#endif

// VR_BaiduImpl
VR_BaiduImpl::VR_BaiduImpl()
: m_controller(nullptr)
, m_pBaiduSDS(nullptr)
, m_pAudioInStreamer(nullptr)
, m_micWrapper(nullptr)
{

}

bool
VR_BaiduImpl::Initialize(VR_ControllerBase* controller, VR_BaiduSDS* pBaiduSDS, std::shared_ptr<VR_PortAudioMicWrapper> micWrapper)
{
    VR_LOGD_FUNC();

    m_controller = controller;
    m_pBaiduSDS = pBaiduSDS;
    m_micWrapper = micWrapper;

    VR_LOGI("VR_BaiduImpl initialize sucessfully\n");
    return true;
}

void VR_BaiduImpl::UnInitialize()
{
    VR_LOGD_FUNC();
}

bool VR_BaiduImpl::Start()
{
    VR_LOGD_FUNC();
    return true;
}

void VR_BaiduImpl::Stop()
{
    VR_LOGD_FUNC();
}

VR_BaiduImpl::~VR_BaiduImpl()
{

}

void VR_BaiduImpl::ProcessMessage(const std::string& message)
{
    VR_LOGD_FUNC();

    if ("startBaiduDialog" == message) {
        if (m_pBaiduSDS) {
            VR_LOGI("ready to startBaiduDialog\n");
            m_pAudioInStreamer = m_pBaiduSDS->startDialog();

            if (nullptr == m_pAudioInStreamer) {
                VR_LOGI("m_pAudioInStreamer is NULL !\n");
                m_controller->PostMessage("IDLE", -1);
                return;
            }
            else if (m_micWrapper) {
                m_micWrapper->SetBaiduAudioIn(m_pAudioInStreamer);
            }
        }
        else {
            VR_LOGI("startBaiduDialog m_pBaiduSDS is NULL\n");
        }
    }
    else if ("stopBaiduDialog" == message) {
        if (m_pBaiduSDS) {
            VR_LOGI("ready to stopBaiduDialog\n");
            m_pBaiduSDS->stopDialog();
        }
        else {
            VR_LOGI("stopBaiduDialog m_pBaiduSDS is NULL\n");
        }
    }
    else {
        VR_LOGI("other VR_BaiduImpl message !\n");
    }

    return;
}

void VR_BaiduImpl::SetDialogState(const std::string& strDialogState)
{
    if (m_micWrapper) {
        m_micWrapper->SetMicState(strDialogState);
    }

    return;
}
/* EOF */
