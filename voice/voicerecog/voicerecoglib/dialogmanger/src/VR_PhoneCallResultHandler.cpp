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
* @file  VR_PhoneCallResultHandler.cpp
* @brief Declaration file of class VR_PhoneCallResultHandler.
*
* This file includes the declaration of class VR_PhoneCallResultHandler.
*
* @attention used for C++ only.
*/


#include   "stdafx.h"
#include   "boost/format.hpp"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include   "EV_EventSender.h"
#include   "VR_PhoneCallResultHandler.h"
#include   "VR_ConfigureIF.h"
#include   "VR_RunableThread.h"
#include   "commu/NDTelCallStatus.pb.h"
#include   "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace nutshell::commu;
using namespace navi::VoiceRecog;

const  nutshell::NCString  PhoneCallSpaceName      =  nutshell::NCString("nutshell.commu.telephony");
const  nutshell::NCString  CallStatus_Event        =  nutshell::NCString("NCTelNotiEvent_CallStatus");


VR_PhoneCallEventHandler::VR_PhoneCallEventHandler(VR_PhoneCallResultHandler *pPhoneCallResult, const nutshell::NCRunnableLooper &looper)
    : nutshell::NCEventHandler(looper)
    , m_pResultHandler(pPhoneCallResult)
    , m_bPhoneStandBy(false)
    , m_bPhoneCall(false)
{
    VR_LOGD_FUNC();
}


VR_PhoneCallEventHandler::~VR_PhoneCallEventHandler()
{
    VR_LOGD_FUNC();
}

void  VR_PhoneCallEventHandler::handleEvent(const nutshell::NCEvent &event)
{
    VR_LOGD_FUNC();

    nutshell::NCString   ncSpaceName = event.space();
    if (-1 == ncSpaceName.find(PhoneCallSpaceName.getString())) {
       VR_LOGD("Event Space Name error: [%s]", ncSpaceName.getString());
       return;
    }

    nutshell::NCString  ncIdentity("");
    event.identity(&ncIdentity);

    if (CallStatus_Event == ncIdentity) {
        int  dataSize = event.extraDataSize();
        char*  pBuffer = VR_new char[dataSize + 1];
        if (NULL == pBuffer) {
            VR_LOGD("new buffer error. datasize: [%d]", dataSize);
            return;
        }

        memset(pBuffer, 0, sizeof(char) * (dataSize + 1));
        nutshell::INT32 iExtraDataSize = event.getExtraData(pBuffer, dataSize);
        if (iExtraDataSize <= 0) {
            delete[] pBuffer;
            pBuffer = NULL;
            VR_LOGE("getExtraData fail!!!");
            return;
        }

        NDTelCallStatus  ndCallStatus;
        bool bParseResult = ndCallStatus.ParseFromArray(pBuffer, dataSize); // ParseFromString sometims parse false.
        if (!bParseResult) {
            delete[] pBuffer;
            pBuffer = NULL;
            VR_LOGE("ParseFromArray fail!!!");
            return;
        }

        int nCount = ndCallStatus.count();
        VR_LOGD("*************nCount = [%d]******************", nCount);
        delete[] pBuffer;
        pBuffer = NULL;

        if (nCount <= 0) {  // When you hang up the phone
            if (!m_bPhoneStandBy) {
                VR_LOGD("*******************SendMessage***********************");
                std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
                loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
                VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
                controlMessage->set_type(DMTaskMessageType_PhoneCallResult);
                VrPhoneCallResult* pcPhoneCallRsult = controlMessage->mutable_phonecallresult();
                pcPhoneCallRsult->set_phonetype(phone_call_end);
                EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
                m_bPhoneStandBy = true;
            }
            m_bPhoneCall = false;
        }
        else {  // Upon receipt / transmission of telephone
            if (!m_bPhoneCall) {
                VR_LOGD("*******************SendMessage***********************");
                VR_REGION_TYPE emRegion = VR_ConfigureIF::Instance()->getVRContry();
                if ((VR_REGION_US != emRegion) && (VR_REGION_OC != emRegion)) {
                    std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
                    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
                    VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
                    controlMessage->set_type(DMTaskMessageType_PhoneCallResult);
                    VrPhoneCallResult* pcPhoneCallRsult = controlMessage->mutable_phonecallresult();
                    pcPhoneCallRsult->set_phonetype(phone_call_end);
                    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
                }
                m_bPhoneCall = true;
            }
            m_bPhoneStandBy = false;
        }
    }
    else {
        VR_LOGD("Event identity error; [%s]", ncIdentity.getString());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// VR_PhoneCallResultHandler
VR_PhoneCallResultHandler::VR_PhoneCallResultHandler(spVR_RunableThread  spRunableThread, spVR_DialogEngineIF spDialogEngine)
    : m_spRunableThread(spRunableThread)
    , m_spDialogEngine(spDialogEngine)
    , m_spPhoneCallEventHandler(NULL)
{
    VR_LOGD_FUNC();
}


VR_PhoneCallResultHandler::~VR_PhoneCallResultHandler()
{
    VR_LOGD_FUNC();
}

void  VR_PhoneCallResultHandler::OnStart()
{
    VR_LOGD_FUNC();

    m_filter.push_back(m_context.buildEventFullName(PhoneCallSpaceName, CallStatus_Event));
    m_spPhoneCallEventHandler =  VR_new  VR_PhoneCallEventHandler(this, m_spRunableThread->getRunableLooper());
    m_context.registerHandler(m_spPhoneCallEventHandler, m_filter);
}

void  VR_PhoneCallResultHandler::OnStop()
{
    VR_LOGD_FUNC();

    m_context.unregisterHandler(m_spPhoneCallEventHandler, m_filter);
    m_filter.clear();
}

void   VR_PhoneCallResultHandler::OnMessage(const VR_ProtoMessageBase& msg)
{
    VR_LOGD_FUNC();
    const  VrPhoneCallResult&  rsPhoneCallResult = (const VrPhoneCallResult&)msg;
    VrPhoneCallType ePhoneCallType = rsPhoneCallResult.phonetype();
    if (phone_call_end == ePhoneCallType) {
        VR_String cancelXml = "<event name=\"cancel\"></event>";
        m_spDialogEngine->SendMessage(cancelXml);
    }

}

void   VR_PhoneCallResultHandler::SendMessage()
{
    VR_String cancelXml = "<event name=\"cancel\"></event>";
    m_spDialogEngine->SendMessage(cancelXml);
}

/* EOF */
