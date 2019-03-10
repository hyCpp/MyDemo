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

#ifndef __Vr_Asr_Impl_IFlyTek_Mgr_Data_h__
#define __Vr_Asr_Impl_IFlyTek_Mgr_Data_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

class C_Engine_Agent;

namespace N_IFlyTek
{

class C_Thread_Data;
class C_Engine_Concrete;

/**
 * @brief The C_Mgr_Data class
 *
 * C_Mgr_Data class
 */

class C_Mgr_Data
{
  public:

    C_Engine_Concrete & m_ro_Engine_Concrete;

    int m_b_Audio_Close;

    boost::shared_ptr<C_Audio_In> m_spo_Audio_In;
    boost::function<void(C_Event_Phase const &)> m_function_On_Event_Phase;

    ~C_Mgr_Data(); // [F]Destructor

    explicit
    C_Mgr_Data // [F]Constructor of Default
    (
        C_Engine_Concrete & ro_Engine_Concrete
    );

    E_Result
    M_Fn_Init();

    E_Result
    M_Fn_Fina();

    void
    M_Fn_On_Thread_Notified();


    E_Result
    M_Fn_Start_Audio();

    E_Result
    M_Fn_Stop_Audio();

PROTECTED:

    E_Result
    _M_Fn_Thread_Init();

    E_Result
    _M_Fn_Thread_Fina();

    E_Result
    _M_Fn_Append_Audio_Data
    (
        void* p_Buffer,
        unsigned int i_Buffer_Size,
        bool& b_Speech_End
    );


    C_Mgr_Data // [F]Constructor: Copy
    (
        C_Mgr_Data const & // [I]
    );

    C_Mgr_Data & // [R]
    operator = // [F]Operator: =
    (
        C_Mgr_Data const & // [I]
    );

    scoped_ptr<C_Thread_Data> m_scpo_Thread_Data;

    C_Mutex m_o_Mutex_Audio_In_Request;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
