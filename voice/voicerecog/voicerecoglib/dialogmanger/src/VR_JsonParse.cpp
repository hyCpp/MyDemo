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
* @file  VR_JsonParse.cpp
* @brief Declaration file of class VR_JsonParse.
*
* This file includes the declaration of class VR_JsonParse.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "boost/format.hpp"
#include <boost/assign/list_of.hpp>
#include "EV_Define.h"
#include "EV_EventSender.h"
#include "VR_JsonParse.h"

#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"

using namespace navi::VoiceRecog;

const VoiceMap<VR_String, VR_String>::type g_strCommand = boost::assign::map_list_of
        ("GetDestinationReq",      "/data/GetDestinationReq.wav")
        ("GetParkReq",             "/data/GetParkReq.wav")
        ("GetGasStationReq",       "/data/GetGasStationReq.wav");

VR_JsonParse::VR_JsonParse()
{
}

VR_JsonParse::~VR_JsonParse()
{
}

VR_String VR_JsonParse::ParseJson(const VR_String &strMsg)
{
    VR_String strEmpty;
    VR_String strRet;
    VR_String strXml = "<event name=\"audioFile\">"
                       "<path>%1%</path>"
                    "</event>";
    if (LoadString(strMsg)) {
        if (m_docJson.HasMember("command") && m_docJson["command"].IsString()) {
            strRet = m_docJson["command"].GetString();
            if ("Stop" == strRet) {
                strRet = "<action agent=\"system\" op=\"stop\"></action>";
                NotifyCommandAction(strRet);
                return strEmpty;
            }
            VoiceMap<VR_String, VR_String>::const_iterator iter = g_strCommand.find(strRet);
            if (iter == g_strCommand.end()) {
                VR_LOGE("strRet[%s] don't exist!", strRet.c_str());
            }
            else {
                strRet = (boost::format(strXml) % iter->second).str();
            }
        }
    }

    VR_LOGI("strRet=[%s]", strRet.c_str());
    return strRet;
}

bool VR_JsonParse::LoadString(const VR_String &strMsg)
{
    VR_LOGI("strMsg=[%s]", strMsg.c_str());
    m_docJson.Parse<0>(strMsg.c_str());

    if (m_docJson.HasParseError()) {
        VR_LOGE("parse strMsg error:[%d]", __LINE__);
        return false;
    }
    return true;
}

void VR_JsonParse::NotifyCommandAction(const VR_String &strMsg)
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
    noitfyProperty->set_sender("Alexa");
    noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyCommonAction);
    noitfyProperty->mutable_vuicommonaction()->set_action(strMsg);

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
}

VR_String VR_JsonParse::getStringFromJson(const VR_String &strMsg, const VR_String &strKey)
{
    m_docJson.Parse<0>(strMsg.c_str());
    if (m_docJson.HasParseError() || !m_docJson.HasMember(strKey.c_str())) {
        return "";
    }

    const rapidjson::Value &jv = m_docJson[strKey.c_str()];
    return jv.GetString();
}

/* EOF */
