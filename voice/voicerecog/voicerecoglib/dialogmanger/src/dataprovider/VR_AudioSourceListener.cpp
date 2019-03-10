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
#include "VR_Log.h"
#include "VR_Configure.h"
#include "VR_XmlStrParse.h"
#include "VR_Indentifier.hpp"
#include "VR_DataProviderComm.h"
#include "VR_AudioSourceListener.h"
#include "VR_AudioSourceXMLBuilder.h"
#include "VR_AudioSourceNameManager.h"
#include "MediaProvider/NMMediaUri.h"
#include "MediaProvider/NMMediaProviderDef.h"
#include "MediaInfoProvider/NMMediaInfoDEF.h"
#include "MediaInfoProvider/NMMediaInfoUri.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "contentresolver/NCContentResolver.h"
#include "EV_Define.h"
#include "EV_EventSender.h"

#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"

using namespace nutshell;
using namespace media;
using namespace std;
namespace navi
{
namespace dataprovider
{
static nutshell::NCSyncObj g_cSyncObj;

VR_AudioSourceListener* VR_AudioSourceListener::m_instance = NULL;

VR_AudioSourceListener::VR_AudioSourceListener()
    : m_xmlBuilder(VR_new VR_AudioSourceXMLBuilder)
{
    VR_LOGD_FUNC();
}

VR_AudioSourceListener::~VR_AudioSourceListener()
{
    VR_LOGD_FUNC();
}

void VR_AudioSourceListener::Destory()
{
    VR_LOGD_FUNC();
    delete m_instance;
    m_instance = nullptr;
}

VR_AudioSourceListener* VR_AudioSourceListener::GetInstance()
{
    nutshell::NCAutoSync lock(g_cSyncObj);
    if (NULL == m_instance) {
        m_instance = VR_new VR_AudioSourceListener();
    }

    return m_instance;
}

void VR_AudioSourceListener::OnNotifyUpdate(NCUri &uri, NCVariant &variant)
{
    bool bRes = uri.equals(NMMediaInfoUri::getKeyContentUri(MediaInfoKey_AllSrcStatus));
    bRes = bRes | uri.equals(NMMediaInfoUri::getKeyContentUri(MediaInfoKey_UsbConnectState));
    bRes = bRes | uri.equals(NCString(VR_SYSTEM_INFO_BT_AVP_URI));
    if (!bRes) {
        VR_LOGD("not audio source url: %s", uri.toString().getString());
        return;
    }

    VR_LOGD("audio uri: %s", uri.toString().getString());
    NotifySourceStatus();
}

void VR_AudioSourceListener::Initialize()
{
    VR_LOGD_FUNC();
    NotifySourceStatus();
}

bool VR_AudioSourceListener::GetSourceStatus(std::string& out)
{
    VR_LOGD_FUNC();
    m_xmlBuilder->AppendSourceStatusXML();

    QueryIpodStatus();

    QueryUDiskStatus();

    QueryBTAStatus();

    m_xmlBuilder->GetXml(out);

    VR_String ConnStatus;
    VR_XmlStrParse parser;
    parser.LoadString(out);
    parser.GetNodeText("//music_sources/music_source[source_id = 1]/connected_status", ConnStatus);
    bool bConnState = ConnStatus == "connected" ? true : false;
    VR_ConfigureIF::Instance()->setMediaConnectStatus(usb_connect, bConnState);
    parser.GetNodeText("//music_sources/music_source[source_id = 2]/connected_status", ConnStatus);
    bConnState = ConnStatus == "connected" ? true : false;
    VR_ConfigureIF::Instance()->setMediaConnectStatus(ipod_connect, bConnState);
    parser.GetNodeText("//music_sources/music_source[source_id = 3]/connected_status", ConnStatus);
    bConnState = ConnStatus == "connected" ? true : false;
    VR_ConfigureIF::Instance()->setMediaConnectStatus(btaudio_connect, bConnState);
    return true;
}

void VR_AudioSourceListener::QueryUDiskStatus()
{
    VR_LOGD_FUNC();
    NCContentValues values;
    INT32 sourceId = VR_MUIFA_SOURCE_U_DISK;
    UINT sourceKey = MediaInfoKey_UsbConnectState;
    MediaInfo_UsbConnectState udState = MediaInfo_UsbUnconnect;
    bool result = NMMediaInfoHelper::load(sourceKey, &udState, sizeof(udState));
    if (!result) {
        VR_LOGD("query u-disk state failed");
        return;
    }
    std::string srcState = (MediaInfo_UsbConnected == udState) ?
                            "connected" : "disconnected";
    values.clear();
    values.put("sourceId", sourceId);
    values.put("state", srcState.c_str());
    m_xmlBuilder->UpdateSourceStatusNode(values);
    return;
}

void VR_AudioSourceListener::QueryIpodStatus()
{
    VR_LOGD_FUNC();
    MediaInfo_AllSrcStatus sStatus;
    UINT sourceKey = MediaInfoKey_AllSrcStatus;
    bool result = NMMediaInfoHelper::load(sourceKey, &sStatus, sizeof(sStatus));
    if (!result) {
        VR_LOGD("query all source value failed.allSourceKey:%d", sourceKey);
        return;
    }

    NCContentValues values;
    std::string srcState;
    INT32 sourceId = MediaInfo_SrcID_Invalid;

    for (int i = 0; i < MediaInfo_SrcID_Num; i++) {
        if (MediaInfo_SrcID_USB1 == sStatus.srcStatus[i].srcId) {
            sourceId = CheckUsbSourceType(sStatus.srcStatus[i].srcId);
        }
        else {
            sourceId = VR_MUIFA_SOURCE_INVALID;
        }

        if (VR_MUIFA_SOURCE_INVALID == sourceId) {
            continue;
        }

        srcState = (MediaInfo_ModeStatus_Valid == sStatus.srcStatus[i].status) ?
                   "connected" : "disconnected";
        values.clear();
        values.put("sourceId", sourceId);
        values.put("state", srcState.c_str());
        m_xmlBuilder->UpdateSourceStatusNode(values);
    }
    return;
}

void VR_AudioSourceListener::QueryBTAStatus()
{
    VR_LOGD_FUNC();

    NCUri uri(VR_SYSTEM_INFO_BT_AVP_URI);
    NCContentResolver resolver;
    NCCursor* pCursor = resolver.query(uri, NCList<NCString>(), NCString(),
                                                    NCList<NCString>(), NCString());
    if (nullptr == pCursor) {
        VR_LOGD("null res");
        return;
    }

    bool res = false;
    VR_String value;
    do {
        if (1 != pCursor->getCount()) {
            VR_LOGD("count invalid");
            break;
        }

        res = (NC_TRUE == pCursor->moveToFirst());
        if (!res) {
            VR_LOGD("move failed");
            break;
        }

        value = pCursor->getString(0).getString();
        res = true;
    } while (0);

    if (!res) {
        return;
    }

    NCContentValues values;
    std::string srcState = ("connected" == value) ? "connected" : "disconnected";

    values.clear();
    values.put("sourceId", VR_MUIFA_SOURCE_BT_AUDIO);
    values.put("state", srcState.c_str());
    m_xmlBuilder->UpdateSourceStatusNode(values);
    return;
}

void VR_AudioSourceListener::NotifySourceStatus()
{
    VR_XmlStrParse parser;
    bool res = true;
    do {
        parser.LoadString(VR_DATA_MEDIA_NOTIFYSOURCESTATUS);
        if (!res) {
            VR_LOGD("xml invalid");
            break;
        }

        string xmlStatus;
        res = GetSourceStatus(xmlStatus);
        if (!res) {
            VR_LOGD("get state failed");
            break;
        }

        VR_XmlStrParse body;
        res = body.LoadString(xmlStatus);
        if (!res) {
            VR_LOGD("body invalid");
            break;
        }

        res = parser.SetNodeText("//notification/id", VR_IdGenerator().GetID());
        if (!res) {
            VR_LOGD("set text failed");
            break;
        }

        res = parser.AddNode("//notification", xmlStatus);
        if (!res) {
            VR_LOGD("add note failed");
            break;
        }

    } while (0);

    if (!res) {
        return;
    }
    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("MESG: %s", mesg.c_str());
    sendMessageNATP(mesg);
}

void VR_AudioSourceListener::sendMessageNATP(std::string& mesg)
{
    using namespace navi::VoiceRecog;
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(mesg.c_str());
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

int VR_AudioSourceListener::CheckUsbSourceType(int originType)
{
    VR_LOGD_FUNC();
    if (MediaInfo_SrcID_USB1 != originType) {
        return VR_MUIFA_SOURCE_INVALID;
    }

    MediaInfo_UsbDeviceType usbType;
    memset(&usbType, 0x00, sizeof(MediaInfo_UsbDeviceType));

    bool result = NMMediaInfoHelper::load(MediaInfoKey_UsbType, &usbType, sizeof(usbType));
    if (!result) {
        VR_LOGD("query usb type failed");
        return VR_MUIFA_SOURCE_INVALID;
    }

    if (MediaInfo_UsbKind_Ipod == usbType.usb1) {
        return VR_MUIFA_SOURCE_IPOD;
    }

    return VR_MUIFA_SOURCE_INVALID;
}

}
}
/* EOF */
