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
#include "VR_NCTutorialInfoDatabaseHelper.h"

namespace nutshell
{
namespace tutorial
{
const NCCHAR* VR_NCTutorialInfoDatabaseHelper::g_createSQLTemplate[] = {
    VR_TUTORIAL_CREATE_TABLE_OF_CATEGORY_SQL,
    VR_TUTORIAL_CREATE_TABLE_OF_INFO_SQL,
    VR_TUTORIAL_CREATE_TABLE_OF_SCREEN_INFO_SQL
};

VR_NCTutorialInfoDatabaseHelper::VR_NCTutorialInfoDatabaseHelper(const NCString& name, INT version)
    : NCSQLiteOpenHelper(name, version)
{
}

VR_NCTutorialInfoDatabaseHelper::~VR_NCTutorialInfoDatabaseHelper()
{
}

VOID VR_NCTutorialInfoDatabaseHelper::onCreate(NCSQLiteDatabase* db)
{
    if (NULL == db) {
        return;
    }

    VR_LOGD("VR_NCTutorialInfoDatabaseHelper::onCreate");
    createTable(db);
}

VOID VR_NCTutorialInfoDatabaseHelper::onUpgrade(NCSQLiteDatabase* db, 
                                                INT oldVersion, INT newVersion)
{
    if (NULL == db) {
        return;
    }

    VR_LOGD("VR_NCTutorialInfoDatabaseHelper::onUpgrade");
}

VOID VR_NCTutorialInfoDatabaseHelper::onDowngrade(NCSQLiteDatabase* db, INT oldVersion, INT newVersion)
{
    VR_LOGD("VR_NCTutorialInfoDatabaseHelper::onDowngrade");
}

VOID VR_NCTutorialInfoDatabaseHelper::onOpen(NCSQLiteDatabase* db)
{
    VR_LOGD("VR_NCTutorialInfoDatabaseHelper::onOpen");
}

VOID VR_NCTutorialInfoDatabaseHelper::createTable(NCSQLiteDatabase* db)
{
    INT result = 0;
    
    // 1.check param
    if (NULL == db) {
        return;
    }

    // 2.create table set
    INT templateCount = sizeof(g_createSQLTemplate) / sizeof(NCCHAR*);
    for (INT i = 0; i < templateCount; i++) {
        result = db->execSQL(g_createSQLTemplate[i]);
        if (0 != result) {
            VR_LOGD("ErrCode:%d.%s failed", result, g_createSQLTemplate[i]);
            return;
        }
    }
}

}
}
/* EOF */
