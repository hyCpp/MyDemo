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
#include "VR_MusicDBHelper_Engine.h"

#include "ncore/NCFile.h"
#include "dbhelper/NCSQLiteDatabase.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{

const char* g_create_sql[] = {
    VR_MUSIC_ENGINE_CREATE_INFO_TABLE_SQL,
    VR_MUSIC_ENGINE_CREATE_ENTITY_TABLE_SQL,
    VR_MUSIC_ENGINE_CREATE_TYPE_SQL,
    VR_MUSIC_ENGINE_CREATE_ALBUMSONG_SQL,
    VR_MUSIC_ENGINE_CREATE_ARTISTSSONG_SQL,
    VR_MUSIC_ENGINE_CREATE_GENRESSONGS_SQL,
    VR_MUSIC_ENGINE_CREATE_PLAYLISTSSONGS_SQL
};

VR_MusicDBHelper_Engine::VR_MusicDBHelper_Engine(const MusicDBHelperItem& item)
    : VR_MusicDBHelper(item)
    , m_CreateSQLList(nullptr)
{
    VR_LOGD_FUNC();
    bool res = NCFile::exists(item.DBItem.Path.c_str());
    if (res) {
        res = NCFile::remove(item.DBItem.Path.c_str());
        if (!res) {
            VR_LOGD("remove failed. %s", item.DBItem.Path.c_str());
            return;
        }
    }

    NCSQLiteDatabase dbAccess(item.DBItem.Path.c_str(), CREATE_IF_NECESSARY, nullptr, 0);
    res = dbAccess.isOpen();
    if (!res) {
        VR_LOGD("open db failed. err:%d, %s", dbAccess.getLastError(), item.DBItem.Path.c_str());
        return;
    }

    NCList<VR_String>* list = CreateDBTableSQL();
    if (nullptr == list) {
        VR_LOGD("get create failed");
        return;
    }

    NCList<VR_String>::Iterator iter = list->begin();
    do {
        VR_String* sql = iter.current();
        if (nullptr == sql) {
            VR_LOGD("sql is null");
            continue;
        }
        int err = dbAccess.execSQL(sql->c_str());
        if (NCSQLITE_OK != err) {
            VR_LOGD("error happend! err:%d, sql:%s", dbAccess.getLastError(), sql->c_str());
            return;
        }
    } while (nullptr != iter());
    dbAccess.close();
    VR_LOGD("create engine database success");
}

VR_MusicDBHelper_Engine::~VR_MusicDBHelper_Engine()
{
    VR_LOGD_FUNC();
    delete m_CreateSQLList;
    m_CreateSQLList = nullptr;
}

NCList<VR_String>* VR_MusicDBHelper_Engine::CreateDBTableSQL()
{
    VR_LOGD_FUNC();
    if (nullptr != m_CreateSQLList) {
        return m_CreateSQLList;
    }

    m_CreateSQLList = VR_new NCList<VR_String>();
    if (nullptr == m_CreateSQLList) {
        VR_LOGD("new list failed");
        return nullptr;
    }

    for (int i = 0; i < VR_MUSIC_ENGINE_MAX; i++) {
        VR_String* sql = VR_new VR_String(g_create_sql[i]);
        if (nullptr == sql) {
            VR_LOGD("new str failed");
            continue;
        }

        *m_CreateSQLList << sql;
    }

    return m_CreateSQLList;
}

NCList<VR_String>* VR_MusicDBHelper_Engine::MergeDBTableSQL()
{
    VR_LOGD_FUNC();
    return nullptr;
}

}
}
/* EOF */
