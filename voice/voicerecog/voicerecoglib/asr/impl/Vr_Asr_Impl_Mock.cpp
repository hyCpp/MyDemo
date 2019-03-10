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

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Impl_Mock.h"

bool
Fn_Check_Real
(
    bool b,
    char const * const pi_Function
)
{
    return b;
}

function<bool(bool, char const * const)> g_function_Check(Fn_Check_Real);

bool
Fn_Check
(
    bool b,
    char const * const pi_Function
)
{
    return g_function_Check(b, pi_Function);
}

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

C_Check::C_Check()
: m_b_Specify_Function(false)
, m_string_Name_Function()
, m_i_Pos(0)
{

}

C_Check::~C_Check()
{

}

bool
C_Check::operator()
(
    bool b,
    char const * const pi_Function
)
{
    if (m_b_Specify_Function) {
        string string_Name_Function(pi_Function);
        if (string_Name_Function.find(m_string_Name_Function) == string::npos) {
            return b;
        }
    }

    bool b_Return = false;

    if (m_i_Pos < m_vector_i_Check.size()) {
        b_Return = m_vector_i_Check[m_i_Pos] != 0;
    }

    ++m_i_Pos;

    return b_Return;
}

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */

