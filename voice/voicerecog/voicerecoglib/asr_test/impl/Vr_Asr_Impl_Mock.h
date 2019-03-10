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
 * @file Vr_Asr_Impl_Mock.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Mock_h__
#define __Vr_Asr_Impl_Mock_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Check class
 *
 * C_Check class
 */

class C_Check
{
  public:

    ~C_Check(); // [F]Destructor

    explicit
    C_Check(); // [F]Constructor of Default

    bool // [R]
    operator() // [F]Operator: =
    (
        bool, // [I]
        char const * const pi_Function
    );

    int m_b_Specify_Function;
    string m_string_Name_Function;
    vector<int> m_vector_i_Check;
    size_t m_i_Pos;
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
