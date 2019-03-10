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
#include "Vr_Asr_Impl_IFlyTek_Thread_Data.h"
#include "Vr_Asr_Impl_IFlyTek_Mgr_Data.h"
#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

C_Mgr_Data::~C_Mgr_Data()
{
    MACRO_Log_Function
}

C_Mgr_Data::C_Mgr_Data
(
    C_Engine_Concrete & ro_Engine_Concrete
)
: m_ro_Engine_Concrete(ro_Engine_Concrete)
, m_b_Audio_Close(true)
, m_spo_Audio_In()
, m_function_On_Event_Phase()
, m_scpo_Thread_Data()
, m_o_Mutex_Audio_In_Request()
{
    MACRO_Log_Function
}

E_Result
C_Mgr_Data::M_Fn_Init()
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
C_Mgr_Data::M_Fn_Fina()
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

E_Result
C_Mgr_Data::M_Fn_Start_Audio()
{
    MACRO_Log_Function

    m_spo_Audio_In = m_ro_Engine_Concrete.m_scpo_Request_Recognize->m_spo_Audio_In;
    m_function_On_Event_Phase = m_ro_Engine_Concrete.m_scpo_Request_Recognize->m_function_On_Event_Phase;

    IF (!m_spo_Audio_In) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    
    m_b_Audio_Close = false;
   
    m_scpo_Thread_Data->Notify();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::M_Fn_Stop_Audio()
{
    MACRO_Log_Function

    m_b_Audio_Close = true;

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Audio_In_Request);

    return E_Result_Succeeded;
}

void
C_Mgr_Data::M_Fn_On_Thread_Notified()
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_o_Mutex_Audio_In_Request);

    E_Result e_Result;
    
    int i_Audio_Ret = 0;
    
    size_t i_Buffer_Size = 0;
    void * p_Void_Buffer = NULL;

    m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Listen_Begin)
    );

    i_Audio_Ret = m_spo_Audio_In->M_Fn_Start();
    IF (1 == i_Audio_Ret) {
        MACRO_Log_Error
        return;
    }
    Fn_Log("start audio"); 

    while (!m_b_Audio_Close) {

        i_Audio_Ret = m_spo_Audio_In->M_Fn_Data_Fetch(
            &p_Void_Buffer,
            &i_Buffer_Size);
        IF (1 == i_Audio_Ret) {
            MACRO_Log_Error
            break;
        }

        IF (0 == i_Buffer_Size) {
            continue;
        }

        e_Result = _M_Fn_Append_Audio_Data(p_Void_Buffer, i_Buffer_Size);
        IF (E_Result_Succeeded != e_Result) {
            break;
        }

        i_Audio_Ret = m_spo_Audio_In->M_Fn_Data_Return(p_Void_Buffer);
        IF (1 == i_Audio_Ret) {
            MACRO_Log_Error
            break;
        }    
    }

    // m_ro_Engine_Concrete.M_Fn_End_Append_Audio_Data();

    i_Audio_Ret = m_spo_Audio_In->M_Fn_Stop();
    IF (1 == i_Audio_Ret) {
        MACRO_Log_Error
        return;
    }

    m_function_On_Event_Phase(
        C_Event_Phase_Impl(C_Event_Phase::E_Event_Phase_Type_Listen_End)
    );

    return;
}

E_Result
C_Mgr_Data::_M_Fn_Thread_Init()
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Thread_Data) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    
    m_scpo_Thread_Data.reset(MACRO_New C_Thread_Data(*this));

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    m_scpo_Thread_Data->RegisterName("VR_ASR_DATA_THREAD");
#endif

    m_scpo_Thread_Data->StartRegistThread();

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::_M_Fn_Thread_Fina()
{
    MACRO_Log_Function

    IF ((bool)m_scpo_Thread_Data) {
        m_scpo_Thread_Data->StopThread();
        m_scpo_Thread_Data.reset();
    }

    return E_Result_Succeeded;
}

E_Result
C_Mgr_Data::_M_Fn_Append_Audio_Data
(
    void* p_Buffer,
    unsigned int i_Buffer_Size
)
{
    E_Result e_Result;

    char* p_i_Data_Buffer = NULL;
    unsigned int i_Write_Unit = 160;   // 160 frames = 320 bytes
    unsigned int i_Writed_Frames = 0;
    unsigned int i_Writed_Size = 0;

    IF (i_Buffer_Size < 320) {
        i_Write_Unit = i_Buffer_Size >> 1;
    }

    p_i_Data_Buffer = static_cast<char*>(p_Buffer);
    while (!m_b_Audio_Close) {
        e_Result = m_ro_Engine_Concrete.M_Fn_Append_Audio_Data(
            reinterpret_cast<short*>(p_i_Data_Buffer + i_Writed_Size), 
            i_Write_Unit,
            i_Writed_Frames);
        IF (E_Result_Succeeded != e_Result) {
            return E_Result_Error;
        }

        i_Writed_Size += (i_Writed_Frames << 1);
        IF (i_Writed_Size == i_Buffer_Size) {
            break;
        }

        IF (i_Writed_Frames != i_Write_Unit) {
            usleep(10000);
        }

        i_Write_Unit = (i_Buffer_Size - i_Writed_Size) >> 1;
        IF (i_Write_Unit  > 160) {
            i_Write_Unit = 160;
        }   
    }
        

    return E_Result_Succeeded;
}


} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
