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
#include "VR_RadioActiveBandAction.h"

#include "VR_ActionEventSender.h"

#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

#include "MediaInfoProvider/NMMediaInfoHelper.h"

using namespace navi::VoiceRecog;
using namespace nutshell::media;

/**
 * @brief The VR_RadioActiveBandAction class
 *
 * class declaration
 */
class VR_MediaInfoLoader
{
public:
    virtual bool load(UINT key, VOID* buff, UINT size)
    {
        return NMMediaInfoHelper::load(key, buff, size);
    }

    virtual ~VR_MediaInfoLoader() {}
};

VR_RadioActiveBandAction::VR_RadioActiveBandAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_RadioActiveBandAction::~VR_RadioActiveBandAction()
{
    VR_LOGD_FUNC();
}

void VR_RadioActiveBandAction::OnReceive(const VR_ProtoMessageBase& proto)
{
    VR_LOGD_FUNC();
}

void VR_RadioActiveBandAction::Run()
{
    VR_LOGD_FUNC();
}
/* EOF */
