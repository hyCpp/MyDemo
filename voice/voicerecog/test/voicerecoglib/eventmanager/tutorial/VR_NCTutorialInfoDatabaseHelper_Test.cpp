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

#include "gtest/gtest.h"

#include "VR_NCTutorialInfoDatabaseHelper.h"

#ifndef VR_DEF_H
#include "VR_Def.h"
#endif

#ifndef NCFILE_H
#include "ncore/NCFile.h"
#endif

namespace nutshell
{
namespace tutorial
{
/**
 * @brief The VR_NCTutorialInfoDatabaseHelper_Test class
 *
 * class declaration.
 */
class VR_NCTutorialInfoDatabaseHelper_Test : public testing::Test
{
public:
    virtual VOID SetUp();
    virtual VOID TearDown();

protected:
    VR_NCTutorialInfoDatabaseHelper_Test();
    ~VR_NCTutorialInfoDatabaseHelper_Test();

private:
    VR_NCTutorialInfoDatabaseHelper_Test(const VR_NCTutorialInfoDatabaseHelper_Test&);
    VR_NCTutorialInfoDatabaseHelper_Test& operator=(const VR_NCTutorialInfoDatabaseHelper_Test&);

    /* test suite level */
    VR_NCTutorialInfoDatabaseHelper * m_testSuite = NULL;
};

VR_NCTutorialInfoDatabaseHelper_Test::VR_NCTutorialInfoDatabaseHelper_Test()
{
}

VR_NCTutorialInfoDatabaseHelper_Test::~VR_NCTutorialInfoDatabaseHelper_Test()
{
}

VOID VR_NCTutorialInfoDatabaseHelper_Test::SetUp()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_new VR_NCTutorialInfoDatabaseHelper("/tmp/tutorialprovider.db", 2);
    }
}

VOID VR_NCTutorialInfoDatabaseHelper_Test::TearDown()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
        m_testSuite = NULL;
    }
}

TEST_F(VR_NCTutorialInfoDatabaseHelper_Test, onCreateException)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->onCreate(NULL);
    m_testSuite->createTable(NULL);

    NCSQLiteDatabase dbAccess("/tmp/tutorialprovider.db", 0x01, NULL, 0);
    dbAccess.close();
    m_testSuite->onCreate(&dbAccess);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoDatabaseHelper_Test, onUpgradeException)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->onUpgrade(NULL, 2, 3);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoDatabaseHelper_Test, onDowngradeException)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->onDowngrade(NULL, 2, 1);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoDatabaseHelper_Test, onOpenException)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->onOpen(NULL);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoDatabaseHelper_Test, getDatabase)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    NCSQLiteDatabase* db = m_testSuite->getDatabase();
    if (NULL == db) {
        SUCCEED();
        return;
    }

    // trigger onUpgrade, set DBFileVersion=1 DBHelperOpenVersion=2
    db->setVersion(1);
    db->close();
    db = m_testSuite->getDatabase();
    if (NULL == db) {
        SUCCEED();
        return;
    }

    // trigger onDowngrade, set DBFileVersion=3 DBHelperOpenVersion=2
    db->setVersion(3);
    db->close();
    db = m_testSuite->getDatabase();
    if (NULL == db) {
        SUCCEED();
        return;
    }
}

}
}
/* EOF */
