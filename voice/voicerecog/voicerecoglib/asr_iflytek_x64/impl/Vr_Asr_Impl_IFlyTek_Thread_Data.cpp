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
#include "Vr_Asr_Impl_IFlyTek_Thread_Data.h"
#include "Vr_Asr_Impl_IFlyTek_Mgr_Data.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

C_Thread_Data::~C_Thread_Data()
{
}

C_Thread_Data::C_Thread_Data
(
    C_Mgr_Data & ro_Mgr_Data
)
: m_ro_Mgr_Data(ro_Mgr_Data)
{
}

void
C_Thread_Data::Run()
{
    while (true) {

        Fn_Log("C_Thread_Data Wait");

        Wait();

        Fn_Log("C_Thread_Data Notified");

        if (CheckQuit()) {
            break;
        }

        m_ro_Mgr_Data.M_Fn_On_Thread_Notified();
    }
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */