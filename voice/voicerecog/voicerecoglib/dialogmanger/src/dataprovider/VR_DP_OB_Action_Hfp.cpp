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
#include "VR_DP_OB_Action_Hfp.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_CommandHandleIF.h"
#include "VR_PhoneBookListener.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{

VR_DP_OB_Action_Hfp::VR_DP_OB_Action_Hfp(NCUri                &uri,
                                         NCVariant            &variant)
    : VR_DP_OB_Action(uri, variant)
{
    VR_LOGD_FUNC();
}

VR_DP_OB_Action_Hfp::~VR_DP_OB_Action_Hfp()
{
}

void VR_DP_OB_Action_Hfp::DoUpdatePrepare()
{
    VR_LOGD_FUNC();
    VR_PhoneBookListener* ptrPBListener = VR_PhoneBookListener::GetInstance();
    if (NULL == ptrPBListener) {
        ptrPBListener = VR_PhoneBookListener::Assign();
        if (NULL == ptrPBListener) {
            VR_LOGD(" listener is not exists");
            return;
        }
        ptrPBListener->Initialize();
    }
    else {
        ptrPBListener->onNotifyUpdate(m_uri, m_variant);
    }
}

}
}
/* EOF */
