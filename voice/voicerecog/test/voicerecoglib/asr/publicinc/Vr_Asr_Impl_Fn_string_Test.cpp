/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

// #define private public
// #define protected public

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Fn_string.h"
#include "Vr_Asr_Impl_Fn_string_Test.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

using ::testing::Return;
using ::testing::_;

Fn_string_Test::Fn_string_Test()
{
}

Fn_string_Test::~Fn_string_Test()
{
}

void Fn_string_Test::SetUp()
{
}

void Fn_string_Test::TearDown()
{
}

TEST_F(Fn_string_Test, Fn_Get_string_Without_Blank)
{
    string str_test("Hello world, nice to meet you!");
    Fn_Get_string_Without_Blank(str_test);

    str_test.assign("Good!");
    Fn_Get_string_Without_Blank(str_test);

    str_test.clear();
    Fn_Get_string_Without_Blank(str_test);
}

TEST_F(Fn_string_Test, Fn_Is_Utf8_String)
{
    string str_test{ 0x70, 0x71, 0x72, 0x80, 0x81, 0x82, 0xf0, 0xf1, 0xf2, 0xfe, 0xfe, 0xff };
    Fn_Is_Utf8_String(str_test);

    str_test.clear();
    Fn_Is_Utf8_String(str_test);
}

} // namespace N_Impl

} // namespace N_Asr

} // namespace N_Vr

/* EOF */