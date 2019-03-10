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
#include "VR_DP_OB_Action_CityList.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_CommandHandleIF.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_DP_OB_Action_CityList::VR_DP_OB_Action_CityList(NCUri              &uri,
                                                NCVariant             &variant,
                                                const VR_ProtoMessageBase   &pMsg,
                                                spVR_DialogEngineIF   spEngine,
                                                spVR_CommandHandleIF  spCommandHandler)
    : VR_DP_OB_Action(uri, variant)
    , m_spMsg(pMsg.New())
    , m_spDialogEngineIF(spEngine)
    , m_spCommandHandleIF(spCommandHandler)
    , m_isInitialize(false)
{
    VR_LOGD_FUNC();
    if (uri.equals(NCString(VR_DATAPROVIDER_INFO_URI))) {
        m_isInitialize = true;
    }
    m_spMsg->CheckTypeAndMergeFrom(pMsg);
}

VR_DP_OB_Action_CityList::~VR_DP_OB_Action_CityList()
{
}

void VR_DP_OB_Action_CityList::DoUpdatePrepare()
{

}
}
}
/* EOF */
