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
#include "VR_DP_OB_Action_TslAppList.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_CommandHandleIF.h"
#include "VR_TslAppList_VrUnit.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_DP_OB_Action_TslAppList::VR_DP_OB_Action_TslAppList(NCUri                &uri,
                                             NCVariant            &variant,
                                             spVR_DialogEngineIF  spEngine,
                                             spVR_CommandHandleIF spCommandHandler)
    : VR_DP_OB_Action(uri, variant)
    , m_spDialogEngineIF(spEngine)
    , m_spCommandHandleIF(spCommandHandler)
{
    VR_LOGD_FUNC();
}

VR_DP_OB_Action_TslAppList::~VR_DP_OB_Action_TslAppList()
{
}

void VR_DP_OB_Action_TslAppList::DoUpdatePrepare()
{
}
}
}
/* EOF */
