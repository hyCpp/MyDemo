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
#include "Vr_Asr_Impl_Event_Phase_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl.h"
#include "Vr_Asr_Impl_Event_Notify_Impl_Signal_Level.h"
#include "Vr_Asr_Impl_Event_Result_Impl.h"
#include "Vr_Asr_Impl_Kaldi_Thread_Rec.h"
#include "Vr_Asr_Impl_Kaldi_Mgr_Rec.h"
#include "Vr_Asr_Impl_Kaldi_Engine_Concrete.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Kaldi
{

C_Mgr_Rec::~C_Mgr_Rec()
{
    MACRO_Log_Function
}

C_Mgr_Rec::C_Mgr_Rec
(
    C_Engine_Concrete & ro_Engine_Concrete
)
: m_ro_Engine_Concrete(ro_Engine_Concrete)
, m_o_Mutex_State()
, m_e_State_Rec(E_State_Rec_Idle_Ack)
, m_scpo_Request_Recognize()
, m_scpo_Thread_Rec()
, m_b_VOD_BOS(false)
, m_b_VOD_TSF(false)
, m_b_VOD_TSR(false)
, m_o_Time_VOD_BOS()
, m_o_Time_VOD_TSF()
, m_o_Time_VOD_TSR()
, m_o_Time_Result()
, m_o_Time_Begin()
, m_o_Time_End()
{
    MACRO_Log_Function
}

E_Result
C_Mgr_Rec::M_Fn_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = _M_Fn_Thread_Init();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::M_Fn_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = _M_Fn_Thread_Fina();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

void
C_Mgr_Rec::M_Fn_On_Thread_Notified()
{
    MACRO_Log_Function

    E_Result e_Result;

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        IF (m_e_State_Rec == E_State_Rec_Run_Req) {

            m_e_State_Rec = E_State_Rec_Run_Ack;
        }
        ELSE
        IF (m_e_State_Rec == E_State_Rec_Idle_Req) {

            // Do nothing.
        }
        ELSE {

            stringstream stringstream_Log;
            stringstream_Log << "    State: " << static_cast<int>(m_e_State_Rec);
            Fn_Log(stringstream_Log.str());
            MACRO_Log_Error
            return;
        }
    }

    IF (!m_ro_Engine_Concrete.m_b_Init) {
        MACRO_Log_Error
        return;
    }

    IF (m_ro_Engine_Concrete.m_b_Error) {
        MACRO_Log_Error
        return;
    }

    IF (!m_scpo_Request_Recognize) {
        MACRO_Log_Error
        return;
    }

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_Begin)
    );

    m_o_Time_Begin = C_Time::M_Fn_Get_Time();

    m_b_VOD_BOS = false;
    m_b_VOD_TSF = false;
    m_b_VOD_TSR = false;

    e_Result = _M_Fn_Proc();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        m_ro_Engine_Concrete.m_b_Error = true;
    }

    m_o_Time_End = C_Time::M_Fn_Get_Time();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Log("");
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    IF (m_b_VOD_BOS) {
        m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_VOD_BOS - m_o_Time_Begin);
    }
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    IF (m_b_VOD_TSF) {
        m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_VOD_TSF - m_o_Time_Begin);
    }
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    IF (m_b_VOD_TSR) {
        m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_VOD_TSR - m_o_Time_Begin);
    }
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_Result - m_o_Time_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Tab();

    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Time(m_o_Time_End - m_o_Time_Begin);
    m_ro_Engine_Concrete.m_o_Log_File.M_Fn_Enter();

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        IF ((m_e_State_Rec != E_State_Rec_Run_Ack) && (m_e_State_Rec != E_State_Rec_Idle_Req)) {
            stringstream stringstream_Log;
            stringstream_Log << "    State: " << static_cast<int>(m_e_State_Rec);
            Fn_Log(stringstream_Log.str());
            MACRO_Log_Error
        }

        m_e_State_Rec = E_State_Rec_Idle_Ack;
    }

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Proc_End)
    );
}

E_Result
C_Mgr_Rec::M_Fn_On_Callback_Advance
(
    bool * po_b_Continue
)
{
    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        *po_b_Continue = (m_e_State_Rec != E_State_Rec_Idle_Req) ? true : false;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::M_Fn_On_Callback_Event
(
    unsigned long i_Type
)
{
/*
    if (1 == i_Type) {
        Fn_Log("    LH_AUDIOCHAIN_EVENT_TS_FX");
        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_End_Fx)
        );
    }

    if (2 == i_Type) {
        string string_result;
        G_o_Kaldi().M_Fn_Kaldi_Get_Recognize_Result(
            m_ro_Engine_Concrete.M_Fn_Get_ro_LH_OBJECT_Rec(),
            string_result);

        Fn_Log("    Lkaldi result: " + string_result);
    }
*/
    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::M_Fn_Start
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    MACRO_Log_Function

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        IF (m_e_State_Rec != E_State_Rec_Idle_Ack) {
            stringstream stringstream_Log;
            stringstream_Log << "    State: " << static_cast<int>(m_e_State_Rec);
            Fn_Log(stringstream_Log.str());
            MACRO_Log_Error
            return E_Result_Failed;
        }

        m_e_State_Rec = E_State_Rec_Run_Req;
    }

    m_scpo_Request_Recognize.reset(
        MACRO_New C_Request_Recognize(
            ro_Request_Recognize
        )
    );

    m_scpo_Thread_Rec->Notify();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::M_Fn_Cancel()
{
    MACRO_Log_Function

    {
        C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_State);

        IF (m_e_State_Rec == E_State_Rec_Idle_Ack) {
            return E_Result_Succeeded;
        }

        m_e_State_Rec = E_State_Rec_Idle_Req;
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Thread_Init()
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Thread_Rec) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    m_scpo_Thread_Rec.reset(MACRO_New C_Thread_Rec(*this));

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    m_scpo_Thread_Rec->RegisterName("VR_ASR_REC_THREAD");
#endif

    m_scpo_Thread_Rec->StartRegistThread();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Thread_Fina()
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Thread_Rec) {
        m_scpo_Thread_Rec->StopThread();
        m_scpo_Thread_Rec.reset();
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc()
{
    MACRO_Log_Function

    E_Result e_Result = E_Result_Succeeded;

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Listen_Begin)
    );


    KALDI_AUDIOIN_INTERFACE audio_interface = Fn_Get_ro_KALDI_AUDIOIN_INTERFACE();
    void * pvoid_Audio_In = m_scpo_Request_Recognize->m_spo_Audio_In.get();

    int i_ret = audio_interface.pfstart(pvoid_Audio_In);
    if (i_ret != 0) {
    }

    do {

        _M_Fn_Proc_Audio_400ms(audio_interface, pvoid_Audio_In);

        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_Begin_Fx)
        );

        _M_Fn_Proc_Audio_400ms(audio_interface, pvoid_Audio_In);
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl_Signal_Level(3000, 300)
        );

        _M_Fn_Proc_Audio_400ms(audio_interface, pvoid_Audio_In);
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl_Signal_Level(4000, 300)
        );

        _M_Fn_Proc_Audio_400ms(audio_interface, pvoid_Audio_In);
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl_Signal_Level(5000, 300)
        );

        _M_Fn_Proc_Audio_400ms(audio_interface, pvoid_Audio_In);
        m_scpo_Request_Recognize->m_function_On_Event_Notify(
            C_Event_Notify_Impl_Signal_Level(6000, 300)
        );

        m_scpo_Request_Recognize->m_function_On_Event_Phase(
            C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Speech_End_Fx)
        );
    } while (0);

    audio_interface.pfstop(pvoid_Audio_In);

    C_Event_Result_Impl o_Event_Result_Impl;
    shared_ptr<string> m_spstring_Result_XML;
    m_spstring_Result_XML.reset(MACRO_New string);
    *m_spstring_Result_XML = string("asr test");
    o_Event_Result_Impl.m_spstring_Result_XML = m_spstring_Result_XML;
    m_scpo_Request_Recognize->m_function_On_Event_Result(
        o_Event_Result_Impl
    );

    m_scpo_Request_Recognize->m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Listen_End)
    );

    return e_Result;
}

E_Result
C_Mgr_Rec::_M_Fn_Proc_Audio_400ms
(
    KALDI_AUDIOIN_INTERFACE& audio_interface,
    void * pvoid_Audio_In
)
{
    int i_ret = 0;
    void * pvoid_Buffer = NULL;
    size_t  i_Size = 0;
    size_t  i_Count = 0;
    bool b_Has_Data = false;

    do {

        i_ret = audio_interface.pfhasdata(pvoid_Audio_In, &b_Has_Data);
        if (!b_Has_Data) {
            continue;
        }

        i_ret = audio_interface.pffetchdata(pvoid_Audio_In, &pvoid_Buffer, &i_Size);
        i_ret = audio_interface.pfreturndata(pvoid_Audio_In, pvoid_Buffer);

        ++i_Count;
        if (10 == i_Count) {
            break;
        }

    } while (0);

    return E_Result_Succeeded;
}


} // name N_Kaldi

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
