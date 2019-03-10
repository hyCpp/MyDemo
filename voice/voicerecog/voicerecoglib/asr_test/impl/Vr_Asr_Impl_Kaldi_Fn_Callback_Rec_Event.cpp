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
#include "Vr_Asr_Impl_Kaldi_Fn_Callback_Rec_Event.h"
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

extern "C"
int
Fn_Callback_Rec_Event
(
    void * pvoid_Inst,
    unsigned long i_Type
)
{
    C_Engine_Concrete * po_Engine_Concrete = reinterpret_cast<C_Engine_Concrete *>(pvoid_Inst);
    IF (!po_Engine_Concrete) {
        return 1;
    }

    IF (!po_Engine_Concrete->m_scpo_Mgr_Rec) {
        return 1;
    }

    E_Result e_Result = po_Engine_Concrete->m_scpo_Mgr_Rec->M_Fn_On_Callback_Event(i_Type);
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return 1;
    }

    return 0;
}

} // name N_Kaldi

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
