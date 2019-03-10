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
 * @file Vr_Asr_Impl_Fn_Get_ro_LH_AUDIOIN_INTERFACE.h
 * @brief interface define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Fn_Get_string_Path_h__
#define __Vr_Asr_Impl_Fn_Get_string_Path_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Get_Path class
 *
 * C_Get_Path class
 */

class C_Get_Path
{
  public:

    virtual
    ~C_Get_Path(); // [F]Destructor

    explicit
    C_Get_Path(); // [F]Constructor of Default

    virtual
    string
    M_Fn_Get_string_Path_VData();

    virtual
    string
    M_Fn_Get_string_Path_PData();

    virtual
    string
    M_Fn_Get_string_Path_Usr();

  PROTECTED:

    C_Get_Path // [F]Constructor: Copy
    (
        C_Get_Path const & // [I]
    );

    C_Get_Path & // [R]
    operator = // [F]Operator: =
    (
        C_Get_Path const & // [I]
    );
};

extern scoped_ptr<C_Get_Path> g_scp_o_Get_Path;

inline
C_Get_Path &
G_o_Get_Path()
{
    return *g_scp_o_Get_Path;
}

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
