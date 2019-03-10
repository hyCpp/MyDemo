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
#include "VR_Indentifier.hpp"
#include "VR_DataProviderComm.h"
#include "VR_MusicDBHelperManager.h"

#include "BL_File.h"
#include "ncore/NCDir.h"
#include "ncore/NCFile.h"
#include "ncore/NCTime.h"
#include "ncore/NCAutoSync.h"

#include "EV_Define.h"
#include "EV_EventSender.h"
#include "dbhelper/NCSQLiteDatabase.h"
#include "navi/Voice/VoiceRecog/VrNotifyiAutoProxy.pb.h"

using namespace nutshell;
using namespace media;
namespace navi
{
namespace dataprovider
{
static nutshell::NCSyncObj g_cSyncObj;

VR_MusicDBHelperManager* VR_MusicDBHelperManager::m_instance = nullptr;

VR_MusicDBHelperManager::VR_MusicDBHelperManager()
{
    VR_LOGD_FUNC();
    Initialize();
}

VR_MusicDBHelperManager::~VR_MusicDBHelperManager()
{
    VR_LOGD_FUNC();
}

VR_MusicDBHelperManager* VR_MusicDBHelperManager::GetInstance()
{
    nutshell::NCAutoSync lock(g_cSyncObj);
    if (nullptr == m_instance) {
        m_instance = new VR_MusicDBHelperManager;
    }

    return m_instance;
}

void VR_MusicDBHelperManager::Destory()
{
    VR_LOGD_FUNC();
    delete m_instance;
    m_instance = NULL;
}

VR_String VR_MusicDBHelperManager::GetMediaDBPath()
{
    VR_LOGD_FUNC();
    return m_mediaDir;
}

bool VR_MusicDBHelperManager::OnNotifyUpdate(tagMusicDBHelperItem& item)
{
    VR_LOGD_FUNC();
    bool res = SyncMediaDatabase(item);
    if (!res) {
        VR_LOGD("sync media database failed");
        return false;
    }
    return true;
}

bool VR_MusicDBHelperManager::Initialize()
{
    VR_LOGD_FUNC();
    bool res = InitWorkDirectory();

    return res;
}

bool VR_MusicDBHelperManager::InitWorkDirectory()
{
    VR_LOGD_FUNC();
    BL_Dir Dir(BL_FILE_PREFIX_RW);
    bool res = Dir.MakeDir(VR_MUSIC_DATA_PATH, CL_FALSE);
    if (!res) {
        VR_LOGD("mkdir failed");
        return false;
    }

    BL_String abs_path;
    res = Dir.GetLogicalPath(VR_MUSIC_DATA_PATH, abs_path);
    if (!res) {
        VR_LOGD("get path failed");
        return false;
    }

    VR_LOGD("init work path: %s", abs_path.GetString());
    m_voiceDir.assign(abs_path.GetString());
    m_mediaDir = "/var/user/media";
    return true;
}

void VR_MusicDBHelperManager::SetEngineDBPath(VR_String& dbPath, int type)
{
    VR_LOGD_FUNC();
    nutshell::NCAutoSync lock(g_cSyncObj);
    if (VR_MUIFA_SOURCE_U_DISK == type) {
        m_engineDBPath_usb = dbPath;
    }
    else if (VR_MUIFA_SOURCE_IPOD == type) {
        m_engineDBPath_ipod = dbPath;
    }
}

void VR_MusicDBHelperManager::sendMessageToNATP(VR_String& mesg)
{
    using namespace navi::VoiceRecog;
    std::unique_ptr<VrNotifyiAutoProxy> protoMsg(VrNotifyiAutoProxy::default_instance().New());
    protoMsg->set_sender("vr_s");
    protoMsg->set_funccode(VrNotifyiAutoProxy_VrFunc_NotifyRecogMessage);
    protoMsg->mutable_recogmsg()->set_xml_out(mesg.c_str());
    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFIAUTOPROXY, protoMsg.release());
}

bool VR_MusicDBHelperManager::SyncMediaDatabase(MusicDBHelperItem& item)
{
    VR_LOGD_FUNC();
    spVR_MusicDBHelper device_db(VR_MusicDBHelper::CreateDBHelper(item));
    if (!device_db) {
        VR_LOGD("create object origin failed");
        return false;
    }

    INT32 srcId = VR_MUIFA_SOURCE_INVALID;
    if (MUSIC_DEVICE_TYPE_IPOD == device_db->m_Info.DeviceItem.DevType) {
        srcId = VR_MUIFA_SOURCE_IPOD;
    }
    else if (MUSIC_DEVICE_TYPE_USB == device_db->m_Info.DeviceItem.DevType) {
        srcId = VR_MUIFA_SOURCE_U_DISK;
    }
    else {
        VR_LOGD("unknown src id. %d", srcId);
        return false;
    }

    MusicDBHelperItem engine_item;
    engine_item.DeviceItem.DevDriverType = MUSIC_DRIVER_TYPE_ENGINE;
    engine_item.DeviceItem.DevName = "engine";
    engine_item.DeviceItem.DevType = MUSIC_DEVICE_TYPE_ENGINE;

    VR_String path;
    bool res = GenerateVoiceDBPath(srcId, path);
    if (!res) {
        VR_LOGD("create path failed");
        return false;
    }

    engine_item.DBItem.Path = path;
    engine_item.DBItem.SyncStatus = MUSIC_DB_DATA_SYNC_STATUS_UNKOWN;

    spVR_MusicDBHelper engine_db(VR_MusicDBHelper::CreateDBHelper(engine_item));
    if (!engine_db) {
        VR_LOGD("create object engine failed");
        return false;
    }

    res = device_db->SyncMusicDB(engine_db.get());
    if (!res) {
        VR_LOGD("sync music database failed");
        return false;
    }

    // update engine database path
    SetEngineDBPath(path, srcId);

    VR_XmlStrParse parser;
    res = parser.LoadString(VR_MEDIA_DATABASE_XML);
    if (!res) {
        VR_LOGD("load error");
        return false;
    }

    parser.SetNodeText("//id", VR_IdGenerator().GetID());
    parser.SetNodeText("//music_data/source_id", srcId);
    parser.SetNodeText("//music_data/data_path", engine_db->m_Info.DBItem.Path);

    VR_String mesg;
    parser.GetXmlString(mesg);
    VR_LOGD("Send Message: %s", mesg.c_str());
    sendMessageToNATP(mesg);

    return res;
}

bool VR_MusicDBHelperManager::GenerateVoiceDBPath(int sourceId, VR_String &dbPath)
{
    VR_LOGD_FUNC();

    dbPath = m_voiceDir + "/VUIMuisc_";
    if (VR_MUIFA_SOURCE_IPOD == sourceId) {
        dbPath = dbPath + "IPOD_";
    }
    else if (VR_MUIFA_SOURCE_U_DISK == sourceId) {
        dbPath = dbPath + "USB_";
    }
    else {
        VR_LOGD("error failed. %d", sourceId);
        return false;
    }

    NCTime time;
    INT64 ticks = time.getTickCount();

    VR_String suffix = std::to_string(ticks);
    dbPath = dbPath + suffix.substr(0, 6) + ".db";
    VR_LOGD("dbPath: %s", dbPath.c_str());
    return true;
}

bool VR_MusicDBHelperManager::GetFavoriableGenreList(std::list<VR_String>& list)
{
    VR_LOGD_FUNC();
    VR_String usbPath = "/var/user/media/NMMediaDataProvider.db";

    NCSQLiteDatabase dbAccess(usbPath.c_str(), OPEN_READONLY);
    bool res = dbAccess.isOpen();
    if (!res) {
        VR_LOGD("open db failed. %s", usbPath.c_str());
        return res;
    }

    NCString sql = "SELECT NAME FROM TBL_GENRES WHERE (ID IN (SELECT GENRE_ID "
                   "FROM TBL_TAGS WHERE FILE_ID IN (SELECT FILE_ID FROM TBL_FAVORITE)) AND ?)";
    NCList<NCString> whereList;
    NCString args("1");
    whereList << &args;

    NCCursor* cursor = dbAccess.rawQuery(sql, whereList);
    if (nullptr == cursor) {
        VR_LOGD("query error");
        return false;
    }

    do {
        int row = cursor->getCount();
        VR_LOGD("row: %d", row);
        if (0 == row) {
            VR_LOGD("none records");
            res = false;
            break;
        }

        if (NC_FALSE == cursor->moveToFirst()) {
            VR_LOGD("move head failed");
            res = false;
            break;
        }

        for (int i = 0; i < row; i++) {
            NCString genre = cursor->getString(0);
            VR_LOGD("%d: %s", i, genre.getString());

            list.push_back(genre.getString());

            cursor->moveToNext();
        }
        res = true;
    } while (0);

    delete cursor;
    cursor = nullptr;
    return res;
}

VR_String VR_MusicDBHelperManager::GetEngineDBPath(int type)
{
    VR_LOGD_FUNC();
    nutshell::NCAutoSync lock(g_cSyncObj);

    if (VR_MUIFA_SOURCE_U_DISK == type) {
        return m_engineDBPath_usb;
    }
    else if (VR_MUIFA_SOURCE_IPOD == type) {
        return m_engineDBPath_ipod;
    }
    else {
        VR_LOGD("unknown type");
    }
    return "";
}

void VR_MusicDBHelperManager::ClearEngineDBPath(int type)
{
    VR_LOGD_FUNC();
    nutshell::NCAutoSync lock(g_cSyncObj);
    if (VR_MUIFA_SOURCE_U_DISK == type) {
        m_engineDBPath_usb = "";
    }
    else if (VR_MUIFA_SOURCE_IPOD == type) {
        m_engineDBPath_ipod = "";
    }
    else {
        VR_LOGD("unknown type");
    }
}

}
}
/* EOF */
