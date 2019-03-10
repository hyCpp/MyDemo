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
* @file  VR_GBookEventHandler.cpp
* @brief Declaration file of class VR_GBookEventHandler.
*
* This file includes the declaration of class VR_GBookEventHandler.
*
* @attention used for C++ only.
*/

#include   "stdafx.h"
#include   "boost/format.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "VR_GBookEventHandler.h"
#include   "NCGBookEventDef.h"
#include   "NCGBookCommDef.h"
#include   "VR_RunableThread.h"
#include   "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace navi::VoiceRecog;

const  nutshell::NCString   GBOOK_EVENT_SPACE   =   nutshell::NCString("nutshell.telema.gbook");

VR_GBookComfirmHandler::VR_GBookComfirmHandler(VR_GBookEventHandler *pGBookHandler, const nutshell::NCRunnableLooper &looper)
    : nutshell::NCEventHandler(looper)
    , m_GBookHandler(pGBookHandler)
{
    VR_LOGD_FUNC();
}

VR_GBookComfirmHandler::~VR_GBookComfirmHandler()
{
    VR_LOGD_FUNC();
}

void   VR_GBookComfirmHandler::handleEvent(const nutshell::NCEvent &event)
{
    VR_LOGD_FUNC();

    nutshell::NCString   ncSpace = event.space();
    if (-1 == ncSpace.find(GBOOK_EVENT_SPACE.getString())) {
        VR_LOGD("Event space error: [%s]", ncSpace.getString());
        return;
    }

    nutshell::NCString  ncIdentity("");
    event.identity(&ncIdentity);
    VR_LOGD("ncIdentify = [%s]", ncIdentity.getString());
    nutshell::NCString  ncGookEvent(nutshell::NCGBOOK_EVENT_CONNECT_CONF_PRESSED_NOTIFY);
    VR_LOGD("ncGookEvent = [%s]", ncGookEvent.getString());
    if (ncGookEvent == ncIdentity) {
        VR_INT32  nDisplayType = event.arg1();
        VR_INT32  nPressType  = event.arg2();
        VR_LOGD("nDisplayType = [%d], ncPressType = [%d]", nDisplayType, nPressType);

        if (nutshell::GBK_CONNECT_CONF_DISPLAY_TYPE_VR == nDisplayType) {
            std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
            loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
            VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
            controlMessage->set_type(DMTaskMessageType_GBookUserType);
            switch (nPressType) {
            case nutshell::GBK_CONNECT_CONF_PRESS_TYPE_YES :
                {
                     controlMessage->mutable_gbookuserresult()->set_usertype(usertype_yes);
                     break;
                }
            case nutshell::GBK_CONNECT_CONF_PRESS_TYPE_NO :
                {
                    controlMessage->mutable_gbookuserresult()->set_usertype(usertype_no);
                    break;
                }
            case nutshell::GBK_CONNECT_CONF_PRESS_TYPE_CANCEL :
                {
                    controlMessage->mutable_gbookuserresult()->set_usertype(usertype_cancle);
                    break;
                }
            default:
                break;
            }
            EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////  ## VR_GBookEventHandler ## ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// VR_GBookEventHandler
VR_GBookEventHandler::VR_GBookEventHandler(spVR_RunableThread  spRunableThread, spVR_EventSenderIF spEventSender)
    : m_spRunnableThread(spRunableThread)
    , m_Vr_EventSender(spEventSender)
    , m_pcGBookEventHandler(NULL)
{
    m_filter.clear();
}


VR_GBookEventHandler::~VR_GBookEventHandler()
{
    VR_LOGD_FUNC();    // acc off
}

void  VR_GBookEventHandler::OnStart()
{
    VR_LOGD_FUNC();
    m_filter.push_back(m_context.buildEventFullName(GBOOK_EVENT_SPACE,  nutshell::NCString(nutshell::NCGBOOK_EVENT_CONNECT_CONF_PRESSED_NOTIFY)));
    m_pcGBookEventHandler = VR_new  VR_GBookComfirmHandler(this, m_spRunnableThread->getRunableLooper());
    m_context.registerHandler(m_pcGBookEventHandler, m_filter);
}

void  VR_GBookEventHandler::OnStop()
{
    VR_LOGD_FUNC();
    m_context.unregisterHandler(m_pcGBookEventHandler, m_filter);
    m_filter.clear();
}

void  VR_GBookEventHandler::OnMessage(const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
    const VrGBookUserResult&  gbookUserResult = (const VrGBookUserResult&)msg;
    VrGBookUserType userType = gbookUserResult.usertype();
    VR_LOGD("userType = [%d]", userType);

    std::unique_ptr<VrNotifyCommonProperty>  notifyCommonProperty(VrNotifyCommonProperty::default_instance().New());
    notifyCommonProperty->set_sender("dm sender");
    notifyCommonProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyHybirdOnsBtnOpe);
    VrNotifyHybirdOnsBtnOpe*  HybirdBtnOpe = notifyCommonProperty->mutable_hybirdonsbtnope();
    std::string  strUserOpe("Hybrid_Button_Unkown");
    switch (userType) {
    case  usertype_yes :
        {
            strUserOpe = "Hybrid_Button_Ok";
            break;
        }
    case usertype_no :
        {
            strUserOpe = "Hybrid_Button_No";
            break;
        }
    case usertype_cancle :
        {
            strUserOpe = "Hybrid_Button_Back";
            break;
        }
    default:
        break;
    }
    HybirdBtnOpe->set_btnope(strUserOpe);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, notifyCommonProperty.release());
}

/* EOF */
