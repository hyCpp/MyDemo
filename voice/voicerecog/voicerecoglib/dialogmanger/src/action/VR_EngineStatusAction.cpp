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
 *  @file   VR_EngineStatusAction.cpp
 *  @brief  Implementation  class  of  VR_EngineStatusAction
 */

#include   "stdafx.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "VR_EngineStatusAction.h"
#include   "VR_TransformXmlToXml.h"

#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"
#include   "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include   "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"

using  namespace navi::VoiceRecog;

const std::string  Vr_Engine_Init    = "init";
const std::string  Vr_Engine_Busy    = "engine";
const std::string  Vr_Engine_Unknow  = "unknow";
const std::string  Vr_VoiceTag_State = "voicetag";

VR_EngineStatusAction::VR_EngineStatusAction(spVR_ActionEventSender sender, int nEngineId, int nActionType, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_nEngineId(nEngineId)
    , m_nActionType(nActionType)
    , m_ReqMsg(proto.New())
{
    VR_LOGD("nEngineId = [%d], nActionType = [%d]", m_nEngineId, m_nActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_EngineStatusAction::~VR_EngineStatusAction()
{
    VR_LOGD_FUNC();
}

void  VR_EngineStatusAction::Run()
{
    VR_LOGD_FUNC();

    const VrDEStatusResult&  deStatus = (const VrDEStatusResult&)(*m_ReqMsg);

    std::string  strType  =  deStatus.type();
    int          nStatus  =  deStatus.result();
    VR_LOGD("Eegnie status Type is: [%s], nValue is: [%d]", strType.c_str(), nStatus);

    if (Vr_Engine_Unknow == strType) {
        VrUnitStatusType  statusType = engine_status_ok;
        bool              bResult    = true;
        if (engine_in_correct == nStatus) {
            statusType = engine_status_ok;
            bResult    = false;
        }
        else if (dictionary_in_update == nStatus) {
            statusType = dictionary_updating;
            bResult    = true;
        }
        else if (engine_in_initialize == nStatus) {
            statusType = engine_updating;
            bResult    = true;
        }

        std::unique_ptr<VrLoopBack>    loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage*  controlMessage = loopMessage->mutable_m_controlmessage();
        controlMessage->set_type(DMTaskMessageType_UnitStatusResult);
        controlMessage->mutable_unitstatusnotify()->set_type(statusType);
        controlMessage->mutable_unitstatusnotify()->set_result(bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    else if (Vr_Engine_Init == strType) {
        bool  bResult = (available == nStatus) ? false : true;

        std::unique_ptr<VrLoopBack>   loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* pControlMessage = loopMessage->mutable_m_controlmessage();
        pControlMessage->set_type(DMTaskMessageType_StartStatusResult);
        pControlMessage->mutable_startstatusresult()->set_starttype(Engine_Updating);
        pControlMessage->mutable_startstatusresult()->set_startreuslt(bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());

        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("dm sender");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyAlexaEngineInitStatus);
        VrNotifyAlexaEngineInitStatus* pVrNotifyAlexaEngineInitStatus = noitfyProperty->mutable_alexaengineinitstatus();
        pVrNotifyAlexaEngineInitStatus->set_status(!bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());

    }
    else if (Vr_Engine_Busy == strType) {
        bool bResult = (available == nStatus) ? false : true;

        std::unique_ptr<VrLoopBack>   loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* pControlMessage = loopMessage->mutable_m_controlmessage();
        pControlMessage->set_type(DMTaskMessageType_StartStatusResult);
        pControlMessage->mutable_startstatusresult()->set_starttype(Engine_Busy);
        pControlMessage->mutable_startstatusresult()->set_startreuslt(bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());

        if (!bResult) {
            VR_String strState = "SYSTEM_STATE_READY";
            VR_String strStateAction = "SYSTEM_STATE_BEGIN";
            VR_String strMsg = VR_TransformXmlToXml().CreateStateXML(0, strState, strStateAction);
            std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
            noitfyProperty->set_sender("NATP");
            noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyVRUpdateData);
            noitfyProperty->mutable_vrupdatedata()->set_updatedata(strMsg);
            EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
        }
    }
    else if (Vr_VoiceTag_State == strType) {
        bool bResult = (available == nStatus) ? false : true;

        std::unique_ptr<VrLoopBack>   loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* pControlMessage = loopMessage->mutable_m_controlmessage();
        pControlMessage->set_type(DMTaskMessageType_StartStatusResult);
        pControlMessage->mutable_startstatusresult()->set_starttype(VoiceTag_Not_Using);
        pControlMessage->mutable_startstatusresult()->set_startreuslt(bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    this->SetActionComplete();
}

void  VR_EngineStatusAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}

/* EOF */
