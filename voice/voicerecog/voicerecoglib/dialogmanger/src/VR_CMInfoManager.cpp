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
 * @file VR_CMInfoManager.cpp
 * @brief Defintion file of class VR_CMInfoManager.
 *
 * This file includes the Defintion of class VR_CMResourceHelper and Derived classes.
 *
 * @attention used for C++ only.
 */

#include <math.h>

#include "VR_Log.h"

#include "VR_XmlStrParse.h"
#include "VR_CMInfoManager.h"
#include "VR_Indentifier.hpp"
#include "VR_CMResourceDEF.h"
#include "VR_CMResourceHelper.h"

#include "EV_Define.h"
#include "EV_EventSender.h"

#include "boost/format.hpp"

#include "navi/Voice/VoiceRecog/VrVehicleInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"

using namespace navi::VoiceRecog;

VR_CMInfoManager::VR_CMInfoManager()
{
    VR_LOGD_FUNC();
    m_CurFunc = "";
    m_Remain = 0;
}

VR_CMInfoManager::~VR_CMInfoManager()
{
    VR_LOGD_FUNC();
}

void VR_CMInfoManager::UpdateVehicleInfoStatus(const VR_ProtoMessageBase &mesg)
{
    VR_LOGD_FUNC();
    if ("navi.VoiceRecog.VrVehicleInfo" != mesg.GetTypeName()) {
        VR_LOGD("proto error: %s", mesg.GetTypeName().c_str());
        return;
    }

    const VrVehicleInfo& status = dynamic_cast<const VrVehicleInfo&>(mesg);

    if (status.has_m_vrfuelwarning()) {
        VR_LOGD("fuel warning: %d", status.m_vrfuelwarning());
        onUpdateFuelLow(status.m_vrfuelwarning());
    }
    else if (status.has_m_vrmaintaininfo()) {
        VR_LOGD("Vehicle MaintainInfo");
        onUpdateMaintain(status.m_vrmaintaininfo());
    }
    else if (status.has_m_vrfrontfoglights()) {
        VR_LOGD("front light: %d", status.m_vrfrontfoglights());
        onUpdateFrontLightAction(status.m_vrfrontfoglights());
    }
    else if (status.has_m_vrrearfoglights()) {
        VR_LOGD("rear light: %d", status.m_vrrearfoglights());
        onUpdateRearLightAction(status.m_vrrearfoglights());
    }
    else if (status.has_m_vrhighbeam()) {
        VR_LOGD("high beam: %d", status.m_vrhighbeam());
        onUpdateHighBeamAction(status.m_vrhighbeam());
    }
    else if (status.has_m_vrdrivingrange()) {
        VR_LOGD("update range: %d", status.m_vrdrivingrange());
        m_Remain = status.m_vrdrivingrange();
    }
    return;
}

void VR_CMInfoManager::onRequestCarManualInquire(const VR_String& mesg)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    bool res = parser.LoadString(mesg);
    if (!res) {
        VR_LOGD("load error");
        return;
    }

    VR_String id;
    VR_String type;
    VR_String category;

    parser.GetNodeText("//id", id);
    parser.GetNodeText("//params/type", type);
    parser.GetNodeText("//category", category);

    int iCategory = m_ResHelper.convertCMCategory(category);
    int iType = m_ResHelper.convertCMType(type);

    VR_String info;
    VR_String funName;
    m_ResHelper.GetCarManualInquireText(iCategory, iType, funName, info);

    parser.Reset();
    res = parser.LoadString(VR_CAR_MANUAL_RESP_INQUIRE);
    if (!res) {
        VR_LOGD("load error");
        return;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_id", id);
    parser.SetNodeText("//infor", info);
    parser.SetNodeText("//func_name", funName);
    parser.SetNodeText("//error_no", "CM_RESULT_OK");

    VR_String xml;
    parser.GetXmlString(xml);

    VR_LOGD("xml: %s", xml.c_str());
    SendMesgToNATP(xml);
    return;
}

void VR_CMInfoManager::onCarManualHowStart(VR_String& funcName)
{
    VR_LOGD_FUNC();

    m_CurFunc = funcName;
}

void VR_CMInfoManager::onCarManualHowEnd()
{
    VR_LOGD_FUNC();

    m_CurFunc = "";
}


void VR_CMInfoManager::onUpdateFuelLow(bool status)
{
    VR_LOGD_FUNC();
    if (!status) {
        VR_LOGD("status false");
        return;
    }

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_CAR_MANUAL_NOTIFY_CAR_WARNING_INFO);
    if (!res) {
        VR_LOGD("load error");
        return;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//cm_category", "CM_FUEL_LOW");

    VR_String info;
    VR_String funName;
    res = m_ResHelper.GetCarManualNotifyText(eCM_FUEL_LOW, funName, info);
    if (!res) {
        VR_LOGD("get text error");
        return;
    }

    parser.SetNodeText("//func_name", funName);
    parser.SetNodeText("//noti_info", info);

    VR_String suggText;
    res = m_ResHelper.GetCarManualSolutionText(eCM_FUEL_LOW, suggText);
    if (!res) {
        VR_LOGD("get text error");
        return;
    }
    parser.SetNodeText("//noti_solution", suggText);

    parser.SetNodeText("//cm_note_type", "CM_NOTE_TYPE_WARNING");
    parser.SetNodeText("//suggestion_type", "CM_SUGGESTION_NAVI");
    parser.AddNode("//notification", "suggenstion_info_list", "");
    parser.AddNode("//suggenstion_info_list", "suggest_info", "");
    parser.AddNode("//suggest_info", "suggenstion_key", "SUGGESTION_KEY_NAME");
    parser.AddNode("//suggest_info", "suggestion_value", "加油站");

    VR_String mesg;
    parser.GetXmlString(mesg);

    VR_LOGD("mesg: %s", mesg.c_str());
    SendMesgToNATP(mesg);

    return;
}

void VR_CMInfoManager::onUpdateMaintain(const VR_ProtoMessageBase &mesg)
{
    VR_LOGD_FUNC();

    const VrMaintainInfo& status = dynamic_cast<const VrMaintainInfo&>(mesg);

    int category = eCM_INVALID;
    if (VrMaintainStatusType_Expired == status.type_maintaindate()) {
        category = eCM_MAINTAIN_WITH_EXPIRED;
    }
    else if (VrMaintainStatusType_Warning == status.type_maintainmile()
        || VrMaintainStatusType_Expired == status.type_maintainmile()) {
        category = eCM_MAINTAIN_WITH_MILES;
    }
    else {
        return;
    }

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_CAR_MANUAL_NOTIFY_CAR_WARNING_INFO);
    if (!res) {
        VR_LOGD("load error");
        return;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//cm_category", "CM_MAINTAIN");

    VR_String info;
    VR_String funName;
    res = m_ResHelper.GetCarManualNotifyText(category, funName, info);
    if (!res) {
        VR_LOGD("get info text error");
        return;
    }

    parser.SetNodeText("//func_name", funName);
    VR_String str;
    if (eCM_MAINTAIN_WITH_EXPIRED == category) {
        double month = floor(status.value_date() / 30);
        str = (boost::format(info) % month).str();
    }
    else {
        str = (boost::format(info) % status.value_mile()).str();
    }
    parser.SetNodeText("//noti_info", str);

    VR_String suggText;
    res = m_ResHelper.GetCarManualSolutionText(category, suggText);
    if (!res) {
        VR_LOGD("get sugg text error");
        return;
    }
    parser.SetNodeText("//noti_solution", suggText);

    parser.SetNodeText("//cm_note_type", "CM_NOTE_TYPE_WARNING");
    parser.SetNodeText("//suggestion_type", "CM_SUGGESTION_PHONE");
    parser.AddNode("//notification", "suggenstion_info_list", "");
    parser.AddNode("//suggenstion_info_list", "suggest_info", "");
    parser.AddNode("//suggest_info", "suggenstion_key", "SUGGESTION_KEY_PHONE_NUMBER");
    parser.AddNode("//suggest_info", "suggestion_value", "400-810-1210");       // 一汽丰田4S店服务电话

    VR_String xml;
    parser.GetXmlString(xml);

    VR_LOGD("mesg: %s", xml.c_str());
    SendMesgToNATP(xml);
    return;
}

void VR_CMInfoManager::onUpdateFrontLightAction(bool status)
{
    VR_LOGD_FUNC();
    VR_UNUSED_VAR(status);
    if ("CM_FOG_LAMP" != m_CurFunc) {
        return;
    }

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_CAR_MANUAL_NOTIFY_CAR_ACTION_RESULT);
    if (!res) {
        VR_LOGD("load error");
        return;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result", "CM_CORRECT_ACTION");
    parser.SetNodeText("//func_name", "雾灯");

    VR_String mesg;
    parser.GetXmlString(mesg);

    VR_LOGD("mesg: %s", mesg.c_str());
    SendMesgToNATP(mesg);
    return;
}

void VR_CMInfoManager::onUpdateRearLightAction(bool status)
{
    VR_LOGD_FUNC();
    VR_UNUSED_VAR(status);
    if ("CM_FOG_LAMP" != m_CurFunc) {
        return;
    }

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_CAR_MANUAL_NOTIFY_CAR_ACTION_RESULT);
    if (!res) {
        VR_LOGD("load error");
        return;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result", "CM_CORRECT_ACTION");
    parser.SetNodeText("//func_name", "雾灯");

    VR_String mesg;
    parser.GetXmlString(mesg);

    VR_LOGD("mesg: %s", mesg.c_str());
    SendMesgToNATP(mesg);
    return;
}

void VR_CMInfoManager::onUpdateHighBeamAction(bool status)
{
    VR_LOGD_FUNC();
    VR_UNUSED_VAR(status);
    if ("CM_FOG_LAMP" != m_CurFunc) {
        return;
    }

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_CAR_MANUAL_NOTIFY_CAR_ACTION_RESULT);
    if (!res) {
        VR_LOGD("load error");
        return;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action_result", "CM_FALLIBLE_ACTION");
    parser.SetNodeText("//func_name", "远光灯");

    VR_String mesg;
    parser.GetXmlString(mesg);

    VR_LOGD("mesg: %s", mesg.c_str());
    SendMesgToNATP(mesg);
    return;
}

void VR_CMInfoManager::SendMesgToNATP(VR_String& mesg)
{
    VR_LOGD_FUNC();
    using namespace navi::VoiceRecog;
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(mesg.c_str());
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

/* EOF */
