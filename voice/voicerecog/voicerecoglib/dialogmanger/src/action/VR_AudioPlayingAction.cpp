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

#include <sstream>

#include "stdafx.h"
#include "VR_AudioPlayingAction.h"
#include "VR_ActionEventSender.h"
#include "VR_ActionInnerParameterDef.h"

#include <boost/format.hpp>

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "EV_Define.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "MediaInfoProvider/NMMediaInfoDEF.h"
#include "system/NCEnvironment.h"
#include "VR_AudioSourceListener.h"

using namespace nutshell::media;
using namespace navi::VoiceRecog;
using namespace nutshell;
using namespace navi::dataprovider;

const std::string usb1Name = "USB Device1";
const std::string usb2Name = "USB Device2";
const std::string ipod1Name = "iPod Device1";
const std::string ipod2Name = "iPod Device2";


VR_AudioPlayingAction::VR_AudioPlayingAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_EngineReqId(id)
    , m_ActionType(type)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_AudioPlayingAction::~VR_AudioPlayingAction()
{
    VR_LOGD_FUNC();
}

void VR_AudioPlayingAction::Run()
{
    VR_LOGD_FUNC();

    RunAction();
    this->SetActionComplete();
}

void VR_AudioPlayingAction::OnReceive(const VR_ProtoMessageBase &proto)
{

}

void VR_AudioPlayingAction::RunAction()
{

}
/* EOF */
