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
#include "VR_ConfigureIF.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_TslStateListener.h"
#include <contentresolver/NCContentResolver.h>
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "navi/Voice/VoiceRecog/VrNotifyRecogResult.pb.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{
VR_TslStateListener* VR_TslStateListener::m_instance = NULL;

VR_TslStateListener::VR_TslStateListener(spVR_DialogEngineIF spEngine)
    : m_spDialogEngineIF(spEngine)
    , m_tslStatus(0)
{
    VR_LOGD_FUNC();
}

VR_TslStateListener::~VR_TslStateListener()
{
    VR_LOGD_FUNC();
}

bool VR_TslStateListener::Initialize()
{
    VR_LOGD_FUNC();

    return GetGlobalState();
}

VR_TslStateListener* VR_TslStateListener::GetInstance(spVR_DialogEngineIF spEngine)
{
    if (NULL == m_instance) {
        m_instance = VR_new VR_TslStateListener(spEngine);
    }
    return(m_instance);
}

VOID VR_TslStateListener::Dispose()
{
    VR_LOGD_FUNC();
    if (NULL != m_instance) {
        delete m_instance;
        m_instance = NULL;
    }
}

void VR_TslStateListener::onNotifyUpdate(NCUri &uri, NCVariant &variant)
{
    VR_LOGD_FUNC();
    if (VR_TSL_STATUS_INFO_URI == uri.toString()) {
        VR_INT32 nTslstatus = 0;
        VR_String valueXml = variant.toString();
        if ("available" == valueXml) {
            nTslstatus = 1;
        }
        VR_String xml = "<display agent=\"Common\" content=\"TSLState\">"
                            "<state>%1%</state>"
                        "</display>";

        if (m_tslStatus != nTslstatus) {
            m_tslStatus = nTslstatus;
            VR_String msg = (boost::format(xml) % std::to_string(nTslstatus)).str();
            VR_LOGD("[TOFC]tsl status xml=[%s]", msg.c_str());
            sendStateMsgToFC(msg);

            xml = "<event name=\"updateState\">"\
                    "<item key=\"TSL_STATE_AVAILABLE\" value=\"%1%\"/>"\
             "</event>";
            VR_String tmp = (m_tslStatus ? "true" : "false");
            msg = (boost::format(xml) % tmp).str();
            if (m_spDialogEngineIF.get()) {
                VR_LOGD("[TODE]tsl status xml=[%s]", msg.c_str());
                m_spDialogEngineIF->SendMessage(msg);
            }
            VR_ConfigureIF::Instance()->setUpdateState(VR_TSL_STATE_AVAILABLE, tmp);
        }
    }
    else {
        VR_LOGD("[listener uri]->[%s]", uri.toString().getString());
    }
}

bool VR_TslStateListener::GetGlobalState()
{
    VR_LOGD_FUNC();
    NCString tslStatusValue;
    get("tsl/status", tslStatusValue);
    VR_INT32 nTslstatus = 0;
    VR_String strTslStatus = tslStatusValue.getString();
    if ("available" == strTslStatus) {
        nTslstatus = 1;
    }
    m_tslStatus = nTslstatus;
    VR_String xml = "<display agent=\"Common\" content=\"TSLState\">"
                        "<state>%1%</state>"
                    "</display>";

    VR_String msg = (boost::format(xml) % std::to_string(nTslstatus)).str();
    VR_LOGD("[TOFC]tsl status xml=[%s]", msg.c_str());
    sendStateMsgToFC(msg);

    xml = "<event name=\"updateState\">"\
            "<item key=\"TSL_STATE_AVAILABLE\" value=\"%1%\"/>"\
     "</event>";
    VR_String tmp = (m_tslStatus ? "true" : "false");
    msg = (boost::format(xml) % tmp).str();
    if (m_spDialogEngineIF.get()) {
        VR_LOGD("[TODE]tsl status xml=[%s]", msg.c_str());
        m_spDialogEngineIF->SendMessage(msg);
    }
    VR_ConfigureIF::Instance()->setUpdateState(VR_TSL_STATE_AVAILABLE, tmp);

    return true;
}

bool VR_TslStateListener::get(const NCString &key, NCString &value)
{
    VR_LOGD_FUNC();
    NCString uriString(VR_SYSTEM_INFO_URI);
    uriString += key;
    NCUri uri(uriString);
    NCContentResolver resolver;
    ncsp<NCCursor>::sp queryResult = resolver.query(uri, NCList<NCString>(), NCString(),
                                                    NCList<NCString>(), NCString());
    if ((queryResult != NULL) && (1 == queryResult->getCount()) && (NC_TRUE == queryResult->moveToFirst())) {
        value = queryResult->getString(0);
        VR_LOGD("value=[%s],return true", value.getString());
        return true;
    }
    VR_LOGD("return false");
    return false;
}

void VR_TslStateListener::sendStateMsgToFC(VR_String &stateXML)
{
    VR_LOGD_FUNC();
    std::unique_ptr<navi::VoiceRecog::VrNotifyRecogResult> protoMsg(navi::VoiceRecog::VrNotifyRecogResult::default_instance().New());
    protoMsg->set_sender("tslStatus");
    protoMsg->set_taskid(-1);
    protoMsg->set_recogresult(stateXML);

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYRECOGRESULT, protoMsg.release());
}

}
}
/* EOF */
