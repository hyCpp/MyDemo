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
#include "Vr_Asr_Impl_Vocon_Thread_Rec.h"
#include "Vr_Asr_Impl_Vocon_Mgr_Rec.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Thread_Rec::~C_Thread_Rec()
{
}

C_Thread_Rec::C_Thread_Rec
(
    C_Mgr_Rec & ro_Mgr_Rec
)
: m_ro_Mgr_Rec(ro_Mgr_Rec)
{
}

void
C_Thread_Rec::Run()
{
    while (true) {

        Fn_Log("C_Thread_Rec Wait");

        Wait();

        Fn_Log("C_Thread_Rec Notified");

        IF (CheckQuit()) {
            break;
        }

        m_ro_Mgr_Rec.M_Fn_On_Thread_Notified();
    }
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */