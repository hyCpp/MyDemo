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
#include <boost/format.hpp>
#include <boost/make_shared.hpp>

#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "VR_ConfigureIF.h"
#include "VR_DataProvider.h"
#include "VR_Indentifier.hpp"
#include "VR_RunableThread.h"
#include "VR_DialogEngineIF.h"
#include "VR_DataProviderComm.h"
#include "VR_AudioSourceListener.h"
#include "VR_MusicDBHelperManager.h"
#include "VR_PhoneBookListener.h"
#include "VR_PhoneBookStateListener.h"

#include "VR_DP_OB_Action_Music.h"

#include "MediaInfoProvider/NMMediaInfoDEF.h"
#include "MediaInfoProvider/NMMediaInfoUri.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"

#include "EV_Define.h"
#include "EV_EventSender.h"

#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"

using namespace std;
using namespace nutshell;
using namespace nutshell::media;
using namespace navi::VoiceRecog;
namespace navi
{
namespace dataprovider
{

const NCString DATAPRIVATE_LISTENER_NAME = "VR_DataProviderListener";
VR_DataProvider::VR_DataProvider()
    : m_spDPThread(VR_new VR_DP_ThreadWorking())
    , m_spRunableThread(VR_new VR_RunableThread(DATAPRIVATE_LISTENER_NAME))
    , m_mediaDataProvider(VR_new NMMediaDataProviderIF())
{
    VR_LOGD_FUNC();
    m_spDataProviderListener = spVR_DP_OBListener(VR_new VR_DP_OBListener(DATAPRIVATE_LISTENER_NAME,
                                                                          m_spRunableThread->getRunableLooper(),
                                                                          m_spDPThread));
}

VR_DataProvider::~VR_DataProvider()
{
    VR_LOGD_FUNC();
     m_mediaDataProvider->onDeinit();
}

bool VR_DataProvider::Initialize()
{
    VR_LOGD_FUNC();
    m_mediaDataProvider->onInit();
    return true;
}

bool VR_DataProvider::RegistObserver()
{
    VR_LOGD_FUNC();
    // add usb spec uri
    const NCString device("mediashare");
    NCString uri = m_mediaDataProvider->getNotifyUri(device);
    tDPListenerURI_Pair item(uri, URI_MUSIC);
    m_spDataProviderListener->AddOneUri(item);

    m_spDataProviderListener->Regist();
    m_spDataProviderListener->Initialize();
    return true;
}

void VR_DataProvider::UnRegistObserver()
{
    VR_LOGD_FUNC();
    m_spDataProviderListener->UnRegist();
    m_spRunableThread->stopThread();
}

void VR_DataProvider::StartDPThread()
{
    VR_LOGD_FUNC();
    m_spDPThread->StartThread();
    m_mediaDataProvider->onStart();
}

void VR_DataProvider::StopDPThread()
{
    VR_LOGD_FUNC();
    m_mediaDataProvider->onStop();
    UnRegistObserver();
    m_spDPThread->StopThread();
}

void VR_DataProvider::OnStartedNotify()
{
    VR_LOGD_FUNC();
    RegistObserver();
}

void VR_DataProvider::setMediaProvider(spNMMediaDataProviderIF& ptr)
{
    VR_LOGD_FUNC();
    m_mediaDataProvider = ptr;
}

void VR_DataProvider::SendMesgNATP(VR_String& mesg)
{
    VR_LOGD_FUNC();
    using namespace navi::VoiceRecog;
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(mesg.c_str());
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

bool VR_DataProvider::OnReplyMusicDevStatus(VR_String& id, VR_String& srcType)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_DATA_MEDIA_RESPSOURCESTATUS);
    if (!res) {
        VR_LOGD("xml invalid");
        return false;
    }

    do {
        res = parser.SetNodeText("//action_result/action_id", id.c_str());
        if (!res) {
            VR_LOGD("set id error");
            break;
        }

        res = parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
        if (!res) {
            VR_LOGD("set aid error");
            break;
        }

        res = parser.SetNodeText("//action_result/error_no", VR_MEDIA_RESULT_OK);
        if (!res) {
            VR_LOGD("set errno error");
            break;
        }

        VR_String state;
        VR_String stateXML;
        res = GetOptionSourceState(srcType, stateXML, state);
        if (!res) {
            VR_LOGD("get option state failed");
            break;
        }

        parser.AddNode("//action_result", stateXML);
    } while (0);

    if (!res) {
        parser.SetNodeText("//action_result/error_no", VR_MEDIA_RESULT_ERROR);
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("MESG: %s", mesg.c_str());
    SendMesgNATP(mesg);
    return true;
}

bool VR_DataProvider::OnReplyMusicGetLocalInfo(VR_String& strxml)
{
    VR_LOGD_FUNC();

    VR_XmlStrParse parser;
    bool res = parser.LoadString(strxml);
    if (!res) {
        VR_LOGD("load error, %s", strxml.c_str());
        return false;
    }
    VR_String id;
    parser.GetNodeText("//action/id", id);

    int srcId = VR_MUIFA_SOURCE_INVALID;
    parser.GetNodeText("//params/source_ID", srcId);

    parser.Reset();
    parser.LoadString(VR_DATA_MEDIA_RESPDEVICEPLAYINFO);
    parser.SetNodeText("//action_result/action_id", id.c_str());
    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());

    int targetSrcId = VR_MUIFA_SOURCE_INVALID;
    if (VR_MUIFA_SOURCE_U_DISK == srcId
        || VR_MUIFA_SOURCE_IPOD == srcId
        || VR_MUIFA_SOURCE_BT_AUDIO == srcId) {
        targetSrcId = srcId;
    }
    else {
        res = GetCurrentMediaSource(targetSrcId);
        if (!res) {
            targetSrcId = VR_MUIFA_SOURCE_INVALID;
        }
    }

    stPlayInfo info;
    if (targetSrcId == VR_MUIFA_SOURCE_IPOD) {
        res = GetIPodPlayInfo(info);
    }
    else if (targetSrcId == VR_MUIFA_SOURCE_U_DISK) {
        res = GetUsbPlayInfo(info);
    }
    else if (targetSrcId == VR_MUIFA_SOURCE_BT_AUDIO) {
        res = GetBTAPlayInfo(info);
    }

    if (!res) {
        parser.SetNodeText("//action_result/error_no", VR_MEDIA_RESULT_OK);
        parser.AddNode("//action_result", "device_state", VR_MEDIA_NOTPLAYING_STATE);
        parser.AddNode("//action_result", "song_result", VR_MEDIA_RESULT_NOT_FOUND);
        if (info.IsEnable) {
            parser.AddNode("//action_result", "is_source_available", "true");
        }
        else {
            parser.AddNode("//action_result", "is_source_available", "false");
        }
    }
    else {
        parser.SetNodeText("//action_result/error_no", VR_MEDIA_RESULT_OK);
        parser.AddNode("//action_result", "output_list", "");
        if (info.HasArtist) {
            parser.AddNode("//action_result/output_list", "output", info.Artist.c_str());
        }
        if (info.HasSong) {
            parser.AddNode("//action_result/output_list", "output", info.Song.c_str());
        }

        if (info.IsPlaying) {
            parser.AddNode("//action_result", "device_state", VR_MEDIA_PLAYING_STATE);
        }
        else {
            parser.AddNode("//action_result", "device_state", VR_MEDIA_NOTPLAYING_STATE);
        }

        if (info.HasArtist && info.HasSong) {
            parser.AddNode("//action_result", "song_result", VR_MEDIA_RESULT_BOTH_FOUND);
        }
        else if (info.HasArtist && !info.HasSong) {
            parser.AddNode("//action_result", "song_result", VR_MEDIA_RESULT_ARTIST_FND);
        }
        else if (!info.HasArtist && info.HasSong) {
            parser.AddNode("//action_result", "song_result", VR_MEDIA_RESULT_SONG_FND);
        }
        else {
            parser.AddNode("//action_result", "song_result", VR_MEDIA_RESULT_NOT_FOUND);
        }

        if (!info.GenreList.empty()) {
            parser.AddNode("//action_result", "type_of_song_in_favorit_list", "");
        }

        for (auto genre : info.GenreList) {
            if ("" == genre) {
                continue;
            }
            parser.AddNode("//type_of_song_in_favorit_list", "type_of_song_in_favorit", genre);
        }

        if (info.IsEnable) {
            parser.AddNode("//action_result", "is_source_available", "true");
        }
        else {
            parser.AddNode("//action_result", "is_source_available", "false");
        }
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("MESG: %s", mesg.c_str());
    SendMesgNATP(mesg);
    return true;
}

bool VR_DataProvider::OnReplyMusicGetDBData(VR_String& id, VR_String &srcId)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    parser.LoadString(VR_DATA_MEDIA_RESPMEDIADBDATAINFO);
    parser.SetNodeText("//action_result/action_id", id);

    parser.SetNodeText("//action_result/id", VR_IdGenerator().GetID());
    bool res = false;
    do {
        VR_MusicDBHelperManager* pManager = VR_MusicDBHelperManager::GetInstance();
        if (nullptr == pManager) {
            res = false;
            break;
        }

        if ("1" == srcId) {
            bool conn = VR_ConfigureIF::Instance()->getMediaConnectStatus(usb_connect);
            if (!conn) {
                res = false;
                break;
            }
            parser.SetNodeText("//music_data/source_id", VR_MUIFA_SOURCE_U_DISK);
            VR_String path = pManager->GetEngineDBPath(VR_MUIFA_SOURCE_U_DISK);
            if ("" == path) {
                res = false;
                break;
            }
            parser.SetNodeText("//music_data/data_path", path);
        }
        else if ("2" == srcId) {
            bool conn = VR_ConfigureIF::Instance()->getMediaConnectStatus(ipod_connect);
            if (!conn) {
                res = false;
                break;
            }

            parser.SetNodeText("//music_data/source_id", VR_MUIFA_SOURCE_IPOD);
            VR_String path = pManager->GetEngineDBPath(VR_MUIFA_SOURCE_IPOD);
            if ("" == path) {
                res = false;
                break;
            }

            parser.SetNodeText("//music_data/data_path", path);
        }
        else {
            VR_LOGD("srcid unknown. %s", srcId.c_str());
            res = false;
            break;
        }

        parser.SetNodeText("//error_no", VR_MEDIA_RESULT_OK);
        res = true;
    } while (0);

    if (!res) {
        parser.SetNodeText("//error_no", VR_MEDIA_RESULT_OK);
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("mesg: %s", mesg.c_str());

    SendMesgNATP(mesg);
    return true;
}

bool VR_DataProvider::OnReplyPhoneDBData(const VR_String& strId)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_DATA_PHONE_RESPDBDATA);
    if (!res) {
        VR_LOGD("xml invalid");
        return false;
    }

    do {
        res = parser.SetNodeText("//action_result/id", strId.c_str());
        if (!res) {
            VR_LOGD("set id error");
            break;
        }

        res = parser.SetNodeText("//action_result/action_id", VR_IdGenerator().GetID());
        if (!res) {
            VR_LOGD("set action_id error");
            break;
        }

        res = parser.SetNodeText("//action_result/error_no", VR_PHONE_RESULT_OK);
        if (!res) {
            VR_LOGD("set error_no error");
            break;
        }

        VR_String strDeviceID;
        res = GetPhoneDataId(strDeviceID);
        if (!res) {
            VR_LOGD("get Device id error");
            break;
        }

        res = parser.SetNodeText("//action_result/phone_data/device_id", strDeviceID);
        if (!res) {
            VR_LOGD("set device_id error");
            break;
        }

        VR_String strDBPath;
        res = GetPhoneDBPath(strDBPath);
        if (!res) {
            VR_LOGD("get DB path error");
            break;
        }

        res = parser.SetNodeText("//action_result/phone_data/data_path", strDBPath);
        if (!res) {
            VR_LOGD("set data_path error");
            break;
        }

    } while (0);

    if (!res) {
        parser.SetNodeText("//action_result/error_no", VR_PHONE_RESULT_ERROR);
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("MESG: %s", mesg.c_str());
    SendMesgNATP(mesg);
    return true;
}

bool VR_DataProvider::OnSetSMSSupportState(const VR_String& strMsg)
{
    VR_PhoneBookStateListener* pListener = VR_PhoneBookStateListener::GetInstance();
    if (!pListener) {
        return false;
    }
    VR_XmlStrParse parser;
    bool res = parser.LoadString(strMsg);
    if (!res) {
        return false;
    }

    VR_String strValue;

    res = parser.GetAttributeValue("//connectstatus", "value", strValue);
    if (!res) {
        return false;
    }

    if (!pListener->setSmsSupport(strValue)) {
        return false;
    }

    return true;
}

bool VR_DataProvider::GetPhoneDataId(VR_String& strDevice)
{
    VR_PhoneBookStateListener* pListener = VR_PhoneBookStateListener::GetInstance();
    if (!pListener) {
        return false;
    }

    if (!pListener->getDeviceId(strDevice)) {
        return false;
    }

    return true;
}

bool VR_DataProvider::GetPhoneDBPath(VR_String& strPath)
{
    VR_PhoneBookListener* pListener = VR_PhoneBookListener::GetInstance();
    if (!pListener) {
        return false;
    }

    if (!pListener->getDBPath(strPath)) {
        return false;
    }
    return true;
}

bool VR_DataProvider::OnReplyPhoneStatus(const VR_String& strId)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_DATA_PHONE_RESPSTATUS);
    if (!res) {
        VR_LOGD("xml invalid");
        return false;
    }

    do {
        res = parser.SetNodeText("//action_result/id", strId.c_str());
        if (!res) {
            VR_LOGD("set id error");
            break;
        }

        uint32_t actId = VR_IdGenerator().GetID();
        res = parser.SetNodeText("//action_result/action_id", actId);
        if (!res) {
            VR_LOGD("set action_id error");
            break;
        }

        res = parser.SetNodeText("//action_result/error_no", VR_PHONE_RESULT_OK);
        if (!res) {
            VR_LOGD("set error_no error");
            break;
        }

        VR_String strPhoneConnect;
        res = GetPhoneStatus(strPhoneConnect);
        if (!res) {
            VR_LOGD("get phone_status error");
            break;
        }

        res = parser.SetNodeText("//action_result/phone_status", strPhoneConnect);
        if (!res) {
            VR_LOGD("set phone_status error");
            break;
        }

        VR_String strSignalStatus;
        res = GetPhoneSignalStatus(strSignalStatus);
        if (!res) {
            VR_LOGD("get phone_network_status error");
            break;
        }

        res = parser.SetNodeText("//action_result/phone_network_status", strSignalStatus);
        if (!res) {
            VR_LOGD("set phone_network_status error");
            break;
        }

        VR_String strDeviceID;
        res = GetPhoneDataId(strDeviceID);
        if (!res) {
            VR_LOGD("get Device id error");
            break;
        }

        res = parser.SetNodeText("//action_result/device_id", strDeviceID);
        if (!res) {
            VR_LOGD("set device_id error");
            break;
        }
    } while (0);

    if (!res) {
        parser.SetNodeText("//action_result/error_no", VR_PHONE_RESULT_ERROR);
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("MESG: %s", mesg.c_str());
    SendMesgNATP(mesg);
    return true;
}

bool VR_DataProvider::OnReplyPhoneNumberGet(const VR_String& strId,
    const VR_String& strContactId, const VR_String& strName)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_DATA_PHONE_RESPNUMBERGET);
    if (!res) {
        VR_LOGD("xml invalid");
        return false;
    }

    do {
        res = parser.SetNodeText("//action_result/id", strId.c_str());
        if (!res) {
            VR_LOGD("set id error");
            break;
        }

        res = parser.SetNodeText("//action_result/action_id", VR_IdGenerator().GetID());
        if (!res) {
            VR_LOGD("set action_id error");
            break;
        }

        res = parser.SetNodeText("//action_result/error_no", VR_PHONE_RESULT_OK);
        if (!res) {
            VR_LOGD("set error_no error");
            break;
        }

        myPersonInfo personInfo;
        res = GetPhoneNumber(strContactId, personInfo);
        if (!res) {
            VR_LOGD("get phone number error");
            break;
        }

        res = parser.SetNodeText("//action_result/phone_contact_name", strName.c_str());
        if (!res) {
            VR_LOGD("set error_no error");
            break;
        }

        VR_XmlStrParse parserContactInfo;
        VR_String strPhoneType;
        VR_String strContactInfo;
        for (auto it = personInfo.lstPhoneNum.cbegin(); it != personInfo.lstPhoneNum.cend(); ++it) {
            res = parserContactInfo.LoadString(VR_DATA_PHONE_CONTACT_INFO);
            if (!res) {
                VR_LOGD("set contact_info error");
                break;
            }

            res = parserContactInfo.SetNodeText("//contact_info/phone_number", it->number.getString());
            if (!res) {
                VR_LOGD("set phone_number error");
                break;
            }

            res = GetPhoneType(it->type, strPhoneType);
            if (!res) {
                VR_LOGD("get phone_type error");
                break;
            }

            res = parserContactInfo.SetNodeText("//contact_info/phone_type", strPhoneType);
            if (!res) {
                VR_LOGD("set phone_type error");
                break;
            }

            parserContactInfo.GetXmlString(strContactInfo);

            res = parser.AddNode("//action_result/contact_info_list", strContactInfo);
            if (!res) {
                VR_LOGD("add contact_info_list error");
                break;
            }
            parserContactInfo.Reset();
        }
    } while (0);

    if (!res) {
        parser.SetNodeText("//action_result/error_no", VR_PHONE_RESULT_ERROR);
    }

    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("MESG: %s", mesg.c_str());
    SendMesgNATP(mesg);
    return true;
}

bool VR_DataProvider::NotifyFinishedData()
{
    VR_LOGD_FUNC();
    // phone
    VR_PhoneBookListener* pListener = VR_PhoneBookListener::GetInstance();
    if (pListener) {
        pListener->notifyPhoneFinishedData();
    }

    NCUri uri = NCString(VR_DATAPROVIDER_INITPERSONDATA_URI);
    NCVariant var = NCVariant(0);

    spVR_DP_OB_Action spMusicAction = spVR_DP_OB_Action_Music(VR_new VR_DP_OB_Action_Music(uri, var));

    m_spDPThread->onUpdataPreare(spMusicAction);
    return true;
}

bool VR_DataProvider::NotifyCheckUDiskSuccess()      // check udisk data status when service start time
{
    VR_LOGD_FUNC();
    NCUri uri;
    NCVariant variant;
    variant.clear();

    uri = NCString(VR_MUSIC_USB_SCANN_FINISHED);
    variant.setValue(NCString("complete"));

    spVR_DP_OB_Action spMusicAction = spVR_DP_OB_Action_Music(VR_new VR_DP_OB_Action_Music(uri, variant));

    m_spDPThread->onUpdataPreare(spMusicAction);
    return true;
}

bool VR_DataProvider::GetPhoneType(int iType, VR_String& strPhoneType)
{
    switch (iType) {
    case 0: {
        strPhoneType = VR_PHONE_NUMBER_MOBILE;
        break;
    }
    case 1: {
        strPhoneType = VR_PHONE_NUMBER_HOME;
        break;
    }
    case 2: {
        strPhoneType = VR_PHONE_NUMBER_OFFICE;
        break;
    }
    case 3: {
        strPhoneType = VR_PHONE_NUMBER_OTHER;
        break;
    }
    default : {
        strPhoneType = "";
        return false;
    }
    }
    return true;
}

bool VR_DataProvider::GetCurrentMediaSource(int& srcId)
{
    VR_LOGD_FUNC();
    MediaInfo_CurSrc Source;
    memset(&Source, 0, sizeof(Source));
    bool res = NMMediaInfoHelper::load(MediaInfoKey_CurSrc, &Source, sizeof(Source));
    if (!res) {
        VR_LOGD("query current source failed");
        return false;
    }

    stPlayInfo info;

    if (MediaInfo_SrcID_USB1 == Source.curSrc) {
        MediaInfo_UsbDeviceType usbType;
        memset(&usbType, 0x00, sizeof(MediaInfo_UsbDeviceType));
        res = NMMediaInfoHelper::load(MediaInfoKey_UsbType, &usbType, sizeof(usbType));
        if (!res) {
            VR_LOGD("query usb type failed");
            return false;
        }

        if (MediaInfo_UsbKind_Ipod == usbType.usb1) {
            srcId = VR_MUIFA_SOURCE_IPOD;
            return true;
        }
    }

    if (MediaInfo_SrcID_MediaShare == Source.curSrc) {
        srcId = VR_MUIFA_SOURCE_U_DISK;
        return true;
    }

    if (MediaInfo_SrcID_BTA == Source.curSrc) {
        srcId = VR_MUIFA_SOURCE_BT_AUDIO;
        return true;
    }

    srcId = VR_MUIFA_SOURCE_INVALID;
    return false;
}

bool VR_DataProvider::GetPhoneNumber(const VR_String& strContactId, myPersonInfo& personInfo)
{
    VR_PhoneBookListener* pListener = VR_PhoneBookListener::GetInstance();
    if (!pListener) {
        return false;
    }

    int iContactId = atoi(strContactId.c_str());
    if (!pListener->getPhoneNumber(iContactId, personInfo)) {
        return false;
    }

    if (iContactId != personInfo.contactId) {
        VR_LOGD("iContactId is different! [%d] != [%d]", iContactId, personInfo.contactId);
        return false;
    }
    return true;
}

bool VR_DataProvider::GetPhoneStatus(VR_String& strStatus)
{
    VR_PhoneBookStateListener* pListener = VR_PhoneBookStateListener::GetInstance();
    if (!pListener) {
        return false;
    }

    if (!pListener->getPhoneStatus(strStatus)) {
        return false;
    }
    return true;
}

bool VR_DataProvider::GetPhoneSignalStatus(VR_String& strStatus)
{
    VR_PhoneBookStateListener* pListener = VR_PhoneBookStateListener::GetInstance();
    if (!pListener) {
        return false;
    }

    if (!pListener->getSignalStatus(strStatus)) {
        return false;
    }
    return true;
}

bool VR_DataProvider::GetUsbPlayInfo(stPlayInfo& info)
{
    VR_LOGD_FUNC();
    MediaInfo_UsbPlayInfo playState;
    memset(&playState, 0, sizeof(playState));

    info.IsEnable = VR_ConfigureIF::Instance()->getMediaConnectStatus(usb_connect);
    bool result = NMMediaInfoHelper::load(MediaInfoKey_UsbPlayInfo, &playState, sizeof(playState));
    if (!result) {
        VR_LOGD("load state failed");
        return false;
    }

    if (MediaInfo_UsbPlayInfo::PlayState_Play == playState.state) {
        info.IsPlaying = true;
    }
    else {
        info.IsPlaying = false;
    }

    MediaInfo_Usb usbPlayInfo;
    memset(&usbPlayInfo, 0, sizeof(usbPlayInfo));
    result = NMMediaInfoHelper::load(MediaInfoKey_Usb, &usbPlayInfo, sizeof(usbPlayInfo));
    if (!result) {
        VR_LOGD("load info failed");
        return false;
    }

    if (usbPlayInfo.artistId >= 0) {
        info.HasArtist = true;
        info.Artist.assign(reinterpret_cast<char*>(&usbPlayInfo.artistName[0]), TITLE_LEN_256);
    }

    if (usbPlayInfo.songId >= 0) {
        info.HasSong = true;
        info.Song.assign(reinterpret_cast<char*>(&usbPlayInfo.fileName[0]), TITLE_LEN_256);
    }

    int srcId = VR_MUIFA_SOURCE_INVALID;
    result = GetCurrentMediaSource(srcId);
    if (!result) {
        VR_LOGD("error");
        return false;
    }
    // pManager->GetFavoriableGenreList(info.GenreList);

    return true;
}

bool VR_DataProvider::GetIPodPlayInfo(stPlayInfo& info)
{
    VR_LOGD_FUNC();
    MediaInfo_iPodPlayInfo playState;
    memset(&playState, 0, sizeof(playState));

    info.IsEnable = VR_ConfigureIF::Instance()->getMediaConnectStatus(ipod_connect);
    bool result = NMMediaInfoHelper::load(MediaInfoKey_iPodPlayInfo, &playState, sizeof(playState));
    if (!result) {
        VR_LOGD("load state failed");
        return false;
    }

    if (MediaInfo_iPodPlayInfo::PlayState_Play == playState.state) {
        info.IsPlaying = true;
    }
    else {
        info.IsPlaying = false;
    }

    MediaInfo_iPod iPodPlayInfo;
    memset(&iPodPlayInfo, 0, sizeof(iPodPlayInfo));
    result = NMMediaInfoHelper::load(MediaInfoKey_iPod, &iPodPlayInfo, sizeof(iPodPlayInfo));
    if (!result) {
        VR_LOGD("load info failed");
        return false;
    }

    if (iPodPlayInfo.artistId >= 0) {
        info.HasArtist = true;
        info.Artist.assign(reinterpret_cast<const char*>(&iPodPlayInfo.artistName[0]), TITLE_LEN_255);
    }

    if (iPodPlayInfo.songId >= 0) {
        info.HasSong = true;
        info.Song.assign(reinterpret_cast<const char*>(&iPodPlayInfo.trackName[0]), TITLE_LEN_255);
    }

    return true;
}

bool VR_DataProvider::GetBTAPlayInfo(stPlayInfo& info)
{
    VR_LOGD_FUNC();
    MediaInfo_BtAudio playState;
    memset(&playState, 0, sizeof(playState));

    info.IsEnable = VR_ConfigureIF::Instance()->getMediaConnectStatus(btaudio_connect);
    bool result = NMMediaInfoHelper::load(MediaInfoKey_BtAudio, &playState, sizeof(playState));
    if (!result) {
        VR_LOGD("load failed");
        return false;
    }

    if (MediaInfo_BTAError_NoError != playState.error) {
        VR_LOGD("BTA error code: %d", playState.error);
        return false;
    }

    if (MediaInfo_BTAPlayStatus_Play == playState.playStatus) {
        info.IsPlaying = true;
    }
    else {
        info.IsPlaying = false;
    }

    VR_String artist;
    artist.assign(reinterpret_cast<char*>(&playState.albumName[0]), TITLE_LEN_255);

    VR_String song;
    artist.assign(reinterpret_cast<char*>(&playState.trackName[0]), TITLE_LEN_255);

    info.HasArtist = (artist != "") ? true : false;
    info.Artist = artist;

    info.HasSong = (song != "") ? true : false;
    info.Song = song;

    return true;
}

bool VR_DataProvider::GetOptionSourceState(const VR_String& key, VR_String& srcStateXml, VR_String& state)
{
    VR_AudioSourceListener* pListener = VR_AudioSourceListener::GetInstance();
    if (NULL == pListener) {
        VR_LOGD("listener null");
        return false;
    }

    VR_String xml;
    bool res = pListener->GetSourceStatus(xml);
    if (!res) {
        VR_LOGD("get status failed");
        return false;
    }

    VR_XmlStrParse originParser;
    originParser.LoadString(xml);

    VR_String text;
    if ("usb" == key) {
        originParser.GetNodeBlock("//music_source[source_id = 1]", text);
        originParser.GetNodeText("//music_source[source_id = 1]/connected_status", state);
    }
    else if ("ipod" == key) {
        originParser.GetNodeBlock("//music_source[source_id = 2]", text);
        originParser.GetNodeText("//music_source[source_id = 2]/connected_status", state);
    }
    else if ("btaudio" == key) {
        originParser.GetNodeBlock("//music_source[source_id = 3]", text);
        originParser.GetNodeText("//music_source[source_id = 3]/connected_status", state);
    }
    else if ("all" == key) {
        text = xml;
        return true;
    }
    else {
        return false;
    }

    VR_XmlStrParse parser;
    parser.LoadString("<music_sources></music_sources>");
    parser.AddNode("//music_sources", text);

    parser.GetXmlString(srcStateXml);
    return true;
}

}
}
/* EOF */
