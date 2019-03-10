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
#include "VR_ConfigureIF.h"
#include "VR_MusicDBHelper.h"
#include "VR_MusicDBHelper_Engine.h"
#include "VR_MusicDBHelper_IPodIAP1.h"
#include "VR_MusicDBHelper_IPodIAP2.h"
#include "VR_MusicDBHelper_USBGeneral.h"

#include "EV_Define.h"
#include "ncore/NCFile.h"
#include "EV_EventSender.h"
#include "boost/format.hpp"
#include "dbhelper/NCSQLiteDatabase.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{

VR_MusicDBHelper::VR_MusicDBHelper(const MusicDBHelperItem& item)
{
    VR_LOGD_FUNC();
    m_Info.DBItem.Path = item.DBItem.Path;
    m_Info.DBItem.SyncStatus = item.DBItem.SyncStatus;
    m_Info.DeviceItem.DevDriverType = item.DeviceItem.DevDriverType;
    m_Info.DeviceItem.DevName = item.DeviceItem.DevName;
    m_Info.DeviceItem.DevType = item.DeviceItem.DevType;
    if (MUSIC_DRIVER_TYPE_ENGINE != item.DeviceItem.DevDriverType) {
        return;
    }
}

VR_MusicDBHelper::~VR_MusicDBHelper()
{
    VR_LOGD_FUNC();
}

VR_MusicDBHelper* VR_MusicDBHelper::CreateDBHelper(const MusicDBHelperItem& item)
{
    VR_MusicDBHelper* dbHelper = nullptr;
    switch (item.DeviceItem.DevDriverType) {
    case MUSIC_DRIVER_TYPE_ENGINE: {
        dbHelper = VR_new VR_MusicDBHelper_Engine(item);
        break;
    }
    case MUSIC_DRIVER_TYPE_USB_GENERAL: {
        dbHelper = VR_new VR_MusicDBHelper_USBGeneral(item);
        break;
    }
    case MUSIC_DRIVER_TYPE_IPOD_IAP1: {
        dbHelper = VR_new VR_MusicDBHelper_IPodIAP1(item);
        break;
    }
    case MUSIC_DRIVER_TYPE_IPOD_IAP2: {
        dbHelper = VR_new VR_MusicDBHelper_IPodIAP2(item);
        break;
    }
    default:
        VR_LOGD("unknown type. %d", item.DeviceItem.DevDriverType);
        return nullptr;
    }

    return dbHelper;
}

/**
 * @brief synchronize media database to voice music database which
 * in nuance specification
 * @param target
 * @return
 */
bool VR_MusicDBHelper::SyncMusicDB(VR_MusicDBHelper* target)
{
    VR_LOGD_FUNC();
    if (nullptr == target) {
        VR_LOGD("target empty ptr");
        return false;
    }

    NCSQLiteDatabase dbAccess(target->m_Info.DBItem.Path.c_str(), CREATE_IF_NECESSARY, nullptr, 0);
    bool res = dbAccess.isOpen();
    if (!res) {
        VR_LOGD("open db failed. %s", m_Info.DBItem.Path.c_str());
        return res;
    }

    VR_String atsql = (boost::format(VR_ATTACH_DB_SQL) % m_Info.DBItem.Path.c_str()).str();
    INT err = dbAccess.execSQL(atsql.c_str());
    if (NCSQLITE_OK != err) {
        VR_LOGD("error happend! err:%d, sql:%s", dbAccess.getLastError(), atsql.c_str());
        return false;
    }

    dbAccess.execSQL(VR_TRANACTION_BEGIN_SQL);
    NCList<VR_String>* list = MergeDBTableSQL();
    if (nullptr == list) {
        VR_LOGD("alloc merge sql failed");
        return res;
    }

    NCList<VR_String>::Iterator iter = list->begin();
    do {
        VR_String* sql = iter.current();
        if (nullptr == sql) {
            VR_LOGD("sql is null");
            continue;
        }
        err = dbAccess.execSQL(sql->c_str());
        if (NCSQLITE_OK != err) {
            VR_LOGD("error happend! err:%d, sql:%s", dbAccess.getLastError(), sql->c_str());
            return false;
        }
    } while (nullptr != iter());

    err = dbAccess.execSQL(VR_FORMAT_SPECIAL_SQL);
    if (NCSQLITE_OK != err) {
        VR_LOGD("error happend! err:%d, sql:%s", dbAccess.getLastError(), VR_FORMAT_SPECIAL_SQL);
        return false;
    }

    dbAccess.execSQL(VR_DEATCH_DB_SQL);
    dbAccess.execSQL(VR_TRANACTION_END_SQL);
    dbAccess.close();
    return true;
}

NCList<VR_String>* VR_MusicDBHelper::CreateDBTableSQL()
{
    VR_LOGD_FUNC();
    VR_LOGD("create db in base class");
    return nullptr;
}

NCList<VR_String>* VR_MusicDBHelper::MergeDBTableSQL()
{
    VR_LOGD_FUNC();
    VR_LOGD("create db in base class");
    return nullptr;
}

}
}
/* EOF */
