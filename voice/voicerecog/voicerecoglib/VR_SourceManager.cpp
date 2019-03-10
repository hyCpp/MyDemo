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
#include "stdafx.h"
#include "VR_SourceManager.h"

#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "NMSourceManagerClientAPI.h"

using namespace nutshell::media;

VR_SourceManager::VR_SourceManager()
{
    VR_LOGD_FUNC();
    m_pSourceReplier = NMSourceManagerClientAPI::instance()->registerSource(MediaInfo_SrcID_Alexa, this);
}

VR_SourceManager::~VR_SourceManager()
{
    VR_LOGD_FUNC();
}

void VR_SourceManager::onHibernate()
{
    VR_LOGD_FUNC();
    if (NULL != m_pSourceReplier) {
        VR_LOGI("notifyAwake");
        m_pSourceReplier->notifyAwake();
    }
}

void VR_SourceManager::onConnected()
{
    VR_LOGD_FUNC();
}

void VR_SourceManager::onRequestSourceOn(const NMMediaMgrSourceInfo &srcInfo)
{
    VR_LOGD_FUNC();
    if (NULL != m_pSourceReplier) {
        VR_LOGI("onRequestSourceOn");
        m_pSourceReplier->replySourceOn(srcInfo);
    }
}

void VR_SourceManager::onRequestSourceOff(const NMMediaMgrSourceInfo &srcInfo)
{
    VR_LOGD_FUNC();
    if (NULL != m_pSourceReplier) {
        VR_LOGI("onRequestSourceOff");
        m_pSourceReplier->replySourceOff(srcInfo);
    }
}

/* EOF */
