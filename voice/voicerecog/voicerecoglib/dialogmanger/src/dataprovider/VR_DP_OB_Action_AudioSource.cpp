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

#include "VR_Log.h"
#include "VR_DP_OB_Action_AudioSource.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_CommandHandleIF.h"
#include "VR_AudioSourceListener.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_DP_OB_Action_AudioSource::VR_DP_OB_Action_AudioSource(NCUri                &uri,
                                                         NCVariant            &variant)
    : VR_DP_OB_Action(uri, variant)
    , m_isInitialize(false)
{
    VR_LOGD_FUNC();

    if (uri.equals(NCString(VR_DATAPROVIDER_INFO_URI))) {
        m_isInitialize = true;
    }
}

VR_DP_OB_Action_AudioSource::~VR_DP_OB_Action_AudioSource()
{

}

void VR_DP_OB_Action_AudioSource::DoUpdatePrepare()
{
    VR_AudioSourceListener* ptrAudioSource = VR_AudioSourceListener::GetInstance();
    if (NULL == ptrAudioSource) {
        VR_LOGD("VR_AudioSourceListener::GetInstance failed");
        return;
    }
    if (m_isInitialize) {
        ptrAudioSource->Initialize();
    }
    else {
        ptrAudioSource->OnNotifyUpdate(m_uri, m_variant);
    }
}

}
}
/* EOF */
