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

/**
 * @file Vr_Asr_Impl_Fn_string_Test.h
 * @brief Declaration file of class Fn_string_Test.
 *
 * This file includes the declaration of class Fn_string_Test.
 *
 * @attention used for C++ only.
 */

#ifndef _Vr_Asr_Impl_Fn_string_Test_h_
#define _Vr_Asr_Impl_Fn_string_Test_h_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The Fn_string_Test class
 *
 * Fn_string_Test class
 */

class Fn_string_Test : public testing::Test
{
public:
    Fn_string_Test();
    virtual ~Fn_string_Test();

    virtual void SetUp();
    virtual void TearDown();

private:
//    Fn_string_Test(const Fn_string_Test&);
//    Fn_string_Test& operator=(const Fn_string_Test);


};

} // namespace N_Impl

} // namespace N_Asr

} // namespace N_Vr

#endif // #ifndef _Vr_Asr_Impl_Fn_string_Test_h_
/* EOF */