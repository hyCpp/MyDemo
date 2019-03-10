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
#include "Vr_Asr_Impl_Engine_Base.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

C_Engine_Base::~C_Engine_Base()
{
}

C_Engine_Base::C_Engine_Base()
{
}

E_Result
C_Engine_Base::M_Fn_Init()
{
    return E_Result_Succeeded;
}

E_Result
C_Engine_Base::M_Fn_Fina()
{
    return E_Result_Succeeded;
}

E_Result
C_Engine_Base::M_Fn_Party_Load
(
    string const & rstring_Id_Party
)
{
    return E_Result_Succeeded;
}

E_Result
C_Engine_Base::M_Fn_Party_Unload
(
    string const & rstring_Id_Party
)
{
    return E_Result_Succeeded;
}

E_Result
C_Engine_Base::M_Fn_Param_Set
(
    C_Request_Param_Set & ro_Request_Param_Set
)
{
    return E_Result_Succeeded;
}

E_Result
C_Engine_Base::M_Fn_Data_Context_List_Update
(
    C_Request_Context_List_Update & ro_Request_Context_List_Update
)
{
    return E_Result_Succeeded;
}

#if 0

E_Result
C_Engine_Base::M_Fn_Data_Context_Fuzzy_Compile
(
    C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile
)
{
    return E_Result_Succeeded;
}

#endif

E_Result
C_Engine_Base::M_Fn_Rec_Start
(
    C_Request_Recognize & ro_Request_Recognize
)
{
    return E_Result_Succeeded;
}

E_Result
C_Engine_Base::M_Fn_Rec_Cancel()
{
    return E_Result_Succeeded;
}

E_Result
C_Engine_Base::M_Fn_Get_Transcriptions
(
    C_Term& ro_Term
)
{
    return E_Result_Error;
}

} // name N_Impl

} // name N_Asr

} // name N_Vr

/* EOF */
