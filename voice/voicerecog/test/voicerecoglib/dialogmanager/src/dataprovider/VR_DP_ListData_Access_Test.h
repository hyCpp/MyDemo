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
 * @file VR_DP_ListData_Access_Test.h
 * @brief Declaration file of class VR_DP_ListData_Access_Test.
 *
 * This file includes the declaration of class VR_DP_ListData_Access_Test.
 *
 * @attention used for C++ only.
 */
#ifndef VR_DP_LISTDATA_ACCESS_TEST_H
#define VR_DP_LISTDATA_ACCESS_TEST_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DP_ListData_Access.h"
#include "gtest/gtest.h"

namespace navi
{
namespace dataprovider
{
/**
 * @brief The VR_DP_ListData_Access_Test class
 *
 * class declaration.
 */
class VR_DP_ListData_Access_Test : public testing::Test
{
public:
    static VOID SetUpTestCase();
    static VOID TearDownTestCase();

protected:
    VR_DP_ListData_Access_Test();
    ~VR_DP_ListData_Access_Test();

private:
    VR_DP_ListData_Access_Test(const VR_DP_ListData_Access_Test&);
    VR_DP_ListData_Access_Test& operator=(const VR_DP_ListData_Access_Test&);

    // test suite level
    static VR_DP_ListData_Access* m_testSuite;
};

}
}
#endif /* VR_DP_LISTDATA_ACCESS_H */
/* EOF */
