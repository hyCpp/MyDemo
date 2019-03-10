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
 * @file VR_NCTutorialInfoDatabaseHelper.h
 * @brief Declaration file of class VR_NCTutorialInfoDatabaseHelper.
 *
 * This file includes the declaration of class VR_NCTutorialInfoDatabaseHelper.
 *
 * @attention used for C++ only.
 */
#ifndef VR_NC_TURORIAL_INFO_DATABASE_HELPER_H
#define VR_NC_TUTORIAL_INFO_DATABASE_HELPER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "dbhelper/NCSQLiteOpenHelper.h"

namespace nutshell
{
namespace tutorial
{

#define VR_TABLE_OF_TUTORIAL_CATEGORY     "tutorial_category"
#define VR_TABLE_OF_TUTORIAL_INFO         "tutorial_info"
#define VR_TABLE_OF_TUTORIAL_SCREEN_INFO  "tutorial_screen_info"

// tutorial_category
#define VR_TUTORIAL_CREATE_TABLE_OF_CATEGORY_SQL \
    "CREATE TABLE IF NOT EXISTS "VR_TABLE_OF_TUTORIAL_CATEGORY" (" \
    "category_index INTEGER(2) PRIMARY KEY," \
    "state          VARCHAR(32)," \
    "language       VARCHAR(32)," \
    "machine_type   VARCHAR(32)" \
    ");"

// tutorial_info
#define VR_TUTORIAL_CREATE_TABLE_OF_INFO_SQL \
    "CREATE TABLE IF NOT EXISTS "VR_TABLE_OF_TUTORIAL_INFO" (" \
    "tutorial_index INTEGER(2) PRIMARY KEY," \
    "name           VARCHAR(256)," \
    "version        VARCHAR(16)," \
    "screen_number  INTEGER(2)," \
    "location_index INTEGER(2)," \
    "category_index INTEGER(2)" \
    ");"

// tutorial_screen_info
#define VR_TUTORIAL_CREATE_TABLE_OF_SCREEN_INFO_SQL \
    "CREATE TABLE IF NOT EXISTS "VR_TABLE_OF_TUTORIAL_SCREEN_INFO" (" \
    "screen_index    INTEGER PRIMARY KEY AUTOINCREMENT," \
    "image           BLOB," \
    "audio           VARCHAR(128)," \
    "sequence_number INTEGER(2)," \
    "tutorial_index  INTEGER(2)" \
    ");"

/**
 * @brief The VR_NCTutorialInfoDatabaseHelper class
 *
 * class declaration.
 */
class VR_NCTutorialInfoDatabaseHelper : public NCSQLiteOpenHelper
{
public:
    VR_NCTutorialInfoDatabaseHelper(const NCString& name, INT version);
    virtual ~VR_NCTutorialInfoDatabaseHelper();
    virtual VOID onCreate(NCSQLiteDatabase* db);
    virtual VOID onUpgrade(NCSQLiteDatabase* db, INT oldVersion, INT newVersion);
    virtual VOID onDowngrade(NCSQLiteDatabase* db, INT oldVersion, INT newVersion);
    virtual VOID onOpen(NCSQLiteDatabase* db);

private:
    VR_NCTutorialInfoDatabaseHelper(const VR_NCTutorialInfoDatabaseHelper&);
    VR_NCTutorialInfoDatabaseHelper& operator=(const VR_NCTutorialInfoDatabaseHelper&);
    VOID createTable(NCSQLiteDatabase* db);

private:
    static const NCCHAR* g_createSQLTemplate[];
};

}
}
#endif /* VR_NC_TUTORIAL_INFO_DATABSE_HELPER_H */
/* EOF */
