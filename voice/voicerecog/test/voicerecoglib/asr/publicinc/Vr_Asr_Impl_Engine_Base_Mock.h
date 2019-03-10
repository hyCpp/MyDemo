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

/**
 * @file Vr_Asr_Impl_Engine_Base_Mock.h
 * @brief Declaration file of class C_Engine_Base_Mock.
 *
 * This file includes the declaration of class C_Engine_Base_Mock.
 *
 * @attention used for C++ only.
 */

#ifndef _Vr_Asr_Impl_Engine_Base_Mock_h_
#define _Vr_Asr_Impl_Engine_Base_Mock_h_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "Vr_Asr_Impl_Engine_Base.h"

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Engine_Base_Mock class
 *
 * C_Engine_Base_Mock class
 */

class C_Engine_Base_Mock : public C_Engine_Base
{
public:
    MOCK_METHOD0(M_Fn_Init, E_Result());
    MOCK_METHOD0(M_Fn_Fina, E_Result());
    MOCK_METHOD1(M_Fn_Party_Load, E_Result(string const & rstring_Id_Party));
    MOCK_METHOD1(M_Fn_Party_Unload, E_Result(string const & rstring_Id_Party));
    MOCK_METHOD1(M_Fn_Param_Set, E_Result(C_Request_Param_Set & ro_Request_Param_Set));
    MOCK_METHOD1(M_Fn_Data_Context_List_Update, E_Result(C_Request_Context_List_Update & ro_Request_Context_List_Update));
    MOCK_METHOD1(M_Fn_Rec_Start, E_Result(C_Request_Recognize & ro_Request_Recognize));
    MOCK_METHOD0(M_Fn_Rec_Cancel, E_Result());
};


} // namespace N_Impl

} // namespace N_Asr

} // namespace N_Vr

#endif // #ifndef _Vr_Asr_Impl_Engine_Base_Mock_h_
/* EOF */