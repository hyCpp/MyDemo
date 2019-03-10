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
#include "VR_DP_OB_Action_Radio.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_CommandHandleIF.h"
#include "VR_RadioListener.h"


using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_DP_OB_Action_Radio::VR_DP_OB_Action_Radio(NCUri                &uri,
                                             NCVariant            &variant)
    : VR_DP_OB_Action(uri, variant)
    , m_isInitialize(false)
{
    VR_LOGD_FUNC();

    if (uri.equals(NCString(VR_DATAPROVIDER_INFO_URI))) {
        m_isInitialize = true;
    }
}

VR_DP_OB_Action_Radio::~VR_DP_OB_Action_Radio()
{
}

void VR_DP_OB_Action_Radio::DoUpdatePrepare()
{
    NCUri uri = m_uri;
    NCVariant variant = m_variant;

    VR_RadioListener* ptrRadioListener = VR_RadioListener::GetInstance();
    if (NULL == ptrRadioListener) {
        VR_LOGD("VR_RadioListener::GetInstance failed");
        return;
    }
    if (m_isInitialize) {
        ptrRadioListener->Initialize();
    }
    else {
        ptrRadioListener->onNotifyUpdate(uri, variant);
    }
}
}
}
/* EOF */
