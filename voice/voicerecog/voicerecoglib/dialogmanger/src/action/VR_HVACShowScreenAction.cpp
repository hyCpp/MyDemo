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
 *  @file   VR_HVACShowScreenAction.cpp
 *  @brief  Implementation  class  of  VR_HVACShowScreenAction
 */

#include  "stdafx.h"
#include  "VR_ActionEventSender.h"
#include  "VR_HVACShowScreenAction.h"
#include  "VR_HAVCTypedefine.h"
#include  "VR_ActionInnerParameterDef.h"
#include  "EV_Define.h"

#include  <boost/format.hpp>

#include  "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include  "navi/Voice/VoiceRecog/Internal/VrDMAction.pb.h"
#include  "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include  "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"


using namespace navi::VoiceRecog;

VR_HVACShowScreenAction::VR_HVACShowScreenAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_EngineReqId(id)
    , m_ActionType(type)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_HVACShowScreenAction::~VR_HVACShowScreenAction()
{
    VR_LOGD_FUNC();
}

void  VR_HVACShowScreenAction::Run()
{
    VR_LOGD_FUNC();
    RunAction();
    this->SetActionComplete();
}

void  VR_HVACShowScreenAction::RunAction()
{
    VR_LOGI("ActionType: [%d]", m_ActionType);
    std::string startXml = "<action agent=\"climate\" op=\"%1%\">"
                           "</action>";
    std::string resultXml = "";
    std::string strOp = "";
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(m_EngineReqId);
    notifyAction->set_actionid(m_ActionType);
    switch (m_ActionType) {
    case HVACFrontScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_SHOW_FRONT_SCREEN;
            break;
        }
    case HVACRearScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_SHOW_REAR_SCREEN;
            break;
        }
    case HVACSteeringScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_SHOW_STEERING_SCREEN;
            break;
        }
    case HVACFrontSeatScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_SHOW_FRONT_SEAT_SCREEN;
            break;
        }
    case HVACRearSeatScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_SHOW_REAR_SEAT_SCREEN;
            break;
        }
    case HVACLexusConciergeScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_SHOW_LEXUS_CONCIERGE_SCREEN;
            break;
        }
    case HVACSeatOperationScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_SEAT_OPERATION_SCREEN;
            break;
        }
    case FrontSeatVentilationScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_FRONT_SEAT_VEN_SCREEN;
            break;
        }
    case RearSeatVentilationScreen:
        {
            strOp = FC_VOICE_ACTION_HVAC_REAR_SEAT_VEN_SCREEN;
            break;
        }
    default:
        break;
    }
    resultXml = (boost::format(startXml) % strOp).str();
    VR_LOGI("resultXml = [%s]", resultXml.c_str());
    notifyAction->set_actioncontent(resultXml);
    m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());
}

void  VR_HVACShowScreenAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}
/* EOF */
