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
#include "Vr_Asr_Impl_Vocon_Fn_Callback_FuzzyMatchCtxCpl_Error.h"
#include "Vr_Asr_Impl_Vocon_Data_Context.h"


namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

#if 0

extern "C"
void
Fn_Callback_FuzzyMatchCtxCpl_Error
(
    void *pvoid_CallBack_Inst, 
    const char *p_i_Message
)
{
    C_Data_Context * po_Data_Context = reinterpret_cast<C_Data_Context *>(pvoid_CallBack_Inst);
    IF (NULL == po_Data_Context) {
        MACRO_Log_Error
        return;
    }

    E_Result e_Result = po_Data_Context->M_Fn_On_Callback_Fuzzy_Compile_Error(string(p_i_Message));
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return;
    }

    return;
}

#endif

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
