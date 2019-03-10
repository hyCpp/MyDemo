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
 * @file Voice_Asr_Impl_IFlyTek_Mgr_Data.h
 * @brief C_Mgr_Data class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_IFlyTek_Callback_Mock_h__
#define __Vr_Asr_Impl_IFlyTek_Callback_Mock_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

class C_IFlyTek_Mock;
class C_IFlytek_Thread_Callback_Mock;

/**
 * @brief The C_Mgr_Data class
 *
 * C_Mgr_Data class
 */

class C_IFlyTek_Callback_Mock
{
public:

    C_IFlyTek_Mock & m_ro_IFlyTek_Mock;

    int m_i_callback_type;

    ~C_IFlyTek_Callback_Mock(); // [F]Destructor

    explicit
    C_IFlyTek_Callback_Mock // [F]Constructor of Default
    (
        C_IFlyTek_Mock & ro_IFlyTek_Mock
    );

    int
    M_Fn_Init();

    int
    M_Fn_Fina();

    void
    M_Fn_Callback_Msg(int i_callback_type);

    void
    M_Fn_On_Thread_Notified();

protected:

    int
    _M_Fn_Thread_Init();

    int
    _M_Fn_Thread_Fina();


    C_IFlyTek_Callback_Mock // [F]Constructor: Copy
    (
        C_IFlyTek_Callback_Mock const & // [I]
    );

    C_IFlyTek_Callback_Mock & // [R]
    operator = // [F]Operator: =
    (
        C_IFlyTek_Callback_Mock const & // [I]
    );

    scoped_ptr<C_IFlytek_Thread_Callback_Mock> m_scpo_IFlytek_Thread_Callback_Mock;

};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */