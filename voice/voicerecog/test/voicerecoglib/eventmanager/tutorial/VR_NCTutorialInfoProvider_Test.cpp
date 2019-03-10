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

#include "VR_NCTutorialInfoProvider.h"

#ifndef VR_DEF_H
#include "VR_Def.h"
#endif

#ifndef NCSTRING_H
#include "NCString.h"
#endif

namespace nutshell
{
namespace tutorial
{
#define VR_TUTORIAL_TABLE_OF_INVALID_URI    "content:://tutorialinfo/tutorial_invalid/data"
#define VR_TUTORIAL_TABLE_OF_CATEGORY_URI   "content://tutorialinfo/tutorial_category/data"
#define VR_TUTORIAL_TABLE_OF_INFO_URI       "content://tutorialinfo/tutorial_info/data"
#define VR_TUTORIAL_TABLE_OF_SCREENINFO_URI "content://tutorialinfo/tutorial_screen_info/data"

/**
 * @brief The VR_NCTutorialInfoProvider_Test class
 *
 * class declaration.
 */
class VR_NCTutorialInfoProvider_Test : public testing::Test
{
public:
    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

protected:
    VR_NCTutorialInfoProvider_Test();
    ~VR_NCTutorialInfoProvider_Test();

private:
    VR_NCTutorialInfoProvider_Test(const VR_NCTutorialInfoProvider_Test&);
    VR_NCTutorialInfoProvider_Test& operator=(const VR_NCTutorialInfoProvider_Test&);

    // test suite level
    static VR_NCTutorialInfoProvider* m_testSuite;
};

VR_NCTutorialInfoProvider* VR_NCTutorialInfoProvider_Test::m_testSuite = NULL;

VR_NCTutorialInfoProvider_Test::VR_NCTutorialInfoProvider_Test()
{
}

VR_NCTutorialInfoProvider_Test::~VR_NCTutorialInfoProvider_Test()
{
}

VOID VR_NCTutorialInfoProvider_Test::SetUpTestCase()
{
    if (NULL == m_testSuite) {
        m_testSuite = VR_new VR_NCTutorialInfoProvider("nutshell.voice.voicerecognizer");
    }
}

VOID VR_NCTutorialInfoProvider_Test::TearDownTestCase()
{
    if (NULL != m_testSuite) {
        delete m_testSuite;
        m_testSuite = NULL;
    }
}

TEST_F(VR_NCTutorialInfoProvider_Test, NULLDestruction)
{
    VR_NCTutorialInfoProvider provider("nutshell.voice.voicerecognizer");
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, queryException1)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->query(NCUri(VR_TUTORIAL_TABLE_OF_INVALID_URI), NCList<NCString>(),
                       NCString(), NCList<NCString>(), NCString());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, queryException2)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->m_MatchRelate[0] = "tutorial_invalid";
    m_testSuite->query(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), NCList<NCString>(),
                       NCString(), NCList<NCString>(), NCString());
    m_testSuite->m_MatchRelate[0] = VR_TABLE_OF_TUTORIAL_CATEGORY;
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, queryCategory)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->query(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), NCList<NCString>(),
                       NCString(), NCList<NCString>(), NCString());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, insertException1)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "America");
    values.put("language", "en-us");
    values.put("machine_type", "T0");
    m_testSuite->insert(NCUri(VR_TUTORIAL_TABLE_OF_INVALID_URI), values);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, insertException2)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "America");
    values.put("language", "en-us");
    values.put("machine_type", "T0");
    values.put("unkown_field", "unkown");
    m_testSuite->insert(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, insertCategory)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "America");
    values.put("language", "en-us");
    values.put("machine_type", "T0");
    m_testSuite->insert(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, updateException1)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "America");
    values.put("language", "en-us");
    values.put("machine_type", "T1");

    NCString ArgOne = "1";
    NCList<NCString> whereArgs;
    whereArgs << &ArgOne;
    m_testSuite->update(NCUri(VR_TUTORIAL_TABLE_OF_INVALID_URI), values,
                        " category_index = ? ", whereArgs);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, updateCategory)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "Mexico");
    values.put("language", "en-us");
    values.put("machine_type", "T1");

    m_testSuite->update(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values,
                        NCString(), NCList<NCString>());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, updateCategoryByCondition)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "America");
    values.put("language", "en-us");
    values.put("machine_type", "T1");

    NCString ArgOne = "1";
    NCList<NCString> whereArgs;
    whereArgs << &ArgOne;
    m_testSuite->update(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values,
                        " category_index = ? ", whereArgs);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, updateCategoryNotExist)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("category_index", "2");
    values.put("state", "America");
    values.put("language", "en-us");
    values.put("machine_type", "T2");

    NCString ArgOne = "2";
    NCList<NCString> whereArgs;
    whereArgs << &ArgOne;
    m_testSuite->update(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values,
                        " category_index = ? ", whereArgs);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, removeException)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    m_testSuite->remove(NCUri(VR_TUTORIAL_TABLE_OF_INVALID_URI), NCString(), NCList<NCString>());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, removeCategory)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    m_testSuite->remove(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), NCString(), NCList<NCString>());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, removeCategoryRepeat)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    m_testSuite->remove(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), NCString(), NCList<NCString>());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, getType)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->getType(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI));
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, onInitialize)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->onInitialize();
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, onStart)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->onStart();
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, onStop)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->onStop();
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, onDeinitialize)
{
    ASSERT_NE(0, (INT64)m_testSuite);
    m_testSuite->onDeinitialize();
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, insertDBHelperIsNull)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    if (NULL != m_testSuite->m_dbHelper) {
        delete m_testSuite->m_dbHelper;
        m_testSuite->m_dbHelper = NULL;
    }

    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "America");
    values.put("language", "en-us");
    values.put("machine_type", "T0");
    m_testSuite->insert(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, updateDBHelperIsNull)
{
    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "Mexico");
    values.put("language", "en-us");
    values.put("machine_type", "T1");

    m_testSuite->update(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values,
                        NCString(), NCList<NCString>());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, removeDBHelperIsNull)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    m_testSuite->remove(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), NCString(), NCList<NCString>());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, queryDBHelperInvalid)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCCHAR* invalidDBPath = "/unkown/tutorialprovider.db";
    m_testSuite->m_dbHelper = VR_new VR_NCTutorialInfoDatabaseHelper(invalidDBPath, 1);
    m_testSuite->query(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), NCList<NCString>(),
                       NCString(), NCList<NCString>(), NCString());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, insertDBHelperInvalid)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "America");
    values.put("language", "en-us");
    values.put("machine_type", "T0");
    m_testSuite->insert(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values);
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, updateDBHelperInvalid)
{
    NCContentValues values;
    values.put("category_index", "1");
    values.put("state", "Mexico");
    values.put("language", "en-us");
    values.put("machine_type", "T1");

    m_testSuite->update(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), values,
                        NCString(), NCList<NCString>());
    SUCCEED();
}

TEST_F(VR_NCTutorialInfoProvider_Test, removeDBHelperInvalid)
{
    ASSERT_NE(0, (INT64)m_testSuite);

    m_testSuite->remove(NCUri(VR_TUTORIAL_TABLE_OF_CATEGORY_URI), NCString(), NCList<NCString>());
    SUCCEED();
}

}
}
/* EOF */
