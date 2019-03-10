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
 * @file Vr_Asr_Impl_Kaldi_Kaldi.h
 * @brief C_Vocon class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Kaldi_h__
#define __Vr_Asr_Impl_Kaldi_h__

#ifdef _KALDI

#include "kaldi_engine.h"

#endif

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Kaldi
{

/**
 * @brief The C_Kaldi class
 *
 * C_Kaldi class
 */

class C_Kaldi
{
  public:

    virtual
    ~C_Kaldi(); // [F]Destructor

    explicit
    C_Kaldi(); // [F]Constructor of Default

    virtual
    int
    M_Fn_Kaldi_Create
    (
        KALDI_EVENT_INTERFACE* pKALDI_EVENT_INTERFACE,
        void* p_User_Args,
        KALDI_HANDLE* o_KALDI_HANDLE
    )
    {
        return Kaldi_Create(pKALDI_EVENT_INTERFACE, p_User_Args, o_KALDI_HANDLE);
    }

    virtual
    int
    M_Fn_Kaldi_Destory(KALDI_HANDLE o_KALDI_HANDLE)
    {
        return Kaldi_Destory(o_KALDI_HANDLE);
    }

    virtual
    int
    M_Fn_Kaldi_Param_Set
    (
        KALDI_HANDLE o_KALDI_HANDLE,
        E_Kaldi_Param_Type e_Param_Type,
        double d_Param_Value
    )
    {
        return Kaldi_Param_Set(o_KALDI_HANDLE, e_Param_Type, d_Param_Value);
    }

    virtual
    int
    M_Fn_Kaldi_Model_Load
    (
        KALDI_HANDLE o_KALDI_HANDLE,
        E_Kaldi_Mode_Type e_Mode_Type,
        std::string string_Mode
    )
    {
        return Kaldi_Model_Load(o_KALDI_HANDLE, e_Mode_Type, string_Mode);
    }

    virtual
    int
    M_Fn_Kaldi_Recognize_Start
    (
        KALDI_HANDLE o_KALDI_HANDLE,
        KALDI_AUDIOIN_INTERFACE* pKALDI_AUDIOIN_INTERFACE,
        void* p_Audio_Inst
    )
    {
        return Kaldi_Recognize_Start(o_KALDI_HANDLE, p_Audio_Inst, pKALDI_AUDIOIN_INTERFACE);
    }

    virtual
    int
    M_Fn_Kaldi_Get_Recognize_Result
    (
        KALDI_HANDLE o_KALDI_HANDLE,
        std::string& o_string_result
    )
    {
        return Kaldi_Get_Recognize_Result(o_KALDI_HANDLE, o_string_result);
    }

protected:

    C_Kaldi // [F]Constructor: Copy
    (
        C_Kaldi const & // [I]
    );

    C_Kaldi & // [R]
    operator = // [F]Operator: =
    (
        C_Kaldi const & // [I]
    );

};

extern scoped_ptr<C_Kaldi> g_scp_o_Kaldi;

inline
C_Kaldi &
G_o_Kaldi()
{
    return *g_scp_o_Kaldi;
}

} // name N_IFlytek

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
