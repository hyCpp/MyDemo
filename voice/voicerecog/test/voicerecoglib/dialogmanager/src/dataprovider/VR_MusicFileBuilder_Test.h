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
 * @file VR_MusicFileBuilder_Test.h
 * @brief Declaration file of class VR_MusicFileBuilder_Test.
 *
 * This file includes the declaration of class VR_MusicFileBuilder_Test.
 *
 * @attention used for C++ only.
 */
#ifndef VR_MUSIC_FILE_BUILDER_TEST_H
#define VR_MUSIC_FILE_BUILDER_TEST_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_MusicFileBuilder.h"
#include "VR_MusicDBHelperManager.h"
#include "gtest/gtest.h"

namespace navi
{
namespace dataprovider
{
/**
 * @brief The VR_MusicFileBuilder_Test class
 *
 * class declaration.
 */
class VR_MusicFileBuilder_Test : public testing::Test
{
public:
    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

protected:
    VR_MusicFileBuilder_Test();
    ~VR_MusicFileBuilder_Test();
    VOID SetUp();
    VOID TearDown();

private:
    VR_MusicFileBuilder_Test(const VR_MusicFileBuilder_Test&);
    VR_MusicFileBuilder_Test& operator=(const VR_MusicFileBuilder_Test&);

    // test suite level
    static VR_MusicFileBuilder* m_testSuite;

    // test level
    VR_MusicFileBuilder* m_test;
};

}
}
#endif /* VR_MUSIC_FILE_BUILDER_TEST_H */
/* EOF */
