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
#include "Win_Linux_Interface.h"
#include "Vr_Asr_Impl_IFlytek_IFlytek_Mock.h"
#include "Vr_Asr_Impl_IFlytek_Callback_Mock.h"
#include "Vr_Asr_Impl_IFlytek_Thread_Callback_Mock.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

C_IFlyTek_Callback_Mock::~C_IFlyTek_Callback_Mock()
{
    MACRO_Log_Function
}

C_IFlyTek_Callback_Mock::C_IFlyTek_Callback_Mock
(
    C_IFlyTek_Mock & ro_IFlyTek_Mock
)
    : m_ro_IFlyTek_Mock(ro_IFlyTek_Mock)
    , m_i_callback_type(0)
    , m_scpo_IFlytek_Thread_Callback_Mock()
{
    MACRO_Log_Function
}

int
C_IFlyTek_Callback_Mock::M_Fn_Init()
{
    MACRO_Log_Function

    int i_reture_value = 0;

    i_reture_value = _M_Fn_Thread_Init();
    if (i_reture_value != 0) {
        MACRO_Log_Error
        return i_reture_value;
    }

    return 0;
}

int
C_IFlyTek_Callback_Mock::M_Fn_Fina()
{
    MACRO_Log_Function

    int i_reture_value;

    i_reture_value = _M_Fn_Thread_Fina();
    if (i_reture_value != 0) {
        MACRO_Log_Error
        return i_reture_value;
    }

    return 0;
}

void
C_IFlyTek_Callback_Mock::M_Fn_Callback_Msg(int i_callback_type)
{
    MACRO_Log_Function

    m_i_callback_type = i_callback_type;
    
    printf("[Xunfei]C_IFlyTek_Callback_Mock::M_Fn_Callback_Msg;\n");
    if (m_scpo_IFlytek_Thread_Callback_Mock) {  
        m_scpo_IFlytek_Thread_Callback_Mock->Notify();
    }

    return;
}

void
C_IFlyTek_Callback_Mock::M_Fn_On_Thread_Notified()
{
    MACRO_Log_Function

    C_Lock_Mutex o_Lock_Mutex(m_ro_IFlyTek_Mock.m_o_Mutex_IFlyTek_Mock);

    Sleep_WinLinux(10);

    if (0 == m_i_callback_type) {
        m_ro_IFlyTek_Mock.M_Fn_Callback_Msg(0);
    }
    else if (1 == m_i_callback_type) {
        m_ro_IFlyTek_Mock.M_Fn_Callback_Msg(1);
    }
    else if (2 == m_i_callback_type) {
        m_ro_IFlyTek_Mock.M_Fn_Callback_Msg(2);
    }
    else {  //  m_i_callback_type = 3:  cancel

    }

    return;
}

int
C_IFlyTek_Callback_Mock::_M_Fn_Thread_Init()
{
    MACRO_Log_Function

    if (m_scpo_IFlytek_Thread_Callback_Mock) {
        MACRO_Log_Error
        return E_Result_Error;
    }

    C_IFlytek_Thread_Callback_Mock* p_IFlytek_Thread_Callback_Mock = MACRO_New C_IFlytek_Thread_Callback_Mock(*this);
    if (NULL == p_IFlytek_Thread_Callback_Mock) {
        MACRO_Log_Error
        return E_Result_Error;
    }
    m_scpo_IFlytek_Thread_Callback_Mock.reset(p_IFlytek_Thread_Callback_Mock);

#if defined(_WIN32) || defined(_WIN32_WCE)
#else
    m_scpo_IFlytek_Thread_Callback_Mock->RegisterName("VR_ASR_REC_THREAD");
#endif

    m_scpo_IFlytek_Thread_Callback_Mock->StartRegistThread();

    Sleep_WinLinux(10);
    
    return 0;
}

int
C_IFlyTek_Callback_Mock::_M_Fn_Thread_Fina()
{
    MACRO_Log_Function

    if (m_scpo_IFlytek_Thread_Callback_Mock) {
        m_scpo_IFlytek_Thread_Callback_Mock->StopThread();
        printf("[Xunfei]m_scpo_IFlytek_Thread_Callback_Mock->StopThread();\n");
        m_scpo_IFlytek_Thread_Callback_Mock.reset();
        printf("[Xunfei]m_scpo_IFlytek_Thread_Callback_Mock.reset();\n");
    }

    return 0;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
