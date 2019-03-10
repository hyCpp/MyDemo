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
#include "Vr_Asr_Impl_IFlyTek_Fn_Callback_Grammar_Build.h"
#include "Vr_Asr_Impl_IFlyTek_Engine_Concrete.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_IFlyTek
{

int
Fn_Callback_Grammar_Build
(
    int i_Ecode,
    const char* p_Info,
    void* pv_User_Data

)
{
    C_Engine_Concrete * po_Engine_Concrete = reinterpret_cast<C_Engine_Concrete *>(pv_User_Data);
    IF (!po_Engine_Concrete) {
        MACRO_Log_Error
        return -1;
    }

    E_Result e_Result = po_Engine_Concrete->M_Fn_On_Callback_Grammar_Build(i_Ecode, p_Info);
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return -1;
    }

    return 0;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
