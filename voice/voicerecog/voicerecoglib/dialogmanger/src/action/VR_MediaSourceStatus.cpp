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

#include <string>
#include "boost/format.hpp"

#include "VR_MediaSourceStatus.h"
#include "VR_DataProviderComm.h"

#include "VR_ConfigureIF.h"

#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

using namespace std;
using namespace navi::dataprovider;
using namespace navi::VoiceRecog;


VR_MediaSourceStatus::VR_MediaSourceStatus(spVR_ActionEventSender sender,
                                           int id, int type,
                                           const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
    , m_ReqMsg(proto.New())
{
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
    VR_LOGD_FUNC();
}

VR_MediaSourceStatus::~VR_MediaSourceStatus()
{
    VR_LOGD_FUNC();
}

void VR_MediaSourceStatus::Run()
{
    VR_LOGD_FUNC();
}

void VR_MediaSourceStatus::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
    return;
}

/* EOF */
