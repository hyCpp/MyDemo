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

#include <boost/assign.hpp>
#include "VR_Log.h"
#include "BL_File.h"
#include "BL_Dir.h"
#include "VR_NCTutorialInfoProvider.h"

namespace nutshell
{
namespace tutorial
{
#define VR_NCTUTORIAL_INFO_PROVIDER_NAME      "tutorialinfo"
#define VR_NCTUTORIAL_DATABASE_NAME           "tutorialprovider.db"
#define VR_NCTUTORIAL_DATABASE_VERSION        1

VR_NCTutorialInfoProvider::UriMatchRelateMap VR_NCTutorialInfoProvider::m_MatchRelate = boost::assign::map_list_of
         ((INT)VR_MATCHER_ID_TUTORIAL_CATEGORY_DATA, VR_TABLE_OF_TUTORIAL_CATEGORY)
         ((INT)VR_MATCHER_ID_TUTORIAL_INFO_DATA, VR_TABLE_OF_TUTORIAL_INFO)
         ((INT)VR_MATCHER_ID_TUTORIAL_INFO_SCREEN_DATA, VR_TABLE_OF_TUTORIAL_SCREEN_INFO);

VR_NCTutorialInfoProvider::VR_NCTutorialInfoProvider(const NCString& serviceName)
    : NCContentProvider(VR_NCTUTORIAL_INFO_PROVIDER_NAME, serviceName)
    , m_dbHelper(NULL)
    , m_uriMatcher()
{ 
    // query insert update delete
    m_uriMatcher.addURI(VR_NCTUTORIAL_INFO_PROVIDER_NAME,
                        m_MatchRelate[(INT)VR_MATCHER_ID_TUTORIAL_CATEGORY_DATA] + "/data", 
                        (INT)VR_MATCHER_ID_TUTORIAL_CATEGORY_DATA);
    m_uriMatcher.addURI(VR_NCTUTORIAL_INFO_PROVIDER_NAME,
                        m_MatchRelate[(INT)VR_MATCHER_ID_TUTORIAL_INFO_DATA] + "/data", 
                        (INT)VR_MATCHER_ID_TUTORIAL_INFO_DATA);
    m_uriMatcher.addURI(VR_NCTUTORIAL_INFO_PROVIDER_NAME,
                        m_MatchRelate[(INT)VR_MATCHER_ID_TUTORIAL_INFO_SCREEN_DATA] + "/data", 
                        (INT)VR_MATCHER_ID_TUTORIAL_INFO_SCREEN_DATA);
}

VR_NCTutorialInfoProvider::~VR_NCTutorialInfoProvider()
{
    if (NULL != m_dbHelper) {
        delete m_dbHelper;
    }
}

NCCursor* VR_NCTutorialInfoProvider::query(const NCUri& uri, const NCList<NCString>& select, 
                                           const NCString& where, const NCList<NCString>& whereArgs, 
                                           const NCString& sort)
{
    VR_LOGD("VR_NCTutorialInfoProvider::query");
    if (NULL == m_dbHelper) {
        // get tutorial root path
        BL_File configDBPath(BL_FILE_PREFIX_RW);
        BL_String tutorialDBFullPath;
        configDBPath.GetAbsolutePath("tutorialprovider", tutorialDBFullPath);

        // create tutorial root path
        BL_Dir dir(BL_FILE_PREFIX_RW);
        dir.MakeDir("tutorialprovider");

        tutorialDBFullPath = tutorialDBFullPath + "/" + VR_NCTUTORIAL_DATABASE_NAME;
        VR_LOGD("tutorial provider db path:%s", tutorialDBFullPath.GetString());
        m_dbHelper = VR_new VR_NCTutorialInfoDatabaseHelper(NCString(tutorialDBFullPath.GetString()),
                                                            VR_NCTUTORIAL_DATABASE_VERSION);
    }

    // 1.get database object
    NCSQLiteDatabase* db = m_dbHelper->getDatabase();
    if (NULL == db) {
        VR_LOGD("VR_NCTutorialInfoProvider database does not exist");
        return NULL;
    }

    // 2.match uri
    INT result = m_uriMatcher.match(uri);
    if (NCUriMatcher::NO_MATCH == result) {
        VR_LOGD("VR_NCTutorialInfoProvider uri is not supported");
        return NULL;
    }

    // 3.query table all data
    NCString table = m_MatchRelate[result];
    
    VR_LOGD("begin query table:%s", table.getString());
    NCCursor* cursor = db->query(table, select, where, whereArgs, sort);
    VR_LOGD("end query table.cursorAddr:%p", cursor);
    return cursor;
}

INT64 VR_NCTutorialInfoProvider::insert(const NCUri& uri, const NCContentValues& values)
{
    VR_LOGD("VR_NCTutorialInfoProvider::insert");
    if (NULL == m_dbHelper) {
        return -1;
    }

    // 1.get database object
    NCSQLiteDatabase* db = m_dbHelper->getDatabase();
    if (NULL == db) {
        VR_LOGD("VR_NCTutorialInfoProvider database does not exist");
        return -1;
    }

    // 2.match uri
    INT result = m_uriMatcher.match(uri);
    if (NCUriMatcher::NO_MATCH == result) {
        VR_LOGD("VR_NCTutorialInfoProvider uri is not supported");
        return -1;
    }

    // 3.insert table data
    NCString table = m_MatchRelate[result];

    VR_LOGD("begin insert table:%s", table.getString());
    INT64 rowId = db->insert(table, values);
    if (-1 == rowId) {
        VR_LOGD("%s table insert failed", table.getString());
        return -1;
    }
    VR_LOGD("end insert table");
    return rowId;
}
        
INT32 VR_NCTutorialInfoProvider::update(const NCUri& uri, const NCContentValues& values, 
                                        const NCString& where, const NCList<NCString>& whereArgs)
{
    VR_LOGD("VR_NCTutorialInfoProvider::update");
    if (NULL == m_dbHelper) {
        return 0;
    }

    // 1.get database object
    NCSQLiteDatabase* db = m_dbHelper->getDatabase();
    if (NULL == db) {
        VR_LOGD("VR_NCTutorialInfoProvider database does not exist");
        return 0;
    }

    // 2.match uri
    INT result = m_uriMatcher.match(uri);
    if (NCUriMatcher::NO_MATCH == result) {
        VR_LOGD("VR_NCTutorialInfoProvider uri is not supported");
        return 0;
    }

    // 3.update table data by index
    NCString table = m_MatchRelate[result];

    VR_LOGD("begin update table:%s", table.getString());
    INT numChanges = db->update(table, values, where, whereArgs);
    if (0 == numChanges) {
        VR_LOGD("%s table update failed", table.getString());
        return 0;
    }
    VR_LOGD("end update table");
    return numChanges;
}

INT32 VR_NCTutorialInfoProvider::remove(const NCUri& uri, const NCString& where,
                                        const NCList<NCString>& whereArgs)
{
    VR_LOGD("VR_NCTutorialInfoProvider::remove");
    if (NULL == m_dbHelper) {
        return 0;
    }

    // 1.get database object
    NCSQLiteDatabase* db = m_dbHelper->getDatabase();
    if (NULL == db) {
        VR_LOGD("VR_NCTutorialInfoProvider database does not exist");
        return 0;
    }

    // 2.match uri
    INT result = m_uriMatcher.match(uri);
    if (NCUriMatcher::NO_MATCH == result) {
        VR_LOGD("VR_NCTutorialInfoProvider uri is not supported");
        return 0;
    }

    // 3.delete table data by index
    NCString table = m_MatchRelate[result];

    VR_LOGD("begin remove table:%s", table.getString());
    INT numChanges = db->remove(table, where, whereArgs);
    if (0 == numChanges) {
        VR_LOGD("%s table remove failed", table.getString());
        return 0;
    }
    VR_LOGD("end remove table");
    return numChanges;
}

NCString VR_NCTutorialInfoProvider::getType(const NCUri& uri)
{
    VR_LOGD("VR_NCTutorialInfoProvider::getType");
    return NCString(VR_NCTUTORIAL_INFO_PROVIDER_NAME);
}

VOID VR_NCTutorialInfoProvider::onInitialize()
{
    VR_LOGD("VR_NCTutorialInfoProvider::onInitialize");
}

VOID VR_NCTutorialInfoProvider::onStart()
{
    VR_LOGD("VR_NCTutorialInfoProvider::onStart");
}

VOID VR_NCTutorialInfoProvider::onStop()
{
    VR_LOGD("VR_NCTutorialInfoProvider::onStop");
}

VOID VR_NCTutorialInfoProvider::onDeinitialize()
{
    VR_LOGD("VR_NCTutorialInfoProvider::onDeinitialize");
}

}
}
/* EOF */
