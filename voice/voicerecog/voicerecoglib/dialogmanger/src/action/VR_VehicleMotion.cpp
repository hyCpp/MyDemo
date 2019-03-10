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
#include "VR_VehicleMotion.h"
#include "VR_ActionEventSender.h"
#include "VR_ContentProvider.h"
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"

using namespace navi::VoiceRecog;

/**
 * @brief The VR_VehicleMotion class
 *
 * class declaration
 */
VR_VehicleMotion::VR_VehicleMotion(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_EngineReqId(id)
    , m_ActionType(type)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_VehicleMotion::~VR_VehicleMotion()
{
    VR_LOGD_FUNC();
}

void VR_VehicleMotion::Run()
{
    VR_LOGD_FUNC();
    RunAction();
    this->SetActionComplete();
}

void VR_VehicleMotion::OnReceive(const VR_ProtoMessageBase &proto)
{

}

void VR_VehicleMotion::RunAction()
{
    VR_ContentProvider vr_cp;
    VR_String strVehicle = vr_cp.GetContentProviderValue(VR_VEHICLE_MOTION);
    VR_LOGI("strVehicle = [%s]", strVehicle.c_str());
    static VR_String startXml = "<action-result agent=\"help\""
                                          " op=\"queryVehicleinMotion\" errcode=\"0\">"
                                          "<motion>%1%</motion>"
                                      "</action-result>";
    static VR_String resultXml = "";
    VR_String strMotion = "";
    if (strVehicle == "on") {
        strMotion = "true";
    }
    else {
        strMotion = "false";
    }
    resultXml = (boost::format(startXml) % strMotion).str();
    VR_LOGI("resultXml = [%s]", resultXml.c_str());
    m_sender->sendEngineReply(m_EngineReqId, resultXml);
}

/* EOF */

