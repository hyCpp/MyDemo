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
* @file  VR_StartServiceCondition.cpp
* @brief Declaration file of class VR_StartServiceCondition.
*
* This file includes the declaration of class VR_StartServiceCondition.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "boost/format.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "VR_SmartAgentHandler.h"
#include "settingservice/NCSetting.h"
#include "VR_ContentProvider.h"
#include "VR_ConfigureIF.h"
#include "VR_RunableThread.h"
#include "settingservice/NCSettingEventDefine.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

const nutshell::NCString DRIVER_SETTING_SPACE  =  nutshell::NCString("nutshell.common.setting");

using namespace navi::VoiceRecog;

VR_DriverSettingEventHandler::VR_DriverSettingEventHandler(VR_SmartAgentHandler *pAgentHandle, const nutshell::NCRunnableLooper &looper)
    : nutshell::NCEventHandler(looper),
      m_pSmartAgent(pAgentHandle)
{

}

VR_DriverSettingEventHandler::~VR_DriverSettingEventHandler()
{

}

void VR_DriverSettingEventHandler::handleEvent(const nutshell::NCEvent &event)
{
    VR_LOGD_FUNC();
    nutshell::NCString  evtSpace = event.space();
    if (-1 == evtSpace.find(DRIVER_SETTING_SPACE.getString())) {
        VR_LOGI("Event space error : [%s]", evtSpace.getString());
        return;
    }

    nutshell::NCString  ncIdentity("");
    event.identity(&ncIdentity);
    size_t size = event.extraDataSize();
    INT64  userID(-1);
    event.getExtraData(&userID, size);
    VR_LOGI("userid = [%d]", userID);
    nutshell::NCString ncSettingAdd(NCSETTING_EVENT_DRIVERSETTING_USER_ADDED);
    nutshell::NCString ncSettingDelete(NCSETTING_EVENT_DRIVERSETTING_USER_DELETED);
    nutshell::NCString ncSettingChange(NCSETTING_EVENT_DRIVERSETTING_USER_CHANGED);

    std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_SmartAgent);
    if (ncSettingAdd == ncIdentity) {
        controlMessage->mutable_smartagentresult()->set_type(smartagent_adduser);
        controlMessage->mutable_smartagentresult()->set_userid(userID);
    }
    else if (ncSettingDelete == ncIdentity) {
        controlMessage->mutable_smartagentresult()->set_type(smartagent_deleteuser);
        controlMessage->mutable_smartagentresult()->set_userid(userID);
    }
    else if (ncSettingChange == ncIdentity) {
        controlMessage->mutable_smartagentresult()->set_type(smartagent_changeuser);
        controlMessage->mutable_smartagentresult()->set_userid(userID);
    }
    else {
        VR_LOGI("Not Find space identity: [%s]", ncIdentity.getString());
        return;
    }
     EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// VR_SmartAgentHandler
VR_SmartAgentHandler::VR_SmartAgentHandler(spVR_RunableThread  spRunableThread, spVR_DialogEngineIF dialogEngine)
    : m_spRunableThread(spRunableThread)
    , m_spDialogEngine(dialogEngine)
    , m_pcEventHandler(NULL)
{
    m_filter.clear();
}

VR_SmartAgentHandler::~VR_SmartAgentHandler()
{

}

void VR_SmartAgentHandler::Start()
{
    VR_LOGD_FUNC();
    m_filter.push_back(m_context.buildEventFullName(DRIVER_SETTING_SPACE, nutshell::NCString(NCSETTING_EVENT_DRIVERSETTING_USER_ADDED)));
    m_filter.push_back(m_context.buildEventFullName(DRIVER_SETTING_SPACE, nutshell::NCString(NCSETTING_EVENT_DRIVERSETTING_USER_DELETED)));
    m_filter.push_back(m_context.buildEventFullName(DRIVER_SETTING_SPACE, nutshell::NCString(NCSETTING_EVENT_DRIVERSETTING_USER_CHANGED)));
    m_pcEventHandler = VR_new VR_DriverSettingEventHandler(this, m_spRunableThread->getRunableLooper());
    m_context.registerHandler(m_pcEventHandler, m_filter);
    this->NotifyDftUser();
}

void VR_SmartAgentHandler::Stop()
{
    VR_LOGD_FUNC();
    m_context.unregisterHandler(m_pcEventHandler, m_filter);
    m_filter.clear();
}

void  VR_SmartAgentHandler::NotifyDftUser()
{
    VR_LOGD_FUNC();
    static const std::string  strDefaultUrl = "content://settings/system?driversetting_current_bdaddr";
    VR_ContentProvider  vr_cp;
    std::string strValue = vr_cp.GetContentProviderValue(strDefaultUrl);

    const VR_String xmlStart = "<event name=\"smartagent\">"
                                      "<param name=\"%1%\" userid=\"%2%\"/>"
                              "</event>";
    VR_String  xmlResult = (boost::format(xmlStart) % "changeuser" % strValue).str();
    VR_LOGD("strValue = [%s], xmlResult = [%s]", strValue.c_str(), xmlResult.c_str());
    m_spDialogEngine->SendMessage(xmlResult);
}

void VR_SmartAgentHandler::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrSmartAgentResult& agentResult = (const VrSmartAgentResult&)msg;
    VrSmartAgentType type    =  agentResult.type();
    int64_t          userid  =  agentResult.userid();

    VR_String         xmlResult  = "";
    const  VR_String  xmlStart   = "<event name=\"smartagent\"><param name=\"%1%\" userid=\"%2%\"/></event>";
    VR_String         strName    = "adduser";
    VR_String         strUserId  = "";
    switch (type) {
    case smartagent_adduser :
        {
            VR_ConfigureIF::Instance()->setSmartAgentStatus(false);
            strName   = "adduser";
            strUserId = (boost::format("%1%") % userid).str();
            break;
        }
    case smartagent_deleteuser :
        {
            VR_ConfigureIF::Instance()->setSmartAgentStatus(false);
            strName   = "deleteuser";
            strUserId = (boost::format("%1%") % userid).str();
            break;
        }
    case smartagent_changeuser :
        {
            VR_ConfigureIF::Instance()->setSmartAgentStatus(false);
            strName = "changeuser";
            strUserId = (boost::format("%1%") % userid).str();
            break;
        }
    case smartagent_end :
        {
            VR_ConfigureIF::Instance()->setSmartAgentStatus(true);
            nutshell::NCSetting  ncSetting;
            ncSetting.userChangeEnd("nutshell.service.vrsmartagent");
            return;
        }
    default:
        break;
    }
    xmlResult = (boost::format(xmlStart) % strName % strUserId).str();
    VR_LOGI("xmlResult = [%s]", xmlResult.c_str());
    m_spDialogEngine->SendMessage(xmlResult);
}

/* EOF */
