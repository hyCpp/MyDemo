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
#include "VR_XmlStrParse.h"
#include "VR_MusicListener.h"
#include "VR_DataProviderComm.h"

#include "ncore/NCUri.h"
#include "ncore/NCFile.h"
#include "MediaProvider/NMMediaUri.h"
#include "MediaInfoProvider/NMMediaInfoDEF.h"
#include "MediaInfoProvider/NMMediaInfoUri.h"
#include "contentprovider/NCContentProvider.h"
#include "contentresolver/NCContentResolver.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"

using namespace std;
using namespace nutshell;
using namespace nutshell::media;

namespace navi
{
namespace dataprovider
{

VR_MusicListener* VR_MusicListener::m_instance = nullptr;

VR_MusicListener::VR_MusicListener()
    : m_dbManager(nullptr)
{
    VR_LOGD_FUNC();
    m_dbManager = VR_MusicDBHelperManager::GetInstance();
    if (nullptr == m_dbManager) {
        VR_LOGD("get db manager failed");
    }

    Initialize();
}

VR_MusicListener::~VR_MusicListener()
{
    VR_LOGD_FUNC();
    VR_MusicDBHelperManager::Destory();
}

VR_MusicListener* VR_MusicListener::GetInstance()
{
    if (nullptr == m_instance) {
        m_instance = VR_new VR_MusicListener;
    }
    return m_instance;
}

void VR_MusicListener::Destory()
{
    VR_LOGD_FUNC();
    delete m_instance;
    m_instance = nullptr;
}

bool VR_MusicListener::Initialize()
{
    VR_LOGD_FUNC();
    NCUri uri;
    NCVariant variant;
    variant.clear();

    uri = NMMediaInfoUri::getKeyContentUri(MediaInfoKey_iPod1DBStatus);
    variant.setValue(INT(MediaInfoKey_iPod1DBStatus));
    onNotifyUpdate(uri, variant);
    return true;
}

bool VR_MusicListener::onNotifyUpdate(NCUri &uri, NCVariant &variant)
{
    VR_LOGD_FUNC();
    INT32 IsForward = HookUSBDataUpdate(uri, variant);
    if (MUSIC_NOTIFY_MSG_DROP == IsForward) {
        VR_LOGD("uri:%s hook usb data update", uri.toString().getString());
        return true;
    }

    IsForward = HookIpodDataUpdate(uri, variant);
    if (MUSIC_NOTIFY_MSG_DROP == IsForward) {
        VR_LOGD("uri:%s hook ipod data update", uri.toString().getString());
        return true;
    }

    IsForward = HookInitMusicData(uri, variant);
    if (MUSIC_NOTIFY_MSG_DROP == IsForward) {
        VR_LOGD("uri:%s hook init music data update", uri.toString().getString());
        return true;
    }

    VR_LOGD("unkown uri:%s", uri.toString().getString());
    return false;
}

INT32 VR_MusicListener::HookUSBDataUpdate(NCUri& uri, NCVariant& variant)
{
    VR_LOGD_FUNC();
    bool res = uri.equals(NCString(VR_MUSIC_USB_SCANN_FINISHED));
    if (!res) {
        return MUSIC_NOTIFY_MSG_FORWARD;
    }

    VR_String type(variant.typeName());
    if (std::string::npos == type.find("NCString")) {
        VR_LOGD("uncare state");
        m_dbManager->ClearEngineDBPath(VR_MUIFA_SOURCE_U_DISK);
        return MUSIC_NOTIFY_MSG_DROP;
    }

    VR_LOGD("Hook usbs data update. uri:%s", uri.toString().getString());

    VR_String status;
    variant.value(status);
    if ("complete" != status) {
        VR_LOGD("status unready");
        return MUSIC_NOTIFY_MSG_DROP;
    }

    VR_String origin = "/var/user/media/NMMediaDataProvider.db";
    VR_String target = "/tmp/voice_media_tmp.db";
    res = BackupMediaDB(origin, target);
    if (!res) {
        VR_LOGD("back up target failed");
        return MUSIC_NOTIFY_MSG_DROP;
    }

    MusicDBHelperItem item;
    item.DeviceItem.DevName = "usb";
    item.DeviceItem.DevDriverType = MUSIC_DRIVER_TYPE_USB_GENERAL;
    item.DeviceItem.DevType = MUSIC_DEVICE_TYPE_USB;
    item.DBItem.Path = target;
    item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_UNKOWN;

    res = m_dbManager->OnNotifyUpdate(item);
    if (!res) {
        VR_LOGD("update db failed");
    }

    return MUSIC_NOTIFY_MSG_DROP;
}

INT32 VR_MusicListener::HookIpodDataUpdate(NCUri &uri, NCVariant &variant)
{
    VR_LOGD_FUNC();
    bool res = uri.equals(NMMediaInfoUri::getKeyContentUri(MediaInfoKey_iPod1DBStatus));
    if (!res) {
        return MUSIC_NOTIFY_MSG_FORWARD;
    }

    VR_LOGD("Hook ipod data update. uri:%s", uri.toString().getString());
    int status = 0;
    MusicDriverType devType = MUSIC_DRIVER_TYPE_UNKOWN;
    VR_String dbName;
    res = GetIpodDeviceInfo(status, devType, dbName);
    if (!res) {
        VR_LOGD("get ipod info failed");
        return MUSIC_NOTIFY_MSG_DROP;
    }

    if (0 == status) {
        m_dbManager->ClearEngineDBPath(VR_MUIFA_SOURCE_IPOD);
        VR_LOGD("ipod db status disable");
        return MUSIC_NOTIFY_MSG_DROP;
    }

    VR_String origin = m_dbManager->GetMediaDBPath() + "/" + dbName.c_str() + ".db";
    VR_String target = "/tmp/voice_media_tmp.db";
    res = BackupMediaDB(origin, target);
    if (!res) {
        VR_LOGD("back up target failed");
        return MUSIC_NOTIFY_MSG_DROP;
    }

    MusicDBHelperItem item;
    item.DeviceItem.DevName = "ipod";
    item.DeviceItem.DevDriverType = devType;
    item.DeviceItem.DevType = MUSIC_DEVICE_TYPE_IPOD;
    item.DBItem.Path = target;
    item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_UNKOWN;

    res = m_dbManager->OnNotifyUpdate(item);
    if (!res) {
        VR_LOGD("update db failed");
    }

    return MUSIC_NOTIFY_MSG_DROP;
}

INT32 VR_MusicListener::HookInitMusicData(NCUri& uri, NCVariant& variant)
{
    VR_LOGD_FUNC();
    bool res = uri.equals(NCString(VR_DATAPROVIDER_INITPERSONDATA_URI));
    if (!res) {
        return MUSIC_NOTIFY_MSG_FORWARD;
    }

    VR_LOGD("Hook init music. uri:%s", uri.toString().getString());

    Initialize();

    return MUSIC_NOTIFY_MSG_DROP;
}

bool VR_MusicListener::GetIpodDeviceInfo(int& deviceStatus, MusicDriverType& deviceDriver, VR_String &name)
{
    VR_LOGD_FUNC();
    MediaInfo_iPodDBStatus info;
    memset(&info, 0, sizeof(info));

    int mediaInfoKey = MediaInfoKey_iPod1DBStatus;
    bool res = NMMediaInfoHelper::load(mediaInfoKey, &info, sizeof(info));
    if (!res) {
        VR_LOGD("query failed");
        return false;
    }

    name.assign(reinterpret_cast<char *>(&info.dbName[0]), 50);
    deviceStatus = (INT)info.isEnable;
    deviceDriver = info.isIAP1 ? MUSIC_DRIVER_TYPE_IPOD_IAP1 :
                                       MUSIC_DRIVER_TYPE_IPOD_IAP2;

    VR_LOGD("Device: %d, Status: %d", deviceDriver, deviceStatus);
    return true;
}

bool VR_MusicListener::GetUSBDeviceInfo(bool& deviceStatus)
{
    VR_LOGD_FUNC();

    NCUri uri(NCString(VR_MUSIC_USB_SCANN_FINISHED));
    NCObjList<NCString> list;
    list << new NCString("insertComplete");
    NCList<NCString> nullList;
    NCContentResolver resolver;
    NCCursor* cursor = resolver.query(uri, list, NCString(), nullList, NCString());
    if (nullptr == cursor) {
        VR_LOGD("query failed");
        return false;
    }


    if (1 != cursor->getCount()) {
        VR_LOGD("query not 1 count");
        delete cursor;
        cursor = nullptr;
        return false;
    }

    cursor->moveToFirst();

    NCString conn = cursor->getString(0);
    deviceStatus = "complete" == conn ? true : false;

    delete cursor;
    cursor = nullptr;
    return true;
}

bool VR_MusicListener::BackupMediaDB(VR_String& origin, VR_String& target)
{
    VR_LOGD_FUNC();

    bool res = NCFile::exists(origin.c_str());
    if (!res) {
        VR_LOGD("origin file not exist. file:%s", origin.c_str());
        return false;
    }

    res = NCFile::exists(target.c_str());
    if (res) {
        NCFile::remove(target.c_str());
        VR_LOGD("remove file. file:%s", target.c_str());
    }

    res = NCFile::copy(origin.c_str(), target.c_str());
    if (!res) {
        VR_LOGD("copy file failed. origin: %s, target: %s", origin.c_str(), target.c_str());
        return false;
    }
    return true;
}

}
}
/* EOF */
