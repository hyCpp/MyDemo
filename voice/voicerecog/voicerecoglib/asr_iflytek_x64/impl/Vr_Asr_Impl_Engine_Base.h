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
 * @file Voice_Asr_Impl_Engine_Base.h
 * @brief Asr class define
 *
 *
 * @attention used for C++ only.
 */

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __Vr_Asr_Impl_Engine_Base_h__
#define __Vr_Asr_Impl_Engine_Base_h__

namespace N_Vr
{

namespace N_Asr
{

namespace N_Impl
{

/**
 * @brief The C_Engine_Base class
 *
 * C_Engine_Base class
 */
class C_Engine_Base
{
  public:

    virtual
    ~C_Engine_Base(); // [F]Destructor

    explicit
    C_Engine_Base(); // [F]Constructor of Default

    virtual
    E_Result
    M_Fn_Init();

    virtual
    E_Result
    M_Fn_Fina();

    virtual
    E_Result
    M_Fn_Party_Load
    (
        string const & rstring_Id_Party
    );

    virtual
    E_Result
    M_Fn_Party_Unload
    (
        string const & rstring_Id_Party
    );

    virtual
    E_Result
    M_Fn_Param_Set
    (
        C_Request_Param_Set & ro_Request_Param_Set
    );

    virtual
    E_Result
    M_Fn_Data_Context_List_Update
    (
        C_Request_Context_List_Update & ro_Request_Context_List_Update
    );
    
#if 0
    virtual
    E_Result
    M_Fn_Data_Context_Fuzzy_Compile
    (
        C_Request_Context_Fuzzy_Compile & ro_Request_Context_Fuzzy_Compile
    );
#endif

    virtual
    E_Result
    M_Fn_Rec_Start
    (
        C_Request_Recognize & ro_Request_Recognize
    );

    virtual
    E_Result
    M_Fn_Rec_Cancel();

    virtual
    E_Result
    M_Fn_Get_Transcriptions
    (
        C_Term& ro_Term
    );

  PROTECTED:

    C_Engine_Base // [F]Constructor: Copy
    (
        C_Engine_Base const & // [I]
    );

    C_Engine_Base & // [R]
    operator = // [F]Operator: =
    (
        C_Engine_Base const & // [I]
    );
};

} // name N_Impl

} // name N_Asr

} // name N_Vr

#endif
/* EOF */
