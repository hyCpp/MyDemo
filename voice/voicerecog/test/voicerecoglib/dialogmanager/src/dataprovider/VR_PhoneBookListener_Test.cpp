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

#include "VR_AvcLanSenderIF.h"
#include "VR_AvcLanSender_mock.h"
#include "VR_DialogEngineIF_mock.h"
#include "VR_DataProviderComm.h"
#include "VR_PhoneBookListener_Test.h"


namespace navi
{
namespace dataprovider
{

using namespace nutshell;

// *********************************************************************** //
// **                 NCPhoneBookDataBaseHelper_Test.cpp                ** //
// *********************************************************************** //
const NCCHAR* NCPhoneBookDataBaseHelper_Test::m_sCreateSQLTemp[] = {
    VR_PHONEBOOK_CREATE_TABLE_OF_CONTACTS_SQL,
    VR_PHONEBOOK_CREATE_TABLE_OF_CALLHIST_SQL };

NCPhoneBookDataBaseHelper_Test::NCPhoneBookDataBaseHelper_Test(const NCString& name, INT version)
    : NCSQLiteOpenHelper(name, version)
{
}

NCPhoneBookDataBaseHelper_Test::~NCPhoneBookDataBaseHelper_Test()
{
}

VOID NCPhoneBookDataBaseHelper_Test::onCreate(NCSQLiteDatabase* db)
{
    if (NULL == db) {
        return;
    }

    VR_LOGD("NCPhoneBookDataBaseHelper_Test::onCreate");
    createTable(db);
}

VOID NCPhoneBookDataBaseHelper_Test::onUpgrade(NCSQLiteDatabase* db, INT oldVersion, INT newVersion)
{
    if (NULL == db) {
        return;
    }

    VR_LOGD("NCPhoneBookDataBaseHelper_Test::onUpgrade");
}

VOID NCPhoneBookDataBaseHelper_Test::onDowngrade(NCSQLiteDatabase* db, INT oldVersion, INT newVersion)
{

}

VOID NCPhoneBookDataBaseHelper_Test::onOpen(NCSQLiteDatabase* db)
{

}

VOID NCPhoneBookDataBaseHelper_Test::createTable(NCSQLiteDatabase* db)
{
    INT result = 0;
    if (NULL == db) {
        return;
    }

    INT templateCount = sizeof(m_sCreateSQLTemp) / sizeof(NCCHAR*);
    for (INT i = 0; i < templateCount; i++) {
        result = db->execSQL(m_sCreateSQLTemp[i]);
        if (0 != result) {
            VR_LOGD("ErrCode:%d.%s failed", result, m_sCreateSQLTemp[i]);
            return;
        }
    }
}


// *********************************************************************** //
// **                  NCPhoneBookSettingProvider_Test.cpp              ** //
// *********************************************************************** //
NCPhoneBookSettingProvider_Test::UriMatchRelateMap NCPhoneBookSettingProvider_Test::m_sMatchRelate = boost::assign::map_list_of
         ((INT)PHONEBOOK_MATCHER_ID_CONTACTS_DATA, VR_TABLE_OF_PHONEBOOK_CONTACTS)
         ((INT)PHONEBOOK_MATCHER_ID_CALLHIST_DATA, VR_TABLE_OF_PHONEBOOK_CALLHISTORY);

NCPhoneBookSettingProvider_Test::NCPhoneBookSettingProvider_Test(const NCString& serviceName)
    : NCContentProvider(VR_NCPHONEBOOK_INFO_PROVIDER_NAME, serviceName)
    , m_pPhoneBookDBHelper(NULL)
    , m_uriMatcher()
{
    m_uriMatcher.addURI(VR_NCPHONEBOOK_INFO_PROVIDER_NAME, "contacts/data", 0);
    m_uriMatcher.addURI(VR_NCPHONEBOOK_INFO_PROVIDER_NAME, "callhistory/data", 1);
}

NCPhoneBookSettingProvider_Test::~NCPhoneBookSettingProvider_Test()
{
    if (NULL != m_pPhoneBookDBHelper) {
        delete m_pPhoneBookDBHelper;
    }
}

NCCursor* NCPhoneBookSettingProvider_Test::query(const NCUri& uri,
                                                 const NCList<NCString>& select,
                                                 const NCString& where,
                                                 const NCList<NCString>& whereArgs,
                                                 const NCString& sort)
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::query");
    if (NULL == m_pPhoneBookDBHelper) {
        BL_File configDBPath(BL_FILE_PREFIX_RW);
        BL_String pbDBFullPath;
        configDBPath.GetAbsolutePath("phonebookprovider", pbDBFullPath);

        BL_Dir dir(BL_FILE_PREFIX_RW);
        dir.MakeDir("phonebookprovider");

        pbDBFullPath = pbDBFullPath + "/" + VR_NCPHONEBOOK_DATABASE_NAME;
        VR_LOGD("phonebook provider db path:%s", pbDBFullPath.GetString());
        m_pPhoneBookDBHelper = VR_new NCPhoneBookDataBaseHelper_Test(NCString(pbDBFullPath.GetString()),
                                                            VR_NCPHONEBOOK_DATABASE_VERSION);
    }

    // 1.get database object
    NCSQLiteDatabase* db = m_pPhoneBookDBHelper->getDatabase();
    if (NULL == db) {
        VR_LOGD("NCPhoneBookSettingProvider_Test database does not exist");
        return NULL;
    }

    // 2.match uri
    INT result = m_uriMatcher.match(uri);
    if (NCUriMatcher::NO_MATCH == result) {
        VR_LOGD("NCPhoneBookSettingProvider_Test uri is not supported");
        return NULL;
    }

    // 3.query table all data
    NCString table = m_sMatchRelate[result];

    VR_LOGD("begin query table:%s", table.getString());
    NCCursor* cursor = db->query(table, select, where, whereArgs, sort);
    VR_LOGD("end query table.cursorAddr:%p", cursor);
    return cursor;
}

INT64 NCPhoneBookSettingProvider_Test::insert(const NCUri& uri, const NCContentValues& values)
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::insert");
    if (NULL == m_pPhoneBookDBHelper) {
        return -1;
    }

    // 1.get database object
    NCSQLiteDatabase* db = m_pPhoneBookDBHelper->getDatabase();
    if (NULL == db) {
        VR_LOGD("NCPhoneBookSettingProvider_Test database does not exist");
        return -1;
    }

    // 2.match uri
    INT result = m_uriMatcher.match(uri);
    if (NCUriMatcher::NO_MATCH == result) {
        VR_LOGD("NCPhoneBookSettingProvider_Test uri is not supported");
        return -1;
    }

    // 3.insert table data
    NCString table = m_sMatchRelate[result];

    VR_LOGD("begin insert table:%s", table.getString());
    INT64 rowId = db->insert(table, values);
    if (-1 == rowId) {
        VR_LOGD("%s table insert failed", table.getString());
        return -1;
    }
    VR_LOGD("end insert table");
    return rowId;
}

INT32 NCPhoneBookSettingProvider_Test::update(const NCUri& uri, const NCContentValues& values,
                                              const NCString& where, const NCList<NCString>& whereArgs)
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::update");
    if (NULL == m_pPhoneBookDBHelper) {
        return 0;
    }

    // 1.get database object
    NCSQLiteDatabase* db = m_pPhoneBookDBHelper->getDatabase();
    if (NULL == db) {
        VR_LOGD("NCPhoneBookSettingProvider_Test database does not exist");
        return 0;
    }

    // 2.match uri
    INT result = m_uriMatcher.match(uri);
    if (NCUriMatcher::NO_MATCH == result) {
        VR_LOGD("NCPhoneBookSettingProvider_Test uri is not supported");
        return 0;
    }

    // 3.update table data by index
    NCString table = m_sMatchRelate[result];

    VR_LOGD("begin update table:%s", table.getString());
    INT numChanges = db->update(table, values, where, whereArgs);
    if (0 == numChanges) {
        VR_LOGD("%s table update failed", table.getString());
        return 0;
    }
    VR_LOGD("end update table");
    return numChanges;
}

INT32 NCPhoneBookSettingProvider_Test::remove(const NCUri& uri, const NCString& where,
                                              const NCList<NCString>& whereArgs)
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::remove");
    if (NULL == m_pPhoneBookDBHelper) {
        return 0;
    }

    // 1.get database object
    NCSQLiteDatabase* db = m_pPhoneBookDBHelper->getDatabase();
    if (NULL == db) {
        VR_LOGD("NCPhoneBookSettingProvider_Test database does not exist");
        return 0;
    }

    // 2.match uri
    INT result = m_uriMatcher.match(uri);
    if (NCUriMatcher::NO_MATCH == result) {
        VR_LOGD("NCPhoneBookSettingProvider_Test uri is not supported");
        return 0;
    }

    // 3.delete table data by index
    NCString table = m_sMatchRelate[result];

    VR_LOGD("begin remove table:%s", table.getString());
    INT numChanges = db->remove(table, where, whereArgs);
    if (0 == numChanges) {
        VR_LOGD("%s table remove failed", table.getString());
        return 0;
    }
    VR_LOGD("end remove table");
    return numChanges;
}

NCString NCPhoneBookSettingProvider_Test::getType(const NCUri& uri)
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::getType");
    return NCString(VR_NCPHONEBOOK_INFO_PROVIDER_NAME);
}

VOID NCPhoneBookSettingProvider_Test::onInitialize()
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::onInitialize");
}

VOID NCPhoneBookSettingProvider_Test::onStart()
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::onStart");
}

VOID NCPhoneBookSettingProvider_Test::onStop()
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::onStop");
}

VOID NCPhoneBookSettingProvider_Test::onDeinitialize()
{
    VR_LOGD("NCPhoneBookSettingProvider_Test::onDeinitialize");
}


// *********************************************************************** //
//                     VR_PhoneBookListener_Test.cpp                       //
// *********************************************************************** //
VR_PhoneBookListener* VR_PhoneBookListener_Test::m_test = NULL;

void VR_PhoneBookListener_Test::SetUpTestCase()
{
    if (NULL != m_test) {
        return;
    }

    m_test = VR_PhoneBookListener::GetInstance();
    if (NULL == m_test) {
        m_test = VR_PhoneBookListener::Assign();
    }
}

void VR_PhoneBookListener_Test::TearDownTestCase()
{
    if (NULL != m_test) {
        // VR_PhoneBookListener::Dispose();
        delete m_test;
        m_test = NULL;
    }
}

VR_PhoneBookListener_Test::VR_PhoneBookListener_Test()
{

}

VR_PhoneBookListener_Test::~VR_PhoneBookListener_Test()
{

}

TEST_F(VR_PhoneBookListener_Test, Test_Initialize)
{
    ASSERT_NE(0, (INT64)m_test);
    VR_AvcLanSenderType senderTypeA = VR_AvcLanSenderTypeAgent;

    EXPECT_EQ(VR_AvcLanSenderTypeAgent, senderTypeA);
    m_test->Initialize();

    VR_AvcLanSenderType senderTypeU = VR_AvcLanSenderTypeUnit;
    EXPECT_EQ(VR_AvcLanSenderTypeUnit, senderTypeU);
    m_test->Initialize();

    VR_AvcLanSenderType senderType = VR_AvcLanSenderTypeNULL;
    EXPECT_EQ(VR_AvcLanSenderTypeNULL, senderType);
    m_test->Initialize();
}

TEST_F(VR_PhoneBookListener_Test, Test_onNotifyUpdate)
{
    ASSERT_NE(0, (INT64)m_test);

    nutshell::NCUri uri(nutshell::NCString(VR_PHONE_BOOK_CONTACT_DATA_URI));
    nutshell::NCVariant variant(nutshell::NCString("TEST"));
    VR_AvcLanSenderType senderTypeA = VR_AvcLanSenderTypeAgent;
    EXPECT_EQ(VR_AvcLanSenderTypeAgent, senderTypeA);
    m_test->onNotifyUpdate(uri, variant);
    VR_AvcLanSenderType senderTypeU = VR_AvcLanSenderTypeUnit;
    EXPECT_EQ(VR_AvcLanSenderTypeUnit, senderTypeU);
    m_test->onNotifyUpdate(uri, variant);
    VR_AvcLanSenderType senderType = VR_AvcLanSenderTypeNULL;
    EXPECT_EQ(VR_AvcLanSenderTypeNULL, senderType);
    m_test->onNotifyUpdate(uri, variant);
}

TEST_F(VR_PhoneBookListener_Test, Test_SetXmlSenderToDE)
{
    ASSERT_NE(0, (INT64)m_test);
    spVR_DialogEngineIF dialogEngineIF(VR_new VR_DialogEngineIF_mock());
    m_test->SetXmlSenderToDE(dialogEngineIF);
}

TEST_F(VR_PhoneBookListener_Test, Test_SetAvcSenderToDP)
{
    ASSERT_NE(0, (INT64)m_test);
    spVR_AvcLanSenderIF avcLanSender(VR_new VR_AvcLanSender_mock());
    m_test->SetAvcSenderToDP(avcLanSender);
}

TEST_F(VR_PhoneBookListener_Test, Test_InitializeDefault)
{
    ASSERT_NE(0, (INT64)m_test);
    m_test->InitializeDefault();
}

TEST_F(VR_PhoneBookListener_Test, Test_onNotifyUpdateDefault)
{
    ASSERT_NE(0, (INT64)m_test);
    nutshell::NCUri uri(nutshell::NCString(""));
    nutshell::NCVariant variant(nutshell::NCString(""));
    m_test->onNotifyUpdateDefault(uri, variant);
}

TEST_F(VR_PhoneBookListener_Test, Test_getDiffContactsDataDefault)
{
    ASSERT_NE(0, (INT64)m_test);
    int id = 3;
    EXPECT_FALSE(m_test->getDiffContactsDataDefault(id));
}

TEST_F(VR_PhoneBookListener_Test, Test_getContactDataDefault)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_FALSE(m_test->getContactDataDefault());
}

TEST_F(VR_PhoneBookListener_Test, Test_getQuickReplyDataDefault)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_TRUE(m_test->getQuickReplyDataDefault());
}

TEST_F(VR_PhoneBookListener_Test, Test_InitializeVrAgent)
{
    ASSERT_NE(0, (INT64)m_test);
    m_test->InitializeVrAgent();
}

TEST_F(VR_PhoneBookListener_Test, Test_onNotifyUpdateVrAgent)
{
    ASSERT_NE(0, (INT64)m_test);
    nutshell::NCUri uri(nutshell::NCString(""));
    nutshell::NCVariant variant(nutshell::NCString(""));
    m_test->onNotifyUpdateVrAgent(uri, variant);
}

TEST_F(VR_PhoneBookListener_Test, Test_getContactDataAgent)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_FALSE(m_test->getContactDataAgent());
}

TEST_F(VR_PhoneBookListener_Test, Test_getQuickReplyDataAgent)
{
    ASSERT_NE(0, (INT64)m_test);
    nutshell::NCVariant variant(nutshell::NCString(""));
    EXPECT_TRUE(m_test->getQuickReplyDataAgent(variant));
}

TEST_F(VR_PhoneBookListener_Test, Test_getCallHistDataAgent)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_FALSE(m_test->getCallHistDataAgent());
}

TEST_F(VR_PhoneBookListener_Test, Test_getMsgHistoryDataAgent)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_FALSE(m_test->getMsgHistoryDataAgent());
}

TEST_F(VR_PhoneBookListener_Test, Test_SendFileToMEU)
{
    ASSERT_NE(0, (INT64)m_test);
    VR_String srcFilePath("");
    VR_String destFilePath("");
    m_test->SendFileToMEU(srcFilePath, destFilePath);
}

TEST_F(VR_PhoneBookListener_Test, Test_InitializeVrUnit)
{
    ASSERT_NE(0, (INT64)m_test);
    m_test->InitializeVrUnit();
}

TEST_F(VR_PhoneBookListener_Test, Test_onNotifyUpdateVrUnit)
{
    ASSERT_NE(0, (INT64)m_test);
    nutshell::NCUri uri(nutshell::NCString(""));
    nutshell::NCVariant variant(nutshell::NCString(""));
    m_test->onNotifyUpdateVrUnit(uri, variant);
}

TEST_F(VR_PhoneBookListener_Test, Test_getContactDataUnit)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_FALSE(m_test->getContactDataUnit());
}

TEST_F(VR_PhoneBookListener_Test, Test_getQuickReplyDataUnit)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_TRUE(m_test->getQuickReplyDataUnit());
}

TEST_F(VR_PhoneBookListener_Test, Test_sendDBPath)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_TRUE(m_test->sendDBPath());
}

TEST_F(VR_PhoneBookListener_Test, Test_saveGramState)
{
    ASSERT_NE(0, (INT64)m_test);
    m_test->saveGramState();
}

TEST_F(VR_PhoneBookListener_Test, Test_readGramState)
{
    ASSERT_NE(0, (INT64)m_test);
    m_test->readGramState();
}

TEST_F(VR_PhoneBookListener_Test, Test_isPhoneConnected)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_TRUE(m_test->isPhoneConnected());
}

TEST_F(VR_PhoneBookListener_Test, Test_copyDbFileForUser)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_FALSE(m_test->copyDbFileForUser());
}

TEST_F(VR_PhoneBookListener_Test, Test_creatTable)
{
    ASSERT_NE(0, (INT64)m_test);
    sqlite3 *dbHandler = nullptr;
    EXPECT_FALSE(m_test->creatTable(dbHandler));
}

TEST_F(VR_PhoneBookListener_Test, Test_fillDataInTable)
{
    ASSERT_NE(0, (INT64)m_test);
    VoiceList<std::shared_ptr<myPersonInfo>>::type personNode;
    EXPECT_FALSE(m_test->fillDataInTable(personNode));
}

TEST_F(VR_PhoneBookListener_Test, Test_getFullName)
{
    ASSERT_NE(0, (INT64)m_test);
    VR_String fullName("");
    VR_String firstName("FIRST");
    VR_String lastName("LAST");
    m_test->getFullName(fullName, firstName, lastName);
}

TEST_F(VR_PhoneBookListener_Test, Test_getDestPath)
{
    ASSERT_NE(0, (INT64)m_test);
    BL_String destPath;
    m_test->getDestPath(destPath);
}

TEST_F(VR_PhoneBookListener_Test, Test_isSupportedNavi)
{
    ASSERT_NE(0, (INT64)m_test);
    EXPECT_TRUE(m_test->isSupportedNavi());
}

} // dataprovider
} // navi

/* EOF */
