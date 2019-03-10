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
#include "Vr_Asr_Impl_Vocon_Data_Context_Userword.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

namespace N_Vocon
{

C_Data_Context_Userword::~C_Data_Context_Userword()
{
}

C_Data_Context_Userword::C_Data_Context_Userword
(
    C_Data_Party & ro_Data_Party,
    string const & rstring_Id_Context,
    string const & rstring_Type_Context,
    string const & rstring_Data_File_Context,
    list<pair<string, string> > & rlist_pair_string_Id_Slot_string_Id_Context
)
: C_Data_Context(ro_Data_Party, rstring_Id_Context, rstring_Type_Context, rstring_Data_File_Context, rlist_pair_string_Id_Slot_string_Id_Context)
{
}

E_Result
C_Data_Context_Userword::M_Fn_Init()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = _M_Fn_Obj_Create_Userword();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
        return e_Result;
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context_Userword::M_Fn_Fina()
{
    MACRO_Log_Function

    E_Result e_Result;

    e_Result = _M_Fn_Obj_Destroy();
    IF (e_Result != E_Result_Succeeded) {
        MACRO_Log_Error
    }

    return E_Result_Succeeded;
}

E_Result
C_Data_Context_Userword::M_Fn_Load
(
    list<shared_ptr<C_Request_Activate> > & rlist_spo_Request_Activate
)
{
    MACRO_Log_Function

    return E_Result_Succeeded;
}

E_Result
C_Data_Context_Userword::M_Fn_Unload()
{
    MACRO_Log_Function

    return E_Result_Succeeded;
}

} // name N_Vocon

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */