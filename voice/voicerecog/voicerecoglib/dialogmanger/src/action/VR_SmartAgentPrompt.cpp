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

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "stdafx.h"
#include "VR_SmartAgentPrompt.h"
#include "VR_ActionEventSender.h"
#include "VR_ContentProvider.h"
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;

/**
 * @brief The VR_SmartAgentPrompt class
 *
 * class declaration
 */
VR_SmartAgentPrompt::VR_SmartAgentPrompt(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_EngineReqId(id)
    , m_ActionType(type)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_SmartAgentPrompt::~VR_SmartAgentPrompt()
{
    VR_LOGD_FUNC();
}

void VR_SmartAgentPrompt::Run()
{
    VR_LOGD_FUNC();
    RunAction();
    this->SetActionComplete();
}

void VR_SmartAgentPrompt::OnReceive(const VR_ProtoMessageBase &proto)
{

}

void VR_SmartAgentPrompt::RunAction()
{
    VR_LOGD_FUNC();

    const VrSmartPromptLevel& promptlevel = (const VrSmartPromptLevel&)(*m_ReqMsg);
    std::string strLevel = promptlevel.promptlevel();
    VR_LOGI("strLevel = [%s]", strLevel.c_str());

    VR_ContentProvider vr_cp;
    vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, "voicesetting_prompts", strLevel);

    static VR_String resultXml = "<action-result agent=\"smartagent\" op=\"setPromptLevel\" errcode=\"0\">"
                                "</action-result>";
    VR_LOGI("resultXml = [%s]", resultXml.c_str());
    m_sender->sendEngineReply(m_EngineReqId, resultXml);
}

/* EOF */
