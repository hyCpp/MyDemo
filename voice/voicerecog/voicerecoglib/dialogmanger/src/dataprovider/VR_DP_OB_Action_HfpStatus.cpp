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
#include "VR_DP_OB_Action_HfpStatus.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_CommandHandleIF.h"
#include "VR_PhoneBookStateListener.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{

VR_DP_OB_Action_HfpStatus::VR_DP_OB_Action_HfpStatus(NCUri                &uri,
                                                     NCVariant            &variant)
    : VR_DP_OB_Action(uri, variant)
{
    VR_LOGD_FUNC();
}

VR_DP_OB_Action_HfpStatus::~VR_DP_OB_Action_HfpStatus()
{
}

void VR_DP_OB_Action_HfpStatus::DoUpdatePrepare()
{
    VR_LOGD_FUNC();
    VR_PhoneBookStateListener* ptrPBStateListener = VR_PhoneBookStateListener::GetInstance();
    if (NULL == ptrPBStateListener) {
        ptrPBStateListener = VR_PhoneBookStateListener::Assign();
        if (NULL == ptrPBStateListener) {
            VR_LOGD(" listener is not exists");
            return;
        }

        ptrPBStateListener->Initialize();
    }
    else {
        ptrPBStateListener->onNotifyUpdate(m_uri, m_variant);
    }
}

}
}
/* EOF */
