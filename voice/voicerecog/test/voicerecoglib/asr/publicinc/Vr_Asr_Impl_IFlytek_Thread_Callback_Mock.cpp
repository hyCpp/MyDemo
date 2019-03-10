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
#include "Vr_Asr_Impl_IFlytek_Thread_Callback_Mock.h"
#include "Vr_Asr_Impl_IFlytek_Callback_Mock.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

C_IFlytek_Thread_Callback_Mock::~C_IFlytek_Thread_Callback_Mock()
{
}

C_IFlytek_Thread_Callback_Mock::C_IFlytek_Thread_Callback_Mock
(
    C_IFlyTek_Callback_Mock & ro_IFlyTek_Callback_Mock
)
    : m_ro_IFlyTek_Callback_Mock(ro_IFlyTek_Callback_Mock)
{
}

void
C_IFlytek_Thread_Callback_Mock::Run()
{
    while (true) {

        Fn_Log("C_IFlytek_Thread_Callback_Mock Wait");

        Wait();

        Fn_Log("C_IFlytek_Thread_Callback_Mock Notified");

        if (CheckQuit()) {
            break;
        }

        m_ro_IFlyTek_Callback_Mock.M_Fn_On_Thread_Notified();
    }
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */