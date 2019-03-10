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
#include "VR_GoogleImpl.h"
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

#ifndef CXX_GOOASSPUBLIC_H
#    include "GooAssPublic.h"
#endif

// VR_GoogleImpl
VR_GoogleImpl::VR_GoogleImpl()
: m_controller(nullptr)
, m_pGoogleAbstractSDS(nullptr)
, m_pAudioInStreamer(nullptr)
, m_micWrapper(nullptr)
{

}

bool
VR_GoogleImpl::Initialize(VR_ControllerBase* controller, AbstractSDS* pGoogleAbstractSDS, std::shared_ptr<VR_PortAudioMicWrapper> micWrapper)
{
    VR_LOGD_FUNC();

    m_controller = controller;
    m_pGoogleAbstractSDS = pGoogleAbstractSDS;
    m_micWrapper = micWrapper;

    VR_LOGI("VR_GoogleImpl initialize sucessfully\n");
    return true;
}

void VR_GoogleImpl::UnInitialize()
{
    VR_LOGD_FUNC();
}

bool VR_GoogleImpl::Start()
{
    VR_LOGD_FUNC();
    return true;
}

void VR_GoogleImpl::Stop()
{
    VR_LOGD_FUNC();
}

VR_GoogleImpl::~VR_GoogleImpl()
{

}

void VR_GoogleImpl::ProcessMessage(const std::string& message)
{
    VR_LOGD_FUNC();

    if ("startGoogleDialog" == message) {
        if (m_pGoogleAbstractSDS) {
            VR_LOGI("ready to start GoogleDialog\n");
            m_pAudioInStreamer = m_pGoogleAbstractSDS->startDialog();

            if (nullptr == m_pAudioInStreamer) {
                VR_LOGI("m_pAudioInStreamer is NULL !\n");
                m_controller->PostMessage("IDLE", -1);
                return;
            }
            else if (m_micWrapper) {
                m_micWrapper->SetGoogleAudioIn(m_pAudioInStreamer);
            }
        }
        else {
            VR_LOGI("startGoogleDialog m_pGoogleAbstractSDS is NULL\n");
        }
    }
    else if ("stopGoogleDialog" == message) {
        if (m_pGoogleAbstractSDS) {
            VR_LOGI("ready to stop GoogleDialog\n");
            m_pGoogleAbstractSDS->stopDialog();
        }
        else {
            VR_LOGI("stopGoogleDialog m_pGoogleAbstractSDS is NULL\n");
        }
    }
    else {
        VR_LOGI("other GoogleImpl message !\n");
    }

    return;
}

void VR_GoogleImpl::SetDialogState(const std::string& strDialogState)
{
    if (m_micWrapper) {
        m_micWrapper->SetMicState(strDialogState);
    }

    return;
}
/* EOF */
