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
#include "VR_RadioListener.h"
#include "VR_DataProviderComm.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "system/NCEnvironment.h"
#include "VR_Configure.h"

#include <boost/format.hpp>

using namespace nutshell;
using namespace nutshell::media;
namespace navi
{
namespace dataprovider
{
VR_RadioListener* VR_RadioListener::m_instance = NULL;

VR_RadioListener::VR_RadioListener()
    : m_bRadioStatus(false)
{
    VR_LOGD_FUNC();
}

VR_RadioListener::~VR_RadioListener()
{
    VR_LOGD_FUNC();
}

VR_RadioListener* VR_RadioListener::GetInstance()
{
    if (NULL == m_instance) {
        m_instance = VR_new VR_RadioListener();
    }
    return(m_instance);
}

VOID VR_RadioListener::Dispose()
{
    VR_LOGD_FUNC();
    if (NULL != m_instance) {
        delete m_instance;
        m_instance = NULL;
    }
}

bool VR_RadioListener::Initialize()
{
    // 1.initiative buid radio status info file
    bool bFlag = CurrentSourceInfo(media::MediaInfoKey_CurSrc);
    if (!bFlag) {
        VR_LOGD("sync current source info failed");
        return false;
    }

    return true;
}

bool VR_RadioListener::CurrentSourceInfo(VR_INT32 mediaInfoKeyValue)
{
    MediaInfo_CurSrc curSrc;
    memset(&curSrc, 0, sizeof(curSrc));
    bool bFlag = NMMediaInfoHelper::load(mediaInfoKeyValue, &curSrc, sizeof(curSrc));
    VR_LOGI("NMMediaInfoHelper::load=[%d]", bFlag);
    if (bFlag) {
        if (MediaInfo_SrcID_NetRadio == curSrc.curSrc) {
            m_bRadioStatus = true;
        }
        else {
            m_bRadioStatus = false;
        }
        VR_ConfigureIF::Instance()->setBRadioStatus(m_bRadioStatus);
    }
    return bFlag;
}


VOID
VR_RadioListener::onNotifyUpdate(NCUri &uri, NCVariant &variant)
{
    INT mediaInfoKeyValue = atoi(variant.toString().c_str());
    switch (mediaInfoKeyValue) {
    case MediaInfoKey_CurSrc: {
        VR_LOGI("MediaInfoKey_CurSrc");
        bool bFlag = CurrentSourceInfo(mediaInfoKeyValue);
        if (!bFlag) {
            VR_LOGD("sync current source info failed");
            return;
        }
        break;
    }
    default :
        break;
    }
}

}
}
/* EOF */
