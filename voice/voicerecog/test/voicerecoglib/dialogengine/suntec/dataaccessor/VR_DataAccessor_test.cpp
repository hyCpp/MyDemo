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
#include "VR_DataAccessorContact.h"

/**
* VR_DataAccessor_Test
*
* The class is just for VR_DataAccessor test.
*/
class VR_DataAccessor_Test : public testing::Test
{
public:
    VR_DataAccessor_Test()
    {
    }

    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    virtual void SetUp() {}
    virtual void TearDown() {}

private:
    VR_DataAccessor_Test(const VR_DataAccessor_Test& other);
    void operator= (const VR_DataAccessor_Test& other);
};

TEST_F(VR_DataAccessor_Test, getPhoneTypeName)
{
    // param of getPhoneTypeName 0,1,2,3
    VR_DataAccessorContact accessor;
    EXPECT_STREQ("mobile", accessor.getPhoneTypeName("0").c_str());
    EXPECT_STREQ("home", accessor.getPhoneTypeName("1").c_str());
    EXPECT_STREQ("work", accessor.getPhoneTypeName("2").c_str());
    EXPECT_STREQ("other", accessor.getPhoneTypeName("3").c_str());

    // uncorrect param
    EXPECT_STREQ("home", accessor.getPhoneTypeName("5").c_str());
    EXPECT_STREQ("other", accessor.getPhoneTypeName("-1").c_str());
    EXPECT_STREQ("mobile", accessor.getPhoneTypeName("abc").c_str());
    EXPECT_STREQ("mobile", accessor.getPhoneTypeName("").c_str());
}

TEST_F(VR_DataAccessor_Test, setPhoneTypeName)
{
    // param of getPhoneTypeName 0,1,2,3
    VR_DataAccessorContact accessor;
    EXPECT_STREQ("mobile", accessor.getPhoneTypeName("0").c_str());
    EXPECT_STREQ("home", accessor.getPhoneTypeName("1").c_str());
    EXPECT_STREQ("work", accessor.getPhoneTypeName("2").c_str());
    EXPECT_STREQ("other", accessor.getPhoneTypeName("3").c_str());

    // uncorrect param
    accessor.setPhoneTypeName(0, "MOBILE_TEST");
    EXPECT_STREQ("MOBILE_TEST", accessor.getPhoneTypeName("0").c_str());
    EXPECT_STREQ("home", accessor.getPhoneTypeName("1").c_str());
    EXPECT_STREQ("work", accessor.getPhoneTypeName("2").c_str());
    EXPECT_STREQ("other", accessor.getPhoneTypeName("3").c_str());

    accessor.setPhoneTypeName(1, "HOME_TEST");
    EXPECT_STREQ("MOBILE_TEST", accessor.getPhoneTypeName("0").c_str());
    EXPECT_STREQ("HOME_TEST", accessor.getPhoneTypeName("1").c_str());
    EXPECT_STREQ("work", accessor.getPhoneTypeName("2").c_str());
    EXPECT_STREQ("other", accessor.getPhoneTypeName("3").c_str());

    accessor.setPhoneTypeName(2, "WORK_TEST");
    EXPECT_STREQ("MOBILE_TEST", accessor.getPhoneTypeName("0").c_str());
    EXPECT_STREQ("HOME_TEST", accessor.getPhoneTypeName("1").c_str());
    EXPECT_STREQ("WORK_TEST", accessor.getPhoneTypeName("2").c_str());
    EXPECT_STREQ("other", accessor.getPhoneTypeName("3").c_str());

    accessor.setPhoneTypeName(3, "OTHER_TEST");
    EXPECT_STREQ("MOBILE_TEST", accessor.getPhoneTypeName("0").c_str());
    EXPECT_STREQ("HOME_TEST", accessor.getPhoneTypeName("1").c_str());
    EXPECT_STREQ("WORK_TEST", accessor.getPhoneTypeName("2").c_str());
    EXPECT_STREQ("OTHER_TEST", accessor.getPhoneTypeName("3").c_str());

    accessor.setPhoneTypeName(0, "VOMOBILE");
    accessor.setPhoneTypeName(1, "VOHOME");
    accessor.setPhoneTypeName(2, "VOWORK");
    accessor.setPhoneTypeName(3, "VOOTHER");
    EXPECT_STREQ("mobile", accessor.getPhoneTypeName("0").c_str());
    EXPECT_STREQ("home", accessor.getPhoneTypeName("1").c_str());
    EXPECT_STREQ("work", accessor.getPhoneTypeName("2").c_str());
    EXPECT_STREQ("other", accessor.getPhoneTypeName("3").c_str());
}

/* EOF */