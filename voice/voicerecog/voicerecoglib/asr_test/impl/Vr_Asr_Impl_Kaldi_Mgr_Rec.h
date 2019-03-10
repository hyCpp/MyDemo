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
 * @file Vv_Asr_Impl_Kaldi_Mgr_Rec.h
 * @brief C_Mgr_Rec class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Kaldi_Mgr_Rec_h__
#define __Vr_Asr_Impl_Kaldi_Mgr_Rec_h__

#include "Vr_Asr_Impl_Kaldi_Fn_Get_ro_AUDIOIN_INTERFACE.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

class C_Engine_Agent;

namespace N_Kaldi
{

class C_Thread_Rec;
class C_Engine_Concrete;

/**
 * @brief The C_Mgr_Rec class
 *
 * C_Mgr_Rec class
 */

class C_Mgr_Rec
{
  public:

    enum E_State_Rec
    {
        E_State_Rec_Run_Req,
        E_State_Rec_Run_Ack,
        E_State_Rec_Idle_Req,
        E_State_Rec_Idle_Ack,
    };

    C_Engine_Concrete & m_ro_Engine_Concrete;

    ~C_Mgr_Rec(); // [F]Destructor

    explicit
    C_Mgr_Rec // [F]Constructor of Default
    (
        C_Engine_Concrete & ro_Engine_Concrete
    );

    E_Result
    M_Fn_Init();

    E_Result
    M_Fn_Fina();

    E_Result
    M_Fn_Start
    (
        C_Request_Recognize & ro_Request_Recognize
    );

    E_Result
    M_Fn_Cancel();

    void
    M_Fn_On_Thread_Notified();

    E_Result
    M_Fn_On_Callback_Advance
    (
        bool * po_b_Continue
    );

    E_Result
    M_Fn_On_Callback_Event
    (
        unsigned long i_Type
    );

  PROTECTED:

    E_Result
    _M_Fn_Thread_Init();

    E_Result
    _M_Fn_Thread_Fina();

    E_Result
    _M_Fn_Proc();

    E_Result
    _M_Fn_Proc_Audio_400ms
    (
        KALDI_AUDIOIN_INTERFACE& audio_interface,
        void * pvoid_Audio_In
    );

    C_Mgr_Rec // [F]Constructor: Copy
    (
        C_Mgr_Rec const & // [I]
    );

    C_Mgr_Rec & // [R]
    operator = // [F]Operator: =
    (
        C_Mgr_Rec const & // [I]
    );

    C_Mutex m_o_Mutex_State;

    E_State_Rec m_e_State_Rec;

    scoped_ptr<C_Request_Recognize> m_scpo_Request_Recognize;

    scoped_ptr<C_Thread_Rec> m_scpo_Thread_Rec;

    int m_b_VOD_BOS;
    int m_b_VOD_TSF;
    int m_b_VOD_TSR;

    C_Time m_o_Time_VOD_BOS;
    C_Time m_o_Time_VOD_TSF;
    C_Time m_o_Time_VOD_TSR;
    C_Time m_o_Time_Result;
    C_Time m_o_Time_Begin;
    C_Time m_o_Time_End;
};

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
