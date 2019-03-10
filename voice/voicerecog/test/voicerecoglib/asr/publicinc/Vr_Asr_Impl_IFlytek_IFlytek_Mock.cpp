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
#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#else
#include "Vr_Asr_Impl_Afx.h"
#endif

#include "Vr_Asr_Impl_IFlytek_IFlytek_Mock.h"
#include "Vr_Asr_Impl_IFlytek_Callback_Mock.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

C_IFlyTek_Mock::~C_IFlyTek_Mock()
{
    M_Fn_Fina();
}

C_IFlyTek_Mock::C_IFlyTek_Mock()
    : p_CallbakcMsgProc(NULL)
    , p_UserData(NULL)
    , m_o_Mutex_IFlyTek_Mock()
{
    vector_b_Param.resize(E_Param_Size, 0);  
}

int
C_IFlyTek_Mock::M_Fn_Init()
{
    C_IFlyTek_Callback_Mock* p_IFlyTek_Callback_Mock = MACRO_New C_IFlyTek_Callback_Mock(*this);
    if (NULL == p_IFlyTek_Callback_Mock) {
        return -1;
    }

    m_scpo_IFlyTek_Callback_Mock.reset(p_IFlyTek_Callback_Mock);

    return m_scpo_IFlyTek_Callback_Mock->M_Fn_Init();
}

void
C_IFlyTek_Mock::M_Fn_Fina()
{
    if (m_scpo_IFlyTek_Callback_Mock) {
        m_scpo_IFlyTek_Callback_Mock->M_Fn_Fina();
    }
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
