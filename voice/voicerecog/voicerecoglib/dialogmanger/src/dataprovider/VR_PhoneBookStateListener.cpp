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

#include "stdafx.h"
#include <boost/format.hpp>
#include <algorithm>
#include <string>
#include "VR_ConfigureIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_PhoneBookXMLFormat.h"
#include "VR_PhoneBookStateListener.h"
#include <contentresolver/NCContentResolver.h>
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

#include "VR_PhoneBookListener.h"
#include "VR_XmlStrParse.h"
#include "VR_Indentifier.hpp"
#include "NCBtSettingProxy.h"
#include "CL_AutoSync.h"
#include "VR_ContentProvider.h"

using namespace navi::VoiceRecog;
using namespace nutshell;

namespace navi
{
namespace dataprovider
{
VR_PhoneBookStateListener* VR_PhoneBookStateListener::m_instance = NULL;

VR_PhoneBookStateListener::VR_PhoneBookStateListener()
    : m_bPhoneConnected(false)
    , m_bSMSSupported(false)
    , m_bPhoneInService(false)
    , m_strDeviceAddr("")
{

}

VR_PhoneBookStateListener::~VR_PhoneBookStateListener()
{
    VR_LOGD_FUNC();
}

VR_PhoneBookStateListener* VR_PhoneBookStateListener::GetInstance()
{
    return(m_instance);
}

VR_PhoneBookStateListener *VR_PhoneBookStateListener::Assign()
{
    if (NULL == m_instance) {
        m_instance = VR_new VR_PhoneBookStateListener();
    }

    return(m_instance);
}

void VR_PhoneBookStateListener::Dispose()
{
    VR_LOGD_FUNC();
    if (NULL != m_instance) {
        delete m_instance;
        m_instance = NULL;
    }
}

bool VR_PhoneBookStateListener::Initialize()
{
    VR_LOGD_FUNC();

    if (!handlePhoneState()) {
        return false;
    }

    return true;
}

void VR_PhoneBookStateListener::onNotifyUpdate(NCUri uri, NCVariant variant)
{
    VR_LOGD_FUNC();

    CL_AutoSync cAutoSync(m_cSync);
    VR_String valueXml = variant.toString();
    VR_LOGD("[URI : %s] [Value : %s]", uri.toString().getString(), valueXml.c_str());
    if (NC_TRUE == uri.equals(NCString(VR_SYSTEM_INFO_BT_HFP_MAIN_URI))) {
        if ("connected" == valueXml) {
            m_bPhoneConnected = true;
            getDevice();

            VR_String strPhoneSingalStatus = "";
            if (getPhoneSignalStatus(strPhoneSingalStatus)) {
                m_bPhoneInService = checkSignalStatus(strPhoneSingalStatus);
            }
            else {
                m_bPhoneInService = false;
            }
        }
        else {
            m_bPhoneConnected = false;
            m_bPhoneInService = false;
        }
        sendPhoneConnectEvent();
    }
    else if (NC_TRUE == uri.equals(NCString(VR_SYSTEM_INFO_BT_RSS_URI))) {
        bool bStatus = checkSignalStatus(valueXml);
        VR_LOGD("current signal is %s", valueXml.c_str());
        if (bStatus != m_bPhoneInService) {
            m_bPhoneInService = bStatus;
            sendPhoneConnectEvent();
        }
    }
    else {
        VR_LOGD("Ignore this uri");
    }

    if (m_bPhoneConnected) {
        std::string strPhoneState("connected");
        VR_ConfigureIF::Instance()->setUpdateState(VR_PHONE_STATE_CONNECTED, strPhoneState);
    }
    else {
        std::string strPhoneState("disconnected");
        VR_ConfigureIF::Instance()->setUpdateState(VR_PHONE_STATE_CONNECTED, strPhoneState);
    }
}

bool VR_PhoneBookStateListener::handlePhoneState()
{
    VR_LOGD_FUNC();

    CL_AutoSync cAutoSync(m_cSync);
    if (!handlePhoneConnect()) {
        return false;
    }

    return true;
}

bool VR_PhoneBookStateListener::handlePhoneConnect()
{
    VR_LOGD_FUNC();

    VR_String strPhoneConnectStatus = "";
    if (!getPhoneConnectStatus(strPhoneConnectStatus)) {
        VR_LOGD("get phone connect uri error");
        return false;
    }

    if ("disconnected" == strPhoneConnectStatus) {
        VR_LOGD("phone is not connect");
        m_bPhoneConnected = false;
        m_bPhoneInService = false;
        return false;
    }
    else {
        m_bPhoneConnected = true;
    }

    // get phone device mac
    getDevice();

    VR_String strPhoneSingalStatus = "";
    if (!getPhoneSignalStatus(strPhoneSingalStatus)) {
        return false;
    }
    m_bPhoneInService = checkSignalStatus(strPhoneSingalStatus);

    // send the event to NATP
    sendPhoneConnectEvent();

    if (m_bPhoneConnected) {
        std::string strPhoneState("connected");
        VR_ConfigureIF::Instance()->setUpdateState(VR_PHONE_STATE_CONNECTED, strPhoneState);
    }
    else {
        std::string strPhoneState("disconnected");
        VR_ConfigureIF::Instance()->setUpdateState(VR_PHONE_STATE_CONNECTED, strPhoneState);
    }

    return true;
}

bool VR_PhoneBookStateListener::getPhoneConnectStatus(VR_String& strStatus)
{
    VR_LOGD_FUNC();

    VR_String strURI = VR_SYSTEM_INFO_BT_HFP_MAIN_URI;
    if (!getURIValue(strURI, strStatus)) {
        return false;
    }
    return true;
}

bool VR_PhoneBookStateListener::getPhoneSignalStatus(VR_String& strStatus)
{
    VR_LOGD_FUNC();

    VR_String strURI = VR_SYSTEM_INFO_BT_RSS_URI;
    if (!getURIValue(strURI, strStatus)) {
        VR_LOGD("get current singal error");
        return false;
    }
    VR_LOGD("current singal is %s", strStatus.c_str());
    return true;
}

bool VR_PhoneBookStateListener::getPhoneDeviceId(VR_String& strDeviceId)
{
    VR_String strURI = VR_SYSTEM_INFO_BT_HFP_CURDEV_URI;
    if (!getURIValue(strURI, strDeviceId)) {
        return false;
    }
    return true;
}

bool VR_PhoneBookStateListener::getURIValue(const VR_String& strURI, VR_String& strValue)
{
    NCString ncstrURI(strURI.c_str());
    NCString ncstrValue;
    NCUri uri(ncstrURI);
    NCContentResolver resolver;
    ncsp<NCCursor>::sp queryResult = resolver.query(uri, NCList<NCString>(), NCString(),
                                                    NCList<NCString>(), NCString());
    if ((queryResult != NULL) && (1 == queryResult->getCount()) && (queryResult->moveToFirst())) {
        ncstrValue = queryResult->getString(0);
        strValue = ncstrValue.getString();
        VR_LOGD("[URI:%s]; [value:%s]", strURI.c_str(), strValue.c_str());
        return true;
    }

    return false;
}

void VR_PhoneBookStateListener::sendPhoneConnectEvent()
{
    VR_XmlStrParse parser;
    parser.LoadString(VR_PHONE_CONNECTE_STATUS_XML);
    parser.SetNodeText("//notification/id", VR_IdGenerator().GetID());
    if (m_bPhoneConnected) {
        parser.SetNodeText("//notification/phone_status", m_bSMSSupported ? VR_PHONE_STATUS_CONNECT : VR_PHONE_STATE_NOT_SUPPORT_SMS);
    }
    else {
        parser.SetNodeText("//notification/phone_status", VR_PHONE_STATUS_DISCONNECT);
    }
    parser.SetNodeText("//notification/phone_network_status", m_bPhoneInService ? VR_PHONE_IN_SIGNAL : VR_PHONE_NO_SIGNAL);
    parser.SetNodeText("//notification/device_id", m_strDeviceAddr);

    VR_String strXML;
    parser.GetXmlString(strXML);

    VR_ContentProvider vr_cp;
    VR_String strEngineType = vr_cp.GetContentProviderValue(VR_IAUTO_VR_ENGINETYPE);
    VR_LOGI("strEngineType = [%s]", strEngineType.c_str());
    if ("Nuance" != strEngineType) {
        SendMsg2DE(strXML);
        return;
    }
    sendEventToNATP(strXML);
}

void VR_PhoneBookStateListener::sendEventToNATP(const VR_String& strEvent)
{
    VR_LOGD("Event : %s", strEvent.c_str());
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(strEvent);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

void VR_PhoneBookStateListener::getDevice()
{
    VR_LOGD_FUNC();
    m_strDeviceAddr = "";
    nutshell::NCBtSettingProxy btSettingProxy;
    UINT64 unitAddr;
    btSettingProxy.getMainPhoneDeviceAddr(unitAddr);

    char buffer[100] = { 0 };
    sprintf(buffer, "%x", unitAddr);
    m_strDeviceAddr = VR_String(buffer);
    transform(m_strDeviceAddr.begin(), m_strDeviceAddr.end(), m_strDeviceAddr.begin(), toupper);
    VR_LOGD("BT address : %s", m_strDeviceAddr.c_str());
}

void VR_PhoneBookStateListener::SendMsg2DE(const VR_String &msg)
{
    VR_LOGI("msg=[%s]", msg.c_str());
    std::unique_ptr<VrLoopBack> loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_DM2DE);
    loopMessage->mutable_m_vrsendmsgdm2de()->set_msg(msg);
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

bool VR_PhoneBookStateListener::getPhoneStatus(VR_String& strStatus)
{
    CL_AutoSync cAutoSync(m_cSync);

    if (m_bPhoneConnected) {
        strStatus = m_bSMSSupported ? VR_PHONE_STATUS_CONNECT : VR_PHONE_STATE_NOT_SUPPORT_SMS;
    }
    else {
        strStatus = VR_PHONE_STATUS_DISCONNECT;
    }
    VR_LOGD("phone status[%s]", strStatus.c_str());
    return true;
}

bool VR_PhoneBookStateListener::getSignalStatus(VR_String& strStatus)
{
    CL_AutoSync cAutoSync(m_cSync);
    strStatus = m_bPhoneInService ? VR_PHONE_IN_SIGNAL : VR_PHONE_NO_SIGNAL;
    return true;
}

bool VR_PhoneBookStateListener::getDeviceId(VR_String& strDevice)
{
    CL_AutoSync cAutoSync(m_cSync);
    if (!m_bPhoneConnected) {
        VR_LOGD("phone is not connected");
        return false;
    }

    if (m_strDeviceAddr == "") {
        VR_LOGD("phone mac address is not empty");
        return false;
    }

    strDevice = m_strDeviceAddr;
    return true;
}

bool VR_PhoneBookStateListener::setSmsSupport(const VR_String& strValue)
{
    CL_AutoSync cAutoSync(m_cSync);
    VR_LOGD("connectstatus  value is [%s]", strValue.c_str());
    if ("1" == strValue) {
        m_bSMSSupported = true;
    }
    else {
        m_bSMSSupported = false;
    }
    return true;
}

bool VR_PhoneBookStateListener::checkSignalStatus(const VR_String& strValue)
{
    if ("noservice" == strValue
        || "unknown" == strValue
        || "0" == strValue) {
        return false;
    }
    return true;
}

}
}
/* EOF */
