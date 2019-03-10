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
 * @file VR_PhoneBookListener.h
 * @brief Declaration file of class VR_PhoneBookListener.
 *
 * This file includes the declaration of class VR_PhoneBookListener.
 *
 * @attention used for C++ only.
 */
#ifndef VR_PHONE_BOOK_LISTENER_TEST_H
#define VR_PHONE_BOOK_LISTENER_TEST_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <contentprovider/NCContentProvider.h>
#include "dbhelper/NCSQLiteOpenHelper.h"
#include <ncore/NCUriMatcher.h>
#include "VR_PhoneBookListener.h"
#include "gtest/gtest.h"


namespace navi
{
namespace dataprovider
{

#define VR_PHONEBOOK_CREATE_TABLE_OF_CONTACTS_SQL \
    "CREATE TABLE IF NOT EXISTS contacts (" \
    "_id                    INTEGER," \
    "contact_id             INTEGER," \
    "firstname              TEXT," \
    "lastname               TEXT," \
    "sort_fl                TEXT," \
    "sort_lf                TEXT," \
    "phone_number           TEXT," \
    "phone1_type            INTEGER," \
    "phone1_index           INTEGER," \
    "phone1_number          TEXT," \
    "phone1_domesticnumber  TEXT," \
    "phone1_widget          INTEGER," \
    "phone2_type            INTEGER," \
    "phone2_index           INTEGER," \
    "phone2_number          TEXT," \
    "phone2_domesticnumber  TEXT," \
    "phone2_widget          INTEGER," \
    "phone3_type            INTEGER," \
    "phone3_index           INTEGER," \
    "phone3_number          TEXT," \
    "phone3_domesticnumber  TEXT," \
    "phone3_widget          INTEGER," \
    "phone4_type            INTEGER," \
    "phone4_index           INTEGER," \
    "phone4_number          TEXT," \
    "phone4_domesticnumber  TEXT," \
    "phone4_widget          INTEGER," \
    "email1_type            INTEGER," \
    "email1_addr            TEXT," \
    "email2_type            INTEGER," \
    "email2_addr            TEXT," \
    "email3_type            INTEGER," \
    "email3_addr            TEXT," \
    "email4_type            INTEGER," \
    "email4_addr            TEXT," \
    "address1               TEXT," \
    "address2               TEXT," \
    "address3               TEXT," \
    "address4               TEXT," \
    "is_favor               INTEGER," \
    "voicetag               INTEGER," \
    "photo_status           INTEGER," \
    "is_selected            INTEGER);"

#define VR_PHONEBOOK_CREATE_TABLE_OF_CALLHIST_SQL \
    "CREATE TABLE IF NOT EXISTS callhistory (" \
    "_id                   INTEGER," \
    "contact_id            INTEGER," \
    "firstname             TEXT," \
    "lastname              TEXT," \
    "phone_type            INTEGER," \
    "phone_index           INTEGER," \
    "phone_number          TEXT," \
    "phone_domesticnumber  TEXT," \
    "calltype              INTEGER," \
    "calltime              TEXT," \
    "timezone              TEXT," \
    "count                 INTEGER);"

#define VR_NCPHONEBOOK_INFO_PROVIDER_NAME "phonebookinfo_test"
#define VR_NCPHONEBOOK_DATABASE_NAME      "pbproviderinfo.db"
#define VR_NCPHONEBOOK_DATABASE_VERSION   1

#define VR_TABLE_OF_PHONEBOOK_CONTACTS    "contacts"
#define VR_TABLE_OF_PHONEBOOK_CALLHISTORY "callhistory"
#define VR_TABLE_OF_PHONEBOOK_MESSAGEHIST "messagehistory"

typedef enum tagPhoneBookInfoMatcherStatus
{
    PHONEBOOK_MATCHER_ID_CONTACTS_DATA = 0,
    PHONEBOOK_MATCHER_ID_CALLHIST_DATA,
    PHONEBOOK_MATCHER_ID_MSGHIST_DATA

}PhoneBookInfoMatchStatus;

// ************************************************************************* //
// **                   NCPhoneBookDataBaseHelper_Test.cpp                ** //
// ************************************************************************* //
/**
 * @brief The NCPhoneBookDataBaseHelper_Test class
 *
 * creat db-file for phonebooklistener testting template
 */
class NCPhoneBookDataBaseHelper_Test : public nutshell::NCSQLiteOpenHelper
{
public:
    NCPhoneBookDataBaseHelper_Test(const nutshell::NCString& name, nutshell::INT version);
    virtual ~NCPhoneBookDataBaseHelper_Test();
    virtual VOID onCreate(nutshell::NCSQLiteDatabase* db);
    virtual VOID onUpgrade(nutshell::NCSQLiteDatabase* db, nutshell::INT oldVersion, nutshell::INT newVersion);
    virtual VOID onDowngrade(nutshell::NCSQLiteDatabase* db, nutshell::INT oldVersion, nutshell::INT newVersion);
    virtual VOID onOpen(nutshell::NCSQLiteDatabase* db);

private:
    NCPhoneBookDataBaseHelper_Test(const NCPhoneBookDataBaseHelper_Test&);
    NCPhoneBookDataBaseHelper_Test& operator=(const NCPhoneBookDataBaseHelper_Test&);
    VOID createTable(nutshell::NCSQLiteDatabase* db);

private:
    static const nutshell::NCCHAR* m_sCreateSQLTemp[];
};


// ************************************************************************* //
// **                   NCPhoneBookSettingProvider_Test.cpp               ** //
// ************************************************************************* //
/**
 * @brief The NCPhoneBookSettingProvider_Test class
 *
 * creat provider for phonebooklistener test templete
 */
class NCPhoneBookSettingProvider_Test : public nutshell::NCContentProvider
{
public:
    NCPhoneBookSettingProvider_Test(const nutshell::NCString& serviceName);
    virtual ~NCPhoneBookSettingProvider_Test();

    virtual nutshell::NCCursor* query(
        const nutshell::NCUri& uri,
        const nutshell::NCList<nutshell::NCString>& select,
        const nutshell::NCString& where,
        const nutshell::NCList<nutshell::NCString>& whereArgs,
        const nutshell::NCString& sort);

    virtual INT64 insert(
        const nutshell::NCUri& uri,
        const nutshell::NCContentValues& values);

    virtual INT32 update(
        const nutshell::NCUri& uri,
        const nutshell::NCContentValues& values,
        const nutshell::NCString& where,
        const nutshell::NCList<nutshell::NCString>& whereArgs);

    virtual INT32 remove(
        const nutshell::NCUri& uri,
        const nutshell::NCString& where,
        const nutshell::NCList<nutshell::NCString>& whereArgs);

    virtual nutshell::NCString getType(const nutshell::NCUri& uri);

protected:
    virtual VOID onInitialize();
    virtual VOID onStart();
    virtual VOID onStop();
    virtual VOID onDeinitialize();

private:
    NCPhoneBookSettingProvider_Test(const NCPhoneBookSettingProvider_Test&);
    NCPhoneBookSettingProvider_Test& operator=(const NCPhoneBookSettingProvider_Test&);

    NCPhoneBookDataBaseHelper_Test* m_pPhoneBookDBHelper;
    nutshell::NCUriMatcher m_uriMatcher;

    typedef VoiceMap<nutshell::INT, nutshell::NCString>::type UriMatchRelateMap;
    static UriMatchRelateMap m_sMatchRelate;
};


// ************************************************************************* //
// **                    VR_PhoneBookListener_Test.cpp                    ** //
// ************************************************************************* //
/**
 * @brief The VR_PhoneBookListener_Test class
 *
 * class declaration
 */
class VR_PhoneBookListener_Test : public testing::Test
{
public:
    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

protected:
    VR_PhoneBookListener_Test();
    ~VR_PhoneBookListener_Test();

private:
    VR_PhoneBookListener_Test(const VR_PhoneBookListener_Test&);
    VR_PhoneBookListener_Test& operator=(const VR_PhoneBookListener_Test&);
    static VR_PhoneBookListener* m_test;
};

} // dataprovider
} // navi

#endif /* VR_PHONE_BOOK_LISTENER_TEST_H */
/* EOF */
