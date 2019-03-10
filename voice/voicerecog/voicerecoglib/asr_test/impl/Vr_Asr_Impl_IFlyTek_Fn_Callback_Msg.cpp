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
#include "Vr_Asr_Impl_IFlyTek_Fn_Callback_Msg.h"
#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

void
Fn_Callback_Msg
(
    const void* pv_UsrArg,
    unsigned int i_Msg,
    unsigned int i_wParam,
    const void* pv_lParam

)
{
    void* pvoid_Inst = const_cast<void *>(pv_UsrArg);
    C_Engine_Concrete * po_Engine_Concrete = reinterpret_cast<C_Engine_Concrete *>(pvoid_Inst);
    IF (!po_Engine_Concrete) {
        MACRO_Log_Error
        return;
    }

    E_Result e_Result = po_Engine_Concrete->M_Fn_On_Callback_Msg(i_Msg, i_wParam, pv_lParam);
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
    }

    return;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
