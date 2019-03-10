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
* @file  VR_PersonDataEventHandler.cpp
* @brief Declaration file of class VR_PersonDataEventHandler.
*
* This file includes the declaration of class VR_PersonDataEventHandler.
*
* @attention used for C++ only.
*/


#include   "stdafx.h"
#include   "boost/format.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "VR_PersonDataEventHandler.h"
#include   "VR_ConfigureIF.h"
#include   "ncore/NCFile.h"
#include   "settingservice/NCSetting.h"
#include   "VR_RunableThread.h"
#include   "settingservice/NCSettingEventDefine.h"
#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using  namespace  navi::VoiceRecog;

const   nutshell::NCString   Common_Setting_Space     =  nutshell::NCString("nutshell.common.setting");
const   nutshell::NCString   Personal_Data_Identify   =  nutshell::NCString("init_personal_data");


VR_PersonSettingEventHandler::VR_PersonSettingEventHandler(VR_PersonDataEventHandler *pEventHandler, const nutshell::NCRunnableLooper &looper)
    : nutshell::NCEventHandler(looper)
    , m_pPersonHandler(pEventHandler)
{
    VR_LOGD_FUNC();
}

VR_PersonSettingEventHandler::~VR_PersonSettingEventHandler()
{
    VR_LOGD_FUNC();
}

void  VR_PersonSettingEventHandler::handleEvent(const nutshell::NCEvent &event)
{
    VR_LOGD_FUNC();

    nutshell::NCString  evtSpace = event.space();
    if (-1 == evtSpace.find(Common_Setting_Space.getString())) {
        VR_LOGD("Event space name error: [%s]", evtSpace.getString());
        return;
    }

    nutshell::NCString  ncIdentify("");
    event.identity(&ncIdentify);
    VR_LOGD("ncIdentify = [%s]", ncIdentify.getString());
    if (Personal_Data_Identify == ncIdentify) {
        std::unique_ptr<VrLoopBack>   loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
        controlMessage->set_type(DMTaskMessageType_InitPersonalData);
        controlMessage->mutable_personalresult()->set_datatype(persondata_start);

        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    else {
        VR_LOGD("Event Identify error: [%s]", ncIdentify.getString());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VR_PersonDataEventHandler::VR_PersonDataEventHandler(spVR_RunableThread spRunableThread, spVR_DialogEngineIF  spDialogEngine)
    : m_spRunableThread(spRunableThread)
    , m_spDialogEngine(spDialogEngine)
    , m_pcEventHandler(NULL)
{
    VR_LOGD_FUNC();
}


VR_PersonDataEventHandler::~VR_PersonDataEventHandler()
{
    VR_LOGD_FUNC();
}

void  VR_PersonDataEventHandler::OnStart()
{
    VR_LOGD_FUNC();

    m_filter.push_back(m_context.buildEventFullName(Common_Setting_Space, Personal_Data_Identify));
    m_pcEventHandler = VR_new  VR_PersonSettingEventHandler(this, m_spRunableThread->getRunableLooper());
    m_context.registerHandler(m_pcEventHandler, m_filter);
}

void  VR_PersonDataEventHandler::OnStop()
{
    VR_LOGD_FUNC();

    m_context.unregisterHandler(m_pcEventHandler, m_filter);
    m_filter.clear();
}


void  VR_PersonDataEventHandler::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();

    const  VrPersonalDataResult&  personResult  =  (const VrPersonalDataResult&)msg;
    VrPersonDataType  type  =  personResult.datatype();

    static  const  VR_String  xmlResult = "<event name=\"initialpersondata\"></event>";

    switch (type) {
    case  persondata_start :
        {
            VR_ConfigureIF::Instance()->setPersonDataStatus(false);
            m_spDialogEngine->SendMessage(xmlResult);
            {
#ifdef HAVE_NUTSHELL_OS20
        VR_LOGD("********xmlResult = [%s]*********", xmlResult.c_str());
        nutshell::NCSetting  ncSetting;
                ncSetting.initPersonalDataEnd("nutshell.service.voicerecog");
#endif
            }
            break;
        }
    case  persondata_end :
        {
            VR_ConfigureIF::Instance()->setPersonDataStatus(true);
#ifdef HAVE_NUTSHELL_OS20
            nutshell::NCSetting  ncSetting;
            ncSetting.initPersonalDataEnd("nutshell.service.voicerecog");
#endif
            break;
        }
    default:
        break;
    }
}

/* EOF */
