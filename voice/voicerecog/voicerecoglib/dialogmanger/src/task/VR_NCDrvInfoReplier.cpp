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

/**
 * @file VR_NCDrvInfoReplier.cpp
 * @brief Implementation of class VR_NCDrvInfoReplier
 */

#include "VR_NCDrvInfoReplier.h"
#include "VR_Log.h"
#include "VR_NCDrvInfoProxy.h"

using namespace nutshell;
using namespace nutshell::drvasst;

VR_NCDrvInfoReplier::VR_NCDrvInfoReplier(VR_NCDrvInfoProxy* pDrvInfoProxy)
    : m_pDrvInfoProxy(pDrvInfoProxy)
{
    VR_LOGD("DrvInfo_VR_NCDrvInfoReplier");
}

VR_NCDrvInfoReplier::~VR_NCDrvInfoReplier()
{
    VR_LOGD("DrvInfo_~VR_NCDrvInfoReplier");
}

void VR_NCDrvInfoReplier::replyGetDrvEvaluate(const nutshell::drvinfoservice::NDDrvinfoEvaluate &evaluateInfo, UINT64 call_id)
{
    VR_LOGD("DrvInfo_replyGetDrvEvaluate");

    m_pDrvInfoProxy->replyDrvInfo(evaluateInfo);
}

/* EOF */
